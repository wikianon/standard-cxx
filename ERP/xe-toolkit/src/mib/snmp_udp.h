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

#ifndef _SNMP_UDP_H_
#define _SNMP_UDP_H_

#include "snmp_base.h"

namespace mib {

class snmp_mib2_udp_t : public snmp_base_t {
public:
    uint_t udpInDatagrams;   // Datagrams Received
    uint_t udpNoPorts;       // Undeliverable Datagrams Received
    uint_t udpInErrors;      // Datagrams Received with Error
    uint_t udpOutDatagrams;  // Datagrams Transmitted

    snmp_mib2_udp_t(const char *hostname = "localhost",
                    int version = SNMP_VERSION_1,
                    const char *community = "public");

    void udp_refresh();
    void udp_rates();
    void udp_refresh_rates() {
        udp_refresh();
        udp_rates();
    }
private:
    void udp_refresh(bool constructor);

    uint_t oldUdpInDatagrams;
    uint_t oldUdpNoPorts;
    uint_t oldUdpInErrors;
    uint_t oldUdpOutDatagrams;
};

}

#endif
