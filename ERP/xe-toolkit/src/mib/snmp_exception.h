/****************************************************************************
**
** Copyright (c) 2006-2008 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the snmp library of Captivity.
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


#ifndef _SNMP_EXCEPTION_H_
#define _SNMP_EXCEPTION_H_

#include <cstdio>
#include <cstdarg>
#include <cerrno>
#include <cstring>
#include <string>
#include <exception>

namespace mib {

////////////////////////////////////////////////////////////////////
// snmp_exception code
////////////////////////////////////////////////////////////////////

class snmp_exception_t : public std::exception {
private:
  std::string message;

public:
  snmp_exception_t(const char *fmt, ...)
  {
    va_list arg;
    char buf[BUFSIZ];

    va_start(arg, fmt);
    vsnprintf(buf, sizeof buf, fmt, arg);
    message = buf;
    if (errno) {
      message += ": ";
      message += strerror(errno);
    }
    va_end(arg);
  }

  ~snmp_exception_t() throw()
  {
  }

  const char *what() const throw()
  {
    return message.c_str();
  }
};

}

#endif

