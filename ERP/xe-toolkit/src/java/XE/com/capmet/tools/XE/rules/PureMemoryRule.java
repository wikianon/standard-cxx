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
import com.capmet.metrics.CmOSFlavor;

public abstract class PureMemoryRule extends Rule<PureMemorySeedValues> {

    public final static int PAGING_OUT = MEMORY_CODES;
    public final static int PAGING_IN  = MEMORY_CODES + 1;

    // these values must be seeded by the live rule
    private CmOSFlavor osFlavor;        // what OS are we on
    private int        scanRate;        // pages scanned
    private int        pageFaults;      // number of page faults
    private int        pagesPagedIn;    // number of pages paged in
    private int        pagesPagedOut;   // number of pages paged out
    private int        clockMHz;        // clock speed in MHz
    private int        pageSize;        // physical page size
    private long       outSpikeBegin;   // track paging time
    private long       inSpikeBegin;    // track paging time

    private Threshold<Integer> scanWarningThr;
    private Threshold<Integer> scanAlertThr;
    private Threshold<Integer> scanProblemThr;
    private Threshold<Integer> pagingTooLongThr;
    private Threshold<Double> pagingWarningThr;
    private Threshold<Double> pagingAlertThr;
    private Threshold<Double> pagingProblemThr;

    private int    scanWarning;
    private int    scanAlert;
    private int    scanProblem;
    private int    pagingTooLong;
    private double pagingWarning;
    private double pagingAlert;
    private double pagingProblem;

    public PureMemoryRule() {
        ruleName = RuleMessages.msgs.getString("MemoryRule_Name");
        shortRuleName = RuleMessages.msgs.getString("Short_Mem");

        scanWarningThr =
            new Threshold<Integer>(
                "MEM_SCAN_WARNING", -1, "pg/s", 6);
        scanAlertThr =
            new Threshold<Integer>(
                "MEM_SCAN_ALERT",   -1, "pg/s", 6);
        scanProblemThr =
            new Threshold<Integer>(
                "MEM_SCAN_PROBLEM", -1, "pg/s", 6);
        pagingTooLongThr =
            new Threshold<Integer>(
                "PAGING_TOO_LONG",  10, "secs", 4);
        pagingWarningThr =
            new Threshold<Double>(
                "PAGING_WARNING",    2.5, "MB/s", 3, 1);
        pagingAlertThr =
            new Threshold<Double>(
                "PAGING_ALERT",     5.0, "MB/s", 3, 1);
        pagingProblemThr =
            new Threshold<Double>(
                "PAGING_PROBLEM",   10.0, "MB/s", 3, 1);

        // signify to seedValues that the values need initializing
        scanWarning = -1;

        // no sub-rules here
        triggerRuleName = ruleName;

        // reset to zero
        inSpikeBegin = 0;
        outSpikeBegin = 0;
    }

    public void seedValues(PureMemorySeedValues sv) {
        osFlavor        = sv.osFlavor;
        scanRate        = sv.scanRate;
        pageFaults      = sv.pageFaults;
        pagesPagedIn    = sv.pagesPagedIn;
        pagesPagedOut   = sv.pagesPagedOut;
        clockMHz        = sv.clockMHz;
        pageSize        = sv.pageSize;

        if (scanWarning == -1) {
            scanProblemThr.defaultValue = (clockMHz * 30) * (8192 / pageSize);
            scanProblem = getThreshInteger(scanProblemThr, osFlavor);

            scanWarningThr.defaultValue = scanProblem / 10;
            scanWarning = getThreshInteger(scanWarningThr, osFlavor);

            scanAlertThr.defaultValue = (scanProblem + scanWarning) / 2;
            scanAlert = getThreshInteger(scanAlertThr, osFlavor);

            pagingTooLong = getThreshInteger(pagingTooLongThr, osFlavor);
            pagingWarning = getThreshDouble(pagingWarningThr, osFlavor);
            pagingAlert   = getThreshDouble(pagingAlertThr,   osFlavor);
            pagingProblem = getThreshDouble(pagingProblemThr, osFlavor);
        }

        ruleSupported = true;
    }

    private String scanPageMessage() {
        return RuleMessages.msgs.getString("Scan_Rate")
               + " " + scanRate + " pg/s; " +
               RuleMessages.msgs.getString("Pages_Out")
               + " " + pagesPagedOut + " pg/s";
    }

    public void firePureRule() {

        // reset this
        problemCode = NO_PROBLEM_DETECTED;

        if (osFlavor.isUnixHost()) {
            // No scanning. This is good.
            if (scanRate == 0) {
                state = Color.WHITE;
                action = RuleMessages.msgs.getString("No_Act");
                explanation = RuleMessages.msgs.getString("Pager_Idle");
                furtherDetail = RuleMessages.msgs.getString("Scan_Rate") + " 0";
                solution = RuleMessages.msgs.getString("None");
                return;
            }

            // Scanning, but no pages being written to swap. Not bad.
            if (pagesPagedOut == 0) {
                state = Color.GREEN;
                action = RuleMessages.msgs.getString("Mild_Act");
                explanation = RuleMessages.msgs.getString("Scan_Only");
                furtherDetail = RuleMessages.msgs.getString("Scan_Rate")
                              + " " + scanRate + " pg/s";
                solution = RuleMessages.msgs.getString("None");
                return;
            }

            // Scanning and paging, but less than a megabyte/second.
            // Not great, but not a disaster.

            furtherDetail = scanPageMessage();
            problemCode = PAGING_OUT;

            if ((pagesPagedOut * pageSize) < (1024*1024)) {
                state = Color.YELLOW;
                action = RuleMessages.msgs.getString("Mem_Cont");
                explanation = RuleMessages.msgs.getString("Mild_Scan");
                solution = RuleMessages.msgs.getString("Exam_Growing");
                return;
            }

            // there is scanning and (paging greater than 1MB/sec)

            if (scanRate < scanAlert) {
                state = Color.ORANGE;
                action = RuleMessages.msgs.getString("Mem_Low");
                explanation = RuleMessages.msgs.getString("RAM_Out");
                solution = RuleMessages.msgs.getString("Exam_Large");
                return;
            } else if (scanRate < scanProblem)
                state = Color.RED;

            // else throttle state against bus bandwidth
            //
            // Sun Solaris: 1062 MHz CPU
            //
            //   1062*30*1=31860                  # scan problem level
            //   31860/(12*1)=2655                # pages out problem level
            //   2655*8192=21749760               # times page size
            //   21749760/(1024*1024)=20.74 MB/s  # bus load
            //
            // PC FreeBSD: 1600 MHz CPU
            //
            //   1600*30*2=96000
            //   96000/(12*2)=4000
            //   4000*4096=16384000
            //   13107200/(1024*1024)=15.63 MB/s
            //
            // Mac Darwin: 1249 MHz CPU
            //
            //   1249*30*2=74940
            //   74940/(12*2)=3123
            //   3123*4096=12791808
            //   12791808/(1024*1024)=12.20 MB/s

            else if (pagesPagedOut >= (scanRate / (12 * (8192 / pageSize))))
                state = Color.BLACK;
            else
                state = Color.RED;

            // state is either red or black at this point

            if (state.equals(Color.RED)) {
                action = RuleMessages.msgs.getString("RAM_Depleted");
                explanation = RuleMessages.msgs.getString("RAM_RED");
                solution = RuleMessages.msgs.getString("Stop_Large");
            } else {
                action = RuleMessages.msgs.getString("No_Free_Mem");
                explanation = RuleMessages.msgs.getString("RAM_BLACK");
                solution = RuleMessages.msgs.getString("Kill_Large");
            }
        } else if (osFlavor.isWindowsHost()) {

            // no page faults; totally quiescent
            if (pageFaults == 0) {
                state = Color.WHITE;
                action = RuleMessages.msgs.getString("No_Act");
                explanation = RuleMessages.msgs.getString("Pager_Idle");
                furtherDetail = RuleMessages.msgs.getString("Page_Faults")
                              + "0";
                solution = RuleMessages.msgs.getString("None");
                outSpikeBegin = 0;
                inSpikeBegin = 0;
                return;
            }

            // page faults but no pages paged out or in
            if (pagesPagedOut == 0 && pagesPagedIn == 0) {
                state = Color.GREEN;
                action = RuleMessages.msgs.getString("Mild_Act");
                explanation = RuleMessages.msgs.getString("Fault_Only");
                furtherDetail = RuleMessages.msgs.getString("Page_Faults")
                              + pageFaults + " pg/s";
                solution = RuleMessages.msgs.getString("None");
                outSpikeBegin = 0;
                inSpikeBegin = 0;
                return;
            }

            // address the more problematic situation, lots-o-pages-out

            if (pagesPagedOut > pagesPagedIn) {
                inSpikeBegin = 0;

                // page faults and disk faults, but the first time seen
                furtherDetail = RuleMessages.msgs.getString("Pages_Out")
                              + pagesPagedOut + " pg/sec";

                double bandwidth = pagesPagedOut * pageSize / (1024.0 * 1024.0);

                if (outSpikeBegin == 0) {
                    outSpikeBegin = System.currentTimeMillis();
                    explanation = RuleMessages.msgs.getString("Pager_Out");
                    solution = RuleMessages.msgs.getString("Attention");
                    if (bandwidth < pagingWarning) {
                        state = Color.GREEN;
                        action = RuleMessages.msgs.getString("Mild_Act");
                    } else {
                        state = Color.YELLOW;
                        action = RuleMessages.msgs.getString("Heated_Act");
                        problemCode = PAGING_OUT;
                    }
                    return;
                }
                long now = System.currentTimeMillis();
                long elapsed = (now - outSpikeBegin) / 1000;

                // how long has this been going on?
                if (elapsed >= pagingTooLong) {

                    if (bandwidth < pagingWarning) {
                        state = Color.GREEN;
                        action = RuleMessages.msgs.getString("Mild_Act");
                        explanation = RuleMessages.msgs.getString("Pager_Out");
                        solution = RuleMessages.msgs.getString("Attention");
                        outSpikeBegin = 0;
                    } else if (bandwidth < pagingAlert) {
                        state = Color.YELLOW;
                        action = RuleMessages.msgs.getString("Heated_Act");
                        explanation = RuleMessages.msgs.getString("RAM_Out");
                        solution = RuleMessages.msgs.getString("Exam_Growing");
                        problemCode = PAGING_OUT;
                    } else if (bandwidth < pagingProblem) {
                        state = Color.ORANGE;
                        action = RuleMessages.msgs.getString("RAM_Depleted");
                        explanation = RuleMessages.msgs.getString("RAM_RED");
                        solution = RuleMessages.msgs.getString("Stop_Large");
                        problemCode = PAGING_OUT;
                    } else {
                        state = Color.RED;
                        action = RuleMessages.msgs.getString("No_Free_Mem");
                        explanation = RuleMessages.msgs.getString("RAM_BLACK");
                        solution = RuleMessages.msgs.getString("Kill_Large");
                        problemCode = PAGING_OUT;
                    }
                } else {
                    explanation = RuleMessages.msgs.getString("Pager_Out");
                    solution = RuleMessages.msgs.getString("Attention");
                    if (bandwidth < pagingWarning) {
                        state = Color.GREEN;
                        action = RuleMessages.msgs.getString("Mild_Act");
                        outSpikeBegin = 0;
                    } else {
                        state = Color.YELLOW;
                        action = RuleMessages.msgs.getString("Heated_Act");
                        problemCode = PAGING_OUT;
                    }
                }

                return;
            }

            // pagesPagedIn wins

            outSpikeBegin = 0;

            // page faults and disk faults, but the first time seen
            furtherDetail = RuleMessages.msgs.getString("Pages_In")
                          + pagesPagedIn + " pg/sec";

            double bandwidth = pagesPagedIn * pageSize / (1024.0 * 1024.0);

            // I/O subsystem using the pager to read files
            if (pagesPagedOut == 0) {
                state = Color.GREEN;
                action = RuleMessages.msgs.getString("Mild_Act");
                explanation = RuleMessages.msgs.getString("IO_Pager_In");
                solution = RuleMessages.msgs.getString("None");
                return;
            }

            if (inSpikeBegin == 0) {
                inSpikeBegin = System.currentTimeMillis();
                explanation = RuleMessages.msgs.getString("Pager_In");
                solution = RuleMessages.msgs.getString("Attention");
                if (bandwidth < (pagingWarning / 2)) {
                    state = Color.GREEN;
                    action = RuleMessages.msgs.getString("Mild_Act");
                } else {
                    state = Color.YELLOW;
                    action = RuleMessages.msgs.getString("Heated_Act");
                    problemCode = PAGING_IN;
                }
                return;
            }
            long now = System.currentTimeMillis();
            long elapsed = (now - inSpikeBegin) / 1000;

            // how long has this been going on?
            if (elapsed >= pagingTooLong) {

                if (bandwidth < (pagingWarning / 2)) {
                    state = Color.GREEN;
                    action = RuleMessages.msgs.getString("Mild_Act");
                    explanation = RuleMessages.msgs.getString("Pager_In");
                    solution = RuleMessages.msgs.getString("Attention");
                    inSpikeBegin = 0;
                } else if (bandwidth < (pagingAlert / 2)) {
                    state = Color.YELLOW;
                    action = RuleMessages.msgs.getString("Heated_Act");
                    explanation = RuleMessages.msgs.getString("RAM_In");
                    solution = RuleMessages.msgs.getString("Exam_Growing");
                    problemCode = PAGING_IN;
                } else if (bandwidth < (pagingProblem / 2)) {
                    state = Color.ORANGE;
                    action = RuleMessages.msgs.getString("Dem_Great");
                    explanation = RuleMessages.msgs.getString("RAM_In");
                    solution = RuleMessages.msgs.getString("Exam_Large");
                    problemCode = PAGING_IN;
                } else {
                    state = Color.RED;
                    action = RuleMessages.msgs.getString("All_Done");
                    explanation = RuleMessages.msgs.getString("RAM_In");
                    solution = RuleMessages.msgs.getString("Exam_Large");
                    problemCode = PAGING_IN;
                }
            } else {
                explanation = RuleMessages.msgs.getString("Pager_In");
                solution = RuleMessages.msgs.getString("Attention");
                if (bandwidth < (pagingWarning / 2)) {
                    state = Color.GREEN;
                    action = RuleMessages.msgs.getString("Mild_Act");
                } else {
                    state = Color.YELLOW;
                    action = RuleMessages.msgs.getString("Heated_Act");
                    problemCode = PAGING_IN;
                }
            }
        }
    }

    public void print(PrintStream out) {
        out.println(formatInteger(scanWarningThr,   osFlavor));
        out.println(formatInteger(scanAlertThr,     osFlavor));
        out.println(formatInteger(scanProblemThr,   osFlavor));
        out.println(formatInteger(pagingTooLongThr, osFlavor));
        out.println(formatDouble(pagingWarningThr,  osFlavor));
        out.println(formatDouble(pagingAlertThr,    osFlavor));
        out.println(formatDouble(pagingProblemThr,  osFlavor));
    }
}
