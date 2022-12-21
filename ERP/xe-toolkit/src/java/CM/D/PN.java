/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Captive Metrics library for Captivity.
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

package D;

import java.io.Serializable;

/**
 * PN is the Process Node class. This is the value type of the PT tree structure and
 * contains the information about processes that is passed from CaptiveCollector on the
 * server to the client process.
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.2
 */
public class PN implements Serializable
{
    /**
     * These are the indices for the members of the class. They are used by the server and
     * client side to construct a bit mask representing whether the class member is supported
     * on the server platform.
     */
    public final static int PN_START_TIME             = 0;
    public final static int PN_PROCESS_ID             = 1;
    public final static int PN_PARENT_PROCESS_ID      = 2;
    public final static int PN_USER_NAME              = 3;
    public final static int PN_COMMAND_NAME           = 4;
    public final static int PN_COMMAND_LINE           = 5;
    public final static int PN_TOTAL_CPU_SECONDS      = 6;
    public final static int PN_USER_CPU_SECONDS       = 7;
    public final static int PN_KERNEL_CPU_SECONDS     = 8;
    public final static int PN_PERCENT_USER_TIME      = 9;
    public final static int PN_PERCENT_KERNEL_TIME    = 10;
    public final static int PN_TOTAL_SIZE_KB          = 11;
    public final static int PN_RESIDENT_SIZE_KB       = 12;
    public final static int PN_TOTAL_IO_KB            = 13;
    public final static int PN_PERCENT_CPU_USE        = 14;
    public final static int PN_PERCENT_PHYSMEM_USE    = 15;
    public final static int PN_PAGE_FAULTS            = 16;
    public final static int PN_PROCESS_STATE          = 17;
    public final static int PN_GLOBAL_PRIORITY        = 18;
    public final static int PN_NUMBER_OF_THREADS      = 19;
    public final static int PN_TOTAL_SIZE_DELTA_KB    = 20;
    public final static int PN_RESIDENT_SIZE_DELTA_KB = 21;
    public final static int PN_SYSTEM_CALLS           = 22;
    public final static int PN_CONTEXT_SWITCHES       = 23;
    public final static int PN_IS_SYSTEM_PROCESS      = 24;
    public final static int PN_FIELD_COUNT            = 25;

    /**
     * The start time of the process represented as the number of milliseconds transpired
     * since the Unix epoch, January 1, 1970 00:00:00.
     */
    public long   startTime;
    /**
     * The numeric identifier of the process.
     */
    public long   processID;
    /**
     * The numeric identifier of the process that created this process.
     */
    public long   parentProcessID;
    /**
     * The text description of the person or entity whose numeric user identifier was associated
     * with the creation of the process.
     */
    public String userName;
    /**
     * The name of the executable program.
     */
    public String commandName;
    /**
     * The full command line or as much is accessible on the server platform.
     */
    public String commandLine;
    /**
     * Total number of CPU seconds executed by the process.
     */
    public double totalCPUSeconds;
    /**
     * Total number of CPU seconds executed by the process in user space.
     */
    public double userCPUSeconds;
    /**
     * Total number of CPU seconds executed by the process in kernel space.
     */
    public double kernelCPUSeconds;
    /**
     * The percentage of the total time spent in user time.
     */
    public double percentUserTime;
    /**
     * The percentage of the total time spent in kernel time.
     */
    public double percentKernelTime;
    /**
     * The virtual size of the process in kilobytes.
     */
    public int    totalSizeKB;
    /**
     * The resident (in memory) size of the process in kilobytes.
     */
    public int    residentSizeKB;
    /**
     * The amount of I-O per second from all sources in kilobytes.
     */
    public int    totalIOKB;
    /**
     * The amount of time spent running over the interval as a percentage of the length
     * of time in the interval.
     */
    public double percentCPUUse;
    /**
     * The resident size of the process as a percentage of the amount of physical memory
     * present in the server system.
     */
    public double percentPhysmemUse;
    /**
     * The number of major (disk) faults incurred by the process per second.
     */
    public int    pageFaults;
    /**
     * The running state of the process. Possible values are Runnable, Sleeping, Blocked, Stopped,
     * Unreaped and Specific.
     */
    public String processState;
    /**
     * The numeric value of the global (not execution class specific) process priority.
     */
    public int    globalPriority;
    /**
     * The number of threads in the system context that share this process' address space.
     */
    public int    numberOfThreads;
    /**
     * The different in total size between the current interval and the previous.
     */
    public int    totalSizeDeltaKB;
    /**
     * The different in resident size between the current interval and the previous.
     */
    public int    residentSizeDeltaKB;
    /**
     * The number of system calls made by the process per second.
     */
    public int    systemCalls;
    /**
     * The number of context switches made by the process per second.
     * Some systems split this value into voluntary and involuntary. Some systems
     * do not indicate which it is, and on those systems, it is assumed that the
     * value is the sum of both. On those that do break them down, this value is
     * the sum of the two.
     */
    public int    contextSwitches;
     /**
      * Indication of whether this process is a system-only process.
      */
    public boolean isSystemProcess;
}
