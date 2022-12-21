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
 * A field to be collected by the PerformanceCollection class.
 */
public class CollectionItem implements Serializable
{
    /**
     * The performance category of this collection item.
     */
    public String  category;
    /**
     * The performance field of this collection item.
     */
    public String  field;
    /**
     * The performance instance of this collection item.
     */
    public String  instance;
    /**
     * The unique identifier of this collection item.
     */
    public UID     objectId;
    /**
     * The value of the counter.
     */
    public Object value;
    /**
     * Whether this value is supported.
     */
    public boolean supported;
}
