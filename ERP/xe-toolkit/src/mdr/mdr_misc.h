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


#ifndef _MDR_MISC_H_
#define _MDR_MISC_H_

#include "mdr.h"

namespace mdr {

// version 1
const int MM_VERSION = 1;

const int MM_HOSTNAME              = 0;
const int MM_OS_PLATFORM           = 1;
const int MM_OS_VERSION            = 2;
const int MM_PHYSICAL_PAGES        = 3;
const int MM_NATIVE_PAGE_SIZE      = 4;
const int MM_PROCESSOR_ARCH        = 5;
const int MM_PROCESSORS_CONFIGURED = 6;
const int MM_PROCESSORS_ONLINE     = 7;
const int MM_TICKS_PER_SECOND      = 8;
const int MM_BOOT_TIME             = 9;
const int MM_INTERRUPTS            = 10;
const int MM_CLOCK_INTERRUPTS      = 11;
const int MM_SYSTEM_CALLS          = 12;
const int MM_CONTEXT_SWITCHES      = 13;
const int MM_PROCESS_COUNT         = 14;
const int MM_1MIN_RUN_QUEUE        = 15;
const int MM_5MIN_RUN_QUEUE        = 16;
const int MM_15MIN_RUN_QUEUE       = 17;
const int MM_CURRENT_TIME          = 18;
const int MM_FIELD_COUNT           = 19;

class mdr_misc_t : public mdr_t {
public:
  // absolutes
  std::string mm_hostname;              // Name of the host
  std::string mm_os_platform;           // OS name
  std::string mm_os_version;            // OS version
  uint_t      mm_physical_pages;        // Number of pages of physical memory
  int         mm_native_page_size;      // Native physical page size
  std::string mm_processor_arch;        // CPU Architecture
  int         mm_processors_configured; // Number of processors configured
  int         mm_processors_online;     // Number of processors on-line
  int         mm_ticks_per_second;      // Clock ticks per second
  long        mm_boot_time;             // Boot time

  // rates
  ulong_t     mm_interrupts;            // Total interrupts received since boot
  ulong_t     mm_clock_interrupts;      // Clock interrupts received since boot
  uint_t      mm_system_calls;          // System calls
  uint_t      mm_context_switches;      // Context switches

  // variants
  uint_t      mm_process_count;         // Number of processes
  double      mm_1min_run_queue;        // Average run queue length, last 1 min
  double      mm_5min_run_queue;        // Average run queue length, last 5 min
  double      mm_15min_run_queue;       // Average run queue length, last 15 min
  long        mm_current_time;          // Current Time

  // constructors/destructors
  mdr_misc_t(const char *remote_host = 0);
 ~mdr_misc_t();

  // updating
  void mdr_refresh(void);
  void mdr_rates(void);

  // other
  const mdr_value_t &mdr_field_value(int field_number, mdr_value_t &value);
  const mdr_value_t &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value)
  {
    return mdr_t::mdr_field_value(key, field_number, value);
  }

private:
  ulong_t old_interrupts;
  ulong_t old_clock_interrupts;
  uint_t  old_system_calls;
  uint_t  old_context_switches;

  void misc_remote_refresh(void);
};

} // end of namespace mdr

#endif
