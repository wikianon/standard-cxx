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

public class SystemInfo implements CategorySource
{
    private static final int SI_PF_TOTAL_MB  = 0;
    private static final int SI_MEM_TOTAL_MB = 1;
    private static final int SI_CLOCK_TICK   = 2;
    private static final int SI_OS_PLATFORM  = 3;
    private static final int SI_OS_VERSION   = 4;
    private static final int SI_PAGE_SIZE    = 5;
    private static final int SI_CPU_ARCH     = 6;
    private static final int SI_CPU_MODEL    = 7;
    private static final int SI_CPUS_CONFIG  = 8;
    private static final int SI_CPUS_ONLINE  = 9;
    private static final int SI_CLOCK_SPEED  = 10;
    private static final int SI_FIELD_COUNT  = 11;

    private static String[] enUsFieldNames = null;

    private static final String[] fieldNames = new String[] {
        WindowsMetrics.msgs.getString("PF_Total"),
        WindowsMetrics.msgs.getString("Physmem"),
        WindowsMetrics.msgs.getString("Clock_Ticks"),
        WindowsMetrics.msgs.getString("OS_Platform"),
        WindowsMetrics.msgs.getString("OS_Version"),
        WindowsMetrics.msgs.getString("Page_Size"),
        WindowsMetrics.msgs.getString("CPU_Arch"),
        WindowsMetrics.msgs.getString("CPU_Model"),
        WindowsMetrics.msgs.getString("CPUs_Conf"),
        WindowsMetrics.msgs.getString("CPUs_Online"),
        WindowsMetrics.msgs.getString("CPU_Speed")
    };
    private static final String[] fieldExplanations = new String[] {
        WindowsMetrics.msgs.getString("PF_Total_Exp"),
        WindowsMetrics.msgs.getString("Physmem_Exp"),
        WindowsMetrics.msgs.getString("Clock_Ticks_Exp"),
        WindowsMetrics.msgs.getString("OS_Platform_Exp"),
        WindowsMetrics.msgs.getString("OS_Version_Exp"),
        WindowsMetrics.msgs.getString("Page_Size_Exp"),
        WindowsMetrics.msgs.getString("CPU_Arch_Exp"),
        WindowsMetrics.msgs.getString("CPU_Model_Exp"),
        WindowsMetrics.msgs.getString("CPUs_Conf_Exp"),
        WindowsMetrics.msgs.getString("CPUs_Online_Exp"),
        WindowsMetrics.msgs.getString("CPU_Speed_Exp")
    };

    /**
     * Total size of the all paging space in megabytes.
     */
    private int    MBytesPagingFileTotal;
    /**
     * Total size of physical memory in megabytes.
     */
    private int    MBytesMemoryTotal;
    /**
     * Number of clock interrupts sent to the CPU per second.
     */
    private int    clockTicksPerSecond;
    /**
     * Name of the operating system product.
     */
    private String osPlatform;
    /**
     * Version string of the operating system.
     */
    private String osVersion;
    /**
     * Size of a page of physical memory in bytes.
     */
    private int    pageSize;
    /**
     * Architecture name of the CPU.
     */
    private String cpuArchitecture;
    /**
     * Model name of the CPU.
     */
    private String cpuModel;
    /**
     * Number of CPUs configured present in the system.
     */
    private int    cpuCountConfigured;
    /**
     * Number of CPUs that are configured for use by the operating system.
     */
    private int    cpuCountOnline;
    /**
     * Clock speed of the CPU in megahertz.
     */
    private int    cpuClockSpeed;

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

    private class SysInfoCat extends WinPluginCat {
        public SysInfoCat() {
            super(WindowsMetrics.SYSTEM_INFO);
        }

        public Object getRawValue(int fieldNumber)
            throws PerformanceException
        {
            switch(fieldNumber) {
            case SI_PF_TOTAL_MB:
                return new Integer(MBytesPagingFileTotal);
            case SI_MEM_TOTAL_MB:
                return new Integer(MBytesMemoryTotal);
            case SI_CLOCK_TICK:
                return new Integer(clockTicksPerSecond);
            case SI_OS_PLATFORM:
                return osPlatform;
            case SI_OS_VERSION:
                return osVersion;
            case SI_PAGE_SIZE:
                return new Integer(pageSize);
            case SI_CPU_ARCH:
                return cpuArchitecture;
            case SI_CPU_MODEL:
                return cpuModel;
            case SI_CPUS_CONFIG:
                return new Integer(cpuCountConfigured);
            case SI_CPUS_ONLINE:
                return new Integer(cpuCountOnline);
            case SI_CLOCK_SPEED:
                return new Integer(cpuClockSpeed);
            default:
                throw new PerformanceException(
                    "SystemInfo::getRawValue: field number: " +
                        fieldNumber + " out of range for (0.." +
                            (SI_FIELD_COUNT - 1) + ")");
            }
        }

        public void categoryRefresh() throws PerformanceException {
            SystemInfo.this.categoryRefresh();
        }
    }

    /**
     * native side of constructor
     */
    private native boolean sysInfoConstructor();

    public SystemInfo(
        TreeMap<String, String> localToEnUsMap,
        TreeMap<String, String> enUsToLocalMap)
            throws PerformanceException
    {
        if (enUsFieldNames == null && !WindowsMetrics.isEnUsLocale)
        {
            enUsFieldNames = new String[] {
                WindowsMetrics.enUsMsgs.getString("PF_Total"),
                WindowsMetrics.enUsMsgs.getString("Physmem"),
                WindowsMetrics.enUsMsgs.getString("Clock_Ticks"),
                WindowsMetrics.enUsMsgs.getString("OS_Platform"),
                WindowsMetrics.enUsMsgs.getString("OS_Version"),
                WindowsMetrics.enUsMsgs.getString("Page_Size"),
                WindowsMetrics.enUsMsgs.getString("CPU_Arch"),
                WindowsMetrics.enUsMsgs.getString("CPU_Model"),
                WindowsMetrics.enUsMsgs.getString("CPUs_Conf"),
                WindowsMetrics.enUsMsgs.getString("CPUs_Online"),
                WindowsMetrics.enUsMsgs.getString("CPU_Speed")
            };

            for(int i=0; i<fieldNames.length; i++)
            {
                localToEnUsMap.put(fieldNames[i], enUsFieldNames[i]);
                enUsToLocalMap.put(enUsFieldNames[i], fieldNames[i]);
            }
        }

        pluginCat = new SysInfoCat();

        if (!sysInfoConstructor())
            throw new PerformanceException(reason);
    }

    public String getCategoryName()
    {
        return WindowsMetrics.SYSTEM_INFO;
    }

    public boolean isCategorySupported()
    {
        return true;
    }

    public boolean isSingleton()
    {
        return true;
    }

    public int getFieldCount()
    {
        return SI_FIELD_COUNT;
    }

    public boolean isFieldSupported(int fieldNumber)
        throws PerformanceException
    {
        return true;
    }

    public String getFieldName(int fieldNumber)
        throws PerformanceException
    {
        if (fieldNumber < 0 || fieldNumber >= SI_FIELD_COUNT)
            throw new PerformanceException(
                "SystemInfo::getFieldName: index " + fieldNumber +
                    "out of range for (0.." + (SI_FIELD_COUNT - 1) + ")");

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
        if (fieldNumber < 0 || fieldNumber >= SI_FIELD_COUNT)
            throw new PerformanceException(
                "SystemInfo::getFieldName: index " + fieldNumber +
                    " out of range for (0.." + (SI_FIELD_COUNT - 1) + ")");

        return fieldExplanations[fieldNumber];
    }

    public String getCategoryExplanation()
        throws PerformanceException
    {
        return WindowsMetrics.msgs.getString("SysInfo_Explain");
    }

    public int getFieldNumber(String fieldName)
        throws PerformanceException
    {
        for(int i=0; i<SI_FIELD_COUNT; i++)
            if (fieldName.equals(fieldNames[i]))
                return i;

        throw new PerformanceException(
            "SystemInfo::getFieldNumber: unknown field name: " + fieldName);
    }

    public FieldSource getField(int fieldNumber)
        throws PerformanceException
    {
        switch(fieldNumber)
        {
        case SI_PF_TOTAL_MB:
            return new WinPluginField(
                SI_PF_TOTAL_MB, CmFieldType.PF_INT, pluginCat);
        case SI_MEM_TOTAL_MB:
            return new WinPluginField(
                SI_MEM_TOTAL_MB, CmFieldType.PF_INT, pluginCat);
        case SI_CLOCK_TICK:
            return new WinPluginField(
                SI_CLOCK_TICK, CmFieldType.PF_INT, pluginCat);
        case SI_OS_PLATFORM:
            return new WinPluginField(
                SI_OS_PLATFORM, CmFieldType.PF_STRING, pluginCat);
        case SI_OS_VERSION:
            return new WinPluginField(
                SI_OS_VERSION, CmFieldType.PF_STRING, pluginCat);
        case SI_PAGE_SIZE:
            return new WinPluginField(
                SI_PAGE_SIZE, CmFieldType.PF_INT, pluginCat);
        case SI_CPU_ARCH:
            return new WinPluginField(
                SI_CPU_ARCH, CmFieldType.PF_STRING, pluginCat);
        case SI_CPU_MODEL:
            return new WinPluginField(
                SI_CPU_MODEL, CmFieldType.PF_STRING, pluginCat);
        case SI_CPUS_CONFIG:
            return new WinPluginField(
                SI_CPUS_CONFIG, CmFieldType.PF_INT, pluginCat);
        case SI_CPUS_ONLINE:
            return new WinPluginField(
                SI_CPUS_ONLINE, CmFieldType.PF_INT, pluginCat);
        case SI_CLOCK_SPEED:
            return new WinPluginField(
                SI_CLOCK_SPEED, CmFieldType.PF_INT, pluginCat);
        default:
            throw new PerformanceException(
                "SystemInfo::getField: field number: " + fieldNumber +
                    " out of range for (0.." + (SI_FIELD_COUNT - 1) + ")");
        }
    }

    public FieldSource getField(String fieldName)
        throws PerformanceException
    {
        return getField(getFieldNumber(fieldName));
    }

    public FieldSource getField(int fieldNumber, String instanceName)
        throws PerformanceException
    {
        throw new PerformanceException(
            "SystemInfo::getField: SystemInfo is a singleton");
    }

    public FieldSource getField(String fieldName, String instanceName)
        throws PerformanceException
    {
        // just a throw
        return getField(-1, null);
    }

    public String[] getInstanceNames()
        throws PerformanceException
    {
        throw new PerformanceException(
            "SystemInfo::getInstanceNames: SystemInfo is a singleton");
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

    private native boolean sysInfoRefresh();

    public void categoryRefresh()
        throws PerformanceException
    {
        if (!sysInfoRefresh())
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
