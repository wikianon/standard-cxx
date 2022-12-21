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


#include "mdr_misc.h"

namespace mdr {

mdr_misc_t::mdr_misc_t(const char *remote_host) :
  mdr_t(MDR_MISC_NAME, true, MM_FIELD_COUNT, true)
{
  if (remote_host) {
    net_client = new mdr_client_t("mdr_misc_t", remote_host, MM_VERSION);
    mdr_category_supported = net_client->mc_category_supported;
    mdr_support_mask = net_client->mc_support_mask;
  } else {
    mdr_support_field(MM_HOSTNAME);
    mdr_support_field(MM_OS_PLATFORM);
    mdr_support_field(MM_OS_VERSION);
    mdr_support_field(MM_PHYSICAL_PAGES);
    mdr_support_field(MM_NATIVE_PAGE_SIZE);
    mdr_support_field(MM_PROCESSOR_ARCH);
    mdr_support_field(MM_PROCESSORS_CONFIGURED);
    mdr_support_field(MM_PROCESSORS_ONLINE);
    mdr_support_field(MM_TICKS_PER_SECOND);
    mdr_support_field(MM_BOOT_TIME);
    mdr_support_field(MM_INTERRUPTS);
    mdr_support_field(MM_CLOCK_INTERRUPTS);
#if !defined(_LINUX)
    mdr_support_field(MM_SYSTEM_CALLS);
#endif
    mdr_support_field(MM_CONTEXT_SWITCHES);
    mdr_support_field(MM_PROCESS_COUNT);
    mdr_support_field(MM_1MIN_RUN_QUEUE);
    mdr_support_field(MM_5MIN_RUN_QUEUE);
    mdr_support_field(MM_15MIN_RUN_QUEUE);
    mdr_support_field(MM_CURRENT_TIME);

    net_client = 0;
  }

  mdr_field_names[MM_HOSTNAME]              = "Host Name";
  mdr_field_names[MM_OS_PLATFORM]           = "OS Name";
  mdr_field_names[MM_OS_VERSION]            = "OS Version";
  mdr_field_names[MM_PHYSICAL_PAGES]        = "Physical Pages";
  mdr_field_names[MM_NATIVE_PAGE_SIZE]      = "Page Size";
  mdr_field_names[MM_PROCESSOR_ARCH]        = "CPU Architecture";
  mdr_field_names[MM_PROCESSORS_CONFIGURED] = "CPUs Configured";
  mdr_field_names[MM_PROCESSORS_ONLINE]     = "CPUs On-line";
  mdr_field_names[MM_TICKS_PER_SECOND]      = "Clock Ticks/sec";
  mdr_field_names[MM_BOOT_TIME]             = "Boot Time";
  mdr_field_names[MM_INTERRUPTS]            = "Total Interrupts";
  mdr_field_names[MM_CLOCK_INTERRUPTS]      = "Clock Interrupts";
  mdr_field_names[MM_SYSTEM_CALLS]          = "System Calls";
  mdr_field_names[MM_CONTEXT_SWITCHES]      = "Context Switches";
  mdr_field_names[MM_PROCESS_COUNT]         = "Process Count";
  mdr_field_names[MM_1MIN_RUN_QUEUE]        = "1 Minute Run Queue Length";
  mdr_field_names[MM_5MIN_RUN_QUEUE]        = "5 Minute Run Queue Length";
  mdr_field_names[MM_15MIN_RUN_QUEUE]       = "15 Minute Run Queue Length";
  mdr_field_names[MM_CURRENT_TIME]          = "Current Time";

  // sync localized names
  mdr_map_names();

  mdr_explanation_text = _GX_("Misc_Category");

  mm_hostname.clear();
  mm_os_platform.clear();
  mm_os_version.clear();
  mm_physical_pages = 0;
  mm_native_page_size = 0;
  mm_processor_arch.clear();
  mm_processors_configured = 0;
  mm_processors_online = 0;
  mm_ticks_per_second = 0;
  mm_boot_time = 0;
  mm_interrupts = 0;
  mm_clock_interrupts = 0;
  mm_system_calls = 0;
  mm_context_switches = 0;
  mm_process_count = 0;
  mm_1min_run_queue = 0.0;
  mm_5min_run_queue = 0.0;
  mm_15min_run_queue = 0.0;
  mm_current_time = 0;

  old_interrupts = 0;
  old_clock_interrupts = 0;
  old_system_calls = 0;
  old_context_switches = 0;

  mdr_refresh_rates();

  // tickless kernel
  if (mm_clock_interrupts == 0)
    mdr_unsupport_field(MM_CLOCK_INTERRUPTS);
}

mdr_misc_t::~mdr_misc_t()
{
  if (net_client)
    delete net_client;
}

const mdr_value_t &mdr_misc_t::mdr_field_value(int field_number, mdr_value_t &value)
{
  switch(field_number) {
  case MM_HOSTNAME:
    value.mv_type = MVT_STRING;
    value.mv_s = &mm_hostname;
    break;
  case MM_OS_PLATFORM:
    value.mv_type = MVT_STRING;
    value.mv_s = &mm_os_platform;
    break;
  case MM_OS_VERSION:
    value.mv_type = MVT_STRING;
    value.mv_s = &mm_os_version;
    break;
  case MM_PHYSICAL_PAGES:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mm_physical_pages;
    break;
  case MM_NATIVE_PAGE_SIZE:
    value.mv_type = MVT_I32;
    value.mv_i32 = mm_native_page_size;
    break;
  case MM_PROCESSOR_ARCH:
    value.mv_type = MVT_STRING;
    value.mv_s = &mm_processor_arch;
    break;
  case MM_PROCESSORS_CONFIGURED:
    value.mv_type = MVT_I32;
    value.mv_i32 = mm_processors_configured;
    break;
  case MM_PROCESSORS_ONLINE:
    value.mv_type = MVT_I32;
    value.mv_i32 = mm_processors_online;
    break;
  case MM_TICKS_PER_SECOND:
    value.mv_type = MVT_I32;
    value.mv_i32 = mm_ticks_per_second;
    break;
  case MM_BOOT_TIME:
    value.mv_type = MVT_LONG;
    value.mv_l = mm_boot_time;
    break;
  case MM_INTERRUPTS:
    value.mv_type = MVT_ULONG;
    value.mv_ul = mm_interrupts;
    break;
  case MM_CLOCK_INTERRUPTS:
    value.mv_type = MVT_ULONG;
    value.mv_ul = mm_clock_interrupts;
    break;
  case MM_SYSTEM_CALLS:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mm_system_calls;
    break;
  case MM_CONTEXT_SWITCHES:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mm_context_switches;
    break;
  case MM_PROCESS_COUNT:
    value.mv_type = MVT_UI32;
    value.mv_ui32 = mm_process_count;
    break;
  case MM_1MIN_RUN_QUEUE:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = mm_1min_run_queue;
    break;
  case MM_5MIN_RUN_QUEUE:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = mm_5min_run_queue;
    break;
  case MM_15MIN_RUN_QUEUE:
    value.mv_type = MVT_DOUBLE;
    value.mv_d = mm_15min_run_queue;
    break;
  case MM_CURRENT_TIME:
    value.mv_type = MVT_LONG;
    value.mv_l = mm_current_time;
    break;
  }

  return value;
}

void mdr_misc_t::mdr_rates(void)
{
  ulong_t tmp_interrupts = mm_interrupts;
  ulong_t tmp_clock_interrupts = mm_clock_interrupts;
  uint_t  tmp_system_calls = mm_system_calls;
  uint_t  tmp_context_switches = mm_context_switches;

#ifdef _LP64
  mm_interrupts       -= old_interrupts;
  mm_clock_interrupts -= old_clock_interrupts;
#else
  mm_interrupts       = wrap_diff_32(mm_interrupts, old_interrupts);
  mm_clock_interrupts = wrap_diff_32(mm_clock_interrupts, old_clock_interrupts);
#endif
  mm_system_calls     = wrap_diff_32(mm_system_calls, old_system_calls);
  mm_context_switches = wrap_diff_32(mm_context_switches, old_context_switches);

  old_interrupts = tmp_interrupts;
  old_clock_interrupts = tmp_clock_interrupts;
  old_system_calls = tmp_system_calls;
  old_context_switches = tmp_context_switches;
}

void mdr_misc_t::misc_remote_refresh(void)
{
  mdr_s_mm_t s_map;
  int i;
  int n;

  net_client->mc_refresh(s_map);
  for(i=0, n=s_map.size(); i<n; i++) {
    switch(i) {
    case MM_HOSTNAME:
      mm_hostname = *s_map[i].mv_s;
      break;
    case MM_OS_PLATFORM:
      mm_os_platform = *s_map[i].mv_s;
      break;
    case MM_OS_VERSION:
      mm_os_version = *s_map[i].mv_s;
      break;
    case MM_PHYSICAL_PAGES:
      mm_physical_pages = s_map[i].mv_ui32;
      break;
    case MM_NATIVE_PAGE_SIZE:
      mm_native_page_size = s_map[i].mv_i32;
      break;
    case MM_PROCESSOR_ARCH:
      mm_processor_arch = *s_map[i].mv_s;
      break;
    case MM_PROCESSORS_CONFIGURED:
      mm_processors_configured = s_map[i].mv_i32;
      break;
    case MM_PROCESSORS_ONLINE:
      mm_processors_online = s_map[i].mv_i32;
      break;
    case MM_TICKS_PER_SECOND:
      mm_ticks_per_second = s_map[i].mv_i32;
      break;
    case MM_BOOT_TIME:
      switch(s_map[i].mv_type) {
      case MVT_I32:
        mm_boot_time = s_map[i].mv_i32;
        break;
      case MVT_I64:
        mm_boot_time = s_map[i].mv_i64;
        break;
      default:
        mm_boot_time = -1;
        break;
      }
      break;
    case MM_INTERRUPTS:
      switch(s_map[i].mv_type) {
      case MVT_UI32:
        mm_interrupts = s_map[i].mv_ui32;
        break;
      case MVT_UI64:
        mm_interrupts = s_map[i].mv_ui64;
        break;
      default:
        mm_interrupts = (ulong_t) -1;
        break;
      }
      break;
    case MM_CLOCK_INTERRUPTS:
      switch(s_map[i].mv_type) {
      case MVT_UI32:
        mm_clock_interrupts = s_map[i].mv_ui32;
        break;
      case MVT_UI64:
        mm_clock_interrupts = s_map[i].mv_ui64;
        break;
      default:
        mm_clock_interrupts = (ulong_t) -1;
        break;
      }
      break;
    case MM_SYSTEM_CALLS:
      mm_system_calls = s_map[i].mv_ui32;
      break;
    case MM_CONTEXT_SWITCHES:
      mm_context_switches = s_map[i].mv_ui32;
      break;
    case MM_PROCESS_COUNT:
      mm_process_count = s_map[i].mv_ui32;
      break;
    case MM_1MIN_RUN_QUEUE:
      mm_1min_run_queue = s_map[i].mv_d;
      break;
    case MM_5MIN_RUN_QUEUE:
      mm_5min_run_queue = s_map[i].mv_d;
      break;
    case MM_15MIN_RUN_QUEUE:
      mm_15min_run_queue = s_map[i].mv_d;
      break;
    case MM_CURRENT_TIME:
      switch(s_map[i].mv_type) {
      case MVT_I32:
        mm_current_time = s_map[i].mv_i32;
        break;
      case MVT_I64:
        mm_current_time = s_map[i].mv_i64;
        break;
      default:
        mm_current_time = -1;
        break;
      }
      break;
    default:
      break;
    }
  }

  net_client->mc_release(s_map);
}

}
