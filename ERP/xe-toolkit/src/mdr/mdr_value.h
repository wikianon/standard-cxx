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


#ifndef _MDR_VALUE_H_
#define _MDR_VALUE_H_

#include <iostream>
#include "mdr_types.h"

namespace mdr {

/// for the union below
enum mdr_value_type_t {
  MVT_I32,  MVT_UI32,  MVT_I64,    MVT_UI64,
  MVT_LONG, MVT_ULONG, MVT_DOUBLE, MVT_STRING,
  MVT_BOOL
};

// for mdr_field_value to return
struct mdr_value_t {
  mdr_value_type_t mv_type;

  // a union of the possible types for a value
  union {
    int          i32;
    uint32_t     ui32;
    int64_t      i64;
    uint64_t     ui64;
    long         l;
    ulong_t      ul;
    double       d;
    std::string *s;
    bool         b;
  } mv_un;
#define mv_i32  mv_un.i32
#define mv_ui32 mv_un.ui32
#define mv_i64  mv_un.i64
#define mv_ui64 mv_un.ui64
#define mv_l    mv_un.l
#define mv_ul   mv_un.ul
#define mv_d    mv_un.d
#define mv_s    mv_un.s
#define mv_b    mv_un.b
};

extern std::ostream &operator<<(std::ostream &out, const mdr_value_t &v);

}

#endif
