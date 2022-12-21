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


#include "mdr_proc_states.h"

namespace mdr {

mdr_proc_states_t::mdr_proc_states_t(const char *remote_host) :
  mdr_t(MDR_PROC_STATES_NAME, true, MPST_FIELD_COUNT, true)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_proc_states_t", remote_host, MPST_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    if (ps.mdr_is_supported(MPST_RUNNABLE))
      mdr_support_field(MPST_RUNNABLE);
    if (ps.mdr_is_supported(MPST_SLEEPING))
      mdr_support_field(MPST_SLEEPING);
    if (ps.mdr_is_supported(MPST_STOPPED))
      mdr_support_field(MPST_STOPPED);
    if (ps.mdr_is_supported(MPST_BLOCKED))
      mdr_support_field(MPST_BLOCKED);
    if (ps.mdr_is_supported(MPST_ZOMBIE))
      mdr_support_field(MPST_ZOMBIE);
    if (ps.mdr_is_supported(MPST_SPECIFIC))
      mdr_support_field(MPST_SPECIFIC);

    net_client = 0;
  }

  mdr_field_names[MPST_RUNNABLE] = "Runnable";
  mdr_field_names[MPST_SLEEPING] = "Sleeping";
  mdr_field_names[MPST_STOPPED]  = "Stopped";
  mdr_field_names[MPST_BLOCKED]  = "Blocked";
  mdr_field_names[MPST_ZOMBIE]   = "Unreaped";
  mdr_field_names[MPST_SPECIFIC] = "Specific";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("ProcStates_Category");

  mpst_procs_runnable = 0;
  mpst_procs_sleeping = 0;
  mpst_procs_stopped = 0;
  mpst_procs_blocked = 0;
  mpst_procs_zombie = 0;
  mpst_procs_specific = 0;

  // don't call mdr_refresh; spare the ps.mdr_refresh
  mpst_procs_runnable = ps.mps_procs_runnable;
  mpst_procs_sleeping = ps.mps_procs_sleeping;
  mpst_procs_stopped  = ps.mps_procs_stopped;
  mpst_procs_blocked  = ps.mps_procs_blocked;
  mpst_procs_zombie   = ps.mps_procs_zombie;
  mpst_procs_specific = ps.mps_procs_specific;
}

mdr_proc_states_t::~mdr_proc_states_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_proc_states_t::mdr_field_value(int field_number, mdr_value_t &value)
{
  switch(field_number) {
  case MPST_RUNNABLE:
    value.mv_type = MVT_I32;
    value.mv_i32 = mpst_procs_runnable;
    break;
  case MPST_SLEEPING:
    value.mv_type = MVT_I32;
    value.mv_i32 = mpst_procs_sleeping;
    break;
  case MPST_STOPPED:
    value.mv_type = MVT_I32;
    value.mv_i32 = mpst_procs_stopped;
    break;
  case MPST_BLOCKED:
    value.mv_type = MVT_I32;
    value.mv_i32 = mpst_procs_blocked;
    break;
  case MPST_ZOMBIE:
    value.mv_type = MVT_I32;
    value.mv_i32 = mpst_procs_zombie;
    break;
  case MPST_SPECIFIC:
    value.mv_type = MVT_I32;
    value.mv_i32 = mpst_procs_specific;
    break;
  }

  return value;
}

void mdr_proc_states_t::mdr_refresh(void)
{
  ps.mdr_refresh();
  mpst_procs_runnable = ps.mps_procs_runnable;
  mpst_procs_sleeping = ps.mps_procs_sleeping;
  mpst_procs_stopped  = ps.mps_procs_stopped;
  mpst_procs_blocked  = ps.mps_procs_blocked;
  mpst_procs_zombie   = ps.mps_procs_zombie;
  mpst_procs_specific = ps.mps_procs_specific;
}

void mdr_proc_states_t::proc_states_remote_refresh(void)
{
  mdr_s_mm_t s_map;
  int i;
  int n;

  net_client->mc_refresh(s_map);
  for(i=0, n=s_map.size(); i<n; i++) {
    switch(i) {
    case MPST_RUNNABLE:
      mpst_procs_runnable = s_map[i].mv_i32;
      break;
    case MPST_SLEEPING:
      mpst_procs_sleeping = s_map[i].mv_i32;
      break;
    case MPST_STOPPED:
      mpst_procs_stopped = s_map[i].mv_i32;
      break;
    case MPST_BLOCKED:
      mpst_procs_blocked = s_map[i].mv_i32;
      break;
    case MPST_ZOMBIE:
      mpst_procs_zombie = s_map[i].mv_i32;
      break;
    case MPST_SPECIFIC:
      mpst_procs_specific = s_map[i].mv_i32;
      break;
    }
  }

  net_client->mc_release(s_map);
}

}
