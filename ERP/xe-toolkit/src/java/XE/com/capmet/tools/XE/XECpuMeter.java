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

import java.awt.*;
import java.awt.event.*;
import java.net.URL;
import java.util.Map;
import javax.swing.*;
import javax.swing.event.*;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.net.MalformedURLException;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.CellConstraints;
import com.jgoodies.forms.layout.ColumnSpec;
import com.jgoodies.forms.builder.PanelBuilder;
import com.jgoodies.forms.factories.FormFactory;
import com.capmet.metrics.cm.CmGlobal;
import com.capmet.metrics.cm.CmCpuStat;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.tools.XE.XEDialogs;

/**
 * <CODE>XECpuMeter</CODE> is the base graphical entity for displaying
 * the User, System, Wait and Idle time of the CPU(s) on
 * the target host in the form of progress bars. The Wait
 * time usage will be missing for hosts not accounting
 * for that time. <CODE>XECpuMeter</CODE> makes no assumptions about
 * whether it is an application or applet. It constructs
 * the graphical entity and makes it available from the
 * <CODE>getPane()</CODE> method.  This object extends <CODE>Timer</CODE>,
 * so after instantiation, a call to the <CODE>start</CODE> method
 * is necessary.
 *
 * <PRE>
 *   XECpuMeter cm = new XECpuMeter(parent, "tweedledum", 5000);
 *
 *   cm.start();
 * </PRE>
 */
public class XECpuMeter extends Timer
{
    /**
     * The interval between updates in milliseconds. The default
     * value is 2000, or 2 seconds.
     */
    private int           CmInterval;
    /**
     * The <CODE>CmCpuStat</CODE> class for retrieving the CPU performance
     * data from the target host.
     */
    private CmGlobal<CmCpuStat> CmCpu;
    /**
     * The panel into which the layout manager places all of
     * the graphical objects. Returned by <CODE>getPane()</CODE>.
     */
    private Container     CmPane;
    
    /**
     * The button with the label "User".
     */
    private JButton       userButton;
    /**
     * The button with the label "System".
     */
    private JButton       kernButton;
    /**
     * The button with the label "Wait".
     */
    private JButton       waitButton;
    /**
     * The button with the label "Idle".
     */
    private JButton       idleButton;
    
    /**
     * The user time progress bar.
     */
    private JProgressBar  userBar;
    /**
     * The system time progress bar.
     */
    private JProgressBar  kernBar;
    /**
     * The wait time progress bar.
     */
    private JProgressBar  waitBar;
    /**
     * The idle time progress bar.
     */
    private JProgressBar  idleBar;
    
    /**
     * The label displaying the percentage user time.
     */
    private JLabel        userLabel;
    /**
     * The label displaying the percentage system time.
     */
    private JLabel        kernLabel;
    /**
     * The label displaying the percentage wait time.
     */
    private JLabel        waitLabel;
    /**
     * The label displaying the percentage idle time.
     */
    private JLabel        idleLabel;
    
    /**
     * The "About..." popup frame.
     */
    private JFrame        aboutFrame;

    /**
     * The parent of XECpuMeter
     */
    private Container     parentFrame;

    /**
     * Constructor for the CPU meter object.
     * @param parent The object that this object will be subordinate to.
     * @param remoteHost The name of the host to monitor.
     * @param msInterval The interval between updates.
     * @param args Optional list of key=value strings for intializing the meter.<br>
     * <p>Possible key=value pairs are:
     * <ul>
     *   <li>orientation=
     *     <ul>
     *       <li>horizontal - horizontal bars</li>
     *       <li>vertical - vertical bars</li>
     *     </ul>
     *   </li>
     *   <li>exit=
     *     <ul>
     *       <li>enabled - U button exits application</li>
     *       <li>disabled - U button does not exit application</li>
     *     </ul>
     *   </li>
     * </ul>
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public XECpuMeter(Container parent, String remoteHost, int msInterval, Object ... args)
        throws NotBoundException, MalformedURLException, RemoteException,
                PerformanceException
    {
        super(msInterval, null);

        boolean vertical = false;
        boolean exitEnabled = true;

        CmInterval = msInterval;

        addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                try {
                    CmCpu.refresh();
                } catch(Exception e) {
                    XECpuMeter.this.stop();
                    XEDialogs.fatalDialog(new CmExceptionParser(e).getMessage());
                }
                
                userBar.setValue(CmCpu.getObject().userTimePercent.intValue());
                kernBar.setValue(CmCpu.getObject().systemTimePercent.intValue());
                if (CmCpu.getObject().waitTimePercent.isSupported())
                    waitBar.setValue(CmCpu.getObject().waitTimePercent.intValue());
                idleBar.setValue(CmCpu.getObject().idleTimePercent.intValue());
                
                userLabel.setText(percent(CmCpu.getObject().userTimePercent.intValue()));
                kernLabel.setText(percent(CmCpu.getObject().systemTimePercent.intValue()));
                if (CmCpu.getObject().waitTimePercent.isSupported())
                    waitLabel.setText(percent(CmCpu.getObject().waitTimePercent.intValue()));
                idleLabel.setText(percent(CmCpu.getObject().idleTimePercent.intValue()));
            }
        });

        parentFrame = parent;

        if (args.length > 0) {
            XEArgsParser ap = new XEArgsParser(args);

            for(Map.Entry<String, String> e : ap.entrySet()) {
                String key = e.getKey();
                String value = e.getValue();

                if (key.equals("orientation")) {
                    if (value.equals("vertical"))
                        vertical = true;
                    else if (value.equals("horizontal"))
                        vertical = false;
                } else if (key.equals("exit")) {
                    if (value.equals("disabled"))
                        exitEnabled = false;
                    else if (value.equals("enabled"))
                        exitEnabled = true;
                }
            }
        }

        // allocate data collectors for indicated host
        CmCpu = CmGlobal.getCpuStat(remoteHost);

        // make the GUI
        if (vertical)
            createVerticalMeter(exitEnabled);
        else
            createHorizontalMeter(exitEnabled);
        
        // set initial values
        userBar.setValue(CmCpu.getObject().userTimePercent.intValue());
        kernBar.setValue(CmCpu.getObject().systemTimePercent.intValue());
        if (CmCpu.getObject().waitTimePercent.isSupported())
            waitBar.setValue(CmCpu.getObject().waitTimePercent.intValue());
        idleBar.setValue(CmCpu.getObject().idleTimePercent.intValue());
        
        userLabel.setText(percent(CmCpu.getObject().userTimePercent.intValue()));
        kernLabel.setText(percent(CmCpu.getObject().systemTimePercent.intValue()));
        if (CmCpu.getObject().waitTimePercent.isSupported())
            waitLabel.setText(percent(CmCpu.getObject().waitTimePercent.intValue()));
        idleLabel.setText(percent(CmCpu.getObject().idleTimePercent.intValue()));
    }
    
    /**
     * Retrieve the <CODE>Container</CODE> holding all of the entities.
     * @return The <CODE>Container</CODE> used by the layout manager where
     * all of the graphical entities are located.
     */
    public Container getPane()
    {
        return CmPane;
    }
    
    /**
     * Create the panel with the progress bars in a vertical
     * orientation.
     */
    private void createVerticalMeter(final boolean exitEnabled)
    {
        String column_layout;
        
        if (CmCpu.getObject().waitTimePercent.isSupported())
            column_layout = "f:p:g, 0pt, f:p:g, 0pt, f:p:g, 0pt, f:p:g";
        else
            column_layout = "f:p:g, 0pt, f:p:g, 0pt, f:p:g";
        
        FormLayout layout = new FormLayout(
                column_layout,
                "p, 0pt, f:50dlu:g, 0pt, p");
        
        PanelBuilder builder = new PanelBuilder(layout);
        CellConstraints cc = new CellConstraints();
        
        builder.setDefaultDialogBorder();
        
        // create the buttons
        userButton = new JButton("U");
        userButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                if (exitEnabled)
                    System.exit(0);
            }
        });
        userButton.setToolTipText(XEMessages.msgs.getString("Press_Exit"));
        kernButton = new JButton("S");
        kernButton.addActionListener(new IntervalListener());
        kernButton.setToolTipText(XEMessages.msgs.getString("Press_Interval"));
        waitButton = null;
        if (CmCpu.getObject().waitTimePercent.isSupported()) {
            waitButton = new JButton("W");
            waitButton.setToolTipText(XEMessages.msgs.getString("Press_About"));
        }
        idleButton = new JButton("I");
        idleButton.setToolTipText(XEMessages.msgs.getString("Press_About"));

        AboutCallback aboutCallback = new AboutCallback();
        if (waitButton != null)
            waitButton.addActionListener(aboutCallback);
        idleButton.addActionListener(aboutCallback);
        
        // and the progress bars
        userBar = new JProgressBar(JProgressBar.VERTICAL, 0, 100);
        kernBar = new JProgressBar(JProgressBar.VERTICAL, 0, 100);
        if (CmCpu.getObject().waitTimePercent.isSupported())
            waitBar = new JProgressBar(JProgressBar.VERTICAL, 0, 100);
        idleBar = new JProgressBar(JProgressBar.VERTICAL, 0, 100);
        
        // use colored bars
        userBar.setForeground(Color.ORANGE);
        kernBar.setForeground(Color.RED);
        if (CmCpu.getObject().waitTimePercent.isSupported())
            waitBar.setForeground(Color.BLUE);
        idleBar.setForeground(Color.GREEN);
        
        // and the labels
        userLabel = new JLabel(percent(CmCpu.getObject().userTimePercent.intValue()));
        kernLabel = new JLabel(percent(CmCpu.getObject().systemTimePercent.intValue()));
        if (CmCpu.getObject().waitTimePercent.isSupported())
            waitLabel = new JLabel(percent(CmCpu.getObject().waitTimePercent.intValue()));
        idleLabel = new JLabel(percent(CmCpu.getObject().idleTimePercent.intValue()));
        
        builder.add(userButton, cc.xy(1, 1));
        builder.add(userBar,    cc.xy(1, 3));
        builder.add(userLabel,  cc.xy(1, 5));
        
        builder.add(kernButton, cc.xy(3, 1));
        builder.add(kernBar,    cc.xy(3, 3));
        builder.add(kernLabel,  cc.xy(3, 5));
        
        if (CmCpu.getObject().waitTimePercent.isSupported()) {
            builder.add(waitButton, cc.xy(5, 1));
            builder.add(waitBar,    cc.xy(5, 3));
            builder.add(waitLabel,  cc.xy(5, 5));
            
            builder.add(idleButton, cc.xy(7, 1));
            builder.add(idleBar,    cc.xy(7, 3));
            builder.add(idleLabel,  cc.xy(7, 5));
        } else {
            builder.add(idleButton, cc.xy(5, 1));
            builder.add(idleBar,    cc.xy(5, 3));
            builder.add(idleLabel,  cc.xy(5, 5));
        }
        
        // set default pane
        CmPane = builder.getPanel();
    }
    
    /**
     * Create the panel with the progress bars in a horizontal
     * orientation.
     */
    private void createHorizontalMeter(final boolean exitEnabled)
    {
        String row_layout;
        
        if (CmCpu.getObject().waitTimePercent.isSupported())
            row_layout = "f:p:g, 0pt, f:p:g, 0pt, f:p:g, 0pt, f:p:g";
        else
            row_layout = "f:p:g, 0pt, f:p:g, 0pt, f:p:g";
        
        FormLayout layout = new FormLayout(
                "p, 0pt, f:50dlu:g, 0pt, r:p",
                row_layout);
        
        PanelBuilder builder = new PanelBuilder(layout);
        CellConstraints cc = new CellConstraints();
        
        builder.setDefaultDialogBorder();
        
        // create the buttons
        userButton = new JButton(XEMessages.msgs.getString("User"));
        userButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                if (exitEnabled)
                    System.exit(0);
            }
        });
        userButton.setToolTipText(XEMessages.msgs.getString("Press_Exit"));
        kernButton = new JButton(XEMessages.msgs.getString("System"));
        kernButton.addActionListener(new IntervalListener());
        kernButton.setToolTipText(XEMessages.msgs.getString("Press_Interval"));
        waitButton = null;
        if (CmCpu.getObject().waitTimePercent.isSupported()) {
            waitButton = new JButton(XEMessages.msgs.getString("Wait"));
            waitButton.setToolTipText(XEMessages.msgs.getString("Press_About"));
        }
        idleButton = new JButton(XEMessages.msgs.getString("Idle"));
        idleButton.setToolTipText(XEMessages.msgs.getString("Press_About"));

        // and the progress bars
        userBar = new JProgressBar(JProgressBar.HORIZONTAL, 0, 100);
        kernBar = new JProgressBar(JProgressBar.HORIZONTAL, 0, 100);
        if (CmCpu.getObject().waitTimePercent.isSupported())
            waitBar = new JProgressBar(JProgressBar.HORIZONTAL, 0, 100);
        idleBar = new JProgressBar(JProgressBar.HORIZONTAL, 0, 100);

        AboutCallback aboutCallback = new AboutCallback();
        if (waitButton != null)
            waitButton.addActionListener(aboutCallback);
        idleButton.addActionListener(aboutCallback);
        
        // use colored bars
        userBar.setForeground(Color.ORANGE);
        kernBar.setForeground(Color.RED);
        if (CmCpu.getObject().waitTimePercent.isSupported())
            waitBar.setForeground(Color.BLUE);
        idleBar.setForeground(Color.GREEN);
        
        // and the labels
        userLabel = new JLabel(percent(CmCpu.getObject().userTimePercent.intValue()));
        kernLabel = new JLabel(percent(CmCpu.getObject().systemTimePercent.intValue()));
        if (CmCpu.getObject().waitTimePercent.isSupported())
            waitLabel = new JLabel(percent(CmCpu.getObject().waitTimePercent.intValue()));
        idleLabel = new JLabel(percent(CmCpu.getObject().idleTimePercent.intValue()));
        
        builder.add(userButton, cc.xy(1, 1));
        builder.add(userBar,    cc.xy(3, 1));
        builder.add(userLabel,  cc.xy(5, 1));
        
        builder.add(kernButton, cc.xy(1, 3));
        builder.add(kernBar,    cc.xy(3, 3));
        builder.add(kernLabel,  cc.xy(5, 3));
        
        if (CmCpu.getObject().waitTimePercent.isSupported()) {
            builder.add(waitButton, cc.xy(1, 5));
            builder.add(waitBar,    cc.xy(3, 5));
            builder.add(waitLabel,  cc.xy(5, 5));
            
            builder.add(idleButton, cc.xy(1, 7));
            builder.add(idleBar,    cc.xy(3, 7));
            builder.add(idleLabel,  cc.xy(5, 7));
        } else {
            builder.add(idleButton, cc.xy(1, 5));
            builder.add(idleBar,    cc.xy(3, 5));
            builder.add(idleLabel,  cc.xy(5, 5));
        }
        
        // set default pane
        CmPane = builder.getPanel();
    }

    /**
     * Encapsulated logic for popping up a frame with a slider
     * control within it for changing the update interval.
     */
    private class IntervalListener
        implements ActionListener, MouseListener
    {
        /**
         * The <CODE>JFrame</CODE> container which is popped up and down.
         */
        private JFrame sliderFrame;

        private JSlider slider;

        /**
         * The text "Interval in seconds: " under the slider control.
         */
        private JLabel label;

        /**
         * The default constructor.
         */
        public IntervalListener()
        {
            sliderFrame = null;
            kernButton.addMouseListener(this);
        }
        
        /**
         * The callback to process action events.
         * @param ae The action event in question.
         */
        public void actionPerformed(ActionEvent ae)
        {
            if (sliderFrame == null) {
                JButton ok;
                FormLayout layout = new FormLayout(
                        "c:p:g",
                        "t:p:g, 6dlu, c:p:g, 6dlu, b:p:g");
                
                PanelBuilder builder = new PanelBuilder(layout);
                CellConstraints cc = new CellConstraints();
                
                builder.setDefaultDialogBorder();
                
                sliderFrame = new JFrame(XEMessages.msgs.getString("Set_Interval"));
                slider = new JSlider(JSlider.HORIZONTAL, 0, 30, CmInterval / 1000);
                label = new JLabel(XEMessages.msgs.getString("Int_Secs") + CmInterval / 1000);
                ok = new JButton(XEMessages.msgs.getString("OK"));
                
                slider.setMinorTickSpacing(1);
                slider.setMajorTickSpacing(5);
                slider.setPaintTicks(true);
                slider.setPaintLabels(true);
                slider.setLabelTable(slider.createStandardLabels(5));
                slider.addChangeListener(new SliderMoved());
                
                ok.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent ae) {
                        XECpuMeter.this.stop();
                        XECpuMeter.this.setDelay(CmInterval);
                        XECpuMeter.this.start();
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
            }
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
                CmInterval = slider.getValue() * 1000;

                if (CmInterval == 0)
                    CmInterval = 1000;
                
                label.setText(
                    XEMessages.msgs.getString("Int_Secs") + CmInterval / 1000);
            }
        }

        // MouseListener methods

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
        }

        /**
         * Mouse button has been released.
         * @param evt The MouseEvent
         */
        public void mouseReleased(MouseEvent evt)
        {
        }

        /**
         * Mouse button has been pushed down and then released.
         * @param evt The MouseEvent
         */
        public void mouseClicked(MouseEvent evt)
        {
            int x = evt.getX();
            int y = evt.getY();
            Point p = new Point(x, y);

            SwingUtilities.convertPointToScreen(p, kernButton);

            Rectangle cr = new Rectangle();
            cr = sliderFrame.getBounds(cr);
            cr.x = p.x;
            cr.y = p.y;
            sliderFrame.setBounds(cr);

            sliderFrame.setVisible(true);
        }
    }
    
    /**
     * The encapsulation of logic for dealing with the About...
     * popup frame.
     */
    private class AboutCallback implements ActionListener, MouseListener
    {
        /**
         * The default constructor.
         */
        public AboutCallback()
        {
            aboutFrame = null;
            if (waitButton != null)
                waitButton.addMouseListener(this);
            idleButton.addMouseListener(this);
        }
        
        /**
         * The callback method for dealing with action events.
         * @param ae The action event in question.
         */
        public void actionPerformed(ActionEvent ae)
        {
            final String ICON_PATH = "com/capmet/tools/XE/images/ktimemon.png";

            if (aboutFrame == null) {
                aboutFrame = new JFrame("About");
                URL imageURL = getClass().getClassLoader().getResource(ICON_PATH);
                Icon cpuMeter = new ImageIcon(imageURL);
                JButton button = new JButton(cpuMeter);
                JLabel label = new JLabel(
                        "<html><c>CpuMeter v1.0<br>by Captive Metrics Software Corporation</c></html>");
                FormLayout layout = new FormLayout(
                        "f:p:n, 8dlu, c:p:g",
                        "c:p");
                
                PanelBuilder builder = new PanelBuilder(layout);
                CellConstraints cc = new CellConstraints();
                
                builder.setDefaultDialogBorder();
                
                button.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent ae) {
                        aboutFrame.setVisible(false);
                    }
                });
                button.setToolTipText(XEMessages.msgs.getString("Press_Dismiss"));
                aboutFrame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
                
                builder.add(button, cc.xy(1, 1));
                builder.add(label,  cc.xy(3, 1));
                
                aboutFrame.setContentPane(builder.getPanel());
                aboutFrame.pack();
            }
        }

        // MouseListener methods

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
        }

        /**
         * Mouse button has been released.
         * @param evt The MouseEvent
         */
        public void mouseReleased(MouseEvent evt)
        {
        }

        /**
         * Mouse button has been pushed down and then released.
         * @param evt The MouseEvent
         */
        public void mouseClicked(MouseEvent evt)
        {
            int x = evt.getX();
            int y = evt.getY();
            Point p = new Point(x, y);
            JButton b = (JButton) evt.getSource();

            SwingUtilities.convertPointToScreen(p, b);

            Rectangle cr = new Rectangle();
            cr = aboutFrame.getBounds(cr);
            cr.x = p.x;
            cr.y = p.y;

            aboutFrame.setBounds(cr);
            aboutFrame.setVisible(true);
        }
    }
    
    /**
     * Format a string containing the value followed by a "%".
     * The resulting string will always have 4 characters
     * depending on the magnitude of the number passed.
     * @param pct The value to format.
     * @return The formatted text as 4 characters depending
     * on how much space is used instead of digits.
     */
    private String percent(int pct)
    {
        Integer i = new Integer(pct);
        
        if (pct < 0)
            return " <0%";
        if (pct > 100)
            return ">100";
        
        if (pct > 9)
            if (pct > 99)
                return i.toString() + "%";
            else
                return " " + i.toString() + "%";
        else
            return "  " + i.toString() + "%";
    }
}
