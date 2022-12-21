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


#ifndef _MDR_MPSTAT_H_
#define _MDR_MPSTAT_H_

#include "mdr.h"
#include "mdr_misc.h"
#include "mdr_mp.h"

namespace mdr {

const int MMPS_USER_TIME_PERCENT   = 0;
const int MMPS_KERNEL_TIME_PERCENT = 1;
const int MMPS_WAIT_TIME_PERCENT   = 2;
const int MMPS_IDLE_TIME_PERCENT   = 3;
const int MMPS_FIELD_COUNT         = 4;

class mmps_can_t {
public:
  int mmps_user_time_percent;       // Percent User Time
  int mmps_kernel_time_percent;     // Percent Kernel Time
  int mmps_wait_time_percent;       // Percent Wait Time
  int mmps_idle_time_percent;       // Percent Idle Time

  mmps_can_t()
  {
    clear();
  }

  void clear(void)
  {
    mmps_user_time_percent = 0;
    mmps_kernel_time_percent = 0;
    mmps_wait_time_percent = 0;
    mmps_idle_time_percent = 0;
  }
};

typedef std::map<std::string, mmps_can_t> mmps_box_t;
typedef mmps_box_t::iterator mmps_node_t;

class mdr_mpstat_t : public mdr_t, public mmps_box_t {
public:
  mdr_mpstat_t(const char *remote_host = 0);
 ~mdr_mpstat_t();

  void mdr_refresh(void);
  void mdr_refresh_rates(void)
  {
    mdr_mpstat_t::mdr_refresh();
    // no rates
  }
  void mdr_rates(void)
  {
    // no rates
  }

  // other
  const mdr_value_t &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value);
  const mdr_value_t &mdr_field_value(int field_number, mdr_value_t &value)
  {
    return mdr_t::mdr_field_value(field_number, value);
  }
  const mdr_keyset_t &mdr_keyset(mdr_keyset_t &keyset);

private:
  mdr_misc_t *misc;
  mdr_mp_t   *mp;

  void mmps_recompute(void);
};

}

#endif
