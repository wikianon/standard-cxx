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


#include "mdr_cpu.h"

namespace mdr {

mdr_cpu_t::mdr_cpu_t(const char *remote_host) :
  mdr_t(MDR_CPU_NAME, true, MCPU_FIELD_COUNT, false)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_cpu_t", remote_host, MCPU_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MCPU_USER_TICKS);
    mdr_support_field(MCPU_KERNEL_TICKS);
#if defined(_LINUX) || defined(_DARWIN) || defined(_FREEBSD)
    mdr_support_field(MCPU_NICE_TICKS);
#endif
    if ((mdr_os_version.is_solaris && (mdr_os_version.os_version < 600)) ||
        (mdr_os_version.is_linux && (mdr_os_version.os_version > 240)) ||
        mdr_os_version.is_aix) {
      mdr_support_field(MCPU_WAIT_TICKS);
    }
    mdr_support_field(MCPU_IDLE_TICKS);
    if (mdr_os_version.is_linux && (mdr_os_version.os_version > 240)) {
      mdr_support_field(MCPU_IRQ_TICKS);
      mdr_support_field(MCPU_SOFT_IRQ_TICKS);
    }
#if defined(_FREEBSD)
    mdr_support_field(MCPU_IRQ_TICKS);
#endif

    net_client = 0;
  }

  mdr_field_names[MCPU_USER_TICKS]     = "User CPU Ticks";
  mdr_field_names[MCPU_KERNEL_TICKS]   = "Kernel CPU Ticks";
  mdr_field_names[MCPU_WAIT_TICKS]     = "Wait CPU Ticks";
  mdr_field_names[MCPU_NICE_TICKS]     = "Nice CPU Ticks";
  mdr_field_names[MCPU_IDLE_TICKS]     = "Idle CPU Ticks";
  mdr_field_names[MCPU_IRQ_TICKS]      = "Hardware Interrupt Ticks";
  mdr_field_names[MCPU_SOFT_IRQ_TICKS] = "Software Interrupt Ticks";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("Cpu_Category");

  mdr_refresh_rates();
}

mdr_cpu_t::~mdr_cpu_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_cpu_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  mcpu_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_cpu_t::mdr_field_value: key %s not found", key.c_str());

  value.mv_type = MVT_UI32;

  switch(field_number) {
  case MCPU_USER_TICKS:
    value.mv_ui32 = n->second.mcpu_user_ticks;
    break;
  case MCPU_KERNEL_TICKS:
    value.mv_ui32 = n->second.mcpu_kernel_ticks;
    break;
  case MCPU_NICE_TICKS:
    value.mv_ui32 = n->second.mcpu_nice_ticks;
    break;
  case MCPU_WAIT_TICKS:
    value.mv_ui32 = n->second.mcpu_wait_ticks;
    break;
  case MCPU_IDLE_TICKS:
    value.mv_ui32 = n->second.mcpu_idle_ticks;
    break;
  case MCPU_IRQ_TICKS:
    value.mv_ui32 = n->second.mcpu_irq_ticks;
    break;
  case MCPU_SOFT_IRQ_TICKS:
    value.mv_ui32 = n->second.mcpu_soft_irq_ticks;
    break;
  }

  return value;
}

void mdr_cpu_t::mdr_rates(void)
{
  mcpu_node_t n;
  mcpu_node_t nn;
  mcpu_can_t *cp;
  mcpu_can_t *ocp;
  uint_t tmp_user_ticks;
  uint_t tmp_kernel_ticks;
  uint_t tmp_nice_ticks;
  uint_t tmp_wait_ticks;
  uint_t tmp_idle_ticks;
  uint_t tmp_irq_ticks;
  uint_t tmp_soft_irq_ticks;

  for(n=begin(); n != end(); n++) {
    cp = &n->second;
    nn = old.find(n->first);
    if (nn == old.end()) {
      // should not happen
      old[n->first].clear();
      continue;
    }
    ocp = &nn->second;

    tmp_user_ticks     = cp->mcpu_user_ticks;
    tmp_kernel_ticks   = cp->mcpu_kernel_ticks;
    tmp_nice_ticks     = cp->mcpu_nice_ticks;
    tmp_wait_ticks     = cp->mcpu_wait_ticks;
    tmp_idle_ticks     = cp->mcpu_idle_ticks;
    tmp_irq_ticks      = cp->mcpu_irq_ticks;
    tmp_soft_irq_ticks = cp->mcpu_soft_irq_ticks;

    cp->mcpu_user_ticks   =
      wrap_diff_32(cp->mcpu_user_ticks, ocp->mcpu_user_ticks);
    cp->mcpu_kernel_ticks =
      wrap_diff_32(cp->mcpu_kernel_ticks, ocp->mcpu_kernel_ticks);
    cp->mcpu_nice_ticks   =
      wrap_diff_32(cp->mcpu_nice_ticks, ocp->mcpu_nice_ticks);
    cp->mcpu_wait_ticks   =
      wrap_diff_32(cp->mcpu_wait_ticks, ocp->mcpu_wait_ticks);
    cp->mcpu_idle_ticks   =
      wrap_diff_32(cp->mcpu_idle_ticks, ocp->mcpu_idle_ticks);
    cp->mcpu_irq_ticks   =
      wrap_diff_32(cp->mcpu_irq_ticks, ocp->mcpu_irq_ticks);
    cp->mcpu_soft_irq_ticks   =
      wrap_diff_32(cp->mcpu_soft_irq_ticks, ocp->mcpu_soft_irq_ticks);

    ocp->mcpu_user_ticks     = tmp_user_ticks;
    ocp->mcpu_kernel_ticks   = tmp_kernel_ticks;
    ocp->mcpu_nice_ticks     = tmp_nice_ticks;
    ocp->mcpu_wait_ticks     = tmp_wait_ticks;
    ocp->mcpu_idle_ticks     = tmp_idle_ticks;
    ocp->mcpu_irq_ticks      = tmp_irq_ticks;
    ocp->mcpu_soft_irq_ticks = tmp_soft_irq_ticks;
  }
}

const mdr_keyset_t &mdr_cpu_t::mdr_keyset(mdr_keyset_t &keyset)
{
  mcpu_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_cpu_t::cpu_remote_refresh(void)
{
  mdr_m_mm_t m_map;
  mdr_m_mm_node_t mn;
  mcpu_can_t can;
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
      case MCPU_USER_TICKS:
        can.mcpu_user_ticks = s_map[i].mv_ui32;
        break;
      case MCPU_KERNEL_TICKS:
        can.mcpu_kernel_ticks = s_map[i].mv_ui32;
        break;
      case MCPU_WAIT_TICKS:
        can.mcpu_wait_ticks = s_map[i].mv_ui32;
        break;
      case MCPU_NICE_TICKS:
        can.mcpu_nice_ticks = s_map[i].mv_ui32;
        break;
      case MCPU_IDLE_TICKS:
        can.mcpu_idle_ticks = s_map[i].mv_ui32;
        break;
      case MCPU_IRQ_TICKS:
        can.mcpu_irq_ticks = s_map[i].mv_ui32;
        break;
      case MCPU_SOFT_IRQ_TICKS:
        can.mcpu_soft_irq_ticks = s_map[i].mv_ui32;
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
