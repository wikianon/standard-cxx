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


#ifndef _MDR_PS_H_
#define _MDR_PS_H_

#include "mdr.h"
#include "mdr_misc.h"
#if defined(_DARWIN)
# include <sys/sysctl.h>
# if OS_VERSION >= 830
#  include <mach/kern_return.h>
# endif
#endif
#if defined(_FREEBSD)
# include <sys/param.h>
# include <sys/user.h>
#endif
#if defined(_AIX)
# include <procinfo.h>
#endif

namespace mdr {

const int MPS_VERSION = 4;

// per-process field ids
const int MPS_START               = 0;
const int MPS_UID                 = 1;
const int MPS_EUID                = 2;
const int MPS_PID                 = 3;
const int MPS_PPID                = 4;
const int MPS_TTY_DEVICE          = 5;
const int MPS_FILE_NAME           = 6;
const int MPS_COMMAND             = 7;
const int MPS_FIRST_ARGUMENT      = 8;
const int MPS_CPU_TIME            = 9;
const int MPS_USR_TIME            = 10;
const int MPS_KERNEL_TIME         = 11;
const int MPS_USR_TIME_PERCENT    = 12;
const int MPS_KERNEL_TIME_PERCENT = 13;
const int MPS_SIZE_K              = 14;
const int MPS_RSSIZE_K            = 15;
const int MPS_IO_K                = 16;
const int MPS_CPU_PERCENT         = 17;
const int MPS_MEMORY_PERCENT      = 18;
const int MPS_VOLUNTARY_CSW       = 19;
const int MPS_INVOLUNTARY_CSW     = 20;
const int MPS_DISK_FAULTS         = 21;
const int MPS_PAGE_RECLAIMS       = 22;
const int MPS_STATE               = 23;
const int MPS_PRIORITY            = 24;
const int MPS_NICE                = 25;
const int MPS_THREAD_COUNT        = 26;
const int MPS_SIZE_DELTA_K        = 27;
const int MPS_RSSIZE_DELTA_K      = 28;
const int MPS_USER_NAME           = 29;
const int MPS_EFFECTIVE_USER_NAME = 30;
const int MPS_SYSTEM_CALLS        = 31;
const int MPS_IS_SYSTEM_PROCESS   = 32;
const int MPS_FIELD_COUNT         = 33;

// These fields do not show up as part of mdr_ps_t.
// Since mdr_ps_t is an n-tuple, and these fields
// are ala singleton, they remain hidden. They are
// accessed by mdr_proc_states_t which IS a singleton.

const int MPS_PROCS_RUNNABLE      = 34;
const int MPS_PROCS_SLEEPING      = 35;
const int MPS_PROCS_STOPPED       = 36;
const int MPS_PROCS_BLOCKED       = 37;
const int MPS_PROCS_ZOMBIE        = 38;
const int MPS_PROCS_SPECIFIC      = 39;

// proc states
const int MPS_RUNNABLE    = 0;
const int MPS_SLEEPING    = 1;
const int MPS_STOPPED     = 2;
const int MPS_BLOCKED     = 3;
const int MPS_ZOMBIE      = 4;
const int MPS_SPECIFIC    = 5;
const int MPS_PROC_STATES = 6;

class mps_can_t {
public:
  long        mps_start;               // Start Time
  uid_t       mps_uid;                 // User ID
  uid_t       mps_euid;                // Effective User ID
  pid_t       mps_pid;                 // Process ID
  pid_t       mps_ppid;                // Parent Process ID
  long        mps_tty_device;          // Terminal Device ID
  std::string mps_file_name;           // File Name
  std::string mps_command;             // Full Command
  std::string mps_first_argument;      // First Argument
  double      mps_cpu_time;            // Total CPU Seconds
  double      mps_user_time;           // User CPU Seconds
  double      mps_kernel_time;         // Kernel CPU Seconds
  double      mps_user_time_percent;   // Percent User Time
  double      mps_kernel_time_percent; // Percent Kernel Time
  ulong_t     mps_size_K;              // Total Size
  ulong_t     mps_rssize_K;            // Resident Size
  ulong_t     mps_io_K;                // Total I/O
  double      mps_cpu_percent;         // Percent CPU Use
  double      mps_memory_percent;      // Percent Physmem Use
  uint_t      mps_system_calls;        // System Calls
  uint_t      mps_voluntary_csw;       // Voluntary Context Switches
  uint_t      mps_involuntary_csw;     // Involuntary Context Switches
  uint_t      mps_disk_faults;         // Major Page Faults
  uint_t      mps_page_reclaims;       // Minor Page Faults
  int         mps_state;               // Process State
  int         mps_priority;            // Global Priority
  int         mps_nice;                // Unix Nice Value
  int         mps_thread_count;        // Number of Threads
  ulong_t     mps_size_delta_K;        // Total Size Delta
  ulong_t     mps_rssize_delta_K;      // Resident Size Delta
  std::string mps_user_name;           // User Name
  std::string mps_effective_user_name; // Effective User Name
  bool        mps_is_system_process;   // Is System Process

  mps_can_t()
  {
    clear();
  }

  void clear(void)
  {
    mps_start = 0;
    mps_uid = 0;
    mps_euid = 0;
    mps_pid = 0;
    mps_ppid = 0;
    mps_tty_device = 0;
    mps_file_name.clear();
    mps_command.clear();
    mps_first_argument.clear();
    mps_cpu_time = 0.0;
    mps_user_time = 0.0;
    mps_kernel_time = 0.0;
    mps_user_time_percent = 0.0;
    mps_kernel_time_percent = 0.0;
    mps_size_K = 0;
    mps_rssize_K = 0;
    mps_io_K = 0;
    mps_cpu_percent = 0;
    mps_memory_percent = 0;
    mps_system_calls = 0;
    mps_voluntary_csw = 0;
    mps_involuntary_csw = 0;
    mps_disk_faults = 0;
    mps_page_reclaims = 0;
    mps_state = 0;
    mps_priority = 0;
    mps_nice = 0;
    mps_thread_count = 0;
    mps_size_delta_K = 0;
    mps_rssize_delta_K = 0;
    mps_user_name.clear();
    mps_effective_user_name.clear();
    mps_is_system_process = false;
  }

private:
  friend class mdr_ps_t;               // allow ps access
};

typedef std::map<std::string, mps_can_t> mps_box_t;
typedef mps_box_t::iterator mps_node_t;

typedef std::map<uid_t, std::string> mps_uid_map_t;
typedef mps_uid_map_t::iterator mps_uid_node_t;

#if defined(_DARWIN)
// info for each process pulled from mach
struct mach_info_t {
  uint_t  system_calls;
  uint_t  context_switches;
  uint_t  maj_faults;
  uint_t  min_faults;
  ulong_t size;
  ulong_t rssize;
  int     priority;
  double  user_time;
  double  system_time;
  bool    swapped;
  int     thread_count;
  char    state_char;
};
typedef struct kinfo_proc kinfo_proc_t;
#endif

#if defined(_FREEBSD)
typedef struct kinfo_proc kinfo_proc_t;
#endif

class mdr_ps_t : public mdr_t, public mps_box_t {
public:
  enum mps_sort_key_t {
      BY_PID=0,
      BY_CPU_TIME=1,
      BY_USER_TIME=2,
      BY_KERNEL_TIME=3,
      BY_CPU_USE=4,
      BY_MEM_USE=5,
      BY_SIZE=6,
      BY_RSSIZE=7,
      BY_IO=8,
      BY_UID=9,
      BY_SYSTEM_CALLS=10,
      BY_VOLUNTARY_CSW=11,
      BY_INVOLUNTARY_CSW=12,
      BY_DISK_FAULTS=13,
      BY_PAGE_RECLAIMS=14,
      BY_THREAD_COUNT=15,
      BY_SIZE_DELTA=16,
      BY_RSSIZE_DELTA=17,
      BY_USERNAME=18,
      BY_STATE=19,
      BY_COMMAND=20,
      BY_FILE_NAME=21
  };

#define MPS_FIRST_SORT_KEY  BY_PID
#define MPS_LAST_SORT_KEY   BY_FILE_NAME

  // members
  int mps_procs_runnable;
  int mps_procs_sleeping;
  int mps_procs_stopped;
  int mps_procs_blocked;
  int mps_procs_zombie;
  int mps_procs_specific;

  // constructors/destructors
  mdr_ps_t(const char *remote_host = 0);
 ~mdr_ps_t();

  // updating
  void mdr_refresh(void);
  void mdr_rates(void);

  // other
  const mdr_value_t  &mdr_field_value(int field_number, mdr_value_t &value);
  const mdr_value_t  &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value);
  const mdr_keyset_t &mdr_keyset(mdr_keyset_t &keyset);

  void        mps_sort(mps_sort_key_t key, bool reverse = false);
  void        mps_rewind(void);
  static std::string mps_dev_to_tty(dev_t dev, std::string &result);

  // return in sorted order, mithout name
  bool        mps_next_sorted_proc(mps_can_t &can);

  // lookup mps_can_t by pid
  const bool  mps_lookup_by_pid(pid_t pid, mps_can_t &can);

private:
  mps_box_t      old;
  mps_uid_map_t  uid_map;
  bool           first_pass;

  ulong_t        mps_total_mem_K;
  mps_sort_key_t mps_sort_key;
  bool           mps_reverse_sort;
  mps_can_t     *mps_sort_array;
  int            mps_sort_array_max;
  int            mps_sort_array_size;
  int            mps_sort_index;
  mdr_misc_t    *mps_misc;
  timeval        mps_last_time;

#if defined(_DARWIN) || defined(_FREEBSD)
  kinfo_proc_t  *mps_array;
  int            mps_array_max;
#endif

#if defined(_DARWIN)
  kern_return_t get_task_info(mach_info_t *mi, pid_t pid);
#endif

#if defined(_AIX)
  procentry64  *mps_array;
  int           mps_array_max;
#endif

  void        ps_remote_refresh(void);
  const char *uid_lookup(uid_t uid);
};

} // end of namespace mdr

#endif
