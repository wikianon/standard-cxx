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


#ifndef _MDR_CACHE_H_
#define _MDR_CACHE_H_

#include "mdr.h"

namespace mdr {

const int MC_VERSION = 1;

const int MC_HITS        = 0;
const int MC_MISSES      = 1;
const int MC_HIT_PERCENT = 2;
const int MC_FIELD_COUNT = 3;

class mc_can_t {
public:
  uint_t mc_hits;         // Cache Hits
  uint_t mc_misses;       // Cache Misses
  double mc_hit_percent;  // Hit Percentage

  mc_can_t()
  {
    clear();
  }

  void clear(void)
  {
    mc_hits = 0;
    mc_misses = 0;
    mc_hit_percent = 0;
  }
};

typedef std::map<std::string, mc_can_t> mc_box_t;
typedef mc_box_t::iterator mc_node_t;

class mdr_cache_t : public mdr_t, public mc_box_t {
public:
  // constructors/destructors
  mdr_cache_t(const char *remote_host = 0);
 ~mdr_cache_t();

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
  mc_box_t     old;

  void cache_remote_refresh(void);
};

} // end of namespace mdr

#endif
