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

#ifndef _SNMP_IP_H_
#define _SNMP_IP_H_

#include "snmp_base.h"

namespace mib {

class snmp_mib2_ip_t : public snmp_base_t {
public:
    int    ipForwarding;      // Forwarder
    int    ipDefaultTTL;      // Time-to-Live
    uint_t ipInReceives;      // Datagrams Received
    uint_t ipInHdrErrors;     // In Discards from Header Errors
    uint_t ipInAddrErrors;    // In Discards from Address Errors
    uint_t ipForwDatagrams;   // Datagrams Forwarded
    uint_t ipInUnknownProtos; // In Discards from Unknown Protocol
    uint_t ipInDiscards;      // In Discards of Good Datagrams
    uint_t ipInDelivers;      // Datagrams Sent Upstream
    uint_t ipOutRequests;     // Upstream Output Requests
    uint_t ipOutDiscards;     // Out Discards of Good Datagrams
    uint_t ipOutNoRoutes;     // Out Discards from No Route
    int    ipReasmTimeout;    // Fragment Reassemble Timeout
    uint_t ipReasmReqds;      // Fragment Reassemblies Required
    uint_t ipReasmOKs;        // Fragments Reassembled
    uint_t ipReasmFails;      // Fragment Reassembly Failures
    uint_t ipFragOKs;         // Datagrams Fragmented
    uint_t ipFragFails;       // Fragment Discards from No-Fragment Flag
    uint_t ipFragCreates;     // Fragments Created
    uint_t ipRoutingDiscards; // Routing Entries Discarded

    snmp_mib2_ip_t(const char *hostname = "localhost",
                   int version = SNMP_VERSION_1,
                   const char *community = "public");

    void ip_refresh();
    void ip_rates();
    void ip_refresh_rates() {
        ip_refresh();
        ip_rates();
    }
private:
    void ip_refresh(bool constructor);

    int    oldIpForwarding;
    int    oldIpDefaultTTL;
    uint_t oldIpInReceives;
    uint_t oldIpInHdrErrors;
    uint_t oldIpInAddrErrors;
    uint_t oldIpForwDatagrams;
    uint_t oldIpInUnknownProtos;
    uint_t oldIpInDiscards;
    uint_t oldIpInDelivers;
    uint_t oldIpOutRequests;
    uint_t oldIpOutDiscards;
    uint_t oldIpOutNoRoutes;
    int    oldIpReasmTimeout;
    uint_t oldIpReasmReqds;
    uint_t oldIpReasmOKs;
    uint_t oldIpReasmFails;
    uint_t oldIpFragOKs;
    uint_t oldIpFragFails;
    uint_t oldIpFragCreates;
    uint_t oldIpRoutingDiscards;
};

}

#endif
