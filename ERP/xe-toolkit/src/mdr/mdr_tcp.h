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


#ifndef _MDR_TCP_H_
#define _MDR_TCP_H_

#include "mdr.h"

#ifdef _AIX
# include <mib/snmp_tcp.h>
#endif

namespace mdr {

const int MTCP_VERSION = 1;

const int MTCP_TCPRTOALGORITHM = 0;
const int MTCP_TCPRTOMIN       = 1;
const int MTCP_TCPRTOMAX       = 2;
const int MTCP_TCPMAXCONN      = 3;
const int MTCP_TCPACTIVEOPENS  = 4;
const int MTCP_TCPPASSIVEOPENS = 5;
const int MTCP_TCPATTEMPTFAILS = 6;
const int MTCP_TCPESTABRESETS  = 7;
const int MTCP_TCPCURRESTAB    = 8;
const int MTCP_TCPINSEGS       = 9;
const int MTCP_TCPOUTSEGS      = 10;
const int MTCP_TCPRETRANSSEGS  = 11;
const int MTCP_TCPINERRS       = 12;
const int MTCP_TCPOUTRSTS      = 13;
const int MTCP_FIELD_COUNT     = 14;

class mdr_tcp_t : public mdr_t {
public:
  int    mtcp_tcpRtoAlgorithm; // Retransmit Timeout Algorithm
  int    mtcp_tcpRtoMin;       // Minimum Retransmission Timeout
  int    mtcp_tcpRtoMax;       // Maximum Retransmission Timeout
  int    mtcp_tcpMaxConn;      // Maximum Connection Count
  uint_t mtcp_tcpActiveOpens;  // Active Opens
  uint_t mtcp_tcpPassiveOpens; // Passive Opens
  uint_t mtcp_tcpAttemptFails; // Passive Open Failures
  uint_t mtcp_tcpEstabResets;  // Connection Resets
  uint_t mtcp_tcpCurrEstab;    // Established Connections
  uint_t mtcp_tcpInSegs;       // Segments Received
  uint_t mtcp_tcpOutSegs;      // Segments Transmitted
  uint_t mtcp_tcpRetransSegs;  // Segments Retransmitted
  uint_t mtcp_tcpInErrs;       // Segments Received with Error
  uint_t mtcp_tcpOutRsts;      // RST Segments Transmitted

  // constructors/destructors
  mdr_tcp_t(const char *remote_host = 0);
 ~mdr_tcp_t();

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
  mib::snmp_mib2_tcp_t mib2_tcp;
#endif

  uint_t old_tcpActiveOpens;
  uint_t old_tcpPassiveOpens;
  uint_t old_tcpAttemptFails;
  uint_t old_tcpEstabResets;
  uint_t old_tcpCurrEstab;
  uint_t old_tcpInSegs;
  uint_t old_tcpOutSegs;
  uint_t old_tcpRetransSegs;
  uint_t old_tcpInErrs;
  uint_t old_tcpOutRsts;

  void tcp_remote_refresh(void);
};

} // end of namespace mdr

#endif
