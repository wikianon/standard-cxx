/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Windows plugin library for CaptiveCollector.
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


#ifndef _TCPMIB_H_
#define _TCPMIB_H_

#include "Stdafx.h"
#include "CCwinplugin.h"
#include <string>

typedef unsigned int uint_t;

class CCWINPLUGIN_API tcp_mib_t {
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

    tcp_mib_t();

    void tcp_refresh(void);
    void tcp_rates(void);
    void tcp_refresh_rates(void);

private:
    uint_t oldTcpActiveOpens;
    uint_t oldTcpPassiveOpens;
    uint_t oldTcpAttemptFails;
    uint_t oldTcpEstabResets;
    uint_t oldTcpCurrEstab;
    uint_t oldTcpInSegs;
    uint_t oldTcpOutSegs;
    uint_t oldTcpRetransSegs;
    uint_t oldTcpInErrs;
    uint_t oldTcpOutRsts;
};

#endif
