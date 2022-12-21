#include "mdr_perf_counter.h"

namespace mdr {

mdr_performance_counter_t::mdr_performance_counter_t(
    const char *category,
    const char *field,
    const char *instance,
    const char *remote_host)
{
  mdr_obj = new mdr_category_t(category, remote_host);

  if (instance) {
    instance_name = instance;
    if (mdr_obj->mdr_is_singleton)
      throw new mdr_exception_t(
        "mdr_performance_counter_t::mdr_performance_counter_t: singleton object does not require an instance");
  } else
    if (!mdr_obj->mdr_is_singleton)
      throw new mdr_exception_t(
        "mdr_performance_counter_t::mdr_performance_counter_t: non-singleton object requires an instance");

  field_number = mdr_obj->mdr_field_name_to_number(field);
  if (field_number == -1)
    throw new mdr_exception_t(
      "mdr_performance_counter_t::mdr_performance_counter_t: field name %s does not exist", field);
}

mdr_performance_counter_t::~mdr_performance_counter_t()
{
  delete mdr_obj;
}

bool mdr_performance_counter_t::mdr_is_supported(void)
{
  return true;
}

const mdr_value_t &mdr_performance_counter_t::mdr_value(void)
{
  if (mdr_obj->mdr_is_singleton)
    field_value = mdr_obj->mdr_field_value(field_number);
  else
    field_value = mdr_obj->mdr_field_value(instance_name, field_number);

  return field_value;
}

const mdr_value_t &mdr_performance_counter_t::mdr_next_value(void)
{
  mdr_obj->mdr_refresh_rates();

  if (mdr_obj->mdr_is_singleton)
    return mdr_obj->mdr_field_value(field_number);
  else
    return mdr_obj->mdr_field_value(instance_name, field_number);
}

}

