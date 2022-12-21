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
import java.util.StringTokenizer;
import java.util.Map;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.border.BevelBorder;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import com.capmet.metrics.cm.CmGlobal;
import com.capmet.metrics.cm.CmCpuStat;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.tools.XE.XEDialogs;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.CellConstraints;
import com.jgoodies.forms.builder.PanelBuilder;

/**
 * <CODE>XECpuGraph</CODE> is the base graphical entity for displaying
 * the User, System, Wait and Idle time of the CPU(s) on
 * the target host in the form of a stacked or unstacked area graph.
 * The Wait time usage will be missing for hosts not accounting
 * for that time. <CODE>XECpuGraph</CODE> makes no assumptions about
 * whether it is an application or applet. It constructs
 * the graphical entity and makes it available from the
 * <CODE>getPane()</CODE> method.  This object extends <CODE>Timer</CODE>,
 * so after instantiation, a call to the <CODE>start</CODE> method
 * is necessary.
 *
 * <PRE>
 *   XECpuGraph cg = new XECpuGraph(parent, "tweedledum", 5000);
 *
 *   cg.start();
 * </PRE>
 */
public class XECpuGraph extends Timer
{
    private final int USER_TIME = 1;
    private final int KERN_TIME = 2;
    private final int WAIT_TIME = 3;
    private final int IDLE_TIME = 4;

    private final int CPUGRAPH_INTERVAL = 2;
    private final int CPUGRAPH_DATAPOINTS = 5;

    private XEGraph               CgCpuGraph;
    private CmGlobal<CmCpuStat>   CgGCgCpuStat;
    private CmCpuStat             CgCpuStat;
    private int                   CgInterval;
    private Container             CgParentFrame;
    private XEGraphProperties     CgProps;

    /**
     * The panel into which the layout manager places all of
     * the graphical objects. Returned by <CODE>getPane()</CODE>.
     */
    private Container CgPane;

    /**
     * Set the graph type to stacked or unstacked.
     * @param stacked Boolean indication of stackedness.
     */
    public void setStacked(boolean stacked) {
        CgCpuGraph.setStacked(stacked);
    }

    public boolean isStacked() {
        return CgCpuGraph.isStacked();
    }

    public void configureAsBarGraph() {
        CgCpuGraph.configureAsBarGraph();
    }

    public void configureAsLineGraph() {
        CgCpuGraph.configureAsLineGraph();
    }

    public void configureAsAreaGraph() {
        CgCpuGraph.configureAsAreaGraph();
    }

    /**
     * Retrieve the <CODE>Container</CODE> holding all of the entities.
     * @return The <CODE>Container</CODE> used by the layout manager where
     * all of the graphical entities are located.
     */
    public Container getPane() {
        return CgPane;
    }

    /**
     * Override the <CODE>setDelay</CODE> method of <CODE>Timer</CODE>
     * so this object can track the interval changes. Sets the
     * <CODE>Timer</CODE> delay, the number of milliseconds
     * between successive action events.
     * @param delay The delay in milliseconds.
     */
    public void setDelay(int delay) {
        CgInterval = delay;
        super.setDelay(delay);
    }

    /**
     * Returns the delay, in milliseconds, 
     * between firings of action events.
     * @return The number of milliseconds of delay.
     */
    public int getDelay() {
        CgInterval = super.getDelay();
        return CgInterval;
    }

    /**
     * Retrieve the properties structure for this object.
     * @return The <CODE>XEGraphProperties</CODE> containing the
     * properties of the area graph.
     */
    public XEGraphProperties getProperties() {
        return CgProps;
    }

    /**
     * Set the area graph properties.
     * @param props An <CODE>XEGraphProperties</CODE> structure containing
     * the properties to be set.
     */
    public void setProperties(XEGraphProperties props) {
        CgProps = props;
        CgInterval = props.intervalSeconds * 1000;
        CgCpuGraph.setProperties(props);
    }

    // The object passed to XEGraph
    private class CpuMetrics implements XEGraphPoint {
        public float getPoint(Object userData)
        {
            switch(((Integer) userData).intValue())
            {
            case USER_TIME:
                // user time is the first one into the vector
                try {
                    CgGCgCpuStat.refresh();
                } catch(Exception e) {
                    XECpuGraph.this.stop();
                    XEDialogs.fatalDialog(new CmExceptionParser(e).getMessage());
                }
                return (float) CgCpuStat.userTimePercent.intValue();
            case KERN_TIME:
                return (float) CgCpuStat.systemTimePercent.intValue();
            case WAIT_TIME:
                return (float) CgCpuStat.waitTimePercent.intValue();
            case IDLE_TIME:
                return (float) CgCpuStat.idleTimePercent.intValue();
            default:
                return 0;
            }
        }
    }

    /**
     * Encapsulated logic for popping up a frame with a slider
     * control within it for changing the update interval.
     */
    private class IntervalListener implements ActionListener {
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

        public void setXY(int x, int y) {
            this.x = x;
            this.y = y;
        }
        
        /**
         * The callback to process action events.
         * @param ae The action event in question.
         */
        public void actionPerformed(ActionEvent ae) {
            if (sliderFrame == null) {
                JButton ok;
                FormLayout layout = new FormLayout(
                        "c:p:g",
                        "t:p:g, 6dlu, c:p:g, 6dlu, b:p:g");
                
                PanelBuilder builder = new PanelBuilder(layout);
                CellConstraints cc = new CellConstraints();
                
                builder.setDefaultDialogBorder();
                
                sliderFrame = new JFrame(
                    XEMessages.msgs.getString("Set_Interval"));
                slider = new JSlider(
                    JSlider.HORIZONTAL, 0, 30, CgInterval / 1000);
                label = new JLabel(
                    XEMessages.msgs.getString("Int_Secs") + CgInterval / 1000);
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
                        XECpuGraph.this.stop();
                        XECpuGraph.this.setDelay(CgInterval);
                        XECpuGraph.this.start();
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
                    XEMessages.msgs.getString("Int_Secs") + CgInterval / 1000);
                slider.setValue(CgInterval / 1000);
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
        private class SliderMoved implements ChangeListener {
            /**
             * The callback method for dealing with state changes in the
             * slider control.
             * @param e The event which caused the state change.
             */
            public void stateChanged(ChangeEvent e) {
                CgInterval = slider.getValue() * 1000;

                if (CgInterval == 0)
                    CgInterval = 1000;
                
                label.setText(
                    XEMessages.msgs.getString("Int_Secs") + CgInterval / 1000);
            }
        }
    }

    /*
     * Encapsulated logic for a right click popup for the interval.
     */
    private class ClickableGraph extends XEGraph implements MouseListener {
        private JPopupMenu       pop;
        private JMenuItem        intervalItem;
        private IntervalListener iList;

        public void erase() {
            intervalItem.removeActionListener(iList);
            pop.remove(0);
            iList.erase();
            pop.setBorder(null);
            pop = null;
            intervalItem = null;
            iList = null;
        }

        public ClickableGraph(XEGraphProperties props) {
            super(props);
            getChart().addMouseListener(this);
            pop = new JPopupMenu();
            intervalItem = new JMenuItem(XEMessages.msgs.getString("Interval"), 'I');
            iList = new IntervalListener();
            intervalItem.addActionListener(iList);
            pop.add(intervalItem);
            pop.setBorder(new BevelBorder(BevelBorder.RAISED));
        }

        private void mouseDroppings(MouseEvent evt) {
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
        public void mouseEntered(MouseEvent evt) {
        }

        /**
         * Pointer has exited our space.
         * @param evt The MouseEvent
         */
        public void mouseExited(MouseEvent evt) {
        }

        /**
         * Mouse button has been pushed down.
         * @param evt The MouseEvent
         */
        public void mousePressed(MouseEvent evt) {
            mouseDroppings(evt);
        }

        /**
         * Mouse button has been released.
         * @param evt The MouseEvent
         */
        public void mouseReleased(MouseEvent evt) {
            mouseDroppings(evt);
        }

        /**
         * Mouse button has been pushed down and then released.
         * @param evt The MouseEvent
         */
        public void mouseClicked(MouseEvent evt) {
            mouseDroppings(evt);
        }
    }

    /**
     * Constructor for the CPU graph object.
     * @param parent The object that this object will be subordinate to.
     * @param remoteHost The name of the host to monitor.
     * @param msInterval The interval between updates.
     * @param args Optional list of key=value strings for intializing the area graph.<br>
     * <p>Possible key=value pairs are:
     * <ul>
     *   <li>title=
     *     <ul>
     *       <li>A title for the top of the graph.</li>
     *     </ul>
     *   </li>
     *   <li>XAxisTitle=
     *     <ul>
     *       <li>A title for the X axis</li>
     *     </ul>
     *   </li>
     *   <li>YAxisTitle=
     *     <ul>
     *       <li>A title for the Y axis</li>
     *     </ul>
     *   </li>
     *   <li>numDataPoints=
     *     <ul>
     *       <li>How many data points to show on the graph (type: <CODE>int</CODE>).</li>
     *     </ul>
     *   </li>
     *   <li>minValue=
     *     <ul>
     *       <li>The minimum value for the Y axis (type: <CODE>int</CODE>).</li>
     *     </ul>
     *   </li>
     *   <li>maxValue=
     *     <ul>
     *       <li>The maximum value for the Y axis (type: <CODE>int</CODE>).</li>
     *     </ul>
     *   </li>
     * </ul>
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public XECpuGraph(Container parent, String remoteHost, int msInterval, Object ... args)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        super(msInterval, null);

        this.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                CgCpuGraph.GraphInterval();
            }
        });

        String[] legendLabels;
        Color[] legendColors;
        int numCounters = 3;
        int n;

        CgParentFrame = parent;
        CgGCgCpuStat = CmGlobal.getCpuStat(remoteHost);
        CgCpuStat = CgGCgCpuStat.getObject();

        if (CgCpuStat.waitTimePercent.isSupported())
            numCounters++;

        legendLabels = new String[numCounters];

        n = 0;
        legendLabels[n++] = XEMessages.msgs.getString("User_Time");
        legendLabels[n++] = XEMessages.msgs.getString("System_Time");
        if (CgCpuStat.waitTimePercent.isSupported())
            legendLabels[n++] = XEMessages.msgs.getString("Wait_Time");
        legendLabels[n++] = XEMessages.msgs.getString("Idle_Time");

        legendColors = new Color[numCounters];

        n = 0;
        legendColors[n++] = Color.ORANGE;
        legendColors[n++] = Color.RED;
        if (CgCpuStat.waitTimePercent.isSupported())
            legendColors[n++] = Color.BLUE;
        legendColors[n++] = Color.GREEN;

        CgProps = new XEGraphProperties(legendLabels, legendColors);
        CgProps.title = XEMessages.msgs.getString("CPU_Title");
        CgProps.XAxisTitle = XEMessages.msgs.getString("Time");
        CgProps.YAxisTitle = XEMessages.msgs.getString("Percentage");
        CgProps.intervalSeconds = CPUGRAPH_INTERVAL;
        CgProps.numDataPoints = CPUGRAPH_DATAPOINTS;

        CgInterval = CPUGRAPH_INTERVAL * 1000;

        if (args.length > 0) {
            XEArgsParser ap = new XEArgsParser(args);

            for(Map.Entry<String, String> e : ap.entrySet()) {
                String key = e.getKey();
                String value = e.getValue();

                if (key.equals("title"))
                    CgProps.title = value;
                else if (key.equals("XAxisTitle"))
                    CgProps.XAxisTitle = value;
                else if (key.equals("YAxisTitle"))
                    CgProps.YAxisTitle = value;
                else if (key.equals("numDataPoints"))
                    CgProps.numDataPoints = Integer.parseInt(value);
                else if (key.equals("minValue"))
                    CgProps.minValue = Integer.parseInt(value);
                else if (key.equals("maxValue"))
                    CgProps.maxValue = Integer.parseInt(value);
                else if (key.equals("graphType")) {
                    if (value.equals("area"))
                        CgProps.configureAsAreaGraph();
                    else if (value.equals("line"))
                        CgProps.configureAsLineGraph();
                    else if (value.equals("bar"))
                        CgProps.configureAsBarGraph();
                }
            }
        }

        CgCpuGraph = new ClickableGraph(CgProps);
        XEGraphPoint cpuMetrics = new CpuMetrics();

        CgCpuGraph.addElement(cpuMetrics, new Integer(USER_TIME));
        CgCpuGraph.addElement(cpuMetrics, new Integer(KERN_TIME));
        if (CgCpuStat.waitTimePercent.isSupported())
            CgCpuGraph.addElement(cpuMetrics, new Integer(WAIT_TIME));
        CgCpuGraph.addElement(cpuMetrics, new Integer(IDLE_TIME));

        CgCpuGraph.GraphInterval();
        CgPane = CgCpuGraph.getChart();
    }
}
