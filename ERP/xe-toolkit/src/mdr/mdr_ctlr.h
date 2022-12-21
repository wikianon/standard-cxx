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


#ifndef _MDR_CTLR_H_
#define _MDR_CTLR_H_

#include "mdr.h"
#include "mdr_disk.h"

namespace mdr {

const int MCTL_VERSION = 2;

const int MCTL_READS            = 0;
const int MCTL_READ_K           = 1;
const int MCTL_READ_MSECS       = 2;
const int MCTL_WRITES           = 3;
const int MCTL_WRITTEN_K        = 4;
const int MCTL_WRITE_MSECS      = 5;
const int MCTL_RUN_MSECS        = 6;
const int MCTL_WAIT_MSECS       = 7;
const int MCTL_RUNQ_MSECS       = 8;
const int MCTL_WAITQ_MSECS      = 9;
const int MCTL_AVG_QUEUE_LENGTH = 10;
const int MCTL_UTIL_PERCENT     = 11;
const int MCTL_SERVICE_TIME     = 12;
const int MCTL_DISK_COUNT       = 13;
const int MCTL_IO_OPERATIONS    = 14;
const int MCTL_FIELD_COUNT      = 15;

class mctl_can_t {
public:
  uint_t mctl_reads;            // Read Operations
  uint_t mctl_read_K;           // KBytes Read
  uint_t mctl_read_msecs;       // Read Time
  uint_t mctl_writes;           // Write Operations
  uint_t mctl_written_K;        // KBytes Written
  uint_t mctl_write_msecs;      // Write Time
  uint_t mctl_run_msecs;        // Run Time
  uint_t mctl_wait_msecs;       // Wait Time
  uint_t mctl_runq_msecs;       // Q-Length*Run-Time
  uint_t mctl_waitq_msecs;      // Q-Length*Wait-Time
  double mctl_avg_queue_length; // Average Request Queue Length
  double mctl_util_percent;     // Percent Time Busy
  double mctl_service_time;     // Average Access Time
  int    mctl_disk_count;       // Disks on Controller
  uint_t mctl_io_operations;    // Total Transfers

  mctl_can_t()
  {
    clear();
  }

  void clear(void)
  {
    mctl_reads = 0;
    mctl_read_K = 0;
    mctl_read_msecs = 0;
    mctl_writes = 0;
    mctl_written_K = 0;
    mctl_write_msecs = 0;
    mctl_run_msecs = 0;
    mctl_wait_msecs = 0;
    mctl_runq_msecs = 0;
    mctl_waitq_msecs = 0;
    mctl_avg_queue_length = 0;
    mctl_util_percent = 0;
    mctl_service_time = 0;
    mctl_disk_count = 0;
    mctl_io_operations = 0;
  }
};

typedef std::map<std::string, mctl_can_t> mctl_box_t;
typedef mctl_box_t::iterator mctl_node_t;

class mdr_ctlr_t : public mdr_t, public mctl_box_t {
public:
  // constructors/destructors
  mdr_ctlr_t(const char *remote_host = 0);
 ~mdr_ctlr_t();

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
  mctl_box_t   old;
  mdr_disk_t  *disk;

  void ctlr_remote_refresh(void);
};

} // end of namespace mdr

#endif
