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

#include "snmp_ip.h"

namespace mib {

static const char *const ip_oids[] = {
    "1.3.6.1.2.1.4.1",   // ipForwarding
    "1.3.6.1.2.1.4.2",   // ipDefaultTTL
    "1.3.6.1.2.1.4.3",   // ipInReceives
    "1.3.6.1.2.1.4.4",   // ipInHdrErrors
    "1.3.6.1.2.1.4.5",   // ipInAddrErrors
    "1.3.6.1.2.1.4.6",   // ipForwDatagrams
    "1.3.6.1.2.1.4.7",   // ipInUnknownProtos
    "1.3.6.1.2.1.4.8",   // ipInDiscards
    "1.3.6.1.2.1.4.9",   // ipInDelivers
    "1.3.6.1.2.1.4.10",  // ipOutRequests
    "1.3.6.1.2.1.4.11",  // ipOutDiscards
    "1.3.6.1.2.1.4.12",  // ipOutNoRoutes
    "1.3.6.1.2.1.4.13",  // ipReasmTimeout
    "1.3.6.1.2.1.4.14",  // ipReasmReqds
    "1.3.6.1.2.1.4.15",  // ipReasmOKs
    "1.3.6.1.2.1.4.16",  // ipReasmFails
    "1.3.6.1.2.1.4.17",  // ipFragOKs
    "1.3.6.1.2.1.4.18",  // ipFragFails
    "1.3.6.1.2.1.4.19",  // ipFragCreates
    "1.3.6.1.2.1.4.23",  // ipRoutingDiscards
    0
};

snmp_mib2_ip_t::snmp_mib2_ip_t(const char *hostname,
                               int version,
                               const char *community) :
    snmp_base_t(ip_oids, hostname, version, community)
{
    oldIpForwarding = 0;
    oldIpDefaultTTL = 0;
    oldIpInReceives = 0;
    oldIpInHdrErrors = 0;
    oldIpInAddrErrors = 0;
    oldIpForwDatagrams = 0;
    oldIpInUnknownProtos = 0;
    oldIpInDiscards = 0;
    oldIpInDelivers = 0;
    oldIpOutRequests = 0;
    oldIpOutDiscards = 0;
    oldIpOutNoRoutes = 0;
    oldIpReasmTimeout = 0;
    oldIpReasmReqds = 0;
    oldIpReasmOKs = 0;
    oldIpReasmFails = 0;
    oldIpFragOKs = 0;
    oldIpFragFails = 0;
    oldIpFragCreates = 0;
    oldIpRoutingDiscards = 0;

    ip_refresh(true);
    ip_rates();
}

void snmp_mib2_ip_t::ip_refresh(void)
{
    ip_refresh(false);
}

void snmp_mib2_ip_t::ip_refresh(bool constructor)
{
    snmp_agg_node_t an;
    snmp_aggregate_t &agg = get_oid_list();
    int i;

    if (!constructor)
        snmp_refresh();

    for(i=0, an=agg.begin(); an != agg.end(); an++, i++) {
        snmp_var_list_t &list = *an;
        snmp_var_node_t vn = list.begin();
        snmp_var_t &var = *vn;
        int last_octet = var.name[var.name_length - 2];

        switch(last_octet) {
        case 1:
            // INTEGER
            ipForwarding = var.val.integer;
            break;
        case 2:
            // INTEGER
            ipDefaultTTL = var.val.integer;
            break;
        case 3:
            // COUNTER32
            ipInReceives = var.val.integer;
            break;
        case 4:
            // COUNTER32
            ipInHdrErrors = var.val.integer;
            break;
        case 5:
            // COUNTER32
            ipInAddrErrors = var.val.integer;
            break;
        case 6:
            // COUNTER32
            ipForwDatagrams = var.val.integer;
            break;
        case 7:
            // COUNTER32
            ipInUnknownProtos = var.val.integer;
            break;
        case 8:
            // COUNTER32
            ipInDiscards = var.val.integer;
            break;
        case 9:
            // COUNTER32
            ipInDelivers = var.val.integer;
            break;
        case 10:
            // COUNTER32
            ipOutRequests = var.val.integer;
            break;
        case 11:
            // COUNTER32
            ipOutDiscards = var.val.integer;
            break;
        case 12:
            // COUNTER32
            ipOutNoRoutes = var.val.integer;
            break;
        case 13:
            // INTEGER
            ipReasmTimeout = var.val.integer;
            break;
        case 14:
            // COUNTER32
            ipReasmReqds = var.val.integer;
            break;
        case 15:
            // COUNTER32
            ipReasmOKs = var.val.integer;
            break;
        case 16:
            // COUNTER32
            ipReasmFails = var.val.integer;
            break;
        case 17:
            // COUNTER32
            ipFragOKs = var.val.integer;
            break;
        case 18:
            // COUNTER32
            ipFragFails = var.val.integer;
            break;
        case 19:
            // COUNTER32
            ipFragCreates = var.val.integer;
            break;
        case 23:
            // COUNTER32
            ipRoutingDiscards = var.val.integer;
            break;
        default:
            break;
        }
    }
}

// generate rates for the counters

void snmp_mib2_ip_t::ip_rates(void)
{
    uint_t tmpIpInReceives = ipInReceives;
    uint_t tmpIpInHdrErrors = ipInHdrErrors;
    uint_t tmpIpInAddrErrors = ipInAddrErrors;
    uint_t tmpIpForwDatagrams = ipForwDatagrams;
    uint_t tmpIpInUnknownProtos = ipInUnknownProtos;
    uint_t tmpIpInDiscards = ipInDiscards;
    uint_t tmpIpInDelivers = ipInDelivers;
    uint_t tmpIpOutRequests = ipOutRequests;
    uint_t tmpIpOutDiscards = ipOutDiscards;
    uint_t tmpIpOutNoRoutes = ipOutNoRoutes;
    uint_t tmpIpReasmReqds = ipReasmReqds;
    uint_t tmpIpReasmOKs = ipReasmOKs;
    uint_t tmpIpReasmFails = ipReasmFails;
    uint_t tmpIpFragOKs = ipFragOKs;
    uint_t tmpIpFragFails = ipFragFails;
    uint_t tmpIpFragCreates = ipFragCreates;
    uint_t tmpIpRoutingDiscards = ipRoutingDiscards;

    ipInReceives = wrap_diff_32(ipInReceives, oldIpInReceives);
    ipInHdrErrors = wrap_diff_32(ipInHdrErrors, oldIpInHdrErrors);
    ipInAddrErrors = wrap_diff_32(ipInAddrErrors, oldIpInAddrErrors);
    ipForwDatagrams = wrap_diff_32(ipForwDatagrams, oldIpForwDatagrams);
    ipInUnknownProtos = wrap_diff_32(ipInUnknownProtos, oldIpInUnknownProtos);
    ipInDiscards = wrap_diff_32(ipInDiscards, oldIpInDiscards);
    ipInDelivers = wrap_diff_32(ipInDelivers, oldIpInDelivers);
    ipOutRequests = wrap_diff_32(ipOutRequests, oldIpOutRequests);
    ipOutDiscards = wrap_diff_32(ipOutDiscards, oldIpOutDiscards);
    ipOutNoRoutes = wrap_diff_32(ipOutNoRoutes, oldIpOutNoRoutes);
    ipReasmReqds = wrap_diff_32(ipReasmReqds, oldIpReasmReqds);
    ipReasmOKs = wrap_diff_32(ipReasmOKs, oldIpReasmOKs);
    ipReasmFails = wrap_diff_32(ipReasmFails, oldIpReasmFails);
    ipFragOKs = wrap_diff_32(ipFragOKs, oldIpFragOKs);
    ipFragFails = wrap_diff_32(ipFragFails, oldIpFragFails);
    ipFragCreates = wrap_diff_32(ipFragCreates, oldIpFragCreates);
    ipRoutingDiscards = wrap_diff_32(ipRoutingDiscards, oldIpRoutingDiscards);

    oldIpInReceives = tmpIpInReceives;
    oldIpInHdrErrors = tmpIpInHdrErrors;
    oldIpInAddrErrors = tmpIpInAddrErrors;
    oldIpForwDatagrams = tmpIpForwDatagrams;
    oldIpInUnknownProtos = tmpIpInUnknownProtos;
    oldIpInDiscards = tmpIpInDiscards;
    oldIpInDelivers = tmpIpInDelivers;
    oldIpOutRequests = tmpIpOutRequests;
    oldIpOutDiscards = tmpIpOutDiscards;
    oldIpOutNoRoutes = tmpIpOutNoRoutes;
    oldIpReasmReqds = tmpIpReasmReqds;
    oldIpReasmOKs = tmpIpReasmOKs;
    oldIpReasmFails = tmpIpReasmFails;
    oldIpFragOKs = tmpIpFragOKs;
    oldIpFragFails = tmpIpFragFails;
    oldIpFragCreates = tmpIpFragCreates;
    oldIpRoutingDiscards = tmpIpRoutingDiscards;
}

} // end of namespace
