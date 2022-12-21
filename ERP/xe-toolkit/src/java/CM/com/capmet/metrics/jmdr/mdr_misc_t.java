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
 * The mdr_misc_t class provides data from the underlying <code>mdr_misc_t</code>
 * C++ class which contains miscellaneous data about the operating system.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_misc_t extends mdr_t
{
    // fields by the number
    public static final int MM_HOSTNAME              = 0;
    public static final int MM_OS_PLATFORM           = 1;
    public static final int MM_OS_VERSION            = 2;
    public static final int MM_PHYSICAL_PAGES        = 3;
    public static final int MM_NATIVE_PAGE_SIZE      = 4;
    public static final int MM_PROCESSOR_ARCH        = 5;
    public static final int MM_PROCESSORS_CONFIGURED = 6;
    public static final int MM_PROCESSORS_ONLINE     = 7;
    public static final int MM_TICKS_PER_SECOND      = 8;
    public static final int MM_BOOT_TIME             = 9;
    public static final int MM_INTERRUPTS            = 10;
    public static final int MM_CLOCK_INTERRUPTS      = 11;
    public static final int MM_SYSTEM_CALLS          = 12;
    public static final int MM_CONTEXT_SWITCHES      = 13;
    public static final int MM_PROCESS_COUNT         = 14;
    public static final int MM_1MIN_RUN_QUEUE        = 15;
    public static final int MM_5MIN_RUN_QUEUE        = 16;
    public static final int MM_15MIN_RUN_QUEUE       = 17;
    public static final int MM_CURRENT_TIME          = 18;
    public static final int MM_FIELD_COUNT           = 19;

    /**
     * Name of the host
     */
    public String mm_hostname;
    /**
     * OS name
     */
    public String mm_os_platform;
    /**
     * OS version
     */
    public String mm_os_version;
    /**
     * pages of physical memory
     */
    public int    mm_physical_pages;
    /**
     * Native physical page size
     */
    public int    mm_native_page_size;
    /**
     * CPU Architecture
     */
    public String mm_processor_arch;
    /**
     * Number of processors configured
     */
    public int    mm_processors_configured;
    /**
     * Number of processors on-line
     */
    public int    mm_processors_online;
    /**
     * Clock ticks per second
     */
    public int    mm_ticks_per_second;
    /**
     * Boot time
     */
    public long   mm_boot_time;
    
    /**
     * Total interrupts since boot
     */
    public long   mm_interrupts;
    /**
     * Clock interrupts since boot
     */
    public long   mm_clock_interrupts;
    /**
     * System calls
     */
    public int    mm_system_calls;
    /**
     * Context switches
     */
    public int    mm_context_switches;
    
    /**
     * Number of processes
     */
    public int    mm_process_count;
    /**
     * Avg run queue length, last 1 min
     */
    public double mm_1min_run_queue;
    /**
     * Avg run queue length, last 5 min
     */
    public double mm_5min_run_queue;
    /**
     * Avg run queue length, last 15 min
     */
    public double mm_15min_run_queue;
    /**
     * Current Time
     */
    public long   mm_current_time;
    
    /**
     * Call the constructor for the C++ class
     */
    private native boolean misc_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean misc_refresh();
    
    /**
     * Call the rates method in the C++ class
     */
    private native boolean misc_rates();
    
    /**
     * mdr_misc_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_misc_t() throws mdr_exception_t
    {
        if (!misc_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    /**
     * mdr_misc_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_misc_t(String remote_host) throws mdr_exception_t
    {
        if (!misc_constructor(remote_host))
            throw new mdr_exception_t(reason);
    }

    /**
     * Wrap things up in the JNI code.
     */
    protected native void finalize();
    
    /**
     * Update the members with the latest values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_refresh() throws mdr_exception_t
    {
        if (!misc_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
        if (!misc_rates())
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
        case MM_HOSTNAME:
            return mm_hostname;
        case MM_OS_PLATFORM:
            return mm_os_platform;
        case MM_OS_VERSION:
            return mm_os_version;
        case MM_PHYSICAL_PAGES:
            return new Integer(mm_physical_pages);
        case MM_NATIVE_PAGE_SIZE:
            return new Integer(mm_native_page_size);
        case MM_PROCESSOR_ARCH:
            return mm_processor_arch;
        case MM_PROCESSORS_CONFIGURED:
            return new Integer(mm_processors_configured);
        case MM_PROCESSORS_ONLINE:
            return new Integer(mm_processors_online);
        case MM_TICKS_PER_SECOND:
            return new Integer(mm_ticks_per_second);
        case MM_BOOT_TIME:
            return new Long(mm_boot_time);
        case MM_INTERRUPTS:
            return new Long(mm_interrupts);
        case MM_CLOCK_INTERRUPTS:
            return new Long(mm_clock_interrupts);
        case MM_SYSTEM_CALLS:
            return new Integer(mm_system_calls);
        case MM_CONTEXT_SWITCHES:
            return new Integer(mm_context_switches);
        case MM_PROCESS_COUNT:
            return new Integer(mm_process_count);
        case MM_1MIN_RUN_QUEUE:
            return new Double(mm_1min_run_queue);
        case MM_5MIN_RUN_QUEUE:
            return new Double(mm_5min_run_queue);
        case MM_15MIN_RUN_QUEUE:
            return new Double(mm_15min_run_queue);
        case MM_CURRENT_TIME:
            return new Long(mm_current_time);
        default:
            throw new mdr_exception_t("mdr_misc_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }
    
    static
    {
        System.loadLibrary("jmdr");
    }
}
