/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Captive Metrics library of Captivity.
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

package com.capmet.metrics;

import com.capmet.metrics.CmFieldType;

/**
 * The Captive Metrics boolean class that is "set-able" and has an indication of whether
 * the value contained is a supported value.
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class CmBoolean extends CmValue {
    /**
     * The contained value.
     */
    private boolean booleanValue;

    /**
     * The default constructor. There is no value and non-support is indicated.
     */
    public CmBoolean() {
        super(false);
        booleanValue = new Boolean(false);
    }

    /**
     * There is a value and it is supported.
     * @param value The <CODE>Boolean</CODE> value to hold.
     */
    public CmBoolean(Boolean value) {
        super(true);
        booleanValue = value.booleanValue();
    }

    /**
     * Constructor with no value and indication of support.
     * @param supported The indication of whether the value is supported.
     */
    public CmBoolean(boolean supported) {
        super(supported);
        booleanValue = false;
    }

    /**
     * Constructor with value and indication of support.
     * @param value The <CODE>Boolean</CODE> value to contain.
     * @param supported The indication of whether the value is supported.
     */
    public CmBoolean(Boolean value, boolean supported) {
        super(supported);
        booleanValue = value.booleanValue();
    }

    /**
     * Set the contained value to a new value.
     * @param value The new value.
     */
    public void setValue(boolean value) {
        booleanValue = value;
        supported = true;
    }

    /**
     * Return the type as indicated by <CODE>CmFieldType</CODE>.
     * @return A <CODE>CmFieldType</CODE> value.
     */
    public CmFieldType getType() {
        return CmFieldType.PF_BOOLEAN;
    }

    /**
     * Return the contained value.
     * @return The <CODE>boolean</CODE> value that is contained by this object.
     */
    public boolean booleanValue() {
        return booleanValue;
    }

    /**
     * Convert the contained value to a string.
     * @return A <CODE>String</CODE> representation of the contained value.
     */
    public String toString() {
        return Boolean.toString(booleanValue);
    }
}
