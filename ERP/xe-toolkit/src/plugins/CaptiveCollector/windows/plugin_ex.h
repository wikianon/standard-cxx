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


#ifndef _PLUGIN_EX_H_
#define _PLUGIN_EX_H_

#include <string>
#include <exception>

////////////////////////////////////////////////////////////////////
// plugin_exception
////////////////////////////////////////////////////////////////////

class __declspec(dllexport) plugin_exception : public std::exception {
private:
  std::string message;

public:
  plugin_exception(const char *msg)
  {
    message = msg;
  }

  ~plugin_exception() throw()
  {
  }

  const char *what() const throw()
  {
    return message.c_str();
  }
};

#endif
