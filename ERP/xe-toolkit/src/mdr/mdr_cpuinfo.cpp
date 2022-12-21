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


#include "mdr_cpuinfo.h"

namespace mdr {

mdr_cpuinfo_t::mdr_cpuinfo_t(const char *remote_host) :
  mdr_t(MDR_CPUINFO_NAME, true, MCI_FIELD_COUNT, false)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_cpuinfo_t", remote_host, MCI_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MCI_INSTANCE);
    mdr_support_field(MCI_ARCHITECTURE);
    mdr_support_field(MCI_MODEL);
    mdr_support_field(MCI_CLOCK_MHZ);

    net_client = 0;
  }

  mdr_field_names[MCI_INSTANCE]     = "CPU Instance Number";
  mdr_field_names[MCI_ARCHITECTURE] = "CPU Architecture";
  mdr_field_names[MCI_MODEL]        = "CPU Model Name";
  mdr_field_names[MCI_CLOCK_MHZ]    = "CPU Speed in Megahertz";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("CpuInfo_Category");

  mdr_refresh_rates();
}

mdr_cpuinfo_t::~mdr_cpuinfo_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_cpuinfo_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  mci_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_cpuinfo_t::mdr_field_value: key %s not found", key.c_str());

  switch(field_number) {
  case MCI_INSTANCE:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mci_instance;
    break;
  case MCI_ARCHITECTURE:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mci_architecture;
    break;
  case MCI_MODEL:
    value.mv_type = MVT_STRING;
    value.mv_s = &n->second.mci_model;
    break;
  case MCI_CLOCK_MHZ:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.mci_clock_MHz;
    break;
  }
  return value;
}

void mdr_cpuinfo_t::mdr_rates(void)
{
}

const mdr_keyset_t &mdr_cpuinfo_t::mdr_keyset(mdr_keyset_t &keyset)
{
  mci_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_cpuinfo_t::cpuinfo_remote_refresh(void)
{
  mdr_m_mm_t m_map;
  mdr_m_mm_node_t mn;
  mci_can_t can;
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
      case MCI_INSTANCE:
        can.mci_instance = s_map[i].mv_i32;
        break;
      case MCI_ARCHITECTURE:
        can.mci_architecture = *s_map[i].mv_s;
        break;
      case MCI_MODEL:
        can.mci_model = *s_map[i].mv_s;
        break;
      case MCI_CLOCK_MHZ:
        can.mci_clock_MHz = s_map[i].mv_i32;
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
