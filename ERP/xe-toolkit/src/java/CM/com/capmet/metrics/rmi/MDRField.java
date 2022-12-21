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
import com.capmet.metrics.jmdr.mdr_performance_counter_t;
import com.capmet.metrics.jmdr.mdr_exception_t;

/**
 * A field object interfacing to the native operating system metrics field.
 */
public class MDRField implements FieldSource
{
    /**
     * The underlying mdr field object.
     */
    private mdr_performance_counter_t fieldObj;
    /**
     * The name of the category.
     */
    private String categoryName;
    /**
     * The name of the field.
     */
    private String fieldName;
    /**
     * The name of the instance.
     */
    private String instanceName;
    /**
     * The type of the field.
     */
    private CmFieldType fieldType;

    /**
     * Singleton constructor.
     * @param category The name of the category.
     * @param field The name of the field.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     */
    public MDRField(String category, String field)
        throws PerformanceException
    {
        this(category, field, null);
    }

    /**
     * N-tuple constructor.
     * @param category The name of the category.
     * @param field The name of the field.
     * @param instance The name of the instance.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     */
    public MDRField(String category, String field, String instance)
        throws PerformanceException
    {
        categoryName = category;
        fieldName = field;
        instanceName = instance;

        try
        {
            fieldObj = new mdr_performance_counter_t(category, field, instance);
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }

        Object o = getValue();

        if (o instanceof Integer)
            fieldType = CmFieldType.PF_INT;
        else if (o instanceof Long)
            fieldType = CmFieldType.PF_LONG;
        else if (o instanceof Double)
            fieldType = CmFieldType.PF_DOUBLE;
        else if (o instanceof String)
            fieldType = CmFieldType.PF_STRING;
        else if (o instanceof Boolean)
            fieldType = CmFieldType.PF_BOOLEAN;
        else
            throw new PerformanceException(
                "MDRField::MDRField: " +
                    RMIMessages.msgs.getString("unknown_type"));
    }

    /**
     * Return indication of whether this field is supported.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return <CODE>true</CODE>: it is supported - <CODE>false</CODE>: it is not supported.
     */
    public boolean isSupported()
        throws PerformanceException
    {
        try
        {
            return fieldObj.mdr_is_supported();
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    /**
     * Get the current value of the performance field.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return <CODE>Object</CODE> value of the performance field.
     */
    public Object getValue() throws PerformanceException
    {
        try
        {
            return fieldObj.mdr_value();
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    /**
     * Re-read the metric and return the new value of the performance field.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return <CODE>Object</CODE> value of the performance field.
     */
    public Object getNextValue() throws PerformanceException
    {
        try
        {
            return fieldObj.mdr_next_value();
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    /**
     * Return the data type of the field as a <CODE>CmFieldType</CODE>.
     * @return A <CODE>CmFieldType</CODE> indicating the
     * data type of the field.
     * @see <CODE>CmFieldType</CODE>
     */
    public CmFieldType getFieldType()
    {
        return fieldType;
    }
}
