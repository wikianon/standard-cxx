/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector plugin for Solaris.
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

package com.capmet.plugins.CaptiveCollector.Solaris;

import java.util.Locale;
import java.util.ResourceBundle;
import java.util.TreeMap;
import com.capmet.metrics.rmi.CategorySource;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.Captivity.CaptiveCollector.MetricSource;
import com.capmet.plugins.CaptiveCollector.Solaris.SolarisCategory;

public class SolarisMetrics implements MetricSource
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
                "com.capmet.plugins.CaptiveCollector.Solaris.MessagesBundle",
                    Locale.US);
        msgs =
            ResourceBundle.getBundle(
                "com.capmet.plugins.CaptiveCollector.Solaris.MessagesBundle");
    }

    private String[]                categoryNames;
    private TreeMap<String, String> localToEnUsMap;
    private TreeMap<String, String> enUsToLocalMap;

    public static String PER_CPU_DATA;

    public SolarisMetrics()
    {
        PER_CPU_DATA = msgs.getString("SunOS_PER_CPU_Data");
        categoryNames = new String[] {
            PER_CPU_DATA
        };

        if (!isEnUsLocale)
        {
            localToEnUsMap = new TreeMap<String, String>();
            enUsToLocalMap = new TreeMap<String, String>();

            localToEnUsMap.put(PER_CPU_DATA,
                enUsMsgs.getString("SunOS_PER_CPU_Data"));

            enUsToLocalMap.put(enUsMsgs.getString("SunOS_PER_CPU_Data"),
                PER_CPU_DATA);
        }
    }

    public CategorySource getCategory(String categoryName)
        throws PerformanceException
    {
        if (categoryName.equals(PER_CPU_DATA))
            return new SolarisCategory(PER_CPU_DATA,
                localToEnUsMap, enUsToLocalMap);
        else
            throw new PerformanceException(
                "SolarisMetrics::getCategory: " + categoryName + " " +
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
