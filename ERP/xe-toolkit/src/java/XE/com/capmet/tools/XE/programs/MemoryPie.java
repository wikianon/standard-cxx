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
import com.capmet.tools.XE.XEMemoryPieChart;
import com.capmet.tools.XE.XEMessages;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.CellConstraints;
import com.jgoodies.forms.builder.PanelBuilder;

/**
 * An application for monitoring the memory use on a host.<p>
 *
 * Use: java MemoryPie [-h remote-host]
 */
public class MemoryPie extends XEApplication
{
    private final int MP_DEFAULT_INTERVAL = 2000;
    private XEMemoryPieChart memPieChart;
    private JFrame memPieFrame;

    /**
     * The main method of the MemoryPie application.
     * @param args Arguments passed from the environment
     */
    public static void main(String[] args)
    {
        new MemoryPie(args);
    }

    // usage blurb
    private void usage()
    {
        System.out.println(XEMessages.msgs.getString("CpuPie_Usage")); 
    }

    /**
     * MemoryPie constructor.
     * @param args Arguments passed from the environment
     */
    public MemoryPie(String[] args)
    {
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
            String title = "MemoryPie: " + hostname;
            
            memPieFrame = new JFrame(title);

            memPieFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            final String ICON_PATH = "com/capmet/tools/XE/images/pie.png";
            URL imageURL = getClass().getClassLoader().getResource(ICON_PATH);
            ImageIcon memPieChartIcon = new ImageIcon(imageURL);
            memPieFrame.setIconImage(memPieChartIcon.getImage());

            // let the constructor do the heavy lifting
            memPieChart = new XEMemoryPieChart(memPieFrame, remoteHost, MP_DEFAULT_INTERVAL);

            JMenuBar menuBar = new JMenuBar();
            JMenu memPieChartMenu = new JMenu("MemoryPie");
            JMenuItem exitItem = new JMenuItem(XEMessages.msgs.getString("Exit"), 'X');
            exitItem.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    System.exit(0);
                }
            });
            memPieChartMenu.add(exitItem);

            JMenu helpMenu = new JMenu("Help");
            JMenuItem aboutItem = new JMenuItem(XEMessages.msgs.getString("About"), 'A');
            aboutItem.addActionListener(new AboutCallback());

            helpMenu.add(aboutItem);

            menuBar.add(memPieChartMenu);
            menuBar.add(helpMenu);
            
            memPieFrame.setJMenuBar(menuBar);
            memPieFrame.setContentPane(memPieChart.getPane());
            
            // reorganize
            memPieFrame.pack();
            
            // start the clock
            memPieChart.start();
            
            // go
            memPieFrame.setVisible(true);
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
            final String imagePath = "com/capmet/tools/XE/images/pie.png";
            final String msg =
                    "<html><c>MemoryPie v1.0<br>by Captive Metrics Software Corporation</c></html>";

            XEDialogs.aboutDialog(memPieFrame, msg, imagePath);
        }
    }
}
