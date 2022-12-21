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


#include "mdr_udp.h"

namespace mdr {

mdr_udp_t::mdr_udp_t(const char *remote_host) :
  mdr_t(MDR_UDP_NAME, true, MUDP_FIELD_COUNT, true)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_udp_t", remote_host, MUDP_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MUDP_UDPINDATAGRAMS);
    mdr_support_field(MUDP_UDPNOPORTS);
    mdr_support_field(MUDP_UDPINERRORS);
    mdr_support_field(MUDP_UDPOUTDATAGRAMS);

    net_client = 0;
  }

  mdr_field_names[MUDP_UDPINDATAGRAMS]  = "Datagrams Received";
  mdr_field_names[MUDP_UDPNOPORTS]      = "Undeliverable Datagrams Received";
  mdr_field_names[MUDP_UDPINERRORS]     = "Datagrams Received mith Error";
  mdr_field_names[MUDP_UDPOUTDATAGRAMS] = "Datagrams Transmitted";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("UDP_Category");

  mudp_udpInDatagrams = 0;
  mudp_udpNoPorts = 0;
  mudp_udpInErrors = 0;
  mudp_udpOutDatagrams = 0;

  old_udpInDatagrams = 0;
  old_udpNoPorts = 0;
  old_udpInErrors = 0;
  old_udpOutDatagrams = 0;

#ifdef _AIX
  constructor = true;
#endif

  mdr_refresh_rates();
}

mdr_udp_t::~mdr_udp_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_udp_t::mdr_field_value(int field_number, mdr_value_t &value)
{
  switch(field_number) {
  case MUDP_UDPINDATAGRAMS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mudp_udpInDatagrams;
    break;
  case MUDP_UDPNOPORTS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mudp_udpNoPorts;
    break;
  case MUDP_UDPINERRORS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mudp_udpInErrors;
    break;
  case MUDP_UDPOUTDATAGRAMS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mudp_udpOutDatagrams;
    break;
  }
  return value;
}

void mdr_udp_t::mdr_rates(void)
{
  uint_t tmp_udpInDatagrams = mudp_udpInDatagrams;
  uint_t tmp_udpNoPorts = mudp_udpNoPorts;
  uint_t tmp_udpInErrors = mudp_udpInErrors;
  uint_t tmp_udpOutDatagrams = mudp_udpOutDatagrams;

  mudp_udpInDatagrams =
    wrap_diff_32(mudp_udpInDatagrams, old_udpInDatagrams);
  mudp_udpNoPorts =
    wrap_diff_32(mudp_udpNoPorts, old_udpNoPorts);
  mudp_udpInErrors =
    wrap_diff_32(mudp_udpInErrors, old_udpInErrors);
  mudp_udpOutDatagrams =
    wrap_diff_32(mudp_udpOutDatagrams, old_udpOutDatagrams);

  old_udpInDatagrams = tmp_udpInDatagrams;
  old_udpNoPorts = tmp_udpNoPorts;
  old_udpInErrors = tmp_udpInErrors;
  old_udpOutDatagrams = tmp_udpOutDatagrams;
}

void mdr_udp_t::udp_remote_refresh(void)
{
  mdr_s_mm_t s_map;
  int i;
  int n;

  net_client->mc_refresh(s_map);
  for(i=0, n=s_map.size(); i<n; i++) {
    switch(i) {
    case MUDP_UDPINDATAGRAMS:
      mudp_udpInDatagrams = s_map[i].mv_ui32;
      break;
    case MUDP_UDPNOPORTS:
      mudp_udpNoPorts = s_map[i].mv_ui32;
      break;
    case MUDP_UDPINERRORS:
      mudp_udpInErrors = s_map[i].mv_ui32;
      break;
    case MUDP_UDPOUTDATAGRAMS:
      mudp_udpOutDatagrams = s_map[i].mv_ui32;
      break;
    default:
      break;
    }
  }

  net_client->mc_release(s_map);
}

}
