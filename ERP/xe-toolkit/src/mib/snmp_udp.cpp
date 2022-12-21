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

#include "snmp_udp.h"

namespace mib {

static const char *const udp_oids[] = {
    "1.3.6.1.2.1.7.1", // udpInDatagrams
    "1.3.6.1.2.1.7.2", // udpNoPorts
    "1.3.6.1.2.1.7.3", // udpInErrors
    "1.3.6.1.2.1.7.4", // udpOutDatagrams
    0
};

snmp_mib2_udp_t::snmp_mib2_udp_t(const char *hostname,
                                 int version,
                                 const char *community) :
    snmp_base_t(udp_oids, hostname, version, community)
{
    oldUdpInDatagrams = 0;
    oldUdpNoPorts = 0;
    oldUdpInErrors = 0;
    oldUdpOutDatagrams = 0;

    udp_refresh(true);
    udp_rates();
}

void snmp_mib2_udp_t::udp_refresh(void)
{
    udp_refresh(false);
}

void snmp_mib2_udp_t::udp_refresh(bool constructor)
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
            udpInDatagrams = var.val.integer;
            break;
        case 2:
            // COUNTER32
            udpNoPorts = var.val.integer;
            break;
        case 3:
            // COUNTER32
            udpInErrors = var.val.integer;
            break;
        case 4:
            // COUNTER32
            udpOutDatagrams = var.val.integer;
            break;
        default:
            break;
        }
    }
}

// generate rates for the counters

void snmp_mib2_udp_t::udp_rates(void)
{
    uint_t tmpUdpInDatagrams = udpInDatagrams;
    uint_t tmpUdpNoPorts = udpNoPorts;
    uint_t tmpUdpInErrors = udpInErrors;
    uint_t tmpUdpOutDatagrams = udpOutDatagrams;

    udpInDatagrams  = wrap_diff_32(udpInDatagrams, oldUdpInDatagrams);
    udpNoPorts      = wrap_diff_32(udpNoPorts, oldUdpNoPorts);
    udpInErrors     = wrap_diff_32(udpInErrors, oldUdpInErrors);
    udpOutDatagrams = wrap_diff_32(udpOutDatagrams, oldUdpOutDatagrams);

    oldUdpInDatagrams = tmpUdpInDatagrams;
    oldUdpNoPorts = tmpUdpNoPorts;
    oldUdpInErrors = tmpUdpInErrors;
    oldUdpOutDatagrams = tmpUdpOutDatagrams;
}

} // end of namespace

