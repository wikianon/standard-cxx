#include <iostream>
#include <PdhMsg.h>
#include <windows.h>
#include <WinNls.h>
#include "mdr.h"
#include "mdr_single.h"
#include "mdr_tuple.h"

namespace mdr {

mdr_host_name_t::mdr_host_name_t()
{
  WORD v;
  WSADATA wsa;
  char hn[128];

  v = MAKEWORD(2, 2);
  if (WSAStartup(v, &wsa) != 0)
    throw mdr_exception_t(
      "mdr_t::mdr_t: WSAStartup failed");
  if (gethostname(hn, sizeof hn) != 0)
    throw mdr_exception_t("mdr_t::mdr_t: gethostname failed");

  this->assign(hn);
}

mdr_host_name_t mdr_t::mdr_local_host;

mdr_t::mdr_t(const char *name, bool singleton, const char *reg_host)
{
  mdr_remote_host = (reg_host ? reg_host : mdr_local_host.c_str());
  mdr_category_name = (name != 0) ? name : "TUPLE INSTANCE";
  mdr_is_singleton = singleton;
  if (name)
    get_category_explanation();
}

const std::string &mdr_t::mdr_field_name(int field_number)
{
  mdr_names_node_t n = mdr_field_names.find(field_number);
  static std::string not_found("FIELD NOT FOUND");

  if (n == mdr_field_names.end())
    return not_found;

  return n->second;
}

int mdr_t::mdr_field_name_to_number(const char *name)
{
  mdr_names_node_t n;

  // oogie<>linear search...
  for(n=mdr_field_names.begin(); n != mdr_field_names.end(); n++) {
    const std::string *match = &n->second;

    mdr_babel_node_t node = win_to_utf8.find(n->second);
    if (node != win_to_utf8.end())
      match = &node->second;
    if (*match == name)
      return n->first;
  }

  return -1;
}

mdr_keyset_t &mdr_t::mdr_all_field_names(mdr_keyset_t &name_set)
{
  mdr_names_node_t n;
  int i;

  name_set.clear();
  name_set.resize(mdr_field_names.size());

  for(i=0, n=mdr_field_names.begin(); n != mdr_field_names.end(); n++, i++)
    name_set[i] = n->second;

  return name_set;
}

void mdr_t::mdr_get_reg_instance_names(
  mdr_keyset_t &name_set, const char *category_name, const char *reg_host)
{
  PDH_STATUS status;
  LPTSTR object_list = 0;
  DWORD object_length = 0;
  LPTSTR counter_list = 0;
  DWORD counter_length = 0;
  LPTSTR instance_list = 0;
  DWORD instance_length = 0;
  const char *inst;
  int i;

  name_set.clear();

  // get the sizes
  if (reg_host == 0)
    reg_host = mdr_local_host.c_str();

  status = PdhEnumObjectItems(
    0, reg_host, category_name, counter_list, &counter_length,
      instance_list, &instance_length, PERF_DETAIL_WIZARD, 0);
  if (status != ERROR_SUCCESS && status != PDH_MORE_DATA)
    throw mdr_exception_t(
      "mdr_t::mdr_get_reg_instance_names: "
        "PdhEnumObjectItems(%s:size) failure: %s",
          category_name,
          mdr_exception_t::PdhErrorMessage(status));

  // category with no instances; no dice
  if (instance_length <= 2)
    return;

  counter_list = new char[counter_length];
  instance_list = new char[instance_length];

  // get the data
  status = PdhEnumObjectItems(
    0, reg_host, category_name, counter_list, &counter_length,
      instance_list, &instance_length, PERF_DETAIL_WIZARD, 0);
  if (status != ERROR_SUCCESS)
  {
    delete[] counter_list;
    delete[] instance_list;
    throw mdr_exception_t(
      "mdr_t::mdr_get_reg_instance_names: "
        "PdhEnumObjectItems(%s:data) failure: %s",
          category_name, mdr_exception_t::PdhErrorMessage(status));
  }

  for(i=0, inst=instance_list; *inst; inst += strlen(inst) + 1, i++)
    ;
  name_set.resize(i);

  for(i=0, inst=instance_list; *inst; inst += strlen(inst) + 1, i++)
    name_set[i] = inst;

  delete[] counter_list;
  delete[] instance_list;
}

mdr_t *mdr_t::mdr_get_mdr_object(const char *category_name, const char *reg_host)
{
  PDH_STATUS status;
  LPTSTR object_list = 0;
  DWORD object_length = 0;
  LPTSTR counter_list = 0;
  DWORD counter_length = 0;
  LPTSTR instance_list = 0;
  DWORD instance_length = 0;
  mdr_t *return_value;

  // get the sizes
  if (reg_host == 0)
    reg_host = mdr_local_host.c_str();

  mdr_babel_node_t node = utf8_to_win.find(category_name);
  if (node != utf8_to_win.end())
    category_name = node->second.c_str();

  status = PdhEnumObjectItems(
    0, reg_host, category_name, counter_list, &counter_length,
      instance_list, &instance_length, PERF_DETAIL_WIZARD, 0);
  if (status != ERROR_SUCCESS && status != PDH_MORE_DATA)
    throw mdr_exception_t(
      "mdr_t::mdr_get_mdr_object: "
        "PdhEnumObjectItems(%s:size) failure: %s",
          category_name,
          mdr_exception_t::PdhErrorMessage(status));

  // category with no instances; no dice
  if (instance_length == 2)
    return 0;

  counter_list = new char[counter_length];
  if (instance_length)
    instance_list = new char[instance_length];

  // get the data
  status = PdhEnumObjectItems(
    0, reg_host, category_name, counter_list, &counter_length,
      instance_list, &instance_length, PERF_DETAIL_WIZARD, 0);
  if (status != ERROR_SUCCESS)
  {
    delete[] counter_list;
    if (instance_list)
      delete[] instance_list;
    throw mdr_exception_t(
      "mdr_t::mdr_get_mdr_object: "
        "PdhEnumObjectItems(%s:data) failure: %s",
          category_name, mdr_exception_t::PdhErrorMessage(status));
  }

  if (instance_list)
  {
    // it's an n-tuple
    mdr_tuple_t *tuple =
    new mdr_tuple_t(category_name, instance_list, counter_list, reg_host);

    return_value = tuple;
  }
  else
  {
    // it's a singleton
    mdr_single_t *single =
    new mdr_single_t(category_name, counter_list, reg_host);

    return_value = single;
  }

  delete[] counter_list;
  if (instance_list)
    delete[] instance_list;

  return return_value;
}

const std::string &mdr_t::get_explain_text(
  PDH_HCOUNTER counter_handle, std::string &explanation)
{
  PDH_STATUS status;
  DWORD size = 0;
  PPDH_COUNTER_INFO pci;

  // ask Pdh how much buffer I need
  status = PdhGetCounterInfo(counter_handle, true, &size, 0);
  if (status != PDH_MORE_DATA && status != ERROR_SUCCESS)
    throw mdr_exception_t(
      "mdr_t::get_explain_text: PdhGetCounterInfo::size() fails: %s",
        mdr_exception_t::PdhErrorMessage(status));

  // allocate that much buffer
  char *buf = new char[size];

  // go get the data
  status = PdhGetCounterInfo(counter_handle, true,
    &size, (PPDH_COUNTER_INFO) buf);
  if (status != ERROR_SUCCESS)
    throw mdr_exception_t(
      "mdr_t::get_explain_text: PdhGetCounterInfo::data() fails: %s",
        mdr_exception_t::PdhErrorMessage(status));

  // here's what's returned
  pci = (PPDH_COUNTER_INFO) buf;

  explanation = pci->szExplainText;

  delete[] buf;

  return explanation;
}

std::ostream &operator<<(std::ostream &out, const mdr_value_t &v)
{
  switch(v.mv_type) {
  case MVT_I32:
    out << v.mv_i32;
    break;
  case MVT_UI32:
    out << v.mv_ui32;
    break;
  case MVT_I64:
    out << v.mv_i64;
    break;
  case MVT_UI64:
    out << v.mv_ui64;
    break;
  case MVT_LONG:
    out << v.mv_l;
    break;
  case MVT_ULONG:
    out << v.mv_ul;
    break;
  case MVT_DOUBLE:
    out << v.mv_d;
    break;
  case MVT_STRING:
    if (v.mv_s == 0)
      out << "null";
    else
      out << *v.mv_s;
    break;
  case MVT_BOOL:
    out << (v.mv_b ? "true" : "false");
    break;
  }

  return out;
}

void mdr_t::get_category_explanation(void)
{
  DWORD type;
  LPBYTE counters = 0;
  DWORD status;
  DWORD size = 0;
  const DWORD increment = 4096;
  LPTSTR counter_list = 0;
  DWORD counter_length = 0;
  LPTSTR instance_list = 0;
  DWORD instance_length = 0;
  LPTSTR object_list = 0;
  DWORD object_length = 0;
  const char *remote_host = mdr_remote_host.c_str();
  int category_index = 0;

  // The Counters registry object looks like this:
  // 1 1847 2 System 4 Memory 6 % Processor Time (etc.)
  // the Help registry object looks like this:
  // 3 The System Performance Object blah
  // 5 The Memory Performance Object blah
  // 7 % Processor Time is blah
  // So, one can posit that the structure of the
  // combined objects is
  // INDEX OBJECT-NAME INDEX OBJECT-DESCRIPTION
  // where every time INDEX appears, it is incremented by one.
  // Therefore, the index of the help that I'm looking for
  // is the index of the object name plus one

  // grapple the Counters from the registry
  do {
    if (counters != 0)
      delete[] counters;
    size += increment;
    counters = new BYTE[size];
    status = RegQueryValueEx(
      HKEY_PERFORMANCE_DATA,
      "Counters",
      NULL,
      &type,
      counters,
      &size);
  } while(status == ERROR_MORE_DATA);

  const char *cp;
  bool index = true;
  DWORD index_number;
  char lbuf[BUFSIZ];
  DWORD lbufsz;

  // squirrel away index to category name
  for (cp = (const char *) counters; *cp; cp += strlen(cp) + 1) {
    if (index) {
      index_number = (DWORD) atoi(cp);
      index = false;
      continue;
    } else
      index = true;

    lbufsz = sizeof(lbuf);
    PdhLookupPerfNameByIndex(0, index_number, lbuf, &lbufsz);

    if (mdr_category_name != lbuf)
      continue;

    category_index = ++index_number;
    break;
  }

  delete[] counters;
  counters = 0;
  size = 0;

  if (category_index == 0)
    throw mdr_exception_t(
      "mdr_t::get_explanation_text: cannot find category: %s",
        mdr_category_name.c_str());

  // grapple the Help from the registry
  do {
    if (counters != 0)
      delete[] counters;
    size += increment;
    counters = new BYTE[size];
    status = RegQueryValueEx(
      HKEY_PERFORMANCE_NLSTEXT,
      "Help",
      NULL,
      &type,
      counters,
      &size);
  } while(status == ERROR_MORE_DATA);

  // discover the help text of the saved category_index
  for (cp = (const char *) counters; *cp; cp += strlen(cp) + 1) {
    if (index) {
      index_number = (DWORD) atoi(cp);
      index = false;
      continue;
    } else
      index = true;

    if (index_number != category_index)
      continue;

    mdr_explanation_text = cp;
    break;
  }

  delete[] counters;
}

}
