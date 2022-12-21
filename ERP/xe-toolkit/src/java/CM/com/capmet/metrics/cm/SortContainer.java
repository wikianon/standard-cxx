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

/**
 * A <CODE>Comparable</CODE> container that holds a <CODE>CmProcessMetrics</CODE>
 * object and the field index to sort by. This class is used as a type parameter
 * to <CODE>Sort</CODE> by the <CODE>CmProcesses</CODE> class.
 */
public class SortContainer implements Comparable<SortContainer>
{
    // private members in support of the class
    private CmProcessMetrics metrics;
    private int              fieldIndex;

    /**
     * The lone constructor for initializing the container.
     * @param metrics The <CODE>CmProcessMetrics</CODE> object.
     * @param fieldIndex The index of the field to sort by.
     */
    public SortContainer(CmProcessMetrics metrics, int fieldIndex)
    {
        this.metrics = metrics;
        this.fieldIndex = fieldIndex;
    }

    /**
     * Retrieve the <CODE>CmProcessMetrics</CODE> object.
     * @return The private object held by the container.
     */
    public CmProcessMetrics getMetrics()
    {
        return this.metrics;
    }

    /**
     * Compare this container to another container.
     * @param c The <CODE>SortContainer</CODE> to compare to this.
     * @return <0: the parameter is greater than this
     * 0: the parameter is equal to this
     * >0: the parameter is less than this
     */
    public int compareTo(SortContainer c)
    {
        double left;
        double right;

        switch(c.fieldIndex) {
        case PN.PN_START_TIME:
            return (int) (metrics.startTime.longValue() - c.metrics.startTime.longValue());
        case PN.PN_PROCESS_ID:
            return (int) (metrics.processID.longValue() - c.metrics.processID.longValue());
        case PN.PN_PARENT_PROCESS_ID:
            return (int) (metrics.parentProcessID.longValue() - c.metrics.parentProcessID.longValue());
        case PN.PN_USER_NAME:
            return metrics.userName.toString().compareTo(c.metrics.userName.toString());
        case PN.PN_COMMAND_NAME:
            return metrics.commandName.toString().compareTo(c.metrics.commandName.toString());
        case PN.PN_COMMAND_LINE:
            return metrics.commandLine.toString().compareTo(c.metrics.commandLine.toString());
        case PN.PN_TOTAL_CPU_SECONDS:
            left = metrics.totalCPUSeconds.doubleValue();
            right = c.metrics.totalCPUSeconds.doubleValue();
            if (left > right)
                return 1;
            if (left < right)
                return -1;
            return 0;
        case PN.PN_USER_CPU_SECONDS:
            left = metrics.userCPUSeconds.doubleValue();
            right = c.metrics.userCPUSeconds.doubleValue();
            if (left > right)
                return 1;
            if (left < right)
                return -1;
            return 0;
        case PN.PN_KERNEL_CPU_SECONDS:
            left = metrics.kernelCPUSeconds.doubleValue();
            right = c.metrics.kernelCPUSeconds.doubleValue();
            if (left > right)
                return 1;
            if (left < right)
                return -1;
            return 0;
        case PN.PN_PERCENT_USER_TIME:
            left = metrics.percentUserTime.doubleValue();
            right = c.metrics.percentUserTime.doubleValue();
            if (left > right)
                return 1;
            if (left < right)
                return -1;
            return 0;
        case PN.PN_PERCENT_KERNEL_TIME:
            left = metrics.percentKernelTime.doubleValue();
            right = c.metrics.percentKernelTime.doubleValue();
            if (left > right)
                return 1;
            if (left < right)
                return -1;
            return 0;
        case PN.PN_TOTAL_SIZE_KB:
            return metrics.totalSizeKB.intValue() - c.metrics.totalSizeKB.intValue();
        case PN.PN_RESIDENT_SIZE_KB:
            return metrics.residentSizeKB.intValue() - c.metrics.residentSizeKB.intValue();
        case PN.PN_TOTAL_IO_KB:
            return metrics.totalIOKB.intValue() - c.metrics.totalIOKB.intValue();
        case PN.PN_PERCENT_CPU_USE:
            left = metrics.percentCPUUse.doubleValue();
            right = c.metrics.percentCPUUse.doubleValue();
            if (left > right)
                return 1;
            if (left < right)
                return -1;
            return 0;
        case PN.PN_PERCENT_PHYSMEM_USE:
            left = metrics.percentPhysmemUse.doubleValue();
            right = c.metrics.percentPhysmemUse.doubleValue();
            if (left > right)
                return 1;
            if (left < right)
                return -1;
            return 0;
        case PN.PN_PAGE_FAULTS:
            return metrics.pageFaults.intValue() - c.metrics.pageFaults.intValue();
        case PN.PN_PROCESS_STATE:
            return metrics.processState.toString().compareTo(c.metrics.processState.toString());
        case PN.PN_GLOBAL_PRIORITY:
            return metrics.globalPriority.intValue() - c.metrics.globalPriority.intValue();
        case PN.PN_NUMBER_OF_THREADS:
            return metrics.numberOfThreads.intValue() - c.metrics.numberOfThreads.intValue();
        case PN.PN_TOTAL_SIZE_DELTA_KB:
            return metrics.totalSizeDeltaKB.intValue() - c.metrics.totalSizeDeltaKB.intValue();
        case PN.PN_RESIDENT_SIZE_DELTA_KB:
            return metrics.residentSizeDeltaKB.intValue() - c.metrics.residentSizeDeltaKB.intValue();
        case PN.PN_SYSTEM_CALLS:
            return metrics.systemCalls.intValue() - c.metrics.systemCalls.intValue();
        case PN.PN_CONTEXT_SWITCHES:
            return metrics.contextSwitches.intValue() - c.metrics.contextSwitches.intValue();
        }

        return 0;
    }
}
