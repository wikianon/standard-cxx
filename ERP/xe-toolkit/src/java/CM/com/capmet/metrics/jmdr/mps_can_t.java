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

/**
 * mps_can_t is the unit container for the mdr_ps_t class
 * which is a mapping of process names to mps_can_t.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mps_can_t
{
    /**
     * Start Time
     */
    public long   mps_start;
    /**
     * User ID
     */
    public int    mps_uid;
    /**
     * Effective User ID
     */
    public int    mps_euid;
    /**
     * Process ID
     */
    public int    mps_pid;
    /**
     * Parent Process ID
     */
    public int    mps_ppid;
    /**
     * Terminal Device ID
     */
    public long   mps_tty_device;
    /**
     * File Name
     */
    public String mps_file_name;
    /**
     * Full Command
     */
    public String mps_command;
    /**
     * First Argument
     */
    public String mps_first_argument;
    /**
     * Total CPU Time
     */
    public double mps_cpu_time;
    /**
     * User CPU Time
     */
    public double mps_user_time;
    /**
     * Kernel CPU Time
     */
    public double mps_kernel_time;
    /**
     * Percent User Time
     */
    public double mps_user_time_percent;
    /**
     * Percent Kernel Time
     */
    public double mps_kernel_time_percent;
    /**
     * Total Size
     */
    public long   mps_size_K;
    /**
     * Resident Size
     */
    public long   mps_rssize_K;
    /**
     * Total I/O
     */
    public long   mps_io_K;
    /**
     * Percent CPU Use
     */
    public double mps_cpu_percent;
    /**
     * Percent Physmem Use
     */
    public double mps_memory_percent;
    /**
     * Voluntary Context Switches
     */
    public int    mps_voluntary_csw;
    /**
     * Involuntary Context Switches
     */
    public int    mps_involuntary_csw;
    /**
     * Major Page Faults
     */
    public int    mps_disk_faults;
    /**
     * Minor Page Faults
     */
    public int    mps_page_reclaims;
    /**
     * Process State
     */
    public int    mps_state;
    /**
     * Global Priority
     */
    public int    mps_priority;
    /**
     * Unix Nice Value
     */
    public int    mps_nice;
    /**
     * Number of Threads
     */
    public int    mps_thread_count;
    /**
     * Total Size Delta
     */
    public long   mps_size_delta_K;
    /**
     * Resident Size Delta
     */
    public long   mps_rssize_delta_K;
    /**
     * User Name
     */
    public String mps_user_name;
    /**
     * Effective User Name
     */
    public String mps_effective_user_name;
    /**
     * System Calls
     */
    public int mps_system_calls;
    /**
     * Is System Process
     */
    public boolean mps_is_system_process;

    /**
     * Set all public members to their initial value.
     */
    public void clear()
    {
        mps_start = 0;
        mps_uid = 0;
        mps_euid = 0;
        mps_pid = 0;
        mps_ppid = 0;
        mps_tty_device = 0;
        mps_file_name = null;
        mps_command = null;
        mps_first_argument = null;
        mps_cpu_time = 0.0;
        mps_user_time = 0.0;
        mps_kernel_time = 0.0;
        mps_user_time_percent = 0.0;
        mps_kernel_time_percent = 0.0;
        mps_size_K = 0;
        mps_rssize_K = 0;
        mps_io_K = 0;
        mps_cpu_percent = 0;
        mps_memory_percent = 0;
        mps_voluntary_csw = 0;
        mps_involuntary_csw = 0;
        mps_disk_faults = 0;
        mps_page_reclaims = 0;
        mps_state = 0;
        mps_priority = 0;
        mps_nice = 0;
        mps_thread_count = 0;
        mps_size_delta_K = 0;
        mps_rssize_delta_K = 0;
        mps_user_name = null;
        mps_effective_user_name = null;
        mps_system_calls = 0;
        mps_is_system_process = false;
    }

    /**
     * The default constructor.
     */
    public mps_can_t()
    {
        clear();
    }
}
