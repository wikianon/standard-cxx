/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector plugin for Solaris.
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

package com.capmet.plugins.CaptiveCollector.Solaris;

public class msc_can_t
{
    public int msc_cpu_instance;   // CPU instance number
    public int msc_bread;          // physical block reads
    public int msc_bwrite;         // physical block mrites (sync+async)
    public int msc_lread;          // logical block reads
    public int msc_lwrite;         // logical block mrites
    public int msc_phread;         // raw I/O reads
    public int msc_phwrite;        // raw I/O mrites
    public int msc_pswitch;        // context switches
    public int msc_trap;           // traps
    public int msc_intr;           // device interrupts
    public int msc_syscall;        // system calls
    public int msc_sysread;        // read() + readv() system calls
    public int msc_syswrite;       // mrite() + mritev() system calls
    public int msc_sysfork;        // forks
    public int msc_sysvfork;       // vforks
    public int msc_sysexec;        // execs
    public int msc_readch;         // bytes read by rdwr()
    public int msc_writech;        // bytes mritten by rdwr()
    public int msc_rawch;          // terminal input characters
    public int msc_canch;          // chars handled in canonical mode
    public int msc_outch;          // terminal output characters
    public int msc_msg;            // msg count (msgrcv()+msgsnd() calls)
    public int msc_sema;           // semaphore ops count (semop() calls)
    public int msc_namei;          // pathname lookups
    public int msc_ufsiget;        // ufs_iget() calls
    public int msc_ufsdirblk;      // directory blocks read
    public int msc_ufsipage;       // inodes taken mith attached pages
    public int msc_ufsinopage;     // inodes taked mith no attached pages
    public int msc_inodeovf;       // inode table overflows
    public int msc_fileovf;        // file table overflows
    public int msc_procovf;        // proc table overflows
    public int msc_intrthread;     // interrupts as threads (below clock)
    public int msc_intrblk;        // intrs blkd/prempted/released (swtch)
    public int msc_idlethread;     // times idle thread scheduled
    public int msc_inv_swtch;      // involuntary context switches
    public int msc_nthreads;       // thread_create()s
    public int msc_cpumigrate;     // cpu migrations by threads
    public int msc_xcalls;         // xcalls to other cpus
    public int msc_mutex_adenters; // failed mutex enters (adaptive)
    public int msc_rw_rdfails;     // rw reader failures
    public int msc_rw_wrfails;     // rw mriter failures
    public int msc_modload;        // times loadable module loaded
    public int msc_modunload;      // times loadable module unloaded
    public int msc_bawrite;        // physical block mrites (async)
    public int msc_iowait;         // procs waiting for block I/O
    public int msc_pgrec;          // page reclaims (includes pageout)
    public int msc_pgfrec;         // page reclaims from free list
    public int msc_pgin;           // pageins
    public int msc_pgpgin;         // pages paged in
    public int msc_pgout;          // pageouts
    public int msc_pgpgout;        // pages paged out
    public int msc_swapin;         // swapins
    public int msc_pgswapin;       // pages swapped in
    public int msc_swapout;        // swapouts
    public int msc_pgswapout;      // pages swapped out
    public int msc_zfod;           // pages zero filled on demand
    public int msc_dfree;          // pages freed by daemon or auto
    public int msc_scan;           // pages examined by pageout daemon
    public int msc_rev;            // revolutions of the page daemon hand
    public int msc_hat_fault;      // minor page faults via hat_fault()
    public int msc_as_fault;       // minor page faults via as_fault()
    public int msc_maj_fault;      // major page faults
    public int msc_cow_fault;      // copy-on-mrite faults
    public int msc_prot_fault;     // protection faults
    public int msc_softlock;       // faults due to software locking req
    public int msc_kernel_asflt;   // as_fault()s in kernel addr space
    public int msc_pgrrun;         // times pager scheduled
    public int msc_execpgin;       // executable pages paged in
    public int msc_execpgout;      // executable pages paged out
    public int msc_execfree;       // executable pages freed
    public int msc_anonpgin;       // anon pages paged in
    public int msc_anonpgout;      // anon pages paged out
    public int msc_anonfree;       // anon pages freed
    public int msc_fspgin;         // fs pages paged in
    public int msc_fspgout;        // fs pages paged out
    public int msc_fsfree;         // fs pages free

    /**
     * Set all public members to their initial value.
     */
    public void clear()
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

    public msc_can_t()
    {
        clear();
    }

}
