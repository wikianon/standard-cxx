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

#include "snmp_tcp.h"

namespace mib {

static const char *const tcp_oids[] = {
  "1.3.6.1.2.1.6.1",   // tcpRtoAlgorithm
  "1.3.6.1.2.1.6.2",   // tcpRtoMin
  "1.3.6.1.2.1.6.3",   // tcpRtoMax
  "1.3.6.1.2.1.6.4",   // tcpMaxConn
  "1.3.6.1.2.1.6.5",   // tcpActiveOpens
  "1.3.6.1.2.1.6.6",   // tcpPassiveOpens
  "1.3.6.1.2.1.6.7",   // tcpAttemptFails
  "1.3.6.1.2.1.6.8",   // tcpEstabResets
  "1.3.6.1.2.1.6.9",   // tcpCurrEstab
  "1.3.6.1.2.1.6.10",  // tcpInSegs
  "1.3.6.1.2.1.6.11",  // tcpOutSegs
  "1.3.6.1.2.1.6.12",  // tcpRetransSegs
  "1.3.6.1.2.1.6.14",  // tcpInErrs
  "1.3.6.1.2.1.6.15",  // tcpOutRsts
  0
};

snmp_mib2_tcp_t::snmp_mib2_tcp_t(const char *hostname,
                                 int version,
                                 const char *community) :
    snmp_base_t(tcp_oids, hostname, version, community)
{
    oldTcpActiveOpens = 0;
    oldTcpPassiveOpens = 0;
    oldTcpAttemptFails = 0;
    oldTcpEstabResets = 0;
    oldTcpInSegs = 0;
    oldTcpOutSegs = 0;
    oldTcpRetransSegs = 0;
    oldTcpInErrs = 0;
    oldTcpOutRsts = 0;

    tcp_refresh(true);
    tcp_rates();
}

void snmp_mib2_tcp_t::tcp_refresh(void)
{
    tcp_refresh(false);
}

void snmp_mib2_tcp_t::tcp_refresh(bool constructor)
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
            tcpRtoAlgorithm = var.val.integer;
            break;
        case 2:
            // INTEGER
            tcpRtoMin = var.val.integer;
            break;
        case 3:
            // INTEGER
            tcpRtoMax = var.val.integer;
            break;
        case 4:
            // INTEGER
            tcpMaxConn = var.val.integer;
            break;
        case 5:
            // COUNTER32
            tcpActiveOpens = var.val.integer;
            break;
        case 6:
            // COUNTER32
            tcpPassiveOpens = var.val.integer;
            break;
        case 7:
            // COUNTER32
            tcpAttemptFails = var.val.integer;
            break;
        case 8:
            // COUNTER32
            tcpEstabResets = var.val.integer;
            break;
        case 9:
            // GAUGE32
            tcpCurrEstab = var.val.integer;
            break;
        case 10:
            // COUNTER32
            tcpInSegs = var.val.integer;
            break;
        case 11:
            // COUNTER32
            tcpOutSegs = var.val.integer;
            break;
        case 12:
            // COUNTER32
            tcpRetransSegs = var.val.integer;
            break;
        case 14:
            // COUNTER32
            tcpInErrs = var.val.integer;
            break;
        case 15:
            // COUNTER32
            tcpOutRsts = var.val.integer;
            break;
        default:
            break;
        }
    }
}

// generate rates for the counters

void snmp_mib2_tcp_t::tcp_rates(void)
{
    uint_t tmpTcpActiveOpens = tcpActiveOpens;
    uint_t tmpTcpPassiveOpens = tcpPassiveOpens;
    uint_t tmpTcpAttemptFails = tcpAttemptFails;
    uint_t tmpTcpEstabResets = tcpEstabResets;
    uint_t tmpTcpInSegs = tcpInSegs;
    uint_t tmpTcpOutSegs = tcpOutSegs;
    uint_t tmpTcpRetransSegs = tcpRetransSegs;
    uint_t tmpTcpInErrs = tcpInErrs;
    uint_t tmpTcpOutRsts = tcpOutRsts;

    tcpActiveOpens  = wrap_diff_32(tcpActiveOpens, oldTcpActiveOpens);
    tcpPassiveOpens = wrap_diff_32(tcpPassiveOpens, oldTcpPassiveOpens);
    tcpAttemptFails = wrap_diff_32(tcpAttemptFails, oldTcpAttemptFails);
    tcpEstabResets  = wrap_diff_32(tcpEstabResets, oldTcpEstabResets);
    tcpInSegs       = wrap_diff_32(tcpInSegs, oldTcpInSegs);
    tcpOutSegs      = wrap_diff_32(tcpOutSegs, oldTcpOutSegs);
    tcpRetransSegs  = wrap_diff_32(tcpRetransSegs, oldTcpRetransSegs);
    tcpInErrs       = wrap_diff_32(tcpInErrs, oldTcpInErrs);
    tcpOutRsts      = wrap_diff_32(tcpOutRsts, oldTcpOutRsts);

    oldTcpActiveOpens = tmpTcpActiveOpens;
    oldTcpPassiveOpens = tmpTcpPassiveOpens;
    oldTcpAttemptFails = tmpTcpAttemptFails;
    oldTcpEstabResets = tmpTcpEstabResets;
    oldTcpInSegs = tmpTcpInSegs;
    oldTcpOutSegs = tmpTcpOutSegs;
    oldTcpRetransSegs = tmpTcpRetransSegs;
    oldTcpInErrs = tmpTcpInErrs;
    oldTcpOutRsts = tmpTcpOutRsts;
}

} // end of namespace
