/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the XE library for The XE Toolkit.
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

package com.capmet.tools.XE.rules;

import com.capmet.metrics.CmOSFlavor;

/**
 * Values seeded into <CODE>PureCPURule</CODE> by <CODE>LiveCPURule</CODE>.
 */
public class PureCPUSeedValues {
    /**
     * The current time at rule evaluation.
     */
    public long currentTime;
    /**
     * The last time of rule evaluation.
     */
    public long lastTime;
    /**
     * The flavor os the OS being monitored.
     */
    public CmOSFlavor osFlavor;            // which OS
    /**
     * The percent user CPU time.
     */
    public int        usrPct;              // % usr time
    /**
     * The percent system CPU time.
     */
    public int        sysPct;              // % sys time
    /**
     * The percent wait CPU time.
     */
    public int        waitPct;             // % wait time
    /**
     * The number of systems calls per second.
     */
    public int        systemCalls;         // system calls
    /**
     * The number of context switches per second.
     */
    public int        contextSwitches;     // context switches
    /**
     * The number of failed adaptive mutex enters (Solaris).
     */
    public int        mutexSpins;          // spins on locked mutices
    /**
     * The number of hardware interrupts per second.
     */
    public int        interrupts;          // total interrupts
    /**
     * The number of processes marked as running or runnable.
     */
    public int        runnableProcesses;   // PROC_RUNNABLE
    /**
     * The number of CPUs online.
     */
    public int        ncpus;               // # of cpus online
    /**
     * The clock speed of the CPUs in megahertz.
     */
    public int        clockMHz;            // processor speed
    /**
     * The sum of user and system CPU time for each online CPU.
     */
    public int[]      cpuPct;              // usr+sys for each cpu
}
