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

import java.util.Locale;
import java.util.ResourceBundle;
import java.util.TreeMap;
import com.capmet.metrics.rmi.CategorySource;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.Captivity.CaptiveCollector.MetricSource;

public class WindowsMetrics implements MetricSource
{
    public static ResourceBundle msgs;
    public static ResourceBundle enUsMsgs;
    public static boolean        isEnUsLocale;

    static
    {
        Locale locale = Locale.getDefault();
        String localeName = locale.toString();

        // grrr
        if (localeName.equals("en"))
            localeName = "en_US";
        
        String enUS = Locale.US.toString();

        isEnUsLocale = localeName.equals(enUS);

        if (isEnUsLocale)
            enUsMsgs = null;
        else
            enUsMsgs = ResourceBundle.getBundle(
                "com.capmet.plugins.CaptiveCollector.Windows.MessagesBundle",
                    Locale.US);
        msgs =
            ResourceBundle.getBundle(
                "com.capmet.plugins.CaptiveCollector.Windows.MessagesBundle");
    }

    private String[]                categoryNames;
    private TreeMap<String, String> localToEnUsMap;
    private TreeMap<String, String> enUsToLocalMap;

    public static String SYSTEM_INFO;
    public static String DISK_INFO;
    public static String NETIF_INFO;
    public static String TCPMIB_INFO;

    public WindowsMetrics()
    {
        SYSTEM_INFO = msgs.getString("System_Info");
        DISK_INFO = msgs.getString("Disk_Info");
        NETIF_INFO = msgs.getString("Netif_Info");
        TCPMIB_INFO = msgs.getString("TcpMib_Info");
        categoryNames = new String[] {
            SYSTEM_INFO,
            DISK_INFO,
            NETIF_INFO,
            TCPMIB_INFO
        };

        if (!isEnUsLocale)
        {
            localToEnUsMap = new TreeMap<String, String>();
            enUsToLocalMap = new TreeMap<String, String>();

            localToEnUsMap.put(SYSTEM_INFO, enUsMsgs.getString("System_Info"));
            localToEnUsMap.put(DISK_INFO,   enUsMsgs.getString("Disk_Info"));
            localToEnUsMap.put(NETIF_INFO,  enUsMsgs.getString("Netif_Info"));
            localToEnUsMap.put(TCPMIB_INFO, enUsMsgs.getString("TcpMib_Info"));

            enUsToLocalMap.put(enUsMsgs.getString("System_Info"), SYSTEM_INFO);
            enUsToLocalMap.put(enUsMsgs.getString("Disk_Info"),   DISK_INFO);
            enUsToLocalMap.put(enUsMsgs.getString("Netif_Info"),  NETIF_INFO);
            enUsToLocalMap.put(enUsMsgs.getString("TcpMib_Info"), TCPMIB_INFO);
        }
    }

    public CategorySource getCategory(String categoryName)
        throws PerformanceException
    {
        if (categoryName.equals(SYSTEM_INFO))
            return new WindowsCategory(
                SYSTEM_INFO, localToEnUsMap, enUsToLocalMap);
        else if (categoryName.equals(DISK_INFO))
            return new WindowsCategory(
                DISK_INFO, localToEnUsMap, enUsToLocalMap);
        else if (categoryName.equals(NETIF_INFO))
            return new WindowsCategory(
                NETIF_INFO, localToEnUsMap, enUsToLocalMap);
        else if (categoryName.equals(TCPMIB_INFO))
            return new WindowsCategory(
                TCPMIB_INFO, localToEnUsMap, enUsToLocalMap);
        else
            throw new PerformanceException(
                "WindowsMetrics::getCategory: " + categoryName + " " +
                    msgs.getString("unknown_category"));
    }

    public String[] getCategoryNames()
        throws PerformanceException
    {
        return categoryNames;
    }

    public void setNamesMaps(
        TreeMap<String, String> localToEnUsMap,
        TreeMap<String, String> enUsToLocalMap)
    {
        this.localToEnUsMap = localToEnUsMap;
        this.enUsToLocalMap = enUsToLocalMap;
    }
}
