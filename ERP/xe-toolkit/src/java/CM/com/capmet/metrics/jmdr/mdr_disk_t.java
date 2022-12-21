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
import com.capmet.metrics.jmdr.mdr_tuple_t;
import com.capmet.metrics.jmdr.mdr_string_t;
import com.capmet.metrics.jmdr.mdr_exception_t;
import com.capmet.metrics.jmdr.md_can_t;


/**
 * The mdr_disk_t class provides data about the disks in the operating
 * system that report data about their effectiveness.
 * <p>
 * Methods in this class are <code>refresh, rates and refresh_rates</code>
 * for updating the variable, computing rates for those values that are
 * rates and both respectively.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_disk_t extends mdr_t implements mdr_tuple_t
{
    // fields by the number
    public static final int MD_TYPE             = 0;
    public static final int MD_READS            = 1;
    public static final int MD_READ_K           = 2;
    public static final int MD_READ_MSECS       = 3;
    public static final int MD_WRITES           = 4;
    public static final int MD_WRITTEN_K        = 5;
    public static final int MD_WRITE_MSECS      = 6;
    public static final int MD_RUN_MSECS        = 7;
    public static final int MD_WAIT_MSECS       = 8;
    public static final int MD_RUNQ_MSECS       = 9;
    public static final int MD_WAITQ_MSECS      = 10;
    public static final int MD_AVG_QUEUE_LENGTH = 11;
    public static final int MD_UTIL_PERCENT     = 12;
    public static final int MD_SERVICE_TIME     = 13;
    public static final int MD_SPACE_MB         = 14;
    public static final int MD_FIELD_COUNT      = 15;

    /**
     * Call the constructor for the C++ class
     */
    private native boolean disk_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean disk_refresh();
    
    /**
     * Call the rates method in the C++ class
     */
    private native boolean disk_rates();
    
    /**
     * mdr_disk_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_disk_t() throws mdr_exception_t
    {
        if (!disk_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    /**
     * mdr_disk_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_disk_t(String remote_host) throws mdr_exception_t
    {
        if (!disk_constructor(remote_host))
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
        if (!disk_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
        if (!disk_rates())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Get the number of entries in this tuple.
     * @return The number of entries in this tuple.
     */
    public native int mdr_size();

    /**
     * Get all the instance names in this tuple.
     * @return An array of <CODE>String</CODE> containing the instance names.
     */
    public native String[] mdr_keyset();
    
    /**
     * Convenience method used with mdr_keyset() to retrieve instances.
     * @param key The instance name.
     * @param can Can to place the results in.
     * @return True - get succeeded; False - get failed, see reason member.
     */
    public native boolean mdr_get(String key, md_can_t can);

    // for exclusive use by mdr_field_value
    private native boolean mdr_get_field
        (String key, md_can_t can, int field_number);

    /**
     * Get the current value of the performance field.
     * @param key The instance name to retrieve the field from.
     * @param field_number The numeric field ID of the field to retrieve.
     * @throws mdr_exception_t Thrown from JNI code
     * @return <CODE>Object</CODE> value of the performance field.
     */
    public Object mdr_field_value(String key, int field_number)
        throws mdr_exception_t
    {
        md_can_t can = new md_can_t();

        if (!mdr_get_field(key, can, field_number))
            throw new mdr_exception_t("mdr_disk_t::mdr_field_value: " +
                msgs.getString("instance") + ": " + key + " " +
                msgs.getString("not_found"));

        switch(field_number)
        {
        case MD_TYPE:
            return new String(can.md_type);
        case MD_READS:
            return new Integer(can.md_reads);
        case MD_READ_K:
            return new Integer(can.md_read_K);
        case MD_READ_MSECS:
            return new Integer(can.md_read_msecs);
        case MD_WRITES:
            return new Integer(can.md_writes);
        case MD_WRITTEN_K:
            return new Integer(can.md_written_K);
        case MD_WRITE_MSECS:
            return new Integer(can.md_write_msecs);
        case MD_RUN_MSECS:
            return new Integer(can.md_run_msecs);
        case MD_WAIT_MSECS:
            return new Integer(can.md_wait_msecs);
        case MD_RUNQ_MSECS:
            return new Integer(can.md_runq_msecs);
        case MD_WAITQ_MSECS:
            return new Integer(can.md_waitq_msecs);
        case MD_AVG_QUEUE_LENGTH:
            return new Double(can.md_avg_queue_length);
        case MD_UTIL_PERCENT:
            return new Double(can.md_util_percent);
        case MD_SERVICE_TIME:
            return new Double(can.md_service_time);
        case MD_SPACE_MB:
            return new Integer(can.md_space_mb);
        default:
            throw new mdr_exception_t("mdr_disk_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }
    
    static
    {
        System.loadLibrary("jmdr");
    }
}
