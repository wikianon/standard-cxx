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

import java.net.URL;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.border.BevelBorder;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import net.sourceforge.chart2d.PieChart2DProperties;
import com.capmet.metrics.cm.CmGlobal;
import com.capmet.metrics.cm.CmSysInfo;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.tools.XE.XEDialogs;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.CellConstraints;
import com.jgoodies.forms.builder.PanelBuilder;

/**
 * <CODE>XEMemoryPieChart</CODE> is the base graphical entity for displaying
 * the memory utilization of the target host in the form of a pie chart.
 * <CODE>XEMemoryPieChart</CODE> makes no assumptions about
 * whether it is an application or applet. It constructs
 * the graphical entity and makes it available from the
 * <CODE>getPane()</CODE> method.  This object extends <CODE>Timer</CODE>,
 * so after instantiation, a call to the <CODE>start</CODE> method
 * is necessary.
 *
 * <PRE>
 *   XEMemoryPieChart mp = new XEMemoryPieChart(parent, "tweedledum", 30000);
 *
 *   mp.start();
 * </PRE>
 */
public class XEMemoryPieChart extends Timer
{
    private final int MEMORY_FREE = 1;
    private final int MEMORY_USED = 2;

    private ClickablePieChart    MpPieChart;
    private CmGlobal<CmSysInfo>  MpGsysInfo;
    private CmSysInfo            MpSysInfo;
    private int                  MpInterval;    // in milliseconds
    private Container            MpParentFrame;
    private XEPieChartProperties MpProps;

    /**
     * The panel into which the layout manager places all of
     * the graphical objects. Returned by <CODE>getPane()</CODE>.
     */
    private Container piePane;

    //
    // public methods
    //

    public void erase() {
        MpPieChart.erase();
        MpPieChart = null;
        MpGsysInfo = null;
        MpSysInfo = null;
        MpParentFrame = null;
        MpProps = null;
    }

    /**
     * Retrieve the <CODE>Container</CODE> holding all of the entities.
     * @return The <CODE>Container</CODE> used by the layout manager where
     * all of the graphical entities are located.
     */
    public Container getPane()
    {
        return piePane;
    }

    /**
     * Override the <CODE>setDelay</CODE> method of <CODE>Timer</CODE>
     * so this object can track the interval changes. Sets the
     * <CODE>Timer</CODE> delay, the number of milliseconds
     * between successive action events.
     * @param delay The delay in milliseconds.
     */
    public void setDelay(int delay)
    {
        MpInterval = delay;
        super.setDelay(delay);
    }


    /**
     * Returns the delay, in milliseconds, 
     * between firings of action events.
     * @return The number of milliseconds of delay.
     */
    public int getDelay()
    {
        MpInterval = super.getDelay();
        return MpInterval;
    }

    /**
     * Retrieve the properties structure for this object.
     * @return The <CODE>XEPieChartProperties</CODE> containing the
     * properties of the pie chart.
     */
    public XEPieChartProperties getProperties()
    {
        MpProps.intervalSeconds = MpInterval / 1000;
        return MpProps;
    }

    /**
     * Set the pie graph properties.
     * @param props An <CODE>XEPieChartProperties</CODE> structure containing
     * the properties to be set.
     */
    public void setProperties(XEPieChartProperties props)
    {
        MpProps = props;
        MpInterval = props.intervalSeconds * 1000;
        MpPieChart.setProperties(props);
    }

    // The object passed to XEPieChart
    private class MemoryMetrics implements XEGraphPoint
    {
        public float getPoint(Object userData)
        {
            switch(((Integer) userData).intValue())
            {
            case MEMORY_FREE:
                // memory free is the first one into the vector
                try {
                    MpGsysInfo.refresh();
                } catch(Exception e) {
                    XEMemoryPieChart.this.stop();
                    XEDialogs.fatalDialog(new CmExceptionParser(e).getMessage());
                }
                return (float) MpSysInfo.memoryFreePercent.intValue();
            case MEMORY_USED:
                return (float) (100 - MpSysInfo.memoryFreePercent.intValue());
            default:
                return 0;
            }
        }
    }

    /**
     * Encapsulated logic for popping up a frame with a slider
     * control within it for changing the update interval.
     */
    private class IntervalListener implements ActionListener
    {
        private int         x;
        private int         y;
        private JFrame      sliderFrame = null;
        private JSlider     slider = null;
        private SliderMoved changeListener = null;
        private JLabel      label = null;
        private JButton     ok;

        public void erase() {
            if (slider != null)
                slider.removeChangeListener(changeListener);
            slider = null;

            if (sliderFrame != null) {
                sliderFrame.setVisible(false);
                sliderFrame.dispose();
            }
            sliderFrame = null;

            changeListener = null;
            label = null;

            if (ok != null)
                for(ActionListener ae : ok.getActionListeners())
                    ok.removeActionListener(ae);
            ok = null;
        }

        public void setXY(int x, int y)
        {
            this.x = x;
            this.y = y;
        }
        
        /**
         * The callback to process action events.
         * @param ae The action event in question.
         */
        public void actionPerformed(ActionEvent ae)
        {
            if (sliderFrame == null) {
                FormLayout layout = new FormLayout(
                        "c:p:g",
                        "t:p:g, 6dlu, c:p:g, 6dlu, b:p:g");
                
                PanelBuilder builder = new PanelBuilder(layout);
                CellConstraints cc = new CellConstraints();
                
                builder.setDefaultDialogBorder();
                
                sliderFrame = new JFrame(
                    XEMessages.msgs.getString("Set_Interval"));
                slider = new JSlider(
                    JSlider.HORIZONTAL, 0, 30, MpInterval / 1000);
                label = new JLabel(
                    XEMessages.msgs.getString("Int_Secs") + MpInterval / 1000);
                ok = new JButton(XEMessages.msgs.getString("OK"));
                
                slider.setMinorTickSpacing(1);
                slider.setMajorTickSpacing(5);
                slider.setPaintTicks(true);
                slider.setPaintLabels(true);
                slider.setLabelTable(slider.createStandardLabels(5));
                changeListener = new SliderMoved();
                slider.addChangeListener(changeListener);
                
                ok.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent ae) {
                        XEMemoryPieChart.this.stop();
                        XEMemoryPieChart.this.setDelay(MpInterval);
                        XEMemoryPieChart.this.start();
                        sliderFrame.setVisible(false);
                    }
                });
                ok.setToolTipText(XEMessages.msgs.getString("Press_Dismiss"));
                sliderFrame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
                
                builder.add(slider, cc.xy(1, 1));
                builder.add(label,  cc.xy(1, 3));
                builder.add(ok,     cc.xy(1, 5));
                
                sliderFrame.setContentPane(builder.getPanel());
                sliderFrame.pack();
            } else {
                label.setText(
                    XEMessages.msgs.getString("Int_Secs") + MpInterval / 1000);
                slider.setValue(MpInterval / 1000);
            }
            
            // pop open the window at the location of the pointer
            Rectangle cr = new Rectangle();
            cr = sliderFrame.getBounds(cr);
            cr.x = this.x;
            cr.y = this.y;
            sliderFrame.setBounds(cr);

            sliderFrame.setVisible(true);
        }
        
        /**
         * The handler subclass for dealing with slider events.
         */
        private class SliderMoved implements ChangeListener
        {
            /**
             * The callback method for dealing with state changes in the
             * slider control.
             * @param e The event which caused the state change.
             */
            public void stateChanged(ChangeEvent e)
            {
                MpInterval = slider.getValue() * 1000;

                if (MpInterval == 0)
                    MpInterval = 1000;
                
                label.setText(
                    XEMessages.msgs.getString("Int_Secs") + MpInterval / 1000);
            }
        }
    }

    /*
     * Encapsulated logic for a right click popup for the interval.
     */
    private class ClickablePieChart
        extends XEPieChart implements MouseListener
    {
        private JPopupMenu       pop;
        private JMenuItem        menuItem;
        private IntervalListener iList;

        public void erase() {
            menuItem.removeActionListener(iList);
            pop.remove(0);
            iList.erase();
            pop.setBorder(null);
            pop = null;
            menuItem = null;
            iList = null;
        }

        public ClickablePieChart(XEPieChartProperties props)
        {
            super(props);
            getChart().addMouseListener(this);
            pop = new JPopupMenu();
            menuItem = new JMenuItem(XEMessages.msgs.getString("Interval"), 'I');
            iList = new IntervalListener();
            menuItem.addActionListener(iList);
            pop.add(menuItem);
            pop.setBorder(new BevelBorder(BevelBorder.RAISED));
        }

        private void mouseDroppings(MouseEvent evt)
        {
            if (evt.isPopupTrigger()) {
                int x = evt.getX();
                int y = evt.getY();
                Point p = new Point(x, y);

                SwingUtilities.convertPointToScreen(p, this.getChart());

                iList.setXY(p.x, p.y);
                pop.show(super.getChart(), x, y);
            }
        }

        /**
         * Pointer has entered our space.
         * @param evt The MouseEvent
         */
        public void mouseEntered(MouseEvent evt)
        {
        }

        /**
         * Pointer has exited our space.
         * @param evt The MouseEvent
         */
        public void mouseExited(MouseEvent evt)
        {
        }

        /**
         * Mouse button has been pushed down.
         * @param evt The MouseEvent
         */
        public void mousePressed(MouseEvent evt)
        {
            mouseDroppings(evt);
        }

        /**
         * Mouse button has been released.
         * @param evt The MouseEvent
         */
        public void mouseReleased(MouseEvent evt)
        {
            mouseDroppings(evt);
        }

        /**
         * Mouse button has been pushed down and then released.
         * @param evt The MouseEvent
         */
        public void mouseClicked(MouseEvent evt)
        {
            mouseDroppings(evt);
        }
    }

    /**
     * Constructor for the memory pie chart object.
     * @param parent The object that this object will be subordinate to.
     * @param remoteHost The name of the host to monitor.
     * @param msInterval The interval between updates.
     * @param args Optional list of name=value strings for intializing the area graph.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public XEMemoryPieChart(Container parent, String remoteHost, int msInterval, Object ... args)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        super(msInterval, null);

        MpInterval = msInterval;
        MpProps = null;

        if (args.length > 0) {
            if (args[0] instanceof XEPieChartProperties) {
                MpProps = (XEPieChartProperties) args[0];
                if ((MpInterval / 1000) != MpProps.intervalSeconds) {
                    MpInterval = MpProps.intervalSeconds * 1000;
                    super.setDelay(MpInterval);
                }
            }
        }

        this.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                MpPieChart.GraphInterval();
            }
        });

        MpParentFrame = parent;
        MpGsysInfo = CmGlobal.getSysInfo(remoteHost);
        MpSysInfo = MpGsysInfo.getObject();

        if (MpProps == null) {
            String[] legendLabels;
            Color[] legendColors;

            legendLabels = new String[2];

            legendLabels[0] = XEMessages.msgs.getString("Memory_Free");
            legendLabels[1] = XEMessages.msgs.getString("Memory_Used");

            legendColors = new Color[2];

            legendColors[0] = Color.GREEN;
            legendColors[1] = Color.RED;

            MpProps = new XEPieChartProperties(legendLabels, legendColors);
            MpProps.title = XEMessages.msgs.getString("Mem_Title");
            MpProps.displayMode = PieChart2DProperties.PERCENT;
            MpProps.intervalSeconds = msInterval / 1000;
        }

        MpPieChart = new ClickablePieChart(MpProps);
        XEGraphPoint memMetrics = new MemoryMetrics();

        MpPieChart.addElement(memMetrics, new Integer(MEMORY_FREE));
        MpPieChart.addElement(memMetrics, new Integer(MEMORY_USED));

        MpPieChart.GraphInterval();
        piePane = MpPieChart.getChart();
    }
}
