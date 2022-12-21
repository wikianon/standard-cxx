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


#ifndef _mdr_ip_H_
#define _mdr_ip_H_

#include "mdr.h"

#ifdef _AIX
# include <mib/snmp_ip.h>
#endif

namespace mdr {

const int MIP_VERSION = 1;

const int MIP_IPFORWARDING      = 0;
const int MIP_IPDEFAULTTTL      = 1;
const int MIP_IPINRECEIVES      = 2;
const int MIP_IPINHDRERRORS     = 3;
const int MIP_IPINADDRERRORS    = 4;
const int MIP_IPFORWDATAGRAMS   = 5;
const int MIP_IPINUNKNOWNPROTOS = 6;
const int MIP_IPINDISCARDS      = 7;
const int MIP_IPINDELIVERS      = 8;
const int MIP_IPOUTREQUESTS     = 9;
const int MIP_IPOUTDISCARDS     = 10;
const int MIP_IPOUTNOROUTES     = 11;
const int MIP_IPREASMTIMEOUT    = 12;
const int MIP_IPREASMREQDS      = 13;
const int MIP_IPREASMOKS        = 14;
const int MIP_IPREASMFAILS      = 15;
const int MIP_IPFRAGOKS         = 16;
const int MIP_IPFRAGFAILS       = 17;
const int MIP_IPFRAGCREATES     = 18;
const int MIP_IPROUTINGDISCARDS = 19;
const int MIP_FIELD_COUNT       = 20;

class mdr_ip_t : public mdr_t {
public:
  int    mip_ipForwarding;      // Forwarder
  int    mip_ipDefaultTTL;      // Time-to-Live
  uint_t mip_ipInReceives;      // Datagrams Received
  uint_t mip_ipInHdrErrors;     // In Discards from Header Errors
  uint_t mip_ipInAddrErrors;    // In Discards from Address Errors
  uint_t mip_ipForwDatagrams;   // Datagrams Forwarded
  uint_t mip_ipInUnknownProtos; // In Discards from Unknown Protocol
  uint_t mip_ipInDiscards;      // In Discards of Good Datagrams
  uint_t mip_ipInDelivers;      // Datagrams Sent Upstream
  uint_t mip_ipOutRequests;     // Upstream Output Requests
  uint_t mip_ipOutDiscards;     // Out Discards of Good Datagrams
  uint_t mip_ipOutNoRoutes;     // Out Discards from No Route
  int    mip_ipReasmTimeout;    // Fragment Reassemble Timeout
  uint_t mip_ipReasmReqds;      // Fragment Reassemblies Required
  uint_t mip_ipReasmOKs;        // Fragments Reassembled
  uint_t mip_ipReasmFails;      // Fragment Reassembly Failures
  uint_t mip_ipFragOKs;         // Datagrams Fragmented
  uint_t mip_ipFragFails;       // Fragment Discards from No-Fragment Flag
  uint_t mip_ipFragCreates;     // Fragments Created
  uint_t mip_ipRoutingDiscards; // Routing Entries Discarded

  // constructors/destructors
  mdr_ip_t(const char *remote_host = 0);
 ~mdr_ip_t();

  // updating
  void mdr_refresh(void);
  void mdr_rates(void);

  // other
  const mdr_value_t &mdr_field_value(int field_number, mdr_value_t &value);
  const mdr_value_t &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value)
  {
    return mdr_t::mdr_field_value(key, field_number, value);
  }

private:
#ifdef _AIX
  bool constructor;
  mib::snmp_mib2_ip_t mib2_ip;
#endif

  uint_t old_ipInReceives;
  uint_t old_ipInHdrErrors;
  uint_t old_ipInAddrErrors;
  uint_t old_ipForwDatagrams;
  uint_t old_ipInUnknownProtos;
  uint_t old_ipInDiscards;
  uint_t old_ipInDelivers;
  uint_t old_ipOutRequests;
  uint_t old_ipOutDiscards;
  uint_t old_ipOutNoRoutes;
  uint_t old_ipReasmReqds;
  uint_t old_ipReasmOKs;
  uint_t old_ipReasmFails;
  uint_t old_ipFragOKs;
  uint_t old_ipFragFails;
  uint_t old_ipFragCreates;
  uint_t old_ipRoutingDiscards;

  void ip_remote_refresh(void);
};

} // end of namespace mdr

#endif
