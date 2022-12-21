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


#include "mdr_mpstat.h"

namespace mdr {

mdr_mpstat_t::mdr_mpstat_t(const char *remote_host) :
  mdr_t(MDR_MPSTAT_NAME, true, MMPS_FIELD_COUNT, false)
{
  mdr_field_names[MMPS_USER_TIME_PERCENT]   = "Percent User Time";
  mdr_field_names[MMPS_KERNEL_TIME_PERCENT] = "Percent Kernel Time";
  mdr_field_names[MMPS_WAIT_TIME_PERCENT]   = "Percent Wait Time";
  mdr_field_names[MMPS_IDLE_TIME_PERCENT]   = "Percent Idle Time";

  // sync localized names
  mdr_map_names();

  // "instantiate"
  misc = new mdr_misc_t(remote_host);
  mp = new mdr_mp_t(remote_host);

  // support mask comes from mp
  if (mp->mdr_is_supported(MMP_USER_TICKS))
    mdr_support_field(MMPS_USER_TIME_PERCENT);
  if (mp->mdr_is_supported(MMP_KERNEL_TICKS))
    mdr_support_field(MMPS_KERNEL_TIME_PERCENT);
  if (mp->mdr_is_supported(MMP_WAIT_TICKS))
    mdr_support_field(MMPS_WAIT_TIME_PERCENT);
  if (mp->mdr_is_supported(MMP_IDLE_TICKS))
    mdr_support_field(MMPS_IDLE_TIME_PERCENT);

  mdr_explanation_text = _GX_("MPStat_Category");

  // recompute
  mmps_recompute();
}

mdr_mpstat_t::~mdr_mpstat_t()
{
  delete misc;
  delete mp;
}

const mdr_value_t &mdr_mpstat_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  mmps_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_mpstat_t::mdr_field_value: key %s not found", key.c_str());

  switch(field_number) {
  case MMPS_USER_TIME_PERCENT:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mmps_user_time_percent;
    break;
  case MMPS_KERNEL_TIME_PERCENT:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mmps_kernel_time_percent;
    break;
  case MMPS_WAIT_TIME_PERCENT:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mmps_wait_time_percent;
    break;
  case MMPS_IDLE_TIME_PERCENT:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mmps_idle_time_percent;
    break;
  }
  return value;
}

const mdr_keyset_t &mdr_mpstat_t::mdr_keyset(mdr_keyset_t &keyset)
{
  mmps_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_mpstat_t::mmps_recompute(void)
{
  double user;
  double kernel;
  double wait;
  double idle;
  double total;
  mmp_node_t n;
  mmp_can_t *cp;
  mmps_can_t can;

  clear();
  for(n=mp->begin(); n != mp->end(); n++) {
    cp = &n->second;
    user    = cp->mmp_user_ticks;
    user   += cp->mmp_nice_ticks;
    kernel  = cp->mmp_kernel_ticks;
    kernel += cp->mmp_irq_ticks;
    kernel += cp->mmp_soft_irq_ticks;
    wait    = cp->mmp_wait_ticks;
    idle    = cp->mmp_idle_ticks;

    total = user + kernel + wait + idle;

    can.mmps_user_time_percent   = (int) ((user   * 100.0) / total);
    can.mmps_kernel_time_percent = (int) ((kernel * 100.0) / total);
    can.mmps_wait_time_percent   = (int) ((wait   * 100.0) / total);
    can.mmps_idle_time_percent   = (int) ((idle   * 100.0) / total);

    total = can.mmps_user_time_percent + can.mmps_kernel_time_percent +
            can.mmps_wait_time_percent + can.mmps_idle_time_percent;

    can.mmps_idle_time_percent += (100 - (int) total);

    (*this)[n->first] = can;
  }
}

void mdr_mpstat_t::mdr_refresh(void)
{
  misc->mdr_refresh_rates();
  mp->mdr_refresh_rates();
  mmps_recompute();
}

}
