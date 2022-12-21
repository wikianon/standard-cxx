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
 * The mdr_icmp_t class provides data from 
 * <p>
 * Methods in this class are <code>refresh, rates</code> and
 * <code>refresh_rates</code> for updating the variable, computing rates
 * for those values that are rates and both respectively.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_icmp_t extends mdr_t
{
    // fields by the number
    public static final int MICMP_ICMPINMSGS           = 0;
    public static final int MICMP_ICMPINERRORS         = 1;
    public static final int MICMP_ICMPINDESTUNREACHS   = 2;
    public static final int MICMP_ICMPINTIMEEXCDS      = 3;
    public static final int MICMP_ICMPINPARMPROBS      = 4;
    public static final int MICMP_ICMPINSRCQUENCHS     = 5;
    public static final int MICMP_ICMPINREDIRECTS      = 6;
    public static final int MICMP_ICMPINECHOS          = 7;
    public static final int MICMP_ICMPINECHOREPS       = 8;
    public static final int MICMP_ICMPINTIMESTAMPS     = 9;
    public static final int MICMP_ICMPINTIMESTAMPREPS  = 10;
    public static final int MICMP_ICMPINADDRMASKS      = 11;
    public static final int MICMP_ICMPINADDRMASKREPS   = 12;
    public static final int MICMP_ICMPOUTMSGS          = 13;
    public static final int MICMP_ICMPOUTERRORS        = 14;
    public static final int MICMP_ICMPOUTDESTUNREACHS  = 15;
    public static final int MICMP_ICMPOUTTIMEEXCDS     = 16;
    public static final int MICMP_ICMPOUTPARMPROBS     = 17;
    public static final int MICMP_ICMPOUTSRCQUENCHS    = 18;
    public static final int MICMP_ICMPOUTREDIRECTS     = 19;
    public static final int MICMP_ICMPOUTECHOS         = 20;
    public static final int MICMP_ICMPOUTECHOREPS      = 21;
    public static final int MICMP_ICMPOUTTIMESTAMPS    = 22;
    public static final int MICMP_ICMPOUTTIMESTAMPREPS = 23;
    public static final int MICMP_ICMPOUTADDRMASKS     = 24;
    public static final int MICMP_ICMPOUTADDRMASKREPS  = 25;
    public static final int MICMP_FIELD_COUNT          = 26;

    /**
     * Messages Received
     */
    public int micmp_icmpInMsgs;
    /**
     * Messages Received with Error
     */
    public int micmp_icmpInErrors;
    /**
     * Dest. Unreach. Msgs. Received
     */
    public int micmp_icmpInDestUnreachs;
    /**
     * Time Exceeded Msgs. Received
     */
    public int micmp_icmpInTimeExcds;
    /**
     * Param. Prob. Msgs. Received
     */
    public int micmp_icmpInParmProbs;
    /**
     * Source Quench Msgs. Received
     */
    public int micmp_icmpInSrcQuenchs;
    /**
     * ICMP Redirect Msgs. Received
     */
    public int micmp_icmpInRedirects;
    /**
     * Echo Request Msgs. Received
     */
    public int micmp_icmpInEchos;
    /**
     * Echo Reply Msgs. Received
     */
    public int micmp_icmpInEchoReps;
    /**
     * Timestamp Msgs. Received
     */
    public int micmp_icmpInTimestamps;
    /**
     * Timestamp Reply Msgs. Received
     */
    public int micmp_icmpInTimestampReps;
    /**
     * Addr. Mask Req. Msgs. Received
     */
    public int micmp_icmpInAddrMasks;
    /**
     * Addr. Mask Reply Msgs. Received
     */
    public int micmp_icmpInAddrMaskReps;
    /**
     * Messages Transmitted
     */
    public int micmp_icmpOutMsgs;
    /**
     * Messages Transmitted for Errors
     */
    public int micmp_icmpOutErrors;
    /**
     * Dest. Unreach. Msgs. Transmitted
     */
    public int micmp_icmpOutDestUnreachs;
    /**
     * Time Exceeded Msgs. Transmitted
     */
    public int micmp_icmpOutTimeExcds;
    /**
     * Param. Prob. Msgs. Transmitted
     */
    public int micmp_icmpOutParmProbs;
    /**
     * Source Quench Msgs. Transmitted
     */
    public int micmp_icmpOutSrcQuenchs;
    /**
     * ICMP Redirect Msgs. Transmitted
     */
    public int micmp_icmpOutRedirects;
    /**
     * Echo Request Msgs. Transmitted
     */
    public int micmp_icmpOutEchos;
    /**
     * Echo Reply Msgs. Transmitted
     */
    public int micmp_icmpOutEchoReps;
    /**
     * Timestamp Msgs. Transmitted
     */
    public int micmp_icmpOutTimestamps;
    /**
     * Timestamp Reply Msgs. Transmitted
     */
    public int micmp_icmpOutTimestampReps;
    /**
     * Addr. Mask Req. Msgs. Transmitted
     */
    public int micmp_icmpOutAddrMasks;
    /**
     * Addr. Mask Reply Msgs. Transmitted
     */
    public int micmp_icmpOutAddrMaskReps;

    /**
     * Call the constructor for the C++ class
     */
    private native boolean icmp_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean icmp_refresh();
    
    /**
     * Call the rates method in the C++ class
     */
    private native boolean icmp_rates();
    
    /**
     * mdr_icmp_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_icmp_t() throws mdr_exception_t
    {
        if (!icmp_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    /**
     * mdr_icmp_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_icmp_t(String remote_host) throws mdr_exception_t
    {
        if (!icmp_constructor(remote_host))
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
        if (!icmp_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
        if (!icmp_rates())
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
        case MICMP_ICMPINMSGS:
            return new Integer(micmp_icmpInMsgs);
        case MICMP_ICMPINERRORS:
            return new Integer(micmp_icmpInErrors);
        case MICMP_ICMPINDESTUNREACHS:
            return new Integer(micmp_icmpInDestUnreachs);
        case MICMP_ICMPINTIMEEXCDS:
            return new Integer(micmp_icmpInTimeExcds);
        case MICMP_ICMPINPARMPROBS:
            return new Integer(micmp_icmpInParmProbs);
        case MICMP_ICMPINSRCQUENCHS:
            return new Integer(micmp_icmpInSrcQuenchs);
        case MICMP_ICMPINREDIRECTS:
            return new Integer(micmp_icmpInRedirects);
        case MICMP_ICMPINECHOS:
            return new Integer(micmp_icmpInEchos);
        case MICMP_ICMPINECHOREPS:
            return new Integer(micmp_icmpInEchoReps);
        case MICMP_ICMPINTIMESTAMPS:
            return new Integer(micmp_icmpInTimestamps);
        case MICMP_ICMPINTIMESTAMPREPS:
            return new Integer(micmp_icmpInTimestampReps);
        case MICMP_ICMPINADDRMASKS:
            return new Integer(micmp_icmpInAddrMasks);
        case MICMP_ICMPINADDRMASKREPS:
            return new Integer(micmp_icmpInAddrMaskReps);
        case MICMP_ICMPOUTMSGS:
            return new Integer(micmp_icmpOutMsgs);
        case MICMP_ICMPOUTERRORS:
            return new Integer(micmp_icmpOutErrors);
        case MICMP_ICMPOUTDESTUNREACHS:
            return new Integer(micmp_icmpOutDestUnreachs);
        case MICMP_ICMPOUTTIMEEXCDS:
            return new Integer(micmp_icmpOutTimeExcds);
        case MICMP_ICMPOUTPARMPROBS:
            return new Integer(micmp_icmpOutParmProbs);
        case MICMP_ICMPOUTSRCQUENCHS:
            return new Integer(micmp_icmpOutSrcQuenchs);
        case MICMP_ICMPOUTREDIRECTS:
            return new Integer(micmp_icmpOutRedirects);
        case MICMP_ICMPOUTECHOS:
            return new Integer(micmp_icmpOutEchos);
        case MICMP_ICMPOUTECHOREPS:
            return new Integer(micmp_icmpOutEchoReps);
        case MICMP_ICMPOUTTIMESTAMPS:
            return new Integer(micmp_icmpOutTimestamps);
        case MICMP_ICMPOUTTIMESTAMPREPS:
            return new Integer(micmp_icmpOutTimestampReps);
        case MICMP_ICMPOUTADDRMASKS:
            return new Integer(micmp_icmpOutAddrMasks);
        case MICMP_ICMPOUTADDRMASKREPS:
            return new Integer(micmp_icmpOutAddrMaskReps);
        default:
            throw new mdr_exception_t("mdr_icmp_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }
    
    static
    {
        System.loadLibrary("jmdr");
    }
}
