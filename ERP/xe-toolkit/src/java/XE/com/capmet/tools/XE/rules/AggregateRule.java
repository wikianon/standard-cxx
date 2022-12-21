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

import java.io.PrintStream;
import java.awt.Color;
import java.util.Vector;
import com.capmet.metrics.CmString;

public abstract class AggregateRule<SeedValues> extends Rule<SeedValues> {
    protected Vector<Rule<SeedValues>> rules;
    protected int                      supportedSubruleCount;

    public AggregateRule() {
        rules = new Vector<Rule<SeedValues>>();
        aggregateRule = true;
    }

    public void addRule(Rule<SeedValues> subrule) {
        rules.addElement(subrule);
    }

    public void firePureRule() {
        int n = rules.size();

        if (n == 0)
            return;

        // reset this
        problemCode = NO_PROBLEM_DETECTED;

        // by setting state to a non-state color, colorValue will return -1
        state = Color.PINK;
        for(int i=0; i<n; i++) {
            Rule<SeedValues> r = rules.elementAt(i);

            if (!r.ruleSupported)
                continue;

            r.firePureRule();

            String act = r.action;
            String exp = r.explanation;
            String fd  = r.furtherDetail;
            String sol = r.solution;
            String trn = r.ruleName;
            int    pc  = r.problemCode;
            Color  st  = r.state;

            if (isWorseThan(st, state)) {
                state = st;
                action = act;
                explanation = exp;
                furtherDetail = fd;
                solution = sol;
                problemCode = pc;
                triggerRuleName = trn;
            }
        }
    }

    public void print(PrintStream out) {
        int n = rules.size();

        // no subrules, andiamo
        if (n == 0)
            return;

        for(int i=0; i<n; i++)
            rules.elementAt(i).print(out);
    }

    public void seedValues(SeedValues sv) {
        int n = rules.size();

        supportedSubruleCount = 0;

        for(int i=0; i<n; i++) {
            Rule<SeedValues> r = rules.elementAt(i);

            r.seedValues(sv);

            if (r.ruleSupported)
                supportedSubruleCount++;
        }

        ruleSupported = (supportedSubruleCount != 0);
    }

    public int getSupportedSubruleCount() {
        return supportedSubruleCount;
    }

    public Vector<Rule<SeedValues>> getSubRules() {
        return rules;
    }
}
