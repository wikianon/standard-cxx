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


#ifndef _MDR_PF_H_
#define _MDR_PF_H_

#include "mdr.h"

namespace mdr {

const int MPF_VERSION = 1;

const int MPF_SIZE_K      = 0;
const int MPF_FREE_K      = 1;
const int MPF_USED_K      = 2;
const int MPF_FIELD_COUNT = 3;

class mpf_can_t {
public:
  uint_t mpf_size_K;       // KBytes Space Total
  uint_t mpf_free_K;       // KBytes Space Free
  uint_t mpf_used_K;       // KBytes Space Used

  mpf_can_t()
  {
    clear();
  }

  void clear(void)
  {
    mpf_size_K = 0;
    mpf_free_K = 0;
    mpf_used_K = 0;
  }
};

typedef std::map<std::string, mpf_can_t> mpf_box_t;
typedef mpf_box_t::iterator mpf_node_t;

class mdr_pf_t : public mdr_t, public mpf_box_t {
public:
  // constructors/destructors
  mdr_pf_t(const char *remote_host = 0);
 ~mdr_pf_t();

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
  mpf_box_t    old;

  void pf_remote_refresh(void);
};

} // end of namespace mdr

#endif
