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


#include <unistd.h>
#include "mdr_thread.h"

namespace mdr {

mdr_thread_t::mdr_thread_t(const char *remote_host) :
  mdr_t(MDR_THREAD_NAME, true, MT_FIELD_COUNT, false)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_thread_t", remote_host, MT_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MT_THREAD_ID);
    mdr_support_field(MT_CPU_TIME);
    mdr_support_field(MT_USR_TIME);
    mdr_support_field(MT_KERNEL_TIME);
    mdr_support_field(MT_USR_TIME_PERCENT);
    mdr_support_field(MT_KERNEL_TIME_PERCENT);
#if defined(_LINUX)
    mdr_support_field(MT_SIZE_K);
    mdr_support_field(MT_RSSIZE_K);
#endif
#if defined(_SOLARIS)
    mdr_support_field(MT_IO_K);
#endif
    mdr_support_field(MT_CPU_PERCENT);
#if defined(_LINUX)
    mdr_support_field(MT_MEMORY_PERCENT);
#endif
#if defined(_SOLARIS) || defined(_AIX)
    mdr_support_field(MT_VOLUNTARY_CSW);       // linux may set it
    mdr_support_field(MT_INVOLUNTARY_CSW);     // linux may set it
#endif
#if defined(_SOLARIS) || defined(_LINUX) || defined(_AIX)
    mdr_support_field(MT_DISK_FAULTS);
    mdr_support_field(MT_PAGE_RECLAIMS);
#endif
    mdr_support_field(MT_STATE);
#if defined(_SOLARIS) || defined(_LINUX) || defined(_AIX)
    mdr_support_field(MT_PRIORITY);
    mdr_support_field(MT_NICE);
#endif
#if defined(_LINUX)
    mdr_support_field(MT_SIZE_DELTA_K);
    mdr_support_field(MT_RSSIZE_DELTA_K);
#endif
    mdr_support_field(MT_PROCESS_ID);

    net_client = 0;
  }

  mdr_field_names[MT_THREAD_ID]           = "Thread ID";
  mdr_field_names[MT_CPU_TIME]            = "Total CPU Seconds";
  mdr_field_names[MT_USR_TIME]            = "User CPU Seconds";
  mdr_field_names[MT_KERNEL_TIME]         = "Kernel CPU Seconds";
  mdr_field_names[MT_USR_TIME_PERCENT]    = "Percent User Time";
  mdr_field_names[MT_KERNEL_TIME_PERCENT] = "Percent Kernel Time";
  mdr_field_names[MT_SIZE_K]              = "Total Size";
  mdr_field_names[MT_RSSIZE_K]            = "Resident Size";
  mdr_field_names[MT_IO_K]                = "Total I/O";
  mdr_field_names[MT_CPU_PERCENT]         = "Percent CPU Use";
  mdr_field_names[MT_MEMORY_PERCENT]      = "Percent Physmem Use";
  mdr_field_names[MT_VOLUNTARY_CSW]       = "Voluntary Context Switches";
  mdr_field_names[MT_INVOLUNTARY_CSW]     = "Involuntary Context Switches";
  mdr_field_names[MT_DISK_FAULTS]         = "Major Page Faults";
  mdr_field_names[MT_PAGE_RECLAIMS]       = "Minor Page Faults";
  mdr_field_names[MT_STATE]               = "Thread State";
  mdr_field_names[MT_PRIORITY]            = "Global Priority";
  mdr_field_names[MT_NICE]                = "Unix Nice Value";
  mdr_field_names[MT_SIZE_DELTA_K]        = "Total Size Delta";
  mdr_field_names[MT_RSSIZE_DELTA_K]      = "Resident Size Delta";
  mdr_field_names[MT_PROCESS_ID]          = "Id of Encapsulating Process";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("Thread_Category");

  mt_sort_array = 0;
  mt_sort_array_max = 0;
  mt_sort_array_size = 0;
  mt_sort_index = 0;
  mt_misc = new mdr_misc_t(remote_host);
  mt_total_mem_K =
    mt_misc->mm_physical_pages * (mt_misc->mm_native_page_size / 1024);
  mt_sort_key = BY_THREAD_ID;
  mt_reverse_sort = false;

  mt_last_time.tv_sec = mt_misc->mm_boot_time;
  mt_last_time.tv_usec = 0;

#ifdef _FREEBSD
  mt_array =0;
  mt_array_max = 0;
#endif

#ifdef _AIX
  mt_array = 0;
  mt_array_max = 0;
  mps_array = 0;
  mps_array_max = 0;
#endif

  mdr_refresh_rates();
  mt_sort(BY_THREAD_ID);
}

mdr_thread_t::~mdr_thread_t()
{
  delete mt_misc;
  if (mt_sort_array)
    delete[] mt_sort_array;
  if (net_client)
    delete net_client;

#ifdef _FREEBSD
  if (mt_array)
    delete[] mt_array;
#endif

#ifdef _AIX
  if (mt_array)
    delete[] mt_array;
  if (mps_array)
    delete[] mps_array;
#endif
}

const mdr_value_t &mdr_thread_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  mt_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_thread_t::mdr_field_value: key %s not found", key.c_str());

  switch(field_number) {
  case MT_THREAD_ID:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mt_thread_id;
    break;
  case MT_CPU_TIME:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mt_cpu_time;
    break;
  case MT_USR_TIME:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mt_user_time;
    break;
  case MT_KERNEL_TIME:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mt_kernel_time;
    break;
  case MT_USR_TIME_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mt_user_time_percent;
    break;
  case MT_KERNEL_TIME_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mt_kernel_time_percent;
    break;
  case MT_SIZE_K:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mt_size_K;
    break;
  case MT_RSSIZE_K:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mt_rssize_K;
    break;
  case MT_IO_K:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mt_io_K;
    break;
  case MT_CPU_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mt_cpu_percent;
    break;
  case MT_MEMORY_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mt_memory_percent;
    break;
  case MT_VOLUNTARY_CSW:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mt_voluntary_csw;
    break;
  case MT_INVOLUNTARY_CSW:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mt_involuntary_csw;
    break;
  case MT_DISK_FAULTS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mt_disk_faults;
    break;
  case MT_PAGE_RECLAIMS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mt_page_reclaims;
    break;
  case MT_STATE:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mt_state;
    break;
  case MT_PRIORITY:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mt_priority;
    break;
  case MT_NICE:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mt_nice;
    break;
  case MT_SIZE_DELTA_K:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mt_size_delta_K;
    break;
  case MT_RSSIZE_DELTA_K:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mt_rssize_delta_K;
    break;
  case MT_PROCESS_ID:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mt_process_id;
    break;
  }
  return value;
}

void mdr_thread_t::mdr_rates(void)
{
  mt_node_t n;
  mt_node_t nn;
  mt_can_t *cp;
  mt_can_t *ocp;
  double tmp_user_time;
  double tmp_kernel_time;
  ulong_t tmp_size_K;
  ulong_t tmp_rssize_K;
  ulong_t tmp_io_K;
  uint_t tmp_voluntary_csw;
  uint_t tmp_involuntary_csw;
  uint_t tmp_disk_faults;
  uint_t tmp_page_reclaims;
  double elapsed;
  timeval now_tod;
  double now_usec;
  double then_usec;

  gettimeofday(&now_tod, 0);
  now_usec = ((double) now_tod.tv_sec * 1000000.0) + now_tod.tv_usec;
  then_usec =
    ((double) mt_last_time.tv_sec * 1000000.0) + mt_last_time.tv_usec;
  elapsed = (now_usec - then_usec) / 1000000.0;
  mt_last_time = now_tod;

  // if less than a second then round it up
  if (elapsed < 1.0)
    elapsed = 1.0;

  for(n=begin(); n != end(); n++) {
    cp = &n->second;
    nn = old.find(n->first);
    if (nn == old.end()) {
      // should not happen
      old[n->first].clear();
      continue;
    }
    ocp = &nn->second;

    tmp_user_time = cp->mt_user_time;
    tmp_kernel_time = cp->mt_kernel_time;
    tmp_size_K = cp->mt_size_K;
    tmp_rssize_K = cp->mt_rssize_K;
    tmp_io_K = cp->mt_io_K;
    tmp_voluntary_csw = cp->mt_voluntary_csw;
    tmp_involuntary_csw = cp->mt_involuntary_csw;
    tmp_disk_faults = cp->mt_disk_faults;
    tmp_page_reclaims = cp->mt_page_reclaims;

    cp->mt_user_time -= ocp->mt_user_time;
    cp->mt_kernel_time -= ocp->mt_kernel_time;
    cp->mt_user_time_percent = (cp->mt_user_time * 100.0) / elapsed;
    cp->mt_kernel_time_percent = (cp->mt_kernel_time * 100.0) / elapsed;
    cp->mt_cpu_percent =
      cp->mt_user_time_percent + cp->mt_kernel_time_percent;
#ifdef _LP64
    cp->mt_size_delta_K = cp->mt_size_K - ocp->mt_size_K;
    cp->mt_rssize_delta_K = cp->mt_rssize_K - ocp->mt_rssize_K;
    cp->mt_io_K -= ocp->mt_io_K;
#else
    cp->mt_size_delta_K =
      wrap_diff_32(cp->mt_size_K, ocp->mt_size_K);
    cp->mt_rssize_delta_K =
      wrap_diff_32(cp->mt_rssize_K, ocp->mt_rssize_K);
    cp->mt_io_K =
      wrap_diff_32(cp->mt_io_K, ocp->mt_io_K);
#endif
    cp->mt_voluntary_csw =
      wrap_diff_32(cp->mt_voluntary_csw, ocp->mt_voluntary_csw);
    cp->mt_involuntary_csw =
      wrap_diff_32(cp->mt_involuntary_csw, ocp->mt_involuntary_csw);
    cp->mt_disk_faults =
      wrap_diff_32(cp->mt_disk_faults, ocp->mt_disk_faults);
    cp->mt_page_reclaims =
      wrap_diff_32(cp->mt_page_reclaims, ocp->mt_page_reclaims);

    ocp->mt_user_time = tmp_user_time;
    ocp->mt_kernel_time = tmp_kernel_time;
    ocp->mt_size_K = tmp_size_K;
    ocp->mt_rssize_K = tmp_rssize_K;
    ocp->mt_io_K = tmp_io_K;
    ocp->mt_voluntary_csw = tmp_voluntary_csw;
    ocp->mt_involuntary_csw = tmp_involuntary_csw;
    ocp->mt_disk_faults = tmp_disk_faults;
    ocp->mt_page_reclaims = tmp_page_reclaims;
  }

  mt_sort(mt_sort_key, mt_reverse_sort);
}

const mdr_keyset_t &mdr_thread_t::mdr_keyset(mdr_keyset_t &keyset)
{
  mt_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_thread_t::thread_remote_refresh(void)
{
  mdr_m_mm_t m_map;
  mdr_m_mm_node_t mn;
  mt_can_t can;
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
      case MT_THREAD_ID:
        can.mt_thread_id = s_map[i].mv_i32;
        break;
      case MT_CPU_TIME:
        can.mt_cpu_time = s_map[i].mv_d;
        break;
      case MT_USR_TIME:
        can.mt_user_time = s_map[i].mv_d;
        break;
      case MT_KERNEL_TIME:
        can.mt_kernel_time = s_map[i].mv_d;
        break;
      case MT_USR_TIME_PERCENT:
        can.mt_user_time_percent = s_map[i].mv_d;
        break;
      case MT_KERNEL_TIME_PERCENT:
        can.mt_kernel_time_percent = s_map[i].mv_d;
        break;
      case MT_SIZE_K:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mt_size_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mt_size_K = s_map[i].mv_ui64;
          break;
        default:
          can.mt_size_K = (ulong_t) -1;
          break;
        }
        break;
      case MT_RSSIZE_K:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mt_rssize_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mt_rssize_K = s_map[i].mv_ui64;
          break;
        default:
          can.mt_rssize_K = (ulong_t) -1;
          break;
        }
        break;
      case MT_IO_K:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mt_io_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mt_io_K = s_map[i].mv_ui64;
          break;
        default:
          can.mt_io_K = (ulong_t) -1;
          break;
        }
        break;
      case MT_CPU_PERCENT:
        can.mt_cpu_percent = s_map[i].mv_d;
        break;
      case MT_MEMORY_PERCENT:
        can.mt_memory_percent = s_map[i].mv_d;
        break;
      case MT_VOLUNTARY_CSW:
        can.mt_voluntary_csw = s_map[i].mv_ui32;
        break;
      case MT_INVOLUNTARY_CSW:
        can.mt_involuntary_csw = s_map[i].mv_ui32;
        break;
      case MT_DISK_FAULTS:
        can.mt_disk_faults = s_map[i].mv_ui32;
        break;
      case MT_PAGE_RECLAIMS:
        can.mt_page_reclaims = s_map[i].mv_ui32;
        break;
      case MT_STATE:
        can.mt_state = s_map[i].mv_i32;
        break;
      case MT_PRIORITY:
        can.mt_priority = s_map[i].mv_i32;
        break;
      case MT_NICE:
        can.mt_nice = s_map[i].mv_i32;
        break;
      case MT_SIZE_DELTA_K:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mt_size_delta_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mt_size_delta_K = s_map[i].mv_ui64;
          break;
        default:
          can.mt_size_delta_K = (ulong_t) -1;
          break;
        }
        break;
      case MT_RSSIZE_DELTA_K:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mt_rssize_delta_K = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mt_rssize_delta_K = s_map[i].mv_ui64;
          break;
        default:
          can.mt_rssize_delta_K = (ulong_t) -1;
          break;
        }
        break;
      case MT_PROCESS_ID:
        can.mt_process_id = s_map[i].mv_i32;
        break;
      default:
        break;
      }
    }

    (*this)[mn->first] = can;
  }

  net_client->mc_release(m_map);
}

extern "C" {

  static int reverse_key = 1;

  static int tid_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    return (ap->mt_thread_id - bp->mt_thread_id) * reverse_key;
  }

  static int time_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    if (ap->mt_cpu_time == bp->mt_cpu_time)
      return 0;
    if (ap->mt_cpu_time > bp->mt_cpu_time)
      return reverse_key;
    return -reverse_key;
  }

  static int utime_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    if (ap->mt_user_time_percent == bp->mt_user_time_percent)
      return 0;
    if (ap->mt_user_time_percent > bp->mt_user_time_percent)
      return reverse_key;
    return -reverse_key;
  }

  static int stime_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    if (ap->mt_kernel_time_percent == bp->mt_kernel_time_percent)
      return 0;
    if (ap->mt_kernel_time_percent > bp->mt_kernel_time_percent)
      return reverse_key;
    return -reverse_key;
  }

  static int ctime_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    if (ap->mt_cpu_percent == bp->mt_cpu_percent)
      return 0;
    if (ap->mt_cpu_percent > bp->mt_cpu_percent)
      return reverse_key;
    return -reverse_key;
  }

  static int memsz_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    if (ap->mt_memory_percent == bp->mt_memory_percent)
      return 0;
    if (ap->mt_memory_percent > bp->mt_memory_percent)
      return reverse_key;
    return -reverse_key;
  }

  static int vsize_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    return (int) ((long) ap->mt_size_K - (long) bp->mt_size_K) * reverse_key;
  }

  static int rsize_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    return
      (int) ((long) ap->mt_rssize_K - (long) bp->mt_rssize_K) * reverse_key;
  }

  static int io_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    return (int) ((long) ap->mt_io_K - (long) bp->mt_io_K) * reverse_key;
  }

  static int vctx_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    return
      ((int) ap->mt_voluntary_csw - (int) bp->mt_voluntary_csw) * reverse_key;
  }

  static int ictx_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    return ((int) ap->mt_involuntary_csw -
              (int) bp->mt_involuntary_csw) * reverse_key;
  }

  static int majf_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    return
      ((int) ap->mt_disk_faults - (int) bp->mt_disk_faults) * reverse_key;
  }

  static int minf_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    return
      ((int) ap->mt_page_reclaims - (int) bp->mt_page_reclaims) * reverse_key;
  }

  static int size_d_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    return (int)
      ((long) ap->mt_size_delta_K -
        (long) bp->mt_size_delta_K) * reverse_key;
  }

  static int rssize_d_cmp(const void *a, const void *b)
  {
    mt_can_t *ap = ((mt_can_t *) a);
    mt_can_t *bp = ((mt_can_t *) b);

    return (int)
      ((long) ap->mt_rssize_delta_K -
        (long) bp->mt_rssize_delta_K) * reverse_key;
  }

  static int (*compare[])(const void *, const void *) = {
    tid_cmp,     // BY_THREAD_ID
    time_cmp,    // BY_CPU_TIME
    utime_cmp,   // BY_USER_TIME
    stime_cmp,   // BY_KERNEL_TIME
    ctime_cmp,   // BY_CPU_USE
    memsz_cmp,   // BY_MEM_USE
    vsize_cmp,   // BY_SIZE
    rsize_cmp,   // BY_RSSIZE
    io_cmp,      // BY_IO
    vctx_cmp,    // BY_VCONTEXTS
    ictx_cmp,    // BY_ICONTEXTS
    majf_cmp,    // BY_DISK_FAULTS
    minf_cmp,    // BY_PAGE_RECLAIMS
    size_d_cmp,  // BY_SIZE_DELTA
    rssize_d_cmp // BY_RSSIZE_DELTA
  };
}

void mdr_thread_t::mt_sort(mt_sort_key_t sort_key, bool reverse)
{
  if (sort_key < BY_THREAD_ID || sort_key > BY_RSSIZE_DELTA)
    return; // less offensive than throwing an exception

  int n = size();
  iterator node;

  if (n == 0)
    return; // nothing to sort
  reverse_key = (reverse ? -1 : 1);
  if (n > mt_sort_array_max) {
    if (mt_sort_array)
      delete[] mt_sort_array;
    mt_sort_array = new mt_can_t[n];
    mt_sort_array_max = n;
  }
  for(mt_sort_array_size=0, node=begin();
      node != end(); node++, mt_sort_array_size++)
    mt_sort_array[mt_sort_array_size] = node->second;
  qsort(mt_sort_array, mt_sort_array_size,
        sizeof(mt_can_t), compare[(int) sort_key]);
  mt_sort_key = sort_key;
  mt_reverse_sort = reverse;
  mt_sort_index = 0;
}

void mdr_thread_t::mt_rewind(void)
{
  mt_sort_index = 0;
}

bool mdr_thread_t::mt_next_sorted_thread(mt_can_t &can)
{
  if (mt_sort_index == mt_sort_array_size)
    return false;
  can = mt_sort_array[mt_sort_index++];
  return true;
}

}
