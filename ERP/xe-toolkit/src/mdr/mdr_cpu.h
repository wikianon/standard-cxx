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


#ifndef _MDR_CPU_H_
#define _MDR_CPU_H_

#include "mdr.h"

namespace mdr {

const int MCPU_VERSION = 1;

const int MCPU_USER_TICKS     = 0;
const int MCPU_KERNEL_TICKS   = 1;
const int MCPU_WAIT_TICKS     = 2;
const int MCPU_NICE_TICKS     = 3;
const int MCPU_IDLE_TICKS     = 4;
const int MCPU_IRQ_TICKS      = 5;
const int MCPU_SOFT_IRQ_TICKS = 6;
const int MCPU_FIELD_COUNT    = 7;

class mcpu_can_t {
public:
  // rates
  uint_t mcpu_user_ticks;          // User CPU Ticks
  uint_t mcpu_kernel_ticks;        // Kernel CPU Ticks
  uint_t mcpu_wait_ticks;          // Wait CPU Ticks
  uint_t mcpu_nice_ticks;          // Nice CPU Ticks
  uint_t mcpu_idle_ticks;          // Idle CPU Ticks
  uint_t mcpu_irq_ticks;           // Hardware Interrupt Ticks
  uint_t mcpu_soft_irq_ticks;      // Software Interrupt Ticks

  mcpu_can_t()
  {
    clear();
  }

  void clear(void)
  {
    mcpu_user_ticks = 0;
    mcpu_kernel_ticks = 0;
    mcpu_wait_ticks = 0;
    mcpu_nice_ticks = 0;
    mcpu_idle_ticks = 0;
    mcpu_irq_ticks = 0;
    mcpu_soft_irq_ticks = 0;
  }
};

typedef std::map<std::string, mcpu_can_t> mcpu_box_t;
typedef mcpu_box_t::iterator mcpu_node_t;

class mdr_cpu_t : public mdr_t, public mcpu_box_t {
public:
  // constructors/destructors
  mdr_cpu_t(const char *remote_host = 0);
 ~mdr_cpu_t();

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
  mcpu_box_t   old;

  void cpu_remote_refresh(void);
};

} // end of namespace mdr

#endif
