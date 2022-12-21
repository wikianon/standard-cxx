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

public abstract class PureTCPRule extends AggregateRule<PureTCPSeedValues> {

    public final static int HIGH_SEGMENT_RETRANSMISSION = TCP_CODES;
    public final static int HIGH_ATTEMPT_FAILURES       = TCP_CODES + 1;
    public final static int HIGH_INPUT_ERRORS           = TCP_CODES + 2;
    public final static int HIGH_RST_SEGMENT_RATE       = TCP_CODES + 3;

    // threshold used by more than one rule
    private CmOSFlavor         osFlavor;
    private Threshold<Integer> coverChargeThr;
    private int                coverCharge;

    public void seedValues(PureTCPSeedValues sv) {
        osFlavor = sv.osFlavor;

        if (coverCharge == -1)
            coverCharge = getThreshInteger(coverChargeThr, osFlavor);

        super.seedValues(sv);
    }

    public void print(PrintStream out) {
        out.println(formatInteger(coverChargeThr, osFlavor));
        super.print(out);
    }

    private class SegmentRetransmissionRule extends Rule<PureTCPSeedValues> {
        // these values must be seeded by the live rule
        private int outputSegs;
        private int retransPercent;

        private Threshold<Integer> retransWarningThr;
        private Threshold<Integer> retransProblemThr;

        // these values are constants, the rest computed
        private int  retransWarning;
        private int  retransProblem;

        public SegmentRetransmissionRule() {
            ruleName = RuleMessages.msgs.getString("TCP_Retrans");
            shortRuleName = RuleMessages.msgs.getString("Short_TR");

            retransWarningThr =
                new Threshold<Integer>(
                    "TCP_RETRANS_WARNING", 10, "%", 2);
            retransProblemThr =
                new Threshold<Integer>(
                    "TCP_RETRANS_PROBLEM", 20, "%", 2);

            // signify to seedValues that the values need initializing
            retransWarning = -1;

            // all current platforms collect TCP metrics
            ruleSupported = true;
        }

        public void seedValues(PureTCPSeedValues sv) {
            outputSegs     = sv.outputSegs;
            retransPercent = sv.retransPercent;

            if (retransWarning == -1) {
                // initialize the constants
                retransWarning = getThreshInteger(retransWarningThr, osFlavor);
                retransProblem = getThreshInteger(retransProblemThr, osFlavor);
            }
        }

        public void firePureRule() {

            // reset this
            problemCode = NO_PROBLEM_DETECTED;

            // only apply the rule if enough segments have transpired
            if (outputSegs < coverCharge || retransPercent == 0) {
                state = Color.WHITE;
                action = RuleMessages.msgs.getString("No_Act");
                explanation = RuleMessages.msgs.getString("Cold_Stack");
                furtherDetail = RuleMessages.msgs.getString("Out_Segs") +
                    outputSegs;
                solution = RuleMessages.msgs.getString("None");
                return;
            }

            furtherDetail = RuleMessages.msgs.getString("Retrans_Pct") +
                retransPercent + "%";

            problemCode = HIGH_SEGMENT_RETRANSMISSION;

            if (retransPercent < retransWarning) {
                state = Color.YELLOW;
                action = RuleMessages.msgs.getString("Some_Retrans");
                explanation = RuleMessages.msgs.getString("Mild_Retrans");
                solution = RuleMessages.msgs.getString("Attention");
            } else if (retransPercent < retransProblem) {
                state = Color.ORANGE;
                action = RuleMessages.msgs.getString("Warning_Retrans");
                explanation = RuleMessages.msgs.getString("Bad_Retrans");
                solution = RuleMessages.msgs.getString("Check_Config");
            } else {
                state = Color.RED;
                action = RuleMessages.msgs.getString("Problem_Retrans");
                explanation = RuleMessages.msgs.getString("Worse_Retrans");
                solution = RuleMessages.msgs.getString("Fix_Network");
            }
        }

        public void print(PrintStream out) {
            out.println(formatInteger(coverChargeThr, osFlavor));
            out.println(formatInteger(retransWarningThr, osFlavor));
            out.println(formatInteger(retransProblemThr, osFlavor));
        }

        // stub
        public void fireLiveRule() throws
            NotBoundException, MalformedURLException,
                RemoteException, PerformanceException {
        }
    }

    private class ConnectionFailureRule extends Rule<PureTCPSeedValues> {
        // these values must be seeded by the live rule
        private int attemptFails;  // connection failures per second

        private Threshold<Integer> failureWarningThr;
        private Threshold<Integer> failureProblemThr;

        private int failureWarning;
        private int failureProblem;

        public ConnectionFailureRule() {
            ruleName = RuleMessages.msgs.getString("TCP_ConnFail");
            shortRuleName = RuleMessages.msgs.getString("Short_TCF");

            failureWarningThr = new Threshold<Integer>(
                "TCP_CONNECT_FAILURE_WARNING", 5, "/sec", 3);
            failureProblemThr = new Threshold<Integer>(
                "TCP_CONNECT_FAILURE_PROBLEM", 10, "/sec", 3);

            // signify to seedValues that the values need initializing
            failureWarning = -1;
        }

        public void seedValues(PureTCPSeedValues sv) {
            attemptFails = sv.attemptFails;

            if (failureWarning == -1) {
                failureWarning = getThreshInteger(failureWarningThr, osFlavor);
                failureProblem = getThreshInteger(failureProblemThr, osFlavor);
            }

            // all platforms for now
            ruleSupported = true;
        }

        public void firePureRule() {
            // reset this
            problemCode = NO_PROBLEM_DETECTED;

            if (attemptFails == 0) {
                state = Color.WHITE;
                action = RuleMessages.msgs.getString("No_Act");
                explanation = RuleMessages.msgs.getString("Cold_Stack");
                furtherDetail = RuleMessages.msgs.getString("No_AttFail");
                solution = RuleMessages.msgs.getString("None");
                return;
            }

            furtherDetail = RuleMessages.msgs.getString("Attempt_Fails") +
                attemptFails + "/sec";

            problemCode = HIGH_ATTEMPT_FAILURES;

            if (attemptFails < failureWarning) {
                state = Color.YELLOW;
                action = RuleMessages.msgs.getString("Some_Fails");
                explanation = RuleMessages.msgs.getString("Mild_Fails");
                solution = RuleMessages.msgs.getString("Attention");
            } else if (attemptFails < failureProblem) {
                state = Color.ORANGE;
                action = RuleMessages.msgs.getString("Warning_Fails");
                explanation = RuleMessages.msgs.getString("Bad_Fails");
                solution = RuleMessages.msgs.getString("Bad_Services");
            } else {
                state = Color.RED;
                action = RuleMessages.msgs.getString("Problem_Fails");
                explanation = RuleMessages.msgs.getString("Worse_Fails");
                solution = RuleMessages.msgs.getString("Check_SYN_DOS");
            }
        }

        public void print(PrintStream out) {
            out.println(formatInteger(failureWarningThr, osFlavor));
            out.println(formatInteger(failureProblemThr, osFlavor));
        }

        // stub
        public void fireLiveRule() throws
            NotBoundException, MalformedURLException,
                RemoteException, PerformanceException {
        }
    }

    private class InputErrorsRule extends Rule<PureTCPSeedValues> {
        // these values must be seeded by the live rule
        private int inputSegs;
        private int inputErrorsPercent;

        private Threshold<Integer> errorWarningThr;
        private Threshold<Integer> errorProblemThr;

        private int errorWarning;
        private int errorProblem;

        public InputErrorsRule() {
            ruleName = RuleMessages.msgs.getString("TCP_InErrs");
            shortRuleName = RuleMessages.msgs.getString("Short_TIE");

            errorWarningThr = new Threshold<Integer>(
                "TCP_INPUT_ERROR_WARNING", 3, "%", 2);
            errorProblemThr = new Threshold<Integer>(
                "TCP_INPUT_ERROR_PROBLEM", 8, "%", 2);

            // signify to seedValues that the values need initializing
            errorWarning = -1;
        }

        public void seedValues(PureTCPSeedValues sv) {
            inputSegs          = sv.inputSegs;
            inputErrorsPercent = sv.inputErrorsPercent;

            if (errorWarning == -1) {
                errorWarning = getThreshInteger(errorWarningThr, osFlavor);
                errorProblem = getThreshInteger(errorProblemThr, osFlavor);
            }

            // all platforms for now
            ruleSupported = true;
        }

        public void firePureRule() {
            // reset this
            problemCode = NO_PROBLEM_DETECTED;

            // only apply the rule if enough segments have transpired
            if (inputSegs < coverCharge || inputErrorsPercent == 0) {
                state = Color.WHITE;
                action = RuleMessages.msgs.getString("No_Act");
                explanation = RuleMessages.msgs.getString("Cold_Stack");
                furtherDetail = RuleMessages.msgs.getString("In_Segs") +
                    inputSegs;
                solution = RuleMessages.msgs.getString("None");
                return;
            }

            furtherDetail = RuleMessages.msgs.getString("InErr_Pct") +
                inputErrorsPercent + "%";

            problemCode = HIGH_INPUT_ERRORS;

            if (inputErrorsPercent < errorWarning) {
                state = Color.YELLOW;
                action = RuleMessages.msgs.getString("Some_InErrs");
                explanation = RuleMessages.msgs.getString("Mild_InErrs");
                solution = RuleMessages.msgs.getString("Attention");
            } else if (inputErrorsPercent < errorProblem) {
                state = Color.ORANGE;
                action = RuleMessages.msgs.getString("Warning_InErrs");
                explanation = RuleMessages.msgs.getString("Bad_InErrs");
                solution = RuleMessages.msgs.getString("Fix_Network");
            } else {
                state = Color.RED;
                action = RuleMessages.msgs.getString("Problem_InErrs");
                explanation = RuleMessages.msgs.getString("Worse_InErrs");
                solution = RuleMessages.msgs.getString("Fix_Network");
            }
        }

        public void print(PrintStream out) {
            out.println(formatInteger(errorWarningThr, osFlavor));
            out.println(formatInteger(errorProblemThr, osFlavor));
        }

        // stub
        public void fireLiveRule() throws
            NotBoundException, MalformedURLException,
                RemoteException, PerformanceException {
        }
    }

    private class ResetAttackRule extends Rule<PureTCPSeedValues> {
        // these values must be seeded by the live rule
        private int outputSegs;
        private int rstPercent;

        private Threshold<Integer> rstWarningThr;
        private Threshold<Integer> rstProblemThr;

        private int rstWarning;
        private int rstProblem;

        public ResetAttackRule() {
            ruleName = RuleMessages.msgs.getString("TCP_RstRule");
            shortRuleName = RuleMessages.msgs.getString("Short_TRR");

            rstWarningThr = new Threshold<Integer>(
                "TCP_RST_WARNING", 3, "%", 2);
            rstProblemThr = new Threshold<Integer>(
                "TCP_RST_PROBLEM", 8, "%", 2);

            // signify to seedValues that the values need initializing
            rstWarning = -1;
        }

        public void seedValues(PureTCPSeedValues sv) {
            outputSegs = sv.outputSegs;
            rstPercent = sv.rstPercent;

            if (rstWarning == -1) {
                rstWarning = getThreshInteger(rstWarningThr, osFlavor);
                rstProblem = getThreshInteger(rstProblemThr, osFlavor);
            }

            // all platforms for now
            ruleSupported = true;
        }

        public void firePureRule() {
            // reset this
            problemCode = NO_PROBLEM_DETECTED;

            // only apply the rule if enough segments have transpired
            if (outputSegs < coverCharge || rstPercent == 0) {
                state = Color.WHITE;
                action = RuleMessages.msgs.getString("No_Act");
                explanation = RuleMessages.msgs.getString("Cold_Stack");
                furtherDetail = RuleMessages.msgs.getString("Out_Segs") +
                    outputSegs;
                solution = RuleMessages.msgs.getString("None");
                return;
            }

            furtherDetail = RuleMessages.msgs.getString("Rst_Pct") +
                rstPercent + "%";

            problemCode = HIGH_RST_SEGMENT_RATE;

            if (rstPercent < rstWarning) {
                state = Color.YELLOW;
                action = RuleMessages.msgs.getString("Some_Rst");
                explanation = RuleMessages.msgs.getString("Mild_Rst");
                solution = RuleMessages.msgs.getString("Attention");
            } else if (rstPercent < rstProblem) {
                state = Color.ORANGE;
                action = RuleMessages.msgs.getString("Warning_Rst");
                explanation = RuleMessages.msgs.getString("Bad_Rst");
                solution = RuleMessages.msgs.getString("Spooky_RST");
            } else {
                state = Color.RED;
                action = RuleMessages.msgs.getString("Problem_Rst");
                explanation = RuleMessages.msgs.getString("Worse_Rst");
                solution = RuleMessages.msgs.getString("Check_RST_DOS");
            }
        }

        public void print(PrintStream out) {
            out.println(formatInteger(rstWarningThr, osFlavor));
            out.println(formatInteger(rstProblemThr, osFlavor));
        }

        // stub
        public void fireLiveRule() throws
            NotBoundException, MalformedURLException,
                RemoteException, PerformanceException {
        }
    }

    public PureTCPRule() {
        ruleName = RuleMessages.msgs.getString("TCP_Name");
        shortRuleName = RuleMessages.msgs.getString("Short_TCP");

        coverChargeThr =
            new Threshold<Integer>(
                "TCP_MINIMUM_SEGMENTS", 1000, "/sec", 5);

        addRule(new SegmentRetransmissionRule());
        addRule(new ConnectionFailureRule());
        addRule(new InputErrorsRule());
        addRule(new ResetAttackRule());
        // add new TCP sub-rules here

        // indicate to seedValues that the value needs to be set
        coverCharge = -1;
    }
}
