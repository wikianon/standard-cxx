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
import com.capmet.metrics.CmFieldType;
import com.capmet.metrics.rmi.FieldSource;
import com.capmet.metrics.rmi.CategoryClone;
import com.capmet.metrics.rmi.CategorySource;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.plugins.CaptiveCollector.Windows.WinPluginField;
import com.capmet.plugins.CaptiveCollector.Windows.NetifInfoCan;

public class NetifInfo implements CategorySource
{
    private static final int NI_IP_ADDRESS  = 0;
    private static final int NI_SUBNET_MASK = 1;
    private static final int NI_FIELD_COUNT = 2;

    private static String[] enUsFieldNames = null;

    private static final String[] fieldNames = new String[] {
        WindowsMetrics.msgs.getString("IP_Address"),
        WindowsMetrics.msgs.getString("Subnet_Mask")
    };
    private static final String[] fieldExplanations = new String[] {
        WindowsMetrics.msgs.getString("IP_Address_Exp"),
        WindowsMetrics.msgs.getString("Subnet_Mask_Exp")
    };

    /**
     * handle to native data structure
     */
    private long handle;
    /**
     * failure reason returned from native land
     */
    private String reason;
    /*
     * For retrieving raw values into WinPluginField
     */
    private WinPluginCat pluginCat;

    private class NetifCat extends WinPluginCat {
        public NetifCat() {
            super(WindowsMetrics.NETIF_INFO);
        }

        public Object getRawValue(int fieldNumber, String instance)
            throws PerformanceException
        {
            NetifInfoCan can = NetifInfo.this.netifInfoInstance(instance);

            if (can == null)
                throw new PerformanceException(NetifInfo.this.reason);

            switch(fieldNumber) {
            case NI_IP_ADDRESS:
                return can.ipAddress;
            case NI_SUBNET_MASK:
                return can.subnetMask;
            default:
                throw new PerformanceException(
                    "NetifInfo::getRawValue: " + fieldNumber +
                        WindowsMetrics.msgs.getString("out_of_range") +
                            " (0.." + (NI_FIELD_COUNT - 1) + ")");
            }
        }

        public void categoryRefresh() throws PerformanceException {
            NetifInfo.this.categoryRefresh();
        }
    }

    /**
     * native side of constructor
     */
    private native boolean netifInfoConstructor();

    public NetifInfo(
        TreeMap<String, String> localToEnUsMap,
        TreeMap<String, String> enUsToLocalMap)
            throws PerformanceException
    {
        if (enUsFieldNames == null && !WindowsMetrics.isEnUsLocale)
        {
            enUsFieldNames = new String[] {
                WindowsMetrics.enUsMsgs.getString("IP_Address"),
                WindowsMetrics.enUsMsgs.getString("Subnet_Mask")
            };

            for(int i=0; i<fieldNames.length; i++)
            {
                localToEnUsMap.put(fieldNames[i], enUsFieldNames[i]);
                enUsToLocalMap.put(enUsFieldNames[i], fieldNames[i]);
            }
        }

        pluginCat = new NetifCat();

        if (!netifInfoConstructor())
            throw new PerformanceException(reason);
    }

    public String getCategoryName()
    {
        return WindowsMetrics.NETIF_INFO;
    }

    public boolean isCategorySupported()
    {
        return true;
    }

    public boolean isSingleton()
    {
        return false;
    }

    public int getFieldCount()
    {
        return NI_FIELD_COUNT;
    }

    public boolean isFieldSupported(int fieldNumber)
        throws PerformanceException
    {
        return true;
    }

    public String getFieldName(int fieldNumber)
        throws PerformanceException
    {
        if (fieldNumber < 0 || fieldNumber >= NI_FIELD_COUNT)
            throw new PerformanceException(
                "NetifInfo::getFieldName: " + fieldNumber +
                    WindowsMetrics.msgs.getString("out_of_range") +
                        " (0.." + (NI_FIELD_COUNT - 1) + ")");

        return fieldNames[fieldNumber];
    }

    public String getFieldExplanation(String fieldName)
        throws PerformanceException
    {
        return getFieldExplanation(getFieldNumber(fieldName));
    }

    public String getFieldExplanation(int fieldNumber)
        throws PerformanceException
    {
        if (fieldNumber < 0 || fieldNumber >= NI_FIELD_COUNT)
            throw new PerformanceException(
                "NetifInfo::getFieldExplanation: " + fieldNumber +
                    WindowsMetrics.msgs.getString("out_of_range") +
                        " (0.." + (NI_FIELD_COUNT - 1) + ")");

        return fieldExplanations[fieldNumber];
    }

    public String getCategoryExplanation()
        throws PerformanceException
    {
        return WindowsMetrics.msgs.getString("NetifInfo_Explain");
    }

    public int getFieldNumber(String fieldName)
        throws PerformanceException
    {
        for(int i=0; i<NI_FIELD_COUNT; i++)
            if (fieldName.equals(fieldNames[i]))
                return i;

        throw new PerformanceException(
            "NetifInfo::getFieldNumber: " + fieldName + " " +
                WindowsMetrics.msgs.getString("unknown_field"));
    }

    public FieldSource getField(int fieldNumber)
        throws PerformanceException
    {
        throw new PerformanceException(
            "NetifInfo::getField: NetifInfo " +
                WindowsMetrics.msgs.getString("n_tuple"));
    }

    public FieldSource getField(String fieldName)
        throws PerformanceException
    {
        throw new PerformanceException(
            "NetifInfo::getField: NetifInfo " +
                WindowsMetrics.msgs.getString("n_tuple"));
    }

    private native NetifInfoCan netifInfoInstance(String instanceName);

    public FieldSource getField(int fieldNumber, String instanceName)
        throws PerformanceException
    {
        NetifInfoCan can = netifInfoInstance(instanceName);

        if (can == null)
            throw new PerformanceException(reason);

        switch(fieldNumber)
        {
        case NI_IP_ADDRESS:
            return new WinPluginField(
                NI_IP_ADDRESS, CmFieldType.PF_STRING, instanceName, pluginCat);
        case NI_SUBNET_MASK:
            return new WinPluginField(
                NI_SUBNET_MASK, CmFieldType.PF_STRING, instanceName, pluginCat);
        default:
            throw new PerformanceException(
                "NetifInfo::getField: " + fieldNumber +
                    WindowsMetrics.msgs.getString("out_of_range") +
                        " (0.." + (NI_FIELD_COUNT - 1) + ")");
        }
    }

    public FieldSource getField(String fieldName, String instanceName)
        throws PerformanceException
    {
        return getField(getFieldNumber(fieldName), instanceName);
    }

    private native String[] netifInfoInstanceNames();

    public String[] getInstanceNames()
        throws PerformanceException
    {
        String[] inst = netifInfoInstanceNames();

        if (inst == null)
            throw new PerformanceException(reason);

        return inst;
    }

    public String[] getAllFieldNames()
        throws PerformanceException
    {
        return fieldNames;
    }

    public String[] getSupportedFieldNames()
        throws PerformanceException
    {
        return fieldNames;
    }

    private native boolean netifInfoRefresh();

    public void categoryRefresh()
        throws PerformanceException
    {
        if (!netifInfoRefresh())
            throw new PerformanceException(reason);
    }

    public void categoryRefreshRates()
        throws PerformanceException
    {
        categoryRefresh();
        // no rates
    }

    static
    {
        System.loadLibrary("CCwinplugin");
    }
}
