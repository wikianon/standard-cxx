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

public abstract class PureNICRule extends AggregateRule<PureNICSeedValues> {

    public final static int NETIF_IMBALANCE       = NET_CODES;
    public final static int INTERFACE_ERRORS      = NET_CODES + 1;
    public final static int INTERFACE_FAILURES    = NET_CODES + 2;
    public final static int INTERFACE_COLLISIONS  = NET_CODES + 3;
    public final static int INTERFACE_UTILIZATION = NET_CODES + 4;
    public final static int INTERFACE_IMBALANCE   = NET_CODES + 5;

    // seeded by the pure rule
    private CmOSFlavor osFlavor;

    // seeded by the pure rule, but available to the world
    public Color[]  ifStates;
    public String[] ifNames;
    public double[] errors;
    public double[] failures;
    public int[]    colls;
    public int[]    utils;

    public void seedValues(PureNICSeedValues sv) {
        osFlavor = sv.osFlavor;
        ifNames  = sv.ifNames;
        errors   = sv.errors;
        failures = sv.failures;
        colls    = sv.colls;
        utils    = sv.utils;

        if (ifStates == null || ifStates.length != ifNames.length) {
            // allocate and initialized to virgin
            ifStates = new Color[ifNames.length];
            for(int i=0; i<ifStates.length; i++)
                ifStates[i] = Color.WHITE;
        }
        super.seedValues(sv);
    }

    public void firePureRule() {
        // allow the interface states to be innocent until proven guilty
        for(int i=0; i<ifStates.length; i++)
            ifStates[i] = Color.WHITE;

        super.firePureRule();
    }

    private class NICErrorsRule extends Rule<PureNICSeedValues> {
        private Threshold<Double>  netErrorProblemThr;
        private double             netErrorProblem;

        public NICErrorsRule() {
            ruleName = RuleMessages.msgs.getString("NIC_Error_Name");
            shortRuleName = RuleMessages.msgs.getString("Short_NIN");

            netErrorProblemThr =
                new Threshold<Double>("NIC_ERROR_PROBLEM", 0.5, "/s", 3, 2);

            netErrorProblem = -1;

            ruleSupported = true;
        }

        public void seedValues(PureNICSeedValues sv) {
            if (netErrorProblem == -1)
                netErrorProblem = getThreshDouble(netErrorProblemThr, osFlavor);
        }

        public void firePureRule() {
            double peakErrors = 0.0;
            String peakIf = null;

            state = Color.WHITE;

            // reset this
            problemCode = NO_PROBLEM_DETECTED;

            for(int i=0; i<errors.length; i++) {
                if (errors[i] > peakErrors) {
                    peakErrors = errors[i];
                    peakIf = ifNames[i];
                }

                if (errors[i] == 0)
                    ifStates[i] = worstState(ifStates[i], Color.WHITE);
                else if (errors[i] < netErrorProblem) {
                    ifStates[i] = worstState(ifStates[i], Color.YELLOW);
                    state = worstState(state, Color.YELLOW);
                } else {
                    ifStates[i] = worstState(ifStates[i], Color.RED);
                    state = worstState(state, Color.RED);
                }
            }

            if (state.equals(Color.WHITE)) {
                action = RuleMessages.msgs.getString("No_Errors");
                explanation = RuleMessages.msgs.getString("Error_Free");
                furtherDetail = RuleMessages.msgs.getString("NIC_Nom");
                solution = RuleMessages.msgs.getString("None");
            } else if (state.equals(Color.YELLOW)) {
                action = RuleMessages.msgs.getString("Few_Errors");
                explanation = RuleMessages.msgs.getString("Low_Errors");
                furtherDetail = RuleMessages.msgs.getString("Error_Rate") +
                  peakErrors + "/sec: " + peakIf;
                solution = RuleMessages.msgs.getString("Vigilance");
                problemCode = INTERFACE_ERRORS;
            } else if (state.equals(Color.RED)) {
                action = RuleMessages.msgs.getString("High_Errors");
                explanation = RuleMessages.msgs.getString("High_Error_Rate");
                furtherDetail = RuleMessages.msgs.getString("Error_Rate") +
                  peakErrors + "/sec: " + peakIf;
                solution = RuleMessages.msgs.getString("Check_NIC");
                problemCode = INTERFACE_ERRORS;
            }
        }

        public void print(PrintStream out) {
            out.println(formatDouble(netErrorProblemThr, osFlavor));
        }

        // stub
        public void fireLiveRule() throws
            NotBoundException, MalformedURLException,
                RemoteException, PerformanceException {
        }
    }

    private class NICFailuresRule extends Rule<PureNICSeedValues> {
        private Threshold<Double>  netFailureProblemThr;
        private double             netFailureProblem;

        public NICFailuresRule() {
            ruleName = RuleMessages.msgs.getString("NIC_Failure_Name");
            shortRuleName = RuleMessages.msgs.getString("Short_NFN");

            netFailureProblemThr =
                new Threshold<Double>("NIC_FAILURE_PROBLEM", 0.1, "/s", 3, 2);

            netFailureProblem = -1;

            ruleSupported = true;
        }

        public void seedValues(PureNICSeedValues sv) {
            if (netFailureProblem == -1)
                netFailureProblem =
                    getThreshDouble(netFailureProblemThr, osFlavor);
        }

        public void firePureRule() {
            double peakFailures = 0.0;
            String peakIf = null;

            state = Color.WHITE;

            // reset this
            problemCode = NO_PROBLEM_DETECTED;

            for(int i=0; i<failures.length; i++) {
                if (failures[i] > peakFailures) {
                    peakFailures = failures[i];
                    peakIf = ifNames[i];
                }

                if (failures[i] == 0)
                    ifStates[i] = worstState(ifStates[i], Color.WHITE);
                else if (failures[i] < netFailureProblem) {
                    ifStates[i] = worstState(ifStates[i], Color.ORANGE);
                    state = worstState(state, Color.ORANGE);
                } else {
                    ifStates[i] = worstState(ifStates[i], Color.RED);
                    state = worstState(state, Color.RED);
                }
            }

            if (state.equals(Color.WHITE)) {
                action = RuleMessages.msgs.getString("No_Failures");
                explanation = RuleMessages.msgs.getString("Failure_Free");
                furtherDetail = RuleMessages.msgs.getString("NIC_Nom");
                solution = RuleMessages.msgs.getString("None");
            } else if (state.equals(Color.ORANGE)) {
                action = RuleMessages.msgs.getString("Few_Failures");
                explanation = RuleMessages.msgs.getString("Low_Failures");
                furtherDetail = RuleMessages.msgs.getString("Failure_Rate") +
                  peakFailures + "/sec: " + peakIf;
                solution = RuleMessages.msgs.getString("Vigilance");
                problemCode = INTERFACE_FAILURES;
            } else if (state.equals(Color.RED)) {
                action = RuleMessages.msgs.getString("High_Failures");
                explanation = RuleMessages.msgs.getString("High_Failure_Rate");
                furtherDetail = RuleMessages.msgs.getString("Failure_Rate") +
                  peakFailures + "/sec: " + peakIf;
                solution = RuleMessages.msgs.getString("Check_NIC");
                problemCode = INTERFACE_FAILURES;
            }
        }

        public void print(PrintStream out) {
            out.println(formatDouble(netFailureProblemThr, osFlavor));
        }

        // stub
        public void fireLiveRule() throws
            NotBoundException, MalformedURLException,
                RemoteException, PerformanceException {
        }
    }

    private class NICCollisionsRule extends Rule<PureNICSeedValues> {
        private Threshold<Integer> netCollisionProblemThr;
        private int                netCollisionProblem;

        public NICCollisionsRule() {
            ruleName = RuleMessages.msgs.getString("NIC_Collision_Name");
            shortRuleName = RuleMessages.msgs.getString("Short_NCN");

            netCollisionProblemThr =
                new Threshold<Integer>("NIC_COLLISION_PROBLEM", 25, "%", 3);

            netCollisionProblem = -1;
        }

        public void seedValues(PureNICSeedValues sv) {
            if (!sv.hasCollisionPercent) {
                ruleSupported = false;
                return;
            }

            ruleSupported = true;

            if (netCollisionProblem == -1)
                netCollisionProblem =
                    getThreshInteger(netCollisionProblemThr, osFlavor);
        }

        public void firePureRule() {
            double peakCollisions = 0.0;
            String peakIf = null;

            state = Color.WHITE;

            // reset this
            problemCode = NO_PROBLEM_DETECTED;

            for(int i=0; i<colls.length; i++) {
                if (colls[i] > peakCollisions) {
                    peakCollisions = colls[i];
                    peakIf = ifNames[i];
                }

                if (colls[i] == 0)
                    ifStates[i] = worstState(ifStates[i], Color.WHITE);
                else if (colls[i] < netCollisionProblem) {
                    ifStates[i] = worstState(ifStates[i], Color.YELLOW);
                    state = worstState(state, Color.YELLOW);
                } else {
                    ifStates[i] = worstState(ifStates[i], Color.RED);
                    state = worstState(state, Color.RED);
                }
            }

            if (state.equals(Color.WHITE)) {
                action = RuleMessages.msgs.getString("No_Collisions");
                explanation = RuleMessages.msgs.getString("Collision_Free");
                furtherDetail = RuleMessages.msgs.getString("NIC_Nom");
                solution = RuleMessages.msgs.getString("None");
            } else if (state.equals(Color.YELLOW)) {
                action = RuleMessages.msgs.getString("Few_Collisions");
                explanation = RuleMessages.msgs.getString("Low_Collisions");
                furtherDetail = RuleMessages.msgs.getString("Collision_Rate") +
                  peakCollisions + "/sec: " + peakIf;
                solution = RuleMessages.msgs.getString("Vigilance");
                problemCode = INTERFACE_COLLISIONS;
            } else if (state.equals(Color.RED)) {
                action = RuleMessages.msgs.getString("High_Collisions");
                explanation = RuleMessages.msgs.getString("High_Collision_Rate");
                furtherDetail = RuleMessages.msgs.getString("Collision_Rate") +
                  peakCollisions + "/sec: " + peakIf;
                solution = RuleMessages.msgs.getString("Check_NIC");
                problemCode = INTERFACE_COLLISIONS;
            }
        }

        public void print(PrintStream out) {
            out.println(formatInteger(netCollisionProblemThr, osFlavor));
        }

        // stub
        public void fireLiveRule() throws
            NotBoundException, MalformedURLException,
                RemoteException, PerformanceException {
        }
    }

    private class NICUtilizationRule extends Rule<PureNICSeedValues> {
        // these values must be seeded by the live rule
        private long currentTime;

        private Threshold<Integer> netUtilizationWarningThr;
        private Threshold<Integer> netUtilizationProblemThr;
        private Threshold<Integer> netBandwidthHogProblemThr;
        private int                netUtilizationWarning;
        private int                netUtilizationProblem;
        private int                netBandwidthHogProblem;
        private long               spikeBegin;

        public NICUtilizationRule() {
            ruleName = RuleMessages.msgs.getString("NIC_Utilization_Name");
            shortRuleName = RuleMessages.msgs.getString("Short_NUN");

            netUtilizationWarningThr =
                new Threshold<Integer>("NIC_UTILIZATION_WARNING", 25, "%", 3);
            netUtilizationProblemThr =
                new Threshold<Integer>("NIC_UTILIZATION_PROBLEM", 75, "%", 3);
            netBandwidthHogProblemThr =
                new Threshold<Integer>("NIC_BANDWIDTH_HOG_PROBLEM", 15, "secs", 3);

            netUtilizationProblem = -1;
            spikeBegin = 0;
        }

        public void seedValues(PureNICSeedValues sv) {
            currentTime   = sv.currentTime;
            ruleSupported = sv.hasUtilizationPercent;

            if (!ruleSupported)
                return;

            if (netUtilizationProblem == -1) {
                netUtilizationWarning =
                    getThreshInteger(netUtilizationWarningThr, osFlavor);
                netUtilizationProblem =
                    getThreshInteger(netUtilizationProblemThr, osFlavor);
                netBandwidthHogProblem =
                    getThreshInteger(netBandwidthHogProblemThr, osFlavor);
            }
        }

        public void firePureRule() {
            double peakUtilization = 0.0;
            long spikeDuration = 0;
            boolean bwHog = false;
            String peakIf = null;

            state = Color.WHITE;

            // reset this
            problemCode = NO_PROBLEM_DETECTED;

            for(int i=0; i<utils.length; i++) {
                if (utils[i] > peakUtilization) {
                    peakUtilization = utils[i];
                    peakIf = ifNames[i];
                }

                if (utils[i] == 0) {
                    ifStates[i] = worstState(ifStates[i], Color.WHITE);
                    spikeBegin = 0;
                } else if (utils[i] < netUtilizationWarning) {
                    ifStates[i] = worstState(ifStates[i], Color.GREEN);
                    state = worstState(state, Color.GREEN);
                } else if (utils[i] < netUtilizationProblem) {
                    ifStates[i] = worstState(ifStates[i], Color.YELLOW);
                    state = worstState(state, Color.YELLOW);
                } else {
                    ifStates[i] = worstState(ifStates[i], Color.RED);
                    state = worstState(state, Color.RED);
                }
            }

            // flag extended lapses of high utilization
            if (isWorseThan(state, Color.GREEN)) {
                if (spikeBegin != 0) {
                    spikeDuration = currentTime - spikeBegin;
                    spikeDuration /= 1000; // ms -> sec
                    if (spikeDuration >= netBandwidthHogProblem)
                        bwHog = true;
                } else
                    spikeBegin = currentTime;
            }

            if (state.equals(Color.WHITE) || state.equals(Color.GREEN)) {
                action = RuleMessages.msgs.getString("Minor_Traffic");
                explanation = RuleMessages.msgs.getString("Not_Enough_Traffic");
                furtherDetail = RuleMessages.msgs.getString("NIC_Nom");
                solution = RuleMessages.msgs.getString("None");
            } else if (state.equals(Color.YELLOW)) {
                action = RuleMessages.msgs.getString("Some_Traffic");
                if (bwHog) {
                    state = Color.ORANGE;
                    explanation = RuleMessages.msgs.getString("BW_Hog") +
                      spikeDuration + " secs";
                } else
                    explanation = RuleMessages.msgs.getString("Low_Traffic");
                furtherDetail = RuleMessages.msgs.getString("Util_Rate") +
                  peakUtilization + "%: " + peakIf;
                solution = RuleMessages.msgs.getString("Vigilance");
                problemCode = INTERFACE_UTILIZATION;
            } else if (state.equals(Color.RED)) {
                action = RuleMessages.msgs.getString("High_Traffic");
                if (bwHog)
                    explanation = RuleMessages.msgs.getString("BW_Hog") +
                      spikeDuration + " secs";
                else
                    explanation = RuleMessages.msgs.getString("High_Traffic_Rate");
                furtherDetail = RuleMessages.msgs.getString("Util_Rate") +
                  peakUtilization + "%: " + peakIf;
                solution = RuleMessages.msgs.getString("Redist_Traffic");
                problemCode = INTERFACE_UTILIZATION;
            }
        }

        public void print(PrintStream out) {
            out.println(formatInteger(netUtilizationWarningThr, osFlavor));
            out.println(formatInteger(netUtilizationProblemThr, osFlavor));
            out.println(formatInteger(netBandwidthHogProblemThr, osFlavor));
        }

        // stub
        public void fireLiveRule() throws
            NotBoundException, MalformedURLException,
                RemoteException, PerformanceException {
        }
    }

    private class NICImbalanceRule extends Rule<PureNICSeedValues> {
        private Threshold<Boolean> useRuleThr;
        private Threshold<Integer> imbalanceProblemThr;
        private boolean useRule;
        private int imbalanceProblem;

        public NICImbalanceRule() {
            ruleName = RuleMessages.msgs.getString("NIC_Imbal_Name");
            shortRuleName = RuleMessages.msgs.getString("Short_NImN");

            imbalanceProblemThr = new Threshold<Integer>(
                "NIC_IMBALANCE_PROBLEM", 25, "%", 3);
            useRuleThr = new Threshold<Boolean>(
                "USE_NIC_IMBALANCE_RULE", false);

            // signify to seedValues that the values need initializing
            imbalanceProblem = -1;
        }

        public void seedValues(PureNICSeedValues sv) {
            if (imbalanceProblem == -1) {
                imbalanceProblem = getThreshInteger(imbalanceProblemThr, osFlavor);
                useRule = getThreshBoolean(useRuleThr, osFlavor);
            }

            ruleSupported = sv.hasUtilizationPercent &&
                                (utils.length > 1) && useRule;

            if (!ruleSupported)
                return;
        }

        public void firePureRule() {
            int highUtil;
            int lowUtil;
            String lowIf = null;
            String highIf = null;
    
            // reset this
            problemCode = NO_PROBLEM_DETECTED;

            // check for interface utilization imbalance
            highUtil = 0;
            lowUtil = 100;

            for(int i=0; i<utils.length; i++) {
                if (utils[i] > highUtil) {
                  highUtil = utils[i];
                  highIf = ifNames[i];
                }
                if (utils[i] < lowUtil) {
                  lowUtil = utils[i];
                  lowIf = ifNames[i];
                }
            }

            if ((highUtil - lowUtil) >= imbalanceProblem) {
                state = Color.YELLOW;
                action = RuleMessages.msgs.getString("NIC_Imbalance");
                explanation = RuleMessages.msgs.getString("NIC_Imb_Problem");
                furtherDetail = lowIf + ": " + lowUtil + "%; " +
                                highIf + ": " + highUtil + "%";
                solution = RuleMessages.msgs.getString("Exam_NIC_Imb");
                problemCode = INTERFACE_IMBALANCE;
                return;
            }

            state = Color.GREEN;
            action = RuleMessages.msgs.getString("Mild_Act");
            explanation = RuleMessages.msgs.getString("NIC_Bal_Nom");
            furtherDetail = RuleMessages.msgs.getString("NIC_Nom");
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

    public PureNICRule() {
        ruleName = RuleMessages.msgs.getString("NIC_Name");
        shortRuleName = RuleMessages.msgs.getString("Short_NIC");

        addRule(new NICErrorsRule());
        addRule(new NICFailuresRule());
        addRule(new NICCollisionsRule());
        addRule(new NICUtilizationRule());
        addRule(new NICImbalanceRule());
        // add new nic sub-rules here
    }
}
