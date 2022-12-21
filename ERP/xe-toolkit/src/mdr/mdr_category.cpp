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


#include <sys/time.h>
#include "mdr_cache.h"
#include "mdr_misc.h"
#include "mdr_cpu.h"
#include "mdr_cpustat.h"
#include "mdr_cpuinfo.h"
#include "mdr_ctlr.h"
#include "mdr_ctlrstat.h"
#include "mdr_disk.h"
#include "mdr_icmp.h"
#include "mdr_iostat.h"
#include "mdr_ip.h"
#include "mdr_memory.h"
#include "mdr_misc.h"
#include "mdr_mnt.h"
#include "mdr_mp.h"
#include "mdr_mpstat.h"
#include "mdr_netif.h"
#include "mdr_pf.h"
#ifdef _AIX
# undef NOACCESS
#endif
#include "mdr_ps.h"
#include "mdr_systat.h"
#include "mdr_tcp.h"
#include "mdr_thread.h"
#include "mdr_udp.h"
#include "mdr_proc_states.h"
#include "mdr_netstat.h"
#include "mdr_category.h"

namespace mdr {

mdr_host_instance_map_t mdr_category_t::cats_per_host;
pthread_mutex_t         mdr_category_t::cats_mux = PTHREAD_MUTEX_INITIALIZER;

mdr_cat_refresh_times_t mdr_category_t::cat_times_map;
pthread_mutex_t         mdr_category_t::cat_times_mux = PTHREAD_MUTEX_INITIALIZER;

mdr_category_t::mdr_category_t(const char *name, const char *hostname)
{
  remote_host = hostname;

  pthread_mutex_lock(&cats_mux);
    const char *const localname = "localhost";
    bool localhost = (remote_host == 0);
    mdr_host_instance_node_t hnode =
      cats_per_host.find(localhost ? localname : remote_host);

    // assure that there is a host node for this request
    if (hnode == cats_per_host.end()) {
      cats_per_host[localhost ? localname : remote_host].clear();
      hnode = cats_per_host.find(localhost ? localname : remote_host);
    }
  pthread_mutex_unlock(&cats_mux);

  cat_inst_map = &hnode->second;

  mdr_cat_instance_node_t inode = cat_inst_map->find(name);

  if (inode == cat_inst_map->end()) {
    std::string cat(name);

    if (cat == MDR_CACHE_NAME)
      mdr_obj = new mdr_cache_t(remote_host);

    else if (cat == MDR_CPU_NAME)
      mdr_obj = new mdr_cpu_t(remote_host);

    else if (cat == MDR_CPUINFO_NAME)
      mdr_obj = new mdr_cpuinfo_t(remote_host);

    else if (cat == MDR_CPUSTAT_NAME)
      mdr_obj = new mdr_cpustat_t(remote_host);

    else if (cat == MDR_CTLR_NAME)
      mdr_obj = new mdr_ctlr_t(remote_host);

    else if (cat == MDR_CTLRSTAT_NAME)
      mdr_obj = new mdr_ctlrstat_t(remote_host);

    else if (cat == MDR_DISK_NAME)
      mdr_obj = new mdr_disk_t(remote_host);

    else if (cat == MDR_ICMP_NAME)
      mdr_obj = new mdr_icmp_t(remote_host);

    else if (cat == MDR_IOSTAT_NAME)
      mdr_obj = new mdr_iostat_t(remote_host);

    else if (cat == MDR_IP_NAME)
      mdr_obj = new mdr_ip_t(remote_host);

    else if (cat == MDR_MEMORY_NAME)
      mdr_obj = new mdr_memory_t(remote_host);

    else if (cat == MDR_MISC_NAME)
      mdr_obj = new mdr_misc_t(remote_host);

    else if (cat == MDR_MNT_NAME)
      mdr_obj = new mdr_mnt_t(remote_host);

    else if (cat == MDR_MP_NAME)
      mdr_obj = new mdr_mp_t(remote_host);

    else if (cat == MDR_MPSTAT_NAME)
      mdr_obj = new mdr_mpstat_t(remote_host);

    else if (cat == MDR_NETIF_NAME)
      mdr_obj = new mdr_netif_t(remote_host);

    else if (cat == MDR_PF_NAME)
      mdr_obj = new mdr_pf_t(remote_host);

    else if (cat == MDR_PS_NAME)
      mdr_obj = new mdr_ps_t(remote_host);

    else if (cat == MDR_SYSTAT_NAME)
      mdr_obj = new mdr_systat_t(remote_host);

    else if (cat == MDR_TCP_NAME)
      mdr_obj = new mdr_tcp_t(remote_host);

    else if (cat == MDR_THREAD_NAME)
      mdr_obj = new mdr_thread_t(remote_host);

    else if (cat == MDR_UDP_NAME)
      mdr_obj = new mdr_udp_t(remote_host);

    else if (cat == MDR_PROC_STATES_NAME)
      mdr_obj = new mdr_proc_states_t(remote_host);

    else if (cat == MDR_NETSTAT_NAME)
      mdr_obj = new mdr_netstat_t(remote_host);

    else throw mdr_exception_t(
      "mdr_category_t::mdr_category_t: invalid name: %s", name);

    (*cat_inst_map)[cat] = mdr_obj;
  } else {
    mdr_obj = inode->second;
    mdr_refresh();
  }

  mdr_category_name = name;
  mdr_category_supported = mdr_obj->mdr_category_supported;
  mdr_is_singleton = mdr_obj->mdr_is_singleton;
  mdr_field_count = mdr_obj->mdr_field_count;

  pthread_mutex_lock(&cat_times_mux);
    // track refresh times on each category and govern to >= 1/sec
    mdr_cat_refresh_node_t rn = cat_times_map.find(mdr_obj);
    if (rn == cat_times_map.end()) {
      struct timeval tv;

      gettimeofday(&tv, 0);
      // convert to milliseconds
      uint64_t now = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

      cat_times_map[mdr_obj] = now;
    }
  pthread_mutex_unlock(&cat_times_mux);
}

bool mdr_category_t::mdr_is_supported(int field_number)
{
  return mdr_obj->mdr_is_supported(field_number);
}

std::string &mdr_category_t::mdr_field_name(int field_number)
{
  return mdr_obj->mdr_field_name(field_number);
}

std::string &mdr_category_t::mdr_field_explanation(int field_number, std::string &explanation)
{
  return mdr_obj->mdr_field_explanation(field_number, explanation);
}

int mdr_category_t::mdr_field_name_to_number(const char *name)
{
  return mdr_obj->mdr_field_name_to_number(name);
}

mdr_keyset_t &mdr_category_t::mdr_all_field_names(mdr_keyset_t &keyset)
{
  return mdr_obj->mdr_all_field_names(keyset);
}

mdr_keyset_t &mdr_category_t::mdr_supported_field_names(mdr_keyset_t &keyset)
{
  return mdr_obj->mdr_supported_field_names(keyset);
}

const mdr_value_t &mdr_category_t::mdr_field_value(int field_number, mdr_value_t &value)
{
  return mdr_obj->mdr_field_value(field_number, value);
}

const mdr_value_t &
  mdr_category_t::mdr_field_value(const char *key, int field_number, mdr_value_t &value)
{
  return mdr_obj->mdr_field_value(key, field_number, value);
}

bool mdr_category_t::mdr_refresh(void)
{
  mdr_cat_refresh_node_t rn;
  struct timeval tv;
  bool updated = false;

  pthread_mutex_lock(&cat_times_mux);
    rn = cat_times_map.find(mdr_obj);
    gettimeofday(&tv, 0);
    // convert to milliseconds
    uint64_t now = (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000);
    char *thresh_env = getenv("MDR_INTERVAL_THRESHOLD");
    uint64_t threshold = 1000;

    // allow setting of threshold through environment
    if (thresh_env != 0)
      threshold = strtoull(thresh_env, 0, 0);

    // do not update category more than once per threshold
    if ((now - rn->second) >= threshold) {
      mdr_obj->mdr_refresh();
      cat_times_map[mdr_obj] = now;
      updated = true;
    }
  pthread_mutex_unlock(&cat_times_mux);

  return updated;
}

void mdr_category_t::mdr_rates(void)
{
  mdr_obj->mdr_rates();
}

const mdr_keyset_t &mdr_category_t::mdr_get_instance_names(mdr_keyset_t &keyset)
{
  if (mdr_obj->mdr_is_singleton)
    throw mdr_exception_t(
      "mdr_category_t::mdr_get_instance_names: %s is not an n-tuple",
        mdr_obj->mdr_category_name.c_str());

  return mdr_obj->mdr_keyset(keyset);
}

// release resources associated with this host
void mdr_category_t::mdr_release_host(const char *hostname)
{
  pthread_mutex_lock(&cats_mux);
    // find the host
    mdr_host_instance_node_t hnode =
      cats_per_host.find(hostname ? hostname : "localhost");
    if (hnode != cats_per_host.end()) {
      // release all of the categories for this host
      mdr_cat_instance_map_t &cmap = hnode->second;
      mdr_cat_instance_node_t cnode;

      for(cnode=cmap.begin(); cnode != cmap.end(); cnode++) {
        mdr_t *mdrp = cnode->second;

        if (mdrp)
          delete mdrp;
      }

      // leave the host node pointing to an empty map
      cmap.clear();
    }
  pthread_mutex_unlock(&cats_mux);
}

}
