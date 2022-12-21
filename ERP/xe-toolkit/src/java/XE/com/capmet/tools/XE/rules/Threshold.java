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

public class Threshold<ThresholdType> {
    public String        propertyName;
    public ThresholdType defaultValue;
    public String        unitsSuffix;
    public int           fieldWidth;
    public int           fieldPrecision;
    public String        comment;

    /**
     * Default constructor.
     */
    public Threshold() {
    }

    /**
     * Constructor for Integer thresholds.
     * @param propertyName Name of the property
     * @param defaultValue Default value for the property.
     * @param unitsSuffix String suffix to add to value on display.
     * @param fieldWidth Size of output field for formatting.
     */
    public Threshold(String        propertyName,
                     ThresholdType defaultValue,
                     String        unitsSuffix,
                     int           fieldWidth) {
        this.propertyName = propertyName;
        this.defaultValue = defaultValue;
        this.unitsSuffix = unitsSuffix;
        this.fieldWidth = fieldWidth;
        this.comment = RuleMessages.msgs.getString(propertyName);
    }

    /**
     * Constructor for Double thresholds.
     * @param propertyName Name of the property
     * @param defaultValue Default value for the property.
     * @param unitsSuffix String suffix to add to value on display.
     * @param fieldWidth Size of output field for formatting.
     * @param fieldPrecision Size of precision field for formatting.
     */
    public Threshold(String        propertyName,
                     ThresholdType defaultValue,
                     String        unitsSuffix,
                     int           fieldWidth,
                     int           fieldPrecision) {
        this.propertyName = propertyName;
        this.defaultValue = defaultValue;
        this.unitsSuffix = unitsSuffix;
        this.fieldWidth = fieldWidth;
        this.fieldPrecision = fieldPrecision;
        this.comment = RuleMessages.msgs.getString(propertyName);
    }

    /**
     * Constructor for Boolean and String thresholds.
     * @param propertyName Name of the property
     * @param defaultValue Default value for the property.
     */
    public Threshold(String        propertyName,
                     ThresholdType defaultValue) {
        this.propertyName = propertyName;
        this.defaultValue = defaultValue;
        this.comment = RuleMessages.msgs.getString(propertyName);
    }
}
