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
#include <map>
#include <string>
#include <unistd.h>
#include <procinfo.h>
#include <libperfstat.h>
#include <netdb.h>
#include <math.h>
#include <sys/utsname.h>
#include <sys/systemcfg.h>
#include <mdr_misc.h>
#include <mdr_cpu.h>
#include <mdr_cache.h>
#include <mdr_cpuinfo.h>
#include <mdr_mp.h>
#include <mdr_ctlr.h>
#include <mdr_disk.h>
#include <mdr_mnt.h>
#include <mdr_pf.h>
#undef NOACCESS
#include <mdr_icmp.h>
#include <mdr_ip.h>
#include <mdr_memory.h>
#include <mdr_netif.h>
#include <mdr_ps.h>
#include <mdr_tcp.h>
#include <mdr_thread.h>
#include <mdr_udp.h>
#include <odm_types.h>
#include <odm_t.h>
#include <sys/mntctl.h>
#include <sys/vmount.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/socket.h>
#include <sys/vminfo.h>
#include <sys/ioctl.h>
#include <sys/sysmacros.h>
#include <net/if.h>

namespace mdr {

typedef std::set<std::string> del_set_t;
typedef del_set_t::iterator   del_node_t;

static const unsigned int MAX_INTEGER = (unsigned int) -1;

// help me to understand why this isn't declared in procinfo.h
extern "C" {
  extern int getprocs64(procentry64 *, int, fdsinfo *, int, pid_t *, int);
  extern int getargs(procentry64 *, int, char *, int);
  extern int getthrds64(pid_t, thrdentry64 *, int, tid64_t *, int);
}

typedef struct vmount vmount_t;
typedef struct statvfs statvfs_t;
typedef struct stat stat_t;
typedef struct pginfo pginfo_t;

void mdr_misc_t::mdr_refresh(void)
{
  if (net_client) {
    misc_remote_refresh();
    return;
  }

  int count = 1 << 16;
  pid_t pid = 0;
  perfstat_cpu_total_t ctinfo;

  static char hostname[256];
  static char platform[256];
  static char version[_SYS_NMLN];
  static char parch[256];
  static uint_t physmem;
  static int pgsz;
  static int cpus_conf;
  static int hertz;

  if (perfstat_cpu_total(0, &ctinfo, sizeof(perfstat_cpu_total_t), 1) <= 0)
    throw mdr_exception_t(
      "mdr_misc_t::mdr_refresh: perfstat_cpu_total fails");

  if (hertz == 0) {
    struct utsname u;
    perfstat_memory_total_t minfo;

    if (perfstat_memory_total(0, &minfo, sizeof(perfstat_memory_total_t), 1) <= 0)
      throw mdr_exception_t(
        "mdr_misc_t::mdr_refresh: perfstat_memory_total fails");
    physmem = (uint_t) minfo.real_total;
    pgsz = (int) sysconf(_SC_PAGE_SIZE);
    cpus_conf = sysconf(_SC_NPROCESSORS_CONF);
    hertz = (int) sysconf(_SC_CLK_TCK);
    gethostname(hostname, sizeof hostname);
    uname(&u);
    snprintf(platform, sizeof platform, "%s", u.sysname);
    snprintf(version, sizeof version, "%s.%s", u.version, u.release);
    snprintf(parch, sizeof parch, "%s", ctinfo.description);
  }

  mm_hostname = hostname;
  mm_os_platform = platform;
  mm_os_version = version;
  mm_physical_pages = physmem;
  mm_native_page_size = pgsz;
  mm_processor_arch = parch;
  mm_processors_configured = cpus_conf;
  mm_processors_online = sysconf(_SC_NPROCESSORS_ONLN);
  mm_ticks_per_second = hertz;
  mm_boot_time = time(0) - (ctinfo.lbolt / hertz);
  mm_interrupts = ctinfo.devintrs + ctinfo.lbolt;
  mm_clock_interrupts = ctinfo.lbolt;
  mm_system_calls = ctinfo.syscall % MAX_INTEGER;
  mm_context_switches = ctinfo.pswitch % MAX_INTEGER;
  mm_process_count = getprocs64(0, sizeof(procentry64), 0, 0, &pid, count);
  mm_1min_run_queue = (double) ctinfo.loadavg[0] / (double) (1 << SBITS);
  mm_5min_run_queue = (double) ctinfo.loadavg[1] / (double) (1 << SBITS);
  mm_15min_run_queue = (double) ctinfo.loadavg[2] / (double) (1 << SBITS);
  mm_current_time = time(0);
}

void mdr_memory_t::mdr_refresh()
{
  if (net_client) {
    memory_remote_refresh();
    return;
  }

  perfstat_memory_total_t minfo;

  static int pgsz = 0;

  if (pgsz == 0)
    pgsz = sysconf(_SC_PAGE_SIZE);

  if (perfstat_memory_total(0, &minfo, sizeof(perfstat_memory_total_t), 1) <= 0)
    throw mdr_exception_t(
      "mdr_memory_t::mdr_refresh: perfstat_memory_total fails");

  mmem_paged_in_K = (uint_t) (((minfo.pgins % MAX_INTEGER) * pgsz) / 1024ULL);
  mmem_paged_out_K = (uint_t) (((minfo.pgouts % MAX_INTEGER) * pgsz) / 1024ULL);
  mmem_pages_scanned = (uint_t) (minfo.scans % MAX_INTEGER);
  mmem_free_memory_K =
    (uint_t) (((minfo.real_free % MAX_INTEGER) * pgsz) / 1024ULL);

  if (mmem_free_memory_K > old_free_memory_K)
    mmem_memory_freed_K = mmem_free_memory_K - old_free_memory_K;
  else
    mmem_memory_freed_K = 0;
}

void mdr_cpu_t::mdr_refresh()
{
  if (net_client) {
    cpu_remote_refresh();
    return;
  }

  int cpu_count = perfstat_cpu(0, 0, sizeof(perfstat_cpu_t), 0);
  if (cpu_count <= 0)
    throw mdr_exception_t(
      "mdr_cpu_t::mdr_refresh: puerfstat_cpu fails");
  perfstat_cpu_t *cinfo = new perfstat_cpu_t[cpu_count];
  perfstat_id_t cpu;
  mcpu_can_t can;
  mcpu_node_t n;
  del_set_t del_set;
  del_node_t sn;

  strcpy(cpu.name, FIRST_CPU);
  cpu_count = perfstat_cpu(&cpu, cinfo, sizeof(perfstat_cpu_t), cpu_count);

  // start new
  clear();

  for(int i=0; i<cpu_count; i++) {
    can.clear();

    can.mcpu_user_ticks = cinfo[i].user % MAX_INTEGER;
    can.mcpu_kernel_ticks = cinfo[i].sys % MAX_INTEGER;
    can.mcpu_wait_ticks = cinfo[i].wait % MAX_INTEGER;
    can.mcpu_idle_ticks = cinfo[i].idle % MAX_INTEGER;

    (*this)[cinfo[i].name] = can;
  }

  delete[] cinfo;

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

void mdr_cache_t::mdr_refresh()
{
  if (net_client) {
    cache_remote_refresh();
    return;
  }

  // no cache stats on AIX
  return;
}

void mdr_cpuinfo_t::mdr_refresh()
{
  if (net_client) {
    cpuinfo_remote_refresh();
    return;
  }

  // start fresh
  clear();

  odm_t<PdDv> pddv(PdDv_CLASS);
  int n = pddv.size();
  int i;
  char *prefix = 0;
  char *arch = 0;
  char *model = 0;
  char format[128];
  char name[128];
  int instance;
  int clock_speed = -1;
  mci_can_t can;
  mci_node_t node;
  del_set_t del_set;
  del_node_t sn;

  switch(_system_configuration.implementation) {
  case POWER_RS1:
     model = "RS1";
     break;
  case POWER_RSC:
     model = "RSC";
     break;
  case POWER_RS2:
     model = "RS2";
     break;
  case POWER_601:
     model = "601";
     break;
  case POWER_603:
     model = "603";
     break;
  case POWER_604:
     model = "604";
     break;
  case POWER_630:
     model = "630";
     break;
  case POWER_A35:
     model = "A35";
     break;
  case POWER_RS64II:
     model = "RS64 II";
     break;
  case POWER_RS64III:
     model = "RS64 III";
     break;
  case POWER_4:
     model = "Power4";
     break;
  case POWER_MPC7450:
     model = "7450";
     break;
  case POWER_5:
     model = "Power5";
     break;
  case POWER_6:
     model = "Power6";
     break;
  default:
     model = "unknown";
     break;
  }

  for(i=0; i<n; i++) {
    PdDv *pobj = pddv[i];

    if (strcmp(pobj->type, "proc_rspc") == 0) {
      prefix = pobj->prefix;
      break;
    }
  }

  if (prefix == 0)
    throw mdr_exception_t(
      "mdr_cpuinfo_t::mdr_refresh: unknown PdDv processor prefix");

  int length = strlen(prefix);

  odm_t<CuDv> cudv(CuDv_CLASS);
  n = cudv.size();

  for(i=0; i<n; i++) {
    CuDv *pobj = cudv[i];

    if (strncmp(pobj->name, prefix, length) == 0) {
      odm_t<CuAt> types(CuAt_CLASS, "attribute=type");
      int x = types.size();
      int j;
      CuAt *type_obj = 0;
      CuAt *freq_obj = 0;

      for(j=0; j<x; j++) {
        type_obj = types[j];
        if (strcmp(type_obj->name, pobj->name) == 0)
          break;
      }

      // architecture; just use the PowerPC part, the end is the model 
      arch = (type_obj == 0 ? "unknown" : type_obj->value);
      char *p = strchr(arch, '_');
      if (p != 0)
        *p = '\0';

      odm_t<CuAt> freq(CuAt_CLASS, "attribute=frequency");
      x = freq.size();

      for(j=0; j<x; j++) {
        freq_obj = freq[j];
        if (strcmp(freq_obj->name, type_obj->name) == 0)
          break;
      }

      clock_speed = (freq_obj == 0 ? -1 : atoi(freq_obj->value) / 1000000);

      // build a format string
      snprintf(format, sizeof format, "%s%%d", prefix);

      // scanf the instance number from the name
      sscanf(pobj->name, format, &instance);

      can.clear();
      can.mci_instance = instance;
      can.mci_architecture = arch;
      can.mci_model = model;
      can.mci_clock_MHz = clock_speed;

      snprintf(name, sizeof name, "cpu%d", instance);
      (*this)[name] = can;
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

void mdr_mp_t::mdr_refresh()
{
  if (net_client) {
    mp_remote_refresh();
    return;
  }

  // start fresh
  clear();

  perfstat_id_t first;
  perfstat_cpu_t *cpus;
  int ncpus = perfstat_cpu(0, 0, sizeof(perfstat_cpu_t), 0);
  if (ncpus <= 0)
    throw mdr_exception_t(
      "mdr_mp_t::mdr_refresh: perfstat_cpu fails");
  mmp_can_t can;
  mmp_node_t n;
  del_set_t del_set;
  del_node_t sn;

  cpus = new perfstat_cpu_t[ncpus];
  strcpy(first.name, FIRST_CPU);
  ncpus = perfstat_cpu(&first, cpus, sizeof(perfstat_cpu_t), ncpus);
  if (ncpus <= 0)
    throw mdr_exception_t(
      "mdr_mp_t::mdr_refresh: perfstat_cpu(first) fails");

  for(int i=0; i<ncpus; i++) {
    can.clear();
    can.mmp_cpu_instance = i;
    can.mmp_user_ticks = cpus[i].user % MAX_INTEGER;
    can.mmp_kernel_ticks = cpus[i].sys % MAX_INTEGER;
    can.mmp_idle_ticks = cpus[i].idle % MAX_INTEGER;
    can.mmp_wait_ticks = cpus[i].wait % MAX_INTEGER;

    (*this)[cpus[i].name] = can;
  }

  delete[] cpus;

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

static class ctlr_map_t : public std::map<std::string, std::string> {
public:
  ctlr_map_t() {
    int ndisks = perfstat_disk(0, 0, sizeof(perfstat_disk_t), 0);
    if (ndisks <= 0)
      throw mdr_exception_t(
        "ctlr_map_t::ctlr_map_t: perfstat_disk fails");
    perfstat_disk_t *disks = new perfstat_disk_t[ndisks];
    perfstat_id_t first;

    strcpy(first.name, FIRST_DISK);
    ndisks = perfstat_disk(&first, disks, sizeof(perfstat_disk_t), ndisks);
    if (ndisks <= 0)
      throw mdr_exception_t(
        "ctlr_map_t::ctlr_map_t: perfstat_disk(first) fails");

    for(int i=0; i<ndisks; i++)
      (*this)[disks[i].name] = disks[i].adapter;

    delete[] disks;
  }
} ctlr_map;

void mdr_ctlr_t::mdr_refresh()
{
  if (net_client) {
    ctlr_remote_refresh();
    return;
  }

  mctl_node_t node;
  del_set_t del_set;
  del_node_t sn;
  int c;
  int t;
  int d;
  std::string cname;
  mctl_can_t *cp;
  md_node_t dn;
  md_can_t *dp;

  // start fresh
  clear();

  // new disk stats
  disk->mdr_refresh();

  // traverse the disks and accumulate totals for controllers
  for(dn=disk->begin(); dn != disk->end(); dn++) {
    dp = &dn->second;
    if (dp->md_type != "physical")
      continue;

    // yield the controller #
    cname = ctlr_map[dn->first];

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
    cp->mctl_io_operations     += dp->md_io_operations;
    cp->mctl_disk_count++;
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

void mdr_disk_t::mdr_refresh()
{
  if (net_client) {
    disk_remote_refresh();
    return;
  }

  int ndisks = perfstat_disk(0, 0, sizeof(perfstat_disk_t), 0);
  if (ndisks <= 0)
    throw mdr_exception_t(
      "mdr_disk_t::mdr_refresh: perfstat_disk fails");
  perfstat_disk_t *disks = new perfstat_disk_t[ndisks];
  perfstat_id_t first;
  md_can_t can;
  md_node_t n;
  del_set_t del_set;
  del_node_t sn;
  odm_t<CuAt> cuat(CuAt_CLASS);
  int cuat_count = cuat.size();
  static int hertz = 0;

  // rserv: read or receive service time
  // No kidding. In what units? 2 nanosecond units? Right.
  // Get hertz so the "time" values can be scaled to ticks.

  if (hertz == 0)
    hertz = sysconf(_SC_CLK_TCK);

  strcpy(first.name, FIRST_DISK);
  ndisks = perfstat_disk(&first, disks, sizeof(perfstat_disk_t), ndisks);
  if (ndisks <= 0)
    throw mdr_exception_t(
      "mdr_disk_t::mdr_refresh: perfstat_disk(first) fails");

  // start over
  clear();

  for(int i=0; i<ndisks; i++) {
    can.clear();
    can.md_type             = "physical";
    can.md_read_K           =
      (uint_t) (((double) can.md_reads * (double) disks[i].bsize) / 1024.0);
    can.md_read_msecs       =
      (uint_t) ((double) disks[i].rserv * ((double) hertz / 500000000.0));
    can.md_written_K        =
      (uint_t) (((double) can.md_writes * (double) disks[i].bsize) / 1024.0);
    can.md_write_msecs      =
      (uint_t) ((double) disks[i].wserv * ((double) hertz / 500000000.0));
    can.md_run_msecs        = disks[i].time;
    can.md_wait_msecs       = disks[i].wq_time;
    can.md_runq_msecs       = can.md_run_msecs;
    can.md_waitq_msecs      = can.md_wait_msecs;
    can.md_avg_queue_length = disks[i].q_sampled;

    // broken: values are either zero or wrong
    // can.md_space_mb         = disks[i].size;

    for(int j=0; j<cuat_count; j++) {
      CuAt *cobj = cuat[j];

      if ((strcmp(cobj->name, disks[i].name) == 0) &&
          (strcmp(cobj->attribute, "size_in_mb") == 0)) {
        can.md_space_mb = atoi(cobj->value);
        break;
      }
    }
    can.md_io_operations    = disks[i].xfers;

    (*this)[disks[i].name] = can;
  }

  delete[] disks;

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

static inline void
get_object(vmount_t *mnt, int index, std::string &arg)
{
  char buf[BUFSIZ];
  int size;

  memset(buf, '\0', sizeof buf);
  size = vmt2datasize(mnt, index);
  if (size >= sizeof(buf) - 1)
    size = sizeof(buf) - 1;
  memcpy(buf, vmt2dataptr(mnt, index), size);
  arg = buf;
}

#define B2K(s,b) ((ulong_t) ((((double) (s) * (double) (b)) / 1024.0)))

void mdr_mnt_t::mdr_refresh()
{
  if (net_client) {
    mnt_remote_refresh();
    return;
  }

  int size = sizeof(vmount_t);
  char *buffer = new char[size];
  vmount_t *mnt;
  std::string special;
  std::string mnt_pt;
  std::string nfs_host;
  std::string opts;
  statvfs_t fsbuf;
  stat_t stbuf;
  mmnt_can_t can;
  bool ok;
  mmnt_node_t node;
  del_set_t del_set;
  del_node_t sn;
  std::string key;

  for(;;) {
    switch(mntctl(MCTL_QUERY, size, buffer)) {
    case 0:
      // not enough space
      size = *((int *) buffer) + sizeof(vmount_t);
      delete[] buffer;
      buffer = new char[size];
      memset(buffer, '\0', size);
      continue;
    case -1:
      // error
      throw mdr_exception_t("mdr_mnt_t::mdr_refresh: mntctl fails");
    default:
      // good to go
      break;
    }
    break;
  }

  // start over
  clear();

  // this is intuitive...
  for(mnt = (vmount_t *) buffer; mnt->vmt_length != 0;
      mnt = (vmount_t *) ((char *) mnt + mnt->vmt_length)) {

    can.clear();

    get_object(mnt, VMT_OBJECT, special);
    get_object(mnt, VMT_STUB, mnt_pt);
    get_object(mnt, VMT_HOST, nfs_host);
    get_object(mnt, VMT_ARGS, opts);

    can.mmnt_mount_point = mnt_pt;

    switch(mnt->vmt_gfstype) {
    case MNT_J2:
      can.mmnt_fs_type = "jfs2";
      break;
    case MNT_NAMEFS:
      can.mmnt_fs_type = "namefs";
      break;
    case MNT_NFS:
      can.mmnt_special = nfs_host + ":";
      can.mmnt_fs_type = "nfs";
      break;
    case MNT_JFS:
      can.mmnt_fs_type = "jfs";
      break;
    case MNT_CDROM:
      can.mmnt_fs_type = "iso9660";
      break;
    case MNT_PROCFS:
      can.mmnt_fs_type = "procfs";
      break;
    case MNT_SFS:
      can.mmnt_fs_type = "sfs";
      break;
    case MNT_CACHEFS:
      can.mmnt_fs_type = "cachefs";
      break;
    case MNT_NFS3:
      can.mmnt_special = nfs_host + ":";
      can.mmnt_fs_type = "nfs3";
      break;
    case MNT_AUTOFS:
      can.mmnt_fs_type = "autofs";
      break;
    case MNT_VXFS:
      can.mmnt_fs_type = "vxfs";
      break;
    case MNT_VXODM:
      can.mmnt_fs_type = "veritas";
      break;
    case MNT_UDF:
      can.mmnt_fs_type = "udfs";
      break;
    case MNT_NFS4:
    case MNT_RFS4:
      can.mmnt_special = nfs_host + ":";
      can.mmnt_fs_type = "nfs4";
      break;
    case MNT_CIFS:
      can.mmnt_fs_type = "samba";
      break;
    }

    can.mmnt_special += special;
    can.mmnt_mount_options = opts;
    can.mmnt_mount_time = mnt->vmt_time;
    can.mmnt_file_system_id = mnt->vmt_fsid.val[0];

#if CHECK_NFS_BEFORE_STAT
    std::map<std::string, bool> host_stat;
    std::map<std::string, bool>::iterator hn;

    if (can.mmnt_fs_type.substr(0, 3) == "nfs") try {
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
      if (statvfs(can.mmnt_mount_point.c_str(), &fsbuf) != -1) {
        can.mmnt_size_K = B2K(fsbuf.f_blocks, fsbuf.f_bsize);
        can.mmnt_free_K = B2K(fsbuf.f_bfree, fsbuf.f_bsize);
        can.mmnt_used_K = can.mmnt_size_K - can.mmnt_free_K;
        ulong_t reserved = can.mmnt_free_K - B2K(fsbuf.f_bavail, fsbuf.f_bsize);
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

  delete[] buffer;

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

void mdr_pf_t::mdr_refresh()
{
  if (net_client) {
    pf_remote_refresh();
    return;
  }

  odm_t<CuAt> cuat(CuAt_CLASS);
  int n = cuat.size();
  char dev[128];
  pginfo_t pfstats;
  mpf_can_t can;
  long pgk = sysconf(_SC_PAGE_SIZE) / 1024;
  mpf_node_t node;
  del_set_t del_set;
  del_node_t sn;

  for(int i=0; i<n; i++) {
    CuAt *cobj = cuat[i];

    if ((strcmp(cobj->attribute, "type") == 0) &&
        (strcmp(cobj->value, "paging") == 0)) {
      snprintf(dev, sizeof dev, "/dev/%s", cobj->name);
      if (swapqry(dev, &pfstats) == -1)
        continue;

      can.clear();

      can.mpf_size_K = pfstats.size * pgk;
      can.mpf_free_K = pfstats.free * pgk;
      can.mpf_used_K = can.mpf_size_K - can.mpf_free_K;

      (*this)[dev] = can;
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

void mdr_ip_t::mdr_refresh()
{
  if (net_client) {
    ip_remote_refresh();
    return;
  }

  if (constructor)
    constructor = false;
  else
    mib2_ip.ip_refresh();

  mip_ipForwarding = mib2_ip.ipForwarding;
  mip_ipDefaultTTL = mib2_ip.ipDefaultTTL;
  mip_ipInReceives = mib2_ip.ipInReceives;
  mip_ipInHdrErrors = mib2_ip.ipInHdrErrors;
  mip_ipInAddrErrors = mib2_ip.ipInAddrErrors;
  mip_ipForwDatagrams = mib2_ip.ipForwDatagrams;
  mip_ipInUnknownProtos = mib2_ip.ipInUnknownProtos;
  mip_ipInDiscards = mib2_ip.ipInDiscards;
  mip_ipInDelivers = mib2_ip.ipInDelivers;
  mip_ipOutRequests = mib2_ip.ipOutRequests;
  mip_ipOutDiscards = mib2_ip.ipOutDiscards;
  mip_ipOutNoRoutes = mib2_ip.ipOutNoRoutes;
  mip_ipReasmTimeout = mib2_ip.ipReasmTimeout;
  mip_ipReasmReqds = mib2_ip.ipReasmReqds;
  mip_ipReasmOKs = mib2_ip.ipReasmOKs;
  mip_ipReasmFails = mib2_ip.ipReasmFails;
  mip_ipFragOKs = mib2_ip.ipFragOKs;
  mip_ipFragFails = mib2_ip.ipFragFails;
  mip_ipFragCreates = mib2_ip.ipFragCreates;
  mip_ipRoutingDiscards = mib2_ip.ipRoutingDiscards;
}

void mdr_tcp_t::mdr_refresh()
{
  if (net_client) {
    tcp_remote_refresh();
    return;
  }

  if (constructor)
    constructor = false;
  else
    mib2_tcp.tcp_refresh();

  mtcp_tcpRtoAlgorithm = mib2_tcp.tcpRtoAlgorithm;
  mtcp_tcpRtoMin = mib2_tcp.tcpRtoMin;
  mtcp_tcpRtoMax = mib2_tcp.tcpRtoMax;
  mtcp_tcpMaxConn = mib2_tcp.tcpMaxConn;
  mtcp_tcpActiveOpens = mib2_tcp.tcpActiveOpens;
  mtcp_tcpPassiveOpens = mib2_tcp.tcpPassiveOpens;
  mtcp_tcpAttemptFails = mib2_tcp.tcpAttemptFails;
  mtcp_tcpEstabResets = mib2_tcp.tcpEstabResets;
  mtcp_tcpCurrEstab = mib2_tcp.tcpCurrEstab;
  mtcp_tcpInSegs = mib2_tcp.tcpInSegs;
  mtcp_tcpOutSegs = mib2_tcp.tcpOutSegs;
  mtcp_tcpRetransSegs = mib2_tcp.tcpRetransSegs;
  mtcp_tcpInErrs = mib2_tcp.tcpInErrs;
  mtcp_tcpOutRsts = mib2_tcp.tcpOutRsts;
}

void mdr_udp_t::mdr_refresh()
{
  if (net_client) {
    udp_remote_refresh();
    return;
  }

  if (constructor)
    constructor = false;
  else
    mib2_udp.udp_refresh();

  mudp_udpInDatagrams = mib2_udp.udpInDatagrams;
  mudp_udpNoPorts = mib2_udp.udpNoPorts;
  mudp_udpInErrors = mib2_udp.udpInErrors;
  mudp_udpOutDatagrams = mib2_udp.udpOutDatagrams;
}

void mdr_icmp_t::mdr_refresh()
{
  if (net_client) {
    icmp_remote_refresh();
    return;
  }

  if (constructor)
    constructor = false;
  else
    mib2_icmp.icmp_refresh();

  micmp_icmpInMsgs = mib2_icmp.icmpInMsgs;
  micmp_icmpInErrors = mib2_icmp.icmpInErrors;
  micmp_icmpInDestUnreachs = mib2_icmp.icmpInDestUnreachs;
  micmp_icmpInTimeExcds = mib2_icmp.icmpInTimeExcds;
  micmp_icmpInParmProbs = mib2_icmp.icmpInParmProbs;
  micmp_icmpInSrcQuenchs = mib2_icmp.icmpInSrcQuenchs;
  micmp_icmpInRedirects = mib2_icmp.icmpInRedirects;
  micmp_icmpInEchos = mib2_icmp.icmpInEchos;
  micmp_icmpInEchoReps = mib2_icmp.icmpInEchoReps;
  micmp_icmpInTimestamps = mib2_icmp.icmpInTimestamps;
  micmp_icmpInTimestampReps = mib2_icmp.icmpInTimestampReps;
  micmp_icmpInAddrMasks = mib2_icmp.icmpInAddrMasks;
  micmp_icmpInAddrMaskReps = mib2_icmp.icmpInAddrMaskReps;
  micmp_icmpOutMsgs = mib2_icmp.icmpOutMsgs;
  micmp_icmpOutErrors = mib2_icmp.icmpOutErrors;
  micmp_icmpOutDestUnreachs = mib2_icmp.icmpOutDestUnreachs;
  micmp_icmpOutTimeExcds = mib2_icmp.icmpOutTimeExcds;
  micmp_icmpOutParmProbs = mib2_icmp.icmpOutParmProbs;
  micmp_icmpOutSrcQuenchs = mib2_icmp.icmpOutSrcQuenchs;
  micmp_icmpOutRedirects = mib2_icmp.icmpOutRedirects;
  micmp_icmpOutEchos = mib2_icmp.icmpOutEchos;
  micmp_icmpOutEchoReps = mib2_icmp.icmpOutEchoReps;
  micmp_icmpOutTimestamps = mib2_icmp.icmpOutTimestamps;
  micmp_icmpOutTimestampReps = mib2_icmp.icmpOutTimestampReps;
  micmp_icmpOutAddrMasks = mib2_icmp.icmpOutAddrMasks;
  micmp_icmpOutAddrMaskReps = mib2_icmp.icmpOutAddrMaskReps;
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

void mdr_netif_t::mdr_refresh()
{
  if (net_client) {
    netif_remote_refresh();
    return;
  }

  mnet_node_t n;
  del_set_t del_set;
  del_node_t sn;

  int nifs;
  perfstat_netinterface_t *netifs;
  perfstat_id_t first;
  mnet_can_t can;

  if (constructor)
    constructor = false;
  else
    mib2_netif.netif_refresh();

  // check how many perfstat_netinterface_t structures are available
  nifs = perfstat_netinterface(0, 0, sizeof(perfstat_netinterface_t), 0);
   
  // allocate enough memory for all the structures
  netifs = new perfstat_netinterface_t[nifs];
   
  // set name to first interface
  strcpy(first.name, FIRST_NETINTERFACE);
   
  // ask to get all the structures available in one call
  // return code is number of structures returned
  nifs = perfstat_netinterface(
      &first, netifs, sizeof(perfstat_netinterface_t), nifs);

  // start fresh
  clear();

  for(int i=0; i<nifs; i++) {
    perfstat_netinterface_t *np = &netifs[i];
    char *name = np->name;
    mib::snmp_netif_can_t *mcp = 0;
    mib::snmp_netif_node_t mnode;

    for(mnode=mib2_netif.begin(); mnode != mib2_netif.end(); mnode++)
      if (mnode->first == name) {
        mcp = &mnode->second;
        break;
      }

    can.clear();
    get_address_info(name, &can);

    can.mnet_input_bytes     = np->ibytes;
    can.mnet_output_bytes    = np->obytes;
    can.mnet_input_packets   = np->ipackets;
    can.mnet_output_packets  = np->opackets;
    can.mnet_input_errors    = np->ierrors;
    can.mnet_output_errors   = np->oerrors;
    if (mcp != 0) {
      can.mnet_input_failures  = mcp->ifInDiscards +
                                 mcp->ifInUnknownProtos;
      can.mnet_output_failures = mcp->ifOutDiscards;
    }
    can.mnet_collisions      = np->collisions;
    can.mnet_interface_speed = np->bitrate;

    // ibm: gigabit is 1024^3
    switch(can.mnet_interface_speed) {
    case 10 * 1024 * 1024:
      can.mnet_interface_speed = 10000000;
      break;
    case 100 * 1024 * 1024:
      can.mnet_interface_speed = 100000000;
      break;
    case 1024 * 1024 * 1024:
      can.mnet_interface_speed = 1000000000;
      break;
    default:
      break;
    }

    (*this)[name] = can;
  }

  delete[] netifs;

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

// convert a timeval into a double as number of seconds
// this is for the conversion of rusage values which
// store nanoseconds in the tv_usec field...
static inline double TIMEVAL(struct timeval64 &t)
{
  return (double) t.tv_sec + ((double) t.tv_usec / 1000000000.0);
}

static void get_command(procentry64 *pp, std::string &cmd)
{
  char buf[BUFSIZ];

  if (getargs(pp, sizeof(*pp), buf, sizeof(buf)) != 0)
    return;

  char *p = buf;

  while(*p) {
    while(*p)
      p++;
    *p++ = ' ';
  }

  cmd = buf;
}

void mdr_ps_t::mdr_refresh()
{
  del_set_t del_set;
  del_node_t sn;
  mps_node_t node;
  del_set_t current_set;

  if (net_client) {
    ps_remote_refresh();
  } else {
    static long pgsz = sysconf(_SC_PAGESIZE);
    static long pgk = pgsz / 1024;
    char path[BUFSIZ];
    char name[128];
    const char *cmd;
    char *fq_cmd;
    int i;
    int n;
    int nprocs;
    int sz = sizeof(procentry64);
    procentry64 *pp;
    pid_t pid = 0;
    mps_can_t can;
    thrdentry64 *tarray = 0;
    int tarray_max = 0;
    tid64_t tid = 0;
    int nthreads;
    bool runnable = false;

    mps_procs_runnable = 0;
    mps_procs_sleeping = 0;
    mps_procs_stopped = 0;
    mps_procs_blocked = 0;
    mps_procs_zombie = 0;
    mps_procs_specific = 0;

    if (mps_array == 0) {
      n = getprocs64(0, sz, 0, 0, &pid, 1 << 16);

      mps_array_max = (n == -1) ? 256 : (n + 2);  // and a pinch to grow on
      mps_array = new procentry64[mps_array_max];
      pid = 0;
    }

    while((nprocs = getprocs64(
      mps_array, sz, 0, 0, &pid, mps_array_max)) == mps_array_max) {
        delete[] mps_array;
        mps_array_max *= 2;
        mps_array = new procentry64[mps_array_max];
        pid = 0;
    }

    for(i=0; i<nprocs; i++) {

      pp = &mps_array[i];

      // this is the key: process_name[process_id]
      if (pp->pi_pid == 0)
        strcpy(pp->pi_comm, "swapper");
      snprintf(name, sizeof name, "%s[%d]", pp->pi_comm, pp->pi_pid);

      node = find(name);
      if (node == end()) {

        can.clear();

        can.mps_start = (long) (pp->pi_start & 0xffffffff);
        can.mps_pid = pp->pi_pid;
        can.mps_ppid = pp->pi_ppid;
        can.mps_tty_device = makedev(major64(pp->pi_ttyd),minor64(pp->pi_ttyd));
        can.mps_file_name = pp->pi_comm;
        get_command(pp, can.mps_command);
        if (can.mps_command.length() == 0)
          can.mps_command = pp->pi_comm;

        // zombies have no name...
        if (can.mps_command.length() == 0 && pp->pi_state == SZOMB) {
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

      cp->mps_uid = pp->pi_uid;
      cp->mps_euid = pp->pi_suid;
      cp->mps_user_time = TIMEVAL(pp->pi_ru.ru_utime);
      cp->mps_kernel_time = TIMEVAL(pp->pi_ru.ru_stime);
      cp->mps_cpu_time = cp->mps_user_time + cp->mps_kernel_time;
      cp->mps_size_K = (ulong_t) (pp->pi_size * pgk);
      cp->mps_rssize_K = (ulong_t) (pp->pi_trss + pp->pi_drss) * pgk;
      cp->mps_io_K = (ulong_t) (pp->pi_ioch / 1024);
      cp->mps_memory_percent =
        (cp->mps_rssize_K * 100.0) / (double) mps_total_mem_K;
      cp->mps_voluntary_csw = pp->pi_ru.ru_nvcsw;
      cp->mps_involuntary_csw = pp->pi_ru.ru_nivcsw;
      cp->mps_disk_faults = pp->pi_majflt;
      cp->mps_page_reclaims = pp->pi_minflt;

      // AIX
      //
      // Us          Them
      // X Specific  O Nonexistent
      // R Runnable  A Active
      // B Blocked   W Swapped
      // S Sleeping  I Idle
      // Z Zombie    Z Cancelled
      // T Stopped   T Stopped
      //

      switch(pp->pi_state) {
      case SACTIVE:
        tid = 0;
        sz = sizeof(thrdentry64);
        n = getthrds64(cp->mps_pid, 0, sz, &tid, 1 << 16);
        n = (n == -1) ? 2 : n + 1;
        if (n > tarray_max) {
          if (tarray)
            delete[] tarray;
          tarray = new thrdentry64[n];
          tarray_max = n;
        }
        tid = 0;
        while((nthreads = getthrds64(cp->mps_pid, tarray,
                            sz, &tid, tarray_max)) == tarray_max) {
          delete[] tarray;
          tarray_max *= 2;
          tarray = new thrdentry64[tarray_max];
          tid = 0;
        }
        runnable = false;
        for(int j=0; j<nthreads; j++)
          if (tarray[j].ti_state == TSRUN) {
            runnable = true;
            break;
          }
        if (runnable) {
          cp->mps_state = MPS_RUNNABLE;
          mps_procs_runnable++;
        } else {
          cp->mps_state = MPS_SLEEPING;
          mps_procs_sleeping++;
        }
        break;
      case SIDL:
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
      case SSWAP:
        cp->mps_state = MPS_BLOCKED;
        mps_procs_blocked++;
        break;
      default: // SNONE
        cp->mps_state = MPS_SPECIFIC;
        mps_procs_specific++;
        break;
      }

      cp->mps_priority = pp->pi_pri;
      cp->mps_nice = pp->pi_nice;

      cp->mps_user_name = uid_lookup(cp->mps_uid);
      cp->mps_effective_user_name = uid_lookup(cp->mps_euid);
      cp->mps_thread_count = pp->pi_thcount;
      cp->mps_is_system_process = ((pp->pi_flags & SKPROC) != 0);

      current_set.insert(name);
    }

    if (tarray)
      delete[] tarray;
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
  static bool initialized = 0;
  static struct tty_devs tty_map[] = {
    { "/dev/console",  "console",  0,  0           },
    { "/dev/pts/0",    "pts/%d",   0,  pts_compose },
    { "/dev/ttyp0",    "tty%c%d",  0,  pty_compose },
    { "/dev/lft0",     "lft%d",    0,  pts_compose },
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

typedef std::map<pid_t, char *> p2name_map_t;
typedef p2name_map_t::iterator  p2name_node_t;

void mdr_thread_t::mdr_refresh()
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
    int i;
    int nthreads;
    int nprocs;
    int sz = sizeof(thrdentry64);
    thrdentry64 *tp;
    tid64_t tid = 0;
    pid_t pid = 0;
    mt_can_t can;
    procentry64 proc;
    p2name_map_t p2name_map;

    if (mt_array == 0) {
      int n = getthrds64(-1, 0, sz, &tid, 1 << 16);

      mt_array_max = (n == -1) ? 256 : (n + 2);  // and a pinch to grow on
      mt_array = new thrdentry64[mt_array_max];
      tid = 0;
    }

    while((nthreads = getthrds64(
      -1, mt_array, sz, &tid, mt_array_max)) == mt_array_max) {
        delete[] mt_array;
        mt_array_max *= 2;
        mt_array = new thrdentry64[mt_array_max];
        tid = 0;
    }

    if (mps_array == 0) {
      sz = sizeof(procentry64);

      int n = getprocs64(0, sz, 0, 0, &pid, 1 << 16);

      mps_array_max = (n == -1) ? 256 : (n + 2);  // and a pinch to grow on
      mps_array = new procentry64[mps_array_max];
      pid = 0;
    }

    while((nprocs = getprocs64(
      mps_array, sz, 0, 0, &pid, mps_array_max)) == mps_array_max) {
        delete[] mps_array;
        mps_array_max *= 2;
        mps_array = new procentry64[mps_array_max];
        pid = 0;
    }

    for(i=0; i<nprocs; i++) {
      char *p = mps_array[i].pi_comm;

      if (mps_array[i].pi_pid == 0)
        p = "swapper";
      p2name_map[mps_array[i].pi_pid] = p;
    }

    for(i=0; i<nthreads; i++) {

      tp = &mt_array[i];

      char *pname = p2name_map[tp->ti_pid];

      // this is the key: process_name[process_id][thread_id]
      snprintf(name, sizeof name, "%s[%d][%lld]",
        pname, tp->ti_pid, tp->ti_tid);

      node = find(name);
      if (node == end()) {

        can.clear();
        can.mt_thread_id = tp->ti_tid;
        can.mt_process_id = tp->ti_pid;

        (*this)[name] = can;
        node = find(name);
      }

      mt_can_t *cp = &node->second;

      cp->mt_user_time = TIMEVAL(tp->ti_ru.ru_utime);
      cp->mt_kernel_time = TIMEVAL(tp->ti_ru.ru_stime);
      cp->mt_cpu_time = cp->mt_user_time + cp->mt_kernel_time;
//      cp->mt_size_K   = not supported
//      cp->mt_rssize_K = not supported
//      cp->mt_io_K     = not supported
      cp->mt_memory_percent =
        (cp->mt_rssize_K * 100.0) / (double) mt_total_mem_K;
      cp->mt_voluntary_csw = tp->ti_ru.ru_nvcsw;
      cp->mt_involuntary_csw = tp->ti_ru.ru_nivcsw;
      cp->mt_disk_faults = tp->ti_ru.ru_majflt;
      cp->mt_page_reclaims = tp->ti_ru.ru_minflt;

      // AIX
      //
      // Us          Them
      // X Specific  O Nonexistent
      // R Runnable  A Active
      // B Blocked   W Swapped
      // S Sleeping  I Idle
      // Z Zombie    Z Cancelled
      // T Stopped   T Stopped
      //

      switch(tp->ti_state) {
      case SACTIVE:
        cp->mt_state = MPS_RUNNABLE;
        break;
      case SIDL:
        cp->mt_state = MPS_SLEEPING;
        break;
      case SSTOP:
        cp->mt_state = MPS_STOPPED;
        break;
      case SZOMB:
        cp->mt_state = MPS_ZOMBIE;
        break;
      case SSWAP:
        cp->mt_state = MPS_BLOCKED;
        break;
      default: // SNONE
        cp->mt_state = MPS_SPECIFIC;
        break;
      }

      cp->mt_priority = tp->ti_pri;
      cp->mt_nice = tp->ti_nice;

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

}
