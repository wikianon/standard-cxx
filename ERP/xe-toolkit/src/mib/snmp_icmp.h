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

#ifndef _SNMP_ICMP_H_
#define _SNMP_ICMP_H_

#include "snmp_base.h"

namespace mib {

class snmp_mib2_icmp_t : public snmp_base_t {
public:
    uint_t icmpInMsgs;           // Messages Received
    uint_t icmpInErrors;         // Messages Received with Error
    uint_t icmpInDestUnreachs;   // Dest. Unreach. Msgs. Received
    uint_t icmpInTimeExcds;      // Time Exceeded Msgs. Received
    uint_t icmpInParmProbs;      // Param. Prob. Msgs. Received
    uint_t icmpInSrcQuenchs;     // Source Quench Msgs. Received
    uint_t icmpInRedirects;      // ICMP Redirect Msgs. Received
    uint_t icmpInEchos;          // Echo Request Msgs. Received
    uint_t icmpInEchoReps;       // Echo Reply Msgs. Received
    uint_t icmpInTimestamps;     // Timestamp Msgs. Received
    uint_t icmpInTimestampReps;  // Timestamp Reply Msgs. Received
    uint_t icmpInAddrMasks;      // Addr. Mask Req. Msgs. Received
    uint_t icmpInAddrMaskReps;   // Addr. Mask Reply Msgs. Received
    uint_t icmpOutMsgs;          // Messages Transmitted
    uint_t icmpOutErrors;        // Messages Transmitted for Errors
    uint_t icmpOutDestUnreachs;  // Dest. Unreach. Msgs. Transmitted
    uint_t icmpOutTimeExcds;     // Time Exceeded Msgs. Transmitted
    uint_t icmpOutParmProbs;     // Param. Prob. Msgs. Transmitted
    uint_t icmpOutSrcQuenchs;    // Source Quench Msgs. Transmitted
    uint_t icmpOutRedirects;     // ICMP Redirect Msgs. Transmitted
    uint_t icmpOutEchos;         // Echo Request Msgs. Transmitted
    uint_t icmpOutEchoReps;      // Echo Reply Msgs. Transmitted
    uint_t icmpOutTimestamps;    // Timestamp Msgs. Transmitted
    uint_t icmpOutTimestampReps; // Timestamp Reply Msgs. Transmitted
    uint_t icmpOutAddrMasks;     // Addr. Mask Req. Msgs. Transmitted
    uint_t icmpOutAddrMaskReps;  // Addr. Mask Reply Msgs. Transmitted

    snmp_mib2_icmp_t(const char *hostname = "localhost",
                     int version = SNMP_VERSION_1,
                     const char *community = "public");

    void icmp_refresh();
    void icmp_rates();
    void icmp_refresh_rates() {
        icmp_refresh();
        icmp_rates();
    }
private:
    void icmp_refresh(bool constructor);

    uint_t oldIcmpInMsgs;
    uint_t oldIcmpInErrors;
    uint_t oldIcmpInDestUnreachs;
    uint_t oldIcmpInTimeExcds;
    uint_t oldIcmpInParmProbs;
    uint_t oldIcmpInSrcQuenchs;
    uint_t oldIcmpInRedirects;
    uint_t oldIcmpInEchos;
    uint_t oldIcmpInEchoReps;
    uint_t oldIcmpInTimestamps;
    uint_t oldIcmpInTimestampReps;
    uint_t oldIcmpInAddrMasks;
    uint_t oldIcmpInAddrMaskReps;
    uint_t oldIcmpOutMsgs;
    uint_t oldIcmpOutErrors;
    uint_t oldIcmpOutDestUnreachs;
    uint_t oldIcmpOutTimeExcds;
    uint_t oldIcmpOutParmProbs;
    uint_t oldIcmpOutSrcQuenchs;
    uint_t oldIcmpOutRedirects;
    uint_t oldIcmpOutEchos;
    uint_t oldIcmpOutEchoReps;
    uint_t oldIcmpOutTimestamps;
    uint_t oldIcmpOutTimestampReps;
    uint_t oldIcmpOutAddrMasks;
    uint_t oldIcmpOutAddrMaskReps;
};

}

#endif

