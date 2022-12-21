/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the mdr library of Captivity.
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


#ifndef _MDR_UDP_H_
#define _MDR_UDP_H_

#include "mdr.h"

#ifdef _AIX
# include <mib/snmp_udp.h>
#endif

namespace mdr {

const int MUDP_VERSION = 1;

const int MUDP_UDPINDATAGRAMS   = 0;
const int MUDP_UDPNOPORTS       = 1;
const int MUDP_UDPINERRORS      = 2;
const int MUDP_UDPOUTDATAGRAMS  = 3;
const int MUDP_FIELD_COUNT      = 4;

class mdr_udp_t : public mdr_t {
public:
  uint_t mudp_udpInDatagrams;   // Datagrams Received
  uint_t mudp_udpNoPorts;       // Undeliverable Datagrams Received
  uint_t mudp_udpInErrors;      // Datagrams Received with Error
  uint_t mudp_udpOutDatagrams;  // Datagrams Transmitted

  // constructors/destructors
  mdr_udp_t(const char *remote_host = 0);
 ~mdr_udp_t();

  // updating
  void mdr_refresh(void);
  void mdr_rates(void);

  // other
  const mdr_value_t &mdr_field_value(int field_number, mdr_value_t &value);
  const mdr_value_t &mdr_field_value(const std::string &key, int field_number, mdr_value_t &value)
  {
    return mdr_t::mdr_field_value(key, field_number, value);
  }

private:
#ifdef _AIX
  bool constructor;
  mib::snmp_mib2_udp_t mib2_udp;
#endif

  uint_t old_udpInDatagrams;
  uint_t old_udpNoPorts;
  uint_t old_udpInErrors;
  uint_t old_udpOutDatagrams;

  void udp_remote_refresh(void);
};

} // end of namespace mdr

#endif
