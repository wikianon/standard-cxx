/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector plugin for Windows.
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

package com.capmet.plugins.CaptiveCollector.Windows;

import java.rmi.server.UID;
import com.capmet.metrics.rmi.FieldSource;
import com.capmet.metrics.rmi.CategorySource;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.CmFieldType;

public class WinPluginField implements FieldSource
{
    private int            fieldIndex;
    private CmFieldType    fieldType;
    private String         instanceName;
    private WinPluginCat   categoryObj;

    public WinPluginField(int fieldIndex, CmFieldType fieldType,
                          String instanceName, WinPluginCat categoryObj)
        throws PerformanceException
    {
        this(fieldIndex, fieldType, categoryObj);
        this.instanceName = instanceName;
    }

    public WinPluginField(int fieldIndex, CmFieldType fieldType,
                          WinPluginCat categoryObj)
        throws PerformanceException
    {
        this.fieldIndex = fieldIndex;
        this.fieldType = fieldType;
        this.categoryObj = categoryObj;
        this.instanceName = null;
    }

    public boolean isSupported()
        throws PerformanceException
    {
        return true;
    }

    public Object getValue()
        throws PerformanceException
    {
        if (instanceName != null)
            return categoryObj.getRawValue(fieldIndex, instanceName);
        else
            return categoryObj.getRawValue(fieldIndex);
    }

    public Object getNextValue()
        throws PerformanceException
    {
        categoryObj.categoryRefresh();
        if (instanceName != null)
            return categoryObj.getRawValue(fieldIndex, instanceName);
        else
            return categoryObj.getRawValue(fieldIndex);
    }

    public CmFieldType getFieldType()
    {
        return fieldType;
    }
}
