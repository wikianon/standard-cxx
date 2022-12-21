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
 * The mdr_cpustat_t class
 * <p>
 * Methods in this class are <code>refresh, rates</code> and
 * <code>refresh_rates</code> for updating the variable, computing rates
 * for those values that are rates and both respectively.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_cpustat_t extends mdr_t
{
    // fields by the number
    public static final int MCS_USER_TIME_PERCENT   = 0;
    public static final int MCS_KERNEL_TIME_PERCENT = 1;
    public static final int MCS_WAIT_TIME_PERCENT   = 2;
    public static final int MCS_IDLE_TIME_PERCENT   = 3;

    /**
     * Percent User Time
     */
    public int mcs_user_time_percent;
    /**
     * Percent Kernel Time
     */
    public int mcs_kernel_time_percent;
    /**
     * Percent Wait Time
     */
    public int mcs_wait_time_percent;
    /**
     * Percent Idle Time
     */
    public int mcs_idle_time_percent;

    /**
     * Call the constructor for the C++ class
     */
    private native boolean cpustat_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean cpustat_refresh();
    
    /**
     * Call the rates method in the C++ class
     */
    private native boolean cpustat_rates();
    
    /**
     * mdr_cpustat_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_cpustat_t() throws mdr_exception_t
    {
        if (!cpustat_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    /**
     * mdr_cpustat_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_cpustat_t(String remote_host) throws mdr_exception_t
    {
        if (!cpustat_constructor(remote_host))
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
        if (!cpustat_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
        if (!cpustat_rates())
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
        case MCS_USER_TIME_PERCENT:
            return new Integer(mcs_user_time_percent);
        case MCS_KERNEL_TIME_PERCENT:
            return new Integer(mcs_kernel_time_percent);
        case MCS_WAIT_TIME_PERCENT:
            return new Integer(mcs_wait_time_percent);
        case MCS_IDLE_TIME_PERCENT:
            return new Integer(mcs_idle_time_percent);
        default:
            throw new mdr_exception_t("mdr_cpustat_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }
    
    static
    {
        System.loadLibrary("jmdr");
    }
}
