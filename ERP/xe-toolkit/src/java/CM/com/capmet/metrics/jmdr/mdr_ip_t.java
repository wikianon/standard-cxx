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
 * The mdr_ip_t class provides data from 
 * <p>
 * Methods in this class are <code>refresh, rates</code> and
 * <code>refresh_rates</code> for updating the variable, computing rates
 * for those values that are rates and both respectively.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_ip_t extends mdr_t
{
    // fields by the number
    public static final int MIP_IPFORWARDING      = 0;
    public static final int MIP_IPDEFAULTTTL      = 1;
    public static final int MIP_IPINRECEIVES      = 2;
    public static final int MIP_IPINHDRERRORS     = 3;
    public static final int MIP_IPINADDRERRORS    = 4;
    public static final int MIP_IPFORWDATAGRAMS   = 5;
    public static final int MIP_IPINUNKNOWNPROTOS = 6;
    public static final int MIP_IPINDISCARDS      = 7;
    public static final int MIP_IPINDELIVERS      = 8;
    public static final int MIP_IPOUTREQUESTS     = 9;
    public static final int MIP_IPOUTDISCARDS     = 10;
    public static final int MIP_IPOUTNOROUTES     = 11;
    public static final int MIP_IPREASMTIMEOUT    = 12;
    public static final int MIP_IPREASMREQDS      = 13;
    public static final int MIP_IPREASMOKS        = 14;
    public static final int MIP_IPREASMFAILS      = 15;
    public static final int MIP_IPFRAGOKS         = 16;
    public static final int MIP_IPFRAGFAILS       = 17;
    public static final int MIP_IPFRAGCREATES     = 18;
    public static final int MIP_IPROUTINGDISCARDS = 19;
    public static final int MIP_FIELD_COUNT       = 20;

    /**
     * Forwarder?
     */
    public int mip_ipForwarding;
    /**
     * Default Time-to-Live
     */
    public int mip_ipDefaultTTL;
    /**
     * Datagrams Received
     */
    public int mip_ipInReceives;
    /**
     * In Discards from Header Errors
     */
    public int mip_ipInHdrErrors;
    /**
     * In Discards from Address Errors
     */
    public int mip_ipInAddrErrors;
    /**
     * Datagrams Forwarded
     */
    public int mip_ipForwDatagrams;
    /**
     * In Discards from Unknown Protocol
     */
    public int mip_ipInUnknownProtos;
    /**
     * In Discards of Good Datagrams
     */
    public int mip_ipInDiscards;
    /**
     * Datagrams Sent Upstream
     */
    public int mip_ipInDelivers;
    /**
     * Upstream Output Requests
     */
    public int mip_ipOutRequests;
    /**
     * Out Discards of Good Datagrams
     */
    public int mip_ipOutDiscards;
    /**
     * Out Discards from No Route
     */
    public int mip_ipOutNoRoutes;
    /**
     * Fragment Reassemble Timeout
     */
    public int mip_ipReasmTimeout;
    /**
     * Fragment Reassemblies Required
     */
    public int mip_ipReasmReqds;
    /**
     * Fragments Reassembled
     */
    public int mip_ipReasmOKs;
    /**
     * Fragment Reassembly Failures
     */
    public int mip_ipReasmFails;
    /**
     * Datagrams Fragmented
     */
    public int mip_ipFragOKs;
    /**
     * Fragment Discards from No-Fragment Flag
     */
    public int mip_ipFragFails;
    /**
     * Fragments Created
     */
    public int mip_ipFragCreates;
    /**
     * Routing Entries Discarded
     */
    public int mip_ipRoutingDiscards;

    /**
     * Call the constructor for the C++ class
     */
    private native boolean ip_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean ip_refresh();
    
    /**
     * Call the rates method in the C++ class
     */
    private native boolean ip_rates();
    
    /**
     * mdr_ip_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_ip_t() throws mdr_exception_t
    {
        if (!ip_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    /**
     * mdr_ip_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_ip_t(String remote_host) throws mdr_exception_t
    {
        if (!ip_constructor(remote_host))
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
        if (!ip_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
        if (!ip_rates())
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
        case MIP_IPFORWARDING:
            return new Integer(mip_ipForwarding);
        case MIP_IPDEFAULTTTL:
            return new Integer(mip_ipDefaultTTL);
        case MIP_IPINRECEIVES:
            return new Integer(mip_ipInReceives);
        case MIP_IPINHDRERRORS:
            return new Integer(mip_ipInHdrErrors);
        case MIP_IPINADDRERRORS:
            return new Integer(mip_ipInAddrErrors);
        case MIP_IPFORWDATAGRAMS:
            return new Integer(mip_ipForwDatagrams);
        case MIP_IPINUNKNOWNPROTOS:
            return new Integer(mip_ipInUnknownProtos);
        case MIP_IPINDISCARDS:
            return new Integer(mip_ipInDiscards);
        case MIP_IPINDELIVERS:
            return new Integer(mip_ipInDelivers);
        case MIP_IPOUTREQUESTS:
            return new Integer(mip_ipOutRequests);
        case MIP_IPOUTDISCARDS:
            return new Integer(mip_ipOutDiscards);
        case MIP_IPOUTNOROUTES:
            return new Integer(mip_ipOutNoRoutes);
        case MIP_IPREASMTIMEOUT:
            return new Integer(mip_ipReasmTimeout);
        case MIP_IPREASMREQDS:
            return new Integer(mip_ipReasmReqds);
        case MIP_IPREASMOKS:
            return new Integer(mip_ipReasmOKs);
        case MIP_IPREASMFAILS:
            return new Integer(mip_ipReasmFails);
        case MIP_IPFRAGOKS:
            return new Integer(mip_ipFragOKs);
        case MIP_IPFRAGFAILS:
            return new Integer(mip_ipFragFails);
        case MIP_IPFRAGCREATES:
            return new Integer(mip_ipFragCreates);
        case MIP_IPROUTINGDISCARDS:
            return new Integer(mip_ipRoutingDiscards);
        default:
            throw new mdr_exception_t("mdr_ip_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }
    
    static
    {
        System.loadLibrary("jmdr");
    }
}
