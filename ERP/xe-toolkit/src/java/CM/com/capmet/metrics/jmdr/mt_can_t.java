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
 * mt_can_t is the unit container for the mdr_thread_t class
 * which is a mapping of thread names to mt_can_t.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mt_can_t
{
    /**
     * Thread ID
     */
    public int     mt_thread_id;
    /**
     * Total CPU Time
     */
    public double  mt_cpu_time;
    /**
     * User CPU Time
     */
    public double  mt_user_time;
    /**
     * Kernel CPU Time
     */
    public double  mt_kernel_time;
    /**
     * Percent User Time
     */
    public double  mt_user_time_percent;
    /**
     * Percent Kernel Time
     */
    public double  mt_kernel_time_percent;
    /**
     * Total Size
     */
    public long    mt_size_K;
    /**
     * Resident Size
     */
    public long    mt_rssize_K;
    /**
     * Total I/O
     */
    public long    mt_io_K;
    /**
     * Percent CPU Use
     */
    public double  mt_cpu_percent;
    /**
     * Percent Physmem Use
     */
    public double  mt_memory_percent;
    /**
     * Voluntary Context Switches
     */
    public int     mt_voluntary_csw;
    /**
     * Involuntary Context Switches
     */
    public int     mt_involuntary_csw;
    /**
     * Major Page Faults
     */
    public int     mt_disk_faults;
    /**
     * Minor Page Faults
     */
    public int     mt_page_reclaims;
    /**
     * Thread State
     */
    public int     mt_state;
    /**
     * Global Priority
     */
    public int     mt_priority;
    /**
     * Unix Nice Value
     */
    public int     mt_nice;
    /**
     * Total Size Delta
     */
    public long    mt_size_delta_K;
    /**
     * Resident Size Delta
     */
    public long    mt_rssize_delta_K;

    /**
     * Set all public members to their initial value.
     */
    public void clear()
    {
        mt_thread_id = 0;
        mt_cpu_time = 0;
        mt_user_time = 0;
        mt_kernel_time = 0;
        mt_user_time_percent = 0;
        mt_kernel_time_percent = 0;
        mt_size_K = 0;
        mt_rssize_K = 0;
        mt_io_K = 0;
        mt_cpu_percent = 0;
        mt_memory_percent = 0;
        mt_voluntary_csw = 0;
        mt_involuntary_csw = 0;
        mt_disk_faults = 0;
        mt_page_reclaims = 0;
        mt_state = 0;
        mt_priority = 0;
        mt_nice = 0;
        mt_size_delta_K = 0;
        mt_rssize_delta_K = 0;
    }

    /**
     * The default constructor.
     */
    public mt_can_t()
    {
        clear();
    }
}
