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

import java.rmi.server.UID;
import java.io.Serializable;

/**
 * Define the properties needed to be transmitted to the
 * <CODE>PerformanceCategory</CODE> class by the RMI server.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class PerformanceCategoryInfo implements Serializable
{
    /**
     * The name of the perfomance category.
     */
    public String     categoryName;
    /**
     * The explanation of the performance category.
     */
    public String     categoryExplanation;
    /**
     * Is this category supported?
     */
    public boolean    categorySupported;
    /**
     * Is this category a singleton?
     */
    public boolean    singleton;
    /**
     * The number of fields in this category.
     */
    public int        fieldCount;
    /**
     * The <CODE>hashCode()</CODE> of the remote object.
     */
    public UID        objectId;
}
