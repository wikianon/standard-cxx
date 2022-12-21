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

public abstract class PurePageFileRule extends Rule<PurePageFileSeedValues> {

    public final static int PAGE_SPACE_LOW  = PAGE_FILE_CODES;
    public final static int PAGE_SPACE_GONE = PAGE_FILE_CODES + 1;

    // these values must be seeded by the live rule
    private CmOSFlavor osFlavor;
    private int        pageFileFreePercent;

    private Threshold<Integer> pfWarningThr;
    private Threshold<Integer> pfProblemThr;
    private Threshold<Integer> pfCriticalThr;

    private int pfWarning;
    private int pfProblem;
    private int pfCritical;

    public PurePageFileRule() {
        ruleName = RuleMessages.msgs.getString("PageFileRule_Name");
        shortRuleName = RuleMessages.msgs.getString("Short_PF");

        pfWarningThr =
            new Threshold<Integer>(
                "PAGE_FILE_WARNING",  50, "%", 3);
        pfProblemThr =
            new Threshold<Integer>(
                "PAGE_FILE_PROBLEM",  25, "%", 3);
        pfCriticalThr =
            new Threshold<Integer>(
                "PAGE_FILE_CRITICAL", 10, "%", 3);

        // cause seedValues to do its thing
        pfWarning = -1;

        // no sub-rules here
        triggerRuleName = ruleName;
    }

    public void seedValues(PurePageFileSeedValues sv) {
        osFlavor = sv.osFlavor;
        pageFileFreePercent = sv.pageFileFreePercent;

        if (pfWarning == -1) {
            pfWarning  = getThreshInteger(pfWarningThr,  osFlavor);
            pfProblem  = getThreshInteger(pfProblemThr,  osFlavor);
            pfCritical = getThreshInteger(pfCriticalThr, osFlavor);
        }

        ruleSupported = true;
    }

    public void firePureRule() {

        // reset this
        problemCode = NO_PROBLEM_DETECTED;

        if (pageFileFreePercent == 100) {
            state = Color.WHITE;
            action = RuleMessages.msgs.getString("Unused_PF");
            explanation = RuleMessages.msgs.getString("Lots_o_phys");
            furtherDetail = RuleMessages.msgs.getString("PF_Zero");
            solution = RuleMessages.msgs.getString("None");
            return;
        }
        if (pageFileFreePercent == 0) {
            problemCode = PAGE_SPACE_GONE;
            state = Color.BLACK;
            action = RuleMessages.msgs.getString("Gone_PF");
            explanation = RuleMessages.msgs.getString("No_PF");
            furtherDetail = RuleMessages.msgs.getString("PF_100");
            solution = RuleMessages.msgs.getString("Add_PF");
            return;
        }

        furtherDetail = RuleMessages.msgs.getString("PF_Free") +
                        pageFileFreePercent + "%";

        if (pageFileFreePercent > pfWarning) {
            state = Color.GREEN;
            action = RuleMessages.msgs.getString("Mild_Util");
            explanation = RuleMessages.msgs.getString("PF_Accept");
            solution = RuleMessages.msgs.getString("None");
            return;
        }

        problemCode = PAGE_SPACE_LOW;

        if (pageFileFreePercent > pfProblem) {
            state = Color.YELLOW;
            action = RuleMessages.msgs.getString("PF_Problem");
            explanation = RuleMessages.msgs.getString("App_Grow");
            solution = RuleMessages.msgs.getString("Stay_Attentive");
            return;
        }

        if (pageFileFreePercent > pfCritical) {
            state = Color.ORANGE;
            action = RuleMessages.msgs.getString("PF_Critical");
            explanation = RuleMessages.msgs.getString("App_Fail");
            solution = RuleMessages.msgs.getString("Add_Pre_Crit");
            return;
        }

        state = Color.RED;
        action = RuleMessages.msgs.getString("PF_2_High");
        explanation = RuleMessages.msgs.getString("App_Fail_Yes");
        solution = RuleMessages.msgs.getString("Add_Pre_Crit");
    }

    public void print(PrintStream out) {
        out.println(formatInteger(pfWarningThr,  osFlavor));
        out.println(formatInteger(pfProblemThr,  osFlavor));
        out.println(formatInteger(pfCriticalThr, osFlavor));
    }
}
