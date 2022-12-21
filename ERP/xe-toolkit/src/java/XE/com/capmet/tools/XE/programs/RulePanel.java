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
import javax.swing.*;
import javax.swing.event.*;
import java.awt.event.*;
import java.net.InetAddress;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.tools.XE.GetOpt;
import com.capmet.tools.XE.XEMessages;
import com.capmet.tools.XE.XEApplication;
import com.capmet.tools.XE.XERulePanel;
import com.capmet.tools.XE.XEDialogs;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.CellConstraints;
import com.jgoodies.forms.builder.PanelBuilder;

public class RulePanel extends XEApplication {
    private JTabbedPane tabPane;
    private JFrame      frame;

    // default interval: 5 seconds
    private static final int INTERVAL = 5000;

    public static void main(final String[] args) {
        new RulePanel(args);
    }

    // usage blurb
    private void usage() {
        System.out.println(XEMessages.msgs.getString("RulePanel_Usage")); 
    }

    public RulePanel(String[] args) {
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
            String title = "RulePanel: " + hostname;
            
            frame = new JFrame(title);
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

            String ICON_PATH = null;
            URL imageURL = null;
            ImageIcon rulePanel = null;

            ICON_PATH = "com/capmet/tools/XE/images/ktimemon.png";
            imageURL = getClass().getClassLoader().getResource(ICON_PATH);
            if (imageURL != null)
                rulePanel = new ImageIcon(imageURL);

            if (rulePanel != null)
                frame.setIconImage(rulePanel.getImage());

            JMenuBar menuBar = new JMenuBar();
            JMenu appMenu = new JMenu(
                XEMessages.msgs.getString("RulePanel"));
            JMenuItem monitorItem = new JMenuItem(
                XEMessages.msgs.getString("Monitor"), 'M');
            JMenuItem exitItem = new JMenuItem(
                XEMessages.msgs.getString("Exit"), 'X');

            appMenu.add(monitorItem);
            appMenu.addSeparator();
            appMenu.add(exitItem);
            menuBar.add(appMenu);

            frame.setJMenuBar(menuBar);

            monitorItem.addActionListener(new AddHost());
            exitItem.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    System.exit(0);
                }
            });

            tabPane = new JTabbedPane();
            for(String host : hostname.split(","))
                addHostPanel(host);
            
            frame.setContentPane(tabPane);
            
            // reorganize
            frame.pack();
            
            // go
            frame.setVisible(true);
        } catch(Exception e) {
            XEDialogs.fatalDialog(frame, new CmExceptionParser(e).getMessage());
        }
    }

    private void addHostPanel(String host) {
        FormLayout layout = new FormLayout("c:d:n", "c:d:n, 6dlu, c:d:n");
        PanelBuilder builder = new PanelBuilder(layout);
        CellConstraints cc = new CellConstraints();
        JButton closeButton = new JButton(
            XEMessages.msgs.getString("Close"));

        try {
            XERulePanel rp = new XERulePanel(tabPane, host, INTERVAL);

            builder.add(rp.getPane(), cc.xy(1, 1, "f, f"));
            builder.add(closeButton,  cc.xy(1, 3));
            builder.setDefaultDialogBorder();

            JScrollPane sp = new JScrollPane(builder.getPanel());

            closeButton.addActionListener(new ClosePanel(sp));
            sp.getVerticalScrollBar().setUnitIncrement(10);
            tabPane.addTab(host, sp);
            rp.start();
        } catch(Exception e) {
            XEDialogs.warningDialog(new CmExceptionParser(e).getMessage());
        }
    }

    private class AddHost implements ActionListener {
        public void actionPerformed(ActionEvent ev) {
            String input = XEDialogs.inputDialog(frame,
                XEMessages.msgs.getString("Enter_Host"),
                    XEMessages.msgs.getString("New_Host"));

            if (input == null)
                return;

            for(String host : input.split(","))
                addHostPanel(host);
        }
    }

    private class ClosePanel implements ActionListener {
        private JScrollPane panel;

        public ClosePanel(JScrollPane panel) {
            this.panel = panel;
        }

        public void actionPerformed(ActionEvent ev) {
            tabPane.remove(panel);
            if (tabPane.getTabCount() == 0)
                System.exit(0);
        }
    }
}
