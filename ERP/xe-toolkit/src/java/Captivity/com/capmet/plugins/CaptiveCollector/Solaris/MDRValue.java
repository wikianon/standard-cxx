/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector plugin for Solaris.
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

package com.capmet.plugins.CaptiveCollector.Solaris;

import java.rmi.server.UID;
import com.capmet.metrics.jmdr.mdr_t;
import com.capmet.metrics.jmdr.mdr_exception_t;
import com.capmet.metrics.rmi.FieldSource;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.CmFieldType;

public class MDRValue implements FieldSource
{
    private mdr_t       mdrObj;
    private int         fieldNumber;
    private String      instanceName;
    private boolean     isSingleton;
    private CmFieldType fieldType;

    public MDRValue(mdr_t mdr, int field)
        throws PerformanceException
    {
        this(mdr, field, null);
    }

    public MDRValue(mdr_t mdr, int field, String instance)
        throws PerformanceException
    {
        mdrObj = mdr;
        fieldNumber = field;
        instanceName = instance;
        isSingleton = mdrObj.mdr_is_singleton;

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
                "MDRValue::MDRValue: " +
                    SolarisMetrics.msgs.getString("unknown_type"));
    }

    public boolean isSupported()
        throws PerformanceException
    {
        return true;
    }

    public Object getValue()
        throws PerformanceException
    {
        try
        {
            if (isSingleton)
                return mdrObj.mdr_field_value(fieldNumber);
            else
                return mdrObj.mdr_field_value(instanceName, fieldNumber);
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public Object getNextValue()
        throws PerformanceException
    {
        try
        {
            mdrObj.mdr_refresh();

            if (isSingleton)
                return mdrObj.mdr_field_value(fieldNumber);
            else
                return mdrObj.mdr_field_value(instanceName, fieldNumber);
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public CmFieldType getFieldType()
    {
        return fieldType;
    }
}
