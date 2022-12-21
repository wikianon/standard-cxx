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


#ifndef _MDR_NETSTAT_H_
#define _MDR_NETSTAT_H_

#include "mdr.h"
#include "mdr_misc.h"
#include "mdr_netif.h"

namespace mdr {

const int MNS_VERSION = 1;

const int MNS_INPUT_BYTES       = 0;
const int MNS_OUTPUT_BYTES      = 1;
const int MNS_INPUT_PACKETS     = 2;
const int MNS_OUTPUT_PACKETS    = 3;
const int MNS_INPUT_ERRORS      = 4;
const int MNS_OUTPUT_ERRORS     = 5;
const int MNS_INPUT_FAILURES    = 6;
const int MNS_OUTPUT_FAILURES   = 7;
const int MNS_COLLISIONS        = 8;
const int MNS_COLLISION_PERCENT = 9;
const int MNS_FIELD_COUNT       = 10;

class mns_can_t {
public:
  double mns_input_bytes;        // Bytes Received/sec
  double mns_output_bytes;       // Bytes Transmitted/sec
  double mns_input_packets;      // Packets Received/sec
  double mns_output_packets;     // Packets Transmitted/sec
  double mns_input_errors;       // Errors on Receipt/sec
  double mns_output_errors;      // Errors on Transmission/sec
  double mns_input_failures;     // Failures on Receipt/sec
  double mns_output_failures;    // Failures on Transmission/sec
  double mns_collisions;         // Collisions on Transmission/sec
  double mns_collision_percent;  // Collision Percent

public:
  mns_can_t()
  {
    clear();
  }

  void clear(void)
  {
    mns_input_bytes = 0;
    mns_output_bytes = 0;
    mns_input_packets = 0;
    mns_output_packets = 0;
    mns_input_errors = 0;
    mns_output_errors = 0;
    mns_input_failures = 0;
    mns_output_failures = 0;
    mns_collisions = 0;
    mns_collision_percent = 0;
  }
};

typedef std::map<std::string, mns_can_t> mns_box_t;
typedef mns_box_t::iterator mns_node_t;

class mdr_netstat_t : public mdr_t, public mns_box_t {
public:
  // constructors/destructors
  mdr_netstat_t(const char *remote_host = 0);
 ~mdr_netstat_t();

  // updating
  void mdr_refresh(void);
  void mdr_refresh_rates(void)
  {
    mdr_netstat_t::mdr_refresh();
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
  mdr_misc_t  *misc;
  mdr_netif_t *netif;
  timeval      last_time;

  void netstat_remote_refresh(void);
  void mns_recompute(void);
};

} // end of namespace mdr

#endif
