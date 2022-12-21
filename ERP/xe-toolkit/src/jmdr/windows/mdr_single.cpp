#include <time.h>
#include "mdr_single.h"

namespace mdr {

mdr_single_t::mdr_single_t(const char *category, const char *counter_list,
                           const char *reg_host) : mdr_t(category, true, reg_host)
{
  int i;
  const char *cntr;

  mdr_instance_count = 1;
  query = 0;

  // if counter_list is null, then we're just a container
  if (counter_list) {
    for(i=0, cntr=counter_list; *cntr; cntr += strlen(cntr)+1, i++) {
      mdr_field_names[i] = cntr;
      (*this)[i].clear();
      mdr_cnt_list[i] = 0;
    }
    mdr_field_count = i;

    mdr_refresh();
  }
}

mdr_single_t::~mdr_single_t()
{
  if (query)
    delete query;
  int n = (int) size();
  for(int i=0; i<n; i++) {
    mdr_value_t *vp = &(*this)[i];
    if (vp->mv_type == MVT_STRING && vp->mv_s != 0)
      delete vp->mv_s;
  }
}

const mdr_value_t &mdr_single_t::mdr_field_value(int field_number)
{
  if (field_number < 0 || field_number >= mdr_field_count)
    throw mdr_exception_t("mdr_single_t::mdr_field_value: "
      "index %d is out of range for (0..%d)",
        field_number, mdr_field_count - 1);

  return (*this)[field_number];
}

const std::string &mdr_single_t::mdr_field_explanation(
  int field_number, std::string &explanation)
{
  if (field_number < 0 || field_number >= mdr_field_count)
    throw mdr_exception_t("mdr_single_t::mdr_field_explanation: "
      "index %d is out of range for (0..%d)",
        field_number, mdr_field_count - 1);

  const PDH_HCOUNTER &hCounter = mdr_cnt_list[field_number];
  get_explain_text(hCounter, explanation);
  return explanation;
}

void mdr_single_t::mdr_refresh(void)
{
  int i;
  char full_name[BUFSIZ];

  if (query == 0) {
    query = new reg_query_t();
    for(i=0; i<mdr_field_count; i++) {
      _snprintf_s(full_name, sizeof full_name, sizeof full_name, "\\\\%s\\%s\\%s",
        mdr_remote_host.c_str(), mdr_category_name.c_str(),
          mdr_field_names[i].c_str());
      query->add_counter(full_name, mdr_cnt_list[i]);
    }
  }

  query->perform_query();
  for(i=0; i<mdr_field_count; i++)
    query->get_counter_data(mdr_cnt_list[i], (*this)[i]);
}

}
