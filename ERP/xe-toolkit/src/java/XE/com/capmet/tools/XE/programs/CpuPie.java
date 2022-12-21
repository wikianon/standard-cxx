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
import javax.swing.*;
import javax.swing.event.*;
import java.awt.event.*;
import java.net.InetAddress;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.tools.XE.GetOpt;
import com.capmet.tools.XE.XEDialogs;
import com.capmet.tools.XE.XEApplication;
import com.capmet.tools.XE.XECpuPieChart;
import com.capmet.tools.XE.XEMessages;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.CellConstraints;
import com.jgoodies.forms.builder.PanelBuilder;

/**
 * An application for monitoring the CPUs on a host.<p>
 *
 * Use: java CpuPie [-h remote-host]
 */
public class CpuPie extends XEApplication {
    private final int CP_DEFAULT_INTERVAL = 2000;
    
    private JFrame        cpuPieFrame;
    private XECpuPieChart cpuPieChart;
    
    /**
     * The main method of the CpuPie application.
     * @param args Arguments passed from the environment
     */
    public static void main(String[] args) {
        new CpuPie(args);
    }
    
    // usage blurb
    private void usage() {
        System.out.println(XEMessages.msgs.getString("CpuPie_Usage"));
    }
    
    /**
     * CpuPie constructor.
     * @param args Arguments passed from the environment
     */
    public CpuPie(String[] args) {
        try {
            // whether localhost or remotehost
            String remoteHost = null;
            String hostname = null;
            GetOpt opts = new GetOpt(args, "h:");
            int c;
            
            opts.optErr = false;
            while((c = opts.getopt()) != opts.optEOF) {
                switch(c) {
                    case 'h':
                        remoteHost = opts.getOptArg();
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
            String title = "CpuPie: " + hostname;
            
            cpuPieFrame = new JFrame(title);
            
            cpuPieFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            final String ICON_PATH = "com/capmet/tools/XE/images/pie.png";
            URL imageURL = getClass().getClassLoader().getResource(ICON_PATH);
            ImageIcon cpuGraph = new ImageIcon(imageURL);
            cpuPieFrame.setIconImage(cpuGraph.getImage());
            
            // let the constructor do the heavy lifting
            cpuPieChart = new XECpuPieChart(
                    cpuPieFrame, remoteHost, CP_DEFAULT_INTERVAL);
            
            JMenuBar menuBar = new JMenuBar();
            JMenu cpuPieChartMenu = new JMenu("CpuPie");
            JMenuItem exitItem = new JMenuItem(XEMessages.msgs.getString("Exit"), 'X');
            exitItem.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    System.exit(0);
                }
            });
            cpuPieChartMenu.add(exitItem);
            
            JMenu helpMenu = new JMenu("Help");
            JMenuItem aboutItem = new JMenuItem(XEMessages.msgs.getString("About"), 'A');
            aboutItem.addActionListener(new AboutListener());
            
            helpMenu.add(aboutItem);
            
            menuBar.add(cpuPieChartMenu);
            menuBar.add(helpMenu);
            
            cpuPieFrame.setJMenuBar(menuBar);
            cpuPieFrame.setContentPane(cpuPieChart.getPane());
            
            // reorganize
            cpuPieFrame.pack();
            
            // start the clock
            cpuPieChart.start();
            
            // go
            cpuPieFrame.setVisible(true);
        } catch(Exception e) {
            System.err.println(new CmExceptionParser(e).getMessage());
        }
    }
    
    /**
     * The encapsulation of logic for dealing with the About...
     * popup frame.
     */
    private class AboutListener implements ActionListener {
        /**
         * The About... frame.
         */
        private JFrame aboutFrame;
        
        /**
         * The default constructor.
         */
        public AboutListener() {
            aboutFrame = null;
        }
        
        /**
         * The callback method for dealing with action events.
         * @param ae The action event in question.
         */
        public void actionPerformed(ActionEvent ae) {
            final String imagePath = "com/capmet/tools/XE/images/pie.png";
            final String msg =
                    "<html><c>CpuPie v1.0<br>by Captive Metrics Software Corporation</c></html>";
            
            XEDialogs.aboutDialog(cpuPieFrame, msg, imagePath);
        }
    }
}
