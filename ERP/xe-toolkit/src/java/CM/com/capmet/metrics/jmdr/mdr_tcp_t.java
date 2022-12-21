/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Captive Metrics library of Captivity.
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

package com.capmet.metrics.jmdr;

import com.capmet.metrics.jmdr.mdr_t;
import com.capmet.metrics.jmdr.mdr_exception_t;


/**
 * The mdr_tcp_t class provides data from 
 * <p>
 * Methods in this class are <code>refresh, rates</code> and
 * <code>refresh_rates</code> for updating the variable, computing rates
 * for those values that are rates and both respectively.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_tcp_t extends mdr_t
{
    // fields by the number
    public static final int MTCP_TCPRTOALGORITHM = 0;
    public static final int MTCP_TCPRTOMIN       = 1;
    public static final int MTCP_TCPRTOMAX       = 2;
    public static final int MTCP_TCPMAXCONN      = 3;
    public static final int MTCP_TCPACTIVEOPENS  = 4;
    public static final int MTCP_TCPPASSIVEOPENS = 5;
    public static final int MTCP_TCPATTEMPTFAILS = 6;
    public static final int MTCP_TCPESTABRESETS  = 7;
    public static final int MTCP_TCPCURRESTAB    = 8;
    public static final int MTCP_TCPINSEGS       = 9;
    public static final int MTCP_TCPOUTSEGS      = 10;
    public static final int MTCP_TCPRETRANSSEGS  = 11;
    public static final int MTCP_TCPINERRS       = 12;
    public static final int MTCP_TCPOUTRSTS      = 13;

    /**
     * Timeout Algorithm
     */
    public int mtcp_tcpRtoAlgorithm;
    /**
     * Minimum Retransmission Timeout
     */
    public int mtcp_tcpRtoMin;
    /**
     * Maximum Retransmission Timeout
     */
    public int mtcp_tcpRtoMax;
    /**
     * Maximum Connection Count
     */
    public int mtcp_tcpMaxConn;
    /**
     * Active Opens
     */
    public int mtcp_tcpActiveOpens;
    /**
     * Passive Opens
     */
    public int mtcp_tcpPassiveOpens;
    /**
     * Open Failures
     */
    public int mtcp_tcpAttemptFails;
    /**
     * Connection Resets
     */
    public int mtcp_tcpEstabResets;
    /**
     * Established Connections
     */
    public int mtcp_tcpCurrEstab;
    /**
     * Segments Received
     */
    public int mtcp_tcpInSegs;
    /**
     * Segments Transmitted
     */
    public int mtcp_tcpOutSegs;
    /**
     * Segments Retransmitted
     */
    public int mtcp_tcpRetransSegs;
    /**
     * Segments Received with Error
     */
    public int mtcp_tcpInErrs;
    /**
     * RST Segments Transmitted
     */
    public int mtcp_tcpOutRsts;

    /**
     * Call the constructor for the C++ class
     */
    private native boolean tcp_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean tcp_refresh();
    
    /**
     * Call the rates method in the C++ class
     */
    private native boolean tcp_rates();
    
    /**
     * mdr_tcp_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_tcp_t() throws mdr_exception_t
    {
        if (!tcp_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    /**
     * mdr_tcp_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_tcp_t(String remote_host) throws mdr_exception_t
    {
        if (!tcp_constructor(remote_host))
            throw new mdr_exception_t(reason);
    }

    /**
     * Wrap things up in the JNI code.
     */
    protected native void finalize();
    
    /**
     * Update the variable with the latest values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_refresh() throws mdr_exception_t
    {
        if (!tcp_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
        if (!tcp_rates())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Get the current value of the performance field.
     * @param field_number The numeric field ID of the field to retrieve.
     * @throws mdr_exception_t Thrown from JNI code
     * @return <CODE>Object</CODE> value of the performance field.
     */
    public Object mdr_field_value(int field_number) throws mdr_exception_t
    {
        switch(field_number)
        {
        case MTCP_TCPRTOALGORITHM:
            return new Integer(mtcp_tcpRtoAlgorithm);
        case MTCP_TCPRTOMIN:
            return new Integer(mtcp_tcpRtoMin);
        case MTCP_TCPRTOMAX:
            return new Integer(mtcp_tcpRtoMax);
        case MTCP_TCPMAXCONN:
            return new Integer(mtcp_tcpMaxConn);
        case MTCP_TCPACTIVEOPENS:
            return new Integer(mtcp_tcpActiveOpens);
        case MTCP_TCPPASSIVEOPENS:
            return new Integer(mtcp_tcpPassiveOpens);
        case MTCP_TCPATTEMPTFAILS:
            return new Integer(mtcp_tcpAttemptFails);
        case MTCP_TCPESTABRESETS:
            return new Integer(mtcp_tcpEstabResets);
        case MTCP_TCPCURRESTAB:
            return new Integer(mtcp_tcpCurrEstab);
        case MTCP_TCPINSEGS:
            return new Integer(mtcp_tcpInSegs);
        case MTCP_TCPOUTSEGS:
            return new Integer(mtcp_tcpOutSegs);
        case MTCP_TCPRETRANSSEGS:
            return new Integer(mtcp_tcpRetransSegs);
        case MTCP_TCPINERRS:
            return new Integer(mtcp_tcpInErrs);
        case MTCP_TCPOUTRSTS:
            return new Integer(mtcp_tcpOutRsts);
        default:
            throw new mdr_exception_t("mdr_tcp_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }
    
    static
    {
        System.loadLibrary("jmdr");
    }
}
