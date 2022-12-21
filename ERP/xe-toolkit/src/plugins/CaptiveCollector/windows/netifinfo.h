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


#ifndef _NETIFINFO_H_
#define _NETIFINFO_H_

#include "stdafx.h"
#include <mdr.h>
#include <map>
#include <string>

class iinfo_can_t {
public:
	std::string serviceName;
	std::string ipAddress;
	std::string subnetMask;

	void clear(void)
	{
		serviceName.clear();
		ipAddress.clear();
		subnetMask.clear();
	}

	iinfo_can_t()
	{
		clear();
	}
};

// map Description to iinfo_can_t
typedef std::map<std::string, iinfo_can_t> iinfo_box_t;
typedef iinfo_box_t::iterator ibox_node_t;

class netif_info_t : public iinfo_box_t {
public:
	netif_info_t()
	{
		refresh();
	}

	void refresh(void);
	mdr::mdr_keyset_t &getInstanceNames(mdr::mdr_keyset_t &keyset);
private:
	void *read_value(HKEY key, LPCSTR name, DWORD type, PBYTE buf, DWORD sz);
	void query_key(HKEY key, mdr::mdr_keyset_t *subkeys, mdr::mdr_keyset_t *values);
};

#endif
