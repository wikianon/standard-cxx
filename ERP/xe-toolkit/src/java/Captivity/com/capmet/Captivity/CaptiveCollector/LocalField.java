/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector application of Captivity.
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

package com.capmet.Captivity.CaptiveCollector;

import com.capmet.metrics.rmi.FieldClone;
import com.capmet.metrics.rmi.PerformanceException;

/**
 * The field source for the local computer.
 */
public class LocalField extends FieldClone
{
    /**
     * The singleton constructor.
     * @param category The name of the category
     * @param field The name of the field.
     * @throws PerformanceException Any lower level exception.
     */
    public LocalField(String category, String field)
        throws PerformanceException
    {
        this(category, field, null);
    }

    /**
     * The n-tuple constructor.
     * @param category The name of the category
     * @param field The name of the field.
     * @param instance The name of the instance.
     * @throws PerformanceException Any lower level exception.
     */
    public LocalField(String category, String field, String instance)
        throws PerformanceException
    {
        LocalCategory categoryObj = new LocalCategory(category);
        int fieldNumber = categoryObj.getFieldNumber(field);

        if (fieldNumber == -1)
            throw new PerformanceException("LocalField::LocalField: field: " +
                field + " does not exist");

        if (instance != null)
        {
            if (categoryObj.isSingleton())
                throw new PerformanceException(
                    "LocalField::LocalField: category " + category +
                        " does not require an instance");

            fieldObj = categoryObj.getField(fieldNumber, instance);
        }
        else
        {
            if (!categoryObj.isSingleton())
                throw new PerformanceException(
                    "LocalField::LocalField: category " + category +
                        " requires an instance");

            fieldObj = categoryObj.getField(fieldNumber);
        }
    }
}
