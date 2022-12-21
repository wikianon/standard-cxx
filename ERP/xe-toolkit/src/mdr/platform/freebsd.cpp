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
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <sys/user.h>
#include <sys/vmmeter.h>
#include <sys/mount.h>
#include <sys/dkstat.h>
#include <sys/stat.h>
#include <sys/disk.h>
#include <sys/utsname.h>
#include <devstat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/namei.h>
#include <machine/cputypes.h>
#include <net/if.h>
#include <net/if_var.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_var.h>
#include <netinet/tcp_timer.h>
#include <netinet/tcp_var.h>
#include <netinet/udp.h>
#include <netinet/udp_var.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp_var.h>
#include <arpa/inet.h>
#include <nfs/nfsproto.h>
#include <nfs/rpcv2.h>
#include <set>
#include <mdr_kvm.h>
#include <mdr_misc.h>
#include <mdr_memory.h>
#include <mdr_cpu.h>
#include <mdr_netif.h>
#include <mdr_cache.h>
#include <mdr_pf.h>
#include <mdr_disk.h>
#include <mdr_ctlr.h>
#include <mdr_mp.h>
#include <mdr_mnt.h>
#include <mdr_cpuinfo.h>
#include <mdr_ip.h>
#include <mdr_tcp.h>
#include <mdr_udp.h>
#include <mdr_icmp.h>
#include <mdr_ps.h>
#include <mdr_thread.h>
#include <mdr_exception.h>

using namespace mdr;

typedef long cpu_time_t[CPUSTATES];
typedef struct kvm_swap kvm_swap_t;
typedef struct devinfo devinfo_t;
typedef struct statinfo statinfo_t;
typedef struct device_selection device_selection_t;
typedef struct devstat devstat_t;
typedef struct statfs statfs_t;
typedef char string20[20];
typedef char string128[128];
typedef struct ipstat ipstat_t;
typedef struct tcpstat tcpstat_t;
typedef struct icmpstat icmpstat_t;
typedef struct udpstat udpstat_t;

typedef std::set<std::string> del_set_t;
typedef del_set_t::iterator   del_node_t;

typedef std::map<std::string, std::string> node_map_t;
typedef node_map_t::iterator               node_node_t;

static kvm_t *kd;  // for use by all. too expensive to open close open close.

template <typename sys_type>
class mdr_sysctl_t {
private:
  sys_type value;
public:
  mdr_sysctl_t(int mib0, int mib1)
  {
    size_t len;
    int mib[4];

    mib[0] = mib0;
    mib[1] = mib1;
    len = sizeof(sys_type);
    if (sysctl(mib, 2, &value, &len, 0, 0) == -1)
      throw mdr_exception_t(
        "mdr_sysctl_t::mdr_sysctl_t: sysctl() failed");
  }

  mdr_sysctl_t(const char *byname)
  {
    size_t len = sizeof(sys_type);

    if (sysctlbyname(byname, &value, &len, 0, 0) == -1)
      throw mdr_exception_t(
        "mdr_sysctl_t::mdr_sysctl_t: sysctlbyname() failed");
  }

  const sys_type operator*(void)
  {
    return value;
  }

  const sys_type *operator->(void)
  {
    return &value;
  }

  // just gimme the value, ok?
  void *operator()(void)
  {
    return (void *) &value;
  }
};

// convert a timeval into a double as number of seconds
static inline double TIMEVAL(struct timeval &t)
{
  return (double) t.tv_sec + ((double) t.tv_usec / 1000000.0);
}

void mdr_misc_t::mdr_refresh(void)
{
  if (net_client) {
    misc_remote_refresh();
    return;
  }

  static mdr_sysctl_t<struct timeval> btime(CTL_KERN, KERN_BOOTTIME);
  static mdr_sysctl_t<unsigned long> pp(CTL_HW, HW_PHYSMEM);
  static mdr_sysctl_t<int> ncpu(CTL_HW, HW_NCPU);
  static long pgsz = sysconf(_SC_PAGESIZE);
  static long hz = sysconf(_SC_CLK_TCK);
  static mdr_kvm_t<int> ticks("ticks");
  static mdr_kvm_t<int> real_hertz("hz");
  static bool do_update = false;
  static char hostname[128];
  static struct utsname ubuf;
  double loadavg[3];
  double m = (double) hz / (double) *real_hertz;
  mdr_sysctl_t<uint_t> intr("vm.stats.sys.v_intr");
  mdr_sysctl_t<uint_t> syscall("vm.stats.sys.v_syscall");
  mdr_sysctl_t<uint_t> swtch("vm.stats.sys.v_swtch");

  if (do_update) {
    ticks.mk_refresh();
  } else {
    uname(&ubuf);
    gethostname(hostname, sizeof hostname);
    do_update = true;
  }

  if (getloadavg(loadavg, 3) == -1)
    throw mdr_exception_t("mdr_misc_t::mdr_refresh: getloadavg failed");

  if (kd == 0) {
    kd = kvm_open(0, 0, 0, O_RDONLY, 0);
    if (kd == 0)
      throw mdr_exception_t("mdr_misc_t::mdr_refresh: cannot kvm_open");
  }

  if (kvm_getprocs(kd, KERN_PROC_PROC, 0, (int *) &mm_process_count) == 0)
    throw mdr_exception_t("mdr_misc_t::mdr_refresh: kvm_getprocs fails");

  mm_hostname = hostname;
  mm_os_platform = ubuf.sysname;
  mm_os_version = ubuf.release;
  mm_physical_pages = *pp;
  mm_physical_pages /= pgsz;
  mm_native_page_size = pgsz;
  mm_processor_arch = ubuf.machine;
  mm_processors_configured = mm_processors_online = *ncpu;
  mm_ticks_per_second = hz;
  mm_boot_time = btime->tv_sec;

  mm_interrupts = *intr;
  mm_clock_interrupts = (unsigned int) ((double) *ticks * m);
  mm_system_calls = *syscall;
  mm_context_switches = *swtch;
  mm_1min_run_queue  = loadavg[0];
  mm_5min_run_queue  = loadavg[1];
  mm_15min_run_queue = loadavg[2];
  mm_current_time = time(0);
}

void mdr_memory_t::mdr_refresh(void)
{
  if (net_client) {
    memory_remote_refresh();
  } else {
    static mdr_sysctl_t<uint_t> page_size("vm.stats.vm.v_page_size");
    static long pgk = *page_size / 1024;
    mdr_sysctl_t<uint_t> free_count("vm.stats.vm.v_free_count");
    mdr_sysctl_t<uint_t> swappgsin("vm.stats.vm.v_swappgsin");
    mdr_sysctl_t<uint_t> swappgsout("vm.stats.vm.v_swappgsout");
    mdr_sysctl_t<uint_t> vnodepgsin("vm.stats.vm.v_vnodepgsin");
    mdr_sysctl_t<uint_t> vnodepgsout("vm.stats.vm.v_vnodepgsout");
    mdr_sysctl_t<uint_t> pdpages("vm.stats.vm.v_pdpages");
    mdr_sysctl_t<uint_t> vm_faults("vm.stats.vm.v_vm_faults");
    mdr_sysctl_t<uint_t> reactivated("vm.stats.vm.v_reactivated");

    mmem_free_memory_K = *free_count * pgk;
    mmem_paged_in_K    = (*swappgsin + *vnodepgsin) * pgk;
    mmem_paged_out_K   = (*swappgsout + *vnodepgsout) * pgk;
    mmem_pages_scanned = *pdpages;
    mmem_page_faults   = *vm_faults;
    mmem_major_faults  = mmem_page_faults - *reactivated;
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
    // FreeBSD has one CPU. Period.
    mdr_kvm_t<cpu_time_t> cp_time("cp_time");
    mcpu_can_t can;
    const char *cpu0 = "cpu0";

    can.clear();
    can.mcpu_user_ticks   = (*cp_time)[CP_USER] + (*cp_time)[CP_NICE];
    can.mcpu_kernel_ticks = (*cp_time)[CP_SYS] + (*cp_time)[CP_INTR];
    can.mcpu_nice_ticks   = (*cp_time)[CP_NICE];
    can.mcpu_idle_ticks   = (*cp_time)[CP_IDLE];
    can.mcpu_irq_ticks    = (*cp_time)[CP_INTR];
    (*this)[cpu0] = can;
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
    if (ioctl(sd, requests[i], &ifr) == -1) {
      close(sd);
      return -1;
    }

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

void mdr_netif_t::mdr_refresh(void)
{
  mnet_node_t n;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    netif_remote_refresh();
  } else {
    static mdr_kvm_t<struct ifnethead> ifh("_ifnet");
    struct ifnet *ifp;
    mnet_can_t can;
    char *if_name;

    // start fresh
    clear();

    ifp = TAILQ_FIRST(&(*ifh));
    while(ifp) {
      mdr_kvm_t<struct ifnet> iface((caddr_t) ifp);

      if (iface->if_flags & IFF_UP) {
        can.clear();
        if_name = iface->if_xname;
        get_address_info(if_name, &can);

        can.mnet_input_bytes     = iface->if_data.ifi_ibytes;
        can.mnet_output_bytes    = iface->if_data.ifi_obytes;
        can.mnet_input_packets   = iface->if_data.ifi_ipackets;
        can.mnet_output_packets  = iface->if_data.ifi_opackets;
        can.mnet_input_errors    = iface->if_data.ifi_ierrors;
        can.mnet_output_errors   = iface->if_data.ifi_oerrors;
        can.mnet_input_failures  = iface->if_data.ifi_iqdrops;
        can.mnet_output_failures = iface->if_data.ifi_noproto;
        can.mnet_collisions      = iface->if_data.ifi_collisions;
        can.mnet_interface_speed = iface->if_data.ifi_baudrate;

        (*this)[if_name] = can;
      }

      ifp = TAILQ_NEXT(&(*iface), if_link);
    }
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
    static mdr_kvm_t<struct nchstats> ncache("_nchstats");
    static bool do_update = false;
    static const char *dentry = "dentry";
    mc_can_t can;

    if (do_update) {
      ncache.mk_refresh();
    } else
      do_update = true;

    can.clear();

    can.mc_hits   = ncache->ncs_goodhits + ncache->ncs_neghits;
    can.mc_misses = ncache->ncs_miss;

    (*this)[dentry] = can;
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
    int n;
    mpf_can_t can;
    static kvm_swap_t *swinfo = 0;
    static int swsize = 0;
    static long pgk = sysconf(_SC_PAGESIZE) / 1024;
    const int SWINC = 16;

    if (swinfo == 0) {
      swinfo = new kvm_swap_t[SWINC];
      swsize = SWINC;
    }

    if (kd == 0) {
      kd = kvm_open(0, 0, 0, O_RDONLY, 0);
      if (kd == 0)
        throw mdr_exception_t("mdr_pf_t::mdr_refresh: cannot kvm_open");
    }

    for(;;) {
      n = kvm_getswapinfo(kd, swinfo, swsize, SWIF_DEV_PREFIX);
      if (n == -1)
        throw mdr_exception_t("mdr_pf_t::mdr_refresh: cannot kvm_getswapinfo");
      if (n == swsize) {
        delete[] swinfo;
        swsize += SWINC;
        swinfo = new kvm_swap_t[swsize];
        continue;
      }
      break;
    }

    clear();

    for(i=0; i<n; i++) {
      can.clear();
      can.mpf_size_K = swinfo[i].ksw_total * pgk;
      can.mpf_used_K = swinfo[i].ksw_used * pgk;
      can.mpf_free_K = can.mpf_size_K - can.mpf_used_K;
      (*this)[swinfo[i].ksw_devname] = can;
    }
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

static inline uint_t MSECS(bintime &bt)
{
  timeval tv;
  uint_t msecs;

  bintime2timeval(&bt, &tv);
  msecs = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
  return msecs;
}

typedef std::map<std::string, double> qmap_t;
typedef qmap_t::iterator qnode_t;

static qmap_t qmap;
static pthread_mutex_t qmux = PTHREAD_MUTEX_INITIALIZER;

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
    int i;
    int ndev;
    devinfo_t di;
    statinfo_t ci;
    devstat_t *dp;
    char disk_name[128];
    md_can_t can;
    del_set_t del_set;
    del_node_t sn;
    qnode_t qnode;
    int qlen;

    memset(&ci, '\0', sizeof ci);
    memset(&di, '\0', sizeof di);

    ci.dinfo = &di;
    if (devstat_getdevs(0, &ci) == -1)
      throw mdr_exception_t("mdr_disk_t::mdr_refresh: cannot getdevs");

    ndev = di.numdevs;

    pthread_mutex_lock(&qmux);
    for(i=0; i<ndev; i++) {
      dp = &di.devices[i];
      if (((dp->device_type & DEVSTAT_TYPE_IF_MASK) == DEVSTAT_TYPE_IF_OTHER) &&
          (dp->priority == DEVSTAT_PRIORITY_OTHER))
        continue;

      snprintf(disk_name, sizeof disk_name, "%s%d",
          dp->device_name, dp->unit_number);

      node = find(disk_name);
      if (node == end()) {
        can.clear();
        can.md_type = "physical";

        pthread_mutex_lock(&space_map_lock);

        sm_node_t smn = space_map.find(disk_name);
        if (smn == space_map.end()) {
          char path[BUFSIZ];
          snprintf(path, sizeof path, "/dev/%s", disk_name);
          int input = open(path, O_RDONLY);
          if (input != -1) {
            off_t size = 0;

            if (ioctl(input, DIOCGMEDIASIZE, &size) != -1) {
              can.md_space_mb = (uint_t) (size / (1024ULL * 1024ULL));
              space_map[disk_name] = can.md_space_mb;
            }

            close(input);
          }
        } else
          can.md_space_mb = smn->second;

        pthread_mutex_unlock(&space_map_lock);

        (*this)[disk_name] = can;
        node = find(disk_name);
      }

      md_can_t *cp = &node->second;

      cp->md_reads            = dp->operations[DEVSTAT_READ];
      cp->md_read_K           = (uint_t) (dp->bytes[DEVSTAT_READ] / 1024);
      cp->md_writes           = dp->operations[DEVSTAT_WRITE];
      cp->md_written_K        = (uint_t) (dp->bytes[DEVSTAT_WRITE] / 1024);
      cp->md_run_msecs        = MSECS(dp->busy_time);
      cp->md_runq_msecs       = cp->md_run_msecs;
      cp->md_io_operations    = cp->md_reads + cp->md_writes;

      qlen = dp->start_count - dp->end_count;
      if (qlen < 0)
        qlen = 0;

      qnode = qmap.find(disk_name);
      qmap[disk_name] += qlen;

      cp->md_avg_queue_length = qmap[disk_name];

      current_set.insert(disk_name);
    }
    pthread_mutex_unlock(&qmux);
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

static void build_ctlr_disk_map(node_map_t &cmap)
{
  int i;
  int ndev;
  int ctlr_num;
  const char *prefix;
  devinfo_t di;
  statinfo_t ci;
  devstat_t *dp;
  char disk_name[128];
  char ctlr_name[128];
  int iftype;

  memset(&ci, '\0', sizeof ci);
  memset(&di, '\0', sizeof di);

  ci.dinfo = &di;
  if (devstat_getdevs(0, &ci) == -1)
    throw mdr_exception_t("mdr_ctlr_t::mdr_refresh: cannot getdevs");

  ndev = di.numdevs;

  for(i=0; i<ndev; i++) {
    dp = &di.devices[i];
// at least...
#if OS_VERSION >= 540
    // XXX: this is just wrong...
    if (dp->device_type == 0)
      dp->device_type = (devstat_type_flags)
        ((int) DEVSTAT_TYPE_DIRECT | (int) DEVSTAT_TYPE_IF_IDE);
#endif
    iftype = (dp->device_type & DEVSTAT_TYPE_IF_MASK);
    switch(iftype) {
    case DEVSTAT_TYPE_IF_IDE:
      prefix = "ide";
      break;
    case DEVSTAT_TYPE_IF_SCSI:
      prefix = "scsi";
      break;
    default:
      continue;
    }

    // XXX: this may not be the case for SCSI, haven't seen one yet.
    ctlr_num = dp->device_name[0] - 'a';
    snprintf(ctlr_name, sizeof ctlr_name, "%s%d", prefix, ctlr_num);
    snprintf(disk_name, sizeof disk_name, "%s%d",
        dp->device_name, dp->unit_number);
    cmap[disk_name] = ctlr_name;
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
    node_map_t cmap;
    node_node_t cnode;
    mctl_can_t can;
    md_node_t dnode;
    md_can_t *dp;

    // build the map
    build_ctlr_disk_map(cmap);

    // zero out all the ctlr cans.
    clear();

    disk->mdr_refresh();

    for(dnode=disk->begin(); dnode != disk->end(); dnode++) {
      dp = &dnode->second;
      if (dp->md_type == "physical") {
        cnode = cmap.find(dnode->first);
        if (cnode == cmap.end())
          // Hard luck. Real unlikely, but tough crap anyway.
          continue;

        // accumulate all values from all disks for this controller
        can.clear();
        can.mctl_reads            += dp->md_reads;
        can.mctl_read_K           += dp->md_read_K;
        can.mctl_read_msecs       += dp->md_read_msecs;
        can.mctl_writes           += dp->md_writes;
        can.mctl_written_K        += dp->md_written_K;
        can.mctl_write_msecs      += dp->md_write_msecs;
        can.mctl_run_msecs        += dp->md_run_msecs;
        can.mctl_runq_msecs       += dp->md_runq_msecs;
        can.mctl_avg_queue_length += dp->md_avg_queue_length;
        can.mctl_disk_count++;

        (*this)[cnode->second] = can;
      }
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

void mdr_mp_t::mdr_refresh(void)
{
  mmp_node_t n;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    mp_remote_refresh();
  } else {
    // FreeBSD has one CPU. Period.
    mdr_kvm_t<cpu_time_t> cp_time("cp_time");
    mmp_can_t can;
    const char *cpu0 = "cpu0";

    can.clear();
    can.mmp_cpu_instance  = 0;
    can.mmp_user_ticks   = (*cp_time)[CP_USER] + (*cp_time)[CP_NICE];
    can.mmp_kernel_ticks = (*cp_time)[CP_SYS] + (*cp_time)[CP_INTR];
    can.mmp_nice_ticks   = (*cp_time)[CP_NICE];
    can.mmp_idle_ticks   = (*cp_time)[CP_IDLE];
    can.mmp_irq_ticks    = (*cp_time)[CP_INTR];
    (*this)[cpu0] = can;
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

#define B2K(s,b) ((ulong_t) ((((double) (s) * (double) (b)) / 1024.0)))

typedef struct statfs statfs_t;

void mdr_mnt_t::mdr_refresh(void)
{
  mmnt_node_t node;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    mnt_remote_refresh();
  } else {
    struct mm {
      int code;
      const char *speak;
    } mmmm[] = {
      { MNT_RDONLY,               "ro"          },
      { MNT_SYNCHRONOUS,          "sync"        },
      { MNT_NOEXEC,               "noexec"      },
      { MNT_NOSUID,               "nosuid"      },
      { MNT_NODEV,                "nodev"       },
      { MNT_UNION,                "union"       },
      { MNT_ASYNC,                "async"       },
      { MNT_EXRDONLY,             "expro"       },
      { MNT_EXPORTED,             "exp"         },
      { MNT_DEFEXPORTED,          "expworld"    },
      { MNT_EXPORTANON,           "expanon"     },
      { MNT_EXKERB,               "expkerb"     },
      { MNT_LOCAL,                "local"       },
      { MNT_QUOTA,                "quota"       },
      { MNT_ROOTFS,               "root"        },
      { 0,                        0             }
    };
    struct mm *mp;
    statfs_t *sp;
    statfs_t fsbuf;
    struct stat stbuf;
    int i;
    int n;
    ulong_t reserved;
    mmnt_can_t can;
    bool ok;
    std::map<std::string, bool> host_stat;
    std::map<std::string, bool>::iterator hn;

    clear();

    n = getfsstat(0, 0, MNT_NOWAIT);
    if (n == -1)
      throw mdr_exception_t("mdr_mnt_t::mdr_refresh: getfsstat failed");
    sp = new statfs_t[n];
    n = getfsstat(sp, n * sizeof(statfs_t), MNT_NOWAIT);
    for(i=0; i<n; i++) {
      can.clear();
      can.mmnt_special     = sp[i].f_mntfromname;
      can.mmnt_mount_point = sp[i].f_mntonname;
      can.mmnt_fs_type     = sp[i].f_fstypename;

      // collect the options
      for(mp=mmmm; mp->code; mp++) {
        if (sp[i].f_flags & mp->code) {
          if (can.mmnt_mount_options.size() > 0)
            can.mmnt_mount_options += ",";
          can.mmnt_mount_options += mp->speak;
        }
      }

#if CHECK_NFS_BEFORE_STAT
      if (can.mmnt_fs_type == "nfs") try {
        static unsigned short nfs_port;
        struct sockaddr_in sa;
        struct hostent *hp;
        int sd;

        if (nfs_port == 0) {
          struct servent *sp = getservbyname("nfsd", "tcp");

          if (sp == 0)
            throw int(-1);

          nfs_port = sp->s_port;
        }

        // get the host name from the special file and do a lookup
        std::string host(can.mmnt_special);
        host = host.substr(0, host.find(':'));
        hn = host_stat.find(host);
        if (hn != host_stat.end()) {
          int how = (int) (hn->second == false);
          throw int(how);
        }

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
        if (statfs(can.mmnt_mount_point.c_str(), &fsbuf) != -1) {
          ((int32_t *) &can.mmnt_file_system_id)[0] = fsbuf.f_fsid.val[0];
          ((int32_t *) &can.mmnt_file_system_id)[1] = fsbuf.f_fsid.val[1];
          can.mmnt_size_K = B2K(fsbuf.f_blocks, fsbuf.f_bsize);
          can.mmnt_free_K = B2K(fsbuf.f_bfree, fsbuf.f_bsize);
          can.mmnt_used_K = can.mmnt_size_K - can.mmnt_free_K;
          reserved      = can.mmnt_free_K - B2K(fsbuf.f_bavail, fsbuf.f_bsize);
          if (can.mmnt_size_K == 0)
            can.mmnt_percent_used = 0.0;
          else
            can.mmnt_percent_used =
              ceil((can.mmnt_used_K * 100.0) / (can.mmnt_size_K - reserved));
          can.mmnt_files      = fsbuf.f_files;
          can.mmnt_free_files = fsbuf.f_ffree;
        }
        if (stat(can.mmnt_mount_point.c_str(), &stbuf) != -1) {
          can.mmnt_major = major(stbuf.st_rdev);
          can.mmnt_minor = minor(stbuf.st_rdev);
        }
      }

      (*this)[sp[i].f_mntonname] = can;
    }
    delete[] sp;
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

static inline int get_clock_speed(int cpu_class, uint_t cpu_freq)
{
  switch(cpu_class) {
  case CPUCLASS_286:
    return 0;
    break;
  case CPUCLASS_386:
    return 0;
    break;
  case CPUCLASS_486:
    return 0;
    break;
  case CPUCLASS_586:
  case CPUCLASS_686:   
    return (cpu_freq + 5000) / 1000000;
    break;
  default:
    return 0;
  }
}

void mdr_cpuinfo_t::mdr_refresh(void)
{
  if (net_client) {
    cpuinfo_remote_refresh();
    return;
  }

  static mdr_sysctl_t<struct timeval> btime(CTL_KERN, KERN_BOOTTIME);
  static mdr_sysctl_t<string128> cpu_model(CTL_HW, HW_MODEL);
  static mdr_sysctl_t<string20> cpu_vendor(CTL_HW, HW_MACHINE);
  static mdr_kvm_t<int> cpu_class("cpu_class");
  static mdr_kvm_t<uint_t> tsc_freq("tsc_freq");
  const char *cpu0 = "cpu0";
  mci_node_t node;
  mci_can_t can;

  can.mci_instance = 0;
  can.mci_model = (char *) cpu_model();
  can.mci_architecture = (char *) cpu_vendor();
  can.mci_clock_MHz = get_clock_speed(*cpu_class, *tsc_freq);
  (*this)[cpu0] = can;
}

void mdr_ip_t::mdr_refresh(void)
{
  if (net_client) {
    ip_remote_refresh();
    return;
  }

  ipstat_t ipstat;
  size_t len = sizeof ipstat;
  int name[4] = { CTL_NET, PF_INET, IPPROTO_IP, 0 };

  if (sysctlbyname("net.inet.ip.stats", &ipstat, &len, 0, 0) == -1)
    throw mdr_exception_t("mdr_ip_t::mdr_refresh: cannot sysctlbyname");

  name[3] = IPCTL_FORWARDING;
  if (sysctl(name, 4, &mip_ipForwarding, &len, 0, 0) == -1)
    throw mdr_exception_t("mdr_ip_t::mdr_refresh: cannot sysctl(FORWARDING)");

  mip_ipForwarding = (mip_ipForwarding ? 1 /* gateway */ : 2 /* host */);

  name[3] = IPCTL_DEFTTL;
  if (sysctl(name, 4, &mip_ipDefaultTTL, &len, 0, 0) == -1)
    throw mdr_exception_t("mdr_ip_t::mdr_refresh: cannot sysctl(TTL)");

  mip_ipInReceives      = ipstat.ips_total;
  mip_ipInHdrErrors     = ipstat.ips_badsum + ipstat.ips_tooshort +
                          ipstat.ips_toosmall + ipstat.ips_badhlen +
                          ipstat.ips_badlen;
  mip_ipInAddrErrors    = ipstat.ips_cantforward;
  mip_ipForwDatagrams   = ipstat.ips_forward;
  mip_ipInUnknownProtos = ipstat.ips_noproto;
  mip_ipInDiscards      = ipstat.ips_fragdropped;
  mip_ipInDelivers      = ipstat.ips_delivered;
  mip_ipOutRequests     = ipstat.ips_localout;
  mip_ipOutDiscards     = ipstat.ips_odropped;
  mip_ipOutNoRoutes     = 0;
  mip_ipReasmTimeout    = IPFRAGTTL;
  mip_ipReasmReqds      = ipstat.ips_fragments;
  mip_ipReasmOKs        = ipstat.ips_reassembled;
  mip_ipReasmFails      = ipstat.ips_fragdropped + ipstat.ips_fragtimeout;
  mip_ipFragOKs         = ipstat.ips_fragments - mip_ipReasmFails;
  mip_ipFragFails       = ipstat.ips_cantfrag;
  mip_ipFragCreates     = ipstat.ips_ofragments;
  mip_ipRoutingDiscards = ipstat.ips_noroute;
}

void mdr_tcp_t::mdr_refresh(void)
{
  if (net_client) {
    tcp_remote_refresh();
    return;
  }

  tcpstat_t tcpstat;
  size_t len = sizeof tcpstat;
  static long hz = sysconf(_SC_CLK_TCK);

  if (sysctlbyname("net.inet.tcp.stats", &tcpstat, &len, 0, 0) == -1)
    throw mdr_exception_t("mdr_tcp_t::mdr_refresh: cannot sysctlbyname");

  mtcp_tcpRtoAlgorithm = 4;                // Van Jacobsen's algorithm
  mtcp_tcpRtoMin       = TCPTV_MIN;
  mtcp_tcpRtoMax       = TCPTV_REXMTMAX;
  mtcp_tcpMaxConn      = -1;
  mtcp_tcpActiveOpens  = tcpstat.tcps_connattempt;
  mtcp_tcpPassiveOpens = tcpstat.tcps_accepts;
  mtcp_tcpAttemptFails = tcpstat.tcps_conndrops;
  mtcp_tcpEstabResets  = tcpstat.tcps_drops;
  mtcp_tcpInSegs       = tcpstat.tcps_rcvtotal;
  mtcp_tcpOutSegs      = tcpstat.tcps_sndtotal - tcpstat.tcps_sndrexmitpack;
  mtcp_tcpRetransSegs  = tcpstat.tcps_sndrexmitpack;
  mtcp_tcpInErrs       = tcpstat.tcps_rcvbadsum + tcpstat.tcps_rcvbadoff;
  mtcp_tcpOutRsts      = tcpstat.tcps_sndctrl - tcpstat.tcps_closed;
}

void mdr_udp_t::mdr_refresh(void)
{
  if (net_client) {
    udp_remote_refresh();
    return;
  }

  udpstat_t udpstat;
  size_t len = sizeof udpstat;

  if (sysctlbyname("net.inet.udp.stats", &udpstat, &len, 0, 0) == -1)
    throw mdr_exception_t("mdr_udp_t::mdr_refresh: cannot sysctlbyname");

  mudp_udpInDatagrams  = udpstat.udps_ipackets;
  mudp_udpNoPorts      = udpstat.udps_noport;
  mudp_udpOutDatagrams = udpstat.udps_opackets;
  mudp_udpInErrors     = udpstat.udps_hdrops + udpstat.udps_badsum +
                         udpstat.udps_fullsock + udpstat.udps_badlen;
}

void mdr_icmp_t::mdr_refresh(void)
{
  if (net_client) {
    icmp_remote_refresh();
    return;
  }

  icmpstat_t icmpstat;
  size_t len = sizeof icmpstat;
  int i;

  if (sysctlbyname("net.inet.icmp.stats", &icmpstat, &len, 0, 0) == -1)
    throw mdr_exception_t("mdr_icmp_t::mdr_refresh: cannot sysctlbyname");

  micmp_icmpInMsgs           = icmpstat.icps_badcode + icmpstat.icps_tooshort +
                               icmpstat.icps_checksum + icmpstat.icps_badlen;
  for (i=0; i<=ICMP_MAXTYPE; i++)
    micmp_icmpInMsgs += icmpstat.icps_inhist[i];
  micmp_icmpInErrors         = icmpstat.icps_badcode + icmpstat.icps_tooshort +
                               icmpstat.icps_checksum + icmpstat.icps_badlen;
  micmp_icmpInDestUnreachs   = icmpstat.icps_inhist[ICMP_UNREACH];
  micmp_icmpInTimeExcds      = icmpstat.icps_inhist[ICMP_TIMXCEED];
  micmp_icmpInParmProbs      = icmpstat.icps_inhist[ICMP_PARAMPROB];
  micmp_icmpInSrcQuenchs     = icmpstat.icps_inhist[ICMP_SOURCEQUENCH];
  micmp_icmpInRedirects      = icmpstat.icps_inhist[ICMP_REDIRECT];
  micmp_icmpInEchos          = icmpstat.icps_inhist[ICMP_ECHO];
  micmp_icmpInEchoReps       = icmpstat.icps_inhist[ICMP_ECHOREPLY];
  micmp_icmpInTimestamps     = icmpstat.icps_inhist[ICMP_TSTAMP];
  micmp_icmpInTimestampReps  = icmpstat.icps_inhist[ICMP_TSTAMPREPLY];
  micmp_icmpInAddrMasks      = icmpstat.icps_inhist[ICMP_MASKREQ];
  micmp_icmpInAddrMaskReps   = icmpstat.icps_inhist[ICMP_MASKREPLY];
  micmp_icmpOutMsgs          = icmpstat.icps_oldshort + icmpstat.icps_oldicmp;
  for (i=0; i<=ICMP_MAXTYPE; i++)
    micmp_icmpOutMsgs += icmpstat.icps_outhist[i];
  micmp_icmpOutErrors        = icmpstat.icps_oldshort + icmpstat.icps_oldicmp;
  micmp_icmpOutDestUnreachs  = icmpstat.icps_outhist[ICMP_UNREACH];
  micmp_icmpOutTimeExcds     = icmpstat.icps_outhist[ICMP_TIMXCEED];
  micmp_icmpOutParmProbs     = icmpstat.icps_outhist[ICMP_PARAMPROB];
  micmp_icmpOutSrcQuenchs    = icmpstat.icps_outhist[ICMP_SOURCEQUENCH];
  micmp_icmpOutRedirects     = icmpstat.icps_outhist[ICMP_REDIRECT];
  micmp_icmpOutEchos         = icmpstat.icps_outhist[ICMP_ECHO];
  micmp_icmpOutEchoReps      = icmpstat.icps_outhist[ICMP_ECHOREPLY];
  micmp_icmpOutTimestamps    = icmpstat.icps_outhist[ICMP_TSTAMP];
  micmp_icmpOutTimestampReps = icmpstat.icps_outhist[ICMP_TSTAMPREPLY];
  micmp_icmpOutAddrMasks     = icmpstat.icps_outhist[ICMP_MASKREQ];
  micmp_icmpOutAddrMaskReps  = icmpstat.icps_outhist[ICMP_MASKREPLY];
}

static const char *get_command(kinfo_proc_t *procp)
{
  static std::string command;
  char **cmd;
  int i;

  cmd = kvm_getargv(kd, procp, 0);
  if (cmd == 0)
    return 0;
  for(i=0; cmd[i]; i++) {
    if (i == 0)
      command = cmd[i];
    else {
      command += " ";
      command += cmd[i];
    }
  }
  return command.c_str();
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
    char path[BUFSIZ];
    char name[128];
    const char *cmd;
    char *fq_cmd;
    mps_can_t can;
    int nprocs;
    int i;
    kinfo_proc_t *pp;
    static long pgsz = sysconf(_SC_PAGESIZE);
    static long pgk = pgsz / 1024;

    mps_procs_runnable = 0;
    mps_procs_sleeping = 0;
    mps_procs_stopped = 0;
    mps_procs_blocked = 0;
    mps_procs_zombie = 0;
    mps_procs_specific = 0;

    if (kd == 0) {
      kd = kvm_open(0, 0, 0, O_RDONLY, 0);
      if (kd == 0)
        throw mdr_exception_t("mdr_ps_t::mdr_refresh: cannot kvm_open");
    }

    for(;;) {
      // get nproc
      kvm_getprocs(kd, KERN_PROC_PROC, 0, &nprocs);

      if (nprocs > mps_array_max) {
        if (mps_array)
          delete[] mps_array;
        mps_array = new kinfo_proc_t[nprocs];
        mps_array_max = nprocs;
      }

      // get data
      pp = kvm_getprocs(kd, KERN_PROC_PROC, 0, &nprocs);
      if (pp == 0)
        throw mdr_exception_t("mdr_ps_t::mdr_refresh: cannot kvm_getprocs");
      if (nprocs > mps_array_max)
        continue;
      memcpy(mps_array, pp, sizeof(kinfo_proc_t) * nprocs);
      break;
    }

    // start over
    // clear();

    for(i=0; i<nprocs; i++) {

      pp = &mps_array[i];

      // this is the key: process_name[process_id]
      snprintf(name, sizeof name, "%s[%d]", pp->ki_comm, pp->ki_pid);

      node = find(name);
      if (node == end()) {

        can.clear();

        can.mps_start = pp->ki_start.tv_sec;
        can.mps_pid = pp->ki_pid;
        can.mps_ppid = pp->ki_ppid;
        can.mps_tty_device = pp->ki_tdev;
        can.mps_file_name = pp->ki_comm;
        cmd = get_command(pp);
        if (cmd == 0)
          can.mps_command = pp->ki_comm;
        else
          can.mps_command = cmd;

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

      cp->mps_uid = pp->ki_uid;
      cp->mps_euid = pp->ki_svuid;
      cp->mps_user_time = TIMEVAL(pp->ki_rusage.ru_utime);
      cp->mps_kernel_time = TIMEVAL(pp->ki_rusage.ru_stime);
      cp->mps_cpu_time = (double) pp->ki_runtime / 1000000.0;
      cp->mps_size_K = pp->ki_size / 1024;
      cp->mps_rssize_K = pp->ki_rssize * pgk;
      cp->mps_memory_percent =
        (cp->mps_rssize_K * 100.0) / (double) mps_total_mem_K;
      cp->mps_voluntary_csw = pp->ki_rusage.ru_nvcsw;
      cp->mps_involuntary_csw = pp->ki_rusage.ru_nivcsw;
      cp->mps_disk_faults = pp->ki_rusage.ru_majflt;
      cp->mps_page_reclaims = pp->ki_rusage.ru_minflt;
      cp->mps_is_system_process = ((pp->ki_flag & P_SYSTEM) != 0);

      // the FreeBSD process states:
      //   D uninterruptable wait
      //   I long term sleep
      //   J jail
      //   L waiting on lock
      //   R runnable
      //   S short term sleep
      //   T stopped
      //   Z zombie

      switch(pp->ki_stat) {
      case SIDL:  // process is being created by fork
      case SRUN:
        cp->mps_state = MPS_RUNNABLE;
        mps_procs_runnable++;
        break;
      case SSLEEP:
        cp->mps_state = MPS_SLEEPING;
        mps_procs_sleeping++;
        break;
      case SSTOP:
        cp->mps_state = MPS_STOPPED;
        mps_procs_stopped++;
        break;
      case SZOMB:
        cp->mps_state = MPS_ZOMBIE;
        mps_procs_zombie++;
        break;
      case SWAIT:
      case SLOCK:
        cp->mps_state = MPS_BLOCKED;
        mps_procs_blocked++;
        break;
      default:    // J and possibly W (swapped, not in man page)
        cp->mps_state = MPS_SPECIFIC;
        mps_procs_specific++;
        break;
      }

      cp->mps_priority = pp->ki_pri.pri_level;
      cp->mps_nice = pp->ki_nice;

      cp->mps_user_name = uid_lookup(cp->mps_uid);
      cp->mps_effective_user_name = uid_lookup(cp->mps_euid);
      cp->mps_thread_count = pp->ki_numthreads;

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
  snprintf(name, MAX_TT_LEN, tp->td_name, minor(dev));
}

static void
tty_compose(char *name, dev_t dev, struct tty_devs *tp)
{
  char p = 'a';

  snprintf(name, MAX_TT_LEN, tp->td_name, p + minor(dev));
}

static void
pty_compose(char *name, dev_t dev, struct tty_devs *tp)
{
  char p = 'p';

  snprintf(name, MAX_TT_LEN,
    tp->td_name, p + minor(dev) / 16, minor(dev) % 16);
}

static void
vvy_compose(char *name, dev_t dev, struct tty_devs *tp)
{
  snprintf(name, MAX_TT_LEN, tp->td_name, minor(dev));
}

std::string mdr_ps_t::mps_dev_to_tty(dev_t dev, std::string &result)
{
  static int initialized = 0;
  static struct tty_devs tty_map[] = {
    { "/dev/console",  "console",  0,  0           },
    { "/dev/pts/0",    "pts/%d",   0,  pts_compose },
    { "/dev/ttya",     "tty%c",    0,  tty_compose },
    { "/dev/ttyp0",    "tty%c%d",  0,  pty_compose },
    { "/dev/ttyv0",    "ttyv%x",   0,  vvy_compose },
    { 0,               0,          0,  0           }
  };
  char name[MAX_TT_LEN];
  struct tty_devs *tp;
  struct stat stbuf;

  if (initialized == 0) {
    for(tp=tty_map; tp->td_device; tp++) {
      if (stat(tp->td_device, &stbuf) == -1)
        continue;
      tp->td_major = major(stbuf.st_rdev);
    }
    initialized = 1;
  }
  for(tp=tty_map; tp->td_device; tp++) {
    if (major(dev) == tp->td_major) {
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
    char path[BUFSIZ];
    char name[128];
    const char *cmd;
    char *fq_cmd;
    mt_can_t can;
    int nprocs;
    int i;
    kinfo_proc_t *pp;
    static long pgsz = sysconf(_SC_PAGESIZE);
    static long pgk = pgsz / 1024;

    if (kd == 0) {
      kd = kvm_open(0, 0, 0, O_RDONLY, 0);
      if (kd == 0)
        throw mdr_exception_t("mdr_ps_t::mdr_refresh: cannot kvm_open");
    }

    for(;;) {
      // get nproc
      kvm_getprocs(kd, KERN_PROC_ALL, 0, &nprocs);

      if (nprocs > mt_array_max) {
        if (mt_array)
          delete[] mt_array;
        mt_array = new kinfo_proc_t[nprocs];
        mt_array_max = nprocs;
      }

      // get data
      pp = kvm_getprocs(kd, KERN_PROC_ALL, 0, &nprocs);
      if (pp == 0)
        throw mdr_exception_t("mdr_ps_t::mdr_refresh: cannot kvm_getprocs");
      if (nprocs > mt_array_max)
        continue;
      memcpy(mt_array, pp, sizeof(kinfo_proc_t) * nprocs);
      break;
    }

    for(i=0; i<nprocs; i++) {

      pp = &mt_array[i];

      // this is the key: process_name[process_id][thread_id]
      snprintf(name, sizeof name, "%s[%d][%d]",
        pp->ki_comm, pp->ki_pid, pp->ki_tid);

      node = find(name);
      if (node == end()) {

        can.clear();

        can.mt_thread_id = pp->ki_tid;
        can.mt_process_id = pp->ki_pid;

        (*this)[name] = can;
        node = find(name);
      }

      mt_can_t *cp = &node->second;

      cp->mt_user_time = TIMEVAL(pp->ki_rusage.ru_utime);
      cp->mt_kernel_time = TIMEVAL(pp->ki_rusage.ru_stime);
      cp->mt_cpu_time = (double) pp->ki_runtime / 1000000.0;
      cp->mt_size_K = pp->ki_size / 1024;
      cp->mt_rssize_K = pp->ki_rssize * pgk;
      cp->mt_memory_percent =
        (cp->mt_rssize_K * 100.0) / (double) mt_total_mem_K;
      cp->mt_voluntary_csw = pp->ki_rusage.ru_nvcsw;
      cp->mt_involuntary_csw = pp->ki_rusage.ru_nivcsw;
      cp->mt_disk_faults = pp->ki_rusage.ru_majflt;
      cp->mt_page_reclaims = pp->ki_rusage.ru_minflt;

      // the FreeBSD process states:
      //   D uninterruptable wait
      //   I long term sleep
      //   J jail
      //   L waiting on lock
      //   R runnable
      //   S short term sleep
      //   T stopped
      //   Z zombie

      switch(pp->ki_stat) {
      case SRUN:
        cp->mt_state = MPS_RUNNABLE;
        break;
      case SIDL:
      case SSLEEP:
        cp->mt_state = MPS_SLEEPING;
        break;
      case SSTOP:
        cp->mt_state = MPS_STOPPED;
        break;
      case SZOMB:
        cp->mt_state = MPS_ZOMBIE;
        break;
      case SWAIT:
      case SLOCK:
        cp->mt_state = MPS_BLOCKED;
        break;
      default: // J and possibly W (swapped, not in man page)
        cp->mt_state = MPS_SPECIFIC;
        break;
      }

      cp->mt_priority = pp->ki_pri.pri_level;
      cp->mt_nice = pp->ki_nice;

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
