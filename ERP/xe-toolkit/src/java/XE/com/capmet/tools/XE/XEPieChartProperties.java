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

import java.awt.Color;
import java.io.Serializable;
import net.sourceforge.chart2d.PieChart2DProperties;

/**
 * The class for defining the properties to be
 * given to the <CODE>XEPieChart</CODE> constructor.
 */
public class XEPieChartProperties implements Serializable
{
    /**
     * The graph title.
     */
    public String   title;
    /**
     * The strings for the legends. Array indexes match
     * those of the <CODE>legendColors</CODE> member.
     */
    public String[] legendStrings;
    /**
     * The colors to be used for the datasets. Array
     * indexes match those of <CODE>legendStrings</CODE>.
     */
    public Color[]  legendColors;
    /**
     * The number of seconds between updates.
     */
    public int      intervalSeconds;
    /**
     * The number of data items collected. Equal to
     * <CODE>legendStrings.length</CODE>.
     */
    public int      numDataItems;
    /**
     * Set the precision of the data labels on the graph.
     */
    public int      precision;
    /**
     * The mode of display of the on-chart values.
     * @see PieChart2DProperties
     */
    public int      displayMode;

    /**
     * Constructor for the pie chart properties class.
     * @param legendStrings The text for the legend for each dataset.
     * @param legendColors The corresponding colors for each dataset.
     */
    public XEPieChartProperties(String[] legendStrings, Color[] legendColors)
    {
        this.legendStrings = legendStrings;
        this.legendColors = legendColors;
        this.numDataItems = legendStrings.length;

        title = XEMessages.msgs.getString("Pie_Chart");
        intervalSeconds = 2;
        precision = 2;

        // PieChart2DProperties: RAW, PERCENT or RAW+PERCENT
        displayMode = PieChart2DProperties.RAW + PieChart2DProperties.PERCENT;
    }
}
