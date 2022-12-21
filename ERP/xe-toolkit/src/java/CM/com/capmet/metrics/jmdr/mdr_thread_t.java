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
import com.capmet.metrics.jmdr.mt_can_t;


/**
 * The mdr_thread_t class provides data about the performance of the CPUs
 * in the machine. This data is broken down into user time, kernel time,
 * idle time and wait time. The hardware instance number of the CPU is
 * also provided.
 * <p>
 * Methods in this class are <code>refresh, rates</code> and
 * <code>refresh_rates</code> for updating the variable, computing rates
 * for those values that are rates and both respectively.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_thread_t extends mdr_t implements mdr_tuple_t
{
    // fields by the number
    public static final int MT_THREAD_ID           = 0;
    public static final int MT_CPU_TIME            = 1;
    public static final int MT_USR_TIME            = 2;
    public static final int MT_KERNEL_TIME         = 3;
    public static final int MT_USR_TIME_PERCENT    = 4;
    public static final int MT_KERNEL_TIME_PERCENT = 5;
    public static final int MT_SIZE_K              = 6;
    public static final int MT_RSSIZE_K            = 7;
    public static final int MT_IO_K                = 8;
    public static final int MT_CPU_PERCENT         = 9;
    public static final int MT_MEMORY_PERCENT      = 10;
    public static final int MT_VOLUNTARY_CSW       = 11;
    public static final int MT_INVOLUNTARY_CSW     = 12;
    public static final int MT_DISK_FAULTS         = 13;
    public static final int MT_PAGE_RECLAIMS       = 14;
    public static final int MT_STATE               = 15;
    public static final int MT_PRIORITY            = 16;
    public static final int MT_NICE                = 17;
    public static final int MT_SIZE_DELTA_K        = 18;
    public static final int MT_RSSIZE_DELTA_K      = 19;

    /**
     * The sort keys for mdr_thread_t
     */
    public enum mt_sort_key_t
    {
        // enum values called out for consistency between Java and C++
        //
        BY_THREAD_ID         (0),
        BY_CPU_TIME          (1),
        BY_USER_TIME         (2),
        BY_KERNEL_TIME       (3),
        BY_CPU_USE           (4),
        BY_MEM_USE           (5),
        BY_SIZE              (6),
        BY_RSSIZE            (7),
        BY_IO                (8),
        BY_VOLUNTARY_CSW     (9),
        BY_INVOLUNTARY_CSW   (10),
        BY_DISK_FAULTS       (11),
        BY_PAGE_RECLAIMS     (12),
        BY_SIZE_DELTA        (13),
        BY_RSSIZE_DELTA      (14);

        private final int value;

        private mt_sort_key_t(int value)
        {
            this.value = value;
        }
    }

    /**
     * Call the constructor for the C++ class
     */
    private native boolean thread_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean thread_refresh();
    
    /**
     * Call the rates method in the C++ class
     */
    private native boolean thread_rates();

    /**
     * Call the mt_sort method in the C++ class
     */
    private native boolean thread_sort(mt_sort_key_t key, boolean reverse);
    
    private native boolean thread_set_process_id(int pid);
    
    /**
     * mdr_thread_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_thread_t() throws mdr_exception_t
    {
        if (!thread_constructor(null))
            throw new mdr_exception_t(reason);
    }

    /**
     * mdr_thread_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_thread_t(String remote_host) throws mdr_exception_t
    {
        if (!thread_constructor(remote_host))
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
        if (!thread_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
        if (!thread_rates())
            throw new mdr_exception_t(reason);
    }

    /**
     * Sort the threads by the indicated key.
     * @param key The sort key.
     * @param reverse Whether the threads should be sorted descending.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mt_sort(mt_sort_key_t key, boolean reverse)
        throws mdr_exception_t
    {
        if (!thread_sort(key, reverse))
            throw new mdr_exception_t(reason);
    }

    /**
     * Rewind the internal pointer to the beginning.
     */
    public native void mt_rewind();

    /**
     * Set the process id of interest.
     * @param pid The process id.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mt_set_process_id(int pid)
        throws mdr_exception_t
    {
        if (!thread_set_process_id(pid))
            throw new mdr_exception_t(reason);
    }

    private native boolean thread_next(mdr_string_t name, mt_can_t can);
    
    /**
     * Convenience method for getting the next thread.
     * @param name The returned key from the underlying C++ structure.
     * @param can The data related to the key.
     * @return True - Next thread, continue; False - EOF, stop looping
     */
    public boolean mt_next_thread(StringBuffer name, mt_can_t can)
    {
        boolean retval;
        mdr_string_t s = new mdr_string_t();
        
        s.s = name.toString();
        retval = thread_next(s, can);
        name.setLength(0);
        name.insert(0, s.s);
        return retval;
    }

    /**
     * Get the next thread from the sorted list.
     * @param can The data related to the key.
     * @return True - Next thread, continue; False - EOF, stop looping
     */
    public native boolean mt_next_sorted_thread(mt_can_t can);
    
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
    public native boolean mdr_get(String key, mt_can_t can);

    // for exclusive use by mdr_field_value
    private native boolean mdr_get_field
        (String key, mt_can_t can, int field_number);

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
        mt_can_t can = new mt_can_t();

        if (!mdr_get_field(key, can, field_number))
            throw new mdr_exception_t("mdr_thread_t::mdr_field_value: " +
                msgs.getString("instance") + ": " + key + " " +
                msgs.getString("not_found"));

        switch(field_number)
        {
        case MT_THREAD_ID:
            return new Integer(can.mt_thread_id);
        case MT_CPU_TIME:
            return new Double(can.mt_cpu_time);
        case MT_USR_TIME:
            return new Double(can.mt_user_time);
        case MT_KERNEL_TIME:
            return new Double(can.mt_kernel_time);
        case MT_USR_TIME_PERCENT:
            return new Double(can.mt_user_time_percent);
        case MT_KERNEL_TIME_PERCENT:
            return new Double(can.mt_kernel_time_percent);
        case MT_SIZE_K:
            return new Long(can.mt_size_K);
        case MT_RSSIZE_K:
            return new Long(can.mt_rssize_K);
        case MT_IO_K:
            return new Long(can.mt_io_K);
        case MT_CPU_PERCENT:
            return new Double(can.mt_cpu_percent);
        case MT_MEMORY_PERCENT:
            return new Double(can.mt_memory_percent);
        case MT_VOLUNTARY_CSW:
            return new Integer(can.mt_voluntary_csw);
        case MT_INVOLUNTARY_CSW:
            return new Integer(can.mt_involuntary_csw);
        case MT_DISK_FAULTS:
            return new Integer(can.mt_disk_faults);
        case MT_PAGE_RECLAIMS:
            return new Integer(can.mt_page_reclaims);
        case MT_STATE:
            return new Integer(can.mt_state);
        case MT_PRIORITY:
            return new Integer(can.mt_priority);
        case MT_NICE:
            return new Integer(can.mt_nice);
        case MT_SIZE_DELTA_K:
            return new Long(can.mt_size_delta_K);
        case MT_RSSIZE_DELTA_K:
            return new Long(can.mt_rssize_delta_K);
        default:
            throw new mdr_exception_t("mdr_thread_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }
    
    static
    {
        System.loadLibrary("jmdr");
    }
}
