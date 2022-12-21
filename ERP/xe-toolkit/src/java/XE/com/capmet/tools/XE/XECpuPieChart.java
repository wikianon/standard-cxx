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
import com.capmet.metrics.cm.CmCpuStat;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.tools.XE.XEDialogs;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.CellConstraints;
import com.jgoodies.forms.builder.PanelBuilder;

/**
 * <CODE>XECpuPieChart</CODE> is the base graphical entity for displaying
 * the User, System, Wait and Idle time of the CPU(s) on
 * the target host in the form of a pie chart.
 * The Wait time usage will be missing for hosts not accounting
 * for that time. <CODE>XECpuPieChart</CODE> makes no assumptions about
 * whether it is an application or applet. It constructs
 * the graphical entity and makes it available from the
 * <CODE>getPane()</CODE> method.  This object extends <CODE>Timer</CODE>,
 * so after instantiation, a call to the <CODE>start</CODE> method
 * is necessary.
 *
 * <PRE>
 *   XECpuPieChart cp = new XECpuPieChart(parent, "tweedledum", 5000);
 *
 *   cp.start();
 * </PRE>
 */
public class XECpuPieChart extends Timer
{
    private final int USER_TIME = 1;
    private final int KERN_TIME = 2;
    private final int WAIT_TIME = 3;
    private final int IDLE_TIME = 4;

    // members in support of the class
    private ClickablePieChart    CpCpuPieChart;
    private CmGlobal<CmCpuStat>  CpGcpuStat;
    private CmCpuStat            CpCpuStat;
    private int                  CpInterval;
    private Container            CpParentFrame;
    private XEPieChartProperties CpProps;

    /**
     * The panel into which the layout manager places all of
     * the graphical objects. Returned by <CODE>getPane()</CODE>.
     */
    private Container CpPane;

    //
    // public methods
    //

    public void erase() {
        CpCpuPieChart.erase();
        CpCpuPieChart = null;
        CpGcpuStat = null;
        CpCpuStat = null;
        CpParentFrame = null;
        CpProps = null;
    }

    /**
     * Retrieve the <CODE>Container</CODE> holding all of the entities.
     * @return The <CODE>Container</CODE> used by the layout manager where
     * all of the graphical entities are located.
     */
    public Container getPane()
    {
        return CpPane;
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
        CpInterval = delay;
        super.setDelay(delay);
    }

    /**
     * Returns the delay, in milliseconds, 
     * between firings of action events.
     * @return The number of milliseconds of delay.
     */
    public int getDelay()
    {
        CpInterval = super.getDelay();
        return CpInterval;
    }

    /**
     * Retrieve the properties structure for this object.
     * @return The <CODE>XEPieChartProperties</CODE> containing the
     * properties of the pie chart.
     */
    public XEPieChartProperties getProperties()
    {
        CpProps.intervalSeconds = CpInterval / 1000;
        return CpProps;
    }

    // The object passed to XEPieChart
    private class CpuMetrics implements XEGraphPoint
    {
        public float getPoint(Object userData)
        {
            switch(((Integer) userData).intValue())
            {
            case USER_TIME:
                // user time is the first one into the vector
                try {
                    CpGcpuStat.refresh();
                } catch(Exception e) {
                    XECpuPieChart.this.stop();
                    XEDialogs.fatalDialog(new CmExceptionParser(e).getMessage());
                }
                return (float) CpCpuStat.userTimePercent.intValue();
            case KERN_TIME:
                return (float) CpCpuStat.systemTimePercent.intValue();
            case WAIT_TIME:
                return (float) CpCpuStat.waitTimePercent.intValue();
            case IDLE_TIME:
                return (float) CpCpuStat.idleTimePercent.intValue();
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
                    JSlider.HORIZONTAL, 0, 30, CpInterval / 1000);
                label = new JLabel(
                    XEMessages.msgs.getString("Int_Secs") + CpInterval / 1000);
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
                        XECpuPieChart.this.stop();
                        XECpuPieChart.this.setDelay(CpInterval);
                        XECpuPieChart.this.start();
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
                    XEMessages.msgs.getString("Int_Secs") + CpInterval / 1000);
                slider.setValue(CpInterval / 1000);
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
                CpInterval = slider.getValue() * 1000;

                if (CpInterval == 0)
                    CpInterval = 1000;
                
                label.setText(
                    XEMessages.msgs.getString("Int_Secs") + CpInterval / 1000);
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
     * Constructor for the CPU pie chart object.
     * @param parent The object that this object will be subordinate to.
     * @param remoteHost The name of the host to monitor.
     * @param msInterval The interval between updates.
     * @param args Optional list of name=value strings for intializing the area graph.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public XECpuPieChart(Container parent, String remoteHost, int msInterval, Object ... args)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        super(msInterval, null);

        CpInterval = msInterval;
        CpProps = null;

        if (args.length > 0) {
            if (args[0] instanceof XEPieChartProperties) {
                CpProps = (XEPieChartProperties) args[0];
                if ((CpInterval / 1000) != CpProps.intervalSeconds) {
                    CpInterval = CpProps.intervalSeconds * 1000;
                    super.setDelay(CpInterval);
                }
            }
        }

        this.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                CpCpuPieChart.GraphInterval();
            }
        });

        CpGcpuStat = CmGlobal.getCpuStat(remoteHost);
        CpCpuStat = CpGcpuStat.getObject();
        CpParentFrame = parent;

        if (CpProps == null) {
            String[] legendLabels;
            Color[] legendColors;
            int numCounters = 3;
            int n;

            if (CpCpuStat.waitTimePercent.isSupported())
                numCounters++;

            legendLabels = new String[numCounters];

            n = 0;
            legendLabels[n++] = XEMessages.msgs.getString("User_Time");
            legendLabels[n++] = XEMessages.msgs.getString("System_Time");
            if (CpCpuStat.waitTimePercent.isSupported())
                legendLabels[n++] = XEMessages.msgs.getString("Wait_Time");
            legendLabels[n++] = XEMessages.msgs.getString("Idle_Time");

            legendColors = new Color[numCounters];

            n = 0;
            legendColors[n++] = Color.ORANGE;
            legendColors[n++] = Color.RED;
            if (CpCpuStat.waitTimePercent.isSupported())
                legendColors[n++] = Color.BLUE;
            legendColors[n++] = Color.GREEN;

            CpProps = new XEPieChartProperties(legendLabels, legendColors);
            CpProps.title = XEMessages.msgs.getString("CPU_Title");
            CpProps.precision = -1;
            CpProps.displayMode = PieChart2DProperties.PERCENT;
            CpProps.intervalSeconds = msInterval / 1000;
        }

        CpCpuPieChart = new ClickablePieChart(CpProps);
        XEGraphPoint cpuMetrics = new CpuMetrics();

        XEPieChart.XEDataset wait = null;
        XEPieChart.XEDataset user =
            CpCpuPieChart.addElement(cpuMetrics, new Integer(USER_TIME));
        XEPieChart.XEDataset kern =
            CpCpuPieChart.addElement(cpuMetrics, new Integer(KERN_TIME));
        if (CpCpuStat.waitTimePercent.isSupported())
            wait = CpCpuPieChart.addElement(cpuMetrics, new Integer(WAIT_TIME));
        XEPieChart.XEDataset idle =
            CpCpuPieChart.addElement(cpuMetrics, new Integer(IDLE_TIME));

        CpCpuPieChart.GraphInterval();
        CpPane = CpCpuPieChart.getChart();
    }
}
