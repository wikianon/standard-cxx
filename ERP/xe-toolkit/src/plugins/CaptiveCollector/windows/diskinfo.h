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


#ifndef _DISKINFO_H_
#define _DISKINFO_H_

#include "stdafx.h"
#include <mdr.h>
#include <map>
#include <string>

class dinfo_can_t {
public:
    std::string diskType;
    ULONG       capacityMB;

    void clear()
    {
        diskType.clear();
        capacityMB = 0;
    }

    dinfo_can_t()
    {
        clear();
    }
};

typedef std::map<std::string, dinfo_can_t> dinfo_box_t;
typedef dinfo_box_t::iterator dbox_node_t;

class diskinfo_t : public dinfo_box_t {
public:
    diskinfo_t()
    {
        refresh();
    }

    void refresh(void);
    mdr::mdr_keyset_t &getInstanceNames(mdr::mdr_keyset_t &keyset);
};

#endif
