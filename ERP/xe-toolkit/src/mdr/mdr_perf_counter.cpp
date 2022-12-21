/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the mdr library of Captivity.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE-GPL.txt contained within the
** same package as this file. This software is subject to a
** dual-licensing mechanism, the details of which are outlined in
** file LICENSE-DUAL.txt, also contained within this package. Be sure
** to use the correct license for your needs. To view the commercial
** license, read LICENSE-COMMERCIAL.txt also contained within this
** package.
**
** If you do not have access to these files or are unsure which license
** is appropriate for your use, please contact the sales department at
** sales@captivemetrics.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


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
  return mdr_obj->mdr_is_supported(field_number);
}

const mdr_value_t &mdr_performance_counter_t::mdr_value(mdr_value_t &value)
{
  if (mdr_obj->mdr_is_singleton)
    return mdr_obj->mdr_field_value(field_number, value);
  else
    return mdr_obj->mdr_field_value(instance_name, field_number, value);
}

const mdr_value_t &mdr_performance_counter_t::mdr_next_value(mdr_value_t &value)
{
  mdr_obj->mdr_refresh_rates();

  if (mdr_obj->mdr_is_singleton)
    return mdr_obj->mdr_field_value(field_number, value);
  else
    return mdr_obj->mdr_field_value(instance_name, field_number, value);
}

}
