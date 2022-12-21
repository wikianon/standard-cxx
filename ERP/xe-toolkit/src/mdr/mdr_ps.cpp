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


#include "mdr_ps.h"
#include <pwd.h>

namespace mdr {

mdr_ps_t::mdr_ps_t(const char *remote_host) :
  mdr_t(MDR_PS_NAME, true, MPS_FIELD_COUNT, false)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_ps_t", remote_host, MPS_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MPS_START);
    mdr_support_field(MPS_UID);
    mdr_support_field(MPS_EUID);
    mdr_support_field(MPS_PID);
    mdr_support_field(MPS_PPID);
    mdr_support_field(MPS_TTY_DEVICE);
    mdr_support_field(MPS_FILE_NAME);
    mdr_support_field(MPS_COMMAND);
    mdr_support_field(MPS_FIRST_ARGUMENT);
    mdr_support_field(MPS_CPU_TIME);
#if defined(_SOLARIS) || defined(_LINUX) || defined(_DARWIN) || defined(_FREEBSD) || defined(_AIX)
    mdr_support_field(MPS_USR_TIME);
    mdr_support_field(MPS_KERNEL_TIME);
    mdr_support_field(MPS_USR_TIME_PERCENT);
    mdr_support_field(MPS_KERNEL_TIME_PERCENT);
#endif
    mdr_support_field(MPS_SIZE_K);
    mdr_support_field(MPS_RSSIZE_K);
#if defined(_SOLARIS) || defined(_AIX)
    mdr_support_field(MPS_IO_K);
#endif
    mdr_support_field(MPS_CPU_PERCENT);
    mdr_support_field(MPS_MEMORY_PERCENT);
#if defined(_SOLARIS) || defined(_DARWIN)
    mdr_support_field(MPS_SYSTEM_CALLS);
#endif
#if defined(_SOLARIS) || defined(_DARWIN) || defined(_FREEBSD) || defined(_AIX)
    mdr_support_field(MPS_VOLUNTARY_CSW);
#endif
#if defined(_SOLARIS) || defined(_FREEBSD) || defined(_AIX)
    mdr_support_field(MPS_INVOLUNTARY_CSW);
#endif
    mdr_support_field(MPS_DISK_FAULTS);
    mdr_support_field(MPS_PAGE_RECLAIMS);
    mdr_support_field(MPS_STATE);
    mdr_support_field(MPS_PRIORITY);
    mdr_support_field(MPS_NICE);
#if defined(_SOLARIS) || defined(_LINUX) || defined(_DARWIN) || defined(_AIX)
    mdr_support_field(MPS_THREAD_COUNT);
#endif
    mdr_support_field(MPS_SIZE_DELTA_K);
    mdr_support_field(MPS_RSSIZE_DELTA_K);

    mdr_support_field(MPS_PROCS_RUNNABLE);
    mdr_support_field(MPS_PROCS_SLEEPING);
    mdr_support_field(MPS_PROCS_STOPPED);
#if defined(_LINUX) || defined(_DARWIN) || defined(_FREEBSD) || defined(_AIX)
    mdr_support_field(MPS_PROCS_BLOCKED);
#endif
    mdr_support_field(MPS_PROCS_ZOMBIE);
    mdr_support_field(MPS_PROCS_SPECIFIC);
    mdr_support_field(MPS_USER_NAME);
    mdr_support_field(MPS_EFFECTIVE_USER_NAME);
    mdr_support_field(MPS_IS_SYSTEM_PROCESS);

    net_client = 0;
  }

  mdr_field_names[MPS_START]               = "Start Time";
  mdr_field_names[MPS_UID]                 = "User ID";
  mdr_field_names[MPS_EUID]                = "Effective User ID";
  mdr_field_names[MPS_PID]                 = "Process ID";
  mdr_field_names[MPS_PPID]                = "Parent Process ID";
  mdr_field_names[MPS_TTY_DEVICE]          = "Terminal Device ID";
  mdr_field_names[MPS_FILE_NAME]           = "File Name";
  mdr_field_names[MPS_COMMAND]             = "Full Command";
  mdr_field_names[MPS_FIRST_ARGUMENT]      = "First Argument";
  mdr_field_names[MPS_CPU_TIME]            = "Total CPU Seconds";
  mdr_field_names[MPS_USR_TIME]            = "User CPU Seconds";
  mdr_field_names[MPS_KERNEL_TIME]         = "Kernel CPU Seconds";
  mdr_field_names[MPS_USR_TIME_PERCENT]    = "Percent User Time";
  mdr_field_names[MPS_KERNEL_TIME_PERCENT] = "Percent Kernel Time";
  mdr_field_names[MPS_SIZE_K]              = "Total Size";
  mdr_field_names[MPS_RSSIZE_K]            = "Resident Size";
  mdr_field_names[MPS_IO_K]                = "Total I/O";
  mdr_field_names[MPS_CPU_PERCENT]         = "Percent CPU Use";
  mdr_field_names[MPS_MEMORY_PERCENT]      = "Percent Physmem Use";
  mdr_field_names[MPS_SYSTEM_CALLS]        = "System Calls";
  mdr_field_names[MPS_VOLUNTARY_CSW]       = "Voluntary Context Switches";
  mdr_field_names[MPS_INVOLUNTARY_CSW]     = "Involuntary Context Switches";
  mdr_field_names[MPS_DISK_FAULTS]         = "Major Page Faults";
  mdr_field_names[MPS_PAGE_RECLAIMS]       = "Minor Page Faults";
  mdr_field_names[MPS_STATE]               = "Process State";
  mdr_field_names[MPS_PRIORITY]            = "Global Priority";
  mdr_field_names[MPS_NICE]                = "Unix Nice Value";
  mdr_field_names[MPS_THREAD_COUNT]        = "Number of Threads";
  mdr_field_names[MPS_SIZE_DELTA_K]        = "Total Size Delta";
  mdr_field_names[MPS_RSSIZE_DELTA_K]      = "Resident Size Delta";
  mdr_field_names[MPS_USER_NAME]           = "User Name";
  mdr_field_names[MPS_EFFECTIVE_USER_NAME] = "Effective User Name";
  mdr_field_names[MPS_IS_SYSTEM_PROCESS]   = "Is System Process";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("PS_Category");

  first_pass = true;

#if defined(_DARWIN) || defined(_FREEBSD) || defined(_AIX)
  mps_array = 0;
  mps_array_max = 0;
#endif
  mps_sort_array = 0;
  mps_sort_array_max = 0;
  mps_sort_array_size = 0;
  mps_sort_index = 0;
  mps_misc = new mdr_misc_t(remote_host);
  mps_total_mem_K =
    mps_misc->mm_physical_pages * (mps_misc->mm_native_page_size / 1024);
  mps_last_time.tv_sec = mps_misc->mm_boot_time;
  mps_last_time.tv_usec = 0;
  mps_sort_key = BY_PID;
  mps_reverse_sort = false;
  mdr_refresh_rates();
  mps_sort(BY_PID);
}

// destructor
mdr_ps_t::~mdr_ps_t()
{
#if defined(_DARWIN) || defined(_FREEBSD) || defined(_AIX)
  if (mps_array)
    delete[] mps_array;
#endif
  delete mps_misc;
  if (mps_sort_array)
    delete[] mps_sort_array;
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_ps_t::mdr_field_value(int field_number, mdr_value_t &value)
{
  switch(field_number) {
  case MPS_PROCS_RUNNABLE:
    value.mv_type = MVT_I32;
    value.mv_i32 = mps_procs_runnable;
    break;
  case MPS_PROCS_SLEEPING:
    value.mv_type = MVT_I32;
    value.mv_i32 = mps_procs_sleeping;
    break;
  case MPS_PROCS_STOPPED:
    value.mv_type = MVT_I32;
    value.mv_i32 = mps_procs_stopped;
    break;
  case MPS_PROCS_BLOCKED:
    value.mv_type = MVT_I32;
    value.mv_i32 = mps_procs_blocked;
    break;
  case MPS_PROCS_ZOMBIE:
    value.mv_type = MVT_I32;
    value.mv_i32 = mps_procs_zombie;
    break;
  case MPS_PROCS_SPECIFIC:
    value.mv_type = MVT_I32;
    value.mv_i32 = mps_procs_specific;
    break;
  }
  return value;
}

const mdr_value_t &mdr_ps_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  mps_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_ps_t::mdr_field_value: key %s not found", key.c_str());

  switch(field_number) {
  case MPS_START:
    value.mv_type = MVT_LONG;
    value.mv_l = n->second.mps_start;
    break;
  case MPS_UID:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mps_uid;
    break;
  case MPS_EUID:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mps_euid;
    break;
  case MPS_PID:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mps_pid;
    break;
  case MPS_PPID:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mps_ppid;
    break;
  case MPS_TTY_DEVICE:
    value.mv_type = MVT_LONG;
    value.mv_l = n->second.mps_tty_device;
    break;
  case MPS_FILE_NAME:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mps_file_name;
    break;
  case MPS_COMMAND:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mps_command;
    break;
  case MPS_FIRST_ARGUMENT:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mps_first_argument;
    break;
  case MPS_CPU_TIME:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mps_cpu_time;
    break;
  case MPS_USR_TIME:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mps_user_time;
    break;
  case MPS_KERNEL_TIME:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mps_kernel_time;
    break;
  case MPS_USR_TIME_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mps_user_time_percent;
    break;
  case MPS_KERNEL_TIME_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mps_kernel_time_percent;
    break;
  case MPS_SIZE_K:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mps_size_K;
    break;
  case MPS_RSSIZE_K:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mps_rssize_K;
    break;
  case MPS_IO_K:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mps_io_K;
    break;
  case MPS_CPU_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mps_cpu_percent;
    break;
  case MPS_MEMORY_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mps_memory_percent;
    break;
  case MPS_SYSTEM_CALLS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mps_system_calls;
    break;
  case MPS_VOLUNTARY_CSW:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mps_voluntary_csw;
    break;
  case MPS_INVOLUNTARY_CSW:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mps_involuntary_csw;
    break;
  case MPS_DISK_FAULTS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mps_disk_faults;
    break;
  case MPS_PAGE_RECLAIMS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mps_page_reclaims;
    break;
  case MPS_STATE:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mps_state;
    break;
  case MPS_PRIORITY:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mps_priority;
    break;
  case MPS_NICE:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mps_nice;
    break;
  case MPS_THREAD_COUNT:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mps_thread_count;
    break;
  case MPS_SIZE_DELTA_K:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mps_size_delta_K;
    break;
  case MPS_RSSIZE_DELTA_K:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mps_rssize_delta_K;
    break;
  case MPS_USER_NAME:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mps_user_name;
    break;
  case MPS_EFFECTIVE_USER_NAME:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mps_effective_user_name;
    break;
  case MPS_IS_SYSTEM_PROCESS:
    value.mv_type = MVT_BOOL;
    value.mv_b = n->second.mps_is_system_process;
    break;
  }
  return value;
}

void mdr_ps_t::mdr_rates(void)
{
  mps_node_t n;
  mps_node_t nn;
  mps_can_t *cp;
  mps_can_t *ocp;
  double tmp_user_time;
  double tmp_kernel_time;
  ulong_t tmp_size_K;
  ulong_t tmp_rssize_K;
  ulong_t tmp_io_K;
  uint_t tmp_system_calls;
  uint_t tmp_voluntary_csw;
  uint_t tmp_involuntary_csw;
  uint_t tmp_disk_faults;
  uint_t tmp_page_reclaims;
  timeval now_tod;
  double now_usec;
  double then_usec;
  double elapsed;
  double potential;
  int engines;

  gettimeofday(&now_tod, 0);
  now_usec = ((double) now_tod.tv_sec * 1000000.0) + now_tod.tv_usec;
  then_usec =
    ((double) mps_last_time.tv_sec * 1000000.0) + mps_last_time.tv_usec;
  elapsed = (now_usec - then_usec) / 1000000.0;
  mps_last_time = now_tod;

  // if less than a second then round it up
  if (elapsed < 1.0)
    elapsed = 1.0;

  // compute elapsed time in seconds
  if (first_pass)
    first_pass = false;
  else
    mps_misc->mdr_refresh_rates();

  for(n=begin(); n != end(); n++) {
    cp = &n->second;
    nn = old.find(n->first);
    if (nn == old.end()) {
      // should not happen
      old[n->first].clear();
      continue;
    }
    ocp = &nn->second;

    tmp_user_time = cp->mps_user_time;
    tmp_kernel_time = cp->mps_kernel_time;
    tmp_size_K = cp->mps_size_K;
    tmp_rssize_K = cp->mps_rssize_K;
    tmp_io_K = cp->mps_io_K;
    tmp_system_calls = cp->mps_system_calls;
    tmp_voluntary_csw = cp->mps_voluntary_csw;
    tmp_involuntary_csw = cp->mps_involuntary_csw;
    tmp_disk_faults = cp->mps_disk_faults;
    tmp_page_reclaims = cp->mps_page_reclaims;

    // cannot use more cpu time than there are threads or cpus
    engines = (mps_misc->mm_processors_online < cp->mps_thread_count) ?
      mps_misc->mm_processors_online : cp->mps_thread_count;
    // zombies have zero threads
    if (engines == 0)
      engines = 1;
    potential = elapsed * engines;

    cp->mps_user_time -= ocp->mps_user_time;
    cp->mps_kernel_time -= ocp->mps_kernel_time;
    cp->mps_user_time_percent = (cp->mps_user_time * 100.0) / potential;
    cp->mps_kernel_time_percent = (cp->mps_kernel_time * 100.0) / potential;
    cp->mps_cpu_percent =
      cp->mps_user_time_percent + cp->mps_kernel_time_percent;
#ifdef _LP64
    cp->mps_size_delta_K = cp->mps_size_K - ocp->mps_size_K;
    cp->mps_rssize_delta_K = cp->mps_rssize_K - ocp->mps_rssize_K;
    cp->mps_io_K -= ocp->mps_io_K;
#else
    cp->mps_size_delta_K =
      wrap_diff_32(cp->mps_size_K, ocp->mps_size_K);
    cp->mps_rssize_delta_K =
      wrap_diff_32(cp->mps_rssize_K, ocp->mps_rssize_K);
    cp->mps_io_K =
      wrap_diff_32(cp->mps_io_K, ocp->mps_io_K);
#endif
    cp->mps_system_calls =
      wrap_diff_32(cp->mps_system_calls, ocp->mps_system_calls);
    cp->mps_voluntary_csw =
      wrap_diff_32(cp->mps_voluntary_csw, ocp->mps_voluntary_csw);
    cp->mps_involuntary_csw =
      wrap_diff_32(cp->mps_involuntary_csw, ocp->mps_involuntary_csw);
    cp->mps_disk_faults =
      wrap_diff_32(cp->mps_disk_faults, ocp->mps_disk_faults);
    cp->mps_page_reclaims =
      wrap_diff_32(cp->mps_page_reclaims, ocp->mps_page_reclaims);

    ocp->mps_user_time = tmp_user_time;
    ocp->mps_kernel_time = tmp_kernel_time;
    ocp->mps_size_K = tmp_size_K;
    ocp->mps_rssize_K = tmp_rssize_K;
    ocp->mps_io_K = tmp_io_K;
    ocp->mps_system_calls = tmp_system_calls;
    ocp->mps_voluntary_csw = tmp_voluntary_csw;
    ocp->mps_involuntary_csw = tmp_involuntary_csw;
    ocp->mps_disk_faults = tmp_disk_faults;
    ocp->mps_page_reclaims = tmp_page_reclaims;
  }

  mps_sort(mps_sort_key, mps_reverse_sort);
}

const mdr_keyset_t &mdr_ps_t::mdr_keyset(mdr_keyset_t &keyset)
{
  mps_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_ps_t::ps_remote_refresh(void)
{
  mdr_m_mm_t m_map;
  mdr_m_mm_node_t mn;
  mdr_s_mm_t ps_map;
  mps_can_t can;
  int i;
  int n;

  // start fresh
  clear();

  net_client->mc_refresh(m_map);
  for(mn=m_map.begin(); mn != m_map.end(); mn++) {
    mdr_s_mm_t &s_map = mn->second;

    can.clear();

    for(i=0, n=s_map.size(); i<n; i++) {
      switch(i) {
      case MPS_START:
        switch(s_map[i].mv_type) {
        case MVT_I32:
          can.mps_start = s_map[i].mv_i32;
          break;
        case MVT_I64:
          can.mps_start = s_map[i].mv_i64;
          break;
        default:
          can.mps_start = -1;
          break;
        }
        break;
      case MPS_UID:
        switch(s_map[i].mv_type) {
        case MVT_I32:
          can.mps_uid = (uid_t) s_map[i].mv_i32;
          break;
        case MVT_UI32:
          can.mps_uid = (uid_t) s_map[i].mv_ui32;
          break;
        case MVT_I64:
          can.mps_uid = (uid_t) s_map[i].mv_i64;
          break;
        default:
          can.mps_uid = (uid_t) -1;
          break;
        }
        break;
      case MPS_EUID:
        switch(s_map[i].mv_type) {
        case MVT_I32:
          can.mps_euid = (uid_t) s_map[i].mv_i32;
          break;
        case MVT_UI32:
          can.mps_euid = (uid_t) s_map[i].mv_ui32;
          break;
        case MVT_I64:
          can.mps_euid = (uid_t) s_map[i].mv_i64;
          break;
        default:
          can.mps_euid = (uid_t) -1;
          break;
        }
        break;
      case MPS_PID:
        switch(s_map[i].mv_type) {
        case MVT_I32:
          can.mps_pid = (pid_t) s_map[i].mv_i32;
          break;
        case MVT_UI32:
          can.mps_pid = (pid_t) s_map[i].mv_ui32;
          break;
        case MVT_I64:
          can.mps_pid = (pid_t) s_map[i].mv_i64;
          break;
        default:
          can.mps_pid = (pid_t) -1;
          break;
        }
        break;
      case MPS_PPID:
        switch(s_map[i].mv_type) {
        case MVT_I32:
          can.mps_ppid = (pid_t) s_map[i].mv_i32;
          break;
        case MVT_UI32:
          can.mps_ppid = (pid_t) s_map[i].mv_ui32;
          break;
        case MVT_I64:
          can.mps_ppid = (pid_t) s_map[i].mv_i64;
          break;
        default:
          can.mps_ppid = (pid_t) -1;
          break;
        }
        break;
      case MPS_TTY_DEVICE:
        switch(s_map[i].mv_type) {
        case MVT_I32:
          can.mps_tty_device = s_map[i].mv_i32;
          break;
        case MVT_I64:
          can.mps_tty_device = s_map[i].mv_i64;
          break;
        default:
          can.mps_tty_device = -1;
          break;
        }
        break;
      case MPS_FILE_NAME:
        can.mps_file_name = *s_map[i].mv_s;
        break;
      case MPS_COMMAND:
        can.mps_command = *s_map[i].mv_s;
        break;
      case MPS_FIRST_ARGUMENT:
        can.mps_first_argument = *s_map[i].mv_s;
        break;
      case MPS_CPU_TIME:
        can.mps_cpu_time = s_map[i].mv_d;
        break;
      case MPS_USR_TIME:
        can.mps_user_time = s_map[i].mv_d;
        break;
      case MPS_KERNEL_TIME:
        can.mps_kernel_time = s_map[i].mv_d;
        break;
      case MPS_USR_TIME_PERCENT:
        can.mps_user_time_percent = s_map[i].mv_d;
        break;
      case MPS_KERNEL_TIME_PERCENT:
        can.mps_kernel_time_percent = s_map[i].mv_d;
        break;
      case MPS_SIZE_K:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mps_size_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mps_size_K = s_map[i].mv_ui64;
          break;
        default:
          can.mps_size_K = (ulong_t) -1;
          break;
        }
        break;
      case MPS_RSSIZE_K:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mps_rssize_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mps_rssize_K = s_map[i].mv_ui64;
          break;
        default:
          can.mps_rssize_K = (ulong_t) -1;
          break;
        }
        break;
      case MPS_IO_K:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mps_io_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mps_io_K = s_map[i].mv_ui64;
          break;
        default:
          can.mps_io_K = (ulong_t) -1;
          break;
        }
        break;
      case MPS_CPU_PERCENT:
        can.mps_cpu_percent = s_map[i].mv_d;
        break;
      case MPS_MEMORY_PERCENT:
        can.mps_memory_percent = s_map[i].mv_d;
        break;
      case MPS_SYSTEM_CALLS:
        can.mps_system_calls = s_map[i].mv_ui32;
        break;
      case MPS_VOLUNTARY_CSW:
        can.mps_voluntary_csw = s_map[i].mv_ui32;
        break;
      case MPS_INVOLUNTARY_CSW:
        can.mps_involuntary_csw = s_map[i].mv_ui32;
        break;
      case MPS_DISK_FAULTS:
        can.mps_disk_faults = s_map[i].mv_ui32;
        break;
      case MPS_PAGE_RECLAIMS:
        can.mps_page_reclaims = s_map[i].mv_ui32;
        break;
      case MPS_STATE:
        can.mps_state = s_map[i].mv_i32;
        break;
      case MPS_PRIORITY:
        can.mps_priority = s_map[i].mv_i32;
        break;
      case MPS_NICE:
        can.mps_nice = s_map[i].mv_i32;
        break;
      case MPS_THREAD_COUNT:
        can.mps_thread_count = s_map[i].mv_i32;
        break;
      case MPS_SIZE_DELTA_K:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mps_size_delta_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mps_size_delta_K = s_map[i].mv_ui64;
          break;
        default:
          can.mps_size_delta_K = (ulong_t) -1;
          break;
        }
        break;
      case MPS_RSSIZE_DELTA_K:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mps_rssize_delta_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mps_rssize_delta_K = s_map[i].mv_ui64;
          break;
        default:
          can.mps_rssize_delta_K = (ulong_t) -1;
          break;
        }
        break;
      case MPS_USER_NAME:
        can.mps_user_name = *s_map[i].mv_s;
        break;
      case MPS_EFFECTIVE_USER_NAME:
        can.mps_effective_user_name = *s_map[i].mv_s;
        break;
      case MPS_IS_SYSTEM_PROCESS:
        can.mps_is_system_process = s_map[i].mv_b;
        break;
      default:
        break;
      }
    }

    (*this)[mn->first] = can;
  }

  net_client->mc_release(m_map);

  // got the processes, now get the process info
  net_client->mc_refresh(ps_map, "mdr_ps_info");
  for(i=0, n=ps_map.size(); i<n; i++) {
    switch(i + MPS_FIELD_COUNT) {
    case MPS_PROCS_RUNNABLE:
      mps_procs_runnable = ps_map[i].mv_i32;
      break;
    case MPS_PROCS_SLEEPING:
      mps_procs_sleeping = ps_map[i].mv_i32;
      break;
    case MPS_PROCS_STOPPED:
      mps_procs_stopped = ps_map[i].mv_i32;
      break;
    case MPS_PROCS_BLOCKED:
      mps_procs_blocked = ps_map[i].mv_i32;
      break;
    case MPS_PROCS_ZOMBIE:
      mps_procs_zombie = ps_map[i].mv_i32;
      break;
    case MPS_PROCS_SPECIFIC:
      mps_procs_specific = ps_map[i].mv_i32;
      break;
    default:
      break;
    }
  }

  net_client->mc_release(ps_map);
}

extern "C" {

  static int reverse_key = 1;

  static int pid_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return (ap->mps_pid - bp->mps_pid) * reverse_key;
  }

  static int time_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    if (ap->mps_cpu_time == bp->mps_cpu_time)
      return 0;
    if (ap->mps_cpu_time > bp->mps_cpu_time)
      return reverse_key;
    return -reverse_key;
  }

  static int utime_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    if (ap->mps_user_time_percent == bp->mps_user_time_percent)
      return 0;
    if (ap->mps_user_time_percent > bp->mps_user_time_percent)
      return reverse_key;
    return -reverse_key;
  }

  static int stime_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    if (ap->mps_kernel_time_percent == bp->mps_kernel_time_percent)
      return 0;
    if (ap->mps_kernel_time_percent > bp->mps_kernel_time_percent)
      return reverse_key;
    return -reverse_key;
  }

  static int ctime_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    if (ap->mps_cpu_percent == bp->mps_cpu_percent)
      return 0;
    if (ap->mps_cpu_percent > bp->mps_cpu_percent)
      return reverse_key;
    return -reverse_key;
  }

  static int memsz_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    if (ap->mps_memory_percent == bp->mps_memory_percent)
      return 0;
    if (ap->mps_memory_percent > bp->mps_memory_percent)
      return reverse_key;
    return -reverse_key;
  }

  static int vsize_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return (int) ((long) ap->mps_size_K - (long) bp->mps_size_K) * reverse_key;
  }

  static int rsize_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return
      (int) ((long) ap->mps_rssize_K - (long) bp->mps_rssize_K) * reverse_key;
  }

  static int io_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return (int) ((long) ap->mps_io_K - (long) bp->mps_io_K) * reverse_key;
  }

  static int uid_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return (ap->mps_uid - bp->mps_uid) * reverse_key;
  }

  static int sysc_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return
      ((int) ap->mps_system_calls - (int) bp->mps_system_calls) * reverse_key;
  }

  static int vctx_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return
      ((int) ap->mps_voluntary_csw - (int) bp->mps_voluntary_csw) * reverse_key;
  }

  static int ictx_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return ((int) ap->mps_involuntary_csw -
              (int) bp->mps_involuntary_csw) * reverse_key;
  }

  static int majf_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return
      ((int) ap->mps_disk_faults - (int) bp->mps_disk_faults) * reverse_key;
  }

  static int minf_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return
      ((int) ap->mps_page_reclaims - (int) bp->mps_page_reclaims) * reverse_key;
  }

  static int thread_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return (ap->mps_thread_count - bp->mps_thread_count) * reverse_key;
  }

  static int size_d_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return (int)
      ((long) ap->mps_size_delta_K -
        (long) bp->mps_size_delta_K) * reverse_key;
  }

  static int rssize_d_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return (int)
      ((long) ap->mps_rssize_delta_K -
        (long) bp->mps_rssize_delta_K) * reverse_key;
  }

  static int username_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return strcmp(ap->mps_user_name.c_str(),
                  bp->mps_user_name.c_str()) * reverse_key;
  }

  static int state_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return (ap->mps_state - bp->mps_state) * reverse_key;
  }

  static int command_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return strcmp(ap->mps_user_name.c_str(),
                  bp->mps_user_name.c_str()) * reverse_key;
  }

  static int fname_cmp(const void *a, const void *b)
  {
    mps_can_t *ap = ((mps_can_t *) a);
    mps_can_t *bp = ((mps_can_t *) b);

    return strcmp(ap->mps_file_name.c_str(),
                  bp->mps_file_name.c_str()) * reverse_key;
  }

  static int (*compare[])(const void *, const void *) = {
    pid_cmp,       // BY_PID
    time_cmp,      // BY_CPU_TIME
    utime_cmp,     // BY_USER_TIME
    stime_cmp,     // BY_KERNEL_TIME
    ctime_cmp,     // BY_CPU_USE
    memsz_cmp,     // BY_MEM_USE
    vsize_cmp,     // BY_SIZE
    rsize_cmp,     // BY_RSSIZE
    io_cmp,        // BY_IO
    uid_cmp,       // BY_UID
    sysc_cmp,      // BY_SYSTEM_CALLS
    vctx_cmp,      // BY_VCONTEXTS
    ictx_cmp,      // BY_ICONTEXTS
    majf_cmp,      // BY_DISK_FAULTS
    minf_cmp,      // BY_PAGE_RECLAIMS
    thread_cmp,    // BY_THREAD_COUNT
    size_d_cmp,    // BY_SIZE_DELTA
    rssize_d_cmp,  // BY_RSSIZE_DELTA
    username_cmp,  // BY_USERNAME
    state_cmp,     // BY_STATE
    command_cmp,   // BY_COMMAND
    fname_cmp      // BY_FILE_NAME
  };
}

void mdr_ps_t::mps_sort(mps_sort_key_t sort_key, bool reverse)
{
  if (sort_key < MPS_FIRST_SORT_KEY || sort_key > MPS_LAST_SORT_KEY)
    return; // less offensive than throwing an exception

  int n = size();
  iterator node;

  if (n == 0)
    return; // nothing to sort
  reverse_key = (reverse ? -1 : 1);
  if (n > mps_sort_array_max) {
    if (mps_sort_array)
      delete[] mps_sort_array;
    mps_sort_array = new mps_can_t[n];
    mps_sort_array_max = n;
  }
  for(mps_sort_array_size=0, node=begin();
      node != end(); node++, mps_sort_array_size++)
    mps_sort_array[mps_sort_array_size] = node->second;
  qsort(mps_sort_array, mps_sort_array_size,
        sizeof(mps_can_t), compare[(int) sort_key]);
  mps_sort_key = sort_key;
  mps_reverse_sort = reverse;
  mps_sort_index = 0;
}

void mdr_ps_t::mps_rewind(void)
{
  mps_sort_index = 0;
}

bool mdr_ps_t::mps_next_sorted_proc(mps_can_t &can)
{
  if (mps_sort_index == mps_sort_array_size)
    return false;
  can = mps_sort_array[mps_sort_index++];
  return true;
}

const char *mdr_ps_t::uid_lookup(uid_t uid)
{
  struct passwd *pwd;
  mps_uid_node_t node;

  node = uid_map.find(uid);
  if (node == uid_map.end()) {
    pwd = getpwuid(uid);
    if (pwd == 0)
      uid_map[uid] = "???";
    else
      uid_map[uid] = pwd->pw_name;
    node = uid_map.find(uid);
  }
  return node->second.c_str();
}

const bool mdr_ps_t::mps_lookup_by_pid(pid_t pid, mps_can_t &can)
{
  mps_node_t n;

  // gak<>linear search
  for(n=begin(); n != end(); n++)
    if (n->second.mps_pid == pid) {
      can = n->second;
      return true;
    }

  return false;
}

}
