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


#ifndef _MDR_SYSTAT_H_
#define _MDR_SYSTAT_H_

#include "mdr.h"
#include "mdr_misc.h"
#include "mdr_cpustat.h"
#include "mdr_pf.h"
#include "mdr_memory.h"
#include "mdr_ps.h"
#include "mdr_disk.h"

namespace mdr {

const int MSYS_USER_TIME_PERCENT           = 0;
const int MSYS_KERNEL_TIME_PERCENT         = 1;
const int MSYS_WAIT_TIME_PERCENT           = 2;
const int MSYS_IDLE_TIME_PERCENT           = 3;
const int MSYS_PAGE_K_IN_PER_SECOND        = 4;
const int MSYS_PAGE_K_OUT_PER_SECOND       = 5;
const int MSYS_INTERRUPTS_PER_SECOND       = 6;
const int MSYS_CONTEXT_SWITCHES_PER_SECOND = 7;
const int MSYS_SYSTEM_CALLS_PER_SECOND     = 8;
const int MSYS_PROCS_RUNNABLE              = 9;
const int MSYS_PROCS_BLOCKED               = 10;
const int MSYS_PROCS_ZOMBIED               = 11;
const int MSYS_FREE_MEMORY_PERCENT         = 12;
const int MSYS_FREE_PAGING_FILE_PERCENT    = 13;
const int MSYS_READ_K_PER_SECOND           = 14;
const int MSYS_WRITTEN_K_PER_SECOND        = 15;
const int MSYS_PAGES_SCANNED               = 16;
const int MSYS_PAGE_FAULTS                 = 17;
const int MSYS_MAJOR_FAULTS                = 18;
const int MSYS_FIELD_COUNT                 = 19;

class mdr_systat_t : public mdr_t {
public:
  int    msys_user_time_percent;            // Percent User Time
  int    msys_kernel_time_percent;          // Percent Kernel Time
  int    msys_wait_time_percent;            // Percent Wait Time
  int    msys_idle_time_percent;            // Percent Idle Time
  int    msys_page_K_in_per_second;         // Kbytes Paged in/sec
  int    msys_page_K_out_per_second;        // Kbytes Paged out/sec
  int    msys_interrupts_per_second;        // Interrupts/sec
  int    msys_context_switches_per_second;  // Context Switches/sec
  int    msys_system_calls_per_second;      // System Calls/sec
  int    msys_procs_runnable;               // Runnable Processes
  int    msys_procs_blocked;                // Blocked Processes
  int    msys_procs_zombied;                // Zombie Processes
  double msys_free_memory_percent;          // Percent Memory Free
  double msys_free_paging_file_percent;     // Percent Paging File Free
  int    msys_read_K_per_second;            // KBytes Read/sec
  int    msys_written_K_per_second;         // KBytes Mritten/sec
  int    msys_pages_scanned;                // Pages Scanned
  int    msys_page_faults;                  // Total Page Faults
  int    msys_major_faults;                 // Major Page Faults

  // constructors/destructors
  mdr_systat_t(const char *remote_host = 0);
 ~mdr_systat_t();

  // updating
  void mdr_refresh(void);
  void mdr_refresh_rates(void)
  {
    mdr_refresh();
    // no rates
  }
  void mdr_rates(void)
  {
    // no rates
  }

  // other
  const mdr_value_t &mdr_field_value(int field_number, mdr_value_t &value);
  const mdr_value_t &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value)
  {
    return mdr_t::mdr_field_value(key, field_number, value);
  }

private:
  mdr_misc_t    *misc;
  mdr_cpustat_t *cpu;
  mdr_pf_t      *pf;
  mdr_memory_t  *mem;
  mdr_ps_t      *proc;
  mdr_disk_t    *disk;
  timeval        last_time;

  void msys_recompute(void);
};

} // end of namespace mdr

#endif
