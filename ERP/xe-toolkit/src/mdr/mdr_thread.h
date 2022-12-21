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


#ifndef _MDR_THREAD_H_
#define _MDR_THREAD_H_

#include "mdr.h"
#include "mdr_misc.h"
#if defined(_FREEBSD)
# include <sys/param.h>
# include <sys/user.h>
#endif
#if defined(_AIX)
# include <procinfo.h>
#endif

namespace mdr {

const int MT_VERSION = 2;

const int MT_THREAD_ID           = 0;
const int MT_CPU_TIME            = 1;
const int MT_USR_TIME            = 2;
const int MT_KERNEL_TIME         = 3;
const int MT_USR_TIME_PERCENT    = 4;
const int MT_KERNEL_TIME_PERCENT = 5;
const int MT_SIZE_K              = 6;
const int MT_RSSIZE_K            = 7;
const int MT_IO_K                = 8;
const int MT_CPU_PERCENT         = 9;
const int MT_MEMORY_PERCENT      = 10;
const int MT_VOLUNTARY_CSW       = 11;
const int MT_INVOLUNTARY_CSW     = 12;
const int MT_DISK_FAULTS         = 13;
const int MT_PAGE_RECLAIMS       = 14;
const int MT_STATE               = 15;
const int MT_PRIORITY            = 16;
const int MT_NICE                = 17;
const int MT_SIZE_DELTA_K        = 18;
const int MT_RSSIZE_DELTA_K      = 19;
const int MT_PROCESS_ID          = 20;
const int MT_FIELD_COUNT         = 21;

#if defined(_FREEBSD)
typedef struct kinfo_proc kinfo_proc_t;
#endif

class mt_can_t {
public:
  int     mt_thread_id;           // Thread ID
  double  mt_cpu_time;            // Total CPU Time
  double  mt_user_time;           // User CPU Time
  double  mt_kernel_time;         // Kernel CPU Time
  double  mt_user_time_percent;   // Percent User Time
  double  mt_kernel_time_percent; // Percent Kernel Time
  ulong_t mt_size_K;              // Total Size
  ulong_t mt_rssize_K;            // Resident Size
  ulong_t mt_io_K;                // Total I/O
  double  mt_cpu_percent;         // Percent CPU Use
  double  mt_memory_percent;      // Percent Physmem Use
  uint_t  mt_voluntary_csw;       // Voluntary Context Switches
  uint_t  mt_involuntary_csw;     // Involuntary Context Switches
  uint_t  mt_disk_faults;         // Major Page Faults
  uint_t  mt_page_reclaims;       // Minor Page Faults
  int     mt_state;               // Thread State
  int     mt_priority;            // Global Priority
  int     mt_nice;                // Unix Nice Value
  ulong_t mt_size_delta_K;        // Total Size Delta
  ulong_t mt_rssize_delta_K;      // Resident Size Delta
  pid_t   mt_process_id;          // Id of Encapsulating Process

  mt_can_t()
  {
    clear();
  }

  void clear(void)
  {
    mt_thread_id = 0;
    mt_cpu_time = 0;
    mt_user_time = 0;
    mt_kernel_time = 0;
    mt_user_time_percent = 0;
    mt_kernel_time_percent = 0;
    mt_size_K = 0;
    mt_rssize_K = 0;
    mt_io_K = 0;
    mt_cpu_percent = 0;
    mt_memory_percent = 0;
    mt_voluntary_csw = 0;
    mt_involuntary_csw = 0;
    mt_disk_faults = 0;
    mt_page_reclaims = 0;
    mt_state = 0;
    mt_priority = 0;
    mt_nice = 0;
    mt_size_delta_K = 0;
    mt_rssize_delta_K = 0;
    mt_process_id = 0;
  }

private:
  friend class mdr_thread_t;      // allow thread access
};

typedef std::map<std::string, mt_can_t> mt_box_t;
typedef mt_box_t::iterator mt_node_t;

class mdr_thread_t : public mdr_t, public mt_box_t {
public:
  enum mt_sort_key_t {
      BY_THREAD_ID=0,
      BY_CPU_TIME=1,
      BY_USER_TIME=2,
      BY_KERNEL_TIME=3,
      BY_CPU_USE=4,
      BY_MEM_USE=5,
      BY_SIZE=6,
      BY_RSSIZE=7,
      BY_IO=8,
      BY_VOLUNTARY_CSW=9,
      BY_INVOLUNTARY_CSW=10,
      BY_DISK_FAULTS=11,
      BY_PAGE_RECLAIMS=12,
      BY_SIZE_DELTA=13,
      BY_RSSIZE_DELTA=14
  };

  // constructors/destructors
  mdr_thread_t(const char *remote_host = 0);
 ~mdr_thread_t();

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

  void        mt_sort(mt_sort_key_t key, bool reverse = false);
  void        mt_rewind(void);

  // return in sorted order, mithout name
  bool        mt_next_sorted_thread(mt_can_t &can);

private:
  mt_box_t     old;

  ulong_t       mt_total_mem_K;
  mt_sort_key_t mt_sort_key;
  bool          mt_reverse_sort;
  mt_can_t     *mt_sort_array;
  int           mt_sort_array_max;
  int           mt_sort_array_size;
  int           mt_sort_index;
  mdr_misc_t   *mt_misc;
  timeval       mt_last_time;

#if defined(_FREEBSD)
  kinfo_proc_t *mt_array;
  int           mt_array_max;
#endif

#if defined(_AIX)
  thrdentry64  *mt_array;
  int           mt_array_max;
  procentry64 *mps_array;
  int          mps_array_max;
#endif

  void          thread_remote_refresh(void);
};

} // end of namespace mdr

#endif
