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

package com.capmet.metrics.cm;

import D.PN;
import com.capmet.metrics.*;

/**
 * This class is the data contained in the value part
 * of the TreeMap used in the <CODE>CmProcesses</CODE> class. For each
 * process name as the key, this class contains the values.
 */
public class CmProcessMetrics
{
    /**
     * Start time of the process as the number of milliseconds
     * elapsed since 00:00:00 January 1, 1970.
     */
    public CmLong    startTime;             // number of ms since 1/1/1970 00:00:00:00
    /**
     * The unique process identifier of the process.
     */
    public CmLong    processID;             // process id
    /**
     * The parent process's unique process identifier.
     */
    public CmLong    parentProcessID;       // parent's process id
    /**
     * The string identifier of the owner of the process.
     */
    public CmString  userName;              // string name of process owner
    /**
     * The name of the command, not the command line.
     */
    public CmString  commandName;           // just the command name, not command line
    /**
     * The command line, possibly truncated.
     */
    public CmString  commandLine;           // the command line (possibly truncated)
    /**
     * The amount of CPU time used by the process in seconds.
     */
    public CmDouble  totalCPUSeconds;       // whole and fractional
    /**
     * The amount of user time used by the process in seconds.
     */
    public CmDouble  userCPUSeconds;        // whole and fractional
    /**
     * The amount of kernel time used by the process in seconds.
     */
    public CmDouble  kernelCPUSeconds;      // whole and fractional
    /**
     * The percentage of total time represented by user time.
     */
    public CmDouble  percentUserTime;       // percent of total
    /**
     * The percentage of total time represented by kernel time.
     */
    public CmDouble  percentKernelTime;     // percent of total
    /**
     * The virtual image size.
     */
    public CmInteger totalSizeKB;           // virtual size
    /**
     * The resident image size.
     */
    public CmInteger residentSizeKB;        // resident size
    /**
     * The amount of I/O per second performed by the process.
     */
    public CmInteger totalIOKB;             // all IO accountable
    /**
     * The percentage of total possible time represented by the
     * total CPU seconds.
     */
    public CmDouble  percentCPUUse;         // total / (elapsed * min(threads, cpus))
    /**
     * The percentage of physical memory represented by the resident size.
     */
    public CmDouble  percentPhysmemUse;     // resident / physmem
    /**
     * The number of major (disk) faults per second.
     */
    public CmInteger pageFaults;            // major faults, not page reclaims
    /**
     * The string representation of the process state as any of
     * "Runnable", "Sleeping", "Stopped", "Blocked", "Unreaped",
     * "Specific" or "Unknown".
     */
    public CmString  processState;          // one of the Captive states
    /**
     * The priority of the process represented in terms of the global
     * priority of all classes of processes in the system.
     */
    public CmInteger globalPriority;        // unique for each platform
    /**
     * The number of threads in the system context associated with this process.
     */
    public CmInteger numberOfThreads;       // number of threads
    /**
     * The change in the total size from the previous to the current interval.
     */
    public CmInteger totalSizeDeltaKB;      // change in virtual size
    /**
     * The change in the resident size from the previous to the current interval.
     */
    public CmInteger residentSizeDeltaKB;   // change in resident size
    /**
     * The number of system calls per second.
     */
    public CmInteger systemCalls;
    /**
     * The number of context switches per second.
     */
    public CmInteger contextSwitches;
    /**
     * Is this a system process?
     */
    public CmBoolean isSystemProcess;

    private boolean isSupported(long supportFlags, int bit)
    {
        return ((1 << bit) & supportFlags) != 0;
    }

    /**
     * Lone constructor for the class.
     * @param src The <CODE>PN</CODE> object containing process information.
     * @param supportFlags A bit field representing which fields are supported.
     */
    public CmProcessMetrics(D.PN src, long supportFlags)
    {
        startTime = new CmLong(src.startTime,
            isSupported(supportFlags, PN.PN_START_TIME));
        processID = new CmLong(src.processID,
            isSupported(supportFlags, PN.PN_PROCESS_ID));
        parentProcessID = new CmLong(src.parentProcessID,
            isSupported(supportFlags, PN.PN_PARENT_PROCESS_ID));
        userName = new CmString(src.userName,
            isSupported(supportFlags, PN.PN_USER_NAME));
        commandName = new CmString(src.commandName,
            isSupported(supportFlags, PN.PN_COMMAND_NAME));
        commandLine = new CmString(src.commandLine,
            isSupported(supportFlags, PN.PN_COMMAND_LINE));
        totalCPUSeconds = new CmDouble(src.totalCPUSeconds,
            isSupported(supportFlags, PN.PN_TOTAL_CPU_SECONDS));
        userCPUSeconds = new CmDouble(src.userCPUSeconds,
            isSupported(supportFlags, PN.PN_USER_CPU_SECONDS));
        kernelCPUSeconds = new CmDouble(src.kernelCPUSeconds,
            isSupported(supportFlags, PN.PN_KERNEL_CPU_SECONDS));
        percentUserTime = new CmDouble(src.percentUserTime,
            isSupported(supportFlags, PN.PN_PERCENT_USER_TIME));
        percentKernelTime = new CmDouble(src.percentKernelTime,
            isSupported(supportFlags, PN.PN_PERCENT_KERNEL_TIME));
        totalSizeKB = new CmInteger(src.totalSizeKB,
            isSupported(supportFlags, PN.PN_TOTAL_SIZE_KB));
        residentSizeKB = new CmInteger(src.residentSizeKB,
            isSupported(supportFlags, PN.PN_RESIDENT_SIZE_KB));
        totalIOKB = new CmInteger(src.totalIOKB,
            isSupported(supportFlags, PN.PN_TOTAL_IO_KB));
        percentCPUUse = new CmDouble(src.percentCPUUse,
            isSupported(supportFlags, PN.PN_PERCENT_CPU_USE));
        percentPhysmemUse = new CmDouble(src.percentPhysmemUse,
            isSupported(supportFlags, PN.PN_PERCENT_PHYSMEM_USE));
        pageFaults = new CmInteger(src.pageFaults,
            isSupported(supportFlags, PN.PN_PAGE_FAULTS));
        processState = new CmString(src.processState,
            isSupported(supportFlags, PN.PN_PROCESS_STATE));
        globalPriority = new CmInteger(src.globalPriority,
            isSupported(supportFlags, PN.PN_GLOBAL_PRIORITY));
        numberOfThreads = new CmInteger(src.numberOfThreads,
            isSupported(supportFlags, PN.PN_NUMBER_OF_THREADS));
        totalSizeDeltaKB = new CmInteger(src.totalSizeDeltaKB,
            isSupported(supportFlags, PN.PN_TOTAL_SIZE_DELTA_KB));
        residentSizeDeltaKB = new CmInteger(src.residentSizeDeltaKB,
            isSupported(supportFlags, PN.PN_RESIDENT_SIZE_DELTA_KB));
        systemCalls = new CmInteger(src.systemCalls,
            isSupported(supportFlags, PN.PN_SYSTEM_CALLS));
        contextSwitches = new CmInteger(src.contextSwitches,
            isSupported(supportFlags, PN.PN_CONTEXT_SWITCHES));
        isSystemProcess = new CmBoolean(src.isSystemProcess,
            isSupported(supportFlags, PN.PN_IS_SYSTEM_PROCESS));
    }

    /**
     * Update the class members from the <CODE>PN</CODE> source.
     * @param src The <CODE>PN</CODE> class containing the process information.
     */
    public void update(D.PN src)
    {
        totalCPUSeconds.setValue(src.totalCPUSeconds);
        userCPUSeconds.setValue(src.userCPUSeconds);
        kernelCPUSeconds.setValue(src.kernelCPUSeconds);
        percentUserTime.setValue(src.percentUserTime);
        percentKernelTime.setValue(src.percentKernelTime);
        totalSizeKB.setValue(src.totalSizeKB);
        residentSizeKB.setValue(src.residentSizeKB);
        totalIOKB.setValue(src.totalIOKB);
        percentCPUUse.setValue(src.percentCPUUse);
        percentPhysmemUse.setValue(src.percentPhysmemUse);
        pageFaults.setValue(src.pageFaults);
        processState.setValue(src.processState);
        globalPriority.setValue(src.globalPriority);
        numberOfThreads.setValue(src.numberOfThreads);
        totalSizeDeltaKB.setValue(src.totalSizeDeltaKB);
        residentSizeDeltaKB.setValue(src.residentSizeDeltaKB);
        systemCalls.setValue(src.systemCalls);
        contextSwitches.setValue(src.contextSwitches);
        isSystemProcess.setValue(src.isSystemProcess);
    }
}
