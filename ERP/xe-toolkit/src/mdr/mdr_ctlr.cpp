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


#include "mdr_ctlr.h"

namespace mdr {

mdr_ctlr_t::mdr_ctlr_t(const char *remote_host) :
  mdr_t(MDR_CTLR_NAME, true, MCTL_FIELD_COUNT, false)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_ctlr_t", remote_host, MCTL_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
    disk = 0;
  } else {
#if defined(_DARWIN)
    mdr_category_supported = false;
#else
# ifndef _AIX
    mdr_support_field(MCTL_READS);
# endif
    mdr_support_field(MCTL_READ_K);
# if defined(_SOLARIS) || defined(_LINUX) || defined(_DARWIN) || defined(_AIX)
    mdr_support_field(MCTL_READ_MSECS);
# endif
# ifndef _AIX
    mdr_support_field(MCTL_WRITES);
# endif
    mdr_support_field(MCTL_WRITTEN_K);
# if defined(_SOLARIS) || defined(_LINUX) || defined(_DARWIN) || defined(_AIX)
    mdr_support_field(MCTL_WRITE_MSECS);
# endif
    mdr_support_field(MCTL_RUN_MSECS);
# if defined(_SOLARIS) || defined(_LINUX) || defined(_FREEBSD) || defined(_AIX)
    mdr_support_field(MCTL_RUNQ_MSECS);
# endif
# if defined(_SOLARIS) || defined(_LINUX) || defined(_AIX)
    mdr_support_field(MCTL_WAIT_MSECS);
    mdr_support_field(MCTL_WAITQ_MSECS);
# endif
# if defined(_SOLARIS) || defined(_LINUX) || defined(_FREEBSD) || defined(_AIX)
    mdr_support_field(MCTL_AVG_QUEUE_LENGTH);
# endif
    mdr_support_field(MCTL_UTIL_PERCENT);
    mdr_support_field(MCTL_SERVICE_TIME);
    mdr_support_field(MCTL_DISK_COUNT);
    mdr_support_field(MCTL_IO_OPERATIONS);
#endif

    net_client = 0;
    disk = new mdr_disk_t(remote_host);
  }

  mdr_field_names[MCTL_READS]            = "Read Operations";
  mdr_field_names[MCTL_READ_K]           = "KBytes Read";
  mdr_field_names[MCTL_READ_MSECS]       = "Read Time";
  mdr_field_names[MCTL_WRITES]           = "Write Operations";
  mdr_field_names[MCTL_WRITTEN_K]        = "KBytes Written";
  mdr_field_names[MCTL_WRITE_MSECS]      = "Write Time";
  mdr_field_names[MCTL_RUN_MSECS]        = "Run Time";
  mdr_field_names[MCTL_WAIT_MSECS]       = "Wait Time";
  mdr_field_names[MCTL_RUNQ_MSECS]       = "Q-Length*Run-Time";
  mdr_field_names[MCTL_WAITQ_MSECS]      = "Q-Length*Wait-Time";
  mdr_field_names[MCTL_AVG_QUEUE_LENGTH] = "Average Request Queue Length";
  mdr_field_names[MCTL_UTIL_PERCENT]     = "Percent Time Busy";
  mdr_field_names[MCTL_SERVICE_TIME]     = "Average Access Time";
  mdr_field_names[MCTL_DISK_COUNT]       = "Disks on Controller";
  mdr_field_names[MCTL_IO_OPERATIONS]    = "Total Transfers";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("Ctlr_Category");

  mdr_refresh_rates();
}

mdr_ctlr_t::~mdr_ctlr_t()
{
  if (net_client)
    delete net_client;
  if (disk)
    delete disk;
}

const mdr_value_t &mdr_ctlr_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  mctl_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_ctlr_t::mdr_field_value: key %s not found", key.c_str());

  switch(field_number) {
  case MCTL_READS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mctl_reads;
    break;
  case MCTL_READ_K:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mctl_read_K;
    break;
  case MCTL_READ_MSECS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mctl_read_msecs;
    break;
  case MCTL_WRITES:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mctl_writes;
    break;
  case MCTL_WRITTEN_K:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mctl_written_K;
    break;
  case MCTL_WRITE_MSECS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mctl_write_msecs;
    break;
  case MCTL_RUN_MSECS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mctl_run_msecs;
    break;
  case MCTL_WAIT_MSECS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mctl_wait_msecs;
    break;
  case MCTL_RUNQ_MSECS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mctl_runq_msecs;
    break;
  case MCTL_WAITQ_MSECS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mctl_waitq_msecs;
    break;
  case MCTL_AVG_QUEUE_LENGTH:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mctl_avg_queue_length;
    break;
  case MCTL_UTIL_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mctl_util_percent;
    break;
  case MCTL_SERVICE_TIME:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mctl_service_time;
    break;
  case MCTL_DISK_COUNT:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mctl_disk_count;
    break;
  case MCTL_IO_OPERATIONS:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mctl_io_operations;
    break;
  }
  return value;
}

void mdr_ctlr_t::mdr_rates(void)
{
  mctl_node_t n;
  mctl_node_t nn;
  mctl_can_t *cp;
  mctl_can_t *ocp;
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

    tmp_reads = cp->mctl_reads;
    tmp_read_K = cp->mctl_read_K;
    tmp_read_msecs = cp->mctl_read_msecs;
    tmp_writes = cp->mctl_writes;
    tmp_written_K = cp->mctl_written_K;
    tmp_write_msecs = cp->mctl_write_msecs;
    tmp_run_msecs = cp->mctl_run_msecs;
    tmp_wait_msecs = cp->mctl_wait_msecs;
    tmp_runq_msecs = cp->mctl_runq_msecs;
    tmp_waitq_msecs = cp->mctl_waitq_msecs;
    tmp_avg_queue_length = cp->mctl_avg_queue_length;
    tmp_io_operations = cp->mctl_io_operations;

    cp->mctl_reads =
      wrap_diff_32(cp->mctl_reads, ocp->mctl_reads);
    cp->mctl_read_K =
      wrap_diff_32(cp->mctl_read_K, ocp->mctl_read_K);
    cp->mctl_read_msecs =
      wrap_diff_32(cp->mctl_read_msecs, ocp->mctl_read_msecs);
    cp->mctl_writes =
      wrap_diff_32(cp->mctl_writes, ocp->mctl_writes);
    cp->mctl_written_K =
      wrap_diff_32(cp->mctl_written_K, ocp->mctl_written_K);
    cp->mctl_write_msecs =
      wrap_diff_32(cp->mctl_write_msecs, ocp->mctl_write_msecs);
    cp->mctl_run_msecs =
      wrap_diff_32(cp->mctl_run_msecs, ocp->mctl_run_msecs);
    cp->mctl_wait_msecs =
      wrap_diff_32(cp->mctl_wait_msecs, ocp->mctl_wait_msecs);
    cp->mctl_runq_msecs =
      wrap_diff_32(cp->mctl_runq_msecs, ocp->mctl_runq_msecs);
    cp->mctl_waitq_msecs =
      wrap_diff_32(cp->mctl_waitq_msecs, ocp->mctl_waitq_msecs);
    cp->mctl_avg_queue_length -= ocp->mctl_avg_queue_length;
    cp->mctl_io_operations =
      wrap_diff_32(cp->mctl_io_operations, ocp->mctl_io_operations);

    ocp->mctl_reads = tmp_reads;
    ocp->mctl_read_K = tmp_read_K;
    ocp->mctl_read_msecs = tmp_read_msecs;
    ocp->mctl_writes = tmp_writes;
    ocp->mctl_written_K = tmp_written_K;
    ocp->mctl_write_msecs = tmp_write_msecs;
    ocp->mctl_run_msecs = tmp_run_msecs;
    ocp->mctl_wait_msecs = tmp_wait_msecs;
    ocp->mctl_runq_msecs = tmp_runq_msecs;
    ocp->mctl_waitq_msecs = tmp_waitq_msecs;
    ocp->mctl_avg_queue_length = tmp_avg_queue_length;
    ocp->mctl_io_operations = tmp_io_operations;
  }
}

const mdr_keyset_t &mdr_ctlr_t::mdr_keyset(mdr_keyset_t &keyset)
{
  mctl_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_ctlr_t::ctlr_remote_refresh(void)
{
  mdr_m_mm_t m_map;
  mdr_m_mm_node_t mn;
  mctl_can_t can;
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
      case MCTL_READS:
        can.mctl_reads = s_map[i].mv_ui32;
        break;
      case MCTL_READ_K:
        can.mctl_read_K = s_map[i].mv_ui32;
        break;
      case MCTL_READ_MSECS:
        can.mctl_read_msecs = s_map[i].mv_ui32;
        break;
      case MCTL_WRITES:
        can.mctl_writes = s_map[i].mv_ui32;
        break;
      case MCTL_WRITTEN_K:
        can.mctl_written_K = s_map[i].mv_ui32;
        break;
      case MCTL_WRITE_MSECS:
        can.mctl_write_msecs = s_map[i].mv_ui32;
        break;
      case MCTL_RUN_MSECS:
        can.mctl_run_msecs = s_map[i].mv_ui32;
        break;
      case MCTL_WAIT_MSECS:
        can.mctl_wait_msecs = s_map[i].mv_ui32;
        break;
      case MCTL_RUNQ_MSECS:
        can.mctl_runq_msecs = s_map[i].mv_ui32;
        break;
      case MCTL_WAITQ_MSECS:
        can.mctl_waitq_msecs = s_map[i].mv_ui32;
        break;
      case MCTL_AVG_QUEUE_LENGTH:
        can.mctl_avg_queue_length = s_map[i].mv_d;
        break;
      case MCTL_UTIL_PERCENT:
        can.mctl_util_percent = s_map[i].mv_d;
        break;
      case MCTL_SERVICE_TIME:
        can.mctl_service_time = s_map[i].mv_d;
        break;
      case MCTL_DISK_COUNT:
        can.mctl_disk_count = s_map[i].mv_i32;
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
