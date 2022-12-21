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
 * The Captive Metrics integer class that is "set-able" and has an indication of whether
 * the value contained is a supported value.
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class CmInteger extends CmValue {
    /**
     * The contained value.
     */
    private int intValue;

    /**
     * The default constructor. There is no value and non-support is indicated.
     */
    public CmInteger() {
        super(false);
        intValue = 0;
    }

    /**
     * There is a value and it is supported.
     * @param value The <CODE>int</CODE> value to hold.
     */
    public CmInteger(int value) {
        super(true);
        intValue = value;
    }

    /**
     * Constructor with no value and indication of support.
     * @param supported The indication of whether the value is supported.
     */
    public CmInteger(boolean supported) {
        super(supported);
        intValue = 0;
    }

    /**
     * Constructor with value and indication of support.
     * @param value The <CODE>int</CODE> value to contain.
     * @param supported The indication of whether the value is supported.
     */
    public CmInteger(int value, boolean supported) {
        super(supported);
        intValue = value;
    }

    /**
     * Set the contained value to a new value.
     * @param value The new value.
     */
    public void setValue(int value) {
        intValue = value;
        supported = true;
    }

    /**
     * Return the type as indicated by <CODE>CmFieldType</CODE>.
     * @return A <CODE>CmFieldType</CODE> value.
     */
    public CmFieldType getType() {
        return CmFieldType.PF_INT;
    }

    /**
     * Return the contained value as an <CODE>int</CODE>.
     * @return The <CODE>int</CODE> value that is contained by this object.
     */
    public int intValue() {
        return intValue;
    }

    /**
     * Return the contained value as a <CODE>long</CODE>.
     * @return The <CODE>int</CODE> value casted to a <CODE>long</CODE>.
     */
    public long longValue() {
        return (long) intValue;
    }

    /**
     * Return the contained value as a <CODE>double</CODE>.
     * @return The <CODE>int</CODE> value casted to a <CODE>double</CODE>.
     */
    public double doubleValue() {
        return (double) intValue;
    }

    /**
     * Convert the contained value to a string.
     * @return A <CODE>String</CODE> representation of the contained value.
     */
    public String toString() {
        return Integer.toString(intValue);
    }
}
