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

public class TcpMibInfo implements CategorySource
{
    private static final int TCP_RTOALGORITHM = 0;
    private static final int TCP_RTOMIN       = 1;
    private static final int TCP_RTOMAX       = 2;
    private static final int TCP_MAXCONN      = 3;
    private static final int TCP_ACTIVEOPENS  = 4;
    private static final int TCP_PASSIVEOPENS = 5;
    private static final int TCP_ATTEMPTFAILS = 6;
    private static final int TCP_ESTABRESETS  = 7;
    private static final int TCP_CURRESTAB    = 8;
    private static final int TCP_INSEGS       = 9;
    private static final int TCP_OUTSEGS      = 10;
    private static final int TCP_RETRANSSEGS  = 11;
    private static final int TCP_INERRS       = 12;
    private static final int TCP_OUTRSTS      = 13;
    private static final int TCP_FIELD_COUNT  = 14;

    private static String[] enUsFieldNames = null;

    private static final String[] fieldNames = new String[] {
        WindowsMetrics.msgs.getString("RtoAlgo"),
        WindowsMetrics.msgs.getString("RtoMin"),
        WindowsMetrics.msgs.getString("RtoMax"),
        WindowsMetrics.msgs.getString("MaxConn"),
        WindowsMetrics.msgs.getString("ActOpen"),
        WindowsMetrics.msgs.getString("PasOpen"),
        WindowsMetrics.msgs.getString("AttFail"),
        WindowsMetrics.msgs.getString("EstReset"),
        WindowsMetrics.msgs.getString("CurrEstab"),
        WindowsMetrics.msgs.getString("InSegs"),
        WindowsMetrics.msgs.getString("OutSegs"),
        WindowsMetrics.msgs.getString("RetSegs"),
        WindowsMetrics.msgs.getString("InErrs"),
        WindowsMetrics.msgs.getString("OutRsts")
    };
    private static final String[] fieldExplanations = new String[] {
        WindowsMetrics.msgs.getString("RtoAlgo_Exp"),
        WindowsMetrics.msgs.getString("RtoMin_Exp"),
        WindowsMetrics.msgs.getString("RtoMax_Exp"),
        WindowsMetrics.msgs.getString("MaxConn_Exp"),
        WindowsMetrics.msgs.getString("ActOpen_Exp"),
        WindowsMetrics.msgs.getString("PasOpen_Exp"),
        WindowsMetrics.msgs.getString("AttFail_Exp"),
        WindowsMetrics.msgs.getString("EstReset_Exp"),
        WindowsMetrics.msgs.getString("CurrEstab_Exp"),
        WindowsMetrics.msgs.getString("InSegs_Exp"),
        WindowsMetrics.msgs.getString("OutSegs_Exp"),
        WindowsMetrics.msgs.getString("RetSegs_Exp"),
        WindowsMetrics.msgs.getString("InErrs_Exp"),
        WindowsMetrics.msgs.getString("OutRsts_Exp")
    };

    /**
     * Retransmit Timeout Algorithm
     */
    private int tcpRtoAlgorithm;
    /**
     * Minimum Retransmission Timeout
     */
    private int tcpRtoMin;
    /**
     * Maximum Retransmission Timeout
     */
    private int tcpRtoMax;
    /**
     * Maximum Connection Count
     */
    private int tcpMaxConn;
    /**
     * Active Opens
     */
    private int tcpActiveOpens;
    /**
     * Passive Opens
     */
    private int tcpPassiveOpens;
    /**
     * Passive Open Failures
     */
    private int tcpAttemptFails;
    /**
     * Connection Resets
     */
    private int tcpEstabResets;
    /**
     * Established Connections
     */
    private int tcpCurrEstab;
    /**
     * Segments Received
     */
    private int tcpInSegs;
    /**
     * Segments Transmitted
     */
    private int tcpOutSegs;
    /**
     * Segments Retransmitted
     */
    private int tcpRetransSegs;
    /**
     * Segments Received with Error
     */
    private int tcpInErrs;
    /**
     * RST Segments Transmitted
     */
    private int tcpOutRsts;

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

    private class TcpCat extends WinPluginCat {
        public TcpCat() {
            super(WindowsMetrics.TCPMIB_INFO);
        }

        public Object getRawValue(int fieldNumber)
            throws PerformanceException
        {
            switch(fieldNumber) {
            case TCP_RTOALGORITHM:
                return new Integer(tcpRtoAlgorithm);
            case TCP_RTOMIN:
                return new Integer(tcpRtoMin);
            case TCP_RTOMAX:
                return new Integer(tcpRtoMax);
            case TCP_MAXCONN:
                return new Integer(tcpMaxConn);
            case TCP_ACTIVEOPENS:
                return new Integer(tcpActiveOpens);
            case TCP_PASSIVEOPENS:
                return new Integer(tcpPassiveOpens);
            case TCP_ATTEMPTFAILS:
                return new Integer(tcpAttemptFails);
            case TCP_ESTABRESETS:
                return new Integer(tcpEstabResets);
            case TCP_CURRESTAB:
                return new Integer(tcpCurrEstab);
            case TCP_INSEGS:
                return new Integer(tcpInSegs);
            case TCP_OUTSEGS:
                return new Integer(tcpOutSegs);
            case TCP_RETRANSSEGS:
                return new Integer(tcpRetransSegs);
            case TCP_INERRS:
                return new Integer(tcpInErrs);
            case TCP_OUTRSTS:
                return new Integer(tcpOutRsts);
            default:
                throw new PerformanceException(
                    "TcpMibInfo::getRawValue: field number: " +
                         fieldNumber + " out of range for (0.." +
                             (TCP_FIELD_COUNT - 1) + ")");
            }
        }

        public void categoryRefresh() throws PerformanceException {
            TcpMibInfo.this.categoryRefresh();
        }
    }

    /**
     * native side of constructor
     */
    private native boolean tcpInfoConstructor();

    public TcpMibInfo(
        TreeMap<String, String> localToEnUsMap,
        TreeMap<String, String> enUsToLocalMap)
            throws PerformanceException
    {
        if (enUsFieldNames == null && !WindowsMetrics.isEnUsLocale)
        {
            enUsFieldNames = new String[] {
                WindowsMetrics.enUsMsgs.getString("RtoAlgo"),
                WindowsMetrics.enUsMsgs.getString("RtoMin"),
                WindowsMetrics.enUsMsgs.getString("RtoMax"),
                WindowsMetrics.enUsMsgs.getString("MaxConn"),
                WindowsMetrics.enUsMsgs.getString("ActOpen"),
                WindowsMetrics.enUsMsgs.getString("PasOpen"),
                WindowsMetrics.enUsMsgs.getString("AttFail"),
                WindowsMetrics.enUsMsgs.getString("EstReset"),
                WindowsMetrics.enUsMsgs.getString("CurrEstab"),
                WindowsMetrics.enUsMsgs.getString("InSegs"),
                WindowsMetrics.enUsMsgs.getString("OutSegs"),
                WindowsMetrics.enUsMsgs.getString("RetSegs"),
                WindowsMetrics.enUsMsgs.getString("InErrs"),
                WindowsMetrics.enUsMsgs.getString("OutRsts")
            };

            for(int i=0; i<fieldNames.length; i++)
            {
                localToEnUsMap.put(fieldNames[i], enUsFieldNames[i]);
                enUsToLocalMap.put(enUsFieldNames[i], fieldNames[i]);
            }
        }

        pluginCat = new TcpCat();

        if (!tcpInfoConstructor())
            throw new PerformanceException(reason);
    }

    public String getCategoryName()
    {
        return WindowsMetrics.TCPMIB_INFO;
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
        return TCP_FIELD_COUNT;
    }

    public boolean isFieldSupported(int fieldNumber)
        throws PerformanceException
    {
        return true;
    }

    public String getFieldName(int fieldNumber)
        throws PerformanceException
    {
        if (fieldNumber < 0 || fieldNumber >= TCP_FIELD_COUNT)
            throw new PerformanceException(
                "TcpMibInfo::getFieldName: index " + fieldNumber +
                    "out of range for (0.." + (TCP_FIELD_COUNT - 1) + ")");

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
        if (fieldNumber < 0 || fieldNumber >= TCP_FIELD_COUNT)
            throw new PerformanceException(
                "TcpMibInfo::getFieldName: index " + fieldNumber +
                    " out of range for (0.." + (TCP_FIELD_COUNT - 1) + ")");

        return fieldExplanations[fieldNumber];
    }

    public String getCategoryExplanation()
        throws PerformanceException
    {
        return WindowsMetrics.msgs.getString("TcpMibInfo_Explain");
    }

    public int getFieldNumber(String fieldName)
        throws PerformanceException
    {
        for(int i=0; i<TCP_FIELD_COUNT; i++)
            if (fieldName.equals(fieldNames[i]))
                return i;

        throw new PerformanceException(
            "TcpMibInfo::getFieldNumber: unknown field name: " + fieldName);
    }

    public FieldSource getField(int fieldNumber)
        throws PerformanceException
    {
        switch(fieldNumber)
        {
        case TCP_RTOALGORITHM:
            return new WinPluginField(
                TCP_RTOALGORITHM, CmFieldType.PF_INT, pluginCat);
        case TCP_RTOMIN:
            return new WinPluginField(
                TCP_RTOMIN, CmFieldType.PF_INT, pluginCat);
        case TCP_RTOMAX:
            return new WinPluginField(
                TCP_RTOMAX, CmFieldType.PF_INT, pluginCat);
        case TCP_MAXCONN:
            return new WinPluginField(
                TCP_MAXCONN, CmFieldType.PF_INT, pluginCat);
        case TCP_ACTIVEOPENS:
            return new WinPluginField(
                TCP_ACTIVEOPENS, CmFieldType.PF_INT, pluginCat);
        case TCP_PASSIVEOPENS:
            return new WinPluginField(
                TCP_PASSIVEOPENS, CmFieldType.PF_INT, pluginCat);
        case TCP_ATTEMPTFAILS:
            return new WinPluginField(
                TCP_ATTEMPTFAILS, CmFieldType.PF_INT, pluginCat);
        case TCP_ESTABRESETS:
            return new WinPluginField(
                TCP_ESTABRESETS, CmFieldType.PF_INT, pluginCat);
        case TCP_CURRESTAB:
            return new WinPluginField(
                TCP_CURRESTAB, CmFieldType.PF_INT, pluginCat);
        case TCP_INSEGS:
            return new WinPluginField(
                TCP_INSEGS, CmFieldType.PF_INT, pluginCat);
        case TCP_OUTSEGS:
            return new WinPluginField(
                TCP_OUTSEGS, CmFieldType.PF_INT, pluginCat);
        case TCP_RETRANSSEGS:
            return new WinPluginField(
                TCP_RETRANSSEGS, CmFieldType.PF_INT, pluginCat);
        case TCP_INERRS:
            return new WinPluginField(
                TCP_INERRS, CmFieldType.PF_INT, pluginCat);
        case TCP_OUTRSTS:
            return new WinPluginField(
                TCP_OUTRSTS, CmFieldType.PF_INT, pluginCat);
        default:
            throw new PerformanceException(
                "TcpMibInfo::getField: field number: " + fieldNumber +
                    " out of range for (0.." + (TCP_FIELD_COUNT - 1) + ")");
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
            "TcpMibInfo::getField: TcpMibInfo is a singleton");
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
            "TcpMibInfo::getInstanceNames: TcpMibInfo is a singleton");
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

    private native boolean tcpMibInfoRefresh();

    public void categoryRefresh()
        throws PerformanceException
    {
        if (!tcpMibInfoRefresh())
            throw new PerformanceException(reason);
    }

    private native boolean tcpMibInfoRefreshRates();

    public void categoryRefreshRates()
        throws PerformanceException
    {
        if (!tcpMibInfoRefreshRates())
            throw new PerformanceException(reason);
    }

    static
    {
        System.loadLibrary("CCwinplugin");
    }
}
