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


#ifndef _MDR_NET_H_
#define _MDR_NET_H_

#include "mdr.h"

#ifdef _AIX
# include <mib/snmp_netif.h>
#endif

namespace mdr {

const int MNET_VERSION = 1;

const int MNET_IP_ADDRESS        = 0;
const int MNET_NETMASK           = 1;
const int MNET_BROADCAST_ADDRESS = 2;
const int MNET_MTU               = 3;
const int MNET_INPUT_BYTES       = 4;
const int MNET_OUTPUT_BYTES      = 5;
const int MNET_INPUT_PACKETS     = 6;
const int MNET_OUTPUT_PACKETS    = 7;
const int MNET_INPUT_ERRORS      = 8;
const int MNET_OUTPUT_ERRORS     = 9;
const int MNET_INPUT_FAILURES    = 10;
const int MNET_OUTPUT_FAILURES   = 11;
const int MNET_COLLISIONS        = 12;
const int MNET_INTERFACE_SPEED   = 13;
const int MNET_COLLISION_PERCENT = 14;
const int MNET_FULL_DUPLEX       = 15;
const int MNET_FIELD_COUNT       = 16;

class mnet_can_t {
public:
  std::string mnet_ip_address;         // IP Address
  std::string mnet_netmask;            // Network Mask
  std::string mnet_broadcast_address;  // Broadcast Address
  int         mnet_mtu;                // Maximum Transfer Unit
  ulong_t     mnet_input_bytes;        // Bytes Received
  ulong_t     mnet_output_bytes;       // Bytes Transmitted
  ulong_t     mnet_input_packets;      // Packets Received
  ulong_t     mnet_output_packets;     // Packets Transmitted
  ulong_t     mnet_input_errors;       // Errors on Receipt
  ulong_t     mnet_output_errors;      // Errors on Transmission
  ulong_t     mnet_input_failures;     // Failures on Receipt
  ulong_t     mnet_output_failures;    // Failures on Transmission
  ulong_t     mnet_collisions;         // Collisions on Transmission
  ulong_t     mnet_interface_speed;    // Interface Speed (b/s)
  double      mnet_collision_percent;  // Collision Percent
  bool        mnet_full_duplex;        // Interface is Full Duplex

public:
  mnet_can_t()
  {
    clear();
  }

  void clear(void)
  {
    mnet_input_bytes = 0;
    mnet_output_bytes = 0;
    mnet_input_packets = 0;
    mnet_output_packets = 0;
    mnet_input_errors = 0;
    mnet_output_errors = 0;
    mnet_input_failures = 0;
    mnet_output_failures = 0;
    mnet_collisions = 0;
    mnet_interface_speed = 0;
    mnet_collision_percent = 0;
    mnet_full_duplex = false;
  }
};

typedef std::map<std::string, mnet_can_t> mnet_box_t;
typedef mnet_box_t::iterator mnet_node_t;

class mdr_netif_t : public mdr_t, public mnet_box_t {
public:
  // constructors/destructors
  mdr_netif_t(const char *remote_host = 0);
 ~mdr_netif_t();

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

  void mnet_duplex(std::string &name, bool is_full);

private:
#ifdef _AIX
  bool constructor;
  mib::snmp_netif_t mib2_netif;
#endif

  mnet_box_t old;

  void netif_remote_refresh(void);
  int get_address_info(const char *name, mnet_can_t *cp);
};

} // end of namespace mdr

#endif
