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


#ifndef _MDR_CPUSTAT_H_
#define _MDR_CPUSTAT_H_

#include "mdr.h"
#include "mdr_mpstat.h"

namespace mdr {

const int MCS_USER_TIME_PERCENT   = 0;
const int MCS_KERNEL_TIME_PERCENT = 1;
const int MCS_WAIT_TIME_PERCENT   = 2;
const int MCS_IDLE_TIME_PERCENT   = 3;
const int MCS_FIELD_COUNT         = 4;

class mdr_cpustat_t : public mdr_t {
public:
  int mcs_user_time_percent;     // Percent User Time
  int mcs_kernel_time_percent;   // Percent Kernel Time
  int mcs_wait_time_percent;     // Percent Wait Time
  int mcs_idle_time_percent;     // Percent Idle Time

  // constructors/destructors
  mdr_cpustat_t(const char *remote_host = 0);
 ~mdr_cpustat_t();

  // updating
  void mdr_refresh(void);
  void mdr_refresh_rates(void)
  {
    mdr_cpustat_t::mdr_refresh();
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
  mdr_mpstat_t *mpstat;

  void mcs_recompute(void);
};

} // end of namespace mdr

#endif
