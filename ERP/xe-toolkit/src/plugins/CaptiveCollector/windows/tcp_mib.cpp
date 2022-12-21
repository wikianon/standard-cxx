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


#include "Stdafx.h"
#include "tcp_mib.h"
#include "plugin_ex.h"
#include <IPHlpApi.h>
#include <Iprtrmib.h>

tcp_mib_t::tcp_mib_t()
{
    oldTcpActiveOpens = 0;
    oldTcpPassiveOpens = 0;
    oldTcpAttemptFails = 0;
    oldTcpEstabResets = 0;
    oldTcpCurrEstab = 0;
    oldTcpInSegs = 0;
    oldTcpOutSegs = 0;
    oldTcpRetransSegs = 0;
    oldTcpInErrs = 0;
    oldTcpOutRsts = 0;

    tcp_refresh_rates();
}

void tcp_mib_t::tcp_refresh(void)
{
    MIB_TCPSTATS stats;

    if (GetTcpStatistics(&stats) == -1)
        throw plugin_exception("tcp_mib_t::refresh: GetTcpStatistics fails");

    tcpRtoAlgorithm = stats.dwRtoAlgorithm;
    tcpRtoMin       = stats.dwRtoMin;
    tcpRtoMax       = stats.dwRtoMax;
    tcpMaxConn      = stats.dwMaxConn;
    tcpActiveOpens  = stats.dwActiveOpens;
    tcpPassiveOpens = stats.dwPassiveOpens;
    tcpAttemptFails = stats.dwAttemptFails;
    tcpEstabResets  = stats.dwEstabResets;
    tcpCurrEstab    = stats.dwCurrEstab;
    tcpInSegs       = stats.dwInSegs;
    tcpOutSegs      = stats.dwOutSegs;
    tcpRetransSegs  = stats.dwRetransSegs;
    tcpInErrs       = stats.dwInErrs;
    tcpOutRsts      = stats.dwOutRsts;
}

void tcp_mib_t::tcp_rates(void)
{
    uint_t tmpTcpActiveOpens  = tcpActiveOpens;
    uint_t tmpTcpPassiveOpens = tcpPassiveOpens;
    uint_t tmpTcpAttemptFails = tcpAttemptFails;
    uint_t tmpTcpEstabResets  = tcpEstabResets;
    uint_t tmpTcpCurrEstab    = tcpCurrEstab;
    uint_t tmpTcpInSegs       = tcpInSegs;
    uint_t tmpTcpOutSegs      = tcpOutSegs;
    uint_t tmpTcpRetransSegs  = tcpRetransSegs;
    uint_t tmpTcpInErrs       = tcpInErrs;
    uint_t tmpTcpOutRsts      = tcpOutRsts;

    tcpActiveOpens  -= oldTcpActiveOpens;
    tcpPassiveOpens -= oldTcpPassiveOpens;
    tcpAttemptFails -= oldTcpAttemptFails;
    tcpEstabResets  -= oldTcpEstabResets;
    tcpCurrEstab    -= oldTcpCurrEstab;
    tcpInSegs       -= oldTcpInSegs;
    tcpOutSegs      -= oldTcpOutSegs;
    tcpRetransSegs  -= oldTcpRetransSegs;
    tcpInErrs       -= oldTcpInErrs;
    tcpOutRsts      -= oldTcpOutRsts;

    oldTcpActiveOpens  = tmpTcpActiveOpens;
    oldTcpPassiveOpens = tmpTcpPassiveOpens;
    oldTcpAttemptFails = tmpTcpAttemptFails;
    oldTcpEstabResets  = tmpTcpEstabResets;
    oldTcpCurrEstab    = tmpTcpCurrEstab;
    oldTcpInSegs       = tmpTcpInSegs;
    oldTcpOutSegs      = tmpTcpOutSegs;
    oldTcpRetransSegs  = tmpTcpRetransSegs;
    oldTcpInErrs       = tmpTcpInErrs;
    oldTcpOutRsts      = tmpTcpOutRsts;
}

void tcp_mib_t::tcp_refresh_rates(void)
{
	tcp_refresh();
	tcp_rates();
}
