/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the mdr library of The XE Toolkit.
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kstat.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/sockio.h>
#include <sys/stat.h>
#include <sys/swap.h>
#include <sys/file.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <sys/sysmacros.h>
#define _STRUCTURED_PROC 1
#include <sys/procfs.h>
#include <sys/dkio.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stropts.h>
#include <sys/stream.h>
#include <sys/stropts.h>
#include <sys/tihdr.h>
#include <sys/tiuser.h>
#include <inet/led.h>
#include <inet/mib2.h>
#include <netinet/igmp_var.h>
#include <mdr_misc.h>
#include <mdr_memory.h>
#include <mdr_cpu.h>
#include <mdr_netif.h>
#include <mdr_cache.h>
#include <mdr_pf.h>
#include <mdr_disk.h>
#include <mdr_mnt.h>
#include <mdr_ctlr.h>
#include <mdr_ip.h>
#include <mdr_tcp.h>
#include <mdr_udp.h>
#include <mdr_icmp.h>
#include <mdr_cpuinfo.h>
#include <mdr_mp.h>
#include <mdr_ps.h>
#include <mdr_thread.h>
#include <mdr_maps.h>
#include <mdr_exception.h>

using namespace mdr;

// for mib stuff
const char *DEV_ARP     = "/dev/arp";
const char *ARP_MODULE  = "arp";
const char *TCP_MODULE  = "tcp";
const char *UDP_MODULE  = "udp";
const char *ICMP_MODULE = "icmp";

// types
typedef std::set<std::string> del_set_t;
typedef del_set_t::iterator   del_node_t;

static inline double TIMESTRUC(timestruc_t &t)
{
  return (double) t.tv_sec + ((double) t.tv_nsec / 1000000000.0);
}

// mib stats got bigger with Solaris 11, maybe even with a patch of Solaris 10

#if OS_VERSION < 610
typedef uint64_t Counter64;
#endif

struct mib2_ip_v2_t : public mib2_ip_t {
	// grappled from Solaris 11 inet/mib2.h
        int     ipRouteAttributeSize;   /* Size of mib2_ipAttributeEntry_t */
        int     transportMLPSize;       /* Size of mib2_transportMLPEntry_t */
};

struct mib2_tcp_v2_t : public mib2_tcp_t {
        /*
         * fields from RFC 4022
         */
        /* total # of segments recv'd                           { tcp 17 } */
        Counter64       tcpHCInSegs;
        /* total # of segments sent                             { tcp 18 } */
        Counter64       tcpHCOutSegs;
};

struct mib2_udp_v2_t : public mib2_udp_t {
        /*
         * fields from RFC 4113
         */
        /* total # of UDP datagrams sent upstream               { udp 8 } */
        Counter64       udpHCInDatagrams;
        /* total # of dg's sent                                 { udp 9 } */
        Counter64       udpHCOutDatagrams;
};

class mib_stats_t {
public:
  mib_stats_t(mib2_ip_v2_t  *ip,  mib2_tcp_v2_t *tcp,
              mib2_udp_v2_t *udp, mib2_icmp_t *icmp);
 ~mib_stats_t();

  void mib_refresh(void);
  void mib_rates(void) { }
  void mib_refresh_rates(void)
  {
    mib_refresh();
    mib_rates();
  }
private:
  static os_version_t os_version;

  int            mib_sd;              // stream descriptor
  mib2_ip_v2_t  *mib_ip;              // IP data
  mib2_tcp_v2_t *mib_tcp;             // TCP data
  mib2_udp_v2_t *mib_udp;             // UDP data
  mib2_icmp_t   *mib_icmp;            // ICMP data
  char          *mib_trash;           // trash can
  int            mib_trash_size;      // size of the trash can
};

// statics
static kstat_ctl_t    *kc;
static pthread_mutex_t kc_mutex = PTHREAD_MUTEX_INITIALIZER;
static uint64_t        kc_update = 0;

static void
init_kstat(void)
{
  int i;
  struct timeval tv;

  pthread_mutex_lock(&kc_mutex);

  // throttle kstat_chain_update to prevent chaos
  gettimeofday(&tv, 0);
  uint64_t now = (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL);
  char *thresh_env = getenv("MDR_INTERVAL_THRESHOLD");
  uint64_t threshold = 1000;

  // allow setting of threshold through environment
  if (thresh_env != 0)
    threshold = strtoull(thresh_env, 0, 0);

  // do not update more than once per threshold
  if ((now - kc_update) >= threshold) {

    if (kc == 0) {
      kc = kstat_open();
      if (kc == 0) {
        pthread_mutex_unlock(&kc_mutex);
        throw mdr_exception_t("init_kstat: kstat_open returns nil");
      }
      return;
    }

    for(; (i = kstat_chain_update(kc)) != 0; ) {
      switch (i) {
      case -1:
        pthread_mutex_unlock(&kc_mutex);
        throw mdr_exception_t("init_kstat: kstat_chain_update fails");
        break;
      default:
#if I_WANT_TO_SEE_STATE_CHANGES
        puts("<<<<<< STATE CHANGE >>>>>>");
#endif
        break;
      }
    }

    kc_update = now;
  }

  // mutex unlock is done by call to done_kstat()
}

static void
done_kstat(void)
{
  pthread_mutex_unlock(&kc_mutex);
}

class p2i_info_t {
public:
  int         p_instance;
  std::string p_prefix;

  p2i_info_t()
  {
    clear();
  }

  p2i_info_t(int inst, char *pref)
  {
    p_instance = inst;
    p_prefix = pref;
  }

  void clear(void)
  {
    p_instance = 0;
    p_prefix.clear();
  }
};

typedef std::map<std::string, p2i_info_t> p2i_map_t;
typedef p2i_map_t::iterator               p2i_node_t;

class charlie_map_t : public std::map<std::string, std::string> {
public:
  charlie_map_t();

  const char *long_name(const char *s);
  std::string &long_name(std::string &s);
};

typedef charlie_map_t::iterator charlie_node_t;

charlie_map_t::charlie_map_t()
{
  // build the path_to_inst map
  char *path;
  const char *disk_dirs[] = {
    "/dev/dsk", "/dev/osa/dev/dsk", 0
  };
  p2i_map_t p2i_map;
  file_map pi("/etc/path_to_inst", " \t\"");
  mdr_mnt_t mnt;

  // populate the p2i_map
  for(int i=0; i<(int)pi.size(); i++) {
    switch(pi[i].size()) {
    case 0:
    case 1:
    case 2:
      continue;
    default:
      break;
    }
    path = pi[i][0].token();
    if (*path == '#')
      continue;
    if (strncmp(path, "/node", 5) == 0)
      path += 5;  // skip path /node
    p2i_info_t pinfo(pi[i][1].to_uint32(), pi[i][2].token());
    p2i_map[path] = pinfo;
  }

  // for each disk directory
  for(const char **dirs = disk_dirs; *dirs; dirs++) {
    dir_list_t dsk(*dirs);

    // for each file in the directory
    for(dir_node_t node=dsk.begin(); node != dsk.end(); node++) {
      int n;
      int length = (*node).length();
      char full[BUFSIZ];
      char short_name[BUFSIZ];
      char points_at[BUFSIZ];

      // just lookup the *s0 devices
      if ((*node).substr(length - 2, length) != "s0")
        continue;

      // build the full path for readlink
      snprintf(full, sizeof full, "%s/%s", *dirs, (*node).c_str());
      n = readlink(full, points_at, sizeof points_at);
      if (n == -1)
        continue;
      points_at[n] = '\0';

      // chop off the :a at the end... and then...
      // remove the ../../devices from the beginning
      std::string dev(points_at);

      length = dev.length();
      dev = dev.substr(0, length - 2);
      dev = dev.substr(13, dev.length());

      // look it up in p2i_map; if not there, hard luck.
      p2i_node_t pnode = p2i_map.find(dev);
      if (pnode == p2i_map.end())
        continue;
      p2i_info_t *ip = &pnode->second;

      snprintf(short_name, sizeof short_name, "%s%d",
        ip->p_prefix.c_str(), ip->p_instance);

      // lop off the s0 at the end
      length = (*node).length();
      dev = (*node).substr(0, length - 2);

      (*this)[short_name] = dev;
    }
  }

  // got all the /dev/dsk, etc. devices; now hunt down partitions from kstat
  init_kstat();

  for(kstat_t *kp=kc->kc_chain; kp; kp=kp->ks_next) {
    // just looking for IO nodes, thanks.
    if (kp->ks_type != KSTAT_TYPE_IO)
      continue;

    // fix up nfs names too
    if (strcmp(kp->ks_module, "nfs") == 0) {
      uint64_t dev_no = strtoull(&kp->ks_name[3], 0, 10);

      for(mmnt_node_t mn=mnt.begin(); mn != mnt.end(); mn++) {
        mmnt_can_t *cp = &mn->second;
        uint64_t fsid = cp->mmnt_file_system_id;

        if ((cp->mmnt_fs_type == "nfs") && ((fsid & 0xFFFF) == dev_no)) {
          (*this)[kp->ks_name] = cp->mmnt_special;
          break;
        }
      }
    } else {
      char ln[BUFSIZ];
      char name[KSTAT_STRLEN];

      strcpy(name, kp->ks_name);
      char *p = strchr(name, ',');
      if (p == 0)
        continue;
      if (strlen(p+1) > 1)
        continue;
      *p++ = '\0';
      *p = '0' + (*p - 'a');
      strcpy(ln, long_name(name));
      if (strcmp(ln, name) == 0)
        continue; // hmmm.
      std::string part(ln);
      part += "s";
      part += p;
      (*this)[kp->ks_name] = part;
    }
  }

  done_kstat();
}

const char *charlie_map_t::long_name(const char *s)
{
  charlie_node_t n = find(s);

  if (n == end())
    return s;
  return n->second.c_str();
}

std::string &charlie_map_t::long_name(std::string &s)
{
  charlie_node_t n = find(s);

  if (n == end())
    return s;
  return n->second;
}

static bool
get_named_value(kstat_t *kp, const char *name[], ulong_t &value)
{
  kstat_named_t *kn = 0;

  // find the first one
  for(int i=0; name[i]; i++) {
    kn = (kstat_named_t *) kstat_data_lookup(kp, (char *) name[i]);
    if (kn)
      break;
  }

  // not there
  if (kn == 0) {
    value = 0;
    return false;
  }

  switch(kn->data_type) {
  case KSTAT_DATA_INT32:
    value = (ulong_t) kn->value.i32;
    break;
  case KSTAT_DATA_UINT32:
    value = (ulong_t) kn->value.ui32;
    break;
  case KSTAT_DATA_INT64:
    value = (ulong_t) kn->value.i64;
    break;
  case KSTAT_DATA_UINT64:
    value = (ulong_t) kn->value.ui64;
    break;
  default:
    return false;
  }

  return true;
}

static bool
get_named_value(kstat_t *kp, const char *name, ulong_t &value)
{
  kstat_named_t *kn = (kstat_named_t *) kstat_data_lookup(kp, (char *) name);

  if (kn == 0)
    return false;

  switch(kn->data_type) {
  case KSTAT_DATA_INT32:
    value = (ulong_t) kn->value.i32;
    break;
  case KSTAT_DATA_UINT32:
    value = (ulong_t) kn->value.ui32;
    break;
  case KSTAT_DATA_INT64:
    value = (ulong_t) kn->value.i64;
    break;
  case KSTAT_DATA_UINT64:
    value = (ulong_t) kn->value.ui64;
    break;
  default:
    return false;
  }

  return true;
}

static bool
get_named_value(kstat_t *kp, const char *name, int &value)
{
  kstat_named_t *kn = (kstat_named_t *) kstat_data_lookup(kp, (char *) name);

  if (kn == 0)
    return false;

  switch(kn->data_type) {
  case KSTAT_DATA_INT32:
    value = kn->value.i32;
    break;
  case KSTAT_DATA_UINT32:
    value = (int) kn->value.ui32;
    break;
  case KSTAT_DATA_INT64:
    value = (int) kn->value.i64;
    break;
  case KSTAT_DATA_UINT64:
    value = (int) kn->value.ui64;
    break;
  default:
    return false;
  }

  return true;
}

static bool
get_named_value(kstat_t *kp, const char *name, uint_t &value)
{
  kstat_named_t *kn = (kstat_named_t *) kstat_data_lookup(kp, (char *) name);

  if (kn == 0)
    return false;

  switch(kn->data_type) {
  case KSTAT_DATA_INT32:
    value = (uint_t) kn->value.i32;
    break;
  case KSTAT_DATA_UINT32:
    value = (uint_t) kn->value.ui32;
    break;
  case KSTAT_DATA_INT64:
    value = (uint_t) kn->value.i64;
    break;
  case KSTAT_DATA_UINT64:
    value = (uint_t) kn->value.ui64;
    break;
  default:
    return false;
  }

  return true;
}

static bool
get_named_value(kstat_t *kp, const char *name, double &value)
{
  kstat_named_t *kn = (kstat_named_t *) kstat_data_lookup(kp, (char *) name);

  if (kn == 0)
    return false;

  switch(kn->data_type) {
  case KSTAT_DATA_INT32:
    value = (double) kn->value.i32;
    break;
  case KSTAT_DATA_UINT32:
    value = (double) kn->value.ui32;
    break;
  case KSTAT_DATA_INT64:
    value = (double) kn->value.i64;
    break;
  case KSTAT_DATA_UINT64:
    value = (double) kn->value.ui64;
    break;
  default:
    return false;
  }

  return true;
}

static bool
get_named_value(kstat_t *kp, const char *name, long &value)
{
  kstat_named_t *kn = (kstat_named_t *) kstat_data_lookup(kp, (char *) name);

  if (kn == 0)
    return false;

  switch(kn->data_type) {
  case KSTAT_DATA_INT32:
    value = (long) kn->value.i32;
    break;
  case KSTAT_DATA_UINT32:
    value = (long) kn->value.ui32;
    break;
  case KSTAT_DATA_INT64:
    value = (long) kn->value.i64;
    break;
  case KSTAT_DATA_UINT64:
    value = (long) kn->value.ui64;
    break;
  default:
    return false;
  }

  return true;
}

#ifndef KSTAT_DATA_STRING
# define KSTAT_DATA_STRING 9
#endif

static bool
get_named_value(kstat_t *kp, const char *name, std::string &value)
{
  kstat_named_t *kn = (kstat_named_t *) kstat_data_lookup(kp, (char *) name);

  if (kn == 0) {
    value = "unknown";
    return false;
  }

  switch(kn->data_type) {
  case KSTAT_DATA_CHAR:
    value = (const char *) &kn->value.c[0];
    break;
  case KSTAT_DATA_STRING:
    // must coddle revs without string
    value = (const char *) kn->value.ul;
    break;
  default:
    return false;
  }

  return true;
}

void mdr_misc_t::mdr_refresh(void)
{
  if (net_client) {
    misc_remote_refresh();
    return;
  }

  bool misc = false;
  kstat_t *kp;
  static char hostname[256];
  static char platform[256];
  static char version[_SYS_NMLN];
  static char parch[256];
  static uint_t physmem;
  static int pgsz;
  static int cpus_conf;
  static int hz;

  if (hz == 0) {
    struct utsname u;

    physmem = (uint_t) sysconf(_SC_PHYS_PAGES);
    pgsz = (int) sysconf(_SC_PAGE_SIZE);
    cpus_conf = (int) sysconf(_SC_NPROCESSORS_CONF);
    hz = (int) sysconf(_SC_CLK_TCK);
    gethostname(hostname, sizeof hostname);
    uname(&u);
    strcpy(platform, u.sysname);
    strcpy(version, u.release);
    strcpy(parch, u.machine);
  }

  init_kstat();

  mm_hostname              = hostname;
  mm_os_platform           = platform;
  mm_os_version            = version;
  mm_physical_pages        = physmem;
  mm_native_page_size      = pgsz;
  mm_processor_arch        = parch;
  mm_processors_configured = cpus_conf;
  mm_processors_online     = (int) sysconf(_SC_NPROCESSORS_ONLN);
  mm_ticks_per_second      = hz;

  // init before loop
  mm_interrupts       = 0;
  mm_system_calls     = 0;
  mm_context_switches = 0;

  for(kp=kc->kc_chain; kp; kp=kp->ks_next) {
    if ((kp->ks_type == KSTAT_TYPE_RAW) &&
        (strcmp(kp->ks_module, "cpu_stat") == 0)) {
      kstat_read(kc, kp, 0);
      cpu_stat *cs = (cpu_stat *) kp->ks_data;
      mm_interrupts       += cs->cpu_sysinfo.intr;
      mm_system_calls     += cs->cpu_sysinfo.syscall;
      mm_context_switches += cs->cpu_sysinfo.pswitch;
      continue;
    }
   
    if (!misc && (kp->ks_type == KSTAT_TYPE_NAMED) &&
        (strcmp(kp->ks_name, "system_misc") == 0)) {
      kstat_read(kc, kp, 0);
      get_named_value(kp, "clk_intr",      mm_clock_interrupts);
      get_named_value(kp, "nproc",         mm_process_count);
      get_named_value(kp, "avenrun_1min",  mm_1min_run_queue);
      get_named_value(kp, "avenrun_5min",  mm_5min_run_queue);
      get_named_value(kp, "avenrun_15min", mm_15min_run_queue);
      get_named_value(kp, "boot_time",     mm_boot_time);

      mm_1min_run_queue  /= 256.0;
      mm_5min_run_queue  /= 256.0;
      mm_15min_run_queue /= 256.0;

      misc = true;
    }
  }
  mm_current_time = time(0);

  done_kstat();
}

void mdr_memory_t::mdr_refresh(void)
{
  if (net_client) {
    memory_remote_refresh();
  } else {
    kstat_t *kp;
    bool pages = false;
    static long pgk = sysconf(_SC_PAGE_SIZE) / 1024;

    init_kstat();

    mmem_paged_in_K = 0;
    mmem_paged_out_K = 0;
    mmem_page_faults = 0;
    mmem_major_faults = 0;
    mmem_pages_scanned = 0;

    for(kp=kc->kc_chain; kp; kp=kp->ks_next) {
      if ((kp->ks_type == KSTAT_TYPE_RAW) &&
          (strcmp(kp->ks_module, "cpu_stat") == 0)) {
        kstat_read(kc, kp, 0);
        cpu_stat *cs = (cpu_stat *) kp->ks_data;
        mmem_paged_in_K  += (cs->cpu_vminfo.pgpgin * pgk);
        mmem_paged_out_K += (cs->cpu_vminfo.pgpgout * pgk);
        mmem_major_faults += cs->cpu_vminfo.maj_fault;
        // track minor faults in mmem_page_faults and add to major later
        mmem_page_faults += (cs->cpu_vminfo.as_fault +
                             cs->cpu_vminfo.hat_fault);
        mmem_pages_scanned += cs->cpu_vminfo.scan;
      }

      if (!pages && (kp->ks_type == KSTAT_TYPE_NAMED) &&
          (strcmp(kp->ks_name, "system_pages") == 0)) {
        kstat_read(kc, kp, 0);
        get_named_value(kp, "freemem", mmem_free_memory_K);
        mmem_free_memory_K *= pgk;
        pages = true;
      }
    }
    mmem_page_faults += mmem_major_faults;

    done_kstat();
  }

  if (mmem_free_memory_K > old_free_memory_K)
    mmem_memory_freed_K = mmem_free_memory_K - old_free_memory_K;
  else
    mmem_memory_freed_K = 0;

  old_free_memory_K = mmem_free_memory_K;
}

void mdr_cpu_t::mdr_refresh(void)
{
  mcpu_node_t n;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    cpu_remote_refresh();
  } else {
    mcpu_can_t can;
    kstat_t *kp;
    char name[64];

    init_kstat();

    // just start over every time
    clear();

    for(kp=kc->kc_chain; kp; kp=kp->ks_next) {
      if ((kp->ks_type == KSTAT_TYPE_RAW) &&
          (strcmp(kp->ks_module, "cpu_stat") == 0)) {
        kstat_read(kc, kp, 0);
        cpu_stat *cs = (cpu_stat *) kp->ks_data;

        can.mcpu_user_ticks   = cs->cpu_sysinfo.cpu[CPU_USER];
        can.mcpu_kernel_ticks = cs->cpu_sysinfo.cpu[CPU_KERNEL];
        can.mcpu_idle_ticks   = cs->cpu_sysinfo.cpu[CPU_IDLE];
        can.mcpu_wait_ticks   = cs->cpu_sysinfo.cpu[CPU_WAIT];

        snprintf(name, sizeof name, "cpu%d", kp->ks_instance);
        (*this)[name] = can;
      }
    }

    done_kstat();
  }

  // now that the "now" is built, make sure there are old nodes for them
  for(n=begin(); n != end(); n++)
    if (old.find(n->first) == old.end())
      old[n->first].clear();

  // now find "old" nodes that no longer exist and delete them
  for(n=old.begin(); n != old.end(); n++)
    if (find(n->first) == end())
      del_set.insert(n->first);

  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    n = old.find(*sn);
    old.erase(n);
  }
}

int mdr_netif_t::get_address_info(const char *name, mnet_can_t *cp)
{
  int sd;
  struct sockaddr_in *sin;
  struct ifreq ifr;
  struct in_addr address;
  static int requests[] = {
   SIOCGIFADDR, SIOCGIFNETMASK, SIOCGIFBRDADDR, SIOCGIFMTU
  };
  unsigned int i;

  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd == -1)
    return -1;

  for(i=0; i<(sizeof(requests)/sizeof(int)); i++) {
    memset(&ifr, '\0', sizeof(ifr));
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, name, sizeof(ifr.ifr_name));
    if (ioctl(sd, requests[i], &ifr) == -1)
      memset(&ifr, '\0', sizeof ifr);

    sin = (struct sockaddr_in *) &ifr.ifr_addr;
    address.s_addr = sin->sin_addr.s_addr;

    switch(i) {
    case 0:
      cp->mnet_ip_address = inet_ntoa(address);
      break;
    case 1:
      cp->mnet_netmask = inet_ntoa(address);
      break;
    case 2:
      cp->mnet_broadcast_address = inet_ntoa(address);
      break;
    case 3:
      cp->mnet_mtu = ifr.ifr_metric;
      break;
    }
  }

  close(sd);

  return 0;
}

static bool
it_looks_like_a_netif(kstat_t *kp)
{
  int i;
  int n;
  int count = 0;
  kstat_named_t *kn;

  // cut down on kstat_reads
  n = strlen(kp->ks_module) - 1;
  if (strncmp(kp->ks_module, kp->ks_name, n) != 0)
    return false;
  if (!isdigit(kp->ks_name[strlen(kp->ks_name) - 1]))
    return false;
  if ((strcmp(kp->ks_module, "chip") == 0) ||
      (strcmp(kp->ks_module, "cpu_info") == 0))
    return false;

  kstat_read(kc, kp, 0);
  for(i=0, kn=(kstat_named_t *) kp->ks_data; i<(int)kp->ks_ndata; i++) {
    if ((strcmp(kn[i].name, "ipackets") == 0) ||
        (strcmp(kn[i].name, "opackets") == 0))
      count++;
    if (count == 2)
      return true;
  }
  return false;
}

void mdr_netif_t::mdr_refresh(void)
{
  mnet_node_t n;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    netif_remote_refresh();
  } else {
    mnet_can_t can;
    kstat_t *kp;
    static const char *ibyte_names[] = {
  #ifdef _LP64
        "rbytes64",
  #endif
        "rbytes",
        "ioctets",
        "ifInOctets",
        0
    };
    static const char *obyte_names[] = {
  #ifdef _LP64
        "obytes64",
  #endif
        "obytes",
        "xbytes",
        "ooctets",
        "ifOutOctets",
        0
    };
    static const char *ifail_names[] = {
        "nocanput",
        "cannotputs",
        "ifInDiscards",
        "norcvbuf",
        "rx_nocanput",
        0
    };
    static const char *ofail_names[] = {
        "defer",
        "defer_xmts",
        "dot3StatsDeferredTransmissions",
        "excess_defer",
        "nobuffer",
        "notbufs",
        "oxmtbuf",
        "outoftbuf",
        "tx_full",
        "tx_nocanput",
        "txringfull",
        0
    };
    static const char *speed_names[] = {
        "ifspeed",
        "ifSpeed",
        0
    };
    std::string string_duplex;
    ulong_t ulong_duplex;

    init_kstat();

    // start fresh
    clear();

    for(kp=kc->kc_chain; kp; kp=kp->ks_next) {
      if ((kp->ks_type != KSTAT_TYPE_NAMED) || !it_looks_like_a_netif(kp))
        continue;

      can.clear();
      get_address_info(kp->ks_name, &can);

      get_named_value(kp, "ipackets", can.mnet_input_packets);
      get_named_value(kp, "opackets", can.mnet_output_packets);

      // if it's not the loopback
      if (strcmp(kp->ks_module, "lo")) {
        get_named_value(kp, ibyte_names, can.mnet_input_bytes);
        get_named_value(kp, obyte_names, can.mnet_output_bytes);
        get_named_value(kp, "ierrors", can.mnet_input_errors);
        get_named_value(kp, "oerrors", can.mnet_output_errors);
        get_named_value(kp, ifail_names, can.mnet_input_failures);
        get_named_value(kp, ofail_names, can.mnet_output_failures);
        get_named_value(kp, speed_names, can.mnet_interface_speed);
        get_named_value(kp, "collisions", can.mnet_collisions);

        switch(can.mnet_interface_speed) {
        case 10:
          can.mnet_interface_speed = 10000000;
          break;
        case 100:
          can.mnet_interface_speed = 100000000;
          break;
        case 1000:
          can.mnet_interface_speed = 1000000000;
          break;
        default:
          break;
        }

        can.mnet_full_duplex = false;

        if (get_named_value(kp, "duplex", string_duplex)) {
          if (string_duplex == "full")
            can.mnet_full_duplex = true;
        } else {
          if (get_named_value(kp, "link_duplex", ulong_duplex))
            can.mnet_full_duplex = (ulong_duplex != 0);
        }

        if (can.mnet_output_packets > 0)
          can.mnet_collision_percent =
            (100.0 * can.mnet_collisions) / (double) can.mnet_output_packets;
        else
          can.mnet_collision_percent = 0.0;

      }

      (*this)[kp->ks_name] = can;
    }

    done_kstat();
  }

  // now that the "now" is built, make sure there are old nodes for them
  for(n=begin(); n != end(); n++)
    if (old.find(n->first) == old.end())
      old[n->first].clear();

  // now find "old" nodes that no longer exist and delete them
  for(n=old.begin(); n != old.end(); n++)
    if (find(n->first) == end())
      del_set.insert(n->first);

  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    n = old.find(*sn);
    old.erase(n);
  }
}

void mdr_cache_t::mdr_refresh(void)
{
  mc_node_t n;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    cache_remote_refresh();
  } else {
    init_kstat();

    kstat_t *buff =
      kstat_lookup(kc, (char *) "unix", 0, (char *) "biostats");
    kstat_t *inod =
      kstat_lookup(kc, (char *) "ufs",  0, (char *) "inode_cache");
    kstat_t *dnlc =
      kstat_lookup(kc, (char *) "unix", 0, (char *) "dnlcstats");
    const char *buffer = "buffer";
    const char *inode = "inode";
    const char *name = "name";
    mc_can_t *cp;
    double total;

    kstat_read(kc, buff, 0);
    (*this)[buffer].clear();
    n = find(buffer);
    cp = &n->second;
    get_named_value(buff, "buffer_cache_lookups", total);
    get_named_value(buff, "buffer_cache_hits", cp->mc_hits);
    cp->mc_misses = (uint_t) (total - cp->mc_hits);
    if (total > 0.0)
      cp->mc_hit_percent = (cp->mc_hits * 100.0) / total;
    else
      cp->mc_hit_percent = 100.0;

    kstat_read(kc, inod, 0);
    (*this)[inode].clear();
    n = find(inode);
    cp = &n->second;
    get_named_value(inod, "hits", cp->mc_hits);
    get_named_value(inod, "misses", cp->mc_misses);
    total = cp->mc_hits + cp->mc_misses;
    if (total > 0.0)
      cp->mc_hit_percent = (cp->mc_hits * 100.0) / total;
    else
      cp->mc_hit_percent = 100.0;

    kstat_read(kc, dnlc, 0);
    (*this)[name].clear();
    n = find(name);
    cp = &n->second;
    get_named_value(dnlc, "hits", cp->mc_hits);
    get_named_value(dnlc, "misses", cp->mc_misses);
    total = cp->mc_hits + cp->mc_misses;
    if (total > 0.0)
      cp->mc_hit_percent = (cp->mc_hits * 100.0) / total;
    else
      cp->mc_hit_percent = 100.0;

    done_kstat();
  }

  // now that the "now" is built, make sure there are old nodes for them
  for(n=begin(); n != end(); n++)
    if (old.find(n->first) == old.end())
      old[n->first].clear();

  // now find "old" nodes that no longer exist and delete them
  for(n=old.begin(); n != old.end(); n++)
    if (find(n->first) == end())
      del_set.insert(n->first);

  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    n = old.find(*sn);
    old.erase(n);
  }
}

void mdr_pf_t::mdr_refresh(void)
{
  mpf_node_t n;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    pf_remote_refresh();
  } else {
    int i;
    int sz;
    int nswap;
    swaptbl_t *st;
    static const int PATHSIZE = 1024;
    static long pgk = sysconf(_SC_PAGE_SIZE) / 1024;
    mpf_can_t can;

    // cop the # of swap devices
    nswap = swapctl(SC_GETNSWP, 0);
    if (nswap == -1)
      throw mdr_exception_t("mdr_pf_t::mdr_refresh: cannot SC_GETNSWP");

    // new the entries
    sz = sizeof(int) + (nswap * sizeof(swapent_t));
    st = (swaptbl_t *) malloc(sz);
    if (st == 0)
      throw mdr_exception_t("mdr_pf_t::mdr_refresh: malloc failed");
    memset(st, '\0', sz);

    // now, malloc the strings (gak!)
    for(i=0; i<nswap; i++)
      st->swt_ent[i].ste_path = new char [PATHSIZE];

    st->swt_n = nswap;

    if (swapctl(SC_LIST, st) == -1)
      throw mdr_exception_t("mdr_pf_t::mdr_refresh: cannot SC_LIST");

    // start over, eh?
    clear();

    for(i=0; i<nswap; i++) {
      can.mpf_size_K = st->swt_ent[i].ste_pages * pgk;
      can.mpf_free_K = st->swt_ent[i].ste_free * pgk;
      can.mpf_used_K = can.mpf_size_K - can.mpf_free_K;
      (*this)[st->swt_ent[i].ste_path] = can;
      delete[] st->swt_ent[i].ste_path;
    }
    free(st);
  }

  // now that the "now" is built, make sure there are old nodes for them
  for(n=begin(); n != end(); n++)
    if (old.find(n->first) == old.end())
      old[n->first].clear();

  // now find "old" nodes that no longer exist and delete them
  for(n=old.begin(); n != old.end(); n++)
    if (find(n->first) == end())
      del_set.insert(n->first);

  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    n = old.find(*sn);
    old.erase(n);
  }
}

typedef std::map<std::string, uint_t> space_map_t;
typedef space_map_t::iterator sm_node_t;

static space_map_t     space_map;
static pthread_mutex_t space_map_lock = PTHREAD_MUTEX_INITIALIZER;

void mdr_disk_t::mdr_refresh(void)
{
  del_set_t del_set;
  del_node_t sn;
  md_node_t node;
  del_set_t current_set;

  if (net_client) {
    disk_remote_refresh();
  } else {
    char *p;
    kstat_t *kp;
    kstat_io_t *kio;
    md_can_t can;
    const char *name;
    const char *type;
    char path[BUFSIZ];
    static long hz = sysconf(_SC_CLK_TCK);
    static charlie_map_t cmap;

    init_kstat();

    // start from nothing, again
    // clear();

    for(kp=kc->kc_chain; kp; kp=kp->ks_next) {
      if (kp->ks_type != KSTAT_TYPE_IO)
        continue;

      can.clear();

      // skip usb devices, etc.
      p = strchr(kp->ks_name, ',');
      if (p)
        if (strlen(p+1) > 1)
          continue;

      // skip stale NFS mounts
      if (kstat_read(kc, kp, 0) == -1)
        continue;

      kio = (kstat_io_t *) kp->ks_data;
      name = cmap.long_name(kp->ks_name);

      // find this node in the current map
      node = find(name);

      // Not there. Compute non-changing values.
      if (node == end()) {
        // use check from above
        if (p) {
          type = "logical";
          mmnt_node_t mn = mounts->find(name);
          if (mn == mounts->end()) {
            snprintf(path, sizeof path, "/dev/dsk/%s", name);
            mpf_node_t pn = swaps->find(path);
            if (pn != swaps->end())
              can.md_space_mb = pn->second.mpf_size_K / 1024;
          } else
            can.md_space_mb = mn->second.mmnt_size_K / 1024;
        } else if (strcmp(kp->ks_module, "nfs") == 0) {
          type = "network";
          mmnt_node_t mn = mounts->find(name);
          if (mn != mounts->end())
            can.md_space_mb = mn->second.mmnt_size_K / 1024;
        } else if (strcmp(kp->ks_module, "md") == 0) {
          // XXX: TODO - md_space_mb on meta devices
          type = "logical";
        } else {
          type = "physical";
          pthread_mutex_lock(&space_map_lock);
          sm_node_t smn = space_map.find(name);
          if (smn == space_map.end()) {
            for(int i=0; i<8; i++) {
              // find a /dev/rdsk file to query
              snprintf(path, sizeof path, "/dev/rdsk/%ss%d", name, i);

              // if it's writable
              if (access(path, W_OK) == 0) {
                // open it for write
                struct dk_geom dkg;
                struct dk_cinfo dki;
                struct dk_minfo dkmi;
                int input = open(path, O_RDWR | O_NDELAY);

                // ok then, don't.
                if (input == -1) {
                  continue;
                }

                // get the dkinfo
                if (ioctl(input, DKIOCINFO, &dki) == -1) {
                  close(input);
                  continue;
                }

                // don't query cdroms
                if (dki.dki_ctype == DKC_CDROM) {

                  // get media info to see if it's a removable disk
                  if (ioctl(input, DKIOCGMEDIAINFO, &dkmi) == -1)
                    dkmi.dki_media_type = DK_UNKNOWN;

                  // if it's a removable disk masquerading as a cdrom
                  // or it's a pcmcia memory stick masquerading as a floppy
                  // then skip it.
                  if (((dki.dki_ctype == DKC_CDROM) &&
                       (dkmi.dki_media_type != DK_ZIP) &&
                       (dkmi.dki_media_type != DK_JAZ)) ||
                       (dki.dki_ctype == DKC_PCMCIA_MEM)) {
                    close(input);
                    break;
                  }
                }

                // get the geometry
                if (ioctl(input, DKIOCGGEOM, &dkg) == -1) {
                  close(input);
                  continue;
                }

                // done with this
                close(input);

                uint64_t space = dkg.dkg_ncyl + dkg.dkg_acyl; // cylinders
                space *= dkg.dkg_nsect;                       // sectors/track
                space *= dkg.dkg_nhead;                       // heads
                space *= 512;                                 // bytes/sector
                space /= (1024*1024);
                can.md_space_mb = (uint_t) space;
                space_map[name] = can.md_space_mb;
                break;
              }
            }
          } else
            can.md_space_mb = smn->second;

          pthread_mutex_unlock(&space_map_lock);
        }

        can.md_type = type;
        (*this)[name] = can;
        node = find(name);
      }

      node->second.md_reads            = kio->reads;
      node->second.md_read_K           = (uint_t) (kio->nread / 1024);
      node->second.md_read_msecs       = 0;
      node->second.md_writes           = kio->writes;
      node->second.md_written_K        = (uint_t) (kio->nwritten / 1024);
      node->second.md_write_msecs      = 0;
      node->second.md_run_msecs        = kio->rtime / 1000000;
      node->second.md_wait_msecs       = kio->wtime / 1000000;
      node->second.md_runq_msecs       = kio->rlentime / 1000000;
      node->second.md_waitq_msecs      = kio->wlentime / 1000000;
      node->second.md_avg_queue_length = kio->wlentime / 1000000;
      node->second.md_io_operations    = kio->reads + kio->writes;

      // keep track of what disks we know are there
      current_set.insert(name);
    }

    done_kstat();
  }

  // We know what *is* there. Now remove any disks in the current set
  // that are *not* there.

  for(node=begin(); node != end(); node++) {
    // if it's not in current_set, then mark it for deletion
    if (current_set.find(node->first) == current_set.end())
      del_set.insert(node->first);
    else
      // it exists. make sure there's an old node for it
      if (old.find(node->first) == old.end())
        old[node->first].clear();
  }

  // traverse the delete set and remove anything that is not there
  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    node = find(*sn);
    erase(node);
  }

  // clear deletion set
  del_set.clear();

  // now find "old" nodes that no longer exist and delete them
  for(node=old.begin(); node != old.end(); node++)
    if (find(node->first) == end())
      del_set.insert(node->first);

  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    node = old.find(*sn);
    old.erase(node);
  }
}

#define B2K(s,b) ((ulong_t) ((((double) (s) * (double) (b)) / 1024.0)))

void mdr_mnt_t::mdr_refresh(void)
{
  mmnt_node_t node;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    mnt_remote_refresh();
  } else {
    file_map mt("/etc/mnttab", " \t");
    mmnt_can_t can;
    statvfs_t vfsbuf;
    struct stat stbuf;
    ulong_t reserved;
    bool ok = false;

    clear();

    for(int i=0; i<(int)mt.size(); i++) {
      line_map &lm = mt[i];

      // skip blank lines
      if (lm.size() == 0)
        continue;

      char *special = lm[0].token();

      // skip comments
      if (special[0] == '#')
        continue;

      char *mount_point;
      char *fs_type;
      char *opts;
      long mount_time;

      mount_point = lm[1].token();
      fs_type = lm[2].token();
      opts = lm[3].token();
      mount_time = lm[4].to_long();

      can.clear();
      can.mmnt_special = special;
      can.mmnt_mount_point = mount_point;
      can.mmnt_fs_type = fs_type;
      can.mmnt_mount_options = opts;
      can.mmnt_mount_time = mount_time;

#if CHECK_NFS_BEFORE_STAT
      std::map<std::string, bool> host_stat;
      std::map<std::string, bool>::iterator hn;

      if (can.mmnt_fs_type == "nfs") try {
        static unsigned short nfs_port;
        struct sockaddr_in sa;
        struct hostent *hp;
        int sd;

        std::string host(can.mmnt_special);
        host = host.substr(0, host.find(':'));
        hn = host_stat.find(host);
        if (hn != host_stat.end()) {
          int how = (int) (hn->second == false);
          throw int(how);
        }

        if (nfs_port == 0) {
          struct servent *sp = getservbyname("nfsd", "tcp");

          if (sp == 0)
            throw int(-1);

          nfs_port = sp->s_port;
        }

        // get the host name from the special file and do a lookup
        // not in the host_stat map; figure it out
        hp = gethostbyname(host.c_str());
        if (hp == 0)
          throw int(-2);

        // set up the address, port, and address family
        memcpy(&sa.sin_addr.s_addr, hp->h_addr, hp->h_length);
        sa.sin_port = htons(nfs_port);
        sa.sin_family = hp->h_addrtype;

        // retrieve a handle to the transport provider
        sd = socket(AF_INET, SOCK_STREAM, PF_UNSPEC);
        if (sd == -1)
          throw int(-3);

        // set non-block
        int flags = fcntl(sd, F_GETFL, 0);
        if (flags == -1)
          throw int(-4);
        if (fcntl(sd, F_SETFL, flags | FNDELAY) == -1)
          throw int(-5);

        // establish an endpoint to the server through the transport provider
        int n = connect(sd, (struct sockaddr *) &sa, sizeof sa);

        // is it really not there?
        if (n == -1) {
          if (errno == EINPROGRESS) {
            fd_set mask;
            struct timeval tv;
            int s;
    
            FD_ZERO(&mask);
            FD_SET(sd, &mask);
            tv.tv_sec = 0;
            tv.tv_usec = 50000;  // 5 clock ticks

            switch(s = select(sd + 1, 0, &mask, 0, &tv)) {
            case 0:
            case -1:
              ok = false;
              break;
            default:
              ok = true;
              break;
            }
          } else
            throw int(-6);
        }

        close(sd);

        // ...remember...
        host_stat[host] = ok;

      } catch(int code) {
        // gdb the code at this point to see what happened
        ok = (code == 0);
      } else
        ok = true;
#else
      ok = true;
#endif

      // if it's there, then get info
      if (ok) {
        if (statvfs(can.mmnt_mount_point.c_str(), &vfsbuf) != -1) {
          can.mmnt_size_K = B2K(vfsbuf.f_blocks, vfsbuf.f_frsize);
          can.mmnt_free_K = B2K(vfsbuf.f_bfree, vfsbuf.f_frsize);
          can.mmnt_used_K = can.mmnt_size_K - can.mmnt_free_K;
          reserved = can.mmnt_free_K - B2K(vfsbuf.f_bavail, vfsbuf.f_frsize);
          if (can.mmnt_size_K == 0)
            can.mmnt_percent_used = 0.0;
          else
            can.mmnt_percent_used =
              ceil((can.mmnt_used_K * 100.0) / (can.mmnt_size_K - reserved));

          can.mmnt_files = vfsbuf.f_files;
          can.mmnt_free_files = vfsbuf.f_ffree;
        }
        if (stat(can.mmnt_mount_point.c_str(), &stbuf) != -1) {
          can.mmnt_major = getemajor(stbuf.st_rdev);
          can.mmnt_minor = geteminor(stbuf.st_rdev);
        }
      }

      line_map o((char *) can.mmnt_mount_options.c_str(), ",");

      for(int i=0; i<(int)o.size(); i++) {
        char *p = o[i].token();
        if (strcmp(p, "-") == 0)
          continue;
        if (strncmp(p, "dev=", 4) == 0)
          can.mmnt_file_system_id = strtoull(strchr(p, '=') + 1, 0, 16);
      }

      // make an index string
      std::string &spp = can.mmnt_special;
      std::string key;

      if (spp.find_first_of("/dev/") != -1) {
        key = spp.substr(spp.rfind('/') + 1, spp.length());
        if ((key[0] == 'd') && isdigit(key[1]))
          key = "m" + key;
      } else
        key = can.mmnt_mount_point;

      (*this)[key] = can;
    }
  }

  // now that the "now" is built, make sure there are old nodes for them
  for(node=begin(); node != end(); node++)
    if (old.find(node->first) == old.end())
      old[node->first].clear();

  // now find "old" nodes that no longer exist and delete them
  for(node=old.begin(); node != old.end(); node++)
    if (find(node->first) == end())
      del_set.insert(node->first);

  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    node = old.find(*sn);
    old.erase(node);
  }
}

void mdr_ctlr_t::mdr_refresh(void)
{
  mctl_node_t node;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    ctlr_remote_refresh();
  } else {
    int c;
    int t;
    int d;
    char cname[64];
    mctl_can_t *cp;
    md_node_t dn;
    md_can_t *dp;

    // start fresh
    clear();

    disk->mdr_refresh();

    // traverse the disks and accumulate totals for controllers
    for(dn=disk->begin(); dn != disk->end(); dn++) {
      dp = &dn->second;
      if (dp->md_type != "physical")
        continue;

      // yield the controller #
      if (sscanf(dn->first.c_str(), "c%dt%dd%d", &c, &t, &d) != 3)
        if (sscanf(dn->first.c_str(), "c%dd%d", &c, &d) != 2)
          continue;  // hard luck

      snprintf(cname, sizeof cname, "c%d", c);

      // find the node for this controller or make it and zero it if new
      node = find(cname);
      if (node == end()) {
        (*this)[cname].clear();
        node = find(cname);
      }
      cp = &node->second;

      // accumulate
      cp->mctl_reads             += dp->md_reads;
      cp->mctl_read_K            += dp->md_read_K;
      cp->mctl_read_msecs        += dp->md_read_msecs;
      cp->mctl_writes            += dp->md_writes;
      cp->mctl_written_K         += dp->md_written_K;
      cp->mctl_write_msecs       += dp->md_write_msecs;
      cp->mctl_run_msecs         += dp->md_run_msecs;
      cp->mctl_wait_msecs        += dp->md_wait_msecs;
      cp->mctl_runq_msecs        += dp->md_runq_msecs;
      cp->mctl_waitq_msecs       += dp->md_waitq_msecs;
      cp->mctl_avg_queue_length  += dp->md_avg_queue_length;
      cp->mctl_util_percent      += dp->md_util_percent;
      cp->mctl_service_time      += dp->md_service_time;
      cp->mctl_disk_count++;
    }
  }

  // now that the "now" is built, make sure there are old nodes for them
  for(node=begin(); node != end(); node++)
    if (old.find(node->first) == old.end())
      old[node->first].clear();

  // now find "old" nodes that no longer exist and delete them
  for(node=old.begin(); node != old.end(); node++)
    if (find(node->first) == end())
      del_set.insert(node->first);

  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    node = old.find(*sn);
    old.erase(node);
  }
}

os_version_t mib_stats_t::os_version;

mib_stats_t::mib_stats_t(mib2_ip_v2_t  *ip,  mib2_tcp_v2_t *tcp,
                         mib2_udp_v2_t *udp, mib2_icmp_t *icmp)
{
  mib_sd = open(DEV_ARP, O_RDWR);
  if (mib_sd == -1)
    throw mdr_exception_t(
      "mib_stats_t::mib_stats_t: cannot open %s", DEV_ARP);

  if (ioctl(mib_sd, I_PUSH, TCP_MODULE) == -1)
    throw mdr_exception_t(
      "mib_stats_t::mib_stats_t: cannot push %s", TCP_MODULE);
  if (ioctl(mib_sd, I_PUSH, UDP_MODULE) == -1)
    throw mdr_exception_t(
      "mib_stats_t::mib_stats_t: cannot push %s", UDP_MODULE);
  if (os_version.os_version >= 580)
    if (ioctl(mib_sd, I_PUSH, ICMP_MODULE) == -1)
      throw mdr_exception_t(
        "mib_stats_t::mib_stats_t: cannot push %s", ICMP_MODULE);

  // ip module already in the stream

  mib_ip = ip;
  mib_tcp = tcp;
  mib_udp = udp;
  mib_icmp = icmp;
  mib_trash = 0;
  mib_trash_size = 0;

  mib_refresh();
}

mib_stats_t::~mib_stats_t()
{
  if (mib_sd != -1)
    close(mib_sd);
  if (mib_trash)
    delete[] mib_trash;
}

void mib_stats_t::mib_refresh(void)
{
  try {
    char buf[8192];
    int flags;
    int n;
    void *p;
    struct strbuf control;
    struct strbuf data;
    struct T_optmgmt_req *req_opt = (struct T_optmgmt_req *) buf;
    struct T_optmgmt_ack *ack_opt = (struct T_optmgmt_ack *) buf;
    struct T_error_ack   *err_opt = (struct T_error_ack *)   buf;
    struct opthdr *req_hdr;
  
    req_opt->PRIM_type = T_OPTMGMT_REQ;
    req_opt->OPT_offset = sizeof(struct T_optmgmt_req);
    req_opt->OPT_length = sizeof(struct opthdr);
    req_opt->MGMT_flags = T_CURRENT;
  
    req_hdr = (struct opthdr *) &req_opt[1];
    req_hdr->level = MIB2_IP;
    req_hdr->name  = 0;
    req_hdr->len   = 0;
  
    control.buf = buf;
    control.len = req_opt->OPT_length + req_opt->OPT_offset;
    if (putmsg(mib_sd, &control, 0, 0) == -1)
      throw int(-1);  // -1: cannot send control message
  
    req_hdr = (struct opthdr *) &ack_opt[1];
    control.maxlen = sizeof buf;
  
    for (;;) {
      flags = 0;
      n = getmsg(mib_sd, &control, 0, &flags);
      if (n == -1)
        throw int(-2);  // -2: cannot read control message
  
      // end of data?
      if ((n == 0) && (control.len >= (int)sizeof(struct T_optmgmt_ack)) &&
          (ack_opt->PRIM_type == T_OPTMGMT_ACK) &&
          (ack_opt->MGMT_flags == T_SUCCESS) &&
          (req_hdr->len == 0))
        break;
  
      if ((control.len >= (int)sizeof(struct T_error_ack)) &&
           err_opt->PRIM_type == T_ERROR_ACK)
        throw int(-3);  // -3: error reading control message
  
      if ((n != MOREDATA) ||
        (control.len < (int)sizeof(struct T_optmgmt_ack)) ||
           (ack_opt->PRIM_type != T_OPTMGMT_ACK) ||
           (ack_opt->MGMT_flags != T_SUCCESS))
        throw int(-4);  // -4: invalid control message received
  
      // cause the default case to happen
      if (req_hdr->name != 0)
        req_hdr->level = 0xffffffff;
  
      switch(req_hdr->level) {
      case MIB2_IP:
        if (mib_ip == 0)
          goto garbage;
        p = (void *) mib_ip;
        break;
      case MIB2_ICMP: 
        if (mib_icmp == 0)
          goto garbage;
        p = (void *) mib_icmp;
        break;
      case MIB2_TCP: 
        if (mib_tcp == 0)
          goto garbage;
        p = (void *) mib_tcp;
        break;
      case MIB2_UDP: 
        if (mib_udp == 0)
          goto garbage;
        p = (void *) mib_udp;
        break;
      default:
garbage:
        if ((mib_trash == 0) || ((int)req_hdr->len > mib_trash_size)) {
          if (mib_trash)
            delete[] mib_trash;
          mib_trash = new char[req_hdr->len];
          mib_trash_size = req_hdr->len;
        }
        p = (void *) mib_trash;
        break;
      }
  
      data.maxlen = req_hdr->len;
      data.buf    = (char *) p;
      data.len    = 0;
      flags = 0;
  
      n = getmsg(mib_sd, 0, &data, &flags);
      if (n != 0)
        throw int(-5);  // -5: error reading data
    }
  } catch(int code) {
    // memset this
    if (mib_ip)
      memset(mib_ip, '\0', sizeof(mib2_ip_v2_t));
    if (mib_tcp)
      memset(mib_tcp, '\0', sizeof(mib2_tcp_v2_t));
    if (mib_udp)
      memset(mib_udp, '\0', sizeof(mib2_udp_v2_t));
    if (mib_icmp)
      memset(mib_icmp, '\0', sizeof(mib2_icmp_t));
  }
}

class mib_source_t : public mib_stats_t {
public:
  mib_source_t() : mib_stats_t(&ip, &tcp, &udp, &icmp) {
    static pthread_mutex_t mux_init = PTHREAD_MUTEX_INITIALIZER;
    struct timeval tv;

    gettimeofday(&tv, 0);
    mib_mutex = mux_init;
    mib_update = (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL);
  }

  void refresh() {
    pthread_mutex_lock(&mib_mutex);

    struct timeval tv;

    // throttle mib update to prevent chaos
    gettimeofday(&tv, 0);
    uint64_t now = (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL);
    char *thresh_env = getenv("MDR_INTERVAL_THRESHOLD");
    uint64_t threshold = 1000;

    // allow setting of threshold through environment
    if (thresh_env != 0)
      threshold = strtoull(thresh_env, 0, 0);

    // do not update more than once per threshold
    if ((now - mib_update) >= threshold) {
      mib_refresh();
      mib_update = now;
    }

    pthread_mutex_unlock(&mib_mutex);
  }

  void get_ip_stats(mib2_ip_v2_t *ipp) {
    pthread_mutex_lock(&mib_mutex);
    if (ipp)
      *ipp = ip;
    pthread_mutex_unlock(&mib_mutex);
  }

  void get_tcp_stats(mib2_tcp_v2_t *tcpp) {
    pthread_mutex_lock(&mib_mutex);
    if (tcpp)
      *tcpp = tcp;
    pthread_mutex_unlock(&mib_mutex);
  }

  void get_udp_stats(mib2_udp_v2_t *udpp) {
    pthread_mutex_lock(&mib_mutex);
    if (udpp)
      *udpp = udp;
    pthread_mutex_unlock(&mib_mutex);
  }

  void get_icmp_stats(mib2_icmp_t *icmpp) {
    pthread_mutex_lock(&mib_mutex);
    if (icmpp)
      *icmpp = icmp;
    pthread_mutex_unlock(&mib_mutex);
  }

private:
  pthread_mutex_t mib_mutex;
  uint64_t        mib_update;
  mib2_ip_v2_t    ip;
  mib2_tcp_v2_t   tcp;
  mib2_udp_v2_t   udp;
  mib2_icmp_t     icmp;
};

// mutex is contained within the class
static mib_source_t mib_source;

void mdr_ip_t::mdr_refresh(void)
{
  if (net_client) {
    ip_remote_refresh();
    return;
  }

  mib2_ip_v2_t ip;

  mib_source.refresh();
  mib_source.get_ip_stats(&ip);

  mip_ipForwarding = ip.ipForwarding;
  mip_ipDefaultTTL = ip.ipDefaultTTL;
  mip_ipInReceives = ip.ipInReceives;
  mip_ipInHdrErrors = ip.ipInHdrErrors;
  mip_ipInAddrErrors = ip.ipInAddrErrors;
  mip_ipForwDatagrams = ip.ipForwDatagrams;
  mip_ipInUnknownProtos = ip.ipInUnknownProtos;
  mip_ipInDiscards = ip.ipInDiscards;
  mip_ipInDelivers = ip.ipInDelivers;
  mip_ipOutRequests = ip.ipOutRequests;
  mip_ipOutDiscards = ip.ipOutDiscards;
  mip_ipOutNoRoutes = ip.ipOutNoRoutes;
  mip_ipReasmTimeout = ip.ipReasmTimeout;
  mip_ipReasmReqds = ip.ipReasmReqds;
  mip_ipReasmOKs = ip.ipReasmOKs;
  mip_ipReasmFails = ip.ipReasmFails;
  mip_ipFragOKs = ip.ipFragOKs;
  mip_ipFragFails = ip.ipFragFails;
  mip_ipFragCreates = ip.ipFragCreates;
  mip_ipRoutingDiscards = ip.ipRoutingDiscards;
}

void mdr_tcp_t::mdr_refresh(void)
{
  if (net_client) {
    tcp_remote_refresh();
    return;
  }

  mib2_ip_v2_t ip;
  mib2_tcp_v2_t tcp;

  mib_source.refresh();
  mib_source.get_ip_stats(&ip);
  mib_source.get_tcp_stats(&tcp);

  mtcp_tcpRtoAlgorithm = tcp.tcpRtoAlgorithm;
  mtcp_tcpRtoMin = tcp.tcpRtoMin;
  mtcp_tcpRtoMax = tcp.tcpRtoMax;
  mtcp_tcpMaxConn = tcp.tcpMaxConn;
  mtcp_tcpActiveOpens = tcp.tcpActiveOpens;
  mtcp_tcpPassiveOpens = tcp.tcpPassiveOpens;
  mtcp_tcpAttemptFails = tcp.tcpAttemptFails;
  mtcp_tcpEstabResets = tcp.tcpEstabResets;
  mtcp_tcpCurrEstab = tcp.tcpCurrEstab;
  mtcp_tcpInSegs = tcp.tcpInSegs;
  mtcp_tcpOutSegs = tcp.tcpOutSegs;
  mtcp_tcpRetransSegs = tcp.tcpRetransSegs;
  mtcp_tcpInErrs = ip.tcpInErrs;
  mtcp_tcpOutRsts = tcp.tcpOutRsts;
}

void mdr_udp_t::mdr_refresh(void)
{
  if (net_client) {
    udp_remote_refresh();
    return;
  }

  mib2_ip_v2_t ip;
  mib2_udp_v2_t udp;

  mib_source.refresh();
  mib_source.get_ip_stats(&ip);
  mib_source.get_udp_stats(&udp);

  mudp_udpInDatagrams = udp.udpInDatagrams;
  mudp_udpNoPorts = ip.udpNoPorts;
  mudp_udpInErrors = udp.udpInErrors;
  mudp_udpOutDatagrams = udp.udpOutDatagrams;
}

void mdr_icmp_t::mdr_refresh(void)
{
  if (net_client) {
    icmp_remote_refresh();
    return;
  }

  mib2_icmp_t icmp;

  mib_source.refresh();
  mib_source.get_icmp_stats(&icmp);

  micmp_icmpInMsgs = icmp.icmpInMsgs;
  micmp_icmpInErrors = icmp.icmpInErrors;
  micmp_icmpInDestUnreachs = icmp.icmpInDestUnreachs;
  micmp_icmpInTimeExcds = icmp.icmpInTimeExcds;
  micmp_icmpInParmProbs = icmp.icmpInParmProbs;
  micmp_icmpInSrcQuenchs = icmp.icmpInSrcQuenchs;
  micmp_icmpInRedirects = icmp.icmpInRedirects;
  micmp_icmpInEchos = icmp.icmpInEchos;
  micmp_icmpInEchoReps = icmp.icmpInEchoReps;
  micmp_icmpInTimestamps = icmp.icmpInTimestamps;
  micmp_icmpInTimestampReps = icmp.icmpInTimestampReps;
  micmp_icmpInAddrMasks = icmp.icmpInAddrMasks;
  micmp_icmpInAddrMaskReps = icmp.icmpInAddrMaskReps;
  micmp_icmpOutMsgs = icmp.icmpOutMsgs;
  micmp_icmpOutErrors = icmp.icmpOutErrors;
  micmp_icmpOutDestUnreachs = icmp.icmpOutDestUnreachs;
  micmp_icmpOutTimeExcds = icmp.icmpOutTimeExcds;
  micmp_icmpOutParmProbs = icmp.icmpOutParmProbs;
  micmp_icmpOutSrcQuenchs = icmp.icmpOutSrcQuenchs;
  micmp_icmpOutRedirects = icmp.icmpOutRedirects;
  micmp_icmpOutEchos = icmp.icmpOutEchos;
  micmp_icmpOutEchoReps = icmp.icmpOutEchoReps;
  micmp_icmpOutTimestamps = icmp.icmpOutTimestamps;
  micmp_icmpOutTimestampReps = icmp.icmpOutTimestampReps;
  micmp_icmpOutAddrMasks = icmp.icmpOutAddrMasks;
  micmp_icmpOutAddrMaskReps = icmp.icmpOutAddrMaskReps;
}

void mdr_cpuinfo_t::mdr_refresh(void)
{
  if (net_client) {
    cpuinfo_remote_refresh();
    return;
  }

  kstat_t *kp;
  mci_can_t can;
  char name[64];
  mci_node_t n;
  del_set_t del_set;
  del_node_t sn;

  init_kstat();

  for(kp=kc->kc_chain; kp; kp=kp->ks_next) {
    if (kp->ks_type != KSTAT_TYPE_NAMED)
      continue;
    if (strcmp(kp->ks_module, "cpu_info"))
      continue;

    kstat_read(kc, kp, 0);

    can.clear();
    can.mci_instance = kp->ks_instance;
    get_named_value(kp, "cpu_type", can.mci_architecture);
    if (mdr_os_version.os_version >= 600)
      get_named_value(kp, "brand", can.mci_model);
    else
      get_named_value(kp, "implementation", can.mci_model);
    get_named_value(kp, "clock_MHz", can.mci_clock_MHz);

    snprintf(name, sizeof name, "cpu%d", kp->ks_instance);
    (*this)[name] = can;
  }

  done_kstat();

  // now that the "now" is built, make sure there are old nodes for them
  for(n=begin(); n != end(); n++)
    if (old.find(n->first) == old.end())
      old[n->first].clear();

  // now find "old" nodes that no longer exist and delete them
  for(n=old.begin(); n != old.end(); n++)
    if (find(n->first) == end())
      del_set.insert(n->first);

  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    n = old.find(*sn);
    old.erase(n);
  }
}

void mdr_mp_t::mdr_refresh(void)
{
  mmp_node_t n;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    mp_remote_refresh();
  } else {
    mmp_can_t can;
    kstat_t *kp;
    char name[64];

    init_kstat();

    // just start over every time
    clear();

    for(kp=kc->kc_chain; kp; kp=kp->ks_next) {
      if ((kp->ks_type == KSTAT_TYPE_RAW) &&
          (strcmp(kp->ks_module, "cpu_stat") == 0)) {
        kstat_read(kc, kp, 0);
        cpu_stat *cs = (cpu_stat *) kp->ks_data;

        can.clear();
        can.mmp_cpu_instance = kp->ks_instance;
        can.mmp_user_ticks   = cs->cpu_sysinfo.cpu[CPU_USER];
        can.mmp_kernel_ticks = cs->cpu_sysinfo.cpu[CPU_KERNEL];
        can.mmp_idle_ticks   = cs->cpu_sysinfo.cpu[CPU_IDLE];
        can.mmp_wait_ticks   = cs->cpu_sysinfo.cpu[CPU_WAIT];

        snprintf(name, sizeof name, "cpu%d", kp->ks_instance);
        (*this)[name] = can;
      }
    }

    done_kstat();
  }

  // now that the "now" is built, make sure there are old nodes for them
  for(n=begin(); n != end(); n++)
    if (old.find(n->first) == old.end())
      old[n->first].clear();

  // now find "old" nodes that no longer exist and delete them
  for(n=old.begin(); n != old.end(); n++)
    if (find(n->first) == end())
      del_set.insert(n->first);

  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    n = old.find(*sn);
    old.erase(n);
  }
}

template <typename proc_struct>
bool get_proc_file(const char *prefix, const char *fname, proc_struct &st)
{
  char path[128];
  int input;
  int n;

  snprintf(path, sizeof path, "%s/%s", prefix, fname);
  input = open(path, O_RDONLY);
  if (input == -1)
    return false;
  n = read(input, &st, sizeof(proc_struct));
  close(input);
  return n > 0;
}

void mdr_ps_t::mdr_refresh(void)
{
  del_set_t del_set;
  del_node_t sn;
  mps_node_t node;
  del_set_t current_set;

  if (net_client) {
    ps_remote_refresh();
  } else {
    dir_list_t proc("/proc");
    dir_node_t n;
    char path[BUFSIZ];
    char name[128];
    const char *cmd;
    char *fq_cmd;
    mps_can_t can;
    psinfo_t pi;
    prusage_t pu;

    if (!proc)
      throw mdr_exception_t("mdr_ps_t::mdr_refresh: cannot opendir /proc");

    mps_procs_runnable = 0;
    mps_procs_sleeping = 0;
    mps_procs_stopped = 0;
    mps_procs_blocked = 0;
    mps_procs_zombie = 0;
    mps_procs_specific = 0;

    // start over
    // clear();

    for(n=proc.begin(); n != proc.end(); n++) {

      snprintf(path, sizeof path, "/proc/%s", n->c_str());
      if (!get_proc_file(path, "psinfo", pi) ||
          !get_proc_file(path, "usage", pu))
        continue;

      // this is the key: file_name[process_id]
      snprintf(name, sizeof name, "%s[%d]", pi.pr_fname, pi.pr_pid);

      // see if it's already there
      node = find(name);
      if (node == end()) {

        // start clean
        can.clear();

        // collect values that do not change
        can.mps_start = pi.pr_lwp.pr_start.tv_sec;
        can.mps_pid = pi.pr_pid;
        can.mps_ppid = pi.pr_ppid;
        can.mps_file_name = pi.pr_fname;
        can.mps_tty_device = pi.pr_ttydev;
        can.mps_command = pi.pr_psargs;

        // zombies have no name...
        if (can.mps_command.length() == 0 && pi.pr_lwp.pr_sname == 'Z') {
          can.mps_file_name = "<unreaped>";
          can.mps_command = can.mps_file_name;
        }

        // snatch up argv[0] from psargs
        memset(path, '\0', sizeof path);
        strncpy(path, can.mps_command.c_str(), sizeof path - 1);
        fq_cmd = strtok(path, " \t");
        if (fq_cmd) {
          cmd = strrchr(fq_cmd, '/');
          if (cmd == 0)
            cmd = fq_cmd;
          else
            cmd++;
        } else
          cmd = (char *) can.mps_command.c_str();

        can.mps_first_argument = cmd;

        (*this)[name] = can;
        node = find(name);
      }

      mps_can_t *cp = &node->second;

      // collect values that change
      cp->mps_uid = pi.pr_uid;
      cp->mps_euid = pi.pr_euid;
      if (cp->mps_pid != 0) {
        // sched always reports 100% utilization
        cp->mps_user_time = TIMESTRUC(pu.pr_utime);
        cp->mps_kernel_time = TIMESTRUC(pu.pr_stime);
        cp->mps_cpu_time = TIMESTRUC(pi.pr_time);
      }
      cp->mps_size_K = pi.pr_size;
      cp->mps_rssize_K = pi.pr_rssize;
      cp->mps_io_K = pu.pr_ioch / 1024;
      cp->mps_memory_percent =
        (pi.pr_rssize * 100.0) / (double) mps_total_mem_K;
      cp->mps_system_calls = pu.pr_sysc;
      cp->mps_voluntary_csw = pu.pr_vctx;
      cp->mps_involuntary_csw = pu.pr_ictx;
      cp->mps_disk_faults = pu.pr_majf;
      cp->mps_page_reclaims = pu.pr_minf;
      cp->mps_state = pi.pr_lwp.pr_sname;
      switch(cp->mps_state) {
      case 'O':
      case 'R':
        cp->mps_state = MPS_RUNNABLE;
        mps_procs_runnable++;
        break;
      case 'S':
        cp->mps_state = MPS_SLEEPING;
        mps_procs_sleeping++;
        break;
      case 'T':
        cp->mps_state = MPS_STOPPED;
        mps_procs_stopped++;
        break;
      case 'Z':
        cp->mps_state = MPS_ZOMBIE;
        mps_procs_zombie++;
        break;
      default:
        cp->mps_state = MPS_SPECIFIC;
        mps_procs_specific++;
        break;
      }
      cp->mps_priority = pi.pr_lwp.pr_pri;
      cp->mps_nice = pi.pr_lwp.pr_nice;
      cp->mps_thread_count = pi.pr_nlwp;

      cp->mps_user_name = uid_lookup(cp->mps_uid);
      cp->mps_effective_user_name = uid_lookup(cp->mps_euid);
      cp->mps_is_system_process = (strcmp(pi.pr_lwp.pr_clname, "SYS") == 0);

      // keep track of what procs we know are there
      current_set.insert(name);
    }
  }

  // We know what *is* there. Now remove any procs in the current set
  // that are *not* there.

  for(node=begin(); node != end(); node++) {
    // if it's not in current_set, then mark it for deletion
    if (current_set.find(node->first) == current_set.end())
      del_set.insert(node->first);
    else
      // it exists. make sure there's an old node for it
      if (old.find(node->first) == old.end())
        old[node->first].clear();
  }

  // traverse the delete set and remove anything that is not there
  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    node = find(*sn);
    erase(node);
  }

  // clear deletion set
  del_set.clear();

  // now find "old" nodes that no longer exist and delete them
  for(node=old.begin(); node != old.end(); node++)
    if (find(node->first) == end())
      del_set.insert(node->first);

  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    node = old.find(*sn);
    old.erase(node);
  }
}

const int MAX_TT_LEN = 128;

struct tty_devs {
  const char  *td_device;
  const char  *td_name;
  int          td_major;
  void       (*td_compose)(char *, dev_t, struct tty_devs *);
};

static void
pts_compose(char *name, dev_t dev, struct tty_devs *tp)
{
  snprintf(name, MAX_TT_LEN, tp->td_name, geteminor(dev));
}

static void
tty_compose(char *name, dev_t dev, struct tty_devs *tp)
{
  snprintf(name, MAX_TT_LEN, tp->td_name, geteminor(dev));
}

static void
pty_compose(char *name, dev_t dev, struct tty_devs *tp)
{
  char p = 'p';

  snprintf(name, MAX_TT_LEN,
    tp->td_name, p + geteminor(dev) / 16, geteminor(dev) % 16);
}

std::string mdr_ps_t::mps_dev_to_tty(dev_t dev, std::string &result)
{
  static int initialized = 0;
  static struct tty_devs tty_map[] = {
    { "/dev/console",  "console",  0,  0           },
    { "/dev/pts/0",    "pts/%d",   0,  pts_compose },
    { "/dev/ttya",     "tty%c",    0,  tty_compose },
    { "/dev/ttyp0",    "tty%c%d",  0,  pty_compose },
    { 0,               0,          0,  0           }
  };
  char name[MAX_TT_LEN];
  struct tty_devs *tp;
  struct stat stbuf;

  if (initialized == 0) {
    for(tp=tty_map; tp->td_device; tp++) {
      if (stat(tp->td_device, &stbuf) == -1)
        continue;
      tp->td_major = getemajor(stbuf.st_rdev);
    }
    initialized = 1;
  }
  for(tp=tty_map; tp->td_device; tp++) {
    if ((int)getemajor(dev) == tp->td_major) {
      if (tp->td_compose) {
        (*tp->td_compose)(name, dev, tp);
        result = name;
        return result;
      }
      result = tp->td_name;
      return result;
    }
  }

  result = "?";
  return result;
}

void mdr_thread_t::mdr_refresh(void)
{
  del_set_t del_set;
  del_node_t sn;
  mt_node_t node;
  del_set_t current_set;

  if (net_client) {
    thread_remote_refresh();
  } else {
    dir_list_t proc("/proc");
    dir_list_t *lwp = 0;
    dir_node_t n;
    dir_node_t l;
    char lwppath[BUFSIZ];
    char path[BUFSIZ];
    char name[128];
    mt_can_t can;
    psinfo_t psi;
    lwpsinfo_t pi;
    prusage_t pu;

    if (!proc)
      throw mdr_exception_t(
        "mdr_thread_t::mdr_refresh: cannot opendir /proc");

    // start over
    // clear();

    for(n=proc.begin(); n != proc.end(); n++) {
      // paths to the process and its lwps
      snprintf(path, sizeof path, "/proc/%s", n->c_str());
      snprintf(lwppath, sizeof lwppath, "%s/lwp", path);

      // get lwp directory contents
      lwp = new dir_list_t(lwppath);
      if (!(*lwp)) {
        delete lwp;
        continue;
      }

      // get process psinfo for the process name
      if (!get_proc_file(path, "psinfo", psi))
        continue;

      for(l=lwp->begin(); l != lwp->end(); l++) {
        snprintf(path, sizeof path, "%s/%s", lwppath, l->c_str());

        if (!get_proc_file(path, "lwpsinfo", pi) ||
            !get_proc_file(path, "lwpusage", pu))
          continue;

        can.clear();
        can.mt_thread_id = pu.pr_lwpid;

        // this is the key: file_name[process_id][thread_id]
        snprintf(name, sizeof name, "%s[%d][%d]",
          psi.pr_fname, psi.pr_pid, can.mt_thread_id);

        node = find(name);
        if (node == end()) {
          (*this)[name] = can;
          node = find(name);
        }

        mt_can_t *cp = &node->second;

        if (psi.pr_pid != 0) {
          // sched always reports 100% utilization
          cp->mt_cpu_time = TIMESTRUC(pi.pr_time);
          cp->mt_user_time = TIMESTRUC(pu.pr_utime);
          cp->mt_kernel_time = TIMESTRUC(pu.pr_stime);
        }
        cp->mt_io_K = pu.pr_ioch / 1024;
        cp->mt_voluntary_csw = pu.pr_vctx;
        cp->mt_involuntary_csw = pu.pr_ictx;
        cp->mt_disk_faults = pu.pr_majf;
        cp->mt_page_reclaims = pu.pr_minf;
        cp->mt_state = pi.pr_sname;
        switch(cp->mt_state) {
        case 'O':
        case 'R':
          cp->mt_state = MPS_RUNNABLE;
          break;
        case 'S':
          cp->mt_state = MPS_SLEEPING;
          break;
        case 'T':
          cp->mt_state = MPS_STOPPED;
          break;
        case 'Z':
          cp->mt_state = MPS_ZOMBIE;
          break;
        default:
          cp->mt_state = MPS_SPECIFIC;
          break;
        }
        cp->mt_priority = pi.pr_pri;
        cp->mt_nice = pi.pr_nice;
        cp->mt_process_id = psi.pr_pid;

        current_set.insert(name);
      }

      if (lwp)
        delete lwp;
    }
  }

  // We know what *is* there. Now remove any threads in the current set
  // that are *not* there.

  for(node=begin(); node != end(); node++) {
    // if it's not in current_set, then mark it for deletion
    if (current_set.find(node->first) == current_set.end())
      del_set.insert(node->first);
    else
      // it exists. make sure there's an old node for it
      if (old.find(node->first) == old.end())
        old[node->first].clear();
  }

  // traverse the delete set and remove anything that is not there
  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    node = find(*sn);
    erase(node);
  }

  // clear deletion set
  del_set.clear();

  // now find "old" nodes that no longer exist and delete them
  for(node=old.begin(); node != old.end(); node++)
    if (find(node->first) == end())
      del_set.insert(node->first);

  for(sn=del_set.begin(); sn != del_set.end(); sn++) {
    node = old.find(*sn);
    old.erase(node);
  }
}
