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


#ifndef _MDR_PROC_STATES_H_
#define _MDR_PROC_STATES_H_

#include "mdr_ps.h"

const int MPST_VERSION = 1;

const int MPST_RUNNABLE    = 0;
const int MPST_SLEEPING    = 1;
const int MPST_STOPPED     = 2;
const int MPST_BLOCKED     = 3;
const int MPST_ZOMBIE      = 4;
const int MPST_SPECIFIC    = 5;
const int MPST_FIELD_COUNT = 6;

namespace mdr {

class mdr_proc_states_t : public mdr_t {
public:
  int mpst_procs_runnable;
  int mpst_procs_sleeping;
  int mpst_procs_stopped;
  int mpst_procs_blocked;
  int mpst_procs_zombie;
  int mpst_procs_specific;

  // constructors/destructors
  mdr_proc_states_t(const char *remote_host = 0);
 ~mdr_proc_states_t();

  // updating
  void mdr_refresh(void);
  void mdr_rates(void)
  {
    // all absolute values
  }

  // other
  const mdr_value_t &mdr_field_value(int field_number, mdr_value_t &value);
  const mdr_value_t &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value)
  {
    return mdr_t::mdr_field_value(key, field_number, value);
  }
private:
  mdr_ps_t ps;        // which collects these values

  void proc_states_remote_refresh(void);
};

}

#endif
