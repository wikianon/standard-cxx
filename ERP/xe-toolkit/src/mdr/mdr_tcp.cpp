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


#include "mdr_tcp.h"

namespace mdr {

mdr_tcp_t::mdr_tcp_t(const char *remote_host) :
  mdr_t(MDR_TCP_NAME, true, MTCP_FIELD_COUNT, true)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_tcp_t", remote_host, MTCP_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MTCP_TCPRTOALGORITHM);
    if (! (mdr_os_version.is_darwin && (mdr_os_version.os_version >= 830))) {
      mdr_support_field(MTCP_TCPRTOMIN);
      mdr_support_field(MTCP_TCPRTOMAX);
    }
    mdr_support_field(MTCP_TCPMAXCONN);
    mdr_support_field(MTCP_TCPACTIVEOPENS);
    mdr_support_field(MTCP_TCPPASSIVEOPENS);
    mdr_support_field(MTCP_TCPATTEMPTFAILS);
    mdr_support_field(MTCP_TCPESTABRESETS);
#if defined(_SOLARIS) || defined(_LINUX) || defined(_AIX)
    mdr_support_field(MTCP_TCPCURRESTAB);
#endif
    mdr_support_field(MTCP_TCPINSEGS);
    mdr_support_field(MTCP_TCPOUTSEGS);
    mdr_support_field(MTCP_TCPRETRANSSEGS);
    mdr_support_field(MTCP_TCPINERRS);
    mdr_support_field(MTCP_TCPOUTRSTS);

    net_client = 0;
  }

  mdr_field_names[MTCP_TCPRTOALGORITHM] = "Retransmit Timeout Algorithm";
  mdr_field_names[MTCP_TCPRTOMIN]       = "Minimum Retransmission Timeout";
  mdr_field_names[MTCP_TCPRTOMAX]       = "Maximum Retransmission Timeout";
  mdr_field_names[MTCP_TCPMAXCONN]      = "Maximum Connection Count";
  mdr_field_names[MTCP_TCPACTIVEOPENS]  = "Active Opens";
  mdr_field_names[MTCP_TCPPASSIVEOPENS] = "Passive Opens";
  mdr_field_names[MTCP_TCPATTEMPTFAILS] = "Passive Open Failures";
  mdr_field_names[MTCP_TCPESTABRESETS]  = "Connection Resets";
  mdr_field_names[MTCP_TCPCURRESTAB]    = "Established Connections";
  mdr_field_names[MTCP_TCPINSEGS]       = "Segments Received";
  mdr_field_names[MTCP_TCPOUTSEGS]      = "Segments Transmitted";
  mdr_field_names[MTCP_TCPRETRANSSEGS]  = "Segments Retransmitted";
  mdr_field_names[MTCP_TCPINERRS]       = "Segments Received mith Error";
  mdr_field_names[MTCP_TCPOUTRSTS]      = "RST Segments Transmitted";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("TCP_Category");

  mtcp_tcpRtoAlgorithm = 0;
  mtcp_tcpRtoMin = 0;
  mtcp_tcpRtoMax = 0;
  mtcp_tcpMaxConn = 0;
  mtcp_tcpActiveOpens = 0;
  mtcp_tcpPassiveOpens = 0;
  mtcp_tcpAttemptFails = 0;
  mtcp_tcpEstabResets = 0;
  mtcp_tcpCurrEstab = 0;
  mtcp_tcpInSegs = 0;
  mtcp_tcpOutSegs = 0;
  mtcp_tcpRetransSegs = 0;
  mtcp_tcpInErrs = 0;
  mtcp_tcpOutRsts = 0;

  old_tcpActiveOpens = 0;
  old_tcpPassiveOpens = 0;
  old_tcpAttemptFails = 0;
  old_tcpEstabResets = 0;
  old_tcpCurrEstab = 0;
  old_tcpInSegs = 0;
  old_tcpOutSegs = 0;
  old_tcpRetransSegs = 0;
  old_tcpInErrs = 0;
  old_tcpOutRsts = 0;

#ifdef _AIX
  constructor = true;
#endif

  mdr_refresh_rates();
}

mdr_tcp_t::~mdr_tcp_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_tcp_t::mdr_field_value(int field_number, mdr_value_t &value)
{
  switch(field_number) {
  case MTCP_TCPRTOALGORITHM:
    value.mv_type = MVT_I32;
    value.mv_i32 = mtcp_tcpRtoAlgorithm;
    break;
  case MTCP_TCPRTOMIN:
    value.mv_type = MVT_I32;
    value.mv_i32 = mtcp_tcpRtoMin;
    break;
  case MTCP_TCPRTOMAX:
    value.mv_type = MVT_I32;
    value.mv_i32 = mtcp_tcpRtoMax;
    break;
  case MTCP_TCPMAXCONN:
    value.mv_type = MVT_I32;
    value.mv_i32 = mtcp_tcpMaxConn;
    break;
  case MTCP_TCPACTIVEOPENS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mtcp_tcpActiveOpens;
    break;
  case MTCP_TCPPASSIVEOPENS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mtcp_tcpPassiveOpens;
    break;
  case MTCP_TCPATTEMPTFAILS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mtcp_tcpAttemptFails;
    break;
  case MTCP_TCPESTABRESETS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mtcp_tcpEstabResets;
    break;
  case MTCP_TCPCURRESTAB:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mtcp_tcpCurrEstab;
    break;
  case MTCP_TCPINSEGS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mtcp_tcpInSegs;
    break;
  case MTCP_TCPOUTSEGS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mtcp_tcpOutSegs;
    break;
  case MTCP_TCPRETRANSSEGS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mtcp_tcpRetransSegs;
    break;
  case MTCP_TCPINERRS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mtcp_tcpInErrs;
    break;
  case MTCP_TCPOUTRSTS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mtcp_tcpOutRsts;
    break;
  }
  return value;
}

void mdr_tcp_t::mdr_rates(void)
{
  uint_t tmp_tcpActiveOpens = mtcp_tcpActiveOpens;
  uint_t tmp_tcpPassiveOpens = mtcp_tcpPassiveOpens;
  uint_t tmp_tcpAttemptFails = mtcp_tcpAttemptFails;
  uint_t tmp_tcpEstabResets = mtcp_tcpEstabResets;
  uint_t tmp_tcpCurrEstab = mtcp_tcpCurrEstab;
  uint_t tmp_tcpInSegs = mtcp_tcpInSegs;
  uint_t tmp_tcpOutSegs = mtcp_tcpOutSegs;
  uint_t tmp_tcpRetransSegs = mtcp_tcpRetransSegs;
  uint_t tmp_tcpInErrs = mtcp_tcpInErrs;
  uint_t tmp_tcpOutRsts = mtcp_tcpOutRsts;

  mtcp_tcpActiveOpens =
    wrap_diff_32(mtcp_tcpActiveOpens, old_tcpActiveOpens);
  mtcp_tcpPassiveOpens =
    wrap_diff_32(mtcp_tcpPassiveOpens, old_tcpPassiveOpens);
  mtcp_tcpAttemptFails =
    wrap_diff_32(mtcp_tcpAttemptFails, old_tcpAttemptFails);
  mtcp_tcpEstabResets =
    wrap_diff_32(mtcp_tcpEstabResets, old_tcpEstabResets);
  mtcp_tcpCurrEstab =
    wrap_diff_32(mtcp_tcpCurrEstab, old_tcpCurrEstab);
  mtcp_tcpInSegs =
    wrap_diff_32(mtcp_tcpInSegs, old_tcpInSegs);
  mtcp_tcpOutSegs =
    wrap_diff_32(mtcp_tcpOutSegs, old_tcpOutSegs);
  mtcp_tcpRetransSegs =
    wrap_diff_32(mtcp_tcpRetransSegs, old_tcpRetransSegs);
  mtcp_tcpInErrs =
    wrap_diff_32(mtcp_tcpInErrs, old_tcpInErrs);
  mtcp_tcpOutRsts =
    wrap_diff_32(mtcp_tcpOutRsts, old_tcpOutRsts);

  old_tcpActiveOpens = tmp_tcpActiveOpens;
  old_tcpPassiveOpens = tmp_tcpPassiveOpens;
  old_tcpAttemptFails = tmp_tcpAttemptFails;
  old_tcpEstabResets = tmp_tcpEstabResets;
  old_tcpCurrEstab = tmp_tcpCurrEstab;
  old_tcpInSegs = tmp_tcpInSegs;
  old_tcpOutSegs = tmp_tcpOutSegs;
  old_tcpRetransSegs = tmp_tcpRetransSegs;
  old_tcpInErrs = tmp_tcpInErrs;
  old_tcpOutRsts = tmp_tcpOutRsts;
}

void mdr_tcp_t::tcp_remote_refresh(void)
{
  mdr_s_mm_t s_map;
  int i;
  int n;

  net_client->mc_refresh(s_map);
  for(i=0, n=s_map.size(); i<n; i++) {
    switch(i) {
    case MTCP_TCPRTOALGORITHM:
      mtcp_tcpRtoAlgorithm = s_map[i].mv_i32;
      break;
    case MTCP_TCPRTOMIN:
      mtcp_tcpRtoMin = s_map[i].mv_i32;
      break;
    case MTCP_TCPRTOMAX:
      mtcp_tcpRtoMax = s_map[i].mv_i32;
      break;
    case MTCP_TCPMAXCONN:
      mtcp_tcpMaxConn = s_map[i].mv_i32;
      break;
    case MTCP_TCPACTIVEOPENS:
      mtcp_tcpActiveOpens = s_map[i].mv_ui32;
      break;
    case MTCP_TCPPASSIVEOPENS:
      mtcp_tcpPassiveOpens = s_map[i].mv_ui32;
      break;
    case MTCP_TCPATTEMPTFAILS:
      mtcp_tcpAttemptFails = s_map[i].mv_ui32;
      break;
    case MTCP_TCPESTABRESETS:
      mtcp_tcpEstabResets = s_map[i].mv_ui32;
      break;
    case MTCP_TCPCURRESTAB:
      mtcp_tcpCurrEstab = s_map[i].mv_ui32;
      break;
    case MTCP_TCPINSEGS:
      mtcp_tcpInSegs = s_map[i].mv_ui32;
      break;
    case MTCP_TCPOUTSEGS:
      mtcp_tcpOutSegs = s_map[i].mv_ui32;
      break;
    case MTCP_TCPRETRANSSEGS:
      mtcp_tcpRetransSegs = s_map[i].mv_ui32;
      break;
    case MTCP_TCPINERRS:
      mtcp_tcpInErrs = s_map[i].mv_ui32;
      break;
    case MTCP_TCPOUTRSTS:
      mtcp_tcpOutRsts = s_map[i].mv_ui32;
      break;
    default:
      break;
    }
  }

  net_client->mc_release(s_map);
}

}
