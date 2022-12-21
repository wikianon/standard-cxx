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


#ifndef _MDR_ICMP_H_
#define _MDR_ICMP_H_

#include "mdr.h"

#ifdef _AIX
# include <mib/snmp_icmp.h>
#endif

namespace mdr {

const int MICMP_VERSION = 1;

const int MICMP_ICMPINMSGS           = 0;
const int MICMP_ICMPINERRORS         = 1;
const int MICMP_ICMPINDESTUNREACHS   = 2;
const int MICMP_ICMPINTIMEEXCDS      = 3;
const int MICMP_ICMPINPARMPROBS      = 4;
const int MICMP_ICMPINSRCQUENCHS     = 5;
const int MICMP_ICMPINREDIRECTS      = 6;
const int MICMP_ICMPINECHOS          = 7;
const int MICMP_ICMPINECHOREPS       = 8;
const int MICMP_ICMPINTIMESTAMPS     = 9;
const int MICMP_ICMPINTIMESTAMPREPS  = 10;
const int MICMP_ICMPINADDRMASKS      = 11;
const int MICMP_ICMPINADDRMASKREPS   = 12;
const int MICMP_ICMPOUTMSGS          = 13;
const int MICMP_ICMPOUTERRORS        = 14;
const int MICMP_ICMPOUTDESTUNREACHS  = 15;
const int MICMP_ICMPOUTTIMEEXCDS     = 16;
const int MICMP_ICMPOUTPARMPROBS     = 17;
const int MICMP_ICMPOUTSRCQUENCHS    = 18;
const int MICMP_ICMPOUTREDIRECTS     = 19;
const int MICMP_ICMPOUTECHOS         = 20;
const int MICMP_ICMPOUTECHOREPS      = 21;
const int MICMP_ICMPOUTTIMESTAMPS    = 22;
const int MICMP_ICMPOUTTIMESTAMPREPS = 23;
const int MICMP_ICMPOUTADDRMASKS     = 24;
const int MICMP_ICMPOUTADDRMASKREPS  = 25;
const int MICMP_FIELD_COUNT          = 26;

class mdr_icmp_t : public mdr_t {
public:
  uint_t micmp_icmpInMsgs;           // Messages Received
  uint_t micmp_icmpInErrors;         // Messages Received with Error
  uint_t micmp_icmpInDestUnreachs;   // Dest. Unreach. Msgs. Received
  uint_t micmp_icmpInTimeExcds;      // Time Exceeded Msgs. Received
  uint_t micmp_icmpInParmProbs;      // Param. Prob. Msgs. Received
  uint_t micmp_icmpInSrcQuenchs;     // Source Quench Msgs. Received
  uint_t micmp_icmpInRedirects;      // ICMP Redirect Msgs. Received
  uint_t micmp_icmpInEchos;          // Echo Request Msgs. Received
  uint_t micmp_icmpInEchoReps;       // Echo Reply Msgs. Received
  uint_t micmp_icmpInTimestamps;     // Timestamp Msgs. Received
  uint_t micmp_icmpInTimestampReps;  // Timestamp Reply Msgs. Received
  uint_t micmp_icmpInAddrMasks;      // Addr. Mask Req. Msgs. Received
  uint_t micmp_icmpInAddrMaskReps;   // Addr. Mask Reply Msgs. Received
  uint_t micmp_icmpOutMsgs;          // Messages Transmitted
  uint_t micmp_icmpOutErrors;        // Messages Transmitted for Errors
  uint_t micmp_icmpOutDestUnreachs;  // Dest. Unreach. Msgs. Transmitted
  uint_t micmp_icmpOutTimeExcds;     // Time Exceeded Msgs. Transmitted
  uint_t micmp_icmpOutParmProbs;     // Param. Prob. Msgs. Transmitted
  uint_t micmp_icmpOutSrcQuenchs;    // Source Quench Msgs. Transmitted
  uint_t micmp_icmpOutRedirects;     // ICMP Redirect Msgs. Transmitted
  uint_t micmp_icmpOutEchos;         // Echo Request Msgs. Transmitted
  uint_t micmp_icmpOutEchoReps;      // Echo Reply Msgs. Transmitted
  uint_t micmp_icmpOutTimestamps;    // Timestamp Msgs. Transmitted
  uint_t micmp_icmpOutTimestampReps; // Timestamp Reply Msgs. Transmitted
  uint_t micmp_icmpOutAddrMasks;     // Addr. Mask Req. Msgs. Transmitted
  uint_t micmp_icmpOutAddrMaskReps;  // Addr. Mask Reply Msgs. Transmitted

  // constructors/destructors
  mdr_icmp_t(const char *remote_host = 0);
 ~mdr_icmp_t();

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
  mib::snmp_mib2_icmp_t mib2_icmp;
#endif

  uint_t old_icmpInMsgs;
  uint_t old_icmpInErrors;
  uint_t old_icmpInDestUnreachs;
  uint_t old_icmpInTimeExcds;
  uint_t old_icmpInParmProbs;
  uint_t old_icmpInSrcQuenchs;
  uint_t old_icmpInRedirects;
  uint_t old_icmpInEchos;
  uint_t old_icmpInEchoReps;
  uint_t old_icmpInTimestamps;
  uint_t old_icmpInTimestampReps;
  uint_t old_icmpInAddrMasks;
  uint_t old_icmpInAddrMaskReps;
  uint_t old_icmpOutMsgs;
  uint_t old_icmpOutErrors;
  uint_t old_icmpOutDestUnreachs;
  uint_t old_icmpOutTimeExcds;
  uint_t old_icmpOutParmProbs;
  uint_t old_icmpOutSrcQuenchs;
  uint_t old_icmpOutRedirects;
  uint_t old_icmpOutEchos;
  uint_t old_icmpOutEchoReps;
  uint_t old_icmpOutTimestamps;
  uint_t old_icmpOutTimestampReps;
  uint_t old_icmpOutAddrMasks;
  uint_t old_icmpOutAddrMaskReps;

  void icmp_remote_refresh(void);
};

} // end of namespace mdr

#endif
