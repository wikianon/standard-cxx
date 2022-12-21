/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector plugin for Windows.
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

package com.capmet.plugins.CaptiveCollector.Windows;

import java.util.TreeMap;
import com.capmet.metrics.rmi.CategoryClone;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.plugins.CaptiveCollector.Windows.WindowsMetrics;

public class WindowsCategory extends CategoryClone
{
    public WindowsCategory(String category,
        TreeMap<String, String> localToEnUsMap,
        TreeMap<String, String> enUsToLocalMap)
            throws PerformanceException
    {
        super(category);

        if (category.equals(WindowsMetrics.SYSTEM_INFO))
            categoryObj = new SystemInfo(localToEnUsMap, enUsToLocalMap);
        else if (category.equals(WindowsMetrics.DISK_INFO))
            categoryObj = new DiskInfo(localToEnUsMap, enUsToLocalMap);
        else if (category.equals(WindowsMetrics.NETIF_INFO))
            categoryObj = new NetifInfo(localToEnUsMap, enUsToLocalMap);
        else if (category.equals(WindowsMetrics.TCPMIB_INFO))
            categoryObj = new TcpMibInfo(localToEnUsMap, enUsToLocalMap);
        else
            throw new PerformanceException(
                "WindowsCategory::WindowsCategory: " + category + " " +
                    WindowsMetrics.msgs.getString("unknown_category"));
    }
}
