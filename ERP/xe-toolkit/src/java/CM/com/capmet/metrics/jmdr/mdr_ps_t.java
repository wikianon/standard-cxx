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
import com.capmet.metrics.jmdr.mps_can_t;


/**
 * The mdr_ps_t class provides data about the performance of the CPUs
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
 * @version 1.2
 */
public class mdr_ps_t extends mdr_t implements mdr_tuple_t
{
    // fields by the number

    // per-process field ids
    public static final int MPS_START               = 0;
    public static final int MPS_UID                 = 1;
    public static final int MPS_EUID                = 2;
    public static final int MPS_PID                 = 3;
    public static final int MPS_PPID                = 4;
    public static final int MPS_TTY_DEVICE          = 5;
    public static final int MPS_FILE_NAME           = 6;
    public static final int MPS_COMMAND             = 7;
    public static final int MPS_FIRST_ARGUMENT      = 8;
    public static final int MPS_CPU_TIME            = 9;
    public static final int MPS_USR_TIME            = 10;
    public static final int MPS_KERNEL_TIME         = 11;
    public static final int MPS_USR_TIME_PERCENT    = 12;
    public static final int MPS_KERNEL_TIME_PERCENT = 13;
    public static final int MPS_SIZE_K              = 14;
    public static final int MPS_RSSIZE_K            = 15;
    public static final int MPS_IO_K                = 16;
    public static final int MPS_CPU_PERCENT         = 17;
    public static final int MPS_MEMORY_PERCENT      = 18;
    public static final int MPS_VOLUNTARY_CSW       = 19;
    public static final int MPS_INVOLUNTARY_CSW     = 20;
    public static final int MPS_DISK_FAULTS         = 21;
    public static final int MPS_PAGE_RECLAIMS       = 22;
    public static final int MPS_STATE               = 23;
    public static final int MPS_PRIORITY            = 24;
    public static final int MPS_NICE                = 25;
    public static final int MPS_THREAD_COUNT        = 26;
    public static final int MPS_SIZE_DELTA_K        = 27;
    public static final int MPS_RSSIZE_DELTA_K      = 28;
    public static final int MPS_USER_NAME           = 39;
    public static final int MPS_EFFECTIVE_USER_NAME = 30;
    public static final int MPS_SYSTEM_CALLS        = 31;
    public static final int MPS_IS_SYSTEM_PROCESS   = 32;
    public static final int MPS_FIELD_COUNT         = 33;

    // the mdr_ps_t class field ids
    public static final int MPS_PROCS_RUNNABLE      = 34;
    public static final int MPS_PROCS_SLEEPING      = 35;
    public static final int MPS_PROCS_STOPPED       = 36;
    public static final int MPS_PROCS_BLOCKED       = 37;
    public static final int MPS_PROCS_ZOMBIE        = 38;
    public static final int MPS_PROCS_SPECIFIC      = 39;

    // proc states
    public static final int MPS_RUNNABLE    = 0;
    public static final int MPS_SLEEPING    = 1;
    public static final int MPS_STOPPED     = 2;
    public static final int MPS_BLOCKED     = 3;
    public static final int MPS_ZOMBIE      = 4;
    public static final int MPS_SPECIFIC    = 5;
    public static final int MPS_PROC_STATES = 6;

    /**
     * Define the sort keys for the mdr_ps_t type.
     */
    public enum mps_sort_key_t
    {
        // enum values called out for consistency between Java and C++
        //
        BY_PID               (0),
        BY_CPU_TIME          (1),
        BY_USER_TIME         (2),
        BY_KERNEL_TIME       (3),
        BY_CPU_USE           (4),
        BY_MEM_USE           (5),
        BY_SIZE              (6),
        BY_RSSIZE            (7),
        BY_IO                (8),
        BY_UID               (9),
        BY_VOLUNTARY_CSW     (10),
        BY_INVOLUNTARY_CSW   (11),
        BY_DISK_FAULTS       (12),
        BY_PAGE_RECLAIMS     (13),
        BY_THREAD_COUNT      (14),
        BY_SIZE_DELTA        (15),
        BY_RSSIZE_DELTA      (16);

        private final int value;

        private mps_sort_key_t(int value)
        {
            this.value = value;
        }
    }

    /**
     * The number of proceses running or runnable.
     */
    public int mps_procs_runnable;
    /**
     * The number of proceses sleeping.
     */
    public int mps_procs_sleeping;
    /**
     * The number of proceses stopped.
     */
    public int mps_procs_stopped;
    /**
     * The number of proceses blocked.
     */
    public int mps_procs_blocked;
    /**
     * The number of proceses that are unreaped.
     */
    public int mps_procs_zombie;
    /**
     * The number of proceses in an OS specific state.
     */
    public int mps_procs_specific;

    /**
     * Call the constructor for the C++ class
     */
    private native boolean ps_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean ps_refresh();
    
    /**
     * Call the rates method in the C++ class
     */
    private native boolean ps_rates();

    /**
     * Call the mps_sort method in the C++ class
     */
    private native boolean ps_sort(mps_sort_key_t key, boolean reverse);
    
    /**
     * Call the mps_dev_to_tty method in the C++ class
     */
    private native String ps_dev_to_tty(long dev);
    
    /**
     * mdr_ps_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_ps_t() throws mdr_exception_t
    {
        if (!ps_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    /**
     * mdr_ps_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_ps_t(String remote_host) throws mdr_exception_t
    {
        if (!ps_constructor(remote_host))
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
        if (!ps_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
        if (!ps_rates())
            throw new mdr_exception_t(reason);
    }

    /**
     * Sort the processes by the indicated key.
     * @param key The sort key.
     * @param reverse Sort by a descending key.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mps_sort(mps_sort_key_t key, boolean reverse)
        throws mdr_exception_t
    {
        if (!ps_sort(key, reverse))
            throw new mdr_exception_t(reason);
    }

    /**
     * Reset the internal pointer to the beginning.
     */
    public native void mps_rewind();

    /**
     * Map a device identifier to a device name if possible.
     * @param dev The device identifier.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public String mps_dev_to_tty(long dev)
        throws mdr_exception_t
    {
        String tty = ps_dev_to_tty(dev);

        if (tty == null)
            throw new mdr_exception_t(reason);

        return tty;
    }

    private native boolean ps_next(mdr_string_t name, mps_can_t can);
    
    /**
     * Convenience function for getting the next process.
     * @param name The returned key name from the underlying C++ structure.
     * @param can The data related to the key.
     * @return True - Next process, continue; False - EOF, stop looping.
     */
    public boolean mps_next_ps(StringBuffer name, mps_can_t can)
    {
        boolean retval;
        mdr_string_t s = new mdr_string_t();
        
        s.s = name.toString();
        retval = ps_next(s, can);
        name.setLength(0);
        name.insert(0, s.s);
        return retval;
    }

    /**
     * Get the next process in the sorted list.
     * @param can The process data.
     * @return True - Next process, continue; False - EOF, stop looping.
     */
    public native boolean mps_next_sorted_ps(mps_can_t can);
    
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
    public native boolean mdr_get(String key, mps_can_t can);

    // for exclusive use by mdr_field_value
    private native boolean mdr_get_field
        (String key, mps_can_t can, int field_number);

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
        case MPS_PROCS_RUNNABLE:
            return new Integer(mps_procs_runnable);
        case MPS_PROCS_SLEEPING:
            return new Integer(mps_procs_sleeping);
        case MPS_PROCS_STOPPED:
            return new Integer(mps_procs_stopped);
        case MPS_PROCS_BLOCKED:
            return new Integer(mps_procs_blocked);
        case MPS_PROCS_ZOMBIE:
            return new Integer(mps_procs_zombie);
        case MPS_PROCS_SPECIFIC:
            return new Integer(mps_procs_specific);
        default:
            throw new mdr_exception_t("mdr_ps_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }

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
        mps_can_t can = new mps_can_t();

        if (!mdr_get_field(key, can, field_number))
            throw new mdr_exception_t("mdr_ps_t::mdr_field_value: " +
                msgs.getString("instance") + ": " + key + " " +
                msgs.getString("not_found"));

        switch(field_number)
        {
        case MPS_START:
            return new Long(can.mps_start);
        case MPS_UID:
            return new Integer(can.mps_uid);
        case MPS_EUID:
            return new Integer(can.mps_euid);
        case MPS_PID:
            return new Integer(can.mps_pid);
        case MPS_PPID:
            return new Integer(can.mps_ppid);
        case MPS_TTY_DEVICE:
            return new Long(can.mps_tty_device);
        case MPS_FILE_NAME:
            return new String(can.mps_file_name);
        case MPS_COMMAND:
            return new String(can.mps_command);
        case MPS_FIRST_ARGUMENT:
            return new String(can.mps_first_argument);
        case MPS_CPU_TIME:
            return new Double(can.mps_cpu_time);
        case MPS_USR_TIME:
            return new Double(can.mps_user_time);
        case MPS_KERNEL_TIME:
            return new Double(can.mps_kernel_time);
        case MPS_USR_TIME_PERCENT:
            return new Double(can.mps_user_time_percent);
        case MPS_KERNEL_TIME_PERCENT:
            return new Double(can.mps_kernel_time_percent);
        case MPS_SIZE_K:
            return new Long(can.mps_size_K);
        case MPS_RSSIZE_K:
            return new Long(can.mps_rssize_K);
        case MPS_IO_K:
            return new Long(can.mps_io_K);
        case MPS_CPU_PERCENT:
            return new Double(can.mps_cpu_percent);
        case MPS_MEMORY_PERCENT:
            return new Double(can.mps_memory_percent);
        case MPS_VOLUNTARY_CSW:
            return new Integer(can.mps_voluntary_csw);
        case MPS_INVOLUNTARY_CSW:
            return new Integer(can.mps_involuntary_csw);
        case MPS_DISK_FAULTS:
            return new Integer(can.mps_disk_faults);
        case MPS_PAGE_RECLAIMS:
            return new Integer(can.mps_page_reclaims);
        case MPS_STATE:
            return new Integer(can.mps_state);
        case MPS_PRIORITY:
            return new Integer(can.mps_priority);
        case MPS_NICE:
            return new Integer(can.mps_nice);
        case MPS_THREAD_COUNT:
            return new Integer(can.mps_thread_count);
        case MPS_SIZE_DELTA_K:
            return new Long(can.mps_size_delta_K);
        case MPS_RSSIZE_DELTA_K:
            return new Long(can.mps_rssize_delta_K);
        case MPS_USER_NAME:
            return new String(can.mps_user_name);
        case MPS_EFFECTIVE_USER_NAME:
            return new String(can.mps_effective_user_name);
        default:
            throw new mdr_exception_t("mdr_ps_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }
    
    static
    {
        System.loadLibrary("jmdr");
    }
}
