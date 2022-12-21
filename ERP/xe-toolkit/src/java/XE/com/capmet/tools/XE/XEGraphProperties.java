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

/**
 * The class for defining the properties to be
 * given to the <CODE>XEGraph</CODE> constructor.
 */
public class XEGraphProperties implements Serializable
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
     * The title for the X axis.
     */
    public String   XAxisTitle;
    /**
     * The title for the Y axis.
     */
    public String   YAxisTitle;
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
     * The number of intervals to display on the graph.
     */
    public int      numDataPoints;
    /**
     * The minimum value shown on the X axis.
     */
    public int      minValue;
    /**
     * The maximum value shown on the X axis.
     */
    public int      maxValue;
    /**
     * Set the precision of the data labels on the graph.
     */
    public int      precision;
    /**
     * Is this a bar graph?
     */
    public boolean  isBarGraph;
    /**
     * Is this a line graph?
     */
    public boolean  isLineGraph;
    /**
     * Is this an area graph?
     */
    public boolean  isAreaGraph;
    /**
     * What is the thickness of the top line/line/bar?
     */
    public int      lineThickness;

    /**
     * Constructor for the area graph properties class.
     * @param legendStrings The text for the legend for each dataset.
     * @param legendColors The corresponding colors for each dataset.
     */
    public XEGraphProperties(String[] legendStrings, Color[] legendColors) {
        // default: area graph
        this.isBarGraph = false;
        this.isLineGraph = false;
        this.isAreaGraph = true;
        this.lineThickness = 1;

        this.legendStrings = legendStrings;
        this.legendColors = legendColors;
        this.numDataItems = legendStrings.length;

        title = XEMessages.msgs.getString("Graph");
        XAxisTitle = XEMessages.msgs.getString("X_Axis");
        YAxisTitle = XEMessages.msgs.getString("Y_Axis");
        intervalSeconds = 2;
        numDataPoints = 10;
        minValue = 0;
        maxValue = 100;
        precision = 2;
    }

    public void configureAsBarGraph() {
        isBarGraph = true;
        isLineGraph = false;
        isAreaGraph = false;
        lineThickness = 5;
    }

    public void configureAsLineGraph() {
        isBarGraph = false;
        isLineGraph = true;
        isAreaGraph = false;
        lineThickness = 5;
    }

    public void configureAsAreaGraph() {
        isBarGraph = false;
        isLineGraph = true;   // area graph is a filled line graph
        isAreaGraph = true;
        lineThickness = 1;
    }
}
