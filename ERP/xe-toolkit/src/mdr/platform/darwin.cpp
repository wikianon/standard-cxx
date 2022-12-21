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

//
// If you find a memory leak in this code, I'll send you a mouse pad.
//

#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/sysctl.h>
#include <sys/resource.h>
#include <sys/mount.h>
#include <sys/socket.h>
#include <sys/sockio.h>
#include <sys/ioctl.h>
#if OS_VERSION < 830
# include <sys/namei.h>
#endif
#include <sys/param.h>
#include <sys/ucred.h>
#include <sys/vmmeter.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/disk.h>
#include <net/if.h>
#include <net/if_var.h>
#if OS_VERSION >= 830
# include <net/route.h>
# include <net/if_dl.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>
#include <nfs/nfsproto.h>
#include <nfs/rpcv2.h>
#include <nfs/nfs.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/mach_error.h>
#include <mach/vm_map.h>
#include <mach/task.h>
#include <mach/task_info.h>
#include <mach/mach_port.h>
#include <mach/thread_act.h>
#include <mach/thread_info.h>
#include <mach/shared_memory_server.h>
#include <sys/queue.h>
#include <sys/protosw.h>
#include <sys/socketvar.h>
#include <netinet/tcp.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip_var.h>
#include <netinet/tcp_var.h>
#include <netinet/icmp_var.h>
#include <netinet/udp_var.h>
#include <mdr_misc.h>
#include <mdr_memory.h>
#include <mdr_cpu.h>
#include <mdr_netif.h>
#include <mdr_cache.h>
#include <mdr_pf.h>
#include <mdr_disk.h>
#include <mdr_ctlr.h>
#include <mdr_mnt.h>
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
#include <mdr_sysctl.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOBSD.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/storage/IOBlockStorageDriver.h>
#include <IOKit/storage/IOMedia.h>
#include <IOKit/network/IONetworkController.h>
#include <IOKit/network/IONetworkMedium.h>
#include <IOKit/network/IONetworkInterface.h>
#include <IOKit/network/IONetworkStats.h>
#include <IOKit/network/IONetworkLib.h>

using namespace mdr;

// 32 ought to do it.
typedef char ifname_t[32];

typedef std::set<std::string> del_set_t;
typedef del_set_t::iterator   del_node_t;

typedef std::map<std::string, io_registry_entry_t> drive_map_t;
typedef drive_map_t::iterator                      drive_node_t;

// All of buf.h is inside of #ifdef KERNEL. Smart.
struct bufstats {
    long    bufs_incore;        /* found incore */
    long    bufs_busyincore;    /* found incore. was busy */
    long    bufs_vmhits;        /* not incore. found in VM */
    long    bufs_miss;          /* not incore. not in VM */
    long    bufs_sleeps;        /* buffer starvation */
    long    bufs_eblk;          /* Calls to geteblk */
    long    bufs_iobufmax;      /* Max. number of IO buffers used */
    long    bufs_iobufinuse;    /* number of IO buffers in use */
    long    bufs_iobufsleeps;   /* IO buffer starvation */
};

#if OS_VERSION >= 830
/*
 * Stats on usefulness of namei caches.
 */
struct	nchstats {
	long	ncs_goodhits;		/* hits that we can really use */
	long	ncs_neghits;		/* negative hits that we can use */
	long	ncs_badhits;		/* hits we must drop */
	long	ncs_falsehits;		/* hits with id mismatch */
	long	ncs_miss;		/* misses */
	long	ncs_long;		/* long names that ignore cache */
	long	ncs_pass2;		/* names found with passes == 2 */
	long	ncs_2passes;		/* number of times we attempt it */
};
#endif

typedef struct ipstat ipstat_t;
typedef struct tcpstat tcpstat_t;
typedef struct icmpstat icmpstat_t;
typedef struct udpstat udpstat_t;
typedef struct kinfo_proc kinfo_proc_t;

// pexpert.h has any number of compile issues.
struct clock_frequency_info_t {
  unsigned long bus_clock_rate_hz;
  unsigned long cpu_clock_rate_hz;
  unsigned long dec_clock_rate_hz;
  unsigned long bus_clock_rate_num;
  unsigned long bus_clock_rate_den;
  unsigned long bus_to_cpu_rate_num;
  unsigned long bus_to_cpu_rate_den;
  unsigned long bus_to_dec_rate_num;
  unsigned long bus_to_dec_rate_den;
};

static const int MAXDNAME  = 32;
static const int PSCMDLEN  = 1024;

// mdr_misc_t needs mdr_ps_t. mdr_ps_t contains an mdr_misc_t.
// To prevent the obvious constructor recursion, this class was crafted.

class scacsw_t {
public:
  uint_t syscalls;
  uint_t cswitch;

  scacsw_t() {
    syscalls = 0;
    cswitch = 0;
    parray = 0;
    parray_max = 0;
    do_compute();
  }

 ~scacsw_t() {
    if (parray)
      delete[] parray;
  }

  void refresh(void) {
    do_compute();
  }

private:
  class two_t {
  public:
    uint_t sysc;
    uint_t csw;

    two_t() {
      clear();
    }

    void clear(void) {
      sysc = 0;
      csw = 0;
    }
  };
  typedef std::map<pid_t, two_t> two_map_t;
  typedef two_map_t::iterator two_node_t;

  typedef std::set<pid_t>  dset_t;
  typedef dset_t::iterator dnode_t;

  two_map_t     pmap;
  two_map_t     old;
  kinfo_proc   *parray;
  int           parray_max;

  void do_compute(void) {
    two_node_t node;

    reread();
    rates();

    for(node=pmap.begin(); node != pmap.end(); node++) {
      two_t *tp = &node->second;

      syscalls += tp->sysc;
      cswitch += tp->csw;
    }
  }

  void reread(void) {
    int i;
    int n;
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
    int proc_count;
    size_t nbytes;
    two_node_t node;
    dset_t del_set;
    dnode_t sn;

    for(;;) {
      // get nproc
      if (sysctl(mib, 4, 0, &nbytes, 0, 0) == -1)
        throw mdr_exception_t("scacsw_t::refresh: sysctl(nproc) failed");

      // total bytes / sizeof proc structure
      n = nbytes / sizeof(kinfo_proc);
      if (n > parray_max) {
        if (parray)
          delete[] parray;
        parray = new kinfo_proc[n];
        parray_max = n;
      }

      // get data
      if (sysctl(mib, 4, parray, &nbytes, 0, 0) == -1) {
        if (errno == ENOMEM) {
          // proc table grew between two sysctl calls
          delete[] parray;
          parray = 0;
          continue;
        }
        throw mdr_exception_t("scacsw_t::refresh: sysctl(data) failed");
      }
      break;
    }

    // start over
    pmap.clear();

    // for each process, gather info and accumulate
    proc_count = nbytes / sizeof(kinfo_proc);
    for(i=0; i<proc_count; i++) {
      task_port_t task;
      kern_return_t err;
      uint_t count;
      task_events_info_data_t task_events;
      two_t t;

      // get the kinfo_proc_t from the array
      pid_t pid = parray[i].kp_proc.p_pid;

      // get task port
      err = task_for_pid(mach_task_self(), pid, &task);
      if (err != KERN_SUCCESS)
        continue;

      // get task events
      count = TASK_EVENTS_INFO_COUNT;
      err = task_info(task, TASK_EVENTS_INFO,
                     (task_info_t) &task_events, &count);
      if (err != KERN_SUCCESS) {
        mach_port_deallocate(mach_task_self(), task);
        continue;
      }

      t.sysc = task_events.syscalls_mach + task_events.syscalls_unix;
      t.csw = task_events.csw;

      mach_port_deallocate(mach_task_self(), task);

      pmap[pid] = t;
    }

    // now that the "now" is built, make sure there are old nodes for them
    for(node=pmap.begin(); node != pmap.end(); node++)
      if (old.find(node->first) == old.end())
        old[node->first].clear();

    // now find "old" nodes that no longer exist and delete them
    for(node=old.begin(); node != old.end(); node++)
      if (pmap.find(node->first) == pmap.end())
        del_set.insert(node->first);

    for(sn=del_set.begin(); sn != del_set.end(); sn++) {
      node = old.find(*sn);
      old.erase(node);
    }
  }

  void rates(void) {
    two_node_t node;

    for(node=pmap.begin(); node != pmap.end(); node++) {
      two_node_t onode = old.find(node->first);
      two_t *tp = &node->second;
      two_t *otp = &onode->second;
      uint_t tmp_sysc = tp->sysc;
      uint_t tmp_csw = tp->csw;

      tp->sysc -= otp->sysc;
      tp->csw -= otp->csw;

      otp->sysc = tmp_sysc;
      otp->csw = tmp_csw;
    }
  }
};

static inline double ld(struct loadavg ldavg, int i)
{
  return (double) ldavg.ldavg[i] / (double) ldavg.fscale;
}

void mdr_misc_t::mdr_refresh(void)
{
  if (net_client) {
    misc_remote_refresh();
    return;
  }

  static scacsw_t sac;
  static bool needs_refreshing = false;
  static mdr_sysctl_t<struct timeval> btime(CTL_KERN, KERN_BOOTTIME);
  static mdr_sysctl_t<unsigned long> ps(CTL_HW, HW_PAGESIZE);
  static mdr_sysctl_t<unsigned long> pp(CTL_HW, HW_PHYSMEM);
  static char hostname[_SYS_NAMELEN];
  static char platform[_SYS_NAMELEN];
  static char version[_SYS_NAMELEN];
  static char parch[_SYS_NAMELEN];

  mdr_sysctl_t<struct loadavg> ldav(CTL_VM, VM_LOADAVG);
  host_cpu_load_info_data_t cpu;
  host_basic_info_data_t hinfo;
  mach_msg_type_number_t len;
  host_t h = mach_host_self();
  kern_return_t kr;
  size_t slen;
  int mib[4];
  int i;

  if (needs_refreshing) {
    sac.refresh();
  } else {
    struct utsname u;

    gethostname(hostname, sizeof hostname);
    uname(&u);
    strcpy(platform, u.sysname);
    strcpy(version, u.release);
    strcpy(parch, u.machine);
    needs_refreshing = true;
  }

  mm_hostname = hostname;
  mm_os_platform = platform;
  mm_os_version = version;
  mm_physical_pages = *pp;
  mm_native_page_size = *ps;
  mm_physical_pages /= mm_native_page_size;

  // cpus conf and onln
  len = HOST_BASIC_INFO_COUNT;
  kr = host_info(h, HOST_BASIC_INFO, (host_info_t) &hinfo, &len);
  if (kr != KERN_SUCCESS)
    throw mdr_exception_t(
      "mdr_misc_t::mdr_refresh: host_info: %s", mach_error_string(kr));

  mm_processor_arch        = parch;
  mm_processors_configured = hinfo.max_cpus;
  mm_processors_online     = hinfo.avail_cpus;
  mm_ticks_per_second      = sysconf(_SC_CLK_TCK);
  mm_boot_time             = btime->tv_sec;

  // clock interrupts
  len = HOST_CPU_LOAD_INFO_COUNT;
  kr = host_statistics(h, HOST_CPU_LOAD_INFO, (host_info_t) &cpu, &len);
  if (kr != KERN_SUCCESS)
    throw mdr_exception_t(
      "mdr_misc_t::mdr_refresh: host_statistics: %s", mach_error_string(kr));
 
  for(mm_clock_interrupts=0, i=0; i<CPU_STATE_MAX; i++)
    mm_clock_interrupts += cpu.cpu_ticks[i];

  // total interrupts (not; kvm access deleted...)
  mm_interrupts = mm_clock_interrupts;
  mm_system_calls = sac.syscalls;
  mm_context_switches = sac.cswitch;

  // Fill out the first three components of the mib
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_ALL;
  if (sysctl(mib, 4, 0, &slen, 0, 0) == -1)
    throw mdr_exception_t(
      "mdr_misc_t::mdr_refresh: sysctl(KERN_PROC_ALL) failed");

  mm_process_count = slen / sizeof(kinfo_proc_t);

  // sysctl stuff
  mm_1min_run_queue  = ld(*ldav, 0);
  mm_5min_run_queue  = ld(*ldav, 1);
  mm_15min_run_queue = ld(*ldav, 2);
  mm_current_time    = time(0);
}

void mdr_memory_t::mdr_refresh(void)
{
  if (net_client) {
    memory_remote_refresh();
  } else {
    static mdr_sysctl_t<unsigned long> ps(CTL_HW, HW_PAGESIZE);
    static long pgk = *ps / 1024;

    vm_statistics_data_t vm;
    host_t h = mach_host_self();
    mach_msg_type_number_t len;
    kern_return_t kr;

    len = HOST_VM_INFO_COUNT;
    kr = host_statistics(h, HOST_VM_INFO, (host_info_t) &vm, &len);
    if (kr != KERN_SUCCESS)
      throw mdr_exception_t(
        "mdr_memory_t::mdr_refresh: host_statistics: %s",
            mach_error_string(kr));

    // if they were purged then they were scanned... I'm guessing...
    mmem_pages_scanned = vm.purges;
    mmem_free_memory_K = vm.free_count * pgk;
    mmem_paged_in_K    = vm.pageins * pgk;
    mmem_paged_out_K   = vm.pageouts * pgk;
    mmem_page_faults   = vm.faults;
    mmem_major_faults  = vm.faults - vm.reactivations;
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
    mach_msg_type_number_t len;
    mach_msg_type_number_t bi_len;
    processor_cpu_load_info_t cpu_array;
    processor_basic_info_t bi_array;
    host_t h = mach_host_self();
    char cpu_name[64];
    kern_return_t kr;
    natural_t ncpu;
    mcpu_can_t can;
    natural_t i;

    // start fresh
    clear();

    len = 0;
    bi_array = 0;
    kr = host_processor_info(h, PROCESSOR_BASIC_INFO, &ncpu,
      (processor_info_array_t *) &bi_array, &len);
    if (kr != KERN_SUCCESS)
      throw mdr_exception_t(
        "mdr_cpu_t::gen_cpu_line: PROCESSOR_BASIC_INFO: %s",
           mach_error_string(kr));
    bi_len = len;

    len = 0;
    cpu_array = 0;
    kr = host_processor_info(h, PROCESSOR_CPU_LOAD_INFO, &ncpu,
      (processor_info_array_t *) &cpu_array, &len);
    if (kr != KERN_SUCCESS)
      throw mdr_exception_t(
        "mdr_cpu_t::gen_cpu_line: PROCESSOR_CPU_LOAD_INFO: %s",
           mach_error_string(kr));

    for(i=0; i<ncpu; i++) {
      can.clear();
      can.mcpu_user_ticks   = cpu_array[i].cpu_ticks[CPU_STATE_USER];
      can.mcpu_kernel_ticks = cpu_array[i].cpu_ticks[CPU_STATE_SYSTEM];
      can.mcpu_nice_ticks   = cpu_array[i].cpu_ticks[CPU_STATE_NICE];
      can.mcpu_idle_ticks   = cpu_array[i].cpu_ticks[CPU_STATE_IDLE];

      snprintf(cpu_name, sizeof cpu_name, "cpu%d", bi_array[i].slot_num);
      (*this)[cpu_name] = can;
    }

    vm_deallocate(mach_task_self(), (vm_address_t) cpu_array, len);
    vm_deallocate(mach_task_self(), (vm_address_t) bi_array, bi_len);
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

// not a method since it's mac specific
// grovel the netif ifspeed, duplex and output failures from IOKit

static void get_iokit_info(const char *node_name, mnet_can_t *cp)
{
  CFMutableDictionaryRef match;
  io_iterator_t iflist;
  kern_return_t status;
  io_registry_entry_t intf;
  io_registry_entry_t ctlr;
  CFDictionaryRef if_props;
  CFDictionaryRef ctlr_props;
  CFDictionaryRef medium;
  CFStringRef name;
  char if_name[128];
  CFNumberRef number;

  // get the dict ref
  match = IOServiceMatching(kIONetworkInterfaceClass);

  // yield all of the interfaces
  status = IOServiceGetMatchingServices(kIOMasterPortDefault, match, &iflist);
  if (status != KERN_SUCCESS)
    return;

  // for each interface
  while((intf = IOIteratorNext(iflist)) != 0) {
    // get the properties of the interface for the name
    status = IORegistryEntryCreateCFProperties(intf,
               (CFMutableDictionaryRef *) &if_props,
                 kCFAllocatorDefault, kNilOptions);
    if (status != KERN_SUCCESS)
      continue;

    // get the name property
    name = (CFStringRef) CFDictionaryGetValue(if_props, CFSTR(kIOBSDNameKey));
    if (name) {
      CFStringGetCString(name, if_name, sizeof(if_name),
        CFStringGetSystemEncoding());

      // if it's not the interface in question then move on
      if (strcmp(if_name, node_name))
        continue;

      // get the network data dictionary
      CFDictionaryRef nd = (CFDictionaryRef)
        CFDictionaryGetValue(if_props, CFSTR(kIONetworkData));
      if (nd) {
        io_connect_t conn;

        // open the interface in question
        status = IONetworkOpen(intf, &conn);
        if (status == kIOReturnSuccess) {
          IOOutputQueueStats oq;
          IONDHandle dh;
          UInt32 len = sizeof(oq);

          // get the data handle
          status = IONetworkGetDataHandle(conn, kIOOutputQueueStatsKey, &dh);
          if (status == kIOReturnSuccess) {
            // read the output queue stats
            status = IONetworkReadData(conn, dh, (UInt8 *) &oq, &len);
            if (status == kIOReturnSuccess)
              cp->mnet_output_failures = oq.dropCount;
          }
          IONetworkClose(conn);
        }
      }

      // get the controller parent
      status = IORegistryEntryGetParentEntry(intf, kIOServicePlane, &ctlr);
      if (status == KERN_SUCCESS) {
        if (IOObjectConformsTo(ctlr, kIONetworkControllerClass) != 0) {
          // get the controller properties for the medium dictionary
          status = IORegistryEntryCreateCFProperties(ctlr,
                     (CFMutableDictionaryRef *) &ctlr_props,
                       kCFAllocatorDefault, kNilOptions);
          if (status == KERN_SUCCESS) {
            medium = (CFDictionaryRef)
              CFDictionaryGetValue(ctlr_props, CFSTR(kIOMediumDictionary));
            if (medium) {
              // get the active medium key for lookup in the medium dictionary
              CFStringRef am = (CFStringRef)
                CFDictionaryGetValue(ctlr_props, CFSTR(kIOActiveMedium));
              if (am) {
                // get the properties for this medium
                CFDictionaryRef mp = (CFDictionaryRef)
                  CFDictionaryGetValue(medium, am);
                if (mp) {
                  uint32_t speed;
                  uint32_t type;

                  // get the interface speed
                  number = (CFNumberRef)
                    CFDictionaryGetValue(mp, CFSTR(kIOMediumSpeed));
                  if (number)
                    CFNumberGetValue(number, kCFNumberSInt32Type, &speed);
                  cp->mnet_interface_speed = speed;

                  // patch up inconsistencies...
                  switch(cp->mnet_interface_speed) {
                  case 10:
                  case 100:
                  case 1000:
                    cp->mnet_interface_speed *= 1000000;
                    break;
                  case 10000000:
                  case 100000000:
                  case 1000000000:
                    // good to go
                    break;
                  default:
                    // when did they put 16Mb/s token ring on the mac?
                    break;
                  }

                  // get the interface type
                  number = (CFNumberRef)
                    CFDictionaryGetValue(mp, CFSTR(kIOMediumType));
                  if (number)
                    CFNumberGetValue(number, kCFNumberSInt32Type, &type);
                  cp->mnet_full_duplex = (type & kIOMediumOptionFullDuplex);
                }
              }
            }
            CFRelease(ctlr_props);
          }
        }
        IOObjectRelease(ctlr);
      }
    }
    CFRelease(if_props);
    IOObjectRelease(intf);
  }
  IOObjectRelease(iflist);
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
    int mib[6] = { CTL_NET, PF_ROUTE, 0, 0, NET_RT_IFLIST, 0 };
    size_t len = 0;
    struct if_msghdr *ifm;
    char *buf;
    char *next;
    char *lim;

    // start fresh
    clear();

    if (sysctl(mib, 6, 0, &len, 0, 0) < 0)
      throw mdr_exception_t("mdr_netif_t::mdr_refresh: cannot get netif size");

    buf = new char[len];

    if (sysctl(mib, 6, buf, &len, 0, 0) < 0)
      throw mdr_exception_t("mdr_netif_t::mdr_refresh: cannot get netif data");

    lim = buf + len;
    for(next=buf; next < lim; ) {
      ifm = (struct if_msghdr *) next;
      next += ifm->ifm_msglen;

      if (ifm->ifm_type == RTM_IFINFO) {
        struct sockaddr_dl *sdl = (struct sockaddr_dl *) (ifm + 1);
        char if_name[128];

        if ((ifm->ifm_flags & IFF_UP) == 0)
          continue;

        strncpy(if_name, sdl->sdl_data, sdl->sdl_nlen);
        if_name[sdl->sdl_nlen] = '\0';

        get_address_info(if_name, &can);
        get_iokit_info(if_name, &can);

        can.mnet_input_bytes     = ifm->ifm_data.ifi_ibytes;
        can.mnet_output_bytes    = ifm->ifm_data.ifi_obytes;
        can.mnet_input_packets   = ifm->ifm_data.ifi_ipackets;
        can.mnet_output_packets  = ifm->ifm_data.ifi_opackets;
        can.mnet_input_errors    = ifm->ifm_data.ifi_ierrors;
        can.mnet_output_errors   = ifm->ifm_data.ifi_oerrors;
        can.mnet_input_failures  = ifm->ifm_data.ifi_iqdrops;
        can.mnet_collisions      = ifm->ifm_data.ifi_collisions;
        // now from IOKit
        // can.mnet_output_failures = ifm->ifm_snd_drops;
        // can.mnet_interface_speed = ifm->ifm_data.ifi_baudrate;

        if (can.mnet_output_packets > 0)
          can.mnet_collision_percent =
            (100.0 * can.mnet_collisions) / (double) can.mnet_output_packets;
        else
          can.mnet_collision_percent = 0.0;

        (*this)[if_name] = can;
      }
    }

    delete[] buf;
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
    static const char *object = "object";
    mc_can_t *cp;
    mc_node_t node;
    double total;
    vm_statistics_data_t vm;
    host_t h = mach_host_self();
    mach_msg_type_number_t len;
    kern_return_t kr;

    len = HOST_VM_INFO_COUNT;
    kr = host_statistics(h, HOST_VM_INFO, (host_info_t) &vm, &len);
    if (kr != KERN_SUCCESS)
      throw mdr_exception_t(
        "mdr_cache_t::mdr_refresh: host_statistics: %s",
            mach_error_string(kr));

    node = find(object);
    if (node == end()) {
      (*this)[object].clear();
      node = find(object);
    }
    cp = &node->second;

    cp->mc_hits = vm.hits;
    cp->mc_misses = vm.lookups - vm.hits;
    total = cp->mc_hits + cp->mc_misses;
    if (total > 0.0)
      cp->mc_hit_percent = (100.0 * cp->mc_hits) / total;
    else
      cp->mc_hit_percent = 100.0;
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

  if (net_client)
    pf_remote_refresh();
  else {
#ifdef VM_SWAPUSAGE
    try {
      static const char *const name = "swap";
      mdr_sysctl_t<struct xsw_usage> swap(CTL_VM, VM_SWAPUSAGE);
      mpf_node_t sn;
      mpf_can_t *cp;

      sn = find(name);
      if (sn == end()) {
        (*this)[name].clear();
        sn = find(name);
      }
      cp = &sn->second;

      cp->mpf_size_K = (int) (swap->xsu_total / 1024);
      cp->mpf_free_K = (int) (swap->xsu_avail / 1024);
      cp->mpf_used_K = (int) (swap->xsu_used / 1024);
    } catch(std::exception &e) {
      // VM_SWAPUSAGE not understood on this machine...
      ;
    }
#endif
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

static bool
record_device(io_registry_entry_t drive, drive_map_t &dm)
{
  io_registry_entry_t parent;
  CFDictionaryRef properties;
  CFStringRef name;
  kern_return_t status;
  char disk_name[MAXDNAME];
  
  // get drive's parent
  status = IORegistryEntryGetParentEntry(drive, kIOServicePlane, &parent);
  if (status != KERN_SUCCESS)
    return false;

  if (IOObjectConformsTo(parent, "IOBlockStorageDriver") == 0) {
    IOObjectRelease(parent);
    return false;
  }

  // get drive properties
  status = IORegistryEntryCreateCFProperties(drive,
             (CFMutableDictionaryRef *) &properties,
               kCFAllocatorDefault, kNilOptions);

  if (status != KERN_SUCCESS) {
    IOObjectRelease(parent);
    return false;
  }

  // get name from properties
  name = (CFStringRef) CFDictionaryGetValue(properties, CFSTR(kIOBSDNameKey));
  CFStringGetCString(name, disk_name, MAXDNAME, CFStringGetSystemEncoding());

  // clean up, return success
  CFRelease(properties);

  dm[disk_name] = parent;
  return true;
}

static bool
record_all_devices(mach_port_t masterPort, drive_map_t &dm)
{
  io_iterator_t drivelist;
  io_registry_entry_t drive;
  CFMutableDictionaryRef match;
  kern_return_t status;
  bool ok;

  // Get an iterator for IOMedia objects.

  match = IOServiceMatching(kIOMediaClass);
  CFDictionaryAddValue(match, CFSTR(kIOMediaWholeKey), kCFBooleanTrue);
  status = IOServiceGetMatchingServices(masterPort, match, &drivelist);
  if (status != KERN_SUCCESS)
    return false;

  // Scan all of the IOMedia objects, and for each
  // object that has a parent IOBlockStorageDriver, save
  // the object's name and the parent (from which we can
  // fetch statistics).
  //
  // XXX What about RAID devices?

  for(ok=false; ((drive = IOIteratorNext(drivelist)) != 0);) {
    if (record_device(drive, dm))
      ok = true;
    IOObjectRelease(drive);
  }

  IOObjectRelease(drivelist);

  return ok;
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
    CFNumberRef number;
    CFDictionaryRef properties;
    CFDictionaryRef statistics;
    uint64_t bytes_read;
    uint64_t bytes_written;
    uint64_t reads;
    uint64_t writes;
    uint64_t read_time;
    uint64_t write_time;
    kern_return_t status;
    drive_map_t dm;
    drive_node_t n;
    md_can_t can;

    if (!record_all_devices(kIOMasterPortDefault, dm))
      throw mdr_exception_t("mdr_disk_t::mdr_refresh: cannot find any disks");

    // clear();

    for(n=dm.begin(); n != dm.end(); n++) {

      // get drive properties
      status = IORegistryEntryCreateCFProperties(n->second,
                 (CFMutableDictionaryRef *) &properties,
                   kCFAllocatorDefault, kNilOptions);

      if (status != KERN_SUCCESS)
        continue;

      // get statistics from properties
      statistics = (CFDictionaryRef)
                     CFDictionaryGetValue(properties,
                       CFSTR(kIOBlockStorageDriverStatisticsKey));

      if (statistics) {

        node = find(n->first);
        if (node == end()) {
          can.clear();

          can.md_type = "physical";

          pthread_mutex_lock(&space_map_lock);

          sm_node_t smn = space_map.find(n->first);
          if (smn == space_map.end()) {
            char path[BUFSIZ];
            snprintf(path, sizeof path, "/dev/%s", n->first.c_str());
            int input = open(path, O_RDONLY);
            if (input != -1) {
              uint32_t bs = 0;
              uint64_t bc = 0;
              if ((ioctl(input, DKIOCGETBLOCKSIZE,  &bs) != -1) &&
                  (ioctl(input, DKIOCGETBLOCKCOUNT, &bc) != -1)) {
                can.md_space_mb = (uint_t) ((bs * bc) / (1024ULL * 1024ULL));
                space_map[n->first] = can.md_space_mb;
              }
              close(input);
            }
          } else
            can.md_space_mb = smn->second;

          pthread_mutex_unlock(&space_map_lock);

          (*this)[n->first] = can;
          node = find(n->first);
        }

        md_can_t *cp = &node->second;

        // get bytes read
        number = (CFNumberRef)
          CFDictionaryGetValue(statistics,
            CFSTR(kIOBlockStorageDriverStatisticsBytesReadKey));

        if (number)
          CFNumberGetValue(number, kCFNumberSInt64Type, &bytes_read);

        // get bytes written
        number = (CFNumberRef)
          CFDictionaryGetValue(statistics,
            CFSTR(kIOBlockStorageDriverStatisticsBytesWrittenKey));

        if (number)
          CFNumberGetValue(number, kCFNumberSInt64Type, &bytes_written);

        // get # of reads
        number = (CFNumberRef)
          CFDictionaryGetValue(statistics,
            CFSTR(kIOBlockStorageDriverStatisticsReadsKey));

        if (number)
          CFNumberGetValue(number, kCFNumberSInt64Type, &reads);

        // get # of writes
        number = (CFNumberRef)
          CFDictionaryGetValue(statistics,
            CFSTR(kIOBlockStorageDriverStatisticsWritesKey));

        if (number)
          CFNumberGetValue(number, kCFNumberSInt64Type, &writes);

        // get read latency
        number = (CFNumberRef)
          CFDictionaryGetValue(statistics,
            CFSTR(kIOBlockStorageDriverStatisticsLatentReadTimeKey));

        if (number)
          CFNumberGetValue(number, kCFNumberSInt64Type, &read_time);

        // get write latency
        number = (CFNumberRef)
          CFDictionaryGetValue(statistics,
            CFSTR(kIOBlockStorageDriverStatisticsLatentWriteTimeKey));

        if (number)
          CFNumberGetValue(number, kCFNumberSInt64Type, &write_time);

        cp->md_reads = reads % ~0U;
        cp->md_read_K = (uint_t) (bytes_read / 1024);
        cp->md_read_msecs = read_time / 1000000;
        cp->md_writes = writes % ~0U;
        cp->md_written_K = (uint_t) (bytes_written / 1024);
        cp->md_write_msecs = write_time / 1000000;
        cp->md_run_msecs = cp->md_read_msecs + cp->md_write_msecs;
        cp->md_io_operations = cp->md_reads + cp->md_writes;

        current_set.insert(n->first);
      }

      CFRelease(properties);
    }
  }

  // We know what *is* there. Now remove any disks in the current set
  // that are *not* there.

  // if it's not in current_set, then mark it for deletion
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

void mdr_ctlr_t::mdr_refresh(void)
{
  // cannot be done on mac
  if (net_client == 0)
    return;

  // remote code only here
  mctl_node_t n;
  del_set_t del_set;
  del_node_t sn;

  ctlr_remote_refresh();

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
      { MNT_DONTBROWSE,           "nobrowse"    },
      { MNT_UNKNOWNPERMISSIONS,   "noperms"     },
      { MNT_AUTOMOUNTED,          "auto"        },
      { MNT_JOURNALED,            "journal"     },
#if OS_VERSION < 830
      { MNT_EXRDONLY,             "expro"       },
#endif
      { MNT_EXPORTED,             "exp"         },
#if OS_VERSION < 830
      { MNT_DEFEXPORTED,          "expworld"    },
      { MNT_EXPORTANON,           "expanon"     },
      { MNT_EXKERB,               "expkerb"     },
#endif
      { MNT_LOCAL,                "local"       },
      { MNT_QUOTA,                "quota"       },
      { MNT_ROOTFS,               "root"        },
      { MNT_DOVOLFS,              "volfs"       },
#if OS_VERSION < 830
      { MNT_FIXEDSCRIPTENCODING,  "fixedscript" },
#endif
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
    bool ok = false;
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

void mdr_ip_t::mdr_refresh(void)
{
  if (net_client) {
    ip_remote_refresh();
    return;
  }

  ipstat_t ipstat;
  size_t len = sizeof ipstat;
  int name[4] = { CTL_NET, PF_INET, IPPROTO_IP, 0 };

  // this transmogrification came from the net-snmp daemon

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

  if (sysctlbyname("net.inet.tcp.stats", &tcpstat, &len, 0, 0) == -1)
    throw mdr_exception_t("mdr_tcp_t::mdr_refresh: cannot sysctlbyname");

  mtcp_tcpRtoAlgorithm = 4;                // Van Jacobsen's algorithm
#if OS_VERSION < 830
  mtcp_tcpRtoMin       = TCPTV_MIN;
  mtcp_tcpRtoMax       = TCPTV_REXMTMAX;
#else
  mtcp_tcpRtoMin       = 0;
  mtcp_tcpRtoMax       = 0;
#endif
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

static void
get_model(cpu_type_t   type, cpu_subtype_t subtype,
          std::string &arch, std::string  &model)
{
  // I'm not going to work real hard for anything but PowerPC
  // ...and x86 I guess :-)
  switch(type) {
  case CPU_TYPE_ANY:
    arch = "unknown";
    model = "unknown";
    break;
  case CPU_TYPE_VAX:
    arch = "vax";
    model = "vax";
    break;
  case CPU_TYPE_MC680x0:
    arch = "mc680x0";
    model = "mc680x0";
    break;
  case CPU_TYPE_I386:
    arch = "GenuineIntel";
    switch(subtype) {
    case CPU_SUBTYPE_I386_ALL:
      model = "Intel(R) 80386";
      break;
    case CPU_SUBTYPE_486:
      model = "Intel(R) 80486";
      break;
    case CPU_SUBTYPE_486SX:
      model = "Intel(R) 80486-sx";
      break;
    case CPU_SUBTYPE_PENT:
      model = "Intel(R) Pentium I";
      break;
    case CPU_SUBTYPE_PENTPRO:
      model = "Intel(R) PentiumPro";
      break;
    case CPU_SUBTYPE_PENTII_M3:
    case CPU_SUBTYPE_PENTII_M5:
      model = "Intel(R) Pentium II";
      break;
    case CPU_SUBTYPE_CELERON:
      model = "Intel(R) Celeron";
      break;
    case CPU_SUBTYPE_CELERON_MOBILE:
      model = "Intel(R) Mobile Celeron";
      break;
    case CPU_SUBTYPE_PENTIUM_3:
    case CPU_SUBTYPE_PENTIUM_3_M:
      model = "Intel(R) Pentium III";
      break;
    case CPU_SUBTYPE_PENTIUM_3_XEON:
      model = "Intel(R) Pentium III Xeon";
      break;
    case CPU_SUBTYPE_PENTIUM_M:
      model = "Intel(R) Mobile Pentium";
      break;
    case CPU_SUBTYPE_PENTIUM_4:
      model = "Intel(R) Pentium IV";
      break;
    case CPU_SUBTYPE_PENTIUM_4_M:
      model = "Intel(R) Mobile Pentium IV";
      break;
    case CPU_SUBTYPE_ITANIUM:
      model = "Intel(R) Itanium";
      break;
    case CPU_SUBTYPE_ITANIUM_2:
      model = "Intel(R) Itanium II";
      break;
    case CPU_SUBTYPE_XEON:
      model = "Intel(R) Xeon";
      break;
    case CPU_SUBTYPE_XEON_MP:
      model = "Intel(R) Xeon MP";
      break;
    default:
      model = "unknown";
      break;
    }
    break;
  case CPU_TYPE_MC98000:
    arch = "mc98000";
    model = "mc98000";
    break;
  case CPU_TYPE_HPPA:
    arch = "hp-pa";
    model = "hp-pa";
    break;
  case CPU_TYPE_MC88000:
    arch = "mc88000";
    model = "mc88000";
    break;
  case CPU_TYPE_SPARC:
    arch = "sparc";
    model = "sparc";
    break;
  case CPU_TYPE_I860:
    arch = "i860";
    model = "i860";
    break;
  case CPU_TYPE_POWERPC:
    arch = "PowerPC";
    switch(subtype) {
    case CPU_SUBTYPE_POWERPC_ALL:
      model = "PowerPC";
      break;
    case CPU_SUBTYPE_POWERPC_601:
      model = "601";
      break;
    case CPU_SUBTYPE_POWERPC_602:
      model = "602";
      break;
    case CPU_SUBTYPE_POWERPC_603:
      model = "603";
      break;
    case CPU_SUBTYPE_POWERPC_603e:
      model = "603e";
      break;
    case CPU_SUBTYPE_POWERPC_603ev:
      model = "603ev";
      break;
    case CPU_SUBTYPE_POWERPC_604:
      model = "604";
      break;
    case CPU_SUBTYPE_POWERPC_604e:
      model = "4e";
      break;
    case CPU_SUBTYPE_POWERPC_620:
      model = "620";
      break;
    case CPU_SUBTYPE_POWERPC_750:
      model = "750";
      break;
    case CPU_SUBTYPE_POWERPC_7400:
      model = "7400";
      break;
    case CPU_SUBTYPE_POWERPC_7450:
      model = "7450";
      break;
    case CPU_SUBTYPE_POWERPC_970:
      model = "970";
      break;
    default:
      model = "unknown";
      break;
    }
    break;
  case CPU_TYPE_POWERPC64:
    arch = "PowerPC";
    model = "PowerPC64";
    break;
  default:
    arch = "unknown";
    model = "unknown";
    break;
  }
}

void mdr_cpuinfo_t::mdr_refresh(void)
{
  if (net_client) {
    cpuinfo_remote_refresh();
    return;
  }

  mach_msg_type_number_t bi_len;
  processor_basic_info_t bi_array;
  host_t h = mach_host_self();
  kern_return_t kr;
  natural_t ncpu;
  char cpu_name[32];
  int i;
  mci_can_t can;
  mci_node_t n;
  del_set_t del_set;
  del_node_t sn;
  char model[128];
  size_t mlen = sizeof(model);
  char arch[128];
  size_t alen = sizeof(arch);
  static mdr_sysctl_t<uint32_t> clkspeed(CTL_HW, HW_CPU_FREQ);

  bi_len = 0;
  bi_array = 0;
  kr = host_processor_info(h, PROCESSOR_BASIC_INFO, &ncpu,
    (processor_info_array_t *) &bi_array, &bi_len);
  if (kr != KERN_SUCCESS)
    throw mdr_exception_t(
      "mdr_cpuinfo_t::mdr_refresh: PROCESSOR_BASIC_INFO: %s",
         mach_error_string(kr));

  clear();

  for(i=0; i<(int)ncpu; i++) {
    can.mci_instance = bi_array[i].slot_num;

    // get the default values anyway
    get_model(bi_array[i].cpu_type, bi_array[i].cpu_subtype,
              can.mci_architecture, can.mci_model);

    // now let's see if they can be improved on
    if (sysctlbyname("machdep.cpu.brand_string", model, &mlen, 0, 0) != -1)
        can.mci_model = model;
    if (sysctlbyname("machdep.cpu.model_string", arch, &alen, 0, 0) != -1)
      can.mci_architecture = arch;

    can.mci_clock_MHz = (*clkspeed / 1000000U);

    snprintf(cpu_name, sizeof cpu_name, "cpu%d", i);
    (*this)[cpu_name] = can;
  }

  vm_deallocate(mach_task_self(), (vm_address_t) bi_array, bi_len);

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
    mach_msg_type_number_t len;
    mach_msg_type_number_t bi_len;
    processor_cpu_load_info_t cpu_array;
    processor_basic_info_t bi_array;
    host_t h = mach_host_self();
    char cpu_name[64];
    kern_return_t kr;
    natural_t ncpu;
    mmp_can_t can;
    natural_t i;

    // start fresh
    clear();

    len = 0;
    bi_array = 0;
    kr = host_processor_info(h, PROCESSOR_BASIC_INFO, &ncpu,
      (processor_info_array_t *) &bi_array, &len);
    if (kr != KERN_SUCCESS)
      throw mdr_exception_t(
        "mdr_cpu_t::gen_cpu_line: PROCESSOR_BASIC_INFO: %s",
           mach_error_string(kr));
    bi_len = len;

    len = 0;
    cpu_array = 0;
    kr = host_processor_info(h, PROCESSOR_CPU_LOAD_INFO, &ncpu,
      (processor_info_array_t *) &cpu_array, &len);
    if (kr != KERN_SUCCESS)
      throw mdr_exception_t(
        "mdr_cpu_t::gen_cpu_line: PROCESSOR_CPU_LOAD_INFO: %s",
           mach_error_string(kr));

    for(i=0; i<ncpu; i++) {
      can.clear();
      can.mmp_cpu_instance = i;
      can.mmp_user_ticks   = cpu_array[i].cpu_ticks[CPU_STATE_USER];
      can.mmp_kernel_ticks = cpu_array[i].cpu_ticks[CPU_STATE_SYSTEM];
      can.mmp_nice_ticks   = cpu_array[i].cpu_ticks[CPU_STATE_NICE];
      can.mmp_idle_ticks   = cpu_array[i].cpu_ticks[CPU_STATE_IDLE];

      snprintf(cpu_name, sizeof cpu_name, "cpu%d", bi_array[i].slot_num);
      (*this)[cpu_name] = can;
    }

    vm_deallocate(mach_task_self(), (vm_address_t) cpu_array, len);
    vm_deallocate(mach_task_self(), (vm_address_t) bi_array, bi_len);
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

static bool get_cmdline(char **cmd_line, pid_t pid)
{
    int     mib[3], argmax, nargs, c = 0;
    size_t  size;
    char    *procargs, *sp, *np, *cp;

    /* Get the maximum process arguments size. */
    mib[0] = CTL_KERN;
    mib[1] = KERN_ARGMAX;

    size = sizeof(argmax);
    if (sysctl(mib, 2, &argmax, &size, NULL, 0) == -1) {
        goto ERROR_A;
    }

    /* Allocate space for the arguments. */
    procargs = (char *)malloc(argmax);
    if (procargs == NULL) {
        goto ERROR_A;
    }

    /*
     * Make a sysctl() call to get the raw argument space of the process.
     * The layout is documented in start.s, which is part of the Csu
     * project.  In summary, it looks like:
     *
     * /---------------\ 0x00000000
     * :               :
     * :               :
     * |---------------|
     * | argc          |
     * |---------------|
     * | arg[0]        |
     * |---------------|
     * :               :
     * :               :
     * |---------------|
     * | arg[argc - 1] |
     * |---------------|
     * | 0             |
     * |---------------|
     * | env[0]        |
     * |---------------|
     * :               :
     * :               :
     * |---------------|
     * | env[n]        |
     * |---------------|
     * | 0             |
     * |---------------| <-- Beginning of data returned by sysctl() is here.
     * | argc          |
     * |---------------|
     * | exec_path     |
     * |:::::::::::::::|
     * |               |
     * | String area.  |
     * |               |
     * |---------------| <-- Top of stack.
     * :               :
     * :               :
     * \---------------/ 0xffffffff
     */
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROCARGS2;
    mib[2] = pid;

    size = (size_t)argmax;
    if (sysctl(mib, 3, procargs, &size, NULL, 0) == -1) {
        goto ERROR_B;
    }

    memcpy(&nargs, procargs, sizeof(nargs));
    cp = procargs + sizeof(nargs);

    /* Skip the saved exec_path. */
    for (; cp < &procargs[size]; cp++) {
        if (*cp == '\0') {
            /* End of exec_path reached. */
            break;
        }
    }
    if (cp == &procargs[size]) {
        goto ERROR_B;
    }

    /* Skip trailing '\0' characters. */
    for (; cp < &procargs[size]; cp++) {
        if (*cp != '\0') {
            /* Beginning of first argument reached. */
            break;
        }
    }
    if (cp == &procargs[size]) {
        goto ERROR_B;
    }
    /* Save where the argv[0] string starts. */
    sp = cp;

    /*
     * Iterate through the '\0'-terminated strings and convert '\0' to ' '
     * until a string is found that has a '=' character in it (or there are
     * no more strings in procargs).  There is no way to deterministically
     * know where the command arguments end and the environment strings
     * start, which is why the '=' character is searched for as a heuristic.
     */
    for (np = NULL; c < nargs && cp < &procargs[size]; cp++) {
        if (*cp == '\0') {
            c++;
            if (np != NULL) {
                /* Convert previous '\0'. */
                *np = ' ';
            }
            /* Note location of current '\0'. */
            np = cp;
        }
    }

    /*
     * sp points to the beginning of the arguments/environment string, and
     * np should point to the '\0' terminator for the string.
     */
    if (np == NULL || np == sp) {
        /* Empty or unterminated string. */
        goto ERROR_B;
    }

    /* Make a copy of the string. */
    asprintf(cmd_line, "%s", sp);

    /* Clean up. */
    free(procargs);
    return true;

    ERROR_B:
    free(procargs);
    ERROR_A:
    *cmd_line = 0;

    return false;
}

static const int STATE_MAX = 7;

// convert a timeval into a double as number of seconds
static inline double TIMEVAL(struct timeval &t)
{
  return (double) t.tv_sec + ((double) t.tv_usec / 1000000.0);
}

static inline double TIME_VALUE_T(struct time_value &t)
{
  return (double) t.seconds + ((double) t.microseconds / 1000000.0);
}

static inline int
mach_state_order(int s, long sleep_time)
{
  switch (s) {
  case TH_STATE_RUNNING:
    return 1;
  case TH_STATE_UNINTERRUPTIBLE:
    return 2;
  case TH_STATE_WAITING:
    return (sleep_time <= 20) ? 3 : 4;
  case TH_STATE_STOPPED:
    return 5;
  case TH_STATE_HALTED:
    return 6;
  default:
    return 7;
  }
}

#ifdef _DEBUG_MEMORY_ISSUES
void cc_darwin_ps_mem_speak(const char *label) {
  pid_t pid = getpid();
  uint_t count;
  kern_return_t err;
  task_basic_info_data_t task_data;
  task_port_t task;

  err = task_for_pid(mach_task_self(), pid, &task);
  if (err != KERN_SUCCESS) {
    puts("no task_for_pid");
    return;
  }

  count = TASK_BASIC_INFO_COUNT;
  err = task_info(task, TASK_BASIC_INFO, (task_info_t) &task_data, &count);
  if (err != KERN_SUCCESS) {
    puts("no task_info");
    mach_port_deallocate(mach_task_self(), task);
    return;
  }

  printf("%s: %7d K\n", label, task_data.resident_size / 1024);
}
#endif

kern_return_t mdr_ps_t::get_task_info(mach_info_t *mi, pid_t pid)
{
  int j;
  int state;
  task_port_t task;
  kern_return_t err;
  kern_return_t r;
  uint_t count;
  uint_t thread_count;
  task_basic_info_data_t task_data;
  task_events_info_data_t task_events;
  vm_region_basic_info_data_64_t b_info;
  vm_address_t address = GLOBAL_SHARED_TEXT_SEGMENT;
  vm_size_t size;
  thread_basic_info_t tb;
  mach_port_t object_name;
#if 0
  task_thread_times_info_t ttimes;
#endif
  policy_timeshare_info_data_t tshare;
  policy_rr_info_data_t rr;
  policy_fifo_info_data_t fifo;
  thread_port_array_t thread_list;
  char state_chars[] = " RUSITH?";

  // get task port
  err = task_for_pid(mach_task_self(), pid, &task);
  if (err != KERN_SUCCESS) {
    return err;
  }

  // get basic info
  count = TASK_BASIC_INFO_COUNT;
  err = task_info(task, TASK_BASIC_INFO, (task_info_t) &task_data, &count);
  if (err != KERN_SUCCESS) {
    mach_port_deallocate(mach_task_self(), task);
    return err;
  }

  // get task events
  count = TASK_EVENTS_INFO_COUNT;
  err = task_info(task, TASK_EVENTS_INFO, (task_info_t) &task_events, &count);
  if (err != KERN_SUCCESS) {
    mach_port_deallocate(mach_task_self(), task);
    return err;
  }

  // given the lack of metrics, I'll assume a minor fault is a fault
  // that did not involve a pagein. I also assume csw means voluntary
  // context switches.

  mi->system_calls = task_events.syscalls_mach + task_events.syscalls_unix;
  mi->context_switches = task_events.csw;
  mi->maj_faults = task_events.pageins;
  mi->min_faults = task_events.faults - task_events.pageins;
 
  // try to determine if this task has the split libraries
  // mapped in... if so, adjust its virtual size down by
  // the 2 segments that are used for split libraries

  count = VM_REGION_BASIC_INFO_COUNT_64;
  err = vm_region_64(task, &address, &size, VM_REGION_BASIC_INFO,
          (vm_region_info_t) &b_info, &count, &object_name);
  if (err != KERN_SUCCESS) {
    mach_port_deallocate(mach_task_self(), task);
    return err;
  }

  if (b_info.reserved && (size == SHARED_TEXT_REGION_SIZE) &&
        task_data.virtual_size >
          (SHARED_TEXT_REGION_SIZE + SHARED_DATA_REGION_SIZE))
    task_data.virtual_size -=
      (SHARED_TEXT_REGION_SIZE + SHARED_DATA_REGION_SIZE);

  mi->size = task_data.virtual_size / 1024;
  mi->rssize = task_data.resident_size / 1024;

#if 0
// this appears to be useless
  count = TASK_THREAD_TIMES_INFO_COUNT;
  err = task_info(task, TASK_THREAD_TIMES_INFO, (task_info_t) &ttimes, &count);
  if (err != KERN_SUCCESS) {
    mach_port_deallocate(mach_task_self(), task);
    return err;
  }
#endif

  // get priority based on scheduling policy
  switch(task_data.policy) {
  case POLICY_TIMESHARE:
    count = POLICY_TIMESHARE_INFO_COUNT;
    err = task_info(task, TASK_SCHED_TIMESHARE_INFO,
                          (task_info_t) &tshare, &count);
    if (err != KERN_SUCCESS) {
      mach_port_deallocate(mach_task_self(), task);
      return err;
    }
        
    mi->priority = tshare.cur_priority;
    break;
  case POLICY_RR:
    count = POLICY_RR_INFO_COUNT;
    err = task_info(task, TASK_SCHED_RR_INFO, (task_info_t) &rr, &count);
    if (err != KERN_SUCCESS) {
      mach_port_deallocate(mach_task_self(), task);
      return err;
    }
    mi->priority = rr.base_priority;
    break;
  case POLICY_FIFO:
    count = POLICY_FIFO_INFO_COUNT;
    err = task_info(task, TASK_SCHED_FIFO_INFO, (task_info_t) &fifo, &count);
    if (err != KERN_SUCCESS) {
      mach_port_deallocate(mach_task_self(), task);
      return err;
    }
    mi->priority = fifo.base_priority;
    break;
  }

  // get thread info
  mi->user_time = 0;
  mi->system_time = 0;
  mi->swapped = true;
  err = task_threads(task, &thread_list, &thread_count);
  if (err != KERN_SUCCESS) {
    mach_port_deallocate(mach_task_self(), task);
    return err;
  }
  mi->thread_count = thread_count;

  state = STATE_MAX;
  tb = new thread_basic_info_data_t[thread_count];

  // traverse the threads
  for (j=0; j<(int)thread_count; j++) {
    int tstate;
    mach_msg_type_number_t tcount;

    // get thread basic info for this thread
    tcount = THREAD_BASIC_INFO_COUNT;
    err = thread_info(thread_list[j], THREAD_BASIC_INFO,
                       (thread_info_t) &tb[j], &tcount);

    if (err == KERN_SUCCESS) {
      // see if the thread's scheduling policy overrides the task's
      switch(tb[j].policy) {
      case POLICY_TIMESHARE:
        tcount = POLICY_TIMESHARE_INFO_COUNT;
        err = thread_info(thread_list[j], THREAD_SCHED_TIMESHARE_INFO,
                            (thread_info_t) &tshare, &tcount);
        if (err != KERN_SUCCESS)
          break;
        if (tshare.cur_priority > mi->priority)
          mi->priority = tshare.cur_priority;
        break;
      case POLICY_FIFO:
        tcount = POLICY_FIFO_INFO_COUNT;
        err = thread_info(thread_list[j], THREAD_SCHED_FIFO_INFO,
                            (thread_info_t) &fifo, &tcount);
        if (err != KERN_SUCCESS)
          break;
        if (fifo.base_priority > mi->priority)
          mi->priority = fifo.base_priority;
        break;
      case POLICY_RR:
        tcount = POLICY_RR_INFO_COUNT;
        err = thread_info(thread_list[j], THREAD_SCHED_RR_INFO,
                            (thread_info_t) &rr, &tcount);
        if (err != KERN_SUCCESS)
          break;
        if (rr.base_priority > mi->priority)
          mi->priority = rr.base_priority;
        break;
      }
    }

    if (err == KERN_SUCCESS) {
      // bump total counters for each thread
      mi->user_time += TIME_VALUE_T(tb[j].user_time);
      mi->system_time += TIME_VALUE_T(tb[j].system_time);
      tstate = mach_state_order(tb[j].run_state, tb[j].sleep_time);
      if (tstate < state)
        state = tstate;
      if ((tb[j].flags & TH_FLAGS_SWAPPED) == 0)
        mi->swapped = false;
    }
    mach_port_deallocate(mach_task_self(), thread_list[j]);
  }

  delete[] tb;

  mi->state_char = state_chars[state];

  r = vm_deallocate(mach_task_self(), (vm_address_t) thread_list,
        sizeof(thread_port_array_t) * thread_count);
#if 0
  // interesting, but not worth throwing and exception...
  if (r != KERN_SUCCESS)
    throw mdr_exception_t("get_task_info: vm_deallocate failed");
#endif

  mach_port_deallocate(mach_task_self(), task);

  return err;
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
    int i;
    int n;
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
    int proc_count;
    size_t nbytes;
    char name[128];
    char path[PSCMDLEN];
    char *cmdline;
    char *cmd;
    char *fq_cmd;
    mps_can_t can;

    mps_procs_runnable = 0;
    mps_procs_sleeping = 0;
    mps_procs_stopped  = 0;
    mps_procs_blocked  = 0;
    mps_procs_zombie   = 0;
    mps_procs_specific = 0;

    // start over
    // clear();

    for(;;) {
      // get nproc
      if (sysctl(mib, 4, 0, &nbytes, 0, 0) == -1)
        throw mdr_exception_t("mdr_ps_t::get_all_procs: sysctl(nproc) failed");

      // total bytes / sizeof proc structure
      n = nbytes / sizeof(kinfo_proc_t);
      if (n > mps_array_max) {
        if (mps_array)
          delete[] mps_array;
        mps_array = new kinfo_proc_t[n];
        mps_array_max = n;
      }

      // get data
      if (sysctl(mib, 4, mps_array, &nbytes, 0, 0) == -1) {
        if (errno == ENOMEM) {
          // proc table grew between two sysctl calls
          delete[] mps_array;
          mps_array = 0;
          continue;
        }
        throw mdr_exception_t("mdr_ps_t::get_all_procs: sysctl(data) failed");
      }
      break;
    }

    // for each process, gather info and accumulate
    proc_count = nbytes / sizeof(kinfo_proc_t);
    for(i=0; i<proc_count; i++) {
      kinfo_proc_t *kip;
      mach_info_t ps;

      // get the kinfo_proc_t from the array
      kip = &mps_array[i];

      // get current data from mach
      // this gets done first, because if it fails, we move on

      memset(&ps, '\0', sizeof ps);
      if (get_task_info(&ps, kip->kp_proc.p_pid) != KERN_SUCCESS)
        continue;

      // key is: process-name[process-id]
      snprintf(name, sizeof name, "%s[%d]",
        kip->kp_proc.p_comm, kip->kp_proc.p_pid);

      node = find(name);
      if (node == end()) {
        can.clear();

        // jam the can with the process info
        can.mps_start      = (long) TIMEVAL(kip->kp_proc.p_starttime);
        can.mps_pid        = kip->kp_proc.p_pid;
        can.mps_ppid       = kip->kp_eproc.e_ppid;
        can.mps_tty_device = kip->kp_eproc.e_tdev;

        // extract the complete command line
        can.mps_file_name = kip->kp_proc.p_comm;
        if (get_cmdline(&cmdline, can.mps_pid)) {
          can.mps_command = cmdline;
          free(cmdline);
        } else
          can.mps_command = can.mps_file_name;

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

      // these values need to be updated on each interval
      cp->mps_uid            = kip->kp_eproc.e_pcred.p_ruid;
      cp->mps_euid           = kip->kp_eproc.e_ucred.cr_uid;
      cp->mps_user_time      = ps.user_time;
      cp->mps_kernel_time    = ps.system_time;
      cp->mps_cpu_time       = cp->mps_user_time + cp->mps_kernel_time;
      cp->mps_size_K         = ps.size;
      cp->mps_rssize_K       = ps.rssize;
      cp->mps_memory_percent =
        (cp->mps_rssize_K * 100.0) / (double) mps_total_mem_K;
      cp->mps_system_calls   = ps.system_calls;
      cp->mps_voluntary_csw  = ps.context_switches;
      cp->mps_disk_faults    = ps.maj_faults;
      cp->mps_page_reclaims  = ps.min_faults;
      cp->mps_state          = ps.state_char;
      cp->mps_priority       = ps.priority;
      cp->mps_nice           = kip->kp_proc.p_nice;
      cp->mps_thread_count   = ps.thread_count;

      cp->mps_user_name = uid_lookup(cp->mps_uid);
      cp->mps_effective_user_name = uid_lookup(cp->mps_euid);
      cp->mps_is_system_process = ((kip->kp_proc.p_flag & P_SYSTEM) != 0);

      // Darwin
      // 
      // us  them
      //
      // R      R Running
      // U      U Uninterruptable
      // S      S Sleeping < 20s
      // S      I Idle     > 20s
      // T      T Stopped
      // X      H Halted
      // Z      Z Zombie

      switch(ps.state_char) {
      case 'R':
        cp->mps_state = MPS_RUNNABLE;
        mps_procs_runnable++;
        break;
      case 'U':
        cp->mps_state = MPS_BLOCKED;
        mps_procs_blocked++;
        break;
      case 'S':
      case 'I':
        cp->mps_state = MPS_SLEEPING;
        mps_procs_sleeping++;
        break;
      case 'T':
        cp->mps_state = MPS_STOPPED;
        mps_procs_stopped++;
      default:
        if (kip->kp_proc.p_stat == SZOMB) {
          cp->mps_state = MPS_ZOMBIE;
          mps_procs_zombie++;
        } else {
          cp->mps_state = MPS_SPECIFIC;
          mps_procs_specific++;
        }
        break;
      }

      // keep track of what procs we know are there
      current_set.insert(name);
    }
  }

  // We know what *is* there. Now remove any procs in the current set
  // that are *not* there.

  // if it's not in current_set, then mark it for deletion
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
    int i;
    int j;
    char name[32];
    task_port_t task;
    kern_return_t err;
    kern_return_t r;
    uint_t thread_count;
    thread_basic_info_t tb;
    mach_msg_type_number_t tcount;
    policy_timeshare_info_data_t tshare;
    policy_rr_info_data_t rr;
    policy_fifo_info_data_t fifo;
    thread_port_array_t thread_list;
    char state_chars[] = " RUSITH?";
    mt_can_t can;
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
    size_t nbytes;
    int n;
    int proc_count;
    kinfo_proc_t *array;

    // start over
    // clear();

    // start out by getting the process info
    for(;;) {
      // get nproc
      if (sysctl(mib, 4, 0, &nbytes, 0, 0) == -1)
        throw mdr_exception_t(
          "mdr_thread_t::get_all_procs: sysctl(nproc) failed");

      // total bytes / sizeof proc structure
      n = nbytes / sizeof(kinfo_proc_t);
      array = new kinfo_proc_t[n];

      // get data
      if (sysctl(mib, 4, array, &nbytes, 0, 0) == -1) {
        if (errno == ENOMEM) {
          // proc table grew between two sysctl calls
          delete[] array;
          array = 0;
          continue;
        }
        delete[] array;
        throw mdr_exception_t(
          "mdr_thread_t::get_all_procs: sysctl(data) failed");
      }
      break;
    }

    proc_count = nbytes / sizeof(kinfo_proc_t);
    for(i=0; i<proc_count; i++) {
      kinfo_proc_t *kip = &array[i];

      // get task port
      err = task_for_pid(mach_task_self(), kip->kp_proc.p_pid, &task);
      if (err != KERN_SUCCESS) {
        delete[] array;
        return;
      }

      // get thread info
      err = task_threads(task, &thread_list, &thread_count);
      if (err != KERN_SUCCESS) {
        delete[] array;
        mach_port_deallocate(mach_task_self(), task);
        return;
      }

      tb = new thread_basic_info_data_t[thread_count];

      // traverse the threads
      for (j=0; j<(int)thread_count; j++) {

        // key is process_name[process_id][thread_id] (no thread id, so index)
        snprintf(name, sizeof name, "%s[%d][%d]",
            kip->kp_proc.p_comm, kip->kp_proc.p_pid, j);

        node = find(name);
        if (node == end()) {

          can.clear();

          can.mt_thread_id = j;
          can.mt_process_id = kip->kp_proc.p_pid;

          (*this)[name] = can;
          node = find(name);
        }

        mt_can_t *cp = &node->second;

        // get thread basic info for this thread
        tcount = THREAD_BASIC_INFO_COUNT;
        err = thread_info(thread_list[j], THREAD_BASIC_INFO,
                           (thread_info_t) &tb[j], &tcount);

        if (err == KERN_SUCCESS) {
          // see if the thread's scheduling policy overrides the task's
          switch(tb[j].policy) {
          case POLICY_TIMESHARE:
            tcount = POLICY_TIMESHARE_INFO_COUNT;
            err = thread_info(thread_list[j], THREAD_SCHED_TIMESHARE_INFO,
                                (thread_info_t) &tshare, &tcount);
            if (err != KERN_SUCCESS)
              break;
            cp->mt_priority = tshare.cur_priority;
            break;
          case POLICY_FIFO:
            tcount = POLICY_FIFO_INFO_COUNT;
            err = thread_info(thread_list[j], THREAD_SCHED_FIFO_INFO,
                                (thread_info_t) &fifo, &tcount);
            if (err != KERN_SUCCESS)
              break;
            cp->mt_priority = fifo.base_priority;
            break;
          case POLICY_RR:
            tcount = POLICY_RR_INFO_COUNT;
            err = thread_info(thread_list[j], THREAD_SCHED_RR_INFO,
                                (thread_info_t) &rr, &tcount);
            if (err != KERN_SUCCESS)
              break;
            cp->mt_priority = rr.base_priority;
            break;
          }
        }

        if (err == KERN_SUCCESS) {
          cp->mt_user_time = TIME_VALUE_T(tb[j].user_time);
          cp->mt_kernel_time = TIME_VALUE_T(tb[j].system_time);
          cp->mt_cpu_time = cp->mt_user_time + cp->mt_kernel_time;
          cp->mt_state = mach_state_order(tb[j].run_state, tb[j].sleep_time);

          // Darwin
          // 
          // us  them
          //
          // R      R Running
          // U      U Uninterruptable
          // S      S Sleeping < 20s
          // S      I Idle     > 20s
          // T      T Stopped
          // X      H Halted
          // Z      Z Zombie

          switch(state_chars[cp->mt_state]) {
          case 'R':
            cp->mt_state = MPS_RUNNABLE;
            break;
          case 'U':
            cp->mt_state = MPS_BLOCKED;
            break;
          case 'S':
          case 'I':
            cp->mt_state = MPS_SLEEPING;
            break;
          case 'T':
            cp->mt_state = MPS_STOPPED;
            break;
          default:
            cp->mt_state = MPS_SPECIFIC;
            break;
          }
        }

        mach_port_deallocate(mach_task_self(), thread_list[j]);

        // all that work for nothing.
        if (err != KERN_SUCCESS)
          continue;

        // track all of the threads that currently exist
        current_set.insert(name);
      }

      delete[] tb;

      r = vm_deallocate(mach_task_self(), (vm_address_t) thread_list,
            sizeof(thread_port_array_t) * thread_count);
#if 0
      // interesting, but not worth throwing and exception...
      if (r != KERN_SUCCESS)
        throw mdr_exception_t("get_task_info: vm_deallocate failed");
#endif

      mach_port_deallocate(mach_task_self(), task);
    }

    delete[] array;
  }

  // We know what *is* there. Now remove any threads in the current set
  // that are *not* there.

  // if it's not in current_set, then mark it for deletion
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

#ifdef _DARWIN_MALLOC_DEBUG
#include <dlfcn.h>

static void *(*mfunc)(size_t) = 0;

void *malloc(size_t size) {
  if (mfunc == 0) {
    void *h = dlopen("libc.dylib", RTLD_LAZY);
    if (h == 0) {
      puts(dlerror());
      exit(1);
    }
    mfunc = (void *(*)(size_t)) dlsym(h, "malloc");
    if (mfunc == 0) {
      puts(dlerror());
      exit(1);
    }
  }

  printf("malloc(%ld)\n", size);
  return (*mfunc)(size);
}
#endif
