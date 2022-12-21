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
 * md_can_t is the unit container for the mdr_disk_t class
 * which is a mapping of disk name to md_can_t.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class md_can_t
{
    /**
     * Disk type: "physical", "logical" or "network".
     */
    public String   md_type;             // Disk Type
    /**
     * The number of read operations from this disk.
     */
    public int      md_reads;            // Read Operations
    /**
     * The number of kilobytes read from this disk.
     */
    public int      md_read_K;           // KBytes Read
    /**
     * The number of milliseconds accumulated while reading from this disk.
     */
    public int      md_read_msecs;       // Read Time
    /**
     * The number of write operations to this disk.
     */
    public int      md_writes;           // Write Operations
    /**
     * The number of kilobytes written to this disk.
     */
    public int      md_written_K;        // KBytes Written
    /**
     * The number of milliseconds accumulated while writing to this disk.
     */
    public int      md_write_msecs;      // Write Time
    /**
     * The number of milliseconds accumulated while
     * I/O operations were running on this disk.
     */
    public int      md_run_msecs;        // Run Time
    /**
     * The number of milliseconds accumulated while waiting for
     * I/O operations to complete on this disk.
     */
    public int      md_wait_msecs;       // Wait Time
    /**
     * The queue length X run time product for this disk.
     */
    public int      md_runq_msecs;       // Q-Length*Run-Time
    /**
     * The queue length X wait time product for this disk.
     */
    public int      md_waitq_msecs;      // Q-Length*Wait-Time
    /**
     * The average queue length of this disk.
     */
    public double   md_avg_queue_length; // Average Request Queue Length
    /**
     * The percentage of run time against elapsed time for this disk.
     */
    public double   md_util_percent;     // Percent Time Busy
    /**
     * Average access time of this disk.
     */
    public double   md_service_time;     // Average Access Time
    /**
     * Total size of this disk in megabytes.
     */
    public int      md_space_mb;         // Megabytes of Capacity

    /**
     * Set all public members to their initial value.
     */
    public void clear()
    {
        md_type = null;
        md_reads = 0;
        md_read_K = 0;
        md_read_msecs = 0;
        md_writes = 0;
        md_written_K = 0;
        md_write_msecs = 0;
        md_run_msecs = 0;
        md_wait_msecs = 0;
        md_runq_msecs = 0;
        md_waitq_msecs = 0;
        md_avg_queue_length = 0.0;
        md_util_percent = 0.0;
        md_service_time = 0.0;
        md_space_mb = 0;
    }

    /**
     * The default constructor for md_can_t.
     */
    public md_can_t()
    {
        clear();
    }
}
