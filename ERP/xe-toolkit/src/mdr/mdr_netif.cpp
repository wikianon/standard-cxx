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


#include "mdr_netif.h"

namespace mdr {

mdr_netif_t::mdr_netif_t(const char *remote_host) :
  mdr_t(MDR_NETIF_NAME, true, MNET_FIELD_COUNT, false)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_netif_t", remote_host, MNET_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MNET_IP_ADDRESS);
    mdr_support_field(MNET_NETMASK);
    mdr_support_field(MNET_BROADCAST_ADDRESS);
    mdr_support_field(MNET_MTU);
    mdr_support_field(MNET_INPUT_BYTES);
    mdr_support_field(MNET_OUTPUT_BYTES);
    mdr_support_field(MNET_INPUT_PACKETS);
    mdr_support_field(MNET_OUTPUT_PACKETS);
    mdr_support_field(MNET_INPUT_ERRORS);
    mdr_support_field(MNET_OUTPUT_ERRORS);
    mdr_support_field(MNET_INPUT_FAILURES);
#if !defined(_DARWIN)
    mdr_support_field(MNET_OUTPUT_FAILURES);
#endif
    mdr_support_field(MNET_COLLISIONS);
#if defined(_SOLARIS) || defined(_DARWIN) || defined(_FREEBSD) || defined(_AIX)
    mdr_support_field(MNET_INTERFACE_SPEED);
#endif
    mdr_support_field(MNET_COLLISION_PERCENT);
#if defined(_SOLARIS)
    mdr_support_field(MNET_FULL_DUPLEX);
#endif

    net_client = 0;
  }

  mdr_field_names[MNET_IP_ADDRESS]        = "IP Address";
  mdr_field_names[MNET_NETMASK]           = "Network Mask";
  mdr_field_names[MNET_BROADCAST_ADDRESS] = "Broadcast Address";
  mdr_field_names[MNET_MTU]               = "Maximum Transfer Unit";
  mdr_field_names[MNET_INPUT_BYTES]       = "Bytes Received";
  mdr_field_names[MNET_OUTPUT_BYTES]      = "Bytes Transmitted";
  mdr_field_names[MNET_INPUT_PACKETS]     = "Packets Received";
  mdr_field_names[MNET_OUTPUT_PACKETS]    = "Packets Transmitted";
  mdr_field_names[MNET_INPUT_ERRORS]      = "Errors on Receipt";
  mdr_field_names[MNET_OUTPUT_ERRORS]     = "Errors on Transmission";
  mdr_field_names[MNET_INPUT_FAILURES]    = "Failures on Receipt";
  mdr_field_names[MNET_OUTPUT_FAILURES]   = "Failures on Transmission";
  mdr_field_names[MNET_COLLISIONS]        = "Collisions on Transmission";
  mdr_field_names[MNET_INTERFACE_SPEED]   = "Interface Speed (b/s)";
  mdr_field_names[MNET_COLLISION_PERCENT] = "Collision Percent";
  mdr_field_names[MNET_FULL_DUPLEX]       = "Interface is Full Duplex";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("Netif_Category");

  mdr_refresh_rates();
}

mdr_netif_t::~mdr_netif_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_netif_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  mnet_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_netif_t::mdr_field_value: key %s not found", key.c_str());

  switch(field_number) {
  case MNET_IP_ADDRESS:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mnet_ip_address;
    break;
  case MNET_NETMASK:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mnet_netmask;
    break;
  case MNET_BROADCAST_ADDRESS:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mnet_broadcast_address;
    break;
  case MNET_MTU:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mnet_mtu;
    break;
  case MNET_INPUT_BYTES:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mnet_input_bytes;
    break;
  case MNET_OUTPUT_BYTES:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mnet_output_bytes;
    break;
  case MNET_INPUT_PACKETS:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mnet_input_packets;
    break;
  case MNET_OUTPUT_PACKETS:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mnet_output_packets;
    break;
  case MNET_INPUT_ERRORS:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mnet_input_errors;
    break;
  case MNET_OUTPUT_ERRORS:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mnet_output_errors;
    break;
  case MNET_INPUT_FAILURES:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mnet_input_failures;
    break;
  case MNET_OUTPUT_FAILURES:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mnet_output_failures;
    break;
  case MNET_COLLISIONS:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mnet_collisions;
    break;
  case MNET_INTERFACE_SPEED:
    value.mv_type = MVT_ULONG;
    value.mv_ul = n->second.mnet_interface_speed;
    break;
  case MNET_COLLISION_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mnet_collision_percent;
    break;
  case MNET_FULL_DUPLEX:
    value.mv_type = MVT_BOOL;
    value.mv_b = n->second.mnet_full_duplex;
    break;
  }
  return value;
}

void mdr_netif_t::mdr_rates(void)
{
  mnet_node_t n;
  mnet_node_t nn;
  mnet_can_t *cp;
  mnet_can_t *ocp;
  ulong_t tmp_input_bytes;
  ulong_t tmp_output_bytes;
  ulong_t tmp_input_packets;
  ulong_t tmp_output_packets;
  ulong_t tmp_input_errors;
  ulong_t tmp_output_errors;
  ulong_t tmp_input_failures;
  ulong_t tmp_output_failures;
  ulong_t tmp_collisions;

  for(n=begin(); n != end(); n++) {
    cp = &n->second;
    nn = old.find(n->first);
    if (nn == old.end()) {
      // should not happen
      old[n->first].clear();
      continue;
    }
    ocp = &nn->second;

    tmp_input_bytes = cp->mnet_input_bytes;
    tmp_output_bytes = cp->mnet_output_bytes;
    tmp_input_packets = cp->mnet_input_packets;
    tmp_output_packets = cp->mnet_output_packets;
    tmp_input_errors = cp->mnet_input_errors;
    tmp_output_errors = cp->mnet_output_errors;
    tmp_input_failures = cp->mnet_input_failures;
    tmp_output_failures = cp->mnet_output_failures;
    tmp_collisions = cp->mnet_collisions;

#ifdef _LP64
    cp->mnet_input_bytes     -= ocp->mnet_input_bytes;
    cp->mnet_output_bytes    -= ocp->mnet_output_bytes;
    cp->mnet_input_packets   -= ocp->mnet_input_packets;
    cp->mnet_output_packets  -= ocp->mnet_output_packets;
    cp->mnet_input_errors    -= ocp->mnet_input_errors;
    cp->mnet_output_errors   -= ocp->mnet_output_errors;
    cp->mnet_input_failures  -= ocp->mnet_input_failures;
    cp->mnet_output_failures -= ocp->mnet_output_failures;
    cp->mnet_collisions      -= ocp->mnet_collisions;
#else
    cp->mnet_input_bytes     =
      wrap_diff_32(cp->mnet_input_bytes, ocp->mnet_input_bytes);
    cp->mnet_output_bytes    =
      wrap_diff_32(cp->mnet_output_bytes, ocp->mnet_output_bytes);
    cp->mnet_input_packets   =
      wrap_diff_32(cp->mnet_input_packets, ocp->mnet_input_packets);
    cp->mnet_output_packets  =
      wrap_diff_32(cp->mnet_output_packets, ocp->mnet_output_packets);
    cp->mnet_input_errors    =
      wrap_diff_32(cp->mnet_input_errors, ocp->mnet_input_errors);
    cp->mnet_output_errors   =
      wrap_diff_32(cp->mnet_output_errors, ocp->mnet_output_errors);
    cp->mnet_input_failures  =
      wrap_diff_32(cp->mnet_input_failures, ocp->mnet_input_failures);
    cp->mnet_output_failures =
      wrap_diff_32(cp->mnet_output_failures, ocp->mnet_output_failures);
    cp->mnet_collisions      =
      wrap_diff_32(cp->mnet_collisions, ocp->mnet_collisions);
#endif
    if (cp->mnet_output_packets > 0)
      cp->mnet_collision_percent =
        (100.0 * cp->mnet_collisions) / cp->mnet_output_packets;
    else
      cp->mnet_collision_percent = 0.0;

    ocp->mnet_input_bytes = tmp_input_bytes;
    ocp->mnet_output_bytes = tmp_output_bytes;
    ocp->mnet_input_packets = tmp_input_packets;
    ocp->mnet_output_packets = tmp_output_packets;
    ocp->mnet_input_errors = tmp_input_errors;
    ocp->mnet_output_errors = tmp_output_errors;
    ocp->mnet_input_failures = tmp_input_failures;
    ocp->mnet_output_failures = tmp_output_failures;
    ocp->mnet_collisions = tmp_collisions;
  }
}

const mdr_keyset_t &mdr_netif_t::mdr_keyset(mdr_keyset_t &keyset)
{
  mnet_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_netif_t::netif_remote_refresh(void)
{
  mdr_m_mm_t m_map;
  mdr_m_mm_node_t mn;
  mnet_can_t can;
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
      case MNET_IP_ADDRESS:
        can.mnet_ip_address = *s_map[i].mv_s;
        break;
      case MNET_NETMASK:
        can.mnet_netmask = *s_map[i].mv_s;
        break;
      case MNET_BROADCAST_ADDRESS:
        can.mnet_broadcast_address = *s_map[i].mv_s;
        break;
      case MNET_MTU:
        can.mnet_mtu = s_map[i].mv_i32;
        break;
      case MNET_INPUT_BYTES:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mnet_input_bytes = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mnet_input_bytes = s_map[i].mv_ui64;
          break;
        default:
          can.mnet_input_bytes = (ulong_t) -1;
          break;
        }
        break;
      case MNET_OUTPUT_BYTES:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mnet_output_bytes = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mnet_output_bytes = s_map[i].mv_ui64;
          break;
        default:
          can.mnet_output_bytes = (ulong_t) -1;
          break;
        }
        break;
      case MNET_INPUT_PACKETS:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mnet_input_packets = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mnet_input_packets = s_map[i].mv_ui64;
          break;
        default:
          can.mnet_input_packets = (ulong_t) -1;
          break;
        }
        break;
      case MNET_OUTPUT_PACKETS:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mnet_output_packets = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mnet_output_packets = s_map[i].mv_ui64;
          break;
        default:
          can.mnet_output_packets = (ulong_t) -1;
          break;
        }
        break;
      case MNET_INPUT_ERRORS:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mnet_input_errors = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mnet_input_errors = s_map[i].mv_ui64;
          break;
        default:
          can.mnet_input_errors = (ulong_t) -1;
          break;
        }
        break;
      case MNET_OUTPUT_ERRORS:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mnet_output_errors = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mnet_output_errors = s_map[i].mv_ui64;
          break;
        default:
          can.mnet_output_errors = (ulong_t) -1;
          break;
        }
        break;
      case MNET_INPUT_FAILURES:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mnet_input_failures = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mnet_input_failures = s_map[i].mv_ui64;
          break;
        default:
          can.mnet_input_failures = (ulong_t) -1;
          break;
        }
        break;
      case MNET_OUTPUT_FAILURES:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mnet_output_failures = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mnet_output_failures = s_map[i].mv_ui64;
          break;
        default:
          can.mnet_output_failures = (ulong_t) -1;
          break;
        }
        break;
      case MNET_COLLISIONS:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mnet_collisions = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mnet_collisions = s_map[i].mv_ui64;
          break;
        default:
          can.mnet_collisions = (ulong_t) -1;
          break;
        }
        break;
      case MNET_INTERFACE_SPEED:
        switch(s_map[i].mv_type) {
        case MVT_UI32:
          can.mnet_interface_speed = s_map[i].mv_ui32;
          break;
        case MVT_UI64:
          can.mnet_interface_speed = s_map[i].mv_ui64;
          break;
        default:
          can.mnet_interface_speed = (ulong_t) -1;
          break;
        }
        break;
      case MNET_COLLISION_PERCENT:
        can.mnet_collision_percent = s_map[i].mv_d;
        break;
      case MNET_FULL_DUPLEX:
        can.mnet_full_duplex = s_map[i].mv_b;
        break;
      default:
        break;
      }
    }

    (*this)[mn->first] = can;
  }

  net_client->mc_release(m_map);
}

void mdr_netif_t::mnet_duplex(std::string &name, bool is_full)
{
  mnet_node_t n = find(name);

  if (n != end())
    n->second.mnet_full_duplex = is_full;
}

}
