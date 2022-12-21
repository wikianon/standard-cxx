/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the mdr tools of Captivity.
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


#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <mdr_misc.h>
#include <mdr_cpuinfo.h>
#include <mdr_pf.h>
#include <mdr_netif.h>
#include <mdr_mnt.h>
#include <mdr_disk.h>

using namespace mdr;

#define A_MB (1024.0 * 1024.0)
#define A_GB (1024.0 * A_MB)
#define A_TB (1024.0 * A_GB)

static inline void size_it(double &d, const char *&sz)
{
  if (d >= A_TB) {
    d /= A_TB;
    sz = "TB";
  } else if (d >= A_GB) {
    d /= A_GB;
    sz = "GB";
  } else {
    d /= A_MB;
    sz = "MB";
  }
}

int main(int argc, char *argv[])
{
  try {
    int c;
    int i;
    int n;
    int clock_MHz = 0;
    bool multi_speed = false;
    bool do_support_matrix = false;
    const char *sz;
    const char *ssz;
    const char *usz;
    const char *fsz;
    const char *tsz;
    const char *format;
    const char *remote_host = 0;
    double physbytes;
    double size;
    double used;
    double freed;

    opterr = 0;
    while((c = getopt(argc, argv, "sh:")) != -1)
      switch(c) {
      case 's':
        do_support_matrix = true;
        break;
      case 'h':
        remote_host = optarg;
        break;
      default:
        std::cerr << "Use: " << argv[0] << " [-s] [-h remote_host]"
                  << std::endl;
        return 1;
      }

    mdr_misc_t misc(remote_host);
    mdr_cpuinfo_t cpu(remote_host);
    mdr_pf_t pf(remote_host);
    mdr_netif_t net(remote_host);
    mdr_mnt_t mnt(remote_host);
    mdr_disk_t disk(remote_host);
    long pagesize = misc.mm_native_page_size;

    if (do_support_matrix) {
      mdr_t::mdr_support_matrix(misc, std::cout);
      std::cout << std::endl;
      mdr_t::mdr_support_matrix(pf, std::cout);
      std::cout << std::endl;
      mdr_t::mdr_support_matrix(net, std::cout);
      std::cout << std::endl;
      mdr_t::mdr_support_matrix(mnt, std::cout);
      return 0;
    }

    printf("Hostname    : %s\n", misc.mm_hostname.c_str());

    mci_node_t cin;

    mci_can_t *cp;

    for(cin=cpu.begin(); cin != cpu.end(); cin++) {
      cp = &cin->second;
      if (clock_MHz == 0)
        clock_MHz = cp->mci_clock_MHz;
      else if (cp->mci_clock_MHz != clock_MHz)
        multi_speed = true;
    }

    // cpus, count and clock
    printf("CPU         : %d %s %s at %d MHz",
      (int) cpu.size(), cp->mci_architecture.c_str(),
      cp->mci_model.c_str(), clock_MHz);

    if (multi_speed)
      puts(" (non uniform)");
    else
      putchar('\n');

    // physical memory
    physbytes = (double) misc.mm_physical_pages * (double) pagesize;
    if (physbytes > A_GB)
      format = "Memory      : %.2f %s\n";
    else
      format = "Memory      : %.0f %s\n";
    size_it(physbytes, sz);
    printf(format, physbytes, sz);

    // paging file
    size = used = freed = 0.0;

    mpf_node_t pfn;

    for(pfn=pf.begin(); pfn != pf.end(); pfn++) {
      const mpf_can_t pf_can = pfn->second;

      size  += ((double) pf_can.mpf_size_K * 1024.0);
      used  += ((double) pf_can.mpf_used_K * 1024.0);
      freed += ((double) pf_can.mpf_free_K * 1024.0);
    }
    double pf_size = size;
    double pf_used = used;
    double pf_free = freed;
    size_it(size,  ssz);
    size_it(used,  usz);
    size_it(freed, fsz);
    printf(
      "Page File   : %d area%s, %.2f %s total, %.2f %s used, %.2f %s free\n",
        (int) pf.size(), (pf.size() == 1 ? "" : "s"),
          size, ssz, used, usz, freed, fsz);

    // os release
    printf("OS          : %s\n", misc.mm_os_platform.c_str());
    printf("Version     : %s\n", misc.mm_os_version.c_str());
    printf("Architecture: %s\n", misc.mm_processor_arch.c_str());

    // network interfaces
    printf("Network     :");

    mnet_node_t nn;
    for(i=0, n=net.size()-1, nn=net.begin(); nn != net.end(); i++, nn++) {
      const std::string &name = nn->first;

      printf(" %s", name.c_str());
      if (i < n)
        putchar(',');
    }
    putchar('\n');

    // disk space
    size = used = freed = 0.0;

    md_node_t dn;
    md_can_t *dcp;
    const char *dname;
    double total = 0.0;

    for(dn=disk.begin(); dn != disk.end(); dn++) {
      dname = dn->first.c_str();
      dcp = &dn->second;
      if (dcp->md_type != "physical")
        continue;
      total += dcp->md_space_mb;
    }

    total *= (1024.0 * 1024.0);
    size_it(total, tsz);

    mmnt_node_t mntn;

    for(mntn=mnt.begin(); mntn != mnt.end(); mntn++) {
      const mmnt_can_t &mnt_can = mntn->second;

      if ((mnt_can.mmnt_fs_type != "ufs")  &&
          (mnt_can.mmnt_fs_type != "ntfs") &&
          (mnt_can.mmnt_fs_type != "ext2") &&
          (mnt_can.mmnt_fs_type != "ext3") &&
          (mnt_can.mmnt_fs_type != "reiserfs") &&
          (mnt_can.mmnt_fs_type != "hfs") &&
          (mnt_can.mmnt_fs_type != "jfs2"))
        continue;
      size  += mnt_can.mmnt_size_K;
      used  += mnt_can.mmnt_used_K;
      freed += mnt_can.mmnt_free_K;
    }
    size  *= 1024.0;
    used  *= 1024.0;
    freed *= 1024.0;
    size  += pf_size;
    used  += pf_used;
    freed += pf_free;
    size_it(size,  ssz);
    size_it(used,  usz);
    size_it(freed, fsz);
    printf("Disk        : %.2f %s total\n", total, tsz);
    printf("FS & Swap   : %.2f %s total, %.2f %s used, %.2f %s free\n",
      size, ssz, used, usz, freed, fsz);

    return 0;
  } catch(std::exception &e) {
    std::cout << argv[0] << ": " << e.what() << std::endl;
  }
}
