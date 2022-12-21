#include <set>
#include <time.h>
#include "mdr_tuple.h"

namespace mdr {

typedef std::set<std::string> inst_set_t;
typedef inst_set_t::iterator  iset_node_t;

mdr_tuple_t::mdr_tuple_t(const char *category, const char *instance_list,
                         const char *counter_list, const char *reg_host) :
							mdr_t(category, false, reg_host)
{
  int i;
  const char *cntr;
  const char *inst;
  bool is_process_or_thread =
    (mdr_category_name == "Process" || mdr_category_name == "Thread");

  query = 0;

  for(i=0, cntr=counter_list; *cntr; cntr += strlen(cntr)+1, i++)
    mdr_field_names[i] = cntr;

  mdr_field_count = i;

  // all but Process and Thread have unique names as keys
  if (! is_process_or_thread)
    for(inst=instance_list; *inst; inst += strlen(inst) + 1)
      (*this)[inst].clear();

  mdr_refresh();
}

mdr_tuple_t::~mdr_tuple_t()
{
  if (query)
    delete query;
}

const mdr_value_t &mdr_tuple_t::mdr_field_value(const char *instance, int field_number)
{
  if (field_number < 0 || field_number >= mdr_field_count)
    throw mdr_exception_t("mdr_tuple_t::mdr_field_value: "
      "index %d is out of range for (0..%d)",
        field_number, mdr_field_count - 1);

  tuple_node_t node = find(instance);

  if (node == end())
    throw mdr_exception_t("mdr_tuple_t::mdr_field_value: "
      "cannot find instance %s", instance);

  return node->second[field_number];
}

const std::string &mdr_tuple_t::mdr_field_explanation(
  int field_number, std::string &explanation)
{
  tuple_node_t node = begin();

  if (node == end())
    throw mdr_exception_t("mdr_tuple_t::mdr_field_value: no instances");

  return node->second.mdr_field_explanation(field_number, explanation);
}

const mdr_keyset_t &mdr_tuple_t::mdr_keyset(mdr_keyset_t &name_set)
{
  tuple_node_t n;
  int i;

  name_set.clear();
  name_set.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    name_set[i] = n->first;
  return name_set;
}

void mdr_tuple_t::mdr_refresh(void)
{
  int i;
  mdr_single_t *sp;
  tuple_node_t n;
  char full_name[BUFSIZ];
  const char *category_name = mdr_category_name.c_str();
  const char *reg_host = mdr_remote_host.c_str();
  bool is_process_or_thread =
    (mdr_category_name == "Process" || mdr_category_name == "Thread");
  bool next_instance;

  if (is_process_or_thread) {
    mdr_keyset_t reg_names;
	mdr_get_reg_instance_names(reg_names, category_name, reg_host);
    int sz = reg_names.size();

    // delete all from this
    clear();

    // destroy query if any
    if (query) {
      delete query;
      query = 0;
    }

    // start over
    for(i=0; i<sz; i++) {
      const char *inst = reg_names[i].c_str();
      const char *instance_name;
      char iname[128];

      // make a new singleton and put it in the tuple map
      tuple_node_t n = find(inst);
      if (n != end()) {
        // Process and Thread have duplicates
        mdr_single_t *sp = &n->second;

        _snprintf_s(iname, sizeof iname, sizeof iname, "%s#%d",
          inst, sp->mdr_instance_count++);
        instance_name = iname;
      } else
        instance_name = inst;

      (*this)[instance_name].clear();
    }
  }

  // first pass through; add all counters
  if (query == 0) {
    query = new reg_query_t();
    for(n=begin(); n != end(); n++) {
      sp = &n->second;
      sp->mdr_field_count = mdr_field_count; // while I'm here...
      for(i=0; i<mdr_field_count; i++) {
        _snprintf_s(full_name, sizeof full_name, sizeof full_name, "\\\\%s\\%s(%s)\\%s",
          reg_host, category_name, n->first.c_str(), mdr_field_names[i].c_str());
        query->add_counter(full_name, sp->mdr_cnt_list[i]);
      }
    }
  } else {
    // Only non-process-and-thread code gets here since the check in
    // the "if (is_process_or_thread)" code sets query to zero.

    // This is the land of unique instance names.
    // The fictional existence of proceses and threads is
    // dealt with elsewhere.

    // get the reg instance names
    mdr_keyset_t reg_names;
    mdr_get_reg_instance_names(reg_names, category_name, reg_host);
    int sz = reg_names.size();
    inst_set_t inst_set;
    inst_set_t delete_set;
    iset_node_t dn;

    // for each reg instance name
    for(i=0; i<sz; i++) {
      std::string &inst = reg_names[i];
      inst_set.insert(inst);
      // if name is not in this
      if (find(inst) == end()) {
        // add new instance to this
        (*this)[inst].clear();
        n = find(inst);
        sp = &n->second;
        // perform loop to add counters to query
        for(int j=0; j<mdr_field_count; j++) {
          _snprintf_s(full_name, sizeof full_name, sizeof full_name, "\\\\%s\\%s(%s)\\%s",
            reg_host, category_name, inst.c_str(), mdr_field_names[j].c_str());
          query->add_counter(full_name, sp->mdr_cnt_list[j]);
        }
      }
    }

    // for each instance in this
    for(n=begin(); n != end(); n++) {
      // if instance is not in the list of reg instance names
      if (inst_set.find(n->first) == inst_set.end()) {
        // add node to delete_set
        delete_set.insert(n->first);
        // perform loop to remove counters
        sp = &n->second;
        for(i=0; i<mdr_field_count; i++)
          query->remove_counter(sp->mdr_cnt_list[i]);
      }
    }

    // for each instance in delete_set
    for(dn=delete_set.begin(); dn != delete_set.end(); dn++)
      // delete instance from this
      erase(*dn);
  }

  query->perform_query();

  for(n=begin(); n != end(); n++) {
    sp = &n->second;
    next_instance = false;
    for(i=0; i<mdr_field_count; i++) {
      try {
        query->get_counter_data(sp->mdr_cnt_list[i], (*sp)[i]);
      } catch(pdh_exception_t &) {
        // Process/thread likely went away; move on.
        next_instance = true;
      }
      if (next_instance)
        break;
    }
  }
}

}
