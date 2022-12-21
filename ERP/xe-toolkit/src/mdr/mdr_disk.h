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


#ifndef _MDR_DISK_H_
#define _MDR_DISK_H_

#include "mdr.h"
#include "mdr_mnt.h"
#include "mdr_pf.h"

namespace mdr {

const int MD_VERSION = 3;

const int MD_TYPE             = 0;
const int MD_READS            = 1;
const int MD_READ_K           = 2;
const int MD_READ_MSECS       = 3;
const int MD_WRITES           = 4;
const int MD_WRITTEN_K        = 5;
const int MD_WRITE_MSECS      = 6;
const int MD_RUN_MSECS        = 7;
const int MD_WAIT_MSECS       = 8;
const int MD_RUNQ_MSECS       = 9;
const int MD_WAITQ_MSECS      = 10;
const int MD_AVG_QUEUE_LENGTH = 11;
const int MD_UTIL_PERCENT     = 12;
const int MD_SERVICE_TIME     = 13;
const int MD_SPACE_MB         = 14;
const int MD_IO_OPERATIONS    = 15;
const int MD_FIELD_COUNT      = 16;

class md_can_t {
public:
  std::string md_type;             // Disk Type
  uint_t      md_reads;            // Read Operations
  uint_t      md_read_K;           // KBytes Read
  uint_t      md_read_msecs;       // Read Time
  uint_t      md_writes;           // Write Operations
  uint_t      md_written_K;        // KBytes Written
  uint_t      md_write_msecs;      // Write Time
  uint_t      md_run_msecs;        // Run Time
  uint_t      md_wait_msecs;       // Wait Time
  uint_t      md_runq_msecs;       // Q-Length*Run-Time
  uint_t      md_waitq_msecs;      // Q-Length*Wait-Time
  double      md_avg_queue_length; // Average Request Queue Length
  double      md_util_percent;     // Percent Time Busy
  double      md_service_time;     // Average Access Time
  uint_t      md_space_mb;         // Megabytes of Capacity
  uint_t      md_io_operations;    // Total Transfers

  md_can_t()
  {
    clear();
  }

  void clear(void)
  {
    md_reads = 0;
    md_read_K = 0;
    md_read_msecs = 0;
    md_writes = 0;
    md_written_K = 0;
    md_write_msecs = 0;
    md_run_msecs = 0;
    md_wait_msecs = 0;
    md_runq_msecs = 0;
    md_waitq_msecs = 0;
    md_avg_queue_length = 0;
    md_util_percent = 0;
    md_service_time = 0;
    md_space_mb = 0;
    md_io_operations = 0;
  }
};

typedef std::map<std::string, md_can_t> md_box_t;
typedef md_box_t::iterator md_node_t;

class mdr_disk_t : public mdr_t, public md_box_t {
public:
  // constructors/destructors
  mdr_disk_t(const char *remote_host = 0);
 ~mdr_disk_t();

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
  mdr_mnt_t    *mounts;
  mdr_pf_t     *swaps;
  md_box_t     old;

  void disk_remote_refresh(void);
};

} // end of namespace mdr

#endif
