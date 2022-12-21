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
import java.io.File;
import java.io.PrintStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;
import java.util.Formatter;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import com.capmet.metrics.CmString;
import com.capmet.metrics.CmOSFlavor;
import com.capmet.metrics.rmi.PerformanceException;

public abstract class Rule<SeedValues> {

    private static Properties properties;

    /**
     * Universal problem code indicating no problem detected.
     */
    public final static int NO_PROBLEM_DETECTED = 0;

    public final static int CPU_CODES       = 100;
    public final static int DISK_CODES      = 200;
    public final static int MEMORY_CODES    = 300;
    public final static int PAGE_FILE_CODES = 400;
    public final static int CACHE_CODES     = 500;
    public final static int NET_CODES       = 600;
    public final static int TCP_CODES       = 700;

    /**
     * Is this rule a single rule or an aggregate rule?
     */
    protected boolean aggregateRule;
    /**
     * This rule may not work on this platform.
     */
    protected boolean ruleSupported;
    /**
     * The name of this rule.
     */
    protected String ruleName;
    /**
     * The one word name of the rule.
     */
    protected String shortRuleName;
    /**
     * Color code of the state.
     */
    protected Color state;
    /**
     * Terse description of the problem.
     */
    protected String action;
    /**
     * Detail explanation of the problem.
     */
    protected String explanation;
    /**
     * Esoteric detail of the problem.
     */
    protected String furtherDetail;
    /**
     * The rule name that caused the current condition.
     */
    protected String triggerRuleName;
    /**
     * A suggestion on how to relieve the problem.
     */
    protected String solution;
    /**
     * Numeric code for specific per-rule problem.
     */
    protected int problemCode;

    /**
     * Is this rule a single rule or an aggregate rule?
     * @return true - aggregate rule; false - single rule
     */
    public boolean isAggregateRule() {
        return aggregateRule;
    }

    /**
     * Is this rule supported?
     * @return true - yes; false - no
     */
    public boolean isRuleSupported() {
        return ruleSupported;
    }

    /**
     * Get the rule name.
     * @return The name of the rule.
     */
    public String getRuleName() {
        return ruleName;
    }

    /**
     * Get the short rule name.
     * @return The short name of the rule.
     */
    public String getShortRuleName() {
        return shortRuleName;
    }

    /**
     * Get the rule state.
     * @return The state the rule is in.
     */
    public Color getState() {
        return state;
    }

    /**
     * Get the action string.
     * @return The action taking place according to the rule.
     */
    public String getAction() {
        return action;
    }

    /**
     * Get the explanation string.
     * @return An explanation of the condition that the rule detects.
     */
    public String getExplanation() {
        return explanation;
    }

    /**
     * Get the detail string.
     * @return Additional detail about the action taking place.
     */
    public String getFurtherDetail() {
        return furtherDetail;
    }

    /**
     * Get the name of the rule that triggered the current condition.
     * @return The trigger rule name.
     */
    public String getTriggerRuleName() {
        return triggerRuleName;
    }

    /**
     * Get the solution string.
     * @return The solution string.
     */
    public String getSolution() {
        return solution;
    }

    /**
     * Get the numeric code of the problem detected.
     * @return The problem code.
     */
    public int getProblemCode() {
        return problemCode;
    }

    //
    // Rule Threshold Code
    //

    static {
        String top = System.getProperty("com.capmet.CaptiveCollector.Top");

        if (top == null)
            throw new RuntimeException(RuleMessages.msgs.getString("No_Top"));

        String threshPath = top + File.separatorChar + "etc"
                          + File.separatorChar + "XE_RuleThresh.properties";

        properties = new Properties();
        File threshFile = new File(threshPath);
        if (threshFile.exists())
            try {
                properties.load(new FileInputStream(threshFile));
            } catch(IOException e) {
                // oh well
            }
    }

    public Rule() {
        state = Color.WHITE;
        action = stateUninitialized;
        explanation = stateUninitialized;
        furtherDetail = stateUninitialized;
        triggerRuleName = stateUninitialized;
        solution = stateUninitialized;
        problemCode = NO_PROBLEM_DETECTED;
        aggregateRule = false;
    }

    public int getThreshInteger(Threshold<Integer> thresh, CmOSFlavor osFlavor) {
        String property = thresh.propertyName + '.' + osFlavor;
        String value;
        int returnValue;

        value = properties.getProperty(property);
        if (value == null)
            value = properties.getProperty(thresh.propertyName);

        if (value == null)
            returnValue = thresh.defaultValue;
        else
            returnValue = Integer.parseInt(value);

        return returnValue;
    }

    public double getThreshDouble(Threshold<Double> thresh, CmOSFlavor osFlavor) {
        String property = thresh.propertyName + '.' + osFlavor;
        String value;
        double returnValue;

        value = properties.getProperty(property);
        if (value == null)
            value = properties.getProperty(thresh.propertyName);

        if (value == null)
            returnValue = thresh.defaultValue;
        else
            returnValue = Double.parseDouble(value);

        return returnValue;
    }

    public String getThreshString(Threshold<String> thresh, CmOSFlavor osFlavor) {
        String property = thresh.propertyName + '.' + osFlavor;
        String value;

        value = properties.getProperty(property);
        if (value == null)
            value = properties.getProperty(thresh.propertyName);

        if (value == null)
            return thresh.defaultValue;
        else
            return value;
    }

    public boolean getThreshBoolean(Threshold<Boolean> thresh, CmOSFlavor osFlavor) {
        String property = thresh.propertyName + '.' + osFlavor;
        String value;
        boolean returnValue;

        value = properties.getProperty(property);
        if (value == null)
            value = properties.getProperty(thresh.propertyName);

        if (value == null)
            returnValue = thresh.defaultValue;
        else
            returnValue = Boolean.parseBoolean(value);

        return returnValue;
    }

    public String formatInteger(Threshold<Integer> thresh, CmOSFlavor osFlavor) {
        String property = thresh.propertyName + '.' + osFlavor;
        StringBuilder sb = new StringBuilder();
        Formatter formatter = new Formatter(sb);
        String fmt;

        fmt = "%s " + RuleMessages.msgs.getString("default")
            + "=%" + thresh.fieldWidth + "d %s "
            + RuleMessages.msgs.getString("property")
            + "=%" + thresh.fieldWidth + "d %s " + thresh.comment;

        formatter.format(fmt, property,
            thresh.defaultValue, thresh.unitsSuffix,
            getThreshInteger(thresh, osFlavor), thresh.unitsSuffix);

        return sb.toString();
    }

    public String formatDouble(Threshold<Double> thresh, CmOSFlavor osFlavor) {
        String property = thresh.propertyName + '.' + osFlavor;
        StringBuilder sb = new StringBuilder();
        Formatter formatter = new Formatter(sb);
        String fmt;

        fmt = "%s " + RuleMessages.msgs.getString("default")
            + "=%" + thresh.fieldWidth + "." + thresh.fieldPrecision
            + "f %s "
            + RuleMessages.msgs.getString("property")
            + "=%" + thresh.fieldWidth + "." + thresh.fieldPrecision
            + "f %s " + thresh.comment;

        formatter.format(fmt, property,
            thresh.defaultValue, thresh.unitsSuffix,
            getThreshDouble(thresh, osFlavor), thresh.unitsSuffix);

        return sb.toString();
    }

    public String formatString(Threshold<String> thresh, CmOSFlavor osFlavor) {
        String property = thresh.propertyName + '.' + osFlavor;
        StringBuilder sb = new StringBuilder();
        Formatter formatter = new Formatter(sb);
        String fmt;

        fmt = "%s " + RuleMessages.msgs.getString("default")
            + "=%s " + RuleMessages.msgs.getString("property")
            + "=%s " + thresh.comment;

        formatter.format(fmt, property,
            thresh.defaultValue, getThreshString(thresh, osFlavor));

        return sb.toString();
    }

    public String formatBoolean(Threshold<Boolean> thresh, CmOSFlavor osFlavor) {
        String property = thresh.propertyName + '.' + osFlavor;
        StringBuilder sb = new StringBuilder();
        Formatter formatter = new Formatter(sb);
        String fmt;

        fmt = "%s " + RuleMessages.msgs.getString("default") + "=%s "
            + RuleMessages.msgs.getString("property") + "=%s "
            + thresh.comment;

        formatter.format(fmt, property,
            thresh.defaultValue ? "true" : "false",
            getThreshBoolean(thresh, osFlavor) ? "true" : "false");

        return sb.toString();
    }

    //
    // Rule State Code
    //

    private static String stateUninitialized =
        RuleMessages.msgs.getString("state_uninit");

    /**
     * Translate a <CODE>Color</CODE> to a color name.
     * @param state The state as a <CODE>Color</CODE>.
     * @return The string representation of the color.
     */
    public static String stateString(Color state) {
        if (state.equals(Color.WHITE))
            return RuleMessages.msgs.getString("white");
        else if (state.equals(Color.BLUE))
            return RuleMessages.msgs.getString("blue");
        else if (state.equals(Color.GREEN))
            return RuleMessages.msgs.getString("green");
        else if (state.equals(Color.YELLOW))
            return RuleMessages.msgs.getString("yellow");
        else if (state.equals(Color.ORANGE))
            return RuleMessages.msgs.getString("orange");
        else if (state.equals(Color.RED))
            return RuleMessages.msgs.getString("red");
        else if (state.equals(Color.BLACK))
            return RuleMessages.msgs.getString("black");
        else
            return state.toString();
    }

    /**
     * Translate a <CODE>Color</CODE> to a description of the condition.
     * @param state The state as a <CODE>Color</CODE>.
     * @return A string description of the color-state.
     */
    public static String stateExplain(Color state) {
        if (state.equals(Color.WHITE))
            return RuleMessages.msgs.getString("white_explain");
        else if (state.equals(Color.BLUE))
            return RuleMessages.msgs.getString("blue_explain");
        else if (state.equals(Color.GREEN))
            return RuleMessages.msgs.getString("green_explain");
        else if (state.equals(Color.YELLOW))
            return RuleMessages.msgs.getString("yellow_explain");
        else if (state.equals(Color.ORANGE))
            return RuleMessages.msgs.getString("orange_explain");
        else if (state.equals(Color.RED))
            return RuleMessages.msgs.getString("red_explain");
        else if (state.equals(Color.BLACK))
            return RuleMessages.msgs.getString("black_explain");
        else
            return RuleMessages.msgs.getString("invalid_state");
    }

    // translate colors to integer values
    public static int colorValue(Color state) {
        if (state.equals(Color.WHITE))
            return 0;
        else if (state.equals(Color.BLUE))
            return 1;
        else if (state.equals(Color.GREEN))
            return 2;
        else if (state.equals(Color.YELLOW))
            return 3;
        else if (state.equals(Color.ORANGE))
            return 4;
        else if (state.equals(Color.RED))
            return 5;
        else if (state.equals(Color.BLACK))
            return 6;
        else
            return -1;
    }

    /**
     * Compare two color states and return color in the worst condition.
     * @param s1 State 1
     * @param s2 State 2
     * @return The color that represents the worst state.
     */
    public static Color worstState(Color s1, Color s2) {
        int c1 = colorValue(s1);
        int c2 = colorValue(s2);

        return (c1 > c2 ? s1 : s2);
    }

    /**
     * Compare two color states and return color in the best condition.
     * @param s1 State 1
     * @param s2 State 2
     * @return The color that represents the best state.
     */
    public static Color bestState(Color s1, Color s2) {
        int c1 = colorValue(s1);
        int c2 = colorValue(s2);

        return (c1 > c2 ? s2 : s1);
    }

    /**
     * Determine if one state is worse than the other.
     * @param s1 State 1
     * @param s2 State 2
     * @return True: s1 is worse than s2; False: s2 is worse than s1
     */
    public static boolean isWorseThan(Color s1, Color s2) {
        int c1 = colorValue(s1);
        int c2 = colorValue(s2);

        return (c1 > c2);
    }

    /**
     * Determine if a state is at a problem level.
     * @param state The state to check.
     * @return true - problem level; false - not problem level.
     */
    public static boolean isProblemLevel(Color state) {
        return state.equals(Color.YELLOW) ||
               state.equals(Color.ORANGE) ||
               state.equals(Color.RED) ||
               state.equals(Color.BLACK);
    }

    /**
     * Offer a background color so text written in the state color
     * upon it will be easily seen.
     * @param state The state to check.
     * @return A contrasting color; either black or white or yellow
     * if handed a non-state color.
     */
    public static Color contrastingColor(Color state) {
        if (state.equals(Color.WHITE))
            return Color.BLACK;
        else if (state.equals(Color.BLUE))
            return Color.WHITE;
        else if (state.equals(Color.GREEN))
            return Color.BLACK;
        else if (state.equals(Color.YELLOW))
            return Color.BLACK;
        else if (state.equals(Color.ORANGE))
            return Color.BLACK;
        else if (state.equals(Color.RED))
            return Color.WHITE;
        else if (state.equals(Color.BLACK))
            return Color.WHITE;
        else
            return Color.YELLOW;
    }

    /**
     * Parse a <CODE>String</CODE> color name and return the appropriate <CODE>Color</CODE> or
     * <CODE>Color.PINK</CODE> if the color is not a state color.
     * @param colorName The name of the color
     * @return A corresponding <CODE>Color</CODE> value or <CODE>Color.PINK</CODE> if handed a non-state color.
     */
    public static Color parseColor(String colorName) {
        if (colorName.equals(RuleMessages.msgs.getString("white")))
            return Color.WHITE;
        else if (colorName.equals(RuleMessages.msgs.getString("blue")))
            return Color.BLUE;
        else if (colorName.equals(RuleMessages.msgs.getString("green")))
            return Color.GREEN;
        else if (colorName.equals(RuleMessages.msgs.getString("yellow")))
            return Color.YELLOW;
        else if (colorName.equals(RuleMessages.msgs.getString("orange")))
            return Color.ORANGE;
        else if (colorName.equals(RuleMessages.msgs.getString("red")))
            return Color.RED;
        else if (colorName.equals(RuleMessages.msgs.getString("black")))
            return Color.BLACK;
        else
            return Color.PINK;
    }

    public abstract void seedValues(SeedValues sv);
    public abstract void firePureRule();
    public abstract void fireLiveRule() throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException;
    public abstract void print(PrintStream out);
}
