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

import java.text.MessageFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Vector;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import com.capmet.tools.XE.XEGraphProperties;
import net.sourceforge.chart2d.*;

/**
 * The XgChart2D area graph object transmogrified into an XE object.
 */
class XEGraph {
    private boolean             XgStackedData = true;
    private LBChart2D           XgChart2D;
    private int                 XgPointsCollected;
    private DatasetCollection   XgDatasetCollection;
    private String[]            XgXAxisLabels;
    private XEGraphProperties   XgProps;

    /**
     * Property indicating whether the graph is stacked.
     * @return true: is stacked; false: is not stacked
     */
    public boolean isStacked() {
        return XgStackedData;
    }

    /**
     * Set stacked graph property.
     * @param isStacked true or false
     */
    public void setStacked(boolean isStacked) {
        GraphProperties graphProps = XgChart2D.getGraphProperties(0);

        XgStackedData = isStacked;
        if (XgProps.isBarGraph) {
            if (isStacked)
                graphProps.setGraphAllowComponentAlignment(true);
            else
                graphProps.setGraphAllowComponentAlignment(false);
        }
        XgChart2D.repaint();
    }

    public void configureAsBarGraph() {
        GraphProperties graphProps = XgChart2D.getGraphProperties(0);

        XgProps.configureAsBarGraph();

        graphProps.setGraphBarsRoundingRatio(0f);

        graphProps.setGraphBarsExistence(true);
        graphProps.setGraphLinesExistence(false);
        graphProps.setGraphLinesFillInterior(false);
        graphProps.setGraphLinesThicknessModel(XgProps.lineThickness);
        graphProps.setGraphComponentsAlphaComposite(graphProps.ALPHA_COMPOSITE_MEDIUM);

        if (XgStackedData)
            graphProps.setGraphAllowComponentAlignment(true);
        else
            graphProps.setGraphAllowComponentAlignment(false);

        XgChart2D.repaint();
    }

    public void configureAsLineGraph() {
        GraphProperties graphProps = XgChart2D.getGraphProperties(0);

        XgProps.configureAsLineGraph();

        graphProps.setGraphBarsExistence(false);
        graphProps.setGraphLinesExistence(true);
        graphProps.setGraphLinesFillInterior(false);
        graphProps.setGraphLinesThicknessModel(XgProps.lineThickness);
        graphProps.setGraphAllowComponentAlignment(true);
        graphProps.setGraphComponentsAlphaComposite(graphProps.ALPHA_COMPOSITE_MEDIUM);

        XgChart2D.repaint();
    }

    public void configureAsAreaGraph() {
        GraphProperties graphProps = XgChart2D.getGraphProperties(0);

        XgProps.configureAsAreaGraph();

        graphProps.setGraphBarsExistence(false);
        graphProps.setGraphLinesExistence(true);
        graphProps.setGraphLinesFillInterior(true);
        graphProps.setGraphLinesThicknessModel(XgProps.lineThickness);
        graphProps.setGraphAllowComponentAlignment(true);
        graphProps.setGraphComponentsAlphaComposite(graphProps.ALPHA_COMPOSITE_MEDIUM);

        XgChart2D.repaint();
    }

    private class DatasetCollection extends Vector<XEDataset> {
        public DatasetCollection(int initialCapacity) {
            super(initialCapacity);
        }
    }

    /**
     * Provide access to the XgChart2D object.
     * @return The XgChart2D object.
     */
    public LBChart2D getChart() {
        return XgChart2D;
    }

    // get the current time and format it nicely
    private class MTime {
        Date date;

        public MTime() {
            date = new Date();
        }

        public String toString() {
            return MessageFormat.format("{0,time,HH:mm:ss}", date);
        }
    }

    /**
     * Constructor for the area graph object.
     * @param props Properties that define the look of the graph.
     */
    public XEGraph(XEGraphProperties props) {
        XgChart2D = new LBChart2D();
        setProperties(props);
    }

    /**
     * Set the properties of the area graph.
     * @param props Properties that define the look of the graph.
     */
    public void setProperties(XEGraphProperties props) {
        // Configure object properties
        Object2DProperties object2DProps = new Object2DProperties();
        object2DProps.setObjectTitleText(props.title);

        // Configure chart properties
        Chart2DProperties chart2DProps = new Chart2DProperties();
        chart2DProps.setChartDataLabelsPrecision(props.precision);

        // Configure graph chart properties
        GraphChart2DProperties graphChart2DProps = new GraphChart2DProperties();
        graphChart2DProps.setLabelsAxisTitleText(props.XAxisTitle);
        graphChart2DProps.setNumbersAxisTitleText(props.YAxisTitle);
        graphChart2DProps.setLabelsAxisTicksAlignment(graphChart2DProps.CENTERED);
        graphChart2DProps.setChartDatasetCustomizeLeastValue(true);
        graphChart2DProps.setChartDatasetCustomLeastValue((float) props.minValue);
        graphChart2DProps.setChartDatasetCustomizeGreatestValue(true);
        graphChart2DProps.setChartDatasetCustomGreatestValue((float) props.maxValue);
        XgXAxisLabels = new String[props.numDataPoints];
        XgXAxisLabels[0] = new MTime().toString();
        this.XgPointsCollected = 1;
        for(int i=1; i<props.numDataPoints; i++)
            XgXAxisLabels[i] = "00:00:00";
        graphChart2DProps.setLabelsAxisLabelsTexts(XgXAxisLabels);

        // Configure graph properties
        GraphProperties graphProps = new GraphProperties();
        if (props.isBarGraph) {
            graphProps.setGraphBarsRoundingRatio(0f);
            graphProps.setGraphOutlineComponentsExistence(true);
            XgStackedData = false;
        } else {
            graphProps.setGraphBarsExistence(false);
            graphProps.setGraphLinesExistence(props.isLineGraph);
            graphProps.setGraphLinesFillInterior(props.isAreaGraph);
            graphProps.setGraphOutlineComponentsExistence(true);
            graphProps.setGraphLinesThicknessModel(props.lineThickness);
            graphProps.setGraphAllowComponentAlignment(true);
            graphProps.setGraphComponentsAlphaComposite(graphProps.ALPHA_COMPOSITE_MEDIUM);
        }

        // Configure legend properties
        LegendProperties legendProps = new LegendProperties();
        legendProps.setLegendLabelsTexts(props.legendStrings);

        MultiColorsProperties multiColorsProps = new MultiColorsProperties();
        multiColorsProps.setColorsCustomize(true);
        multiColorsProps.setColorsCustom(props.legendColors);

        // Configure chart
        XgChart2D.setObject2DProperties(object2DProps);
        XgChart2D.setChart2DProperties(chart2DProps);
        XgChart2D.setGraphChart2DProperties(graphChart2DProps);
        XgChart2D.addGraphProperties(graphProps);
        XgChart2D.setLegendProperties(legendProps);
        XgChart2D.addMultiColorsProperties(multiColorsProps);

        Dataset dataset = new Dataset(props.numDataItems, props.numDataPoints, 1);
        XgChart2D.addDataset(dataset);

        XgProps = props;
        XgDatasetCollection = new DatasetCollection(props.numDataItems);
    }

    /**
     * Add a dataset to the area graph.
     * @param gp The graph point source.
     * @param userData Optional user data.
     * @return A new <CODE>XEDataset</CODE> object.
     */
    public XEDataset addElement(XEGraphPoint gp, Object userData) {
        XEDataset ds = new XEDataset(gp, userData);

        XgDatasetCollection.addElement(ds);
        return ds;
    }

    /**
     * Invoked at each time interval to update the graph.
     */
    public void GraphInterval() {
        if (XgPointsCollected == XgProps.numDataPoints) {
            for(int i=1; i<XgProps.numDataPoints; i++)
                XgXAxisLabels[i-1] = XgXAxisLabels[i];
            XgPointsCollected--;
        }

        XgXAxisLabels[XgPointsCollected++] = new MTime().toString();
        GraphChart2DProperties graphChart2DProps = XgChart2D.getGraphChart2DProperties();
        graphChart2DProps.setLabelsAxisLabelsTexts(XgXAxisLabels);

        Dataset dataset = XgChart2D.getDataset(0);
        int cats = dataset.getNumCats();
        int i = 0;
        int j;

        for(XEDataset ds : XgDatasetCollection) {
            ds.getNextPoint();
            float[] values = ds.getDataPoints();
            for(j=0; j<cats; j++)
                dataset.set(i, j, 0, values[j]);
            i++;
        }

        if (XgStackedData)
            dataset.doConvertToStacked();
        XgChart2D.addDataset(dataset);
        XgChart2D.setGraphChart2DProperties(graphChart2DProps);
        XgChart2D.repaint();
    }

    /**
     * The data point structure for the area graph.
     */
    public class XEDataset {
        /**
         * Callback for the current data point.
         */
        private XEGraphPoint  graphPoint;
        /**
         * Collection of data points.
         */
        private float[] dataPoints;
        /**
         * Number of points collected so far.
         */
        private int           pointCount;
        /**
         * User data for graphPoint.
         */
        private Object        userData;

        /**
         * The constructor for the class.
         * @param gp A XEGraphPoint callback to collect point data
         * @param userData passed in and then passed out to <CODE>graphPoint</CODE>
         * @returns a new <CODE>DataSet</CODE> object
         */
        public XEDataset(XEGraphPoint gp, Object userData) {
            graphPoint = gp;
            dataPoints = new float[XgProps.numDataPoints];
            pointCount = 0;
            this.userData = userData;
        }

        /**
         * Method that collects the next point and stores it.
         */
        public void getNextPoint() {
            float value = graphPoint.getPoint(userData);

            if (pointCount == XgProps.numDataPoints) {
                for(int i=1; i<XgProps.numDataPoints; i++)
                    dataPoints[i-1] = dataPoints[i];
                pointCount--;
            }
            dataPoints[pointCount++] = value;
        }

        /**
         * Obtain the collection of data points for this set.
         */
        public float[] getDataPoints() {
            return dataPoints;
        }
    }
}
