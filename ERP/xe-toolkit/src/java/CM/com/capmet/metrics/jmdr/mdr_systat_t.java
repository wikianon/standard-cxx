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
 * The mdr_systat_t class
 * <p>
 * Methods in this class are <code>refresh, rates</code> and
 * <code>refresh_rates</code> for updating the variable, computing rates
 * for those values that are rates and both respectively.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_systat_t extends mdr_t
{
    // fields by the number
    public static final int MSYS_USER_TIME_PERCENT           = 0;
    public static final int MSYS_KERNEL_TIME_PERCENT         = 1;
    public static final int MSYS_WAIT_TIME_PERCENT           = 2;
    public static final int MSYS_IDLE_TIME_PERCENT           = 3;
    public static final int MSYS_PAGE_K_IN_PER_SECOND        = 4;
    public static final int MSYS_PAGE_K_OUT_PER_SECOND       = 5;
    public static final int MSYS_INTERRUPTS_PER_SECOND       = 6;
    public static final int MSYS_CONTEXT_SWITCHES_PER_SECOND = 7;
    public static final int MSYS_SYSTEM_CALLS_PER_SECOND     = 8;
    public static final int MSYS_PROCS_RUNNABLE              = 9;
    public static final int MSYS_PROCS_BLOCKED               = 10;
    public static final int MSYS_PROCS_ZOMBIED               = 11;
    public static final int MSYS_FREE_MEMORY_PERCENT         = 12;
    public static final int MSYS_FREE_PAGING_FILE_PERCENT    = 13;
    public static final int MSYS_READ_K_PER_SECOND           = 14;
    public static final int MSYS_WRITTEN_K_PER_SECOND        = 15;
    public static final int MSYS_PAGES_SCANNED               = 16;
    public static final int MSYS_FIELD_COUNT                 = 17;

    /**
     * Percent User Time
     */
    public int    msys_user_time_percent;
    /**
     * Percent Kernel Time
     */
    public int    msys_kernel_time_percent;
    /**
     * Percent Wait Time
     */
    public int    msys_wait_time_percent;
    /**
     * Percent Idle Time
     */
    public int    msys_idle_time_percent;
    /**
     * Kbytes Paged in/sec
     */
    public int    msys_page_K_in_per_second;
    /**
     * Kbytes Paged out/sec
     */
    public int    msys_page_K_out_per_second;
    /**
     * Interrupts/sec
     */
    public int    msys_interrupts_per_second;
    /**
     * Context Switches/sec
     */
    public int    msys_context_switches_per_second;
    /**
     * System Calls/sec
     */
    public int    msys_system_calls_per_second;
    /**
     * Runnable Processes
     */
    public int    msys_procs_runnable;
    /**
     * Blocked Processes
     */
    public int    msys_procs_blocked;
    /**
     * Zombie Processes
     */
    public int    msys_procs_zombied;
    /**
     * Percent Memory Free
     */
    public double msys_free_memory_percent;
    /**
     * Percent Paging File Free
     */
    public double msys_free_paging_file_percent;
    /**
     * KBytes Read/sec
     */
    public int    msys_read_K_per_second;
    /**
     * KBytes Mritten/sec
     */
    public int    msys_written_K_per_second;
    /**
     * Pages Scanned
     */
    public int    msys_pages_scanned;

    /**
     * Call the constructor for the C++ class
     */
    private native boolean systat_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean systat_refresh();
    
    /**
     * mdr_systat_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_systat_t() throws mdr_exception_t
    {
        if (!systat_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    /**
     * mdr_systat_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_systat_t(String remote_host) throws mdr_exception_t
    {
        if (!systat_constructor(remote_host))
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
        if (!systat_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
        // no rates
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
        case MSYS_USER_TIME_PERCENT:
            return new Integer(msys_user_time_percent);
        case MSYS_KERNEL_TIME_PERCENT:
            return new Integer(msys_kernel_time_percent);
        case MSYS_WAIT_TIME_PERCENT:
            return new Integer(msys_wait_time_percent);
        case MSYS_IDLE_TIME_PERCENT:
            return new Integer(msys_idle_time_percent);
        case MSYS_PAGE_K_IN_PER_SECOND:
            return new Integer(msys_page_K_in_per_second);
        case MSYS_PAGE_K_OUT_PER_SECOND:
            return new Integer(msys_page_K_out_per_second);
        case MSYS_INTERRUPTS_PER_SECOND:
            return new Integer(msys_interrupts_per_second);
        case MSYS_CONTEXT_SWITCHES_PER_SECOND:
            return new Integer(msys_context_switches_per_second);
        case MSYS_SYSTEM_CALLS_PER_SECOND:
            return new Integer(msys_system_calls_per_second);
        case MSYS_PROCS_RUNNABLE:
            return new Integer(msys_procs_runnable);
        case MSYS_PROCS_BLOCKED:
            return new Integer(msys_procs_blocked);
        case MSYS_PROCS_ZOMBIED:
            return new Integer(msys_procs_zombied);
        case MSYS_FREE_MEMORY_PERCENT:
            return new Double(msys_free_memory_percent);
        case MSYS_FREE_PAGING_FILE_PERCENT:
            return new Double(msys_free_paging_file_percent);
        case MSYS_READ_K_PER_SECOND:
            return new Integer(msys_read_K_per_second);
        case MSYS_WRITTEN_K_PER_SECOND:
            return new Integer(msys_written_K_per_second);
        case MSYS_PAGES_SCANNED:
            return new Integer(msys_pages_scanned);
        default:
            throw new mdr_exception_t("mdr_systat_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }
    
    static
    {
        System.loadLibrary("jmdr");
    }
}
