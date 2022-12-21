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

public abstract class PureCacheRule extends Rule<PureCacheSeedValues> {

    public final static int POOR_CACHE_UTILIZATION = CACHE_CODES;

    // these are values available to the world
    public Color[]     cacheStates;
    public String[]    cacheNames;

    // these values must be seeded by the live rule
    private CmOSFlavor osFlavor;        // what OS are we on
    private int[]      cacheUtils;
    private int[]      cacheSwings;

    private Threshold<Integer> minEntriesThr;
    private Threshold<Integer> yellowLevelThr;
    private Threshold<Integer> orangeLevelThr;
    private Threshold<Integer> redLevelThr;

    private int minEntries;
    private int yellowLevel;
    private int orangeLevel;
    private int redLevel;

    public PureCacheRule() {
        ruleName = RuleMessages.msgs.getString("Cache_Rule_Name");
        shortRuleName = RuleMessages.msgs.getString("Short_Cache");

        minEntriesThr =
            new Threshold<Integer>("CACHE_MINIMUM_ENTRIES", 1000, "lookups", 5);
        yellowLevelThr =
            new Threshold<Integer>("CACHE_YELLOW_LEVEL", 70, "%", 3);
        orangeLevelThr =
            new Threshold<Integer>("CACHE_ORANGE_LEVEL", 50, "%", 3);
        redLevelThr =
            new Threshold<Integer>("CACHE_RED_LEVEL", 25, "%", 3);

        yellowLevel = -1;

        // no sub-rules here
        triggerRuleName = ruleName;
    }

    public void seedValues(PureCacheSeedValues sv) {
        osFlavor    = sv.osFlavor;
        cacheNames  = sv.cacheNames;
        cacheSwings = sv.cacheSwings;
        cacheUtils  = sv.cacheUtils;

        if (yellowLevel == -1) {
            // initialize the constants
            minEntries  = getThreshInteger(minEntriesThr, osFlavor);
            yellowLevel = getThreshInteger(yellowLevelThr, osFlavor);
            orangeLevel = getThreshInteger(orangeLevelThr, osFlavor);
            redLevel    = getThreshInteger(redLevelThr, osFlavor);

            // invoke the "if" in the next statement
            cacheStates = null;
        }

        if (cacheStates == null || cacheStates.length != cacheUtils.length)
            cacheStates = new Color[cacheUtils.length];

        // Linux and AIX maintain no cache metrics
        ruleSupported = ((osFlavor != CmOSFlavor.MOS_LINUX) &&
                         (osFlavor != CmOSFlavor.MOS_AIX));
    }

    public void firePureRule() {
        int worstIndex = -1;

        // reset this
        state = Color.WHITE;
        problemCode = NO_PROBLEM_DETECTED;

        for(int i=0; i<cacheUtils.length; i++) {
            if (cacheSwings[i] < minEntries) {
                cacheStates[i] = Color.WHITE;
            } else if (cacheUtils[i] >= yellowLevel) {
                cacheStates[i] = Color.GREEN;
                if (isWorseThan(Color.GREEN, state))
                    worstIndex = i;
                state = worstState(state, Color.GREEN);
            } else if (cacheUtils[i] >= orangeLevel) {
                cacheStates[i] = Color.YELLOW;
                if (isWorseThan(Color.YELLOW, state))
                    worstIndex = i;
                state = worstState(state, Color.YELLOW);
            } else if (cacheUtils[i] >= redLevel) {
                cacheStates[i] = Color.ORANGE;
                if (isWorseThan(Color.ORANGE, state))
                    worstIndex = i;
                state = worstState(state, Color.ORANGE);
            } else {
                cacheStates[i] = Color.RED;
                if (isWorseThan(Color.RED, state))
                    worstIndex = i;
                state = worstState(state, Color.RED);
            }
        }

        if (worstIndex != -1)
            furtherDetail = cacheNames[worstIndex] + " "
                + RuleMessages.msgs.getString("Cache_Detail")
                + cacheUtils[worstIndex] + "%.";

        // set the action
        if (state.equals(Color.WHITE)) {
            action = RuleMessages.msgs.getString("No_Act");
            explanation = RuleMessages.msgs.getString("Cache_Unused");
            furtherDetail = RuleMessages.msgs.getString("No_Swings");
            solution = RuleMessages.msgs.getString("None");
        } else if (state.equals(Color.GREEN)) {
            action = RuleMessages.msgs.getString("Mild_Act");
            explanation = RuleMessages.msgs.getString("Cache_Idle");
            furtherDetail = RuleMessages.msgs.getString("All_Cache_Idle");
            solution = RuleMessages.msgs.getString("None");
        } else if (state.equals(Color.YELLOW)) {
            action = RuleMessages.msgs.getString("Heated_Act");
            explanation = RuleMessages.msgs.getString("Cache_Yellow");
            solution = RuleMessages.msgs.getString("None");
            problemCode = POOR_CACHE_UTILIZATION;
        } else if (state.equals(Color.ORANGE)) {
            action = RuleMessages.msgs.getString("Cache_Peak");
            explanation = RuleMessages.msgs.getString("Cache_Orange");
            solution = RuleMessages.msgs.getString("Bigger_Cache");
            problemCode = POOR_CACHE_UTILIZATION;
        } else if (state.equals(Color.RED)) {
            action = RuleMessages.msgs.getString("Cache_Overload");
            explanation = RuleMessages.msgs.getString("Cache_Red");
            solution = RuleMessages.msgs.getString("Bigger_Cache");
            problemCode = POOR_CACHE_UTILIZATION;
        }
    }

    public void print(PrintStream out) {
        out.println(formatInteger(minEntriesThr, osFlavor));
        out.println(formatInteger(yellowLevelThr, osFlavor));
        out.println(formatInteger(orangeLevelThr, osFlavor));
        out.println(formatInteger(redLevelThr, osFlavor));
    }
}
