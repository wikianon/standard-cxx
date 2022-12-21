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

import java.util.Vector;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import net.sourceforge.chart2d.*;

/**
 * The Chart2D pie chart object transmogrified into an XE object.
 */
class XEPieChart
{
    /**
     * The Chart2D area chart.
     */
    private PieChart2D chart2D;

    // a Vector of XEDataset
    private class DatasetCollection extends Vector<XEDataset>
    {
        public DatasetCollection(int initialCapacity)
        {
            super(initialCapacity);
        }
    }

    /**
     * Collection of datasets.
     */
    private DatasetCollection datasetCollection;

    /**
     * Provide access to the Chart2D object.
     * @return The Chart2D object.
     */
    public PieChart2D getChart()
    {
        return chart2D;
    }

    /**
     * Constructor for the pie chart object.
     * @param props Properties that define the look of the chart.
     */
    public XEPieChart(XEPieChartProperties props)
    {
        chart2D = new PieChart2D();
        setProperties(props);
    }

    /**
     * Set the properties of the pie chart.
     * @param props Properties that define the look of the chart.
     */
    public void setProperties(XEPieChartProperties props)
    {
        // Configure object properties
        Object2DProperties object2DProps = new Object2DProperties();
        object2DProps.setObjectTitleText(props.title);

        // Configure chart properties
        Chart2DProperties chart2DProps = new Chart2DProperties();
        chart2DProps.setChartDataLabelsPrecision(props.precision);

        // Configure legend properties
        LegendProperties legendProps = new LegendProperties();
        legendProps.setLegendLabelsTexts(props.legendStrings);

        MultiColorsProperties multiColorsProps = new MultiColorsProperties();
        multiColorsProps.setColorsCustomize(true);
        multiColorsProps.setColorsCustom(props.legendColors);

        // Configure pie area
        PieChart2DProperties pieChart2DProps = new PieChart2DProperties();
        pieChart2DProps.setPieLabelsType(props.displayMode);

        // Configure chart
        chart2D.setObject2DProperties(object2DProps);
        chart2D.setChart2DProperties(chart2DProps);
        chart2D.setLegendProperties(legendProps);
        chart2D.setMultiColorsProperties(multiColorsProps);
        chart2D.setPieChart2DProperties(pieChart2DProps);

        Dataset dataset = new Dataset(props.numDataItems, 1, 1);
        chart2D.setDataset(dataset);

        datasetCollection = new DatasetCollection(props.numDataItems);
    }

    /**
     * Add a dataset to the pie chart.
     * @param gp The pie chart value source.
     * @param userData Optional user data.
     * @return A new <CODE>XEDataset</CODE> object.
     */
    public XEDataset addElement(XEGraphPoint gp, Object userData)
    {
        XEDataset ds = new XEDataset(gp, userData);

        datasetCollection.addElement(ds);
        return ds;
    }

    /**
     * Invoked at each time interval to update the chart.
     */
    public void GraphInterval()
    {
        int i = 0;
        Dataset dataset = chart2D.getDataset();

        for(XEDataset ds : datasetCollection) {
            float f = ds.getNextPoint();
            dataset.set(i++, 0, 0, f);
        }

        chart2D.setDataset(dataset);
        chart2D.repaint();
    }

    /**
     * The data point structure for the pie chart.
     */
    public class XEDataset
    {
        /**
         * Callback for the current data point.
         */
        private XEGraphPoint graphPoint;
        /**
         * User data for graphPoint.
         */
        private Object userData;

        /**
         * The constructor for the class.
         * @param gp A XEGraphPoint callback to collect point data
         * @param userData passed in and then passed out to <CODE>graphPoint</CODE>
         * @returns a new <CODE>DataSet</CODE> object
         */
        public XEDataset(XEGraphPoint gp, Object userData)
        {
            graphPoint = gp;
            this.userData = userData;
        }

        public float getNextPoint()
        {
            return graphPoint.getPoint(userData);
        }
    }
}
