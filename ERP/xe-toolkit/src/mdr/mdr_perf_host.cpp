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


#include "mdr_perf_host.h"
#include "mdr_category.h"

namespace mdr {

const mdr_keyset_t &
  mdr_performance_host_t::mdr_get_category_names()
{
  return category_names;
}

mdr_performance_host_t::mdr_performance_host_t(const char *host_name)
{
  const char **const names[] = {
#if !defined(_LINUX) && !defined(_AIX)
    &MDR_CACHE_NAME,
#endif
    &MDR_CPU_NAME,
    &MDR_CPUINFO_NAME,
    &MDR_CPUSTAT_NAME,
#ifndef _DARWIN
    &MDR_CTLR_NAME,
    &MDR_CTLRSTAT_NAME,
#endif
    &MDR_DISK_NAME,
    &MDR_ICMP_NAME,
    &MDR_IOSTAT_NAME,
    &MDR_IP_NAME,
    &MDR_MEMORY_NAME,
    &MDR_MISC_NAME,
    &MDR_MNT_NAME,
    &MDR_MP_NAME,
    &MDR_MPSTAT_NAME,
    &MDR_NETIF_NAME,
    &MDR_PF_NAME,
    &MDR_PS_NAME,
    &MDR_SYSTAT_NAME,
    &MDR_TCP_NAME,
    &MDR_THREAD_NAME,
    &MDR_UDP_NAME,
    &MDR_PROC_STATES_NAME,
    &MDR_NETSTAT_NAME
  };

  remote_host = host_name;

  if (remote_host) {
    mdr_s_mm_t s_map;
    size_t i;
    size_t n;

    net_client =
      new mdr_client_t("mdr_performance_host_t", remote_host, MPH_VERSION);

    net_client->mc_refresh(s_map);
    n = s_map.size();

    category_names.resize(n);

    for(i=0; i<n-1; i++)
      category_names[i] = *s_map[i].mv_s;

    mdr_os_flavor = (mdr_os_flavor_t) s_map[n-1].mv_i32;

    net_client->mc_release(s_map);
  } else {
    size_t size = sizeof names / sizeof(char **);

    category_names.resize(size);

    for(size_t i=0; i<size; i++)
      category_names[i] = *names[i];

#ifdef _LINUX
    mdr_os_flavor = MOS_LINUX;
#endif
#ifdef _DARWIN
    mdr_os_flavor = MOS_MACOS;
#endif
#ifdef _SOLARIS
    mdr_os_flavor = MOS_SOLARIS;
#endif
#ifdef _FREEBSD
    mdr_os_flavor = MOS_BSD;
#endif
#ifdef _AIX
    mdr_os_flavor = MOS_AIX;
#endif
#ifdef _HPUX
    mdr_os_flavor = MOS_HPUX;
#endif

    net_client = 0;
  }
}

mdr_performance_host_t::~mdr_performance_host_t()
{
  if (net_client)
    delete net_client;
}

mdr_category_t *
  mdr_performance_host_t::mdr_get_category_by_name(const char *name)
{
  if (name == 0)
    throw mdr_exception_t(
      "mdr_performance_host_t::mdr_get_category_by_name: null name");

  return new mdr_category_t(name, remote_host);
}

std::ostream &operator<<
  (std::ostream &out, const mdr_performance_host_t::mdr_os_flavor_t v)
{
  switch(v) {
  case mdr_performance_host_t::MOS_LINUX:
    out << "Linux";
    break;
  case mdr_performance_host_t::MOS_SOLARIS:
    out << "Solaris";
    break;
  case mdr_performance_host_t::MOS_AIX:
    out << "AIX";
    break;
  case mdr_performance_host_t::MOS_HPUX:
    out << "HP-UX";
    break;
  case mdr_performance_host_t::MOS_MACOS:
    out << "MacOS";
    break;
  case mdr_performance_host_t::MOS_BSD:
    out << "BSD";
    break;
  default:
    out << _GT_("Unknown: ") << (int) v;
    break;
  }

  return out;
}

std::string &mdr_performance_host_t::mdr_localized_name(
  std::string &en_us_name, std::string &localized_name)
{
  return mdr_t::mdr_localized_name(en_us_name, localized_name);
}

std::string &mdr_performance_host_t::mdr_en_us_name(
  std::string &localized_name, std::string &en_us_name)
{
  return mdr_t::mdr_en_us_name(localized_name, en_us_name);
}

}
