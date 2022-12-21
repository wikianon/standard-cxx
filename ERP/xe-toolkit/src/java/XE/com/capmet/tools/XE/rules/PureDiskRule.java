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
import java.util.Formatter;
import com.capmet.metrics.CmOSFlavor;

public abstract class PureDiskRule extends Rule<PureDiskSeedValues> {

    public final static int HIGH_SERVICE_TIME = DISK_CODES;
    public final static int HIGH_BUSY_TIME    = DISK_CODES + 1;
    public final static int DISK_IMBALANCE    = DISK_CODES + 2;

    // these are values available to the world
    public Color[]     diskStates;
    public String[]    diskNames;

    // these values must be seeded by the live rule
    private CmOSFlavor osFlavor;        // what OS are we on

    // so must these; in addition, they are available to the world
    public double[]   busyTime;        // % utilization per disk
    public double[]   serviceTime;     // service time per disk

    private Threshold<Double> busyWarningThr;
    private Threshold<Double> busyProblemThr;
    private Threshold<Double> serviceWarningThr;
    private Threshold<Double> serviceProblemThr;

    private double busyWarning;
    private double busyProblem;
    private double serviceWarning;
    private double serviceProblem;

    // stuff
    private Formatter     fmt;
    private StringBuilder fmtStr;

    public PureDiskRule() {
        ruleName = RuleMessages.msgs.getString("Disk_Rule_Name");
        shortRuleName = RuleMessages.msgs.getString("Short_Disk");

        busyWarningThr =
            new Threshold<Double>(
                "DISK_BUSY_WARNING",   5.0, "%",  4, 1);
        busyProblemThr =
            new Threshold<Double>(
                "DISK_BUSY_PROBLEM",  20.0, "%",  4, 1);
        serviceWarningThr =
            new Threshold<Double>(
                "DISK_SERVICE_WARNING", 30.0, "ms", 4, 1);
        serviceProblemThr =
            new Threshold<Double>(
                "DISK_SERVICE_PROBLEM", 50.0, "ms", 4, 1);

        // signify to seedValues that the values need initializing
        busyWarning = -1;

        // no sub-rules here
        triggerRuleName = ruleName;
        fmtStr = new StringBuilder();
        fmt = new Formatter(fmtStr);
    }

    public void seedValues(PureDiskSeedValues sv) {
        osFlavor    = sv.osFlavor;
        busyTime    = sv.busyTime;
        serviceTime = sv.serviceTime;
        diskNames   = sv.diskNames;

        if (busyWarning == -1) {
            // initialize the constants
            busyWarning    = getThreshDouble(busyWarningThr, osFlavor);
            busyProblem    = getThreshDouble(busyProblemThr, osFlavor);
            serviceWarning = getThreshDouble(serviceWarningThr, osFlavor);
            serviceProblem = getThreshDouble(serviceProblemThr, osFlavor);

            // invoke the "if" in the next statement
            diskStates = null;
        }

        if (diskStates == null || diskStates.length != busyTime.length)
            diskStates = new Color[busyTime.length];

        // MacOS contains disk metrics, but they're all zero
        ruleSupported = (osFlavor != CmOSFlavor.MOS_MACOS);
    }

    public void firePureRule() {
        Color busiest = Color.WHITE;
        Color quietest = Color.RED;
        int busyIndex = 0;
        int quietIndex = 0;
        String busyFmt;
        String svcFmt;

        // reset this
        problemCode = NO_PROBLEM_DETECTED;

        for(int i=0; i<busyTime.length; i++) {
            if (busyTime[i] < busyWarning) {
                diskStates[i] = Color.WHITE;
                quietest = Color.WHITE;
                quietIndex = i;
            } else if (serviceTime[i] < serviceWarning) {
                diskStates[i] = Color.GREEN;
                // if the quietest state is worse than ours
                if (isWorseThan(quietest, Color.GREEN))
                    quietIndex = i;
                quietest = bestState(quietest, Color.GREEN);
                // if our state is worse than the busiest
                if (isWorseThan(Color.GREEN, busiest))
                    busyIndex = i;
                busiest = worstState(busiest, Color.GREEN);
            } else if (busyTime[i] > busyProblem) {
                if (serviceTime[i] > serviceWarning &&
                    serviceTime[i] < serviceProblem) {
                    diskStates[i] = Color.ORANGE;
                    // if the quietest state is worse than ours
                    if (isWorseThan(quietest, Color.ORANGE))
                        quietIndex = i;
                    quietest = bestState(quietest, Color.ORANGE);
                    // if our state is worse than the busiest
                    if (isWorseThan(Color.ORANGE, busiest))
                        busyIndex = i;
                    busiest = worstState(busiest, Color.ORANGE);
                } else if (serviceTime[i] > serviceProblem) {
                    diskStates[i] = Color.RED;
                    busiest = worstState(busiest, Color.RED);
                    // if our state is worse than the busiest
                    if (isWorseThan(Color.RED, busiest))
                        busyIndex = i;
                }
                problemCode = HIGH_SERVICE_TIME;
            } else { // not busy enough to be a problem
                diskStates[i] = Color.YELLOW;
                // if the quietest state is worse than ours
                if (isWorseThan(quietest, Color.YELLOW))
                    quietIndex = i;
                quietest = bestState(quietest, Color.YELLOW);
                // if our state is worse than the busiest
                if (isWorseThan(Color.YELLOW, busiest))
                    busyIndex = i;
                busiest = worstState(busiest, Color.YELLOW);
                problemCode = HIGH_BUSY_TIME;
            }
        }

        if (busyTime.length > 1) {
            fmt.format("%6.2f", busyTime[quietIndex]);
            busyFmt = fmtStr.toString();
            fmtStr.setLength(0);

            fmt.format("%6.2f", serviceTime[quietIndex]);
            svcFmt = fmtStr.toString();
            fmtStr.setLength(0);

            furtherDetail = RuleMessages.msgs.getString("Quietest")
                + busyFmt + " % / " + svcFmt + " ms" + "; ";

            fmt.format("%6.2f", busyTime[busyIndex]);
            busyFmt = fmtStr.toString();
            fmtStr.setLength(0);

            fmt.format("%6.2f", serviceTime[busyIndex]);
            svcFmt = fmtStr.toString();
            fmtStr.setLength(0);

            furtherDetail += RuleMessages.msgs.getString("Busiest")
                + busyFmt + " % / " + svcFmt + " ms";
        } else {
            fmt.format("%6.2f", busyTime[0]);
            busyFmt = fmtStr.toString();
            fmtStr.setLength(0);

            fmt.format("%6.2f", serviceTime[0]);
            svcFmt = fmtStr.toString();
            fmtStr.setLength(0);

            furtherDetail = RuleMessages.msgs.getString("Lone_Disk")
                + busyFmt + " % / " + svcFmt + " ms";
        }

        state = busiest;

        if (busyTime.length > 1 &&
            colorValue(busiest)  >= colorValue(Color.ORANGE) &&
            colorValue(quietest) <= colorValue(Color.GREEN)) {

            action = RuleMessages.msgs.getString("Disk_Imb");
            explanation = RuleMessages.msgs.getString("Hot_Cold_Disk");
            solution = RuleMessages.msgs.getString("Move_Load_Disk");
            problemCode = DISK_IMBALANCE;

            // convert white disks to blue
            for(int i=0; i<diskStates.length; i++)
                if (diskStates[i] == Color.WHITE)
                    diskStates[i] = Color.BLUE;

        } else {
            // set the action
            if (state.equals(Color.WHITE)) {
                action = RuleMessages.msgs.getString("No_Act");
                explanation = RuleMessages.msgs.getString("Disk_Idle");
                furtherDetail = RuleMessages.msgs.getString("All_Disk_Idle");
                solution = RuleMessages.msgs.getString("None");
            } else if (state.equals(Color.GREEN)) {
                action = RuleMessages.msgs.getString("Mild_Act");
                explanation = RuleMessages.msgs.getString("Disk_Nom");
                solution = RuleMessages.msgs.getString("None");
            } else if (state.equals(Color.ORANGE)) {
                action = RuleMessages.msgs.getString("Disk_Peak");
                explanation = RuleMessages.msgs.getString("No_More_Load");
                solution = RuleMessages.msgs.getString("Exam_HighWait");
            } else if (state.equals(Color.RED)) {
                action = RuleMessages.msgs.getString("Disk_Overload");
                explanation = RuleMessages.msgs.getString("Reduce_Load");
                solution = RuleMessages.msgs.getString("Exam_HighWait");
            }
        }
    }

    public void print(PrintStream out) {
        out.println(formatDouble(busyWarningThr, osFlavor));
        out.println(formatDouble(busyProblemThr, osFlavor));
        out.println(formatDouble(serviceWarningThr, osFlavor));
        out.println(formatDouble(serviceProblemThr, osFlavor));
    }
}
