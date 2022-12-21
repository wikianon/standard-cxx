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

package com.capmet.metrics.rmi;

import com.capmet.metrics.CmFieldType;

/**
 * Define the interface for a field that all field subobjects must implement.
 */
public interface FieldSource
{
    /**
     * Return indication of whether this field is supported.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return <CODE>true</CODE>: it is supported - <CODE>false</CODE>: it is not supported.
     */
    public boolean isSupported()
        throws PerformanceException;
    /**
     * Get the current value of the performance field.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return <CODE>Object</CODE> value of the performance field.
     */
    public Object getValue()
        throws PerformanceException;
    /**
     * Re-read the metric and return the new value of the performance field.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return <CODE>Object</CODE> value of the performance field.
     */
    public Object getNextValue()
        throws PerformanceException;
    /**
     * Return the data type of the field as a <CODE>CmFieldType</CODE>.
     * @return A <CODE>CmFieldType</CODE> indicating the
     * data type of the field.
     * @see <CODE>CmFieldType</CODE>
     */
    public CmFieldType getFieldType();
}
