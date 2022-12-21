/****************************************************************************
**
** Copyright (c) 2006-2008 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the mib library of Captivity.
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

#ifndef _SNMP_TCP_H_
#define _SNMP_TCP_H_

#include "snmp_base.h"

namespace mib {

class snmp_mib2_tcp_t : public snmp_base_t {
public:
    int    tcpRtoAlgorithm; // Retransmit Timeout Algorithm
    int    tcpRtoMin;       // Minimum Retransmission Timeout
    int    tcpRtoMax;       // Maximum Retransmission Timeout
    int    tcpMaxConn;      // Maximum Connection Count
    uint_t tcpActiveOpens;  // Active Opens
    uint_t tcpPassiveOpens; // Passive Opens
    uint_t tcpAttemptFails; // Passive Open Failures
    uint_t tcpEstabResets;  // Connection Resets
    uint_t tcpCurrEstab;    // Established Connections
    uint_t tcpInSegs;       // Segments Received
    uint_t tcpOutSegs;      // Segments Transmitted
    uint_t tcpRetransSegs;  // Segments Retransmitted
    uint_t tcpInErrs;       // Segments Received with Error
    uint_t tcpOutRsts;      // RST Segments Transmitted

    snmp_mib2_tcp_t(const char *hostname = "localhost",
                    int version = SNMP_VERSION_1,
                    const char *community = "public");

    void tcp_refresh();
    void tcp_rates();
    void tcp_refresh_rates() {
        tcp_refresh();
        tcp_rates();
    }
private:
    void tcp_refresh(bool constructor);

    uint_t oldTcpActiveOpens;
    uint_t oldTcpPassiveOpens;
    uint_t oldTcpAttemptFails;
    uint_t oldTcpEstabResets;
    uint_t oldTcpInSegs;
    uint_t oldTcpOutSegs;
    uint_t oldTcpRetransSegs;
    uint_t oldTcpInErrs;
    uint_t oldTcpOutRsts;
};

}

#endif
