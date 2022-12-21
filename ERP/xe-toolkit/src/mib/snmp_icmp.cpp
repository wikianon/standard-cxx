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

#include "snmp_icmp.h"

namespace mib {

static const char *const icmp_oids[] = {
    "1.3.6.1.2.1.5.1",
    "1.3.6.1.2.1.5.2",
    "1.3.6.1.2.1.5.3",
    "1.3.6.1.2.1.5.4",
    "1.3.6.1.2.1.5.5",
    "1.3.6.1.2.1.5.6",
    "1.3.6.1.2.1.5.7",
    "1.3.6.1.2.1.5.8",
    "1.3.6.1.2.1.5.9",
    "1.3.6.1.2.1.5.10",
    "1.3.6.1.2.1.5.11",
    "1.3.6.1.2.1.5.12",
    "1.3.6.1.2.1.5.13",
    "1.3.6.1.2.1.5.14",
    "1.3.6.1.2.1.5.15",
    "1.3.6.1.2.1.5.16",
    "1.3.6.1.2.1.5.17",
    "1.3.6.1.2.1.5.18",
    "1.3.6.1.2.1.5.19",
    "1.3.6.1.2.1.5.20",
    "1.3.6.1.2.1.5.21",
    "1.3.6.1.2.1.5.22",
    "1.3.6.1.2.1.5.23",
    "1.3.6.1.2.1.5.24",
    "1.3.6.1.2.1.5.25",
    "1.3.6.1.2.1.5.26",
    0
};

snmp_mib2_icmp_t::snmp_mib2_icmp_t(const char *hostname,
                                   int version,
                                   const char *community) :
    snmp_base_t(icmp_oids, hostname, version, community)
{
    oldIcmpInMsgs = 0;
    oldIcmpInErrors = 0;
    oldIcmpInDestUnreachs = 0;
    oldIcmpInTimeExcds = 0;
    oldIcmpInParmProbs = 0;
    oldIcmpInSrcQuenchs = 0;
    oldIcmpInRedirects = 0;
    oldIcmpInEchos = 0;
    oldIcmpInEchoReps = 0;
    oldIcmpInTimestamps = 0;
    oldIcmpInTimestampReps = 0;
    oldIcmpInAddrMasks = 0;
    oldIcmpInAddrMaskReps = 0;
    oldIcmpOutMsgs = 0;
    oldIcmpOutErrors = 0;
    oldIcmpOutDestUnreachs = 0;
    oldIcmpOutTimeExcds = 0;
    oldIcmpOutParmProbs = 0;
    oldIcmpOutSrcQuenchs = 0;
    oldIcmpOutRedirects = 0;
    oldIcmpOutEchos = 0;
    oldIcmpOutEchoReps = 0;
    oldIcmpOutTimestamps = 0;
    oldIcmpOutTimestampReps = 0;
    oldIcmpOutAddrMasks = 0;
    oldIcmpOutAddrMaskReps = 0;

    icmp_refresh(true);
    icmp_rates();
}

void snmp_mib2_icmp_t::icmp_refresh(void)
{
    icmp_refresh(false);
}

void snmp_mib2_icmp_t::icmp_refresh(bool constructor)
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
            // COUNTER32
            icmpInMsgs = var.val.integer;
            break;
        case 2:
            // COUNTER32
            icmpInErrors = var.val.integer;
            break;
        case 3:
            // COUNTER32
            icmpInDestUnreachs = var.val.integer;
            break;
        case 4:
            // COUNTER32
            icmpInTimeExcds = var.val.integer;
            break;
        case 5:
            // COUNTER32
            icmpInParmProbs = var.val.integer;
            break;
        case 6:
            // COUNTER32
            icmpInSrcQuenchs = var.val.integer;
            break;
        case 7:
            // COUNTER32
            icmpInRedirects = var.val.integer;
            break;
        case 8:
            // COUNTER32
            icmpInEchos = var.val.integer;
            break;
        case 9:
            // COUNTER32
            icmpInEchoReps = var.val.integer;
            break;
        case 10:
            // COUNTER32
            icmpInTimestamps = var.val.integer;
            break;
        case 11:
            // COUNTER32
            icmpInTimestampReps = var.val.integer;
            break;
        case 12:
            // COUNTER32
            icmpInAddrMasks = var.val.integer;
            break;
        case 13:
            // COUNTER32
            icmpInAddrMaskReps = var.val.integer;
            break;
        case 14:
            // COUNTER32
            icmpOutMsgs = var.val.integer;
            break;
        case 15:
            // COUNTER32
            icmpOutErrors = var.val.integer;
            break;
        case 16:
            // COUNTER32
            icmpOutDestUnreachs = var.val.integer;
            break;
        case 17:
            // COUNTER32
            icmpOutTimeExcds = var.val.integer;
            break;
        case 18:
            // COUNTER32
            icmpOutParmProbs = var.val.integer;
            break;
        case 19:
            // COUNTER32
            icmpOutSrcQuenchs = var.val.integer;
            break;
        case 20:
            // COUNTER32
            icmpOutRedirects = var.val.integer;
            break;
        case 21:
            // COUNTER32
            icmpOutEchos = var.val.integer;
            break;
        case 22:
            // COUNTER32
            icmpOutEchoReps = var.val.integer;
            break;
        case 23:
            // COUNTER32
            icmpOutTimestamps = var.val.integer;
            break;
        case 24:
            // COUNTER32
            icmpOutTimestampReps = var.val.integer;
            break;
        case 25:
            // COUNTER32
            icmpOutAddrMasks = var.val.integer;
            break;
        case 26:
            // COUNTER32
            icmpOutAddrMaskReps = var.val.integer;
            break;
        default:
            break;
        }
    }
}

// generate rates for the counters

void snmp_mib2_icmp_t::icmp_rates(void)
{
    uint_t tmpIcmpInMsgs = icmpInMsgs;
    uint_t tmpIcmpInErrors = icmpInErrors;
    uint_t tmpIcmpInDestUnreachs = icmpInDestUnreachs;
    uint_t tmpIcmpInTimeExcds = icmpInTimeExcds;
    uint_t tmpIcmpInParmProbs = icmpInParmProbs;
    uint_t tmpIcmpInSrcQuenchs = icmpInSrcQuenchs;
    uint_t tmpIcmpInRedirects = icmpInRedirects;
    uint_t tmpIcmpInEchos = icmpInEchos;
    uint_t tmpIcmpInEchoReps = icmpInEchoReps;
    uint_t tmpIcmpInTimestamps = icmpInTimestamps;
    uint_t tmpIcmpInTimestampReps = icmpInTimestampReps;
    uint_t tmpIcmpInAddrMasks = icmpInAddrMasks;
    uint_t tmpIcmpInAddrMaskReps = icmpInAddrMaskReps;
    uint_t tmpIcmpOutMsgs = icmpOutMsgs;
    uint_t tmpIcmpOutErrors = icmpOutErrors;
    uint_t tmpIcmpOutDestUnreachs = icmpOutDestUnreachs;
    uint_t tmpIcmpOutTimeExcds = icmpOutTimeExcds;
    uint_t tmpIcmpOutParmProbs = icmpOutParmProbs;
    uint_t tmpIcmpOutSrcQuenchs = icmpOutSrcQuenchs;
    uint_t tmpIcmpOutRedirects = icmpOutRedirects;
    uint_t tmpIcmpOutEchos = icmpOutEchos;
    uint_t tmpIcmpOutEchoReps = icmpOutEchoReps;
    uint_t tmpIcmpOutTimestamps = icmpOutTimestamps;
    uint_t tmpIcmpOutTimestampReps = icmpOutTimestampReps;
    uint_t tmpIcmpOutAddrMasks = icmpOutAddrMasks;
    uint_t tmpIcmpOutAddrMaskReps = icmpOutAddrMaskReps;

    icmpInMsgs = wrap_diff_32(icmpInMsgs, oldIcmpInMsgs);
    icmpInErrors = wrap_diff_32(icmpInErrors, oldIcmpInErrors);
    icmpInDestUnreachs = wrap_diff_32(icmpInDestUnreachs, oldIcmpInDestUnreachs);
    icmpInTimeExcds = wrap_diff_32(icmpInTimeExcds, oldIcmpInTimeExcds);
    icmpInParmProbs = wrap_diff_32(icmpInParmProbs, oldIcmpInParmProbs);
    icmpInSrcQuenchs = wrap_diff_32(icmpInSrcQuenchs, oldIcmpInSrcQuenchs);
    icmpInRedirects = wrap_diff_32(icmpInRedirects, oldIcmpInRedirects);
    icmpInEchos = wrap_diff_32(icmpInEchos, oldIcmpInEchos);
    icmpInEchoReps = wrap_diff_32(icmpInEchoReps, oldIcmpInEchoReps);
    icmpInTimestamps = wrap_diff_32(icmpInTimestamps, oldIcmpInTimestamps);
    icmpInTimestampReps = wrap_diff_32(icmpInTimestampReps, oldIcmpInTimestampReps);
    icmpInAddrMasks = wrap_diff_32(icmpInAddrMasks, oldIcmpInAddrMasks);
    icmpInAddrMaskReps = wrap_diff_32(icmpInAddrMaskReps, oldIcmpInAddrMaskReps);
    icmpOutMsgs = wrap_diff_32(icmpOutMsgs, oldIcmpOutMsgs);
    icmpOutErrors = wrap_diff_32(icmpOutErrors, oldIcmpOutErrors);
    icmpOutDestUnreachs = wrap_diff_32(icmpOutDestUnreachs, oldIcmpOutDestUnreachs);
    icmpOutTimeExcds = wrap_diff_32(icmpOutTimeExcds, oldIcmpOutTimeExcds);
    icmpOutParmProbs = wrap_diff_32(icmpOutParmProbs, oldIcmpOutParmProbs);
    icmpOutSrcQuenchs = wrap_diff_32(icmpOutSrcQuenchs, oldIcmpOutSrcQuenchs);
    icmpOutRedirects = wrap_diff_32(icmpOutRedirects, oldIcmpOutRedirects);
    icmpOutEchos = wrap_diff_32(icmpOutEchos, oldIcmpOutEchos);
    icmpOutEchoReps = wrap_diff_32(icmpOutEchoReps, oldIcmpOutEchoReps);
    icmpOutTimestamps = wrap_diff_32(icmpOutTimestamps, oldIcmpOutTimestamps);
    icmpOutTimestampReps = wrap_diff_32(icmpOutTimestampReps, oldIcmpOutTimestampReps);
    icmpOutAddrMasks = wrap_diff_32(icmpOutAddrMasks, oldIcmpOutAddrMasks);
    icmpOutAddrMaskReps = wrap_diff_32(icmpOutAddrMaskReps, oldIcmpOutAddrMaskReps);

    oldIcmpInMsgs = tmpIcmpInMsgs;
    oldIcmpInErrors = tmpIcmpInErrors;
    oldIcmpInDestUnreachs = tmpIcmpInDestUnreachs;
    oldIcmpInTimeExcds = tmpIcmpInTimeExcds;
    oldIcmpInParmProbs = tmpIcmpInParmProbs;
    oldIcmpInSrcQuenchs = tmpIcmpInSrcQuenchs;
    oldIcmpInRedirects = tmpIcmpInRedirects;
    oldIcmpInEchos = tmpIcmpInEchos;
    oldIcmpInEchoReps = tmpIcmpInEchoReps;
    oldIcmpInTimestamps = tmpIcmpInTimestamps;
    oldIcmpInTimestampReps = tmpIcmpInTimestampReps;
    oldIcmpInAddrMasks = tmpIcmpInAddrMasks;
    oldIcmpInAddrMaskReps = tmpIcmpInAddrMaskReps;
    oldIcmpOutMsgs = tmpIcmpOutMsgs;
    oldIcmpOutErrors = tmpIcmpOutErrors;
    oldIcmpOutDestUnreachs = tmpIcmpOutDestUnreachs;
    oldIcmpOutTimeExcds = tmpIcmpOutTimeExcds;
    oldIcmpOutParmProbs = tmpIcmpOutParmProbs;
    oldIcmpOutSrcQuenchs = tmpIcmpOutSrcQuenchs;
    oldIcmpOutRedirects = tmpIcmpOutRedirects;
    oldIcmpOutEchos = tmpIcmpOutEchos;
    oldIcmpOutEchoReps = tmpIcmpOutEchoReps;
    oldIcmpOutTimestamps = tmpIcmpOutTimestamps;
    oldIcmpOutTimestampReps = tmpIcmpOutTimestampReps;
    oldIcmpOutAddrMasks = tmpIcmpOutAddrMasks;
    oldIcmpOutAddrMaskReps = tmpIcmpOutAddrMaskReps;
}

} // end of namespace


