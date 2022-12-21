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
 * mctl_can_t is the unit container for the mdr_ctlr_t class
 * which is a mapping of controller name to mctl_can_t.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mctl_can_t
{
    /**
     * The number of read operations from disks attached to this controller.
     */
    public int    mctl_reads;            // Read Operations
    /**
     * The number of kilobytes read from disks attached to this controller.
     */
    public int    mctl_read_K;           // KBytes Read
    /**
     * The number of milliseconds accumulated while reading from
     * disks attached to this controller.
     */
    public int    mctl_read_msecs;       // Read Time
    /**
     * The number of write operations to disks attached to this controller.
     */
    public int    mctl_writes;           // Write Operations
    /**
     * The number of kilobytes written to disks attached to this controller.
     */
    public int    mctl_written_K;        // KBytes Written
    /**
     * The number of milliseconds accumulated while writing to
     * disks attached to this controller.
     */
    public int    mctl_write_msecs;      // Write Time
    /**
     * The number of milliseconds accumulated while
     * I/O operations were running on disks attached to this controller.
     */
    public int    mctl_run_msecs;        // Busy Time
    /**
     * The number of milliseconds accumulated while waiting for
     * I/O operations to complete on disks attached to this controller.
     */
    public int    mctl_wait_msecs;       // Wait Time
    /**
     * The queue length X run time product for all disks
     * attached to this controller.
     */
    public int    mctl_runq_msecs;       // Q-Length*Run-Time
    /**
     * The queue length X wait time product for all disks
     * attached to this controller.
     */
    public int    mctl_waitq_msecs;      // Q-Length*Wait-Time
    /**
     * The average queue length of disks attached to this controller.
     */
    public double mctl_avg_queue_length; // Average Length of Request Queue
    /**
     * The percentage of run time against elapsed time X number of disks
     * attached to this controller.
     */
    public double mctl_util_percent;     // Percent Time Busy
    /**
     * Average access time of all disks attached to this controller.
     */
    public double mctl_service_time;     // Average Access Time
    /**
     * Number of disks attached to this controller.
     */
    public int    mctl_disk_count;       // Disks on Controller

    /**
     * Set all public members to their initial value.
     */
    public void clear()
    {
        mctl_reads = 0;
        mctl_read_K = 0;
        mctl_read_msecs = 0;
        mctl_writes = 0;
        mctl_written_K = 0;
        mctl_write_msecs = 0;
        mctl_run_msecs = 0;
        mctl_wait_msecs = 0;
        mctl_runq_msecs = 0;
        mctl_waitq_msecs = 0;
        mctl_avg_queue_length = 0.0;
        mctl_util_percent = 0.0;
        mctl_service_time = 0.0;
        mctl_disk_count = 0;
    }

    /**
     * The default constructor for mctl_can_t.
     */
    public mctl_can_t()
    {
        clear();
    }
}
