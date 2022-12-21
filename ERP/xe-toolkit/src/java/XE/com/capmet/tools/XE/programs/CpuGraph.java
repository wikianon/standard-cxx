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

import java.net.URL;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.net.InetAddress;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.tools.XE.GetOpt;
import com.capmet.tools.XE.XEDialogs;
import com.capmet.tools.XE.XEApplication;
import com.capmet.tools.XE.XECpuGraph;
import com.capmet.tools.XE.XEMessages;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.CellConstraints;
import com.jgoodies.forms.builder.PanelBuilder;

/**
 * An application for monitoring the CPUs on a host.<p>
 *
 * Use: java CpuGraph [-h remote-host]
 */
public class CpuGraph extends XEApplication {
    private JFrame     cpuFrame;
    private XECpuGraph cpuGraph;
    private final int CG_DEFAULT_INTERVAL = 2000;
    
    /**
     * The main method of the CpuGraph application.
     * @param args Arguments passed from the environment
     */
    public static void main(String[] args) {
        new CpuGraph(args);
    }
    
    // usage blurb
    private void usage() {
        System.out.println(XEMessages.msgs.getString("CpuGraph_Usage"));
    }
    
    private enum GraphType { BAR, LINE, AREA };

    /**
     * CpuGraph constructor.
     * @param args Arguments passed from the environment
     */
    public CpuGraph(String[] args) {
        try {
            // whether localhost or remotehost
            String remoteHost = null;
            String hostname = null;
            String graphTypeName = "area";
            GetOpt opts = new GetOpt(args, "h:p:abl");
            int c;
            int points = 5;
            GraphType graphType = GraphType.AREA;
            
            opts.optErr = false;
            while((c = opts.getopt()) != opts.optEOF) {
                switch(c) {
                    case 'a':
                        break;
                    case 'b':
                        graphType = GraphType.BAR;
                        graphTypeName = "bar";
                        break;
                    case 'l':
                        graphType = GraphType.LINE;
                        graphTypeName = "line";
                        break;
                    case 'h':
                        remoteHost = opts.getOptArg();
                        break;
                    case 'p':
                        points = Integer.parseInt(opts.getOptArg());
                        break;
                    default:
                        usage();
                        return;
                }
            }
            
            // if localhost, use the actual name of the host
            if (remoteHost == null) {
                hostname = InetAddress.getLocalHost().getHostName();
                int dot = hostname.indexOf(".");
                if (dot != -1)
                    hostname = hostname.substring(0, dot);
            } else
                hostname = remoteHost;
            
            // for JFrame
            String title = "CpuGraph: " + hostname;
            
            cpuFrame = new JFrame(title);
            
            cpuFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            final String ICON_PATH = "com/capmet/tools/XE/images/kchart.png";
            URL imageURL = getClass().getClassLoader().getResource(ICON_PATH);
            ImageIcon cgIcon = null;
            if (imageURL != null)
                cgIcon = new ImageIcon(imageURL);
            if (cgIcon != null)
                cpuFrame.setIconImage(cgIcon.getImage());
            
            // let the constructor do the heavy lifting
            String pointsArg = "numDataPoints=" + points;
            String graphArg = "graphType=" + graphTypeName;
            cpuGraph = new XECpuGraph(
                cpuFrame, remoteHost, CG_DEFAULT_INTERVAL,
                    pointsArg + "," + graphArg);
            
            JMenuBar menuBar = new JMenuBar();
            JMenu cpuGraphMenu = new JMenu("CpuGraph");
            JRadioButtonMenuItem barRadio = new JRadioButtonMenuItem(
                XEMessages.msgs.getString("Bar_Graph"));
            JRadioButtonMenuItem lineRadio = new JRadioButtonMenuItem(
                XEMessages.msgs.getString("Line_Graph"));
            JRadioButtonMenuItem areaRadio = new JRadioButtonMenuItem(
                XEMessages.msgs.getString("Area_Graph"));
            ButtonGroup buttonGroup = new ButtonGroup();
            buttonGroup.add(barRadio);
            buttonGroup.add(lineRadio);
            buttonGroup.add(areaRadio);
            barRadio.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    cpuGraph.configureAsBarGraph();
                }
            });
            lineRadio.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    cpuGraph.configureAsLineGraph();
                }
            });
            areaRadio.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    cpuGraph.configureAsAreaGraph();
                }
            });
            switch(graphType) {
            case BAR:
                barRadio.setSelected(true);
                break;
            case LINE:
                lineRadio.setSelected(true);
                break;
            case AREA:
                areaRadio.setSelected(true);
                break;
            }
            JCheckBoxMenuItem stackedItem =
                new JCheckBoxMenuItem(
                    XEMessages.msgs.getString("Stacked_Data"),
                        cpuGraph.isStacked());
            JMenuItem exitItem = new JMenuItem(
                    XEMessages.msgs.getString("Exit"), 'X');
            
            class StackToggle implements ActionListener {
                private boolean stackedData = cpuGraph.isStacked();
                
                public void actionPerformed(ActionEvent ae) {
                    stackedData = !stackedData;
                    cpuGraph.setStacked(stackedData);
                }
            }
            
            stackedItem.addActionListener(new StackToggle());
            exitItem.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    System.exit(0);
                }
            });
            
            cpuGraphMenu.add(stackedItem);
            cpuGraphMenu.addSeparator();
            cpuGraphMenu.add(barRadio);
            cpuGraphMenu.add(lineRadio);
            cpuGraphMenu.add(areaRadio);
            cpuGraphMenu.addSeparator();
            cpuGraphMenu.add(exitItem);
            
            JMenu helpMenu = new JMenu(XEMessages.msgs.getString("Help"));
            JMenuItem aboutItem = new JMenuItem(XEMessages.msgs.getString("About"), 'A');
            aboutItem.addActionListener(new AboutCallback());
            
            helpMenu.add(aboutItem);
            
            menuBar.add(cpuGraphMenu);
            menuBar.add(helpMenu);
            
            cpuFrame.setJMenuBar(menuBar);
            cpuFrame.setContentPane(cpuGraph.getPane());
            
            // reorganize
            cpuFrame.pack();
            
            // start the clock
            cpuGraph.start();
            
            // go
            cpuFrame.setVisible(true);
        } catch(Exception e) {
            System.err.println(new CmExceptionParser(e).getMessage());
        }
    }
    
    /**
     * The encapsulation of logic for dealing with the About...
     * popup frame.
     */
    private class AboutCallback implements ActionListener {
        /**
         * The callback method for dealing with action events.
         * @param ae The action event in question.
         */
        public void actionPerformed(ActionEvent ae) {
            final String imagePath = "com/capmet/tools/XE/images/kchart.png";
            final String msg =
                    "<html><c>CpuGraph v1.0<br>by Captive Metrics Software Corporation</c></html>";
            
            XEDialogs.aboutDialog(cpuFrame, msg, imagePath);
        }
    }
}
