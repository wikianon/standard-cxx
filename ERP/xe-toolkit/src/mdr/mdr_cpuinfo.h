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


#ifndef _MDR_CPUINFO_H_
#define _MDR_CPUINFO_H_

#include "mdr.h"

namespace mdr {

const int MCI_VERSION = 1;

const int MCI_INSTANCE     = 0;
const int MCI_ARCHITECTURE = 1;
const int MCI_MODEL        = 2;
const int MCI_CLOCK_MHZ    = 3;
const int MCI_FIELD_COUNT  = 4;

class mci_can_t {
public:
  int         mci_instance;      // CPU Instance Number
  std::string mci_architecture;  // CPU Architecture
  std::string mci_model;         // CPU Model Name
  int         mci_clock_MHz;     // CPU Speed in Megahertz

  mci_can_t()
  {
    clear();
  }

  void clear(void)
  {
    mci_instance = 0;
    mci_architecture.clear();
    mci_model.clear();
    mci_clock_MHz = 0;
  }
};

typedef std::map<std::string, mci_can_t> mci_box_t;
typedef mci_box_t::iterator mci_node_t;

class mdr_cpuinfo_t : public mdr_t, public mci_box_t {
public:
  // constructors/destructors
  mdr_cpuinfo_t(const char *remote_host = 0);
 ~mdr_cpuinfo_t();

  // updating
  void mdr_refresh(void);
  void mdr_rates(void);

  // other
  const mdr_value_t &mdr_field_value(int field_number, mdr_value_t &value)
  {
    return mdr_t::mdr_field_value(field_number, value);
  }
  const mdr_value_t &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value);
  const mdr_keyset_t &mdr_keyset(mdr_keyset_t &keyset);

private:
  mci_box_t old;

  void cpuinfo_remote_refresh(void);
};

} // end of namespace mdr

#endif
