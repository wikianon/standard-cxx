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


#include "mdr_pf.h"

namespace mdr {

mdr_pf_t::mdr_pf_t(const char *remote_host) :
  mdr_t(MDR_PF_NAME, true, MPF_FIELD_COUNT, false)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_pf_t", remote_host, MPF_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    if (mdr_os_version.is_darwin && (mdr_os_version.os_version < 840))
      mdr_category_supported = false;
    else {
      mdr_support_field(MPF_SIZE_K);
      mdr_support_field(MPF_FREE_K);
      mdr_support_field(MPF_USED_K);
    }

    net_client = 0;
  }

  mdr_field_names[MPF_SIZE_K] = "KBytes Total";
  mdr_field_names[MPF_FREE_K] = "KBytes Free";
  mdr_field_names[MPF_USED_K] = "KBytes Used";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("PF_Category");

  mdr_refresh_rates();
}

mdr_pf_t::~mdr_pf_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_pf_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  mpf_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_pf_t::mdr_field_value: key %s not found", key.c_str());

  switch(field_number) {
  case MPF_SIZE_K:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mpf_size_K;
    break;
  case MPF_FREE_K:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mpf_free_K;
    break;
  case MPF_USED_K:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mpf_used_K;
    break;
  }
  return value;
}

void mdr_pf_t::mdr_rates(void)
{
  // there are no rates
}

const mdr_keyset_t &mdr_pf_t::mdr_keyset(mdr_keyset_t &keyset)
{
  mpf_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_pf_t::pf_remote_refresh(void)
{
  mdr_m_mm_t m_map;
  mdr_m_mm_node_t mn;
  mpf_can_t can;
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
      case MPF_SIZE_K:
        can.mpf_size_K = s_map[i].mv_ui32;
        break;
      case MPF_FREE_K:
        can.mpf_free_K = s_map[i].mv_ui32;
        break;
      case MPF_USED_K:
        can.mpf_used_K = s_map[i].mv_ui32;
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
