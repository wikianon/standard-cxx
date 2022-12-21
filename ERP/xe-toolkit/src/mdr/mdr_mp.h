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


#ifndef _MMP_MP_H_
#define _MMP_MP_H_

#include "mdr.h"

namespace mdr {

const int MMP_VERSION = 1;

const int MMP_CPU_INSTANCE   = 0;
const int MMP_USER_TICKS     = 1;
const int MMP_KERNEL_TICKS   = 2;
const int MMP_IDLE_TICKS     = 3;
const int MMP_NICE_TICKS     = 4;
const int MMP_WAIT_TICKS     = 5;
const int MMP_IRQ_TICKS      = 6;
const int MMP_SOFT_IRQ_TICKS = 7;
const int MMP_FIELD_COUNT    = 8;

class mmp_can_t {
public:
  int    mmp_cpu_instance;     // CPU Instance Number
  uint_t mmp_user_ticks;       // User CPU Ticks
  uint_t mmp_kernel_ticks;     // Kernel CPU Ticks
  uint_t mmp_idle_ticks;       // Idle CPU Ticks
  uint_t mmp_nice_ticks;       // Nice CPU Ticks
  uint_t mmp_wait_ticks;       // Wait CPU Ticks
  uint_t mmp_irq_ticks;        // Hardware Interrupt Ticks
  uint_t mmp_soft_irq_ticks;   // Software Interrupt Ticks

  mmp_can_t()
  {
    clear();
  }

  void clear(void)
  {
    mmp_cpu_instance = 0;
    mmp_user_ticks = 0;
    mmp_kernel_ticks = 0;
    mmp_idle_ticks = 0;
    mmp_nice_ticks = 0;
    mmp_wait_ticks = 0;
    mmp_irq_ticks = 0;
    mmp_soft_irq_ticks = 0;
  }
};

typedef std::map<std::string, mmp_can_t> mmp_box_t;
typedef mmp_box_t::iterator mmp_node_t;

class mdr_mp_t : public mdr_t, public mmp_box_t {
public:
  // constructors/destructors
  mdr_mp_t(const char *remote_host = 0);
 ~mdr_mp_t();

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
  mmp_box_t    old;

  void mp_remote_refresh(void);
};

} // end of namespace mdr

#endif
