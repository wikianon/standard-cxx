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


#include "mdr_cache.h"

namespace mdr {

mdr_cache_t::mdr_cache_t(const char *remote_host) :
  mdr_t(MDR_CACHE_NAME, true, MC_FIELD_COUNT, false)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_cache_t", remote_host, MC_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
#if defined(_LINUX) || defined(_AIX)
    mdr_category_supported = false;
#else
    mdr_support_field(MC_HITS);
    mdr_support_field(MC_MISSES);
    mdr_support_field(MC_HIT_PERCENT);
#endif

    net_client = 0;
  }

  mdr_field_names[MC_HITS]        = "Cache Hits";
  mdr_field_names[MC_MISSES]      = "Cache Misses";
  mdr_field_names[MC_HIT_PERCENT] = "Hit Percentage";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("Cache_Category");

  mdr_refresh_rates();
}

mdr_cache_t::~mdr_cache_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_cache_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  mc_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_cache_t::mdr_field_value: key %s not found", key.c_str());

  switch(field_number) {
  case MC_HITS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mc_hits;
    break;
  case MC_MISSES:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.mc_misses;
    break;
  case MC_HIT_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = n->second.mc_hit_percent;
    break;
  }
  return value;
}

void mdr_cache_t::mdr_rates(void)
{
  mc_node_t n;
  mc_node_t nn;
  mc_can_t *cp;
  mc_can_t *ocp;
  uint_t tmp_hits;
  uint_t tmp_misses;
  double total;

  for(n=begin(); n != end(); n++) {
    cp = &n->second;
    nn = old.find(n->first);
    if (nn == old.end()) {
      // should not happen
      old[n->first].clear();
      continue;
    }
    ocp = &nn->second;

    tmp_hits = cp->mc_hits;
    tmp_misses = cp->mc_misses;

    cp->mc_hits   = wrap_diff_32(cp->mc_hits, ocp->mc_hits);
    cp->mc_misses = wrap_diff_32(cp->mc_misses, ocp->mc_misses);
    total = cp->mc_hits + cp->mc_misses;
    if (total > 0.0)
      cp->mc_hit_percent = (100.0 * cp->mc_hits) / total;
    else
      cp->mc_hit_percent = 100.0;

    ocp->mc_hits = tmp_hits;
    ocp->mc_misses = tmp_misses;
  }
}

const mdr_keyset_t &mdr_cache_t::mdr_keyset(mdr_keyset_t &keyset)
{
  mc_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_cache_t::cache_remote_refresh(void)
{
  mdr_m_mm_t m_map;
  mdr_m_mm_node_t mn;
  mc_can_t can;
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
      case MC_HITS:
        can.mc_hits = s_map[i].mv_ui32;
        break;
      case MC_MISSES:
        can.mc_misses = s_map[i].mv_ui32;
        break;
      case MC_HIT_PERCENT:
        can.mc_hit_percent = s_map[i].mv_d;
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
