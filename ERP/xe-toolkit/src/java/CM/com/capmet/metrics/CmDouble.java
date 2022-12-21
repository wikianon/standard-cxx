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

/**
 * The Captive Metrics integer class that is "set-able" and has an indication of whether
 * the value contained is a supported value.
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class CmDouble extends CmValue {
    /**
     * The contained value.
     */
    private double doubleValue;

    /**
     * The default constructor. There is no value and non-support is indicated.
     */
    public CmDouble() {
        super(false);
        doubleValue = 0.0;
    }

    /**
     * There is a value and it is supported.
     * @param value The <CODE>double</CODE> value to hold.
     */
    public CmDouble(double value) {
        super(true);
        doubleValue = value;
    }

    /**
     * Constructor with no value and indication of support.
     * @param supported The indication of whether the value is supported.
     */
    public CmDouble(boolean supported) {
        super(supported);
        doubleValue = 0.0;
    }

    /**
     * Constructor with value and indication of support.
     * @param value The <CODE>double</CODE> value to contain.
     * @param supported The indication of whether the value is supported.
     */
    public CmDouble(double value, boolean supported) {
        super(supported);
        doubleValue = value;
    }

    /**
     * Set the contained value to a new value.
     * @param value The new value.
     */
    public void setValue(double value) {
        doubleValue = value;
        supported = true;
    }

    /**
     * Return the type as indicated by <CODE>CmFieldType</CODE>.
     * @return A <CODE>CmFieldType</CODE> value.
     */
    public CmFieldType getType() {
        return CmFieldType.PF_DOUBLE;
    }

    /**
     * Return the contained value as an <CODE>int</CODE>.
     * @return The <CODE>double</CODE> value casted to an <CODE>int</CODE>.
     */
    public int intValue() {
        return (int) doubleValue;
    }

    /**
     * Return the contained value as a <CODE>long</CODE>.
     * @return The <CODE>double</CODE> value casted to a <CODE>long</CODE>.
     */
    public long longValue() {
        return (long) doubleValue;
    }

    /**
     * Return the contained value as a <CODE>double</CODE>.
     * @return The <CODE>double</CODE> value that is contained by this object.
     */
    public double doubleValue() {
        return doubleValue;
    }

    /**
     * Convert the contained value to a string.
     * @return A <CODE>String</CODE> representation of the contained value.
     */
    public String toString() {
        return Double.toString(doubleValue);
    }
}
