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


#include "mdr_ip.h"

namespace mdr {

mdr_ip_t::mdr_ip_t(const char *remote_host) :
  mdr_t(MDR_IP_NAME, true, MIP_FIELD_COUNT, true)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_ip_t", remote_host, MIP_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MIP_IPFORWARDING);
    mdr_support_field(MIP_IPDEFAULTTTL);
    mdr_support_field(MIP_IPINRECEIVES);
    mdr_support_field(MIP_IPINHDRERRORS);
    mdr_support_field(MIP_IPINADDRERRORS);
    mdr_support_field(MIP_IPFORWDATAGRAMS);
    mdr_support_field(MIP_IPINUNKNOWNPROTOS);
    mdr_support_field(MIP_IPINDISCARDS);
    mdr_support_field(MIP_IPINDELIVERS);
    mdr_support_field(MIP_IPOUTREQUESTS);
    mdr_support_field(MIP_IPOUTDISCARDS);
#if defined(_SOLARIS) || defined(_LINUX) || defined(_AIX)
    mdr_support_field(MIP_IPOUTNOROUTES);
#endif
    mdr_support_field(MIP_IPREASMTIMEOUT);
    mdr_support_field(MIP_IPREASMREQDS);
    mdr_support_field(MIP_IPREASMOKS);
    mdr_support_field(MIP_IPREASMFAILS);
    mdr_support_field(MIP_IPFRAGOKS);
    mdr_support_field(MIP_IPFRAGFAILS);
    mdr_support_field(MIP_IPFRAGCREATES);
#if defined(_SOLARIS) || defined(_DARWIN) || defined(_FREEBSD) || defined(_AIX)
    mdr_support_field(MIP_IPROUTINGDISCARDS);
#endif

    net_client = 0;
  }

  mdr_field_names[MIP_IPFORWARDING]      = "Forwarder";
  mdr_field_names[MIP_IPDEFAULTTTL]      = "Time-to-Live";
  mdr_field_names[MIP_IPINRECEIVES]      = "Datagrams Received";
  mdr_field_names[MIP_IPINHDRERRORS]     = "In Discards from Header Errors";
  mdr_field_names[MIP_IPINADDRERRORS]    = "In Discards from Address Errors";
  mdr_field_names[MIP_IPFORWDATAGRAMS]   = "Datagrams Forwarded";
  mdr_field_names[MIP_IPINUNKNOWNPROTOS] = "In Discards from Unknown Protocol";
  mdr_field_names[MIP_IPINDISCARDS]      = "In Discards of Good Datagrams";
  mdr_field_names[MIP_IPINDELIVERS]      = "Datagrams Sent Upstream";
  mdr_field_names[MIP_IPOUTREQUESTS]     = "Upstream Output Requests";
  mdr_field_names[MIP_IPOUTDISCARDS]     = "Out Discards of Good Datagrams";
  mdr_field_names[MIP_IPOUTNOROUTES]     = "Out Discards from No Route";
  mdr_field_names[MIP_IPREASMTIMEOUT]    = "Fragment Reassemble Timeout";
  mdr_field_names[MIP_IPREASMREQDS]      = "Fragment Reassemblies Required";
  mdr_field_names[MIP_IPREASMOKS]        = "Fragments Reassembled";
  mdr_field_names[MIP_IPREASMFAILS]      = "Fragment Reassembly Failures";
  mdr_field_names[MIP_IPFRAGOKS]         = "Datagrams Fragmented";
  mdr_field_names[MIP_IPFRAGFAILS]       = "Fragment Discards from No-Fragment Flag";
  mdr_field_names[MIP_IPFRAGCREATES]     = "Fragments Created";
  mdr_field_names[MIP_IPROUTINGDISCARDS] = "Routing Entries Discarded";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("IP_Category");

  mip_ipForwarding = 0;
  mip_ipDefaultTTL = 0;
  mip_ipInReceives = 0;
  mip_ipInHdrErrors = 0;
  mip_ipInAddrErrors = 0;
  mip_ipForwDatagrams = 0;
  mip_ipInUnknownProtos = 0;
  mip_ipInDiscards = 0;
  mip_ipInDelivers = 0;
  mip_ipOutRequests = 0;
  mip_ipOutDiscards = 0;
  mip_ipOutNoRoutes = 0;
  mip_ipReasmTimeout = 0;
  mip_ipReasmReqds = 0;
  mip_ipReasmOKs = 0;
  mip_ipReasmFails = 0;
  mip_ipFragOKs = 0;
  mip_ipFragFails = 0;
  mip_ipFragCreates = 0;
  mip_ipRoutingDiscards = 0;

  old_ipInReceives = 0;
  old_ipInHdrErrors = 0;
  old_ipInAddrErrors = 0;
  old_ipForwDatagrams = 0;
  old_ipInUnknownProtos = 0;
  old_ipInDiscards = 0;
  old_ipInDelivers = 0;
  old_ipOutRequests = 0;
  old_ipOutDiscards = 0;
  old_ipOutNoRoutes = 0;
  old_ipReasmReqds = 0;
  old_ipReasmOKs = 0;
  old_ipReasmFails = 0;
  old_ipFragOKs = 0;
  old_ipFragFails = 0;
  old_ipFragCreates = 0;
  old_ipRoutingDiscards = 0;

#ifdef _AIX
  constructor = true;
#endif

  mdr_refresh_rates();
}

mdr_ip_t::~mdr_ip_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_ip_t::mdr_field_value(int field_number, mdr_value_t &value)
{
  switch(field_number) {
  case MIP_IPFORWARDING:
    value.mv_type = MVT_I32;
    value.mv_i32 = mip_ipForwarding;
    break;
  case MIP_IPDEFAULTTTL:
    value.mv_type = MVT_I32;
    value.mv_i32 = mip_ipDefaultTTL;
    break;
  case MIP_IPINRECEIVES:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipInReceives;
    break;
  case MIP_IPINHDRERRORS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipInHdrErrors;
    break;
  case MIP_IPINADDRERRORS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipInAddrErrors;
    break;
  case MIP_IPFORWDATAGRAMS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipForwDatagrams;
    break;
  case MIP_IPINUNKNOWNPROTOS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipInUnknownProtos;
    break;
  case MIP_IPINDISCARDS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipInDiscards;
    break;
  case MIP_IPINDELIVERS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipInDelivers;
    break;
  case MIP_IPOUTREQUESTS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipOutRequests;
    break;
  case MIP_IPOUTDISCARDS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipOutDiscards;
    break;
  case MIP_IPOUTNOROUTES:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipOutNoRoutes;
    break;
  case MIP_IPREASMTIMEOUT:
    value.mv_type = MVT_I32;
    value.mv_i32 = mip_ipReasmTimeout;
    break;
  case MIP_IPREASMREQDS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipReasmReqds;
    break;
  case MIP_IPREASMOKS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipReasmOKs;
    break;
  case MIP_IPREASMFAILS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipReasmFails;
    break;
  case MIP_IPFRAGOKS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipFragOKs;
    break;
  case MIP_IPFRAGFAILS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipFragFails;
    break;
  case MIP_IPFRAGCREATES:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipFragCreates;
    break;
  case MIP_IPROUTINGDISCARDS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mip_ipRoutingDiscards;
    break;
  }
  return value;
}

void mdr_ip_t::mdr_rates(void)
{
  uint_t tmp_ipInReceives = mip_ipInReceives;
  uint_t tmp_ipInHdrErrors = mip_ipInHdrErrors;
  uint_t tmp_ipInAddrErrors = mip_ipInAddrErrors;
  uint_t tmp_ipForwDatagrams = mip_ipForwDatagrams;
  uint_t tmp_ipInUnknownProtos = mip_ipInUnknownProtos;
  uint_t tmp_ipInDiscards = mip_ipInDiscards;
  uint_t tmp_ipInDelivers = mip_ipInDelivers;
  uint_t tmp_ipOutRequests = mip_ipOutRequests;
  uint_t tmp_ipOutDiscards = mip_ipOutDiscards;
  uint_t tmp_ipOutNoRoutes = mip_ipOutNoRoutes;
  uint_t tmp_ipReasmReqds = mip_ipReasmReqds;
  uint_t tmp_ipReasmOKs = mip_ipReasmOKs;
  uint_t tmp_ipReasmFails = mip_ipReasmFails;
  uint_t tmp_ipFragOKs = mip_ipFragOKs;
  uint_t tmp_ipFragFails = mip_ipFragFails;
  uint_t tmp_ipFragCreates = mip_ipFragCreates;
  uint_t tmp_ipRoutingDiscards = mip_ipRoutingDiscards;

  mip_ipInReceives =
    wrap_diff_32(mip_ipInReceives, old_ipInReceives);
  mip_ipInHdrErrors =
    wrap_diff_32(mip_ipInHdrErrors, old_ipInHdrErrors);
  mip_ipInAddrErrors =
    wrap_diff_32(mip_ipInAddrErrors, old_ipInAddrErrors);
  mip_ipForwDatagrams =
    wrap_diff_32(mip_ipForwDatagrams, old_ipForwDatagrams);
  mip_ipInUnknownProtos =
    wrap_diff_32(mip_ipInUnknownProtos, old_ipInUnknownProtos);
  mip_ipInDiscards =
    wrap_diff_32(mip_ipInDiscards, old_ipInDiscards);
  mip_ipInDelivers =
    wrap_diff_32(mip_ipInDelivers, old_ipInDelivers);
  mip_ipOutRequests =
    wrap_diff_32(mip_ipOutRequests, old_ipOutRequests);
  mip_ipOutDiscards =
    wrap_diff_32(mip_ipOutDiscards, old_ipOutDiscards);
  mip_ipOutNoRoutes =
    wrap_diff_32(mip_ipOutNoRoutes, old_ipOutNoRoutes);
  mip_ipReasmReqds =
    wrap_diff_32(mip_ipReasmReqds, old_ipReasmReqds);
  mip_ipReasmOKs =
    wrap_diff_32(mip_ipReasmOKs, old_ipReasmOKs);
  mip_ipReasmFails =
    wrap_diff_32(mip_ipReasmFails, old_ipReasmFails);
  mip_ipFragOKs =
    wrap_diff_32(mip_ipFragOKs, old_ipFragOKs);
  mip_ipFragFails =
    wrap_diff_32(mip_ipFragFails, old_ipFragFails);
  mip_ipFragCreates =
    wrap_diff_32(mip_ipFragCreates, old_ipFragCreates);
  mip_ipRoutingDiscards =
    wrap_diff_32(mip_ipRoutingDiscards, old_ipRoutingDiscards);

  old_ipInReceives = tmp_ipInReceives;
  old_ipInHdrErrors = tmp_ipInHdrErrors;
  old_ipInAddrErrors = tmp_ipInAddrErrors;
  old_ipForwDatagrams = tmp_ipForwDatagrams;
  old_ipInUnknownProtos = tmp_ipInUnknownProtos;
  old_ipInDiscards = tmp_ipInDiscards;
  old_ipInDelivers = tmp_ipInDelivers;
  old_ipOutRequests = tmp_ipOutRequests;
  old_ipOutDiscards = tmp_ipOutDiscards;
  old_ipOutNoRoutes = tmp_ipOutNoRoutes;
  old_ipReasmReqds = tmp_ipReasmReqds;
  old_ipReasmOKs = tmp_ipReasmOKs;
  old_ipReasmFails = tmp_ipReasmFails;
  old_ipFragOKs = tmp_ipFragOKs;
  old_ipFragFails = tmp_ipFragFails;
  old_ipFragCreates = tmp_ipFragCreates;
  old_ipRoutingDiscards = tmp_ipRoutingDiscards;
}

void mdr_ip_t::ip_remote_refresh(void)
{
  mdr_s_mm_t s_map;
  int i;
  int n;

  net_client->mc_refresh(s_map);
  for(i=0, n=s_map.size(); i<n; i++) {
    switch(i) {
    case MIP_IPFORWARDING:
      mip_ipForwarding = s_map[i].mv_i32;
      break;
    case MIP_IPDEFAULTTTL:
      mip_ipDefaultTTL = s_map[i].mv_i32;
      break;
    case MIP_IPINRECEIVES:
      mip_ipInReceives = s_map[i].mv_ui32;
      break;
    case MIP_IPINHDRERRORS:
      mip_ipInHdrErrors = s_map[i].mv_ui32;
      break;
    case MIP_IPINADDRERRORS:
      mip_ipInAddrErrors = s_map[i].mv_ui32;
      break;
    case MIP_IPFORWDATAGRAMS:
      mip_ipForwDatagrams = s_map[i].mv_ui32;
      break;
    case MIP_IPINUNKNOWNPROTOS:
      mip_ipInUnknownProtos = s_map[i].mv_ui32;
      break;
    case MIP_IPINDISCARDS:
      mip_ipInDiscards = s_map[i].mv_ui32;
      break;
    case MIP_IPINDELIVERS:
      mip_ipInDelivers = s_map[i].mv_ui32;
      break;
    case MIP_IPOUTREQUESTS:
      mip_ipOutRequests = s_map[i].mv_ui32;
      break;
    case MIP_IPOUTDISCARDS:
      mip_ipOutDiscards = s_map[i].mv_ui32;
      break;
    case MIP_IPOUTNOROUTES:
      mip_ipOutNoRoutes = s_map[i].mv_ui32;
      break;
    case MIP_IPREASMTIMEOUT:
      mip_ipReasmTimeout = s_map[i].mv_i32;
      break;
    case MIP_IPREASMREQDS:
      mip_ipReasmReqds = s_map[i].mv_ui32;
      break;
    case MIP_IPREASMOKS:
      mip_ipReasmOKs = s_map[i].mv_ui32;
      break;
    case MIP_IPREASMFAILS:
      mip_ipReasmFails = s_map[i].mv_ui32;
      break;
    case MIP_IPFRAGOKS:
      mip_ipFragOKs = s_map[i].mv_ui32;
      break;
    case MIP_IPFRAGFAILS:
      mip_ipFragFails = s_map[i].mv_ui32;
      break;
    case MIP_IPFRAGCREATES:
      mip_ipFragCreates = s_map[i].mv_ui32;
      break;
    case MIP_IPROUTINGDISCARDS:
      mip_ipRoutingDiscards = s_map[i].mv_ui32;
      break;
    default:
      break;
    }
  }

  net_client->mc_release(s_map);
}

}
