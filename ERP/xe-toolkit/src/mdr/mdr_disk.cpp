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


#include "mdr_disk.h"

namespace mdr {

mdr_disk_t::mdr_disk_t(const char *remote_host) :
  mdr_t(MDR_DISK_NAME, true, MD_FIELD_COUNT, false)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_disk_t", remote_host, MD_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
    mounts = 0;
    swaps = 0;
  } else {
    mdr_support_field(MD_TYPE);
#ifndef _AIX
    mdr_support_field(MD_READS);
#endif
    mdr_support_field(MD_READ_K);
#if defined(_SOLARIS) || defined(_LINUX) || defined(_DARWIN) || defined(_AIX)
    mdr_support_field(MD_READ_MSECS);
#endif
#ifndef _AIX
    mdr_support_field(MD_WRITES);
#endif
    mdr_support_field(MD_WRITTEN_K);
#if defined(_SOLARIS) || defined(_LINUX) || defined(_DARWIN) || defined(_AIX)
    mdr_support_field(MD_WRITE_MSECS);
#endif
    mdr_support_field(MD_RUN_MSECS);
#if defined(_SOLARIS) || defined(_LINUX) || defined(_FREEBSD) || defined(_AIX)
    mdr_support_field(MD_RUNQ_MSECS);
#endif
#if defined(_SOLARIS) || defined(_LINUX) || defined(_AIX)
    mdr_support_field(MD_WAIT_MSECS);
    mdr_support_field(MD_WAITQ_MSECS);
#endif
#if defined(_SOLARIS) || defined(_LINUX) || defined(_FREEBSD) || defined(_AIX)
    mdr_support_field(MD_AVG_QUEUE_LENGTH);
#endif
    mdr_support_field(MD_UTIL_PERCENT);
    mdr_support_field(MD_SERVICE_TIME);
    mdr_support_field(MD_SPACE_MB);
    mdr_support_field(MD_IO_OPERATIONS);

    mounts = new mdr_mnt_t(remote_host);
    swaps = new mdr_pf_t(remote_host);

    net_client = 0;
  }

  mdr_field_names[MD_TYPE]             = "Disk Type";
  mdr_field_names[MD_READS]            = "Read Operations";
  mdr_field_names[MD_READ_K]           = "KBytes Read";
  mdr_field_names[MD_READ_MSECS]       = "Read Time";
  mdr_field_names[MD_WRITES]           = "Write Operations";
  mdr_field_names[MD_WRITTEN_K]        = "KBytes Written";
  mdr_field_names[MD_WRITE_MSECS]      = "Write Time";
  mdr_field_names[MD_RUN_MSECS]        = "Run Time";
  mdr_field_names[MD_WAIT_MSECS]       = "Wait Time";
  mdr_field_names[MD_RUNQ_MSECS]       = "Q-Length*Run-Time";
  mdr_field_names[MD_WAITQ_MSECS]      = "Q-Length*Wait-Time";
  mdr_field_names[MD_AVG_QUEUE_LENGTH] = "Average Request Queue Length";
  mdr_field_names[MD_UTIL_PERCENT]     = "Percent Time Busy";
  mdr_field_names[MD_SERVICE_TIME]     = "Average Access Time";
  mdr_field_names[MD_SPACE_MB]         = "Megabytes of Capacity";
  mdr_field_names[MD_IO_OPERATIONS]    = "Total Transfers";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("Disk_Category");

  mdr_refresh_rates();
}

mdr_disk_t::~mdr_disk_t()
{
  if (net_client)
    delete net_client;
  if (mounts)
    delete mounts;
  if (swaps)
    delete swaps;
}

const mdr_value_t &mdr_disk_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  md_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_disk_t::mdr_field_value: key %s not found", key.c_str());

  switch(field_number) {
  case MD_TYPE:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.md_type;
    break;
  case MD_READS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.md_reads;
    break;
  case MD_READ_K:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.md_read_K;
    break;
  case MD_READ_MSECS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.md_read_msecs;
    break;
  case MD_WRITES:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.md_writes;
    break;
  case MD_WRITTEN_K:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.md_written_K;
    break;
  case MD_WRITE_MSECS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.md_write_msecs;
    break;
  case MD_RUN_MSECS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.md_run_msecs;
    break;
  case MD_WAIT_MSECS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.md_wait_msecs;
    break;
  case MD_RUNQ_MSECS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.md_runq_msecs;
    break;
  case MD_WAITQ_MSECS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.md_waitq_msecs;
    break;
  case MD_AVG_QUEUE_LENGTH:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.md_avg_queue_length;
    break;
  case MD_UTIL_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.md_util_percent;
    break;
  case MD_SERVICE_TIME:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.md_service_time;
    break;
  case MD_SPACE_MB:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.md_space_mb;
    break;
  case MD_IO_OPERATIONS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.md_io_operations;
    break;
  }
  return value;
}

void mdr_disk_t::mdr_rates(void)
{
  md_node_t n;
  md_node_t nn;
  md_can_t *cp;
  md_can_t *ocp;
  uint_t tmp_reads;
  uint_t tmp_read_K;
  uint_t tmp_read_msecs;
  uint_t tmp_writes;
  uint_t tmp_written_K;
  uint_t tmp_write_msecs;
  uint_t tmp_run_msecs;
  uint_t tmp_wait_msecs;
  uint_t tmp_runq_msecs;
  uint_t tmp_waitq_msecs;
  uint_t tmp_io_operations;
  double tmp_avg_queue_length;

  for(n=begin(); n != end(); n++) {
    cp = &n->second;
    nn = old.find(n->first);
    if (nn == old.end()) {
      // should not happen
      old[n->first].clear();
      continue;
    }
    ocp = &nn->second;

    tmp_reads = cp->md_reads;
    tmp_read_K = cp->md_read_K;
    tmp_read_msecs = cp->md_read_msecs;
    tmp_writes = cp->md_writes;
    tmp_written_K = cp->md_written_K;
    tmp_write_msecs = cp->md_write_msecs;
    tmp_run_msecs = cp->md_run_msecs;
    tmp_wait_msecs = cp->md_wait_msecs;
    tmp_runq_msecs = cp->md_runq_msecs;
    tmp_waitq_msecs = cp->md_waitq_msecs;
    tmp_io_operations = cp->md_io_operations;
    tmp_avg_queue_length = cp->md_avg_queue_length;

    cp->md_reads =
      wrap_diff_32(cp->md_reads, ocp->md_reads);
    cp->md_read_K =
      wrap_diff_32(cp->md_read_K, ocp->md_read_K);
    cp->md_read_msecs =
      wrap_diff_32(cp->md_read_msecs, ocp->md_read_msecs);
    cp->md_writes =
      wrap_diff_32(cp->md_writes, ocp->md_writes);
    cp->md_written_K =
      wrap_diff_32(cp->md_written_K, ocp->md_written_K);
    cp->md_write_msecs =
      wrap_diff_32(cp->md_write_msecs, ocp->md_write_msecs);
    cp->md_run_msecs =
      wrap_diff_32(cp->md_run_msecs, ocp->md_run_msecs);
    cp->md_wait_msecs =
      wrap_diff_32(cp->md_wait_msecs, ocp->md_wait_msecs);
    cp->md_runq_msecs =
      wrap_diff_32(cp->md_runq_msecs, ocp->md_runq_msecs);
    cp->md_waitq_msecs =
      wrap_diff_32(cp->md_waitq_msecs, ocp->md_waitq_msecs);
    cp->md_io_operations =
      wrap_diff_32(cp->md_io_operations, ocp->md_io_operations);
    cp->md_avg_queue_length -= ocp->md_avg_queue_length;

    ocp->md_reads = tmp_reads;
    ocp->md_read_K = tmp_read_K;
    ocp->md_read_msecs = tmp_read_msecs;
    ocp->md_writes = tmp_writes;
    ocp->md_written_K = tmp_written_K;
    ocp->md_write_msecs = tmp_write_msecs;
    ocp->md_run_msecs = tmp_run_msecs;
    ocp->md_wait_msecs = tmp_wait_msecs;
    ocp->md_runq_msecs = tmp_runq_msecs;
    ocp->md_waitq_msecs = tmp_waitq_msecs;
    ocp->md_io_operations = tmp_io_operations;
    ocp->md_avg_queue_length = tmp_avg_queue_length;
  }
}

const mdr_keyset_t &mdr_disk_t::mdr_keyset(mdr_keyset_t &keyset)
{
  md_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_disk_t::disk_remote_refresh(void)
{
  mdr_m_mm_t m_map;
  mdr_m_mm_node_t mn;
  md_can_t can;
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
      case MD_TYPE:
        can.md_type = *s_map[i].mv_s;
        break;
      case MD_READS:
        can.md_reads = s_map[i].mv_ui32;
        break;
      case MD_READ_K:
        can.md_read_K = s_map[i].mv_ui32;
        break;
      case MD_READ_MSECS:
        can.md_read_msecs = s_map[i].mv_ui32;
        break;
      case MD_WRITES:
        can.md_writes = s_map[i].mv_ui32;
        break;
      case MD_WRITTEN_K:
        can.md_written_K = s_map[i].mv_ui32;
        break;
      case MD_WRITE_MSECS:
        can.md_write_msecs = s_map[i].mv_ui32;
        break;
      case MD_RUN_MSECS:
        can.md_run_msecs = s_map[i].mv_ui32;
        break;
      case MD_WAIT_MSECS:
        can.md_wait_msecs = s_map[i].mv_ui32;
        break;
      case MD_RUNQ_MSECS:
        can.md_runq_msecs = s_map[i].mv_ui32;
        break;
      case MD_WAITQ_MSECS:
        can.md_waitq_msecs = s_map[i].mv_ui32;
        break;
      case MD_AVG_QUEUE_LENGTH:
        can.md_avg_queue_length = s_map[i].mv_d;
        break;
      case MD_UTIL_PERCENT:
        can.md_util_percent = s_map[i].mv_d;
        break;
      case MD_SERVICE_TIME:
        can.md_service_time = s_map[i].mv_d;
        break;
      case MD_SPACE_MB:
        can.md_space_mb = s_map[i].mv_ui32;
        break;
      default:
        break;
      }
    }

    (*this)[mn->first] = can;
  }

  net_client->mc_release(m_map);
}

}
