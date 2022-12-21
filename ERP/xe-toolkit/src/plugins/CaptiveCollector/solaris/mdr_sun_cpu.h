/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Solaris plugin library for CaptiveCollector.
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


#ifndef _MDR_SUN_CPU_H_
#define _MDR_SUN_CPU_H_

// This code lived in the mdr library once upon a time.
// It did not belong there and it was moved here.
// It belongs here.
//
// The code was left looking like it did because it's just
// too much work to rewrite it so that it will do exactly
// what it already does. It just depends on mdr now.

#include <kstat.h>
#include <mdr.h>

namespace mdr {

const char *const MDR_SUN_CPU_NAME  = "SunOS Per-CPU Data";

const int MSC_VERSION = 1;

const int MSC_CPU_INSTANCE   = 0;
const int MSC_BREAD          = 1;
const int MSC_BWRITE         = 2;
const int MSC_LREAD          = 3;
const int MSC_LWRITE         = 4;
const int MSC_PHREAD         = 5;
const int MSC_PHWRITE        = 6;
const int MSC_PSWITCH        = 7;
const int MSC_TRAP           = 8;
const int MSC_INTR           = 9;
const int MSC_SYSCALL        = 10;
const int MSC_SYSREAD        = 11;
const int MSC_SYSWRITE       = 12;
const int MSC_SYSFORK        = 13;
const int MSC_SYSVFORK       = 14;
const int MSC_SYSEXEC        = 15;
const int MSC_READCH         = 16;
const int MSC_WRITECH        = 17;
const int MSC_RAWCH          = 18;
const int MSC_CANCH          = 19;
const int MSC_OUTCH          = 20;
const int MSC_MSG            = 21;
const int MSC_SEMA           = 22;
const int MSC_NAMEI          = 23;
const int MSC_UFSIGET        = 24;
const int MSC_UFSDIRBLK      = 25;
const int MSC_UFSIPAGE       = 26;
const int MSC_UFSINOPAGE     = 27;
const int MSC_INODEOVF       = 28;
const int MSC_FILEOVF        = 29;
const int MSC_PROCOVF        = 30;
const int MSC_INTRTHREAD     = 31;
const int MSC_INTRBLK        = 32;
const int MSC_IDLETHREAD     = 33;
const int MSC_INV_SWTCH      = 34;
const int MSC_NTHREADS       = 35;
const int MSC_CPUMIGRATE     = 36;
const int MSC_XCALLS         = 37;
const int MSC_MUTEX_ADENTERS = 38;
const int MSC_RW_RDFAILS     = 39;
const int MSC_RW_WRFAILS     = 40;
const int MSC_MODLOAD        = 41;
const int MSC_MODUNLOAD      = 42;
const int MSC_BAWRITE        = 43;
const int MSC_IOWAIT         = 44;
const int MSC_PGREC          = 45;
const int MSC_PGFREC         = 46;
const int MSC_PGIN           = 47;
const int MSC_PGPGIN         = 48;
const int MSC_PGOUT          = 49;
const int MSC_PGPGOUT        = 50;
const int MSC_SWAPIN         = 51;
const int MSC_PGSWAPIN       = 52;
const int MSC_SWAPOUT        = 53;
const int MSC_PGSWAPOUT      = 54;
const int MSC_ZFOD           = 55;
const int MSC_DFREE          = 56;
const int MSC_SCAN           = 57;
const int MSC_REV            = 58;
const int MSC_HAT_FAULT      = 59;
const int MSC_AS_FAULT       = 60;
const int MSC_MAJ_FAULT      = 61;
const int MSC_COW_FAULT      = 62;
const int MSC_PROT_FAULT     = 63;
const int MSC_SOFTLOCK       = 64;
const int MSC_KERNEL_ASFLT   = 65;
const int MSC_PGRRUN         = 66;
const int MSC_EXECPGIN       = 67;
const int MSC_EXECPGOUT      = 68;
const int MSC_EXECFREE       = 69;
const int MSC_ANONPGIN       = 70;
const int MSC_ANONPGOUT      = 71;
const int MSC_ANONFREE       = 72;
const int MSC_FSPGIN         = 73;
const int MSC_FSPGOUT        = 74;
const int MSC_FSFREE         = 75;
const int MSC_FIELD_COUNT    = 76;

class msc_can_t {
public:
  int    msc_cpu_instance;   // CPU instance number
  uint_t msc_bread;          // physical block reads
  uint_t msc_bwrite;         // physical block mrites (sync+async)
  uint_t msc_lread;          // logical block reads
  uint_t msc_lwrite;         // logical block mrites
  uint_t msc_phread;         // raw I/O reads
  uint_t msc_phwrite;        // raw I/O mrites
  uint_t msc_pswitch;        // context switches
  uint_t msc_trap;           // traps
  uint_t msc_intr;           // device interrupts
  uint_t msc_syscall;        // system calls
  uint_t msc_sysread;        // read() + readv() system calls
  uint_t msc_syswrite;       // mrite() + mritev() system calls
  uint_t msc_sysfork;        // forks
  uint_t msc_sysvfork;       // vforks
  uint_t msc_sysexec;        // execs
  uint_t msc_readch;         // bytes read by rdwr()
  uint_t msc_writech;        // bytes mritten by rdwr()
  uint_t msc_rawch;          // terminal input characters
  uint_t msc_canch;          // chars handled in canonical mode
  uint_t msc_outch;          // terminal output characters
  uint_t msc_msg;            // msg count (msgrcv()+msgsnd() calls)
  uint_t msc_sema;           // semaphore ops count (semop() calls)
  uint_t msc_namei;          // pathname lookups
  uint_t msc_ufsiget;        // ufs_iget() calls
  uint_t msc_ufsdirblk;      // directory blocks read
  uint_t msc_ufsipage;       // inodes taken mith attached pages
  uint_t msc_ufsinopage;     // inodes taked mith no attached pages
  uint_t msc_inodeovf;       // inode table overflows
  uint_t msc_fileovf;        // file table overflows
  uint_t msc_procovf;        // proc table overflows
  uint_t msc_intrthread;     // interrupts as threads (below clock)
  uint_t msc_intrblk;        // intrs blkd/prempted/released (swtch)
  uint_t msc_idlethread;     // times idle thread scheduled
  uint_t msc_inv_swtch;      // involuntary context switches
  uint_t msc_nthreads;       // thread_create()s
  uint_t msc_cpumigrate;     // cpu migrations by threads
  uint_t msc_xcalls;         // xcalls to other cpus
  uint_t msc_mutex_adenters; // failed mutex enters (adaptive)
  uint_t msc_rw_rdfails;     // rw reader failures
  uint_t msc_rw_wrfails;     // rw mriter failures
  uint_t msc_modload;        // times loadable module loaded
  uint_t msc_modunload;      // times loadable module unloaded
  uint_t msc_bawrite;        // physical block mrites (async)
  uint_t msc_iowait;         // procs waiting for block I/O
  uint_t msc_pgrec;          // page reclaims (includes pageout)
  uint_t msc_pgfrec;         // page reclaims from free list
  uint_t msc_pgin;           // pageins
  uint_t msc_pgpgin;         // pages paged in
  uint_t msc_pgout;          // pageouts
  uint_t msc_pgpgout;        // pages paged out
  uint_t msc_swapin;         // swapins
  uint_t msc_pgswapin;       // pages swapped in
  uint_t msc_swapout;        // swapouts
  uint_t msc_pgswapout;      // pages swapped out
  uint_t msc_zfod;           // pages zero filled on demand
  uint_t msc_dfree;          // pages freed by daemon or auto
  uint_t msc_scan;           // pages examined by pageout daemon
  uint_t msc_rev;            // revolutions of the page daemon hand
  uint_t msc_hat_fault;      // minor page faults via hat_fault()
  uint_t msc_as_fault;       // minor page faults via as_fault()
  uint_t msc_maj_fault;      // major page faults
  uint_t msc_cow_fault;      // copy-on-mrite faults
  uint_t msc_prot_fault;     // protection faults
  uint_t msc_softlock;       // faults due to software locking req
  uint_t msc_kernel_asflt;   // as_fault()s in kernel addr space
  uint_t msc_pgrrun;         // times pager scheduled
  uint_t msc_execpgin;       // executable pages paged in
  uint_t msc_execpgout;      // executable pages paged out
  uint_t msc_execfree;       // executable pages freed
  uint_t msc_anonpgin;       // anon pages paged in
  uint_t msc_anonpgout;      // anon pages paged out
  uint_t msc_anonfree;       // anon pages freed
  uint_t msc_fspgin;         // fs pages paged in
  uint_t msc_fspgout;        // fs pages paged out
  uint_t msc_fsfree;         // fs pages free

  msc_can_t()
  {
    clear();
  }

  void clear(void)
  {
    msc_cpu_instance = 0;
    msc_bread = 0;
    msc_bwrite = 0;
    msc_lread = 0;
    msc_lwrite = 0;
    msc_phread = 0;
    msc_phwrite = 0;
    msc_pswitch = 0;
    msc_trap = 0;
    msc_intr = 0;
    msc_syscall = 0;
    msc_sysread = 0;
    msc_syswrite = 0;
    msc_sysfork = 0;
    msc_sysvfork = 0;
    msc_sysexec = 0;
    msc_readch = 0;
    msc_writech = 0;
    msc_rawch = 0;
    msc_canch = 0;
    msc_outch = 0;
    msc_msg = 0;
    msc_sema = 0;
    msc_namei = 0;
    msc_ufsiget = 0;
    msc_ufsdirblk = 0;
    msc_ufsipage = 0;
    msc_ufsinopage = 0;
    msc_inodeovf = 0;
    msc_fileovf = 0;
    msc_procovf = 0;
    msc_intrthread = 0;
    msc_intrblk = 0;
    msc_idlethread = 0;
    msc_inv_swtch = 0;
    msc_nthreads = 0;
    msc_cpumigrate = 0;
    msc_xcalls = 0;
    msc_mutex_adenters = 0;
    msc_rw_rdfails = 0;
    msc_rw_wrfails = 0;
    msc_modload = 0;
    msc_modunload = 0;
    msc_bawrite = 0;
    msc_iowait = 0;
    msc_pgrec = 0;
    msc_pgfrec = 0;
    msc_pgin = 0;
    msc_pgpgin = 0;
    msc_pgout = 0;
    msc_pgpgout = 0;
    msc_swapin = 0;
    msc_pgswapin = 0;
    msc_swapout = 0;
    msc_pgswapout = 0;
    msc_zfod = 0;
    msc_dfree = 0;
    msc_scan = 0;
    msc_rev = 0;
    msc_hat_fault = 0;
    msc_as_fault = 0;
    msc_maj_fault = 0;
    msc_cow_fault = 0;
    msc_prot_fault = 0;
    msc_softlock = 0;
    msc_kernel_asflt = 0;
    msc_pgrrun = 0;
    msc_execpgin = 0;
    msc_execpgout = 0;
    msc_execfree = 0;
    msc_anonpgin = 0;
    msc_anonpgout = 0;
    msc_anonfree = 0;
    msc_fspgin = 0;
    msc_fspgout = 0;
    msc_fsfree = 0;
  }
};

typedef std::map<std::string, msc_can_t> msc_box_t;
typedef msc_box_t::iterator msc_node_t;

class mdr_sun_cpu_t : public mdr_t, public msc_box_t {
public:
  // constructors/destructors
  mdr_sun_cpu_t(const char *remote_host = 0);
 ~mdr_sun_cpu_t();

  // updating
  void mdr_refresh(void);
  void mdr_rates(void);

  // other
  const mdr_value_t &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value);
  const mdr_value_t &mdr_field_value(int field_number, mdr_value_t &value)
  {
    return mdr_t::mdr_field_value(field_number, value);
  }
  const mdr_keyset_t &mdr_keyset(mdr_keyset_t &keyset);

private:
  kstat_ctl_t *kc;     // no longer using the platform/solaris.cpp stuff
  msc_box_t    old;

  void check_kstat(void);
  void sun_cpu_remote_refresh(void);
};

} // end of namespace mdr

#endif
