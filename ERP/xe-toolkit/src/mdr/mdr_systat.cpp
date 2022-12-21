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


#include "mdr_systat.h"

namespace mdr {

mdr_systat_t::mdr_systat_t(const char *remote_host) :
  mdr_t(MDR_SYSTAT_NAME, true, MSYS_FIELD_COUNT, true)
{
  mdr_field_names[MSYS_USER_TIME_PERCENT]           = "Percent User Time";
  mdr_field_names[MSYS_KERNEL_TIME_PERCENT]         = "Percent Kernel Time";
  mdr_field_names[MSYS_WAIT_TIME_PERCENT]           = "Percent Wait Time";
  mdr_field_names[MSYS_IDLE_TIME_PERCENT]           = "Percent Idle Time";
  mdr_field_names[MSYS_PAGE_K_IN_PER_SECOND]        = "KBytes Paged in/sec";
  mdr_field_names[MSYS_PAGE_K_OUT_PER_SECOND]       = "KBytes Paged out/sec";
  mdr_field_names[MSYS_INTERRUPTS_PER_SECOND]       = "Interrupts/sec";
  mdr_field_names[MSYS_CONTEXT_SWITCHES_PER_SECOND] = "Context Switches/sec";
  mdr_field_names[MSYS_SYSTEM_CALLS_PER_SECOND]     = "System Calls/sec";
  mdr_field_names[MSYS_PROCS_RUNNABLE]              = "Runnable Processes";
  mdr_field_names[MSYS_PROCS_BLOCKED]               = "Blocked Processes";
  mdr_field_names[MSYS_PROCS_ZOMBIED]               = "Zombie Processes";
  mdr_field_names[MSYS_FREE_MEMORY_PERCENT]         = "Percent Memory Free";
  mdr_field_names[MSYS_FREE_PAGING_FILE_PERCENT]    = "Percent Paging File Free";
  mdr_field_names[MSYS_READ_K_PER_SECOND]           = "KBytes Read/sec";
  mdr_field_names[MSYS_WRITTEN_K_PER_SECOND]        = "KBytes Written/sec";
  mdr_field_names[MSYS_PAGES_SCANNED]               = "Pages Scanned";
  mdr_field_names[MSYS_PAGE_FAULTS]                 = "Total Page Faults";
  mdr_field_names[MSYS_MAJOR_FAULTS]                = "Major Page Faults";

  // sync localized names
  mdr_map_names();

  misc = new mdr_misc_t(remote_host);
  cpu  = new mdr_cpustat_t(remote_host);
  pf   = new mdr_pf_t(remote_host);
  mem  = new mdr_memory_t(remote_host);
  proc = new mdr_ps_t(remote_host);
  disk = new mdr_disk_t(remote_host);

  if (cpu->mdr_is_supported(MCS_USER_TIME_PERCENT))
    mdr_support_field(MSYS_USER_TIME_PERCENT);
  if (cpu->mdr_is_supported(MCS_KERNEL_TIME_PERCENT))
    mdr_support_field(MSYS_KERNEL_TIME_PERCENT);
  if (cpu->mdr_is_supported(MCS_WAIT_TIME_PERCENT))
    mdr_support_field(MSYS_WAIT_TIME_PERCENT);
  if (cpu->mdr_is_supported(MCS_IDLE_TIME_PERCENT))
    mdr_support_field(MSYS_IDLE_TIME_PERCENT);
  if (mem->mdr_is_supported(MMEM_PAGED_IN_K))
    mdr_support_field(MSYS_PAGE_K_IN_PER_SECOND);
  if (mem->mdr_is_supported(MMEM_PAGED_OUT_K))
    mdr_support_field(MSYS_PAGE_K_OUT_PER_SECOND);
  if (misc->mdr_is_supported(MM_INTERRUPTS))
    mdr_support_field(MSYS_INTERRUPTS_PER_SECOND);
  if (misc->mdr_is_supported(MM_CONTEXT_SWITCHES))
    mdr_support_field(MSYS_CONTEXT_SWITCHES_PER_SECOND);
  if (misc->mdr_is_supported(MM_SYSTEM_CALLS))
    mdr_support_field(MSYS_SYSTEM_CALLS_PER_SECOND);
  if (proc->mdr_is_supported(MPS_PROCS_RUNNABLE))
    mdr_support_field(MSYS_PROCS_RUNNABLE);
  if (proc->mdr_is_supported(MPS_PROCS_BLOCKED))
    mdr_support_field(MSYS_PROCS_BLOCKED);
  if (proc->mdr_is_supported(MPS_PROCS_ZOMBIE))
    mdr_support_field(MSYS_PROCS_ZOMBIED);
  if (mem->mdr_is_supported(MMEM_FREE_MEMORY_K) &&
      misc->mdr_is_supported(MM_PHYSICAL_PAGES))
    mdr_support_field(MSYS_FREE_MEMORY_PERCENT);
  if (pf->mdr_is_supported(MPF_SIZE_K) &&
      pf->mdr_is_supported(MPF_FREE_K))
    mdr_support_field(MSYS_FREE_PAGING_FILE_PERCENT);
  if (disk->mdr_is_supported(MD_READ_K))
    mdr_support_field(MSYS_READ_K_PER_SECOND);
  if (disk->mdr_is_supported(MD_WRITTEN_K))
    mdr_support_field(MSYS_WRITTEN_K_PER_SECOND);
  if (mem->mdr_is_supported(MMEM_PAGES_SCANNED))
    mdr_support_field(MSYS_PAGES_SCANNED);
  if (mem->mdr_is_supported(MMEM_PAGE_FAULTS))
    mdr_support_field(MSYS_PAGE_FAULTS);
  if (mem->mdr_is_supported(MMEM_MAJOR_FAULTS))
    mdr_support_field(MSYS_MAJOR_FAULTS);

  mdr_explanation_text = _GX_("SysStat_Category");

  msys_user_time_percent = 0;
  msys_kernel_time_percent = 0;
  msys_wait_time_percent = 0;
  msys_idle_time_percent = 0;
  msys_page_K_in_per_second = 0;
  msys_page_K_out_per_second = 0;
  msys_interrupts_per_second = 0;
  msys_context_switches_per_second = 0;
  msys_system_calls_per_second = 0;
  msys_procs_runnable = 0;
  msys_procs_blocked = 0;
  msys_procs_zombied = 0;
  msys_free_memory_percent = 0;
  msys_free_paging_file_percent = 0;
  msys_read_K_per_second = 0;
  msys_written_K_per_second = 0;
  msys_pages_scanned = 0;
  msys_page_faults = 0;
  msys_major_faults = 0;

  last_time.tv_sec = misc->mm_boot_time;
  last_time.tv_usec = 0;

  msys_recompute();
}

mdr_systat_t::~mdr_systat_t()
{
  delete misc;
  delete cpu;
  delete pf;
  delete mem;
  delete proc;
  delete disk;
}

const mdr_value_t &mdr_systat_t::mdr_field_value(int field_number, mdr_value_t &value)
{
  switch(field_number) {
  case MSYS_USER_TIME_PERCENT:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_user_time_percent;
    break;
  case MSYS_KERNEL_TIME_PERCENT:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_kernel_time_percent;
    break;
  case MSYS_WAIT_TIME_PERCENT:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_wait_time_percent;
    break;
  case MSYS_IDLE_TIME_PERCENT:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_idle_time_percent;
    break;
  case MSYS_PAGE_K_IN_PER_SECOND:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_page_K_in_per_second;
    break;
  case MSYS_PAGE_K_OUT_PER_SECOND:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_page_K_out_per_second;
    break;
  case MSYS_INTERRUPTS_PER_SECOND:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_interrupts_per_second;
    break;
  case MSYS_CONTEXT_SWITCHES_PER_SECOND:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_context_switches_per_second;
    break;
  case MSYS_SYSTEM_CALLS_PER_SECOND:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_system_calls_per_second;
    break;
  case MSYS_PROCS_RUNNABLE:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_procs_runnable;
    break;
  case MSYS_PROCS_BLOCKED:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_procs_blocked;
    break;
  case MSYS_PROCS_ZOMBIED:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_procs_zombied;
    break;
  case MSYS_FREE_MEMORY_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = msys_free_memory_percent;
    break;
  case MSYS_FREE_PAGING_FILE_PERCENT:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = msys_free_paging_file_percent;
    break;
  case MSYS_READ_K_PER_SECOND:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_read_K_per_second;
    break;
  case MSYS_WRITTEN_K_PER_SECOND:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_written_K_per_second;
    break;
  case MSYS_PAGES_SCANNED:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_pages_scanned;
    break;
  case MSYS_PAGE_FAULTS:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_page_faults;
    break;
  case MSYS_MAJOR_FAULTS:
    value.mv_type = MVT_I32;
    value.mv_i32 = msys_major_faults;
    break;
  }
  return value;
}

void mdr_systat_t::msys_recompute(void)
{
  uint_t total = 0;
  int free = 0;
  double dtotal;
  mpf_node_t pnode;
  mpf_can_t *pp;
  md_node_t dnode;
  md_can_t *dp;
  ulong_t elapsed;
  double now_usec;
  double then_usec;
  timeval now_tod;
  uint_t read = 0;
  uint_t mrite = 0;

  gettimeofday(&now_tod, 0);
  now_usec = ((double) now_tod.tv_sec * 1000000.0) + now_tod.tv_usec;
  then_usec =
    ((double) last_time.tv_sec * 1000000.0) + last_time.tv_usec;
  elapsed = (ulong_t) ((now_usec - then_usec) / 1000000.0);
  last_time = now_tod;

  // no divide-by-zero please
  if (elapsed == 0)
    elapsed = 1;

  msys_user_time_percent   = cpu->mcs_user_time_percent;
  msys_kernel_time_percent = cpu->mcs_kernel_time_percent;
  msys_wait_time_percent   = cpu->mcs_wait_time_percent;
  msys_idle_time_percent   = cpu->mcs_idle_time_percent;

  msys_page_K_in_per_second = mem->mmem_paged_in_K / elapsed;
  msys_page_K_out_per_second = mem->mmem_paged_out_K / elapsed;

  msys_interrupts_per_second       = misc->mm_interrupts / elapsed;
  msys_context_switches_per_second = misc->mm_context_switches / elapsed;
  msys_system_calls_per_second     = misc->mm_system_calls / elapsed;

  msys_procs_runnable = proc->mps_procs_runnable;
  msys_procs_blocked  = proc->mps_procs_blocked;
  msys_procs_zombied  = proc->mps_procs_zombie;

  dtotal =
    ((double) misc->mm_physical_pages *
      (double) misc->mm_native_page_size) / 1024.0;
  msys_free_memory_percent = (mem->mmem_free_memory_K * 100.0) / dtotal;

  total = 0;
  for(pnode=pf->begin(); pnode != pf->end(); pnode++) {
    pp = &pnode->second;
    total += pp->mpf_size_K;
    free  += pp->mpf_free_K;
  }
  if (total == 0)
    msys_free_paging_file_percent = 100.0;
  else
    msys_free_paging_file_percent = (free * 100) / total;

  const char *physical_disk = "physical";

  for(dnode=disk->begin(); dnode != disk->end(); dnode++) {
    dp = &dnode->second;
    if (dp->md_type != physical_disk)
      continue;
    read  += dp->md_read_K;
    mrite += dp->md_written_K;
  }
  msys_read_K_per_second = read / elapsed;
  msys_written_K_per_second = mrite / elapsed;
  msys_pages_scanned = mem->mmem_pages_scanned;
  msys_page_faults = mem->mmem_page_faults;
  msys_major_faults = mem->mmem_major_faults;
}

void mdr_systat_t::mdr_refresh(void)
{
  misc->mdr_refresh_rates();
  cpu->mdr_refresh();
  pf->mdr_refresh_rates();
  mem->mdr_refresh_rates();
  proc->mdr_refresh_rates();
  disk->mdr_refresh_rates();

  msys_recompute();
}

}
