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


#include <set>
#include <sys/sysinfo.h>
#include "mdr_sun_cpu.h"

namespace mdr {

void mdr_sun_cpu_t::check_kstat(void)
{
  int i;

  for(; (i = kstat_chain_update(kc)) != 0; ) {
    switch (i) {
    case -1:
      throw mdr_exception_t("init_kstat: kstat_chain_update fails");
      break;
    default:
#if I_WANT_TO_SEE_STATE_CHANGES
      puts("<<<<<< STATE CHANGE >>>>>>");
#endif
      break;
    }
  }
}

mdr_sun_cpu_t::mdr_sun_cpu_t(const char *remote_host) :
  mdr_t(MDR_SUN_CPU_NAME, true, MSC_FIELD_COUNT, false)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_sun_cpu_t", remote_host, MSC_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
#ifndef _SOLARIS
    mdr_category_supported = false;
#else
    mdr_support_field(MSC_CPU_INSTANCE);
    mdr_support_field(MSC_BREAD);
    mdr_support_field(MSC_BWRITE);
    mdr_support_field(MSC_LREAD);
    mdr_support_field(MSC_LWRITE);
    mdr_support_field(MSC_PHREAD);
    mdr_support_field(MSC_PHWRITE);
    mdr_support_field(MSC_PSWITCH);
    mdr_support_field(MSC_TRAP);
    mdr_support_field(MSC_INTR);
    mdr_support_field(MSC_SYSCALL);
    mdr_support_field(MSC_SYSREAD);
    mdr_support_field(MSC_SYSWRITE);
    mdr_support_field(MSC_SYSFORK);
    mdr_support_field(MSC_SYSVFORK);
    mdr_support_field(MSC_SYSEXEC);
    mdr_support_field(MSC_READCH);
    mdr_support_field(MSC_WRITECH);
    mdr_support_field(MSC_RAWCH);
    mdr_support_field(MSC_CANCH);
    mdr_support_field(MSC_OUTCH);
    mdr_support_field(MSC_MSG);
    mdr_support_field(MSC_SEMA);
    mdr_support_field(MSC_NAMEI);
    mdr_support_field(MSC_UFSIGET);
    mdr_support_field(MSC_UFSDIRBLK);
    mdr_support_field(MSC_UFSIPAGE);
    mdr_support_field(MSC_UFSINOPAGE);
    mdr_support_field(MSC_INODEOVF);
    mdr_support_field(MSC_FILEOVF);
    mdr_support_field(MSC_PROCOVF);
    mdr_support_field(MSC_INTRTHREAD);
    mdr_support_field(MSC_INTRBLK);
    mdr_support_field(MSC_IDLETHREAD);
    mdr_support_field(MSC_INV_SWTCH);
    mdr_support_field(MSC_NTHREADS);
    mdr_support_field(MSC_CPUMIGRATE);
    mdr_support_field(MSC_XCALLS);
    mdr_support_field(MSC_MUTEX_ADENTERS);
    mdr_support_field(MSC_RW_RDFAILS);
    mdr_support_field(MSC_RW_WRFAILS);
    mdr_support_field(MSC_MODLOAD);
    mdr_support_field(MSC_MODUNLOAD);
    mdr_support_field(MSC_BAWRITE);
    mdr_support_field(MSC_IOWAIT);
    mdr_support_field(MSC_PGREC);
    mdr_support_field(MSC_PGFREC);
    mdr_support_field(MSC_PGIN);
    mdr_support_field(MSC_PGPGIN);
    mdr_support_field(MSC_PGOUT);
    mdr_support_field(MSC_PGPGOUT);
    mdr_support_field(MSC_SWAPIN);
    mdr_support_field(MSC_PGSWAPIN);
    mdr_support_field(MSC_SWAPOUT);
    mdr_support_field(MSC_PGSWAPOUT);
    mdr_support_field(MSC_ZFOD);
    mdr_support_field(MSC_DFREE);
    mdr_support_field(MSC_SCAN);
    mdr_support_field(MSC_REV);
    mdr_support_field(MSC_HAT_FAULT);
    mdr_support_field(MSC_AS_FAULT);
    mdr_support_field(MSC_MAJ_FAULT);
    mdr_support_field(MSC_COW_FAULT);
    mdr_support_field(MSC_PROT_FAULT);
    mdr_support_field(MSC_SOFTLOCK);
    mdr_support_field(MSC_KERNEL_ASFLT);
    mdr_support_field(MSC_PGRRUN);
    mdr_support_field(MSC_EXECPGIN);
    mdr_support_field(MSC_EXECPGOUT);
    mdr_support_field(MSC_EXECFREE);
    mdr_support_field(MSC_ANONPGIN);
    mdr_support_field(MSC_ANONPGOUT);
    mdr_support_field(MSC_ANONFREE);
    mdr_support_field(MSC_FSPGIN);
    mdr_support_field(MSC_FSPGOUT);
    mdr_support_field(MSC_FSFREE);
#endif

    net_client = 0;

    kc = kstat_open();
    if (kc == 0)
      throw mdr_exception_t("mdr_sun_cpu_t::mdr_sun_cpu_t: kstat_open returns nil");
  }

  mdr_field_names[MSC_CPU_INSTANCE] = "CPU instance number";
  mdr_field_names[MSC_BREAD] = "physical block reads";
  mdr_field_names[MSC_BWRITE] = "physical block mrites (sync+async)";
  mdr_field_names[MSC_LREAD] = "logical block reads";
  mdr_field_names[MSC_LWRITE] = "logical block mrites";
  mdr_field_names[MSC_PHREAD] = "raw I/O reads";
  mdr_field_names[MSC_PHWRITE] = "raw I/O mrites";
  mdr_field_names[MSC_PSWITCH] = "context switches";
  mdr_field_names[MSC_TRAP] = "traps";
  mdr_field_names[MSC_INTR] = "device interrupts";
  mdr_field_names[MSC_SYSCALL] = "system calls";
  mdr_field_names[MSC_SYSREAD] = "read() + readv() system calls";
  mdr_field_names[MSC_SYSWRITE] = "mrite() + mritev() system calls";
  mdr_field_names[MSC_SYSFORK] = "forks";
  mdr_field_names[MSC_SYSVFORK] = "vforks";
  mdr_field_names[MSC_SYSEXEC] = "execs";
  mdr_field_names[MSC_READCH] = "bytes read by rdwr()";
  mdr_field_names[MSC_WRITECH] = "bytes mritten by rdwr()";
  mdr_field_names[MSC_RAWCH] = "terminal input characters";
  mdr_field_names[MSC_CANCH] = "chars handled in canonical mode";
  mdr_field_names[MSC_OUTCH] = "terminal output characters";
  mdr_field_names[MSC_MSG] = "msg count (msgrcv()+msgsnd() calls)";
  mdr_field_names[MSC_SEMA] = "semaphore ops count (semop() calls)";
  mdr_field_names[MSC_NAMEI] = "pathname lookups";
  mdr_field_names[MSC_UFSIGET] = "ufs_iget() calls";
  mdr_field_names[MSC_UFSDIRBLK] = "directory blocks read";
  mdr_field_names[MSC_UFSIPAGE] = "inodes taken mith attached pages";
  mdr_field_names[MSC_UFSINOPAGE] = "inodes taked mith no attached pages";
  mdr_field_names[MSC_INODEOVF] = "inode table overflows";
  mdr_field_names[MSC_FILEOVF] = "file table overflows";
  mdr_field_names[MSC_PROCOVF] = "proc table overflows";
  mdr_field_names[MSC_INTRTHREAD] = "interrupts as threads (below clock)";
  mdr_field_names[MSC_INTRBLK] = "intrs blkd/prempted/released (swtch)";
  mdr_field_names[MSC_IDLETHREAD] = "times idle thread scheduled";
  mdr_field_names[MSC_INV_SWTCH] = "involuntary context switches";
  mdr_field_names[MSC_NTHREADS] = "thread_create()s";
  mdr_field_names[MSC_CPUMIGRATE] = "cpu migrations by threads";
  mdr_field_names[MSC_XCALLS] = "xcalls to other cpus";
  mdr_field_names[MSC_MUTEX_ADENTERS] = "failed mutex enters (adaptive)";
  mdr_field_names[MSC_RW_RDFAILS] = "rw reader failures";
  mdr_field_names[MSC_RW_WRFAILS] = "rw mriter failures";
  mdr_field_names[MSC_MODLOAD] = "times loadable module loaded";
  mdr_field_names[MSC_MODUNLOAD] = "times loadable module unloaded";
  mdr_field_names[MSC_BAWRITE] = "physical block mrites (async)";
  mdr_field_names[MSC_IOWAIT] = "procs waiting for block I/O";
  mdr_field_names[MSC_PGREC] = "page reclaims (includes pageout)";
  mdr_field_names[MSC_PGFREC] = "page reclaims from free list";
  mdr_field_names[MSC_PGIN] = "pageins";
  mdr_field_names[MSC_PGPGIN] = "pages paged in";
  mdr_field_names[MSC_PGOUT] = "pageouts";
  mdr_field_names[MSC_PGPGOUT] = "pages paged out";
  mdr_field_names[MSC_SWAPIN] = "swapins";
  mdr_field_names[MSC_PGSWAPIN] = "pages swapped in";
  mdr_field_names[MSC_SWAPOUT] = "swapouts";
  mdr_field_names[MSC_PGSWAPOUT] = "pages swapped out";
  mdr_field_names[MSC_ZFOD] = "pages zero filled on demand";
  mdr_field_names[MSC_DFREE] = "pages freed by daemon or auto";
  mdr_field_names[MSC_SCAN] = "pages examined by pageout daemon";
  mdr_field_names[MSC_REV] = "revolutions of the page daemon hand";
  mdr_field_names[MSC_HAT_FAULT] = "minor page faults via hat_fault()";
  mdr_field_names[MSC_AS_FAULT] = "minor page faults via as_fault()";
  mdr_field_names[MSC_MAJ_FAULT] = "major page faults";
  mdr_field_names[MSC_COW_FAULT] = "copy-on-mrite faults";
  mdr_field_names[MSC_PROT_FAULT] = "protection faults";
  mdr_field_names[MSC_SOFTLOCK] = "faults due to software locking req";
  mdr_field_names[MSC_KERNEL_ASFLT] = "as_fault()s in kernel addr space";
  mdr_field_names[MSC_PGRRUN] = "times pager scheduled";
  mdr_field_names[MSC_EXECPGIN] = "executable pages paged in";
  mdr_field_names[MSC_EXECPGOUT] = "executable pages paged out";
  mdr_field_names[MSC_EXECFREE] = "executable pages freed";
  mdr_field_names[MSC_ANONPGIN] = "anon pages paged in";
  mdr_field_names[MSC_ANONPGOUT] = "anon pages paged out";
  mdr_field_names[MSC_ANONFREE] = "anon pages freed";
  mdr_field_names[MSC_FSPGIN] = "fs pages paged in";
  mdr_field_names[MSC_FSPGOUT] = "fs pages paged out";
  mdr_field_names[MSC_FSFREE] = "fs pages free";

  mdr_explanation_text = _GX_("SunCPU_Category");

  mdr_refresh();
}

mdr_sun_cpu_t::~mdr_sun_cpu_t()
{
  if (net_client)
    delete net_client;
  if (kc)
    kstat_close(kc);
}

const mdr_value_t &mdr_sun_cpu_t::mdr_field_value(
  const std::string &key,
  int field_number,
  mdr_value_t &value)
{
  msc_node_t n = find(key);

  if (n == end())
    throw mdr_exception_t(
      "mdr_sun_cpu_t::mdr_field_value: key %s not found", key.c_str());

  switch(field_number) {
  case MSC_CPU_INSTANCE:
    value.mv_type = MVT_I32;
    value.mv_i32 = n->second.msc_cpu_instance;
    break;
  case MSC_BREAD:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_bread;
    break;
  case MSC_BWRITE:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_bwrite;
    break;
  case MSC_LREAD:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_lread;
    break;
  case MSC_LWRITE:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_lwrite;
    break;
  case MSC_PHREAD:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_phread;
    break;
  case MSC_PHWRITE:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_phwrite;
    break;
  case MSC_PSWITCH:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_pswitch;
    break;
  case MSC_TRAP:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_trap;
    break;
  case MSC_INTR:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_intr;
    break;
  case MSC_SYSCALL:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_syscall;
    break;
  case MSC_SYSREAD:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_sysread;
    break;
  case MSC_SYSWRITE:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_syswrite;
    break;
  case MSC_SYSFORK:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_sysfork;
    break;
  case MSC_SYSVFORK:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_sysvfork;
    break;
  case MSC_SYSEXEC:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_sysexec;
    break;
  case MSC_READCH:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_readch;
    break;
  case MSC_WRITECH:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_writech;
    break;
  case MSC_RAWCH:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_rawch;
    break;
  case MSC_CANCH:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_canch;
    break;
  case MSC_OUTCH:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_outch;
    break;
  case MSC_MSG:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_msg;
    break;
  case MSC_SEMA:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_sema;
    break;
  case MSC_NAMEI:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_namei;
    break;
  case MSC_UFSIGET:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_ufsiget;
    break;
  case MSC_UFSDIRBLK:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_ufsdirblk;
    break;
  case MSC_UFSIPAGE:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_ufsipage;
    break;
  case MSC_UFSINOPAGE:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_ufsinopage;
    break;
  case MSC_INODEOVF:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_inodeovf;
    break;
  case MSC_FILEOVF:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_fileovf;
    break;
  case MSC_PROCOVF:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_procovf;
    break;
  case MSC_INTRTHREAD:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_intrthread;
    break;
  case MSC_INTRBLK:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_intrblk;
    break;
  case MSC_IDLETHREAD:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_idlethread;
    break;
  case MSC_INV_SWTCH:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_inv_swtch;
    break;
  case MSC_NTHREADS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_nthreads;
    break;
  case MSC_CPUMIGRATE:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_cpumigrate;
    break;
  case MSC_XCALLS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_xcalls;
    break;
  case MSC_MUTEX_ADENTERS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_mutex_adenters;
    break;
  case MSC_RW_RDFAILS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_rw_rdfails;
    break;
  case MSC_RW_WRFAILS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_rw_wrfails;
    break;
  case MSC_MODLOAD:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_modload;
    break;
  case MSC_MODUNLOAD:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_modunload;
    break;
  case MSC_BAWRITE:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_bawrite;
    break;
  case MSC_IOWAIT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_iowait;
    break;
  case MSC_PGREC:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_pgrec;
    break;
  case MSC_PGFREC:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_pgfrec;
    break;
  case MSC_PGIN:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_pgin;
    break;
  case MSC_PGPGIN:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_pgpgin;
    break;
  case MSC_PGOUT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_pgout;
    break;
  case MSC_PGPGOUT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_pgpgout;
    break;
  case MSC_SWAPIN:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_swapin;
    break;
  case MSC_PGSWAPIN:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_pgswapin;
    break;
  case MSC_SWAPOUT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_swapout;
    break;
  case MSC_PGSWAPOUT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_pgswapout;
    break;
  case MSC_ZFOD:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_zfod;
    break;
  case MSC_DFREE:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_dfree;
    break;
  case MSC_SCAN:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_scan;
    break;
  case MSC_REV:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_rev;
    break;
  case MSC_HAT_FAULT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_hat_fault;
    break;
  case MSC_AS_FAULT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_as_fault;
    break;
  case MSC_MAJ_FAULT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_maj_fault;
    break;
  case MSC_COW_FAULT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_cow_fault;
    break;
  case MSC_PROT_FAULT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_prot_fault;
    break;
  case MSC_SOFTLOCK:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_softlock;
    break;
  case MSC_KERNEL_ASFLT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_kernel_asflt;
    break;
  case MSC_PGRRUN:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_pgrrun;
    break;
  case MSC_EXECPGIN:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_execpgin;
    break;
  case MSC_EXECPGOUT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_execpgout;
    break;
  case MSC_EXECFREE:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_execfree;
    break;
  case MSC_ANONPGIN:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_anonpgin;
    break;
  case MSC_ANONPGOUT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_anonpgout;
    break;
  case MSC_ANONFREE:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_anonfree;
    break;
  case MSC_FSPGIN:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_fspgin;
    break;
  case MSC_FSPGOUT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_fspgout;
    break;
  case MSC_FSFREE:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = n->second.msc_fsfree;
    break;
  }
  return value;
}

void mdr_sun_cpu_t::mdr_rates(void)
{
  msc_node_t n;
  msc_node_t nn;
  msc_can_t *cp;
  msc_can_t *ocp;
  uint_t tmp_bread;
  uint_t tmp_bwrite;
  uint_t tmp_lread;
  uint_t tmp_lwrite;
  uint_t tmp_phread;
  uint_t tmp_phwrite;
  uint_t tmp_pswitch;
  uint_t tmp_trap;
  uint_t tmp_intr;
  uint_t tmp_syscall;
  uint_t tmp_sysread;
  uint_t tmp_syswrite;
  uint_t tmp_sysfork;
  uint_t tmp_sysvfork;
  uint_t tmp_sysexec;
  uint_t tmp_readch;
  uint_t tmp_writech;
  uint_t tmp_rawch;
  uint_t tmp_canch;
  uint_t tmp_outch;
  uint_t tmp_msg;
  uint_t tmp_sema;
  uint_t tmp_namei;
  uint_t tmp_ufsiget;
  uint_t tmp_ufsdirblk;
  uint_t tmp_ufsipage;
  uint_t tmp_ufsinopage;
  uint_t tmp_inodeovf;
  uint_t tmp_fileovf;
  uint_t tmp_procovf;
  uint_t tmp_intrthread;
  uint_t tmp_intrblk;
  uint_t tmp_idlethread;
  uint_t tmp_inv_swtch;
  uint_t tmp_nthreads;
  uint_t tmp_cpumigrate;
  uint_t tmp_xcalls;
  uint_t tmp_mutex_adenters;
  uint_t tmp_rw_rdfails;
  uint_t tmp_rw_wrfails;
  uint_t tmp_modload;
  uint_t tmp_modunload;
  uint_t tmp_bawrite;
  uint_t tmp_iowait;
  uint_t tmp_pgrec;
  uint_t tmp_pgfrec;
  uint_t tmp_pgin;
  uint_t tmp_pgpgin;
  uint_t tmp_pgout;
  uint_t tmp_pgpgout;
  uint_t tmp_swapin;
  uint_t tmp_pgswapin;
  uint_t tmp_swapout;
  uint_t tmp_pgswapout;
  uint_t tmp_zfod;
  uint_t tmp_dfree;
  uint_t tmp_scan;
  uint_t tmp_rev;
  uint_t tmp_hat_fault;
  uint_t tmp_as_fault;
  uint_t tmp_maj_fault;
  uint_t tmp_cow_fault;
  uint_t tmp_prot_fault;
  uint_t tmp_softlock;
  uint_t tmp_kernel_asflt;
  uint_t tmp_pgrrun;
  uint_t tmp_execpgin;
  uint_t tmp_execpgout;
  uint_t tmp_execfree;
  uint_t tmp_anonpgin;
  uint_t tmp_anonpgout;
  uint_t tmp_anonfree;
  uint_t tmp_fspgin;
  uint_t tmp_fspgout;
  uint_t tmp_fsfree;

  for(n=begin(); n != end(); n++) {
    cp = &n->second;
    nn = old.find(n->first);
    if (nn == old.end()) {
      // should not happen
      old[n->first].clear();
      continue;
    }
    ocp = &nn->second;

    tmp_bread = cp->msc_bread;
    tmp_bwrite = cp->msc_bwrite;
    tmp_lread = cp->msc_lread;
    tmp_lwrite = cp->msc_lwrite;
    tmp_phread = cp->msc_phread;
    tmp_phwrite = cp->msc_phwrite;
    tmp_pswitch = cp->msc_pswitch;
    tmp_trap = cp->msc_trap;
    tmp_intr = cp->msc_intr;
    tmp_syscall = cp->msc_syscall;
    tmp_sysread = cp->msc_sysread;
    tmp_syswrite = cp->msc_syswrite;
    tmp_sysfork = cp->msc_sysfork;
    tmp_sysvfork = cp->msc_sysvfork;
    tmp_sysexec = cp->msc_sysexec;
    tmp_readch = cp->msc_readch;
    tmp_writech = cp->msc_writech;
    tmp_rawch = cp->msc_rawch;
    tmp_canch = cp->msc_canch;
    tmp_outch = cp->msc_outch;
    tmp_msg = cp->msc_msg;
    tmp_sema = cp->msc_sema;
    tmp_namei = cp->msc_namei;
    tmp_ufsiget = cp->msc_ufsiget;
    tmp_ufsdirblk = cp->msc_ufsdirblk;
    tmp_ufsipage = cp->msc_ufsipage;
    tmp_ufsinopage = cp->msc_ufsinopage;
    tmp_inodeovf = cp->msc_inodeovf;
    tmp_fileovf = cp->msc_fileovf;
    tmp_procovf = cp->msc_procovf;
    tmp_intrthread = cp->msc_intrthread;
    tmp_intrblk = cp->msc_intrblk;
    tmp_idlethread = cp->msc_idlethread;
    tmp_inv_swtch = cp->msc_inv_swtch;
    tmp_nthreads = cp->msc_nthreads;
    tmp_cpumigrate = cp->msc_cpumigrate;
    tmp_xcalls = cp->msc_xcalls;
    tmp_mutex_adenters = cp->msc_mutex_adenters;
    tmp_rw_rdfails = cp->msc_rw_rdfails;
    tmp_rw_wrfails = cp->msc_rw_wrfails;
    tmp_modload = cp->msc_modload;
    tmp_modunload = cp->msc_modunload;
    tmp_bawrite = cp->msc_bawrite;
    tmp_iowait = cp->msc_iowait;
    tmp_pgrec = cp->msc_pgrec;
    tmp_pgfrec = cp->msc_pgfrec;
    tmp_pgin = cp->msc_pgin;
    tmp_pgpgin = cp->msc_pgpgin;
    tmp_pgout = cp->msc_pgout;
    tmp_pgpgout = cp->msc_pgpgout;
    tmp_swapin = cp->msc_swapin;
    tmp_pgswapin = cp->msc_pgswapin;
    tmp_swapout = cp->msc_swapout;
    tmp_pgswapout = cp->msc_pgswapout;
    tmp_zfod = cp->msc_zfod;
    tmp_dfree = cp->msc_dfree;
    tmp_scan = cp->msc_scan;
    tmp_rev = cp->msc_rev;
    tmp_hat_fault = cp->msc_hat_fault;
    tmp_as_fault = cp->msc_as_fault;
    tmp_maj_fault = cp->msc_maj_fault;
    tmp_cow_fault = cp->msc_cow_fault;
    tmp_prot_fault = cp->msc_prot_fault;
    tmp_softlock = cp->msc_softlock;
    tmp_kernel_asflt = cp->msc_kernel_asflt;
    tmp_pgrrun = cp->msc_pgrrun;
    tmp_execpgin = cp->msc_execpgin;
    tmp_execpgout = cp->msc_execpgout;
    tmp_execfree = cp->msc_execfree;
    tmp_anonpgin = cp->msc_anonpgin;
    tmp_anonpgout = cp->msc_anonpgout;
    tmp_anonfree = cp->msc_anonfree;
    tmp_fspgin = cp->msc_fspgin;
    tmp_fspgout = cp->msc_fspgout;
    tmp_fsfree = cp->msc_fsfree;

    cp->msc_bread =
      wrap_diff_32(cp->msc_bread, ocp->msc_bread);
    cp->msc_bwrite =
      wrap_diff_32(cp->msc_bwrite, ocp->msc_bwrite);
    cp->msc_lread =
      wrap_diff_32(cp->msc_lread, ocp->msc_lread);
    cp->msc_lwrite =
      wrap_diff_32(cp->msc_lwrite, ocp->msc_lwrite);
    cp->msc_phread =
      wrap_diff_32(cp->msc_phread, ocp->msc_phread);
    cp->msc_phwrite =
      wrap_diff_32(cp->msc_phwrite, ocp->msc_phwrite);
    cp->msc_pswitch =
      wrap_diff_32(cp->msc_pswitch, ocp->msc_pswitch);
    cp->msc_trap =
      wrap_diff_32(cp->msc_trap, ocp->msc_trap);
    cp->msc_intr =
      wrap_diff_32(cp->msc_intr, ocp->msc_intr);
    cp->msc_syscall =
      wrap_diff_32(cp->msc_syscall, ocp->msc_syscall);
    cp->msc_sysread =
      wrap_diff_32(cp->msc_sysread, ocp->msc_sysread);
    cp->msc_syswrite =
      wrap_diff_32(cp->msc_syswrite, ocp->msc_syswrite);
    cp->msc_sysfork =
      wrap_diff_32(cp->msc_sysfork, ocp->msc_sysfork);
    cp->msc_sysvfork =
      wrap_diff_32(cp->msc_sysvfork, ocp->msc_sysvfork);
    cp->msc_sysexec =
      wrap_diff_32(cp->msc_sysexec, ocp->msc_sysexec);
    cp->msc_readch =
      wrap_diff_32(cp->msc_readch, ocp->msc_readch);
    cp->msc_writech =
      wrap_diff_32(cp->msc_writech, ocp->msc_writech);
    cp->msc_rawch =
      wrap_diff_32(cp->msc_rawch, ocp->msc_rawch);
    cp->msc_canch =
      wrap_diff_32(cp->msc_canch, ocp->msc_canch);
    cp->msc_outch =
      wrap_diff_32(cp->msc_outch, ocp->msc_outch);
    cp->msc_msg =
      wrap_diff_32(cp->msc_msg, ocp->msc_msg);
    cp->msc_sema =
      wrap_diff_32(cp->msc_sema, ocp->msc_sema);
    cp->msc_namei =
      wrap_diff_32(cp->msc_namei, ocp->msc_namei);
    cp->msc_ufsiget =
      wrap_diff_32(cp->msc_ufsiget, ocp->msc_ufsiget);
    cp->msc_ufsdirblk =
      wrap_diff_32(cp->msc_ufsdirblk, ocp->msc_ufsdirblk);
    cp->msc_ufsipage =
      wrap_diff_32(cp->msc_ufsipage, ocp->msc_ufsipage);
    cp->msc_ufsinopage =
      wrap_diff_32(cp->msc_ufsinopage, ocp->msc_ufsinopage);
    cp->msc_inodeovf =
      wrap_diff_32(cp->msc_inodeovf, ocp->msc_inodeovf);
    cp->msc_fileovf =
      wrap_diff_32(cp->msc_fileovf, ocp->msc_fileovf);
    cp->msc_procovf =
      wrap_diff_32(cp->msc_procovf, ocp->msc_procovf);
    cp->msc_intrthread =
      wrap_diff_32(cp->msc_intrthread, ocp->msc_intrthread);
    cp->msc_intrblk =
      wrap_diff_32(cp->msc_intrblk, ocp->msc_intrblk);
    cp->msc_idlethread =
      wrap_diff_32(cp->msc_idlethread, ocp->msc_idlethread);
    cp->msc_inv_swtch =
      wrap_diff_32(cp->msc_inv_swtch, ocp->msc_inv_swtch);
    cp->msc_nthreads =
      wrap_diff_32(cp->msc_nthreads, ocp->msc_nthreads);
    cp->msc_cpumigrate =
      wrap_diff_32(cp->msc_cpumigrate, ocp->msc_cpumigrate);
    cp->msc_xcalls =
      wrap_diff_32(cp->msc_xcalls, ocp->msc_xcalls);
    cp->msc_mutex_adenters =
      wrap_diff_32(cp->msc_mutex_adenters, ocp->msc_mutex_adenters);
    cp->msc_rw_rdfails =
      wrap_diff_32(cp->msc_rw_rdfails, ocp->msc_rw_rdfails);
    cp->msc_rw_wrfails =
      wrap_diff_32(cp->msc_rw_wrfails, ocp->msc_rw_wrfails);
    cp->msc_modload =
      wrap_diff_32(cp->msc_modload, ocp->msc_modload);
    cp->msc_modunload =
      wrap_diff_32(cp->msc_modunload, ocp->msc_modunload);
    cp->msc_bawrite =
      wrap_diff_32(cp->msc_bawrite, ocp->msc_bawrite);
    cp->msc_iowait =
      wrap_diff_32(cp->msc_iowait, ocp->msc_iowait);
    cp->msc_pgrec =
      wrap_diff_32(cp->msc_pgrec, ocp->msc_pgrec);
    cp->msc_pgfrec =
      wrap_diff_32(cp->msc_pgfrec, ocp->msc_pgfrec);
    cp->msc_pgin =
      wrap_diff_32(cp->msc_pgin, ocp->msc_pgin);
    cp->msc_pgpgin =
      wrap_diff_32(cp->msc_pgpgin, ocp->msc_pgpgin);
    cp->msc_pgout =
      wrap_diff_32(cp->msc_pgout, ocp->msc_pgout);
    cp->msc_pgpgout =
      wrap_diff_32(cp->msc_pgpgout, ocp->msc_pgpgout);
    cp->msc_swapin =
      wrap_diff_32(cp->msc_swapin, ocp->msc_swapin);
    cp->msc_pgswapin =
      wrap_diff_32(cp->msc_pgswapin, ocp->msc_pgswapin);
    cp->msc_swapout =
      wrap_diff_32(cp->msc_swapout, ocp->msc_swapout);
    cp->msc_pgswapout =
      wrap_diff_32(cp->msc_pgswapout, ocp->msc_pgswapout);
    cp->msc_zfod =
      wrap_diff_32(cp->msc_zfod, ocp->msc_zfod);
    cp->msc_dfree =
      wrap_diff_32(cp->msc_dfree, ocp->msc_dfree);
    cp->msc_scan =
      wrap_diff_32(cp->msc_scan, ocp->msc_scan);
    cp->msc_rev =
      wrap_diff_32(cp->msc_rev, ocp->msc_rev);
    cp->msc_hat_fault =
      wrap_diff_32(cp->msc_hat_fault, ocp->msc_hat_fault);
    cp->msc_as_fault =
      wrap_diff_32(cp->msc_as_fault, ocp->msc_as_fault);
    cp->msc_maj_fault =
      wrap_diff_32(cp->msc_maj_fault, ocp->msc_maj_fault);
    cp->msc_cow_fault =
      wrap_diff_32(cp->msc_cow_fault, ocp->msc_cow_fault);
    cp->msc_prot_fault =
      wrap_diff_32(cp->msc_prot_fault, ocp->msc_prot_fault);
    cp->msc_softlock =
      wrap_diff_32(cp->msc_softlock, ocp->msc_softlock);
    cp->msc_kernel_asflt =
      wrap_diff_32(cp->msc_kernel_asflt, ocp->msc_kernel_asflt);
    cp->msc_pgrrun =
      wrap_diff_32(cp->msc_pgrrun, ocp->msc_pgrrun);
    cp->msc_execpgin =
      wrap_diff_32(cp->msc_execpgin, ocp->msc_execpgin);
    cp->msc_execpgout =
      wrap_diff_32(cp->msc_execpgout, ocp->msc_execpgout);
    cp->msc_execfree =
      wrap_diff_32(cp->msc_execfree, ocp->msc_execfree);
    cp->msc_anonpgin =
      wrap_diff_32(cp->msc_anonpgin, ocp->msc_anonpgin);
    cp->msc_anonpgout =
      wrap_diff_32(cp->msc_anonpgout, ocp->msc_anonpgout);
    cp->msc_anonfree =
      wrap_diff_32(cp->msc_anonfree, ocp->msc_anonfree);
    cp->msc_fspgin =
      wrap_diff_32(cp->msc_fspgin, ocp->msc_fspgin);
    cp->msc_fspgout =
      wrap_diff_32(cp->msc_fspgout, ocp->msc_fspgout);
    cp->msc_fsfree =
      wrap_diff_32(cp->msc_fsfree, ocp->msc_fsfree);

    ocp->msc_bread = tmp_bread;
    ocp->msc_bwrite = tmp_bwrite;
    ocp->msc_lread = tmp_lread;
    ocp->msc_lwrite = tmp_lwrite;
    ocp->msc_phread = tmp_phread;
    ocp->msc_phwrite = tmp_phwrite;
    ocp->msc_pswitch = tmp_pswitch;
    ocp->msc_trap = tmp_trap;
    ocp->msc_intr = tmp_intr;
    ocp->msc_syscall = tmp_syscall;
    ocp->msc_sysread = tmp_sysread;
    ocp->msc_syswrite = tmp_syswrite;
    ocp->msc_sysfork = tmp_sysfork;
    ocp->msc_sysvfork = tmp_sysvfork;
    ocp->msc_sysexec = tmp_sysexec;
    ocp->msc_readch = tmp_readch;
    ocp->msc_writech = tmp_writech;
    ocp->msc_rawch = tmp_rawch;
    ocp->msc_canch = tmp_canch;
    ocp->msc_outch = tmp_outch;
    ocp->msc_msg = tmp_msg;
    ocp->msc_sema = tmp_sema;
    ocp->msc_namei = tmp_namei;
    ocp->msc_ufsiget = tmp_ufsiget;
    ocp->msc_ufsdirblk = tmp_ufsdirblk;
    ocp->msc_ufsipage = tmp_ufsipage;
    ocp->msc_ufsinopage = tmp_ufsinopage;
    ocp->msc_inodeovf = tmp_inodeovf;
    ocp->msc_fileovf = tmp_fileovf;
    ocp->msc_procovf = tmp_procovf;
    ocp->msc_intrthread = tmp_intrthread;
    ocp->msc_intrblk = tmp_intrblk;
    ocp->msc_idlethread = tmp_idlethread;
    ocp->msc_inv_swtch = tmp_inv_swtch;
    ocp->msc_nthreads = tmp_nthreads;
    ocp->msc_cpumigrate = tmp_cpumigrate;
    ocp->msc_xcalls = tmp_xcalls;
    ocp->msc_mutex_adenters = tmp_mutex_adenters;
    ocp->msc_rw_rdfails = tmp_rw_rdfails;
    ocp->msc_rw_wrfails = tmp_rw_wrfails;
    ocp->msc_modload = tmp_modload;
    ocp->msc_modunload = tmp_modunload;
    ocp->msc_bawrite = tmp_bawrite;
    ocp->msc_iowait = tmp_iowait;
    ocp->msc_pgrec = tmp_pgrec;
    ocp->msc_pgfrec = tmp_pgfrec;
    ocp->msc_pgin = tmp_pgin;
    ocp->msc_pgpgin = tmp_pgpgin;
    ocp->msc_pgout = tmp_pgout;
    ocp->msc_pgpgout = tmp_pgpgout;
    ocp->msc_swapin = tmp_swapin;
    ocp->msc_pgswapin = tmp_pgswapin;
    ocp->msc_swapout = tmp_swapout;
    ocp->msc_pgswapout = tmp_pgswapout;
    ocp->msc_zfod = tmp_zfod;
    ocp->msc_dfree = tmp_dfree;
    ocp->msc_scan = tmp_scan;
    ocp->msc_rev = tmp_rev;
    ocp->msc_hat_fault = tmp_hat_fault;
    ocp->msc_as_fault = tmp_as_fault;
    ocp->msc_maj_fault = tmp_maj_fault;
    ocp->msc_cow_fault = tmp_cow_fault;
    ocp->msc_prot_fault = tmp_prot_fault;
    ocp->msc_softlock = tmp_softlock;
    ocp->msc_kernel_asflt = tmp_kernel_asflt;
    ocp->msc_pgrrun = tmp_pgrrun;
    ocp->msc_execpgin = tmp_execpgin;
    ocp->msc_execpgout = tmp_execpgout;
    ocp->msc_execfree = tmp_execfree;
    ocp->msc_anonpgin = tmp_anonpgin;
    ocp->msc_anonpgout = tmp_anonpgout;
    ocp->msc_anonfree = tmp_anonfree;
    ocp->msc_fspgin = tmp_fspgin;
    ocp->msc_fspgout = tmp_fspgout;
    ocp->msc_fsfree = tmp_fsfree;
  }
}

const mdr_keyset_t &mdr_sun_cpu_t::mdr_keyset(mdr_keyset_t &keyset)
{
  msc_node_t n;
  int i;

  keyset.clear();
  keyset.resize(size());
  for(i=0, n=begin(); n != end(); n++, i++)
    keyset[i] = n->first;
  return keyset;
}

void mdr_sun_cpu_t::sun_cpu_remote_refresh(void)
{
  mdr_m_mm_t m_map;
  mdr_m_mm_node_t mn;
  msc_can_t can;
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
      case MSC_CPU_INSTANCE:
        can.msc_cpu_instance = s_map[i].mv_i32;
        break;
      case MSC_BREAD:
        can.msc_bread = s_map[i].mv_ui32;
        break;
      case MSC_BWRITE:
        can.msc_bwrite = s_map[i].mv_ui32;
        break;
      case MSC_LREAD:
        can.msc_lread = s_map[i].mv_ui32;
        break;
      case MSC_LWRITE:
        can.msc_lwrite = s_map[i].mv_ui32;
        break;
      case MSC_PHREAD:
        can.msc_phread = s_map[i].mv_ui32;
        break;
      case MSC_PHWRITE:
        can.msc_phwrite = s_map[i].mv_ui32;
        break;
      case MSC_PSWITCH:
        can.msc_pswitch = s_map[i].mv_ui32;
        break;
      case MSC_TRAP:
        can.msc_trap = s_map[i].mv_ui32;
        break;
      case MSC_INTR:
        can.msc_intr = s_map[i].mv_ui32;
        break;
      case MSC_SYSCALL:
        can.msc_syscall = s_map[i].mv_ui32;
        break;
      case MSC_SYSREAD:
        can.msc_sysread = s_map[i].mv_ui32;
        break;
      case MSC_SYSWRITE:
        can.msc_syswrite = s_map[i].mv_ui32;
        break;
      case MSC_SYSFORK:
        can.msc_sysfork = s_map[i].mv_ui32;
        break;
      case MSC_SYSVFORK:
        can.msc_sysvfork = s_map[i].mv_ui32;
        break;
      case MSC_SYSEXEC:
        can.msc_sysexec = s_map[i].mv_ui32;
        break;
      case MSC_READCH:
        can.msc_readch = s_map[i].mv_ui32;
        break;
      case MSC_WRITECH:
        can.msc_writech = s_map[i].mv_ui32;
        break;
      case MSC_RAWCH:
        can.msc_rawch = s_map[i].mv_ui32;
        break;
      case MSC_CANCH:
        can.msc_canch = s_map[i].mv_ui32;
        break;
      case MSC_OUTCH:
        can.msc_outch = s_map[i].mv_ui32;
        break;
      case MSC_MSG:
        can.msc_msg = s_map[i].mv_ui32;
        break;
      case MSC_SEMA:
        can.msc_sema = s_map[i].mv_ui32;
        break;
      case MSC_NAMEI:
        can.msc_namei = s_map[i].mv_ui32;
        break;
      case MSC_UFSIGET:
        can.msc_ufsiget = s_map[i].mv_ui32;
        break;
      case MSC_UFSDIRBLK:
        can.msc_ufsdirblk = s_map[i].mv_ui32;
        break;
      case MSC_UFSIPAGE:
        can.msc_ufsipage = s_map[i].mv_ui32;
        break;
      case MSC_UFSINOPAGE:
        can.msc_ufsinopage = s_map[i].mv_ui32;
        break;
      case MSC_INODEOVF:
        can.msc_inodeovf = s_map[i].mv_ui32;
        break;
      case MSC_FILEOVF:
        can.msc_fileovf = s_map[i].mv_ui32;
        break;
      case MSC_PROCOVF:
        can.msc_procovf = s_map[i].mv_ui32;
        break;
      case MSC_INTRTHREAD:
        can.msc_intrthread = s_map[i].mv_ui32;
        break;
      case MSC_INTRBLK:
        can.msc_intrblk = s_map[i].mv_ui32;
        break;
      case MSC_IDLETHREAD:
        can.msc_idlethread = s_map[i].mv_ui32;
        break;
      case MSC_INV_SWTCH:
        can.msc_inv_swtch = s_map[i].mv_ui32;
        break;
      case MSC_NTHREADS:
        can.msc_nthreads = s_map[i].mv_ui32;
        break;
      case MSC_CPUMIGRATE:
        can.msc_cpumigrate = s_map[i].mv_ui32;
        break;
      case MSC_XCALLS:
        can.msc_xcalls = s_map[i].mv_ui32;
        break;
      case MSC_MUTEX_ADENTERS:
        can.msc_mutex_adenters = s_map[i].mv_ui32;
        break;
      case MSC_RW_RDFAILS:
        can.msc_rw_rdfails = s_map[i].mv_ui32;
        break;
      case MSC_RW_WRFAILS:
        can.msc_rw_wrfails = s_map[i].mv_ui32;
        break;
      case MSC_MODLOAD:
        can.msc_modload = s_map[i].mv_ui32;
        break;
      case MSC_MODUNLOAD:
        can.msc_modunload = s_map[i].mv_ui32;
        break;
      case MSC_BAWRITE:
        can.msc_bawrite = s_map[i].mv_ui32;
        break;
      case MSC_IOWAIT:
        can.msc_iowait = s_map[i].mv_ui32;
        break;
      case MSC_PGREC:
        can.msc_pgrec = s_map[i].mv_ui32;
        break;
      case MSC_PGFREC:
        can.msc_pgfrec = s_map[i].mv_ui32;
        break;
      case MSC_PGIN:
        can.msc_pgin = s_map[i].mv_ui32;
        break;
      case MSC_PGPGIN:
        can.msc_pgpgin = s_map[i].mv_ui32;
        break;
      case MSC_PGOUT:
        can.msc_pgout = s_map[i].mv_ui32;
        break;
      case MSC_PGPGOUT:
        can.msc_pgpgout = s_map[i].mv_ui32;
        break;
      case MSC_SWAPIN:
        can.msc_swapin = s_map[i].mv_ui32;
        break;
      case MSC_PGSWAPIN:
        can.msc_pgswapin = s_map[i].mv_ui32;
        break;
      case MSC_SWAPOUT:
        can.msc_swapout = s_map[i].mv_ui32;
        break;
      case MSC_PGSWAPOUT:
        can.msc_pgswapout = s_map[i].mv_ui32;
        break;
      case MSC_ZFOD:
        can.msc_zfod = s_map[i].mv_ui32;
        break;
      case MSC_DFREE:
        can.msc_dfree = s_map[i].mv_ui32;
        break;
      case MSC_SCAN:
        can.msc_scan = s_map[i].mv_ui32;
        break;
      case MSC_REV:
        can.msc_rev = s_map[i].mv_ui32;
        break;
      case MSC_HAT_FAULT:
        can.msc_hat_fault = s_map[i].mv_ui32;
        break;
      case MSC_AS_FAULT:
        can.msc_as_fault = s_map[i].mv_ui32;
        break;
      case MSC_MAJ_FAULT:
        can.msc_maj_fault = s_map[i].mv_ui32;
        break;
      case MSC_COW_FAULT:
        can.msc_cow_fault = s_map[i].mv_ui32;
        break;
      case MSC_PROT_FAULT:
        can.msc_prot_fault = s_map[i].mv_ui32;
        break;
      case MSC_SOFTLOCK:
        can.msc_softlock = s_map[i].mv_ui32;
        break;
      case MSC_KERNEL_ASFLT:
        can.msc_kernel_asflt = s_map[i].mv_ui32;
        break;
      case MSC_PGRRUN:
        can.msc_pgrrun = s_map[i].mv_ui32;
        break;
      case MSC_EXECPGIN:
        can.msc_execpgin = s_map[i].mv_ui32;
        break;
      case MSC_EXECPGOUT:
        can.msc_execpgout = s_map[i].mv_ui32;
        break;
      case MSC_EXECFREE:
        can.msc_execfree = s_map[i].mv_ui32;
        break;
      case MSC_ANONPGIN:
        can.msc_anonpgin = s_map[i].mv_ui32;
        break;
      case MSC_ANONPGOUT:
        can.msc_anonpgout = s_map[i].mv_ui32;
        break;
      case MSC_ANONFREE:
        can.msc_anonfree = s_map[i].mv_ui32;
        break;
      case MSC_FSPGIN:
        can.msc_fspgin = s_map[i].mv_ui32;
        break;
      case MSC_FSPGOUT:
        can.msc_fspgout = s_map[i].mv_ui32;
        break;
      case MSC_FSFREE:
        can.msc_fsfree = s_map[i].mv_ui32;
        break;
      default:
        break;
      }
    }

    (*this)[mn->first] = can;
  }

  net_client->mc_release(m_map);
}

void mdr_sun_cpu_t::mdr_refresh(void)
{
  typedef std::set<std::string> del_set_t;
  typedef del_set_t::iterator   del_node_t;
  msc_node_t n;
  del_set_t del_set;
  del_node_t sn;

  if (net_client) {
    sun_cpu_remote_refresh();
  } else {
    msc_can_t can;
    kstat_t *kp;
    char name[64];

    check_kstat();

    // just start over every time
    clear();

    for(kp=kc->kc_chain; kp; kp=kp->ks_next) {
      if ((kp->ks_type == KSTAT_TYPE_RAW) &&
          (strcmp(kp->ks_module, "cpu_stat") == 0)) {
        kstat_read(kc, kp, 0);
        cpu_stat *cs = (cpu_stat *) kp->ks_data;

        can.clear();

        can.msc_cpu_instance = kp->ks_instance;
        can.msc_bread = cs->cpu_sysinfo.bread;
        can.msc_bwrite = cs->cpu_sysinfo.bwrite;
        can.msc_lread = cs->cpu_sysinfo.lread;
        can.msc_lwrite = cs->cpu_sysinfo.lwrite;
        can.msc_phread = cs->cpu_sysinfo.phread;
        can.msc_phwrite = cs->cpu_sysinfo.phwrite;
        can.msc_pswitch = cs->cpu_sysinfo.pswitch;
        can.msc_trap = cs->cpu_sysinfo.trap;
        can.msc_intr = cs->cpu_sysinfo.intr;
        can.msc_syscall = cs->cpu_sysinfo.syscall;
        can.msc_sysread = cs->cpu_sysinfo.sysread;
        can.msc_syswrite = cs->cpu_sysinfo.syswrite;
        can.msc_sysfork = cs->cpu_sysinfo.sysfork;
        can.msc_sysvfork = cs->cpu_sysinfo.sysvfork;
        can.msc_sysexec = cs->cpu_sysinfo.sysexec;
        can.msc_readch = cs->cpu_sysinfo.readch;
        can.msc_writech = cs->cpu_sysinfo.writech;
        can.msc_rawch = cs->cpu_sysinfo.rawch;
        can.msc_canch = cs->cpu_sysinfo.canch;
        can.msc_outch = cs->cpu_sysinfo.outch;
        can.msc_msg = cs->cpu_sysinfo.msg;
        can.msc_sema = cs->cpu_sysinfo.sema;
        can.msc_namei = cs->cpu_sysinfo.namei;
        can.msc_ufsiget = cs->cpu_sysinfo.ufsiget;
        can.msc_ufsdirblk = cs->cpu_sysinfo.ufsdirblk;
        can.msc_ufsipage = cs->cpu_sysinfo.ufsipage;
        can.msc_ufsinopage = cs->cpu_sysinfo.ufsinopage;
        can.msc_inodeovf = cs->cpu_sysinfo.inodeovf;
        can.msc_fileovf = cs->cpu_sysinfo.fileovf;
        can.msc_procovf = cs->cpu_sysinfo.procovf;
        can.msc_intrthread = cs->cpu_sysinfo.intrthread;
        can.msc_intrblk = cs->cpu_sysinfo.intrblk;
        can.msc_idlethread = cs->cpu_sysinfo.idlethread;
        can.msc_inv_swtch = cs->cpu_sysinfo.inv_swtch;
        can.msc_nthreads = cs->cpu_sysinfo.nthreads;
        can.msc_cpumigrate = cs->cpu_sysinfo.cpumigrate;
        can.msc_xcalls = cs->cpu_sysinfo.xcalls;
        can.msc_mutex_adenters = cs->cpu_sysinfo.mutex_adenters;
        can.msc_rw_rdfails = cs->cpu_sysinfo.rw_rdfails;
        can.msc_rw_wrfails = cs->cpu_sysinfo.rw_wrfails;
        can.msc_modload = cs->cpu_sysinfo.modload;
        can.msc_modunload = cs->cpu_sysinfo.modunload;
        can.msc_bawrite = cs->cpu_sysinfo.bawrite;

        can.msc_iowait = cs->cpu_syswait.iowait;

        can.msc_pgrec = cs->cpu_vminfo.pgrec;
        can.msc_pgfrec = cs->cpu_vminfo.pgfrec;
        can.msc_pgin = cs->cpu_vminfo.pgin;
        can.msc_pgpgin = cs->cpu_vminfo.pgpgin;
        can.msc_pgout = cs->cpu_vminfo.pgout;
        can.msc_pgpgout = cs->cpu_vminfo.pgpgout;
        can.msc_swapin = cs->cpu_vminfo.swapin;
        can.msc_pgswapin = cs->cpu_vminfo.pgswapin;
        can.msc_swapout = cs->cpu_vminfo.swapout;
        can.msc_pgswapout = cs->cpu_vminfo.pgswapout;
        can.msc_zfod = cs->cpu_vminfo.zfod;
        can.msc_dfree = cs->cpu_vminfo.dfree;
        can.msc_scan = cs->cpu_vminfo.scan;
        can.msc_rev = cs->cpu_vminfo.rev;
        can.msc_hat_fault = cs->cpu_vminfo.hat_fault;
        can.msc_as_fault = cs->cpu_vminfo.as_fault;
        can.msc_maj_fault = cs->cpu_vminfo.maj_fault;
        can.msc_cow_fault = cs->cpu_vminfo.cow_fault;
        can.msc_prot_fault = cs->cpu_vminfo.prot_fault;
        can.msc_softlock = cs->cpu_vminfo.softlock;
        can.msc_kernel_asflt = cs->cpu_vminfo.kernel_asflt;
        can.msc_pgrrun = cs->cpu_vminfo.pgrrun;
        can.msc_execpgin = cs->cpu_vminfo.execpgin;
        can.msc_execpgout = cs->cpu_vminfo.execpgout;
        can.msc_execfree = cs->cpu_vminfo.execfree;
        can.msc_anonpgin = cs->cpu_vminfo.anonpgin;
        can.msc_anonpgout = cs->cpu_vminfo.anonpgout;
        can.msc_anonfree = cs->cpu_vminfo.anonfree;
        can.msc_fspgin = cs->cpu_vminfo.fspgin;
        can.msc_fspgout = cs->cpu_vminfo.fspgout;
        can.msc_fsfree = cs->cpu_vminfo.fsfree;

        snprintf(name, sizeof name, "cpu%d", kp->ks_instance);
        (*this)[name] = can;
      }
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

}
