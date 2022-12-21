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


#ifndef _MDR_SYSCTL_H_
#define _MDR_SYSCTL_H_

#include <sys/sysctl.h>
#include "mdr_types.h"
#include "mdr_exception.h"

template <typename sys_type>
class mdr_sysctl_t {
private:
  sys_type value;
public:
  mdr_sysctl_t(int mib0, int mib1)
  {
    size_t len;
    int mib[4];

    mib[0] = mib0;
    mib[1] = mib1;
    mib[2] = 0;
    mib[3] = 0;
    len = sizeof(sys_type);
    if (sysctl(mib, 2, &value, &len, 0, 0) == -1)
      throw mdr::mdr_exception_t(
        "mdr_sysctl_t::mdr_sysctl_t: sysctl() failed");
  }

  const sys_type operator*(void)
  {
    return value;
  }

  const sys_type *operator->(void)
  {
    return &value;
  }
};

#endif
