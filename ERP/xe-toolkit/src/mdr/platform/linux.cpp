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


#include <set>
#include <unistd.h>
#include <ctype.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/utsname.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/major.h>
#include <sys/types.h>
  // defined in <arch>/types.h but ifdefed by KERNEL
  typedef unsigned char u8;
  typedef unsigned short u16;
  typedef unsigned int u32;
  typedef unsigned long long u64;

  typedef unsigned char __u8;
  typedef unsigned short __u16;
  typedef unsigned int __u32;
  typedef unsigned long long __u64;
#include <linux/ethtool.h>
#include <linux/sockios.h>
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
#include <mdr_client.h>
#include <mdr_exception.h>

using namespace mdr;

typedef std::set<std::string> del_set_t;
typedef del_set_t::iterator   del_node_t;

class dc_map_t : public std::map<std::string, std::string> {
public:
  dc_map_t();
};
typedef dc_map_t::iterator dc_node_t;

typedef std::map<std::string, uint32_t> mib_map_t;
typedef mib_map_t::iterator             mib_node_t;

void mdr_misc_t::mdr_refresh(void)
{
  if (net_client) {
    misc_remote_refresh();
    return;
  }

  file_map stat("/proc/stat");
  file_map loadavg("/proc/loadavg");
  file_map meminfo("/proc/meminfo");
  static long hz;
  static long pgsz;
  static long cpus_conf;
  static long pgk;
  static char hostname[256];
  static char platform[256];
  static char version[_UTSNAME_LENGTH];
  static char parch[256];

  if (!stat)
    throw mdr_exception_t("mdr_misc_t::mdr_refresh: cannot open /proc/stat");
  if (!loadavg)
    throw mdr_exception_t("mdr_misc_t::mdr_refresh: cannot open /proc/loadavg");
  if (!meminfo)
    throw mdr_exception_t("mdr_misc_t::mdr_refresh: cannot open /proc/meminfo");

  if (hz == 0) {
    struct utsname u;

    hz = sysconf(_SC_CLK_TCK); // legacy at this point...
    pgsz = sysconf(_SC_PAGE_SIZE);
    cpus_conf = sysconf(_SC_NPROCESSORS_CONF);
    pgk = pgsz / 1024;

    gethostname(hostname, sizeof hostname);
    uname(&u);
    strcpy(platform, u.sysname);
    strcpy(version, u.release);
    strcpy(parch, u.machine);
  }

  mm_hostname = hostname;
  mm_os_platform = platform;
  mm_os_version = version;
  mm_physical_pages        = meminfo["MemTotal:"][1].to_uint32() / pgk;
  mm_native_page_size      = (int) pgsz;

  mm_processor_arch        = parch;
  mm_processors_configured = (int) cpus_conf;
  mm_processors_online     = (int) sysconf(_SC_NPROCESSORS_ONLN);
  mm_ticks_per_second      = (int) hz;
  mm_boot_time             = stat["btime"][1].to_long();

  mm_interrupts       = stat["intr"][1].to_ulong();
  if (stat["intr"].size() > 2)
    mm_clock_interrupts = stat["intr"][2].to_ulong();
  else
    mm_clock_interrupts = 0;
  mm_system_calls     = 0;
  mm_context_switches = stat["ctxt"][1].to_uint32();

  char *p = strchr(loadavg[0][3].token(), '/');
  mm_process_count = (p ? (uint_t) atoi(p+1) : 0);
  mm_1min_run_queue  = loadavg[0][0].to_double();
  mm_5min_run_queue  = loadavg[0][1].to_double();
  mm_15min_run_queue = loadavg[0][2].to_double();
  mm_current_time    = time(0);
}

void mdr_memory_t::mdr_refresh(void)
{
  if (net_client) {
    memory_remote_refresh();
  } else {

    file_map meminfo("/proc/meminfo");
    static long pgk = sysconf(_SC_PAGE_SIZE) / 1024;

    if (!meminfo)
      throw mdr_exception_t(
        "mdr_memory_t::mdr_refresh: cannot open /proc/meminfo");

    // vmstat in 2.6 kernels and newer (I hope)
    file_map vmstat("/proc/vmstat");
    if (vmstat()) {
      mmem_paged_in_K    = vmstat["pgpgin"][1].to_uint32() * pgk;
      mmem_paged_out_K   = vmstat["pgpgout"][1].to_uint32() * pgk;
      mmem_pages_scanned = 0;
      int nlines = vmstat.size();
      for(int i=0; i<nlines; i++)
        if (strncmp(vmstat[i][0].token(), "pgscan_", 7) == 0)
          mmem_pages_scanned += vmstat[i][1].to_uint32();
      mmem_page_faults = vmstat["pgfault"][1].to_uint32();
      mmem_major_faults = vmstat["pgmajfault"][1].to_uint32();
    } else {
      file_map stat("/proc/stat");

      if (!stat)
        throw mdr_exception_t(
          "mdr_memory_t::mdr_refresh: cannot open /proc/stat");

      mmem_paged_in_K    = stat["page"][1].to_uint32() * pgk;
      mmem_paged_out_K   = stat["page"][2].to_uint32() * pgk;
      mmem_pages_scanned = 0;
      mmem_page_faults = 0;
      mmem_major_faults = 0;
    }
    mmem_free_memory_K = meminfo["MemFree:"][1].to_uint32();

    if (mmem_free_memory_K > old_free_memory_K)
      mmem_memory_freed_K = mmem_free_memory_K - old_free_memory_K;
    else
      mmem_memory_freed_K = 0;

    old_free_memory_K = mmem_free_memory_K;
  }
}

void mdr_cpu_t::mdr_refresh(void)
{
  mcpu_node_t n;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    cpu_remote_refresh();
  } else {
    file_map stat("/proc/stat");
    mcpu_can_t can;
    char name[64];
    static long ncpus = sysconf(_SC_NPROCESSORS_CONF);
  
    if (!stat)
      throw mdr_exception_t("mdr_cpu_t::mdr_refresh: cannot open /proc/stat");
  
    // just start over every time
    clear();
  
    for(int i=0; i<ncpus; i++) {
      snprintf(name, sizeof name, "cpu%d", i);
      line_map &lm = stat[name];
  
      can.clear();
  
      can.mcpu_user_ticks     = lm[1].to_uint32();
      can.mcpu_nice_ticks     = lm[2].to_uint32();
      can.mcpu_kernel_ticks   = lm[3].to_uint32();
      can.mcpu_idle_ticks     = lm[4].to_uint32();
      if (lm.size() > 5) {
        can.mcpu_wait_ticks     = lm[5].to_uint32();
        can.mcpu_irq_ticks      = lm[6].to_uint32();
        can.mcpu_soft_irq_ticks = lm[7].to_uint32();
      }
  
      (*this)[name] = can;
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

int mdr_netif_t::get_address_info(const char *name, mnet_can_t *cp)
{
  int n;
  int sd;
  struct sockaddr_in *sin;
  struct ifreq ifr;
  struct in_addr address;
  struct ethtool_cmd cmd;
  static int requests[] = {
   SIOCGIFADDR, SIOCGIFNETMASK, SIOCGIFBRDADDR, SIOCGIFMTU
  };
  unsigned int i;

  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd == -1)
    return -1;

  memset(&ifr, '\0', sizeof(ifr));
  strncpy(ifr.ifr_name, name, sizeof(ifr.ifr_name));

  for(i=0; i<(sizeof(requests)/sizeof(int)); i++) {
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

  // ethtool requires root; oh well
  cmd.cmd = ETHTOOL_GSET;
  ifr.ifr_data = (caddr_t) &cmd;
  n = ioctl(sd, SIOCETHTOOL, &ifr);
  if (n == 0) {
    switch(cmd.speed) {
    case SPEED_10:
      cp->mnet_interface_speed = 10000000;
      mdr_support_field(MNET_INTERFACE_SPEED);
      break;
    case SPEED_100:
      cp->mnet_interface_speed = 100000000;
      mdr_support_field(MNET_INTERFACE_SPEED);
      break;
    case SPEED_1000:
      cp->mnet_interface_speed = 1000000000;
      mdr_support_field(MNET_INTERFACE_SPEED);
      break;
    default:
      break;
    }
    switch (cmd.duplex) {
    case DUPLEX_HALF:
      cp->mnet_full_duplex = false;
      mdr_support_field(MNET_FULL_DUPLEX);
      break;
    case DUPLEX_FULL:
      cp->mnet_full_duplex = true;
      mdr_support_field(MNET_FULL_DUPLEX);
      break;
    default:
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
    file_map dev("/proc/net/dev", " :");
    mnet_can_t can;
    char *name;

    if (!dev)
      throw mdr_exception_t(
        "mdr_netif_t::mdr_refresh: cannot open /proc/net/dev");

    // start fresh
    clear();

    // start at line 2; 0 and 1 are headers
    for(int i=2; i<(int)dev.size(); i++) {

      can.clear();

      name = dev[i][0].token();
      get_address_info(name, &can);

      can.mnet_input_bytes     = dev[i][1].to_ulong();
      can.mnet_input_packets   = dev[i][2].to_ulong();
      can.mnet_input_errors    = dev[i][3].to_ulong();
      can.mnet_input_failures  = dev[i][4].to_ulong();
      can.mnet_output_bytes    = dev[i][9].to_ulong();
      can.mnet_output_packets  = dev[i][10].to_ulong();
      can.mnet_output_errors   = dev[i][11].to_ulong();
      can.mnet_output_failures = dev[i][12].to_ulong();
      can.mnet_collisions      = dev[i][14].to_ulong();

      (*this)[name] = can;
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
  if (net_client == 0)
    // no cache data in Linux
    return;

  // remote code only here
  mc_node_t n;
  del_set_t del_set;
  del_node_t sn;

  cache_remote_refresh();

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
    file_map swaps("/proc/swaps", " \t");
    mpf_can_t can;
    char *name;

    if (!swaps)
      throw mdr_exception_t("mdr_pf_t::mdr_refresh: cannot open /proc/swaps");

    // start fresh
    clear();

    for(int i=1; i<(int)swaps.size(); i++) {
      name = swaps[i][0].token();
      can.mpf_size_K = swaps[i][2].to_uint32();
      can.mpf_used_K = swaps[i][3].to_uint32();
      can.mpf_free_K = can.mpf_size_K - can.mpf_used_K;
      (*this)[name] = can;
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

void mdr_disk_t::mdr_refresh(void)
{
  del_set_t del_set;
  del_node_t sn;
  md_node_t node;
  del_set_t current_set;

  if (net_client) {
    disk_remote_refresh();
  } else {
    int n;
    md_can_t can;
    const char *type;

    if (access("/proc/diskstats", R_OK) == 0) {
      // kernel 2.6 and later
      file_map ds("/proc/diskstats");

      // get capacity from part file
      file_map part("/proc/partitions");

      if (!ds)
        throw mdr_exception_t(
          "mdr_disk_t::mdr_refresh: cannot open /proc/diskstats");

      if (!part)
        throw mdr_exception_t(
          "mdr_disk_t::mdr_refresh: cannot open /proc/partitions");

      n = ds.size();

      for(int i=0; i<n; i++) {
        line_map &lm = ds[i];
        char *name = lm[2].token();

        node = find(name);
        if (node == end()) {

          // zip
          can.clear();

          // determine 14 field devices
          if (strncmp(name, "ram", 3) == 0)
            type = "ram";
          else if (strncmp(name, "loop", 4) == 0)
            type = "loop";
          else if (strncmp(name, "md", 2) == 0)
            type = "meta";
          else
            type = "physical";

          switch(lm.size()) {
          case 7: // partition
            can.md_type = "logical";
            break;
          case 14: // disk
            can.md_type = type;
            break;
          default:
            continue;
          }

          int x = part.size();

          // partitions file has header, blank line, data
          for(int j=2; j<x; j++) {
            line_map &plm = part[j];
            char *part_name = plm[3].token();

            if (strcmp(part_name, name))
              continue;

            can.md_space_mb = plm[2].to_uint32() / 1024;
            break;
          }

          (*this)[name] = can;
          node = find(name);
        }

        md_can_t *cp = &node->second;

        switch(lm.size()) {
        case 7:   // partition
          cp->md_reads            = lm[3].to_uint32();
          cp->md_read_K           = lm[4].to_uint32() >> 1;
          cp->md_writes           = lm[5].to_uint32();
          cp->md_written_K        = lm[6].to_uint32() >> 1;
          cp->md_io_operations    = cp->md_reads + cp->md_writes;
          break;
        case 14:  // disk
          cp->md_reads            = lm[3].to_uint32();
          cp->md_read_K           = lm[5].to_uint32() >> 1;
          cp->md_read_msecs       = lm[6].to_uint32();
          cp->md_writes           = lm[7].to_uint32();
          cp->md_written_K        = lm[9].to_uint32() >> 1;
          cp->md_write_msecs      = lm[10].to_uint32();
          cp->md_run_msecs        = lm[12].to_uint32();
          cp->md_wait_msecs       = lm[13].to_uint32();
          cp->md_runq_msecs       = cp->md_run_msecs;
          cp->md_waitq_msecs      = cp->md_wait_msecs;
          cp->md_avg_queue_length = cp->md_wait_msecs; // div by elapsed...
          cp->md_io_operations    = cp->md_reads + cp->md_writes;
          break;
        default:
          continue;
        }

        current_set.insert(name);
      }
    } else {

      // pre-2.6
      file_map part("/proc/partitions");

      if (!part)
        throw mdr_exception_t(
          "mdr_disk_t::mdr_refresh: cannot open /proc/partitions");

      n = part.size();

      for(int i=2; i<n; i++) {
        line_map &lm = part[i];
        char *name = lm[3].token();

        if (lm.size() != 15)
          continue;

        node = find(name);
        if (node == end()) {

          can.clear();

          if (isdigit(name[strlen(name) - 1]))
            type = "logical";
          else
            type = "physical";

          can.md_type     = type;
          can.md_space_mb = lm[2].to_uint32() / 1024;

          (*this)[name] = can;
          node = find(name);
        }

        md_can_t *cp = &node->second;

        cp->md_reads            = lm[4].to_uint32();
        cp->md_read_K           = lm[6].to_uint32() >> 1;
        cp->md_read_msecs       = lm[7].to_uint32();
        cp->md_writes           = lm[8].to_uint32();
        cp->md_written_K        = lm[10].to_uint32() >> 1;
        cp->md_write_msecs      = lm[11].to_uint32();
        cp->md_run_msecs        = lm[13].to_uint32();
        cp->md_wait_msecs       = lm[14].to_uint32();
        cp->md_runq_msecs       = lm[13].to_uint32();
        cp->md_waitq_msecs      = lm[14].to_uint32();
        cp->md_avg_queue_length = cp->md_wait_msecs;
        cp->md_io_operations    = cp->md_reads + cp->md_writes;

        // very old kernel bug mhere ios_in_flight turns negative
        // and wait queue time runs backwards

        if (lm[12].to_int32() == -1) {
          cp->md_wait_msecs = 0;
          cp->md_avg_queue_length = 0;
        }

        current_set.insert(name);
      }
    }
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

typedef struct statfs statfs_t;

void mdr_mnt_t::mdr_refresh(void)
{
  mmnt_node_t node;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    mnt_remote_refresh();
  } else {
    file_map mt("/etc/mtab", " \t");
    mmnt_can_t can;
    statfs_t fsbuf;
    struct stat stbuf;
    ulong_t reserved;
    bool ok = false;
    std::map<std::string, bool> host_stat;
    std::map<std::string, bool>::iterator hn;
    char *special;
    char *opts;
    std::string key;

    if (!mt)
      throw mdr_exception_t(
        "mdr_mnt_t::mdr_refresh: cannot open /etc/mtab");

    clear();

    for(int i=0; i<(int)mt.size(); i++) {
      line_map &lm = mt[i];

      if (lm.size() < 3)
        continue;

      special = lm[0].token();

      if (special[0] == '#')
        continue;

      opts = lm[3].token();

      can.clear();
      can.mmnt_special = special;
      can.mmnt_mount_point = lm[1].token();
      can.mmnt_fs_type = lm[2].token();
      can.mmnt_mount_options = (opts ? opts : "");

#if CHECK_NFS_BEFORE_STAT
      if (can.mmnt_fs_type == "nfs") try {
        static unsigned short nfs_port;
        struct sockaddr_in sa;
        struct hostent *hp;
        int sd;

        if (nfs_port == 0) {
          struct servent *sp = getservbyname("nfs", "tcp");

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
        // gdb the code at this point to see mhat happened
        ok = (code == 0);
      } else
        ok = true;
#else
      ok = true;
#endif

      // if it's there, then get info
      if (ok) {
        if (statfs(can.mmnt_mount_point.c_str(), &fsbuf) != -1) {
          can.mmnt_size_K = B2K(fsbuf.f_blocks, fsbuf.f_bsize);
          can.mmnt_free_K = B2K(fsbuf.f_bfree, fsbuf.f_bsize);
          can.mmnt_used_K = can.mmnt_size_K - can.mmnt_free_K;
          reserved = can.mmnt_free_K - B2K(fsbuf.f_bavail, fsbuf.f_bsize);
          if (can.mmnt_size_K == 0)
            can.mmnt_percent_used = 0.0;
          else
            can.mmnt_percent_used =
              ceil((can.mmnt_used_K * 100.0) / (can.mmnt_size_K - reserved));

          can.mmnt_files = fsbuf.f_files;
          can.mmnt_free_files = fsbuf.f_ffree;
        }
        if (stat(can.mmnt_mount_point.c_str(), &stbuf) != -1) {
          can.mmnt_major = major(stbuf.st_rdev);
          can.mmnt_minor = minor(stbuf.st_rdev);
        }
      }

      // make an index string
      // it's the end of the special if special starts with '/'
      std::string &spp = can.mmnt_special;

      if (spp[0] == '/')
        key = spp.substr(spp.rfind('/') + 1, spp.length());
      else
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

dc_map_t::dc_map_t()
{
  mdr::file_map part("/proc/partitions");
  int major;
  char major_name[64];
  const char *ctlr;
  char *name;
  char *p;

  if (!part)
    throw mdr_exception_t(
      "mdr_ctlr_t::mdr_refresh: cannot open /proc/partitions");

  for(int i=2; i<(int)part.size(); i++) {
    name = part[i][3].token();
    p = strchr(name, '/');
    if (p == 0) {
      // hda/sda syntax
      if (isdigit(name[strlen(name) - 1]))
        continue;
    } else {
      // array syntax
      p = strchr(p+1, 'p');
      if (p)
        continue;
    }

    switch((major = part[i][0].to_int32())) {
    case IDE0_MAJOR:
      ctlr = "ide0";
      break;
    case IDE1_MAJOR:
      ctlr = "ide1";
      break;
    case IDE2_MAJOR:
      ctlr = "ide2";
      break;
    case IDE3_MAJOR:
      ctlr = "ide3";
      break;
    case IDE4_MAJOR:
      ctlr = "ide4";
      break;
    case IDE5_MAJOR:
      ctlr = "ide5";
      break;
    case IDE6_MAJOR:
      ctlr = "ide6";
      break;
    case IDE7_MAJOR:
      ctlr = "ide7";
      break;
    case IDE8_MAJOR:
      ctlr = "ide8";
      break;
    case IDE9_MAJOR:
      ctlr = "ide9";
      break;
    case SCSI_DISK0_MAJOR:
      ctlr = "scsi0";
      break;
    case SCSI_DISK1_MAJOR:
      ctlr = "scsi1";
      break;
    case SCSI_DISK2_MAJOR:
      ctlr = "scsi2";
      break;
    case SCSI_DISK3_MAJOR:
      ctlr = "scsi3";
      break;
    case SCSI_DISK4_MAJOR:
      ctlr = "scsi4";
      break;
    case SCSI_DISK5_MAJOR:
      ctlr = "scsi5";
      break;
    case SCSI_DISK6_MAJOR:
      ctlr = "scsi6";
      break;
    case SCSI_DISK7_MAJOR:
      ctlr = "scsi7";
      break;
#ifdef SCSI_DISK8_MAJOR
    case SCSI_DISK8_MAJOR:
      ctlr = "scsi8";
      break;
    case SCSI_DISK9_MAJOR:
      ctlr = "scsi9";
      break;
    case SCSI_DISK10_MAJOR:
      ctlr = "scsi10";
      break;
    case SCSI_DISK11_MAJOR:
      ctlr = "scsi11";
      break;
    case SCSI_DISK12_MAJOR:
      ctlr = "scsi12";
      break;
    case SCSI_DISK13_MAJOR:
      ctlr = "scsi13";
      break;
    case SCSI_DISK14_MAJOR:
      ctlr = "scsi14";
      break;
    case SCSI_DISK15_MAJOR:
      ctlr = "scsi15";
      break;
#endif
    default:
      snprintf(major_name, sizeof major_name, "major%d", major);
      ctlr = major_name;
      break;
    }

    (*this)[name] = ctlr;
  }
}

// this is read-only once built
static dc_map_t dcmap;

void mdr_ctlr_t::mdr_refresh(void)
{
  mctl_node_t node;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    ctlr_remote_refresh();
  } else {
    dc_node_t n;
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

      // map the disk to controller name
      n = dcmap.find(dn->first);
      if (n == dcmap.end())
        continue; // hard luck

      // find the node for this controller or make it and zero it if new
      node = find(n->second);
      if (node == end()) {
        (*this)[n->second].clear();
        node = find(n->second);
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

void mdr_ip_t::mdr_refresh(void)
{
  if (net_client) {
    ip_remote_refresh();
    return;
  }

  file_map snmp("/proc/net/snmp");
  line_map &header = snmp.find("Ip:", 0);
  line_map &data = snmp.find("Ip:", 1);
  mib_map_t mm;

  if (!snmp)
    throw mdr_exception_t(
      "mdr_ip_t::mdr_refresh: cannot open /proc/net/snmp");

  for(int i=1; i<(int)header.size(); i++)
    mm[header[i].token()] = data[i].to_uint32();

  mip_ipForwarding = (int) mm["Forwarding"];
  mip_ipDefaultTTL = (int) mm["DefaultTTL"];
  mip_ipInReceives = mm["InReceives"];
  mip_ipInHdrErrors = mm["InHdrErrors"];
  mip_ipInAddrErrors = mm["InAddrErrors"];
  mip_ipForwDatagrams = mm["ForwDatagrams"];
  mip_ipInUnknownProtos = mm["InUnknownProtos"];
  mip_ipInDiscards = mm["InDiscards"];
  mip_ipInDelivers = mm["InDelivers"];
  mip_ipOutRequests = mm["OutRequests"];
  mip_ipOutDiscards = mm["OutDiscards"];
  mip_ipOutNoRoutes = mm["OutNoRoutes"];
  mip_ipReasmTimeout = (int) mm["ReasmTimeout"];
  mip_ipReasmReqds = mm["ReasmReqds"];
  mip_ipReasmOKs = mm["ReasmOKs"];
  mip_ipReasmFails = mm["ReasmFails"];
  mip_ipFragOKs = mm["FragOKs"];
  mip_ipFragFails = mm["FragFails"];
  mip_ipFragCreates = mm["FragCreates"];
}

void mdr_tcp_t::mdr_refresh(void)
{
  if (net_client) {
    tcp_remote_refresh();
    return;
  }

  file_map snmp("/proc/net/snmp");
  line_map &header = snmp.find("Tcp:", 0);
  line_map &data = snmp.find("Tcp:", 1);
  mib_map_t mm;

  if (!snmp)
    throw mdr_exception_t(
      "mdr_tcp_t::mdr_refresh: cannot open /proc/net/snmp");

  for(int i=1; i<(int)header.size(); i++)
    mm[header[i].token()] = data[i].to_uint32();

  mtcp_tcpRtoAlgorithm = mm["RtoAlgorithm"];
  mtcp_tcpRtoMin = mm["RtoMin"];
  mtcp_tcpRtoMax = mm["RtoMax"];
  mtcp_tcpMaxConn = mm["MaxConn"];
  mtcp_tcpActiveOpens = mm["ActiveOpens"];
  mtcp_tcpPassiveOpens = mm["PassiveOpens"];
  mtcp_tcpAttemptFails = mm["AttemptFails"];
  mtcp_tcpEstabResets = mm["EstabResets"];
  mtcp_tcpCurrEstab = mm["CurrEstab"];
  mtcp_tcpInSegs = mm["InSegs"];
  mtcp_tcpOutSegs = mm["OutSegs"];
  mtcp_tcpRetransSegs = mm["RetransSegs"];
  mtcp_tcpInErrs = mm["InErrs"];
  mtcp_tcpOutRsts = mm["OutRsts"];
}

void mdr_udp_t::mdr_refresh(void)
{
  if (net_client) {
    udp_remote_refresh();
    return;
  }

  file_map snmp("/proc/net/snmp");
  line_map &header = snmp.find("Udp:", 0);
  line_map &data = snmp.find("Udp:", 1);
  mib_map_t mm;

  if (!snmp)
    throw mdr_exception_t(
      "mdr_udp_t::mdr_refresh: cannot open /proc/net/snmp");

  for(int i=1; i<(int)header.size(); i++)
    mm[header[i].token()] = data[i].to_uint32();

  mudp_udpInDatagrams = mm["InDatagrams"];
  mudp_udpNoPorts = mm["NoPorts"];
  mudp_udpInErrors = mm["InErrors"];
  mudp_udpOutDatagrams = mm["OutDatagrams"];
}

void mdr_icmp_t::mdr_refresh(void)
{
  if (net_client) {
    icmp_remote_refresh();
    return;
  }

  file_map snmp("/proc/net/snmp");
  line_map &header = snmp.find("Icmp:", 0);
  line_map &data = snmp.find("Icmp:", 1);
  mib_map_t mm;

  if (!snmp)
    throw mdr_exception_t(
      "mdr_icmp_t::mdr_refresh: cannot open /proc/net/snmp");

  for(int i=1; i<(int)header.size(); i++)
    mm[header[i].token()] = data[i].to_uint32();

  micmp_icmpInMsgs = mm["InMsgs"];
  micmp_icmpInErrors = mm["InErrors"];
  micmp_icmpInDestUnreachs = mm["InDestUnreachs"];
  micmp_icmpInTimeExcds = mm["InTimeExcds"];
  micmp_icmpInParmProbs = mm["InParmProbs"];
  micmp_icmpInSrcQuenchs = mm["InSrcQuenchs"];
  micmp_icmpInRedirects = mm["InRedirects"];
  micmp_icmpInEchos = mm["InEchos"];
  micmp_icmpInEchoReps = mm["InEchoReps"];
  micmp_icmpInTimestamps = mm["InTimestamps"];
  micmp_icmpInTimestampReps = mm["InTimestampReps"];
  micmp_icmpInAddrMasks = mm["InAddrMasks"];
  micmp_icmpInAddrMaskReps = mm["InAddrMaskReps"];
  micmp_icmpOutMsgs = mm["OutMsgs"];
  micmp_icmpOutErrors = mm["OutErrors"];
  micmp_icmpOutDestUnreachs = mm["OutDestUnreachs"];
  micmp_icmpOutTimeExcds = mm["OutTimeExcds"];
  micmp_icmpOutParmProbs = mm["OutParmProbs"];
  micmp_icmpOutSrcQuenchs = mm["OutSrcQuenchs"];
  micmp_icmpOutRedirects = mm["OutRedirects"];
  micmp_icmpOutEchos = mm["OutEchos"];
  micmp_icmpOutEchoReps = mm["OutEchoReps"];
  micmp_icmpOutTimestamps = mm["OutTimestamps"];
  micmp_icmpOutTimestampReps = mm["OutTimestampReps"];
  micmp_icmpOutAddrMasks = mm["OutAddrMasks"];
  micmp_icmpOutAddrMaskReps = mm["OutAddrMaskReps"];
}

void mdr_cpuinfo_t::mdr_refresh(void)
{
  if (net_client) {
    cpuinfo_remote_refresh();
    return;
  }

  file_map cpu("/proc/cpuinfo", "\t:");
  long ncpus = sysconf(_SC_NPROCESSORS_ONLN);
  char name[32];
  mci_can_t can;
  mci_node_t n;
  del_set_t del_set;
  del_node_t sn;

  if (!cpu)
    throw mdr_exception_t(
      "mdr_cpuinfo_t::mdr_refresh: cannot open /proc/cpuinfo");

  clear();

  for(int i=0; i<ncpus; i++) {
    can.clear();
#if defined(i386) || defined(i486) || defined(i586) || defined(i686) || defined(x86_64)
    can.mci_instance = cpu.find("processor", i)[1].to_int32();
    can.mci_architecture = cpu.find("vendor_id", i)[1].token() + 1;
    can.mci_model = cpu.find("model name", i)[1].token() + 1;
    can.mci_clock_MHz = cpu.find("cpu MHz", i)[1].to_uint32();
#endif
#if defined(ppc) || defined(ppc64)
    can.mci_instance = cpu.find("processor", i)[1].to_int32();
    can.mci_architecture = "PowerPC";
    can.mci_model = cpu.find("cpu", i)[1].token() + 1;
    can.mci_model += "; rev";
    can.mci_model += cpu.find("revision", i)[1].token();
    can.mci_clock_MHz = cpu.find("clock", i)[1].to_uint32();
#endif
#if defined(s390) || defined(s390x)
    can.mci_instance = i;
    can.mci_architecture = cpu.find("vendor_id       ", i)[1].token() + 1;
    // XXX: fudge
    can.mci_model = cpu.find("processor 0", i)[1].token() + 1;
    // XXX: fudge
    can.mci_clock_MHz = cpu.find("bogomips per cpu", i)[1].to_uint32() / 2;
#endif

    snprintf(name, sizeof name, "cpu%d", i);
    (*this)[name] = can;
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

void mdr_mp_t::mdr_refresh(void)
{
  mmp_node_t n;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    mp_remote_refresh();
  } else {
    file_map stat("/proc/stat");
    mmp_can_t can;
    char name[64];
    static long ncpus = sysconf(_SC_NPROCESSORS_CONF);

    if (!stat)
      throw mdr_exception_t(
        "mdr_mp_t::mdr_refresh: cannot open /proc/stat");

    clear();

    for(int i=0; i<ncpus; i++) {
      snprintf(name, sizeof name, "cpu%d", i);
      line_map &lm = stat[name];

      can.clear();

      can.mmp_cpu_instance   = i;
      can.mmp_user_ticks     = lm[1].to_uint32();
      can.mmp_nice_ticks     = lm[2].to_uint32();
      can.mmp_kernel_ticks   = lm[3].to_uint32();
      can.mmp_idle_ticks     = lm[4].to_uint32();
      if (lm.size() > 5) {
        can.mmp_wait_ticks     = lm[5].to_uint32();
        can.mmp_irq_ticks      = lm[6].to_uint32();
        can.mmp_soft_irq_ticks = lm[7].to_uint32();
      }

      (*this)[name] = can;
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

static bool get_command(int pid, std::string &cmd)
{
  char path[128];
  char buf[1024];
  int input;
  int n;

  snprintf(path, sizeof path, "/proc/%d/cmdline", pid);
  input = open(path, O_RDONLY);
  if (input == -1)
    return false;
  memset(buf, '\0', sizeof buf);
  n = read(input, buf, sizeof buf);
  close(input);
  if (n <= 0)
    return false;
  for(int i=0; i<n-1; i++)
    if (buf[i] == '\0')
      buf[i] = ' ';
  cmd = buf;
  return true;
}

// map process id to thread info
typedef std::map<pid_t, int> thread_map_t;

void mdr_ps_t::mdr_refresh(void)
{
  mps_node_t node;
  del_set_t del_set;
  del_node_t sn;
  bool dot_thread = (access("/proc/1/task", R_OK) == -1);
  thread_map_t thread_map;
  del_set_t current_set;

  if (net_client) {
    ps_remote_refresh();
  } else {
    dir_list_t proc("/proc");
    dir_node_t n;
    char path[BUFSIZ];
    char taskpath[BUFSIZ];
    char name[128];
    const char *cmd;
    char *fq_cmd;
    char *p;
    char *q;
    mps_can_t can;

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
      if ((*n)[0] == '.') {
        // .thread nomenclature

        // the stat file path
        snprintf(path, sizeof path, "/proc/%s/stat", n->c_str());
        file_map stat(path);

        // the first line of the file
        line_map &lm = stat[0];

        // index by pid
        pid_t pid = (pid_t) lm[3].to_int32();

        // count threads
        thread_map[pid]++;

        // carry on
        continue;
      }

      // all else
      if (! isdigit((*n)[0]))
        continue;

      // gather the stat files together
      snprintf(path, sizeof path, "/proc/%s/stat", n->c_str());
      file_map stat(path);
      snprintf(path, sizeof path, "/proc/%s/status", n->c_str());
      file_map status(path);

      // probably went away
      if (!stat || !status)
        continue;

      // everything is good to go...

      line_map &lm = stat[0];
      line_map &ul = status["Uid:"];

      can.clear();

      // pid
      can.mps_pid = lm[0].to_int32();

      // take (ksh) and remove ( and )
      p = lm[1].token() + 1;
      q = strchr(p, ')');
      if (q)
        *q = '\0';
      can.mps_file_name = p;

      // key is process_name[process_id]
      snprintf(name, sizeof name, "%s[%d]", 
          can.mps_file_name.c_str(), can.mps_pid);

      // find existing node
      node = find(name);
      if (node == end()) {
        // It's not there. New process. Collect static info.

        can.mps_start =
          time(0) - (lm[21].to_uint32() / mps_misc->mm_ticks_per_second);
        can.mps_ppid = lm[3].to_int32();
        can.mps_tty_device = lm[6].to_long();

        if (!get_command(can.mps_pid, can.mps_command))
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

      // now just deal with existing node
      mps_can_t *cp = &node->second;

      cp->mps_uid = ul[1].to_int32();
      cp->mps_euid = ul[2].to_int32();

      // initialize
      cp->mps_user_time = 0;
      cp->mps_kernel_time = 0;
      cp->mps_thread_count = 0;

      if (dot_thread) {
        // .thread nomenclature

        // account for the main process as a thread
        thread_map[cp->mps_pid]++;

        // increment counters
        cp->mps_user_time =
          (lm[41].to_double() / mps_misc->mm_ticks_per_second);
        cp->mps_kernel_time =
          (lm[42].to_double() / mps_misc->mm_ticks_per_second);
      } else {
        // /proc/pid/task nomenclature

        snprintf(taskpath, sizeof taskpath, "/proc/%s/task", n->c_str());

        // all of the task processes
        dir_list_t task(taskpath);
        dir_node_t tn;

        // accumulate user and kernel time from all the tasks
        for(tn=task.begin(); tn != task.end(); tn++) {
          snprintf(path, sizeof path, "%s/%s/stat", taskpath, tn->c_str());
          file_map tstat(path);

          if (!stat)
            continue;

          line_map &tlm = tstat[0];

          cp->mps_thread_count++;
          cp->mps_user_time +=
            (tlm[13].to_double() / mps_misc->mm_ticks_per_second);
          cp->mps_kernel_time +=
            (tlm[14].to_double() / mps_misc->mm_ticks_per_second);
        }
        cp->mps_cpu_time = cp->mps_user_time + cp->mps_kernel_time;
      }

      cp->mps_size_K = (ulong_t) lm[22].to_uint64() / 1024ULL;
      cp->mps_rssize_K =
        lm[23].to_ulong() * (mps_misc->mm_native_page_size / 1024);
      cp->mps_memory_percent =
        (cp->mps_rssize_K * 100.0) / (double) mps_total_mem_K;
      if (lm.size() == 43) {
        cp->mps_voluntary_csw = lm[41].to_uint32();
        cp->mps_involuntary_csw = lm[42].to_uint32();
        mdr_support_field(MPS_VOLUNTARY_CSW);
        mdr_support_field(MPS_INVOLUNTARY_CSW);
      }
      cp->mps_disk_faults = lm[11].to_uint32();
      cp->mps_page_reclaims = lm[9].to_uint32();
      cp->mps_state = lm[2].token()[0];
      switch(cp->mps_state) {
      case 'R':
        cp->mps_state = MPS_RUNNABLE;
        mps_procs_runnable++;
        break;
      case 'D':
        cp->mps_state = MPS_BLOCKED;
        mps_procs_blocked++;
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
      cp->mps_priority = lm[17].to_int32();
      cp->mps_nice = lm[18].to_int32();

      cp->mps_user_name = uid_lookup(cp->mps_uid);
      cp->mps_effective_user_name = uid_lookup(cp->mps_euid);
      cp->mps_is_system_process = (cp->mps_size_K == 0);

      current_set.insert(name);
    }
  }

  // We know what *is* there. Now remove any procs in the current set
  // that are *not* there.

  for(node=begin(); node != end(); node++) {
    // if it's not in current_set, then mark it for deletion
    if (current_set.find(node->first) == current_set.end())
      del_set.insert(node->first);
    else {
      // it exists. update its thread count
      if (dot_thread)
        node->second.mps_thread_count = thread_map[node->second.mps_pid];

      // it exists. make sure there's an old node for it
      if (old.find(node->first) == old.end())
        old[node->first].clear();
    }
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
  snprintf(name, MAX_TT_LEN, tp->td_name, minor(dev));
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
    { "/dev/console",    "console",  0,  0           },
    { "SEARCH:/dev/pts", "pts/%d",   0,  pts_compose },
    { "/dev/tty0",       "tty%d",    0,  tty_compose },
    { "/dev/ttyp0",      "tty%c%d",  0,  pty_compose },
    { 0,                 0,          0,  0           }
  };
  char name[MAX_TT_LEN];
  struct tty_devs *tp;
  struct stat stbuf;

  if (initialized == 0) {
    for(tp=tty_map; tp->td_device; tp++) {
      if (strncmp(tp->td_device, "SEARCH", 6) == 0) {
        char *p = strchr(tp->td_device, ':') + 1;
        dir_list_t tty(p);
        if (!tty)
          continue;
        dir_node_t n = tty.begin();
        char name[128];
        snprintf(name, sizeof name, "%s/%s", p, n->c_str());
        if (stat(name, &stbuf) == -1)
          continue;
      } else
        if (stat(tp->td_device, &stbuf) == -1)
          continue;

      tp->td_major = major(stbuf.st_rdev);
    }
    initialized = 1;
  }
  for(tp=tty_map; tp->td_device; tp++) {
    if ((int)major(dev) == tp->td_major) {
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
    dir_list_t *task = 0;
    dir_node_t n;
    dir_node_t t;
    char taskpath[BUFSIZ];
    char path[BUFSIZ];
    char name[128];
    mt_can_t can;
    bool dot_thread = false;

    // if task dir exists, then not using .1960 as threads
    snprintf(path, sizeof path, "/proc/%d/task", getpid());
    if (access(path, R_OK) == -1)
      dot_thread = true;

    // tell dir_list_t whether to include . files
    dir_list_t proc("/proc", dot_thread);

    if (!proc)
      throw mdr_exception_t(
        "mdr_thread_t::mdr_refresh: cannot opendir /proc");

    // start over
    // clear();

    // no point convoluting the code with lots of ifs; just segregate
    if (dot_thread) {
      for(n=proc.begin(); n != proc.end(); n++) {
        if (*n == "." || *n == "..")
          continue;
        if ((*n)[0] != '.')
          continue;

        // get task stat file
        snprintf(path, sizeof path, "/proc/%s/stat", n->c_str());
        file_map stat(path);

        // thread gone
        if (!stat)
          continue;

        // one line
        line_map &lm = stat[0];

        // populate the can
        can.clear();

        can.mt_thread_id = lm[0].to_int32();
        can.mt_process_id = lm[3].to_int32();

        // extract the file name from stat
        pid_t ppid = lm[3].to_int32();
        snprintf(path, sizeof path, "/proc/%d/stat", ppid);
        file_map pstat(path);
        line_map &plm = pstat[0];
        char *p = plm[1].token() + 1;
        char *q = strchr(p, ')');
        if (q)
          *q = '\0';

        // this is the key: file_name[process_id][thread_id]
        snprintf(name, sizeof name, "%s[%d][%d]",
          p, can.mt_process_id, can.mt_thread_id);

        // try to find it
        node = find(name);
        if (node == end()) {
          (*this)[name] = can;
          node = find(name);
        }

        mt_can_t *cp = &node->second;

        cp->mt_user_time = lm[13].to_double() / mt_misc->mm_ticks_per_second;
        cp->mt_kernel_time = lm[14].to_double() / mt_misc->mm_ticks_per_second;
        cp->mt_cpu_time = cp->mt_user_time + cp->mt_kernel_time;
        cp->mt_size_K = (ulong_t) lm[22].to_uint64() / 1024ULL;
        cp->mt_rssize_K =
          lm[23].to_ulong() * (mt_misc->mm_native_page_size / 1024);
        cp->mt_memory_percent =
          (cp->mt_rssize_K * 100.0) / (double) mt_total_mem_K;
        cp->mt_disk_faults = lm[11].to_uint32();
        cp->mt_page_reclaims = lm[9].to_uint32();
        cp->mt_state = lm[2].token()[0];
        switch(cp->mt_state) {
        case 'R':
          cp->mt_state = MPS_RUNNABLE;
          break;
        case 'D':
          cp->mt_state = MPS_BLOCKED;
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
        cp->mt_priority = lm[17].to_int32();
        cp->mt_nice = lm[18].to_int32();

        current_set.insert(name);
      }
    } else {
      for(n=proc.begin(); n != proc.end(); n++) {
        // skip /proc/stat, et al
        if (!isdigit((*n)[0]))
          continue;

        // traverse task directory
        snprintf(taskpath, sizeof taskpath, "/proc/%s/task", (*n).c_str());
        task = new dir_list_t(taskpath);
        if (!(*task)) {
          delete task;
          continue;
        }

        // for each task
        for(t=task->begin(); t != task->end(); t++) {

          // get its stat file
          snprintf(path, sizeof path, "%s/%s/stat", taskpath, t->c_str());
          file_map stat(path);

          // gone
          if (!stat)
            continue;

          // one line
          line_map &lm = stat[0];

          // extract the file name from stat
          char *p = lm[1].token() + 1;
          char *q = strchr(p, ')');
          if (q)
            *q = '\0';

          // populate
          can.clear();

          can.mt_process_id = lm[3].to_int32();
          can.mt_thread_id = lm[0].to_int32();

          // this is the key: file_name[process_id][thread_id]
          snprintf(name, sizeof name, "%s[%d][%d]",
            p, can.mt_process_id, can.mt_thread_id);

          // get it if we got it
          node = find(name);
          if (node == end()) {
            (*this)[name] = can;
            node = find(name);
          }

          mt_can_t *cp = &node->second;

          cp->mt_user_time = lm[13].to_double() / mt_misc->mm_ticks_per_second;
          cp->mt_kernel_time = lm[14].to_double() / mt_misc->mm_ticks_per_second;
          cp->mt_cpu_time = cp->mt_user_time + cp->mt_kernel_time;
          cp->mt_size_K = (ulong_t) lm[22].to_uint64() / 1024ULL;
          cp->mt_rssize_K =
            lm[23].to_ulong() * (mt_misc->mm_native_page_size / 1024);
          cp->mt_memory_percent =
            (cp->mt_rssize_K * 100.0) / (double) mt_total_mem_K;
          if (lm.size() == 43) {
            cp->mt_voluntary_csw = lm[41].to_uint32();
            cp->mt_involuntary_csw = lm[42].to_uint32();
            mdr_support_field(MT_VOLUNTARY_CSW);
            mdr_support_field(MT_INVOLUNTARY_CSW);
          }
          cp->mt_disk_faults = lm[11].to_uint32();
          cp->mt_page_reclaims = lm[9].to_uint32();
          cp->mt_state = lm[2].token()[0];
          switch(cp->mt_state) {
          case 'R':
            cp->mt_state = MPS_RUNNABLE;
            break;
          case 'D':
            cp->mt_state = MPS_BLOCKED;
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
          cp->mt_priority = lm[17].to_int32();
          cp->mt_nice = lm[18].to_int32();

          current_set.insert(name);
        }

        if (task)
          delete task;
      }
    }
  }

  // We know what *is* there. Now remove any threads in the current set
  // that are *not* there.

  // now that the "now" is built, make sure there are old nodes for them
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
