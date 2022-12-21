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


#include "mdr_cpustat.h"

namespace mdr {

mdr_cpustat_t::mdr_cpustat_t(const char *remote_host) :
  mdr_t(MDR_CPUSTAT_NAME, true, MCS_FIELD_COUNT, true)
{
  mdr_field_names[MCS_USER_TIME_PERCENT]   = "Percent User Time";
  mdr_field_names[MCS_KERNEL_TIME_PERCENT] = "Percent Kernel Time";
  mdr_field_names[MCS_WAIT_TIME_PERCENT]   = "Percent Wait Time";
  mdr_field_names[MCS_IDLE_TIME_PERCENT]   = "Percent Idle Time";

  // sync localized names
  mdr_map_names();

  mcs_user_time_percent = 0;
  mcs_kernel_time_percent = 0;
  mcs_wait_time_percent = 0;
  mcs_idle_time_percent = 0;

  mpstat = new mdr_mpstat_t(remote_host);

  if (mpstat->mdr_is_supported(MMPS_USER_TIME_PERCENT))
    mdr_support_field(MCS_USER_TIME_PERCENT);
  if (mpstat->mdr_is_supported(MMPS_KERNEL_TIME_PERCENT))
    mdr_support_field(MCS_KERNEL_TIME_PERCENT);
  if (mpstat->mdr_is_supported(MMPS_WAIT_TIME_PERCENT))
    mdr_support_field(MCS_WAIT_TIME_PERCENT);
  if (mpstat->mdr_is_supported(MMPS_IDLE_TIME_PERCENT))
    mdr_support_field(MCS_IDLE_TIME_PERCENT);

  mdr_explanation_text = _GX_("CpuStat_Category");

  mcs_recompute();
}

mdr_cpustat_t::~mdr_cpustat_t()
{
  delete mpstat;
}

const mdr_value_t &mdr_cpustat_t::mdr_field_value(int field_number, mdr_value_t &value)
{
  switch(field_number) {
  case MCS_USER_TIME_PERCENT:
    value.mv_type = MVT_I32;
    value.mv_i32 = mcs_user_time_percent;
    break;
  case MCS_KERNEL_TIME_PERCENT:
    value.mv_type = MVT_I32;
    value.mv_i32 = mcs_kernel_time_percent;
    break;
  case MCS_WAIT_TIME_PERCENT:
    value.mv_type = MVT_I32;
    value.mv_i32 = mcs_wait_time_percent;
    break;
  case MCS_IDLE_TIME_PERCENT:
    value.mv_type = MVT_I32;
    value.mv_i32 = mcs_idle_time_percent;
    break;
  }
  return value;
}

void mdr_cpustat_t::mcs_recompute(void)
{
  int user;
  int kernel;
  int wait;
  int idle;
  int total;
  mmps_node_t n;
  mmps_can_t *cp;

  user = kernel = wait = idle = 0;
  for(n=mpstat->begin(); n != mpstat->end(); n++) {
    cp = &n->second;
    user   += cp->mmps_user_time_percent;
    kernel += cp->mmps_kernel_time_percent;
    wait   += cp->mmps_wait_time_percent;
    idle   += cp->mmps_idle_time_percent;
  }
  total = user + kernel + wait + idle;

  mcs_user_time_percent   = (user   * 100) / total;
  mcs_kernel_time_percent = (kernel * 100) / total;
  mcs_wait_time_percent   = (wait   * 100) / total;
  mcs_idle_time_percent   = (idle   * 100) / total;

  total = mcs_user_time_percent + mcs_kernel_time_percent +
          mcs_wait_time_percent + mcs_idle_time_percent;

  mcs_idle_time_percent += (100 - total);
}

void mdr_cpustat_t::mdr_refresh(void)
{
  mpstat->mdr_refresh();
  mcs_recompute();
}

}
