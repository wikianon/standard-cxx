/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the XE programs for The XE Toolkit.
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

package com.capmet.tools.XE.programs;

import java.io.Serializable;
import java.awt.Rectangle;
import com.capmet.tools.XE.XEPieChartProperties;

/**
 * The properties of the XE Gadgets selected within the tab of that name
 * in the <CODE>Top</CODE> application. These properties can be save and
 * and restored to and from the gadget objects.
 */
public class GadgetProperties implements Serializable {
    /**
     * Has the cpu pie chart been created?
     */
    public boolean cpuPieCreated;
    /**
     * Has the memory pie chart been created?
     */
    public boolean memPieCreated;
    /**
     * Has the page-file pie chart been created?
     */
    public boolean pfPieCreated;

    /**
     * Is the cpu pie chart closed?
     */
    public boolean cpuPieClosed;
    /**
     * Is the memory pie chart closed?
     */
    public boolean memPieClosed;
    /**
     * Is the page-file pie chart closed?
     */
    public boolean pfPieClosed;

    /**
     * The cpu pie chart bounds.
     */
    public Rectangle cpuPieBounds;
    /**
     * The memory pie chart bounds.
     */
    public Rectangle memPieBounds;
    /**
     * The page-file pie chart bounds.
     */
    public Rectangle pfPieBounds;

    /**
     * The <CODE>XEPieChart</CODE> properties for the cpu chart.
     */
    public XEPieChartProperties cpuProperties;
    /**
     * The <CODE>XEPieChart</CODE> properties for the memory chart.
     */
    public XEPieChartProperties memProperties;
    /**
     * The <CODE>XEPieChart</CODE> properties for the page-file chart.
     */
    public XEPieChartProperties pfProperties;

    /**
     * Default constructor.
     */
    public GadgetProperties() {
        cpuPieCreated = false;
        memPieCreated = false;
        pfPieCreated = false;

        cpuPieClosed = false;
        memPieClosed = false;
        pfPieClosed = false;

        cpuPieBounds = null;
        memPieBounds = null;
        pfPieBounds = null;

        cpuProperties = null;
        memProperties = null;
        pfProperties = null;
    }
}
