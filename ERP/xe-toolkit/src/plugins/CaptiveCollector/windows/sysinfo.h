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


#ifndef _SYSINFO_H_
#define _SYSINFO_H_

#include "Stdafx.h"
#include "CCwinplugin.h"
#include <string>

class CCWINPLUGIN_API sysinfo_t {
public:
  int         MBytesPagingFileTotal;
  int         MBytesMemoryTotal;
  int         clockTicksPerSecond;
  std::string osPlatform;
  std::string osVersion;
  int         pageSize;
  std::string cpuArchitecture;
  std::string cpuModel;
  int         cpuCountConfigured;
  int         cpuCountOnline;
  int         cpuClockSpeed;

  sysinfo_t();
  void refresh(void);

private:
  void *read_value(HKEY key, LPCSTR name, DWORD type, char buf[], DWORD sz);
  void trimout(void *s, DWORD sz);
};

#endif
