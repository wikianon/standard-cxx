/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the XE library for The XE Toolkit.
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

package com.capmet.tools.XE;

/**
 * A class that contains information about counters
 * that have been selected by the XEPerformanceFieldSelector class.
 */
public class XECounterData
{
    /**
     * The host name of the counter.
     */
    public String host;
    /**
     * The category name of the counter.
     */
    public String category;
    /**
     * The field name of the counter.
     */
    public String counter;
    /**
     * The instance name of the counter.
     */
    public String instance;
    /**
     * The scale of the counter.
     */
    public double scale;

    /**
     * Set all the members to an initial value.
     */
    public void clear()
    {
        host = null;
        category = null;
        counter = null;
        instance = null;
        scale = 0.0;
    }

    /**
     * Default constructor.
     */
    public XECounterData()
    {
        clear();
    }
}
