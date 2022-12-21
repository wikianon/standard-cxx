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

import java.awt.Color;
import java.io.PrintStream;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import com.capmet.metrics.CmOSFlavor;
import com.capmet.metrics.rmi.PerformanceException;

public abstract class PureCPURule extends AggregateRule<PureCPUSeedValues> {

    public final static int HIGH_CONTEXT_SWITCHES = CPU_CODES;
    public final static int HIGH_SYSTEM_CALLS     = CPU_CODES + 1;
    public final static int HIGH_MUTEX_SPINS      = CPU_CODES + 2;
    public final static int HIGH_INTERRUPTS       = CPU_CODES + 3;
    public final static int HIGH_SYSTEM_TIME      = CPU_CODES + 4;
    public final static int HIGH_RUNQ             = CPU_CODES + 5;
    public final static int CPU_HOG               = CPU_CODES + 6;
    public final static int CPU_SPIKE             = CPU_CODES + 7;
    public final static int HIGH_WAIT_TIME        = CPU_CODES + 8;
    public final static int CPU_IMBALANCE         = CPU_CODES + 9;

    private class CPUUtilizationRule extends Rule<PureCPUSeedValues> {
        // these values must be seeded by the live rule
        private long       currentTime;         // in milliseconds
        private CmOSFlavor osFlavor;            // which OS
        private int        usrPct;              // % usr time
        private int        sysPct;              // % sys time
        private int        waitPct;             // % wait time
        private int        systemCalls;         // system calls
        private int        contextSwitches;     // context switches
        private int        mutexSpins;          // spins on locked mutices
        private int        interrupts;          // total interrupts
        private int        runnableProcesses;   // PROC_RUNNABLE
        private int        ncpus;               // # of cpus online
        private int        clockMHz;            // processor speed

        private Threshold<Integer> utilProblemThr;
        private Threshold<Integer> usrProblemThr;
        private Threshold<Integer> sysProblemThr;
        private Threshold<Integer> waitProblemThr;
        private Threshold<Integer> runqProblemThr;
        private Threshold<Integer> syscallProblemThr;
        private Threshold<Integer> cswitchProblemThr;
        private Threshold<Integer> mutexProblemThr;
        private Threshold<Integer> interruptProblemThr;
        private Threshold<Integer> cpuHogThr;

        // these values are constants, the rest computed
        private long spikeBegin;         // in milliseconds
        private int  utilProblem;
        private int  usrProblem;
        private int  sysProblem;
        private int  waitProblem;
        private int  runqProblem;
        private int  syscallProblem;
        private int  cswitchProblem;
        private int  mutexProblem;
        private int  interruptProblem;
        private int  cpuHogProblem;

        public CPUUtilizationRule() {
            ruleName = RuleMessages.msgs.getString("CPUUtil_Name");
            shortRuleName = RuleMessages.msgs.getString("Short_CUN");

            utilProblemThr =
                new Threshold<Integer>(
                    "CPU_UTIL_PROBLEM",      30, "%",       3);
            usrProblemThr =
                new Threshold<Integer>(
                    "CPU_USR_PROBLEM",       80, "%",       3);
            sysProblemThr =
                new Threshold<Integer>(
                    "CPU_SYS_PROBLEM",       30, "%",       3);
            waitProblemThr =
                new Threshold<Integer>(
                    "CPU_WAIT_PROBLEM",      20, "%",       3);
            runqProblemThr =
                new Threshold<Integer>(
                    "CPU_RUNQ_PROBLEM",       4, "procs",   4);
            syscallProblemThr =
                new Threshold<Integer>(
                    "CPU_SYSCALL_PROBLEM",   -1, "sc/s",    7);
            cswitchProblemThr =
                new Threshold<Integer>(
                    "CPU_CSWITCH_PROBLEM",   -1, "cs/s",    7);
            mutexProblemThr =
                new Threshold<Integer>(
                    "CPU_MUTEX_PROBLEM",     -1, "spins/s", 5);
            interruptProblemThr =
                new Threshold<Integer>(
                    "CPU_INTERRUPT_PROBLEM", -1, "ints/s",  5);
            cpuHogThr =
                new Threshold<Integer>(
                    "CPU_HOG_PROBLEM",        15, "secs",   3);

            // signify to seedValues that the values need initializing
            utilProblem = -1;

            // track cpu hogs
            spikeBegin = 0;

            // all current platforms track cpu use (at least)
            ruleSupported = true;
        }

        public void seedValues(PureCPUSeedValues sv) {
            currentTime       = sv.currentTime;
            osFlavor          = sv.osFlavor;
            usrPct            = sv.usrPct;
            sysPct            = sv.sysPct;
            waitPct           = sv.waitPct;
            systemCalls       = sv.systemCalls;
            contextSwitches   = sv.contextSwitches;
            mutexSpins        = sv.mutexSpins;
            interrupts        = sv.interrupts;
            runnableProcesses = sv.runnableProcesses;
            ncpus             = sv.ncpus;
            clockMHz          = sv.clockMHz;

            if (utilProblem == -1) {
                // initialize the constants
                utilProblem   = getThreshInteger(utilProblemThr, osFlavor);
                usrProblem    = getThreshInteger(usrProblemThr,  osFlavor);
                sysProblem    = getThreshInteger(sysProblemThr,  osFlavor);
                waitProblem   = getThreshInteger(waitProblemThr, osFlavor);
                runqProblem   = getThreshInteger(runqProblemThr, osFlavor);
                cpuHogProblem = getThreshInteger(cpuHogThr,      osFlavor);
            }

            // if the values are not -1 then they were given a value
            // by the administrator
            //
            if (syscallProblemThr.defaultValue == -1) {
                syscallProblemThr.defaultValue = clockMHz * 500;
                syscallProblem = getThreshInteger(syscallProblemThr, osFlavor);
            }

            if (cswitchProblemThr.defaultValue == -1) {
                cswitchProblemThr.defaultValue = clockMHz * 200 * ncpus;
                cswitchProblem = getThreshInteger(cswitchProblemThr, osFlavor);
            }

            if (mutexProblemThr.defaultValue == -1) {
                mutexProblemThr.defaultValue = clockMHz * ncpus;
                mutexProblem = getThreshInteger(mutexProblemThr, osFlavor);
            }

            if (interruptProblemThr.defaultValue == -1) {
                interruptProblemThr.defaultValue = clockMHz;
                interruptProblem = getThreshInteger(interruptProblemThr, osFlavor);
            }
        }

        public void firePureRule() {
            int utilization = usrPct + sysPct;
            int lowWater = (utilProblem < 5 ? utilProblem : 5);

            // reset this
            problemCode = NO_PROBLEM_DETECTED;

            // if CPU util is below the low-water mark, then it's idle
            if (utilization < lowWater && waitPct < waitProblem) {
                // cancel red alert
                spikeBegin = 0;

                state = Color.WHITE;
                action = RuleMessages.msgs.getString("No_Act");
                explanation = RuleMessages.msgs.getString("CPU_Idle");
                furtherDetail = RuleMessages.msgs.getString("CPU_At") +
                    utilization + " %";
                solution = RuleMessages.msgs.getString("None");
                return;
            }

            // if CPU util is below 30%, then it's not idle but it's not busy
            if (utilization < utilProblem && waitPct < waitProblem) {
                // cancel red alert
                spikeBegin = 0;

                state = Color.GREEN;
                action = RuleMessages.msgs.getString("Mild_Act");
                explanation = RuleMessages.msgs.getString("CPU_Nom");
                furtherDetail = RuleMessages.msgs.getString("CPU_At") +
                    utilization + " %";
                solution = RuleMessages.msgs.getString("None");
                return;
            }

            // CPU util is greater than 30%. Figure out what's up.

            // check for high system time

            if (sysPct >= sysProblem) {
                // cancel red alert
                spikeBegin = 0;

                action = RuleMessages.msgs.getString("High_Sys");
                if (contextSwitches >= cswitchProblem) {
                    state = Color.ORANGE;
                    explanation = RuleMessages.msgs.getString("CSW_Problem");
                    furtherDetail = RuleMessages.msgs.getString("CSW_At") +
                        contextSwitches + "/sec; " +
                        RuleMessages.msgs.getString("CPU_At") +
                        utilization + " %";
                    solution = RuleMessages.msgs.getString("Exam_CSW");
                    problemCode = HIGH_CONTEXT_SWITCHES;
                    return;
                }
                if (systemCalls >= syscallProblem) {
                    state = Color.ORANGE;
                    explanation = RuleMessages.msgs.getString("SysC_Problem");
                    furtherDetail = RuleMessages.msgs.getString("SysC_At") +
                        systemCalls + "/sec; " +
                        RuleMessages.msgs.getString("CPU_At") +
                        utilization + " %";
                    solution = RuleMessages.msgs.getString("Exam_SysC");
                    problemCode = HIGH_SYSTEM_CALLS;
                    return;
                }
                if (mutexSpins >= mutexProblem) {
                    state = Color.ORANGE;
                    explanation = RuleMessages.msgs.getString("Mux_Problem");
                    furtherDetail = RuleMessages.msgs.getString("Mux_At") +
                        mutexSpins + "/sec; " +
                        RuleMessages.msgs.getString("CPU_At") +
                        utilization + " %";
                    solution = RuleMessages.msgs.getString("Exam_HiTC");
                    problemCode = HIGH_MUTEX_SPINS;
                    return;
                }
                if (interrupts >= interruptProblem) {
                    state = Color.ORANGE;
                    explanation = RuleMessages.msgs.getString("Int_Problem");
                    furtherDetail = RuleMessages.msgs.getString("Int_At") +
                        interrupts + "/sec; " +
                        RuleMessages.msgs.getString("CPU_At") +
                        utilization + " %";
                    solution = RuleMessages.msgs.getString("Exam_Dev");
                    problemCode = HIGH_INTERRUPTS;
                    return;
                }

                state = Color.ORANGE;
                explanation = RuleMessages.msgs.getString("Kern_Problem");
                furtherDetail = RuleMessages.msgs.getString("Sys_At") +
                    sysPct + " %";
                solution = RuleMessages.msgs.getString("Exam_HiSys");
                problemCode = HIGH_SYSTEM_TIME;
                return;
            }

            // check for high user time

            if (usrPct >= usrProblem) {
                action = RuleMessages.msgs.getString("High_Usr");
                if (runnableProcesses >= runqProblem) {
                    state = Color.ORANGE;
                    explanation = RuleMessages.msgs.getString("RunQ_Problem");
                    furtherDetail = RuleMessages.msgs.getString("Runq_At") +
                        runnableProcesses + "; " +
                        RuleMessages.msgs.getString("Usr_At") +
                        usrPct + " %";
                    solution = RuleMessages.msgs.getString("Exam_HiRunQ");
                    problemCode = HIGH_RUNQ;
                    return;
                }
                boolean cpuHog = false;
                long spikeDuration = 0;

                state = Color.YELLOW;
                if (spikeBegin != 0) {
                    spikeDuration = currentTime - spikeBegin;
                    spikeDuration /= 1000; // ms -> sec
                    if (spikeDuration >= cpuHogProblem)
                        cpuHog = true;
                } else
                    spikeBegin = currentTime;

                if (cpuHog) {
                    state = Color.ORANGE;
                    explanation = RuleMessages.msgs.getString("CPU_Hog") +
                        spikeDuration + " " +
                        RuleMessages.msgs.getString("Seconds");
                    problemCode = CPU_HOG;
                } else {
                    explanation = RuleMessages.msgs.getString("CPU_Spike");
                    problemCode = CPU_SPIKE;
                }

                furtherDetail = RuleMessages.msgs.getString("Usr_At") +
                    usrPct + " %";
                solution = RuleMessages.msgs.getString("Exam_CPUHog");

                return;
            }

            if (waitPct >= waitProblem) {
                // cancel red alert
                spikeBegin = 0;

                action = RuleMessages.msgs.getString("High_Wait");
                state = Color.ORANGE;
                explanation = RuleMessages.msgs.getString("Wait_Problem");
                furtherDetail = RuleMessages.msgs.getString("Wait_At") +
                    waitPct + " %";
                solution = RuleMessages.msgs.getString("Exam_HighWait");
                problemCode = HIGH_WAIT_TIME;
                return;
            }

            // cancel red alert
            spikeBegin = 0;

            state = Color.GREEN;
            action = RuleMessages.msgs.getString("Mild_Act");
            explanation = RuleMessages.msgs.getString("CPU_Nom");
            furtherDetail = RuleMessages.msgs.getString("CPU_At") +
                utilization + " %";
            solution = RuleMessages.msgs.getString("None");
        }

        public void print(PrintStream out) {
            out.println(formatInteger(utilProblemThr, osFlavor));
            out.println(formatInteger(usrProblemThr, osFlavor));
            out.println(formatInteger(sysProblemThr, osFlavor));
            out.println(formatInteger(waitProblemThr, osFlavor));
            out.println(formatInteger(runqProblemThr, osFlavor));
            out.println(formatInteger(syscallProblemThr, osFlavor));
            out.println(formatInteger(cswitchProblemThr, osFlavor));
            out.println(formatInteger(mutexProblemThr, osFlavor));
            out.println(formatInteger(interruptProblemThr, osFlavor));
            out.println(formatInteger(cpuHogThr, osFlavor));
        }

        // stub
        public void fireLiveRule() throws
            NotBoundException, MalformedURLException,
                RemoteException, PerformanceException {
        }
    }

    private class CPUImbalanceRule extends Rule<PureCPUSeedValues> {
        // these values must be seeded by the live rule
        private CmOSFlavor osFlavor;
        private int        ncpus;
        private int[]      cpuPct;

        private Threshold<Integer> imbalanceProblemThr;
        private int imbalanceProblem;

        public CPUImbalanceRule() {
            ruleName = RuleMessages.msgs.getString("CPUImbal_Name");
            shortRuleName = RuleMessages.msgs.getString("Short_CIN");

            imbalanceProblemThr = new Threshold<Integer>(
                "CPU_IMBALANCE_PROBLEM", 10, "%", 3);

            // signify to seedValues that the values need initializing
            imbalanceProblem = -1;
        }

        public void seedValues(PureCPUSeedValues sv) {
            osFlavor = sv.osFlavor;
            ncpus    = sv.cpuPct.length;
            cpuPct   = sv.cpuPct;

            // bsd has no per-cpu metrics
            ruleSupported = (ncpus > 1) && (osFlavor != CmOSFlavor.MOS_BSD);

            if (imbalanceProblem == -1)
                imbalanceProblem = getThreshInteger(imbalanceProblemThr, osFlavor);
        }

        private int min(int a, int b) {
            return (a < b ? a : b);
        }

        private int max(int a, int b) {
            return (a > b ? a : b);
        }

        public void firePureRule() {
            int highUtil;
            int lowUtil;
            int totalUtil = 0;
            boolean specialCase = false;
    
            // reset this
            problemCode = NO_PROBLEM_DETECTED;

            // check for processor utilization imbalance
            highUtil = 0;
            lowUtil = 100;

            for(int i=0; i<ncpus; i++) {
                totalUtil += cpuPct[i];
                highUtil = max(highUtil, cpuPct[i]);
                lowUtil = min(lowUtil, cpuPct[i]);
                // If either of the CPUs is 80% or greater and the median
                // utilization is below 50%, an alarm should be shown;
                // for instance CPU0 is at 0% and CPU2 is at 90%. The median
                // would be 45% but it's not a good situation with 90 vs. 0.
                if (cpuPct[i] >= 80.0)
                    specialCase = true;
            }

            furtherDetail = RuleMessages.msgs.getString("Util_Diff") +
                (highUtil - lowUtil) + "%";

            totalUtil /= ncpus;

            // if aggregate CPU utilization is below 50%
            if (!specialCase && totalUtil < 50.0) {
                // if aggregate CPU utilization is below 25%
                if (totalUtil < 25.0) {
                    // even if the imbalance violates the rule, it's not
                    // a big deal since the CPUs are not being stressed...
                    state = Color.GREEN;
                    action = RuleMessages.msgs.getString("Mild_Act");
                    explanation = RuleMessages.msgs.getString("Bal_Nom");
                    solution = RuleMessages.msgs.getString("None");
                }
            }

            if ((highUtil - lowUtil) >= imbalanceProblem) {
                state = Color.YELLOW;
                action = RuleMessages.msgs.getString("CPU_Imbalance");
                explanation = RuleMessages.msgs.getString("Imb_Problem");
                solution = RuleMessages.msgs.getString("Exam_Imb");
                problemCode = CPU_IMBALANCE;
                return;
            }

            state = Color.GREEN;
            action = RuleMessages.msgs.getString("Mild_Act");
            explanation = RuleMessages.msgs.getString("Bal_Nom");
            solution = RuleMessages.msgs.getString("None");
        }

        public void print(PrintStream out) {
            out.println(formatInteger(imbalanceProblemThr, osFlavor));
        }

        // stub
        public void fireLiveRule() throws
            NotBoundException, MalformedURLException,
                RemoteException, PerformanceException {
        }
    }

    public PureCPURule() {
        ruleName = RuleMessages.msgs.getString("CPU_Name");
        shortRuleName = RuleMessages.msgs.getString("Short_CPU");

        addRule(new CPUUtilizationRule());
        addRule(new CPUImbalanceRule());
        // add new cpu sub-rules here
    }
}
