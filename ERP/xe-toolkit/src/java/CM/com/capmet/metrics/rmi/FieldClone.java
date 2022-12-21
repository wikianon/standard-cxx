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
 * A class that implements <CODE>FieldSource</CODE> and simply
 * calls into the protected field object for all methods.
 */
public abstract class FieldClone implements FieldSource
{
    /**
     * The <CODE>FieldSource</CODE> object populated by the subclass.
     */
    protected FieldSource fieldObj;

    /**
     * Constructor.
     * @throws PerformanceException Any lower level exception.
     */
    public FieldClone()
        throws PerformanceException
    {
        fieldObj = null;
    }

    /**
     * Return the supported indication.
     * @throws PerformanceException Any lower level exception.
     * @return true - is supported; false - is not supported
     */
    public boolean isSupported()
        throws PerformanceException
    {
        return fieldObj.isSupported();
    }
    
    /**
     * Get the value of the field.
     * @throws PerformanceException Any lower level exception.
     * @return The field object.
     */
    public Object getValue() throws PerformanceException
    {
        return fieldObj.getValue();
    }

    /**
     * Update the field and return the value.
     * @throws PerformanceException Any lower level exception.
     * @return The field object.
     */
    public Object getNextValue() throws PerformanceException
    {
        return fieldObj.getNextValue();
    }

    /**
     * Get the type of the field.
     * @return A <CODE>CmFieldType</CODE> indication of field type.
     */
    public CmFieldType getFieldType()
    {
        return fieldObj.getFieldType();
    }
}
