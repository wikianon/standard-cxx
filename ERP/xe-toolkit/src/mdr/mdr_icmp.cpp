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


#include "mdr_icmp.h"

namespace mdr {

mdr_icmp_t::mdr_icmp_t(const char *remote_host) :
  mdr_t(MDR_ICMP_NAME, true, MICMP_FIELD_COUNT, true)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_icmp_t", remote_host, MICMP_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MICMP_ICMPINMSGS);
    mdr_support_field(MICMP_ICMPINERRORS);
    mdr_support_field(MICMP_ICMPINDESTUNREACHS);
    mdr_support_field(MICMP_ICMPINTIMEEXCDS);
    mdr_support_field(MICMP_ICMPINPARMPROBS);
    mdr_support_field(MICMP_ICMPINSRCQUENCHS);
    mdr_support_field(MICMP_ICMPINREDIRECTS);
    mdr_support_field(MICMP_ICMPINECHOS);
    mdr_support_field(MICMP_ICMPINECHOREPS);
    mdr_support_field(MICMP_ICMPINTIMESTAMPS);
    mdr_support_field(MICMP_ICMPINTIMESTAMPREPS);
    mdr_support_field(MICMP_ICMPINADDRMASKS);
    mdr_support_field(MICMP_ICMPINADDRMASKREPS);
    mdr_support_field(MICMP_ICMPOUTMSGS);
    mdr_support_field(MICMP_ICMPOUTERRORS);
    mdr_support_field(MICMP_ICMPOUTDESTUNREACHS);
    mdr_support_field(MICMP_ICMPOUTTIMEEXCDS);
    mdr_support_field(MICMP_ICMPOUTPARMPROBS);
    mdr_support_field(MICMP_ICMPOUTSRCQUENCHS);
    mdr_support_field(MICMP_ICMPOUTREDIRECTS);
    mdr_support_field(MICMP_ICMPOUTECHOS);
    mdr_support_field(MICMP_ICMPOUTECHOREPS);
    mdr_support_field(MICMP_ICMPOUTTIMESTAMPS);
    mdr_support_field(MICMP_ICMPOUTTIMESTAMPREPS);
    mdr_support_field(MICMP_ICMPOUTADDRMASKS);
    mdr_support_field(MICMP_ICMPOUTADDRMASKREPS);

    net_client = 0;
  }

  mdr_field_names[MICMP_ICMPINMSGS]           = "Messages Received";
  mdr_field_names[MICMP_ICMPINERRORS]         = "Messages Received mith Error";
  mdr_field_names[MICMP_ICMPINDESTUNREACHS]   = "Dest. Unreach. Msgs. Received";
  mdr_field_names[MICMP_ICMPINTIMEEXCDS]      = "Time Exceeded Msgs. Received";
  mdr_field_names[MICMP_ICMPINPARMPROBS]      = "Param. Prob. Msgs. Received";
  mdr_field_names[MICMP_ICMPINSRCQUENCHS]     = "Source Quench Msgs. Received";
  mdr_field_names[MICMP_ICMPINREDIRECTS]      = "ICMP Redirect Msgs. Received";
  mdr_field_names[MICMP_ICMPINECHOS]          = "Echo Request Msgs. Received";
  mdr_field_names[MICMP_ICMPINECHOREPS]       = "Echo Reply Msgs. Received";
  mdr_field_names[MICMP_ICMPINTIMESTAMPS]     = "Timestamp Msgs. Received";
  mdr_field_names[MICMP_ICMPINTIMESTAMPREPS]  = "Timestamp Reply Msgs. Received";
  mdr_field_names[MICMP_ICMPINADDRMASKS]      = "Addr. Mask Req. Msgs. Received";
  mdr_field_names[MICMP_ICMPINADDRMASKREPS]   = "Addr. Mask Reply Msgs. Received";
  mdr_field_names[MICMP_ICMPOUTMSGS]          = "Messages Transmitted";
  mdr_field_names[MICMP_ICMPOUTERRORS]        = "Messages Transmitted for Errors";
  mdr_field_names[MICMP_ICMPOUTDESTUNREACHS]  = "Dest. Unreach. Msgs. Transmitted";
  mdr_field_names[MICMP_ICMPOUTTIMEEXCDS]     = "Time Exceeded Msgs. Transmitted";
  mdr_field_names[MICMP_ICMPOUTPARMPROBS]     = "Param. Prob. Msgs. Transmitted";
  mdr_field_names[MICMP_ICMPOUTSRCQUENCHS]    = "Source Quench Msgs. Transmitted";
  mdr_field_names[MICMP_ICMPOUTREDIRECTS]     = "ICMP Redirect Msgs. Transmitted";
  mdr_field_names[MICMP_ICMPOUTECHOS]         = "Echo Request Msgs. Transmitted";
  mdr_field_names[MICMP_ICMPOUTECHOREPS]      = "Echo Reply Msgs. Transmitted";
  mdr_field_names[MICMP_ICMPOUTTIMESTAMPS]    = "Timestamp Msgs. Transmitted";
  mdr_field_names[MICMP_ICMPOUTTIMESTAMPREPS] = "Timestamp Reply Msgs. Transmitted";
  mdr_field_names[MICMP_ICMPOUTADDRMASKS]     = "Addr. Mask Req. Msgs. Transmitted";
  mdr_field_names[MICMP_ICMPOUTADDRMASKREPS]  = "Addr. Mask Reply Msgs. Transmitted";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("ICMP_Category");

  micmp_icmpInMsgs = 0;
  micmp_icmpInErrors = 0;
  micmp_icmpInDestUnreachs = 0;
  micmp_icmpInTimeExcds = 0;
  micmp_icmpInParmProbs = 0;
  micmp_icmpInSrcQuenchs = 0;
  micmp_icmpInRedirects = 0;
  micmp_icmpInEchos = 0;
  micmp_icmpInEchoReps = 0;
  micmp_icmpInTimestamps = 0;
  micmp_icmpInTimestampReps = 0;
  micmp_icmpInAddrMasks = 0;
  micmp_icmpInAddrMaskReps = 0;
  micmp_icmpOutMsgs = 0;
  micmp_icmpOutErrors = 0;
  micmp_icmpOutDestUnreachs = 0;
  micmp_icmpOutTimeExcds = 0;
  micmp_icmpOutParmProbs = 0;
  micmp_icmpOutSrcQuenchs = 0;
  micmp_icmpOutRedirects = 0;
  micmp_icmpOutEchos = 0;
  micmp_icmpOutEchoReps = 0;
  micmp_icmpOutTimestamps = 0;
  micmp_icmpOutTimestampReps = 0;
  micmp_icmpOutAddrMasks = 0;
  micmp_icmpOutAddrMaskReps = 0;

  old_icmpInMsgs = 0;
  old_icmpInErrors = 0;
  old_icmpInDestUnreachs = 0;
  old_icmpInTimeExcds = 0;
  old_icmpInParmProbs = 0;
  old_icmpInSrcQuenchs = 0;
  old_icmpInRedirects = 0;
  old_icmpInEchos = 0;
  old_icmpInEchoReps = 0;
  old_icmpInTimestamps = 0;
  old_icmpInTimestampReps = 0;
  old_icmpInAddrMasks = 0;
  old_icmpInAddrMaskReps = 0;
  old_icmpOutMsgs = 0;
  old_icmpOutErrors = 0;
  old_icmpOutDestUnreachs = 0;
  old_icmpOutTimeExcds = 0;
  old_icmpOutParmProbs = 0;
  old_icmpOutSrcQuenchs = 0;
  old_icmpOutRedirects = 0;
  old_icmpOutEchos = 0;
  old_icmpOutEchoReps = 0;
  old_icmpOutTimestamps = 0;
  old_icmpOutTimestampReps = 0;
  old_icmpOutAddrMasks = 0;
  old_icmpOutAddrMaskReps = 0;

#ifdef _AIX
  constructor = true;
#endif

  mdr_refresh_rates();
}

mdr_icmp_t::~mdr_icmp_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_icmp_t::mdr_field_value(int field_number, mdr_value_t &value)
{
  switch(field_number) {
  case MICMP_ICMPINMSGS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInMsgs;
    break;
  case MICMP_ICMPINERRORS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInErrors;
    break;
  case MICMP_ICMPINDESTUNREACHS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInDestUnreachs;
    break;
  case MICMP_ICMPINTIMEEXCDS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInTimeExcds;
    break;
  case MICMP_ICMPINPARMPROBS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInParmProbs;
    break;
  case MICMP_ICMPINSRCQUENCHS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInSrcQuenchs;
    break;
  case MICMP_ICMPINREDIRECTS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInRedirects;
    break;
  case MICMP_ICMPINECHOS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInEchos;
    break;
  case MICMP_ICMPINECHOREPS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInEchoReps;
    break;
  case MICMP_ICMPINTIMESTAMPS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInTimestamps;
    break;
  case MICMP_ICMPINTIMESTAMPREPS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInTimestampReps;
    break;
  case MICMP_ICMPINADDRMASKS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInAddrMasks;
    break;
  case MICMP_ICMPINADDRMASKREPS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpInAddrMaskReps;
    break;
  case MICMP_ICMPOUTMSGS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutMsgs;
    break;
  case MICMP_ICMPOUTERRORS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutErrors;
    break;
  case MICMP_ICMPOUTDESTUNREACHS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutDestUnreachs;
    break;
  case MICMP_ICMPOUTTIMEEXCDS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutTimeExcds;
    break;
  case MICMP_ICMPOUTPARMPROBS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutParmProbs;
    break;
  case MICMP_ICMPOUTSRCQUENCHS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutSrcQuenchs;
    break;
  case MICMP_ICMPOUTREDIRECTS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutRedirects;
    break;
  case MICMP_ICMPOUTECHOS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutEchos;
    break;
  case MICMP_ICMPOUTECHOREPS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutEchoReps;
    break;
  case MICMP_ICMPOUTTIMESTAMPS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutTimestamps;
    break;
  case MICMP_ICMPOUTTIMESTAMPREPS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutTimestampReps;
    break;
  case MICMP_ICMPOUTADDRMASKS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutAddrMasks;
    break;
  case MICMP_ICMPOUTADDRMASKREPS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = micmp_icmpOutAddrMaskReps;
    break;
  }
  return value;
}

void mdr_icmp_t::mdr_rates(void)
{
  uint_t tmp_icmpInMsgs = micmp_icmpInMsgs;
  uint_t tmp_icmpInErrors = micmp_icmpInErrors;
  uint_t tmp_icmpInDestUnreachs = micmp_icmpInDestUnreachs;
  uint_t tmp_icmpInTimeExcds = micmp_icmpInTimeExcds;
  uint_t tmp_icmpInParmProbs = micmp_icmpInParmProbs;
  uint_t tmp_icmpInSrcQuenchs = micmp_icmpInSrcQuenchs;
  uint_t tmp_icmpInRedirects = micmp_icmpInRedirects;
  uint_t tmp_icmpInEchos = micmp_icmpInEchos;
  uint_t tmp_icmpInEchoReps = micmp_icmpInEchoReps;
  uint_t tmp_icmpInTimestamps = micmp_icmpInTimestamps;
  uint_t tmp_icmpInTimestampReps = micmp_icmpInTimestampReps;
  uint_t tmp_icmpInAddrMasks = micmp_icmpInAddrMasks;
  uint_t tmp_icmpInAddrMaskReps = micmp_icmpInAddrMaskReps;
  uint_t tmp_icmpOutMsgs = micmp_icmpOutMsgs;
  uint_t tmp_icmpOutErrors = micmp_icmpOutErrors;
  uint_t tmp_icmpOutDestUnreachs = micmp_icmpOutDestUnreachs;
  uint_t tmp_icmpOutTimeExcds = micmp_icmpOutTimeExcds;
  uint_t tmp_icmpOutParmProbs = micmp_icmpOutParmProbs;
  uint_t tmp_icmpOutSrcQuenchs = micmp_icmpOutSrcQuenchs;
  uint_t tmp_icmpOutRedirects = micmp_icmpOutRedirects;
  uint_t tmp_icmpOutEchos = micmp_icmpOutEchos;
  uint_t tmp_icmpOutEchoReps = micmp_icmpOutEchoReps;
  uint_t tmp_icmpOutTimestamps = micmp_icmpOutTimestamps;
  uint_t tmp_icmpOutTimestampReps = micmp_icmpOutTimestampReps;
  uint_t tmp_icmpOutAddrMasks = micmp_icmpOutAddrMasks;
  uint_t tmp_icmpOutAddrMaskReps = micmp_icmpOutAddrMaskReps;

  micmp_icmpInMsgs =
    wrap_diff_32(micmp_icmpInMsgs, old_icmpInMsgs);
  micmp_icmpInErrors =
    wrap_diff_32(micmp_icmpInErrors, old_icmpInErrors);
  micmp_icmpInDestUnreachs =
    wrap_diff_32(micmp_icmpInDestUnreachs, old_icmpInDestUnreachs);
  micmp_icmpInTimeExcds =
    wrap_diff_32(micmp_icmpInTimeExcds, old_icmpInTimeExcds);
  micmp_icmpInParmProbs =
    wrap_diff_32(micmp_icmpInParmProbs, old_icmpInParmProbs);
  micmp_icmpInSrcQuenchs =
    wrap_diff_32(micmp_icmpInSrcQuenchs, old_icmpInSrcQuenchs);
  micmp_icmpInRedirects =
    wrap_diff_32(micmp_icmpInRedirects, old_icmpInRedirects);
  micmp_icmpInEchos =
    wrap_diff_32(micmp_icmpInEchos, old_icmpInEchos);
  micmp_icmpInEchoReps =
    wrap_diff_32(micmp_icmpInEchoReps, old_icmpInEchoReps);
  micmp_icmpInTimestamps =
    wrap_diff_32(micmp_icmpInTimestamps, old_icmpInTimestamps);
  micmp_icmpInTimestampReps =
    wrap_diff_32(micmp_icmpInTimestampReps, old_icmpInTimestampReps);
  micmp_icmpInAddrMasks =
    wrap_diff_32(micmp_icmpInAddrMasks, old_icmpInAddrMasks);
  micmp_icmpInAddrMaskReps =
    wrap_diff_32(micmp_icmpInAddrMaskReps, old_icmpInAddrMaskReps);
  micmp_icmpOutMsgs =
    wrap_diff_32(micmp_icmpOutMsgs, old_icmpOutMsgs);
  micmp_icmpOutErrors =
    wrap_diff_32(micmp_icmpOutErrors, old_icmpOutErrors);
  micmp_icmpOutDestUnreachs =
    wrap_diff_32(micmp_icmpOutDestUnreachs, old_icmpOutDestUnreachs);
  micmp_icmpOutTimeExcds =
    wrap_diff_32(micmp_icmpOutTimeExcds, old_icmpOutTimeExcds);
  micmp_icmpOutParmProbs =
    wrap_diff_32(micmp_icmpOutParmProbs, old_icmpOutParmProbs);
  micmp_icmpOutSrcQuenchs =
    wrap_diff_32(micmp_icmpOutSrcQuenchs, old_icmpOutSrcQuenchs);
  micmp_icmpOutRedirects =
    wrap_diff_32(micmp_icmpOutRedirects, old_icmpOutRedirects);
  micmp_icmpOutEchos =
    wrap_diff_32(micmp_icmpOutEchos, old_icmpOutEchos);
  micmp_icmpOutEchoReps =
    wrap_diff_32(micmp_icmpOutEchoReps, old_icmpOutEchoReps);
  micmp_icmpOutTimestamps =
    wrap_diff_32(micmp_icmpOutTimestamps, old_icmpOutTimestamps);
  micmp_icmpOutTimestampReps =
    wrap_diff_32(micmp_icmpOutTimestampReps, old_icmpOutTimestampReps);
  micmp_icmpOutAddrMasks =
    wrap_diff_32(micmp_icmpOutAddrMasks, old_icmpOutAddrMasks);
  micmp_icmpOutAddrMaskReps =
    wrap_diff_32(micmp_icmpOutAddrMaskReps, old_icmpOutAddrMaskReps);

  old_icmpInMsgs = tmp_icmpInMsgs;
  old_icmpInErrors = tmp_icmpInErrors;
  old_icmpInDestUnreachs = tmp_icmpInDestUnreachs;
  old_icmpInTimeExcds = tmp_icmpInTimeExcds;
  old_icmpInParmProbs = tmp_icmpInParmProbs;
  old_icmpInSrcQuenchs = tmp_icmpInSrcQuenchs;
  old_icmpInRedirects = tmp_icmpInRedirects;
  old_icmpInEchos = tmp_icmpInEchos;
  old_icmpInEchoReps = tmp_icmpInEchoReps;
  old_icmpInTimestamps = tmp_icmpInTimestamps;
  old_icmpInTimestampReps = tmp_icmpInTimestampReps;
  old_icmpInAddrMasks = tmp_icmpInAddrMasks;
  old_icmpInAddrMaskReps = tmp_icmpInAddrMaskReps;
  old_icmpOutMsgs = tmp_icmpOutMsgs;
  old_icmpOutErrors = tmp_icmpOutErrors;
  old_icmpOutDestUnreachs = tmp_icmpOutDestUnreachs;
  old_icmpOutTimeExcds = tmp_icmpOutTimeExcds;
  old_icmpOutParmProbs = tmp_icmpOutParmProbs;
  old_icmpOutSrcQuenchs = tmp_icmpOutSrcQuenchs;
  old_icmpOutRedirects = tmp_icmpOutRedirects;
  old_icmpOutEchos = tmp_icmpOutEchos;
  old_icmpOutEchoReps = tmp_icmpOutEchoReps;
  old_icmpOutTimestamps = tmp_icmpOutTimestamps;
  old_icmpOutTimestampReps = tmp_icmpOutTimestampReps;
  old_icmpOutAddrMasks = tmp_icmpOutAddrMasks;
  old_icmpOutAddrMaskReps = tmp_icmpOutAddrMaskReps;
}

void mdr_icmp_t::icmp_remote_refresh(void)
{
  mdr_s_mm_t s_map;
  int i;
  int n;

  net_client->mc_refresh(s_map);
  for(i=0, n=s_map.size(); i<n; i++) {
    switch(i) {
    case MICMP_ICMPINMSGS:
      micmp_icmpInMsgs = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPINERRORS:
      micmp_icmpInErrors = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPINDESTUNREACHS:
      micmp_icmpInDestUnreachs = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPINTIMEEXCDS:
      micmp_icmpInTimeExcds = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPINPARMPROBS:
      micmp_icmpInParmProbs = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPINSRCQUENCHS:
      micmp_icmpInSrcQuenchs = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPINREDIRECTS:
      micmp_icmpInRedirects = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPINECHOS:
      micmp_icmpInEchos = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPINECHOREPS:
      micmp_icmpInEchoReps = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPINTIMESTAMPS:
      micmp_icmpInTimestamps = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPINTIMESTAMPREPS:
      micmp_icmpInTimestampReps = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPINADDRMASKS:
      micmp_icmpInAddrMasks = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPINADDRMASKREPS:
      micmp_icmpInAddrMaskReps = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTMSGS:
      micmp_icmpOutMsgs = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTERRORS:
      micmp_icmpOutErrors = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTDESTUNREACHS:
      micmp_icmpOutDestUnreachs = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTTIMEEXCDS:
      micmp_icmpOutTimeExcds = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTPARMPROBS:
      micmp_icmpOutParmProbs = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTSRCQUENCHS:
      micmp_icmpOutSrcQuenchs = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTREDIRECTS:
      micmp_icmpOutRedirects = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTECHOS:
      micmp_icmpOutEchos = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTECHOREPS:
      micmp_icmpOutEchoReps = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTTIMESTAMPS:
      micmp_icmpOutTimestamps = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTTIMESTAMPREPS:
      micmp_icmpOutTimestampReps = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTADDRMASKS:
      micmp_icmpOutAddrMasks = s_map[i].mv_ui32;
      break;
    case MICMP_ICMPOUTADDRMASKREPS:
      micmp_icmpOutAddrMaskReps = s_map[i].mv_ui32;
      break;
    default:
      break;
    }
  }

  net_client->mc_release(s_map);
}

}
