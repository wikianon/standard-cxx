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


#include "mdr_netstat.h"

namespace mdr {

mdr_netstat_t::mdr_netstat_t(const char *remote_host) :
  mdr_t(MDR_NETSTAT_NAME, true, MNS_FIELD_COUNT, false)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_netstat_t", remote_host, MNS_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MNS_INPUT_BYTES);
    mdr_support_field(MNS_OUTPUT_BYTES);
    mdr_support_field(MNS_INPUT_PACKETS);
    mdr_support_field(MNS_OUTPUT_PACKETS);
    mdr_support_field(MNS_INPUT_ERRORS);
    mdr_support_field(MNS_OUTPUT_ERRORS);
    mdr_support_field(MNS_INPUT_FAILURES);
    mdr_support_field(MNS_OUTPUT_FAILURES);
    mdr_support_field(MNS_COLLISIONS);
    mdr_support_field(MNS_COLLISION_PERCENT);

    net_client = 0;
  }

  mdr_field_names[MNS_INPUT_BYTES]       = "Bytes Received/sec";
  mdr_field_names[MNS_OUTPUT_BYTES]      = "Bytes Transmitted/sec";
  mdr_field_names[MNS_INPUT_PACKETS]     = "Packets Received/sec";
  mdr_field_names[MNS_OUTPUT_PACKETS]    = "Packets Transmitted/sec";
  mdr_field_names[MNS_INPUT_ERRORS]      = "Errors on Receipt/sec";
  mdr_field_names[MNS_OUTPUT_ERRORS]     = "Errors on Transmission/sec";
  mdr_field_names[MNS_INPUT_FAILURES]    = "Failures on Receipt/sec";
  mdr_field_names[MNS_OUTPUT_FAILURES]   = "Failures on Transmission/sec";
  mdr_field_names[MNS_COLLISIONS]        = "Collisions on Transmission/sec";
  mdr_field_names[MNS_COLLISION_PERCENT] = "Collision Percent";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("NetStat_Category");

  misc = new mdr_misc_t(remote_host);
  netif = new mdr_netif_t(remote_host);

  last_time.tv_sec = misc->mm_boot_time;
  last_time.tv_usec = 0;

  mns_recompute();
}

mdr_netstat_t::~mdr_netstat_t()
{
  if (net_client)
    delete net_client;
  if (misc)
    delete misc;
  if (netif)
    delete netif;
}

void mdr_netstat_t::mdr_refresh(void)
{
  netif->mdr_refresh_rates();
  mns_recompute();
}

void mdr_netstat_t::mns_recompute(void)
{
  double elapsed;
  double now_usec;
  double then_usec;
  mns_can_t can;
  timeval now_tod;

  gettimeofday(&now_tod, 0);
  now_usec = ((double) now_tod.tv_sec * 1000000.0) + now_tod.tv_usec;
  then_usec =
    ((double) last_time.tv_sec * 1000000.0) + last_time.tv_usec;
  elapsed = (now_usec - then_usec) / 1000000.0;
  last_time = now_tod;

  // no divide-by-zero please
  if (elapsed == 0.0)
    elapsed = 1.0;

  clear();
  for(mnet_node_t n=netif->begin(); n != netif->end(); n++) {
    const std::string &name = n->first;
    mnet_can_t *cp = &n->second;

    can.clear();

    can.mns_input_bytes = cp->mnet_input_bytes / elapsed;
    can.mns_output_bytes = cp->mnet_output_bytes / elapsed;
    can.mns_input_packets = cp->mnet_input_packets / elapsed;
    can.mns_output_packets = cp->mnet_output_packets / elapsed;
    can.mns_input_errors = cp->mnet_input_errors / elapsed;
    can.mns_output_errors = cp->mnet_output_errors / elapsed;
    can.mns_input_failures = cp->mnet_input_failures / elapsed;
    can.mns_output_failures = cp->mnet_output_failures / elapsed;
    can.mns_collisions = cp->mnet_collisions / elapsed;
    can.mns_collision_percent = cp->mnet_collision_percent;

    (*this)[name] = can;
  }
}

const mdr_value_t &mdr_netstat_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  mns_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_netstat_t::mdr_field_value: key %s not found", key.c_str());

  switch(field_number) {
  case MNS_INPUT_BYTES:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mns_input_bytes;
    break;
  case MNS_OUTPUT_BYTES:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mns_output_bytes;
    break;
  case MNS_INPUT_PACKETS:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mns_input_packets;
    break;
  case MNS_OUTPUT_PACKETS:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mns_output_packets;
    break;
  case MNS_INPUT_ERRORS:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mns_input_errors;
    break;
  case MNS_OUTPUT_ERRORS:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mns_output_errors;
    break;
  case MNS_INPUT_FAILURES:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mns_input_failures;
    break;
  case MNS_OUTPUT_FAILURES:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mns_output_failures;
    break;
  case MNS_COLLISIONS:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mns_collisions;
    break;
  case MNS_COLLISION_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mns_collision_percent;
    break;
  }
  return value;
}

const mdr_keyset_t &mdr_netstat_t::mdr_keyset(mdr_keyset_t &keyset)
{
  mns_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_netstat_t::netstat_remote_refresh(void)
{
  mdr_m_mm_t m_map;
  mdr_m_mm_node_t mn;
  mns_can_t can;
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
      case MNS_INPUT_BYTES:
        can.mns_input_bytes = s_map[i].mv_d;
        break;
      case MNS_OUTPUT_BYTES:
        can.mns_output_bytes = s_map[i].mv_d;
        break;
      case MNS_INPUT_PACKETS:
        can.mns_input_packets = s_map[i].mv_d;
        break;
      case MNS_OUTPUT_PACKETS:
        can.mns_output_packets = s_map[i].mv_d;
        break;
      case MNS_INPUT_ERRORS:
        can.mns_input_errors = s_map[i].mv_d;
        break;
      case MNS_OUTPUT_ERRORS:
        can.mns_output_errors = s_map[i].mv_d;
        break;
      case MNS_INPUT_FAILURES:
        can.mns_input_failures = s_map[i].mv_d;
        break;
      case MNS_OUTPUT_FAILURES:
        can.mns_output_failures = s_map[i].mv_d;
        break;
      case MNS_COLLISIONS:
        can.mns_collisions = s_map[i].mv_d;
        break;
      case MNS_COLLISION_PERCENT:
        can.mns_collision_percent = s_map[i].mv_d;
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
