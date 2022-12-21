/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector application of Captivity.
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

package com.capmet.Captivity.CaptiveCollector;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.border.TitledBorder;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.InetAddress;
import java.net.URL;
import java.net.MalformedURLException;
import java.net.UnknownHostException;
import java.rmi.server.UID;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.ObjectInputStream;
import java.io.IOException;
import java.util.Vector;
import java.util.StringTokenizer;
import com.capmet.metrics.rmi.CaptiveConnection;
import com.capmet.metrics.rmi.RMIAccessPoint;
import com.capmet.metrics.rmi.RMIMethods;
import com.capmet.tools.XE.XEDialogs;
import com.capmet.tools.XE.XEApplication;
import com.capmet.metrics.rmi.CaptiveCollectorStats;
import com.jgoodies.forms.layout.*;
import com.jgoodies.forms.builder.PanelBuilder;
import com.jgoodies.forms.factories.FormFactory;

/**
 * The Controller GUI for CaptiveCollector.
 */
public class CaptiveControl extends XEApplication {
    private JFrame     frame;
    private String     top;
    private String     hostName;
    private String     startCommand;
    private boolean    isWindows;
    private CCStatus   ccStatus = null;
    private boolean    cRunning;
    private JLabel     runStopLabel;
    private JPanel     runStopPanel;
    private JButton    startButton;
    private JButton    stopButton;
    private JLabel     text;

    private void refresh() {
        try {
            if (ccStatus == null)
                ccStatus = new CCStatus("localhost");
            ccStatus.refresh(null);
            String labelText = "<html><body><table border=1>";
            for(int i=0; i<ccStatus.values.length; i++) {
                labelText += "<tr><td>";
                labelText += ccStatus.tags[i];
                labelText += "</td><td>";
                labelText += ccStatus.values[i];
                labelText += "</td></tr>";
            }
            labelText += "</table></body></html>";
            text.setText(labelText);
            runStopPanel.setBackground(Color.GREEN);
            runStopLabel.setText("Running");
            startButton.setEnabled(false);
            stopButton.setEnabled(true);
            cRunning = true;
        } catch(Exception e) {
            text.setText(CCStatus.msgs.getString("CC_Not_Running"));
            runStopPanel.setBackground(Color.RED);
            runStopLabel.setText("Stopped");
            startButton.setEnabled(true);
            stopButton.setEnabled(false);
            cRunning = false;
        }
    }

    private class LocalException extends Exception {
        public LocalException(String msg) {
            super(msg);
        }
    }

    // too clumsy to modify CCShutdown for use here.
    private void stop() throws LocalException {
        UID challenge = null;

        String cFile = top + File.separatorChar + "logs"
            + File.separatorChar + "CaptiveCollector" + File.separatorChar +
            "challenge" + '.' + hostName;

        try {
            FileInputStream fis = new FileInputStream(cFile);
            ObjectInputStream ois = new ObjectInputStream(fis);

            challenge = (UID) ois.readObject();
            ois.close();
        } catch(Exception e) {
            throw new LocalException(CCStatus.msgs.getString("Denied"));
        }

        try {
            RMIAccessPoint rap =
                new RMIAccessPoint(new CaptiveConnection("localhost"));
            RMIMethods remote = (RMIMethods) rap.getInterface();

            if (!remote.CCShutdown(challenge))
                throw new LocalException(CCStatus.msgs.getString("Denied"));
        } catch(Exception e) {
            throw new LocalException(e.getMessage());
        }
    }

    private void start() throws LocalException {
        UID challenge = null;

        if (isWindows) {
            startCommand = "java -classpath "
                + "\"" + System.getProperty("java.class.path") + "\""
                + " \"-Dcom.capmet.CaptiveCollector.Top=" + top + "\""
                + " com.capmet.Captivity.CaptiveCollector.CaptiveCollector";
        } else {
            startCommand = "java -classpath "
                + System.getProperty("java.class.path")
                + " -Dcom.capmet.CaptiveCollector.Top=" + top
                + " com.capmet.Captivity.CaptiveCollector.CaptiveCollector";
        }

        try {
            Runtime.getRuntime().exec(startCommand);
        } catch(Exception e) {
            throw new LocalException(e.getMessage());
        }
    }

    private class LogViewer implements ActionListener {
        private class LogInfo {
            public String date;
            public String time;
            public String level;
            public String source;
            public String method;
            public String message;
            public String other;
        }

        private class LogModel extends AbstractTableModel {
            private Object[][] rowData;
            private Object[]   columnNames;

            public LogModel(Object[][] rowData, Object[] columnNames) {
                this.rowData = rowData;
                this.columnNames = columnNames;
            }

            public String getColumnName(int column) {
                return columnNames[column].toString();
            }

            public int getRowCount() {
                return rowData.length;
            }

            public int getColumnCount() {
                return columnNames.length;
            }

            public Object getValueAt(int row, int col) {
                return rowData[row][col];
            }

            public void setValueAt(Object value, int row, int col) {
                rowData[row][col] = value;
                fireTableCellUpdated(row, col);
            }

            public boolean isCellEditable(int row, int column) {
                return false;
            }

            public void setData(Object[][] rowData) {
                this.rowData = rowData;
                fireTableDataChanged();
            }
        }

        private JFrame     logFrame;
        private String[]   headings;
        private String[][] tableData;
        private LogModel   logModel;
        private Vector<LogInfo> logEntries = null;

        private boolean makeTableData() {
            if (logEntries == null) {
                logEntries = new Vector<LogInfo>();
                headings = new String[] {
                    CCStatus.msgs.getString("Date"),
                    CCStatus.msgs.getString("Time"),
                    CCStatus.msgs.getString("Level"),
                    CCStatus.msgs.getString("Source"),
                    CCStatus.msgs.getString("Method"),
                    CCStatus.msgs.getString("Message"),
                    CCStatus.msgs.getString("Other")
                };
            } else
                logEntries.clear();

            try {
                String logFile = top + File.separatorChar + "logs" +
                    File.separatorChar + "CaptiveCollector" + 
                    File.separatorChar + hostName;
                FileInputStream fis = new FileInputStream(logFile);
                InputStreamReader isr = new InputStreamReader(fis);
                BufferedReader br = new BufferedReader(isr);

                for(;;) {
                    String line = br.readLine();
                    int token = 0;

                    if (line == null)
                        break;

                    LogInfo li = new LogInfo();
                    StringTokenizer st = new StringTokenizer(line, "~~");

                    while(st.hasMoreTokens()) {
                        switch(token++) {
                        case 0:
                            li.date = st.nextToken();
                            break;
                        case 1:
                            li.time = st.nextToken();
                            break;
                        case 2:
                            li.level = st.nextToken();
                            break;
                        case 3:
                            li.source = st.nextToken();
                            break;
                        case 4:
                            li.method = st.nextToken();
                            break;
                        case 5:
                            li.message = st.nextToken();
                            break;
                        case 6:
                            li.other = st.nextToken();
                            break;
                        }
                    }

                    logEntries.addElement(li);
                }
            } catch(IOException e) {
                XEDialogs.warningDialog(frame, e.getMessage());
                return false;
            }

            int n = logEntries.size();
            tableData = new String[n][7];

            for(int i=0; i<n; i++) {
                LogInfo li = logEntries.elementAt(i);

                tableData[i][0] = li.date;
                tableData[i][1] = li.time;
                tableData[i][2] = li.level;
                tableData[i][3] = li.source;
                tableData[i][4] = li.method;
                tableData[i][5] = li.message;
                tableData[i][6] = li.other;
            }

            return true;
        }

        public void actionPerformed(ActionEvent ae) {
            if (!makeTableData())
                return;

            if (logFrame == null) {
                logFrame = new JFrame(
                    CCStatus.msgs.getString("Log_File") + hostName);

                JTable logTable = new JTable();
                JScrollPane scroller = new JScrollPane(logTable);
                FormLayout layout = new FormLayout(
                   "c:p:g", "f:p:g, 6dlu, c:p:g");
                PanelBuilder builder = new PanelBuilder(layout);
                FormLayout bLayout = new FormLayout(
                   "c:p:g, 4dlu, c:p:g", "c:p:g");
                PanelBuilder bBuilder = new PanelBuilder(bLayout);
                JButton refreshButton = new JButton(CCStatus.msgs.getString("Refresh"));
                CellConstraints cc = new CellConstraints();
                JButton closeButton = new JButton(CCStatus.msgs.getString("Close"));
                TableColumnModel cm = logTable.getColumnModel();
                TableColumn tc;
                int width;

                logModel = new LogModel(tableData, headings);
                logTable.setModel(logModel);
                logTable.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
                logTable.setPreferredScrollableViewportSize(new Dimension(450,100));

                // source; enlarge by 50%
                tc = cm.getColumn(3);
                width = tc.getPreferredWidth();
                width += (width / 2);
                tc.setPreferredWidth(width);

                // method; enlarge by 50%
                tc = cm.getColumn(4);
                width = tc.getPreferredWidth();
                width += (width / 2);
                tc.setPreferredWidth(width);

                // message; enlarge by 400%
                tc = cm.getColumn(5);
                width = tc.getPreferredWidth();
                width += (width * 4);
                tc.setPreferredWidth(width);

                refreshButton.setToolTipText(
                    CCStatus.msgs.getString("Refresh_TTT"));
                refreshButton.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent ae) {
                        makeTableData();
                        logModel.setData(tableData);
                    }
                });
                closeButton.setToolTipText(
                    CCStatus.msgs.getString("Close_TTT"));
                closeButton.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent ae) {
                        logFrame.setVisible(false);
                    }
                });

                builder.setDefaultDialogBorder();

                bBuilder.add(refreshButton, cc.xy(1, 1));
                bBuilder.add(closeButton,   cc.xy(3, 1));

                builder.add(scroller,            cc.xy(1, 1, "f, f"));
                builder.add(bBuilder.getPanel(), cc.xy(1, 3));

                logFrame.setContentPane(builder.getPanel());
                logFrame.pack();
            }

            Rectangle pr = new Rectangle();
            Rectangle cr = new Rectangle();

            pr = frame.getBounds(pr);
            cr = logFrame.getBounds(cr);
            cr.x = pr.x;
            cr.y = pr.y + pr.height;
            logFrame.setBounds(cr);

            logFrame.setVisible(true);
        }
    }

    /**
     * Constructor.
     * @param args The same args as passed to main.
     */
    public CaptiveControl(String[] args) {
        JButton refreshButton = new JButton(
            CCStatus.msgs.getString("Refresh"));
        FormLayout topLayout = new FormLayout(
            "f:p:g", "f:p:g, 10dlu, f:p:g");
        PanelBuilder topBuilder = new PanelBuilder(topLayout);
        FormLayout statusLayout = new FormLayout("f:p:g",
            "f:p:g, 6dlu, c:p:g, 6dlu");
        PanelBuilder statusBuilder = new PanelBuilder(statusLayout);
        FormLayout statsLayout = new FormLayout("c:p:g",
            "c:p:g, 6dlu, c:p:g, 6dlu");
        PanelBuilder statsBuilder = new PanelBuilder(statsLayout);
        FormLayout startStopLayout = new FormLayout(
            "c:p:g, 6dlu c:p:g", "c:p:g");
        PanelBuilder startStopBuilder = new PanelBuilder(startStopLayout);
        CellConstraints cc = new CellConstraints();
        JMenuBar menuBar = new JMenuBar();
        JMenu appMenu = new JMenu(CCStatus.msgs.getString("Cap_Con"));
        JMenuItem logItem = new JMenuItem(CCStatus.msgs.getString("Log_View"));
        JMenuItem closeItem = new JMenuItem(CCStatus.msgs.getString("Exit"));

        top = System.getProperty("com.capmet.CaptiveCollector.Top");
        if (top == null || top.length() == 0)
            XEDialogs.fatalDialog(CCStatus.msgs.getString("No_Top"));

        // This is not the way to do this, but starting applications as
        // root with a modified library path does not work on a lot of
        // systems and the mdr classes would require a native method and
        // therefore, here we are.

        isWindows = System.getProperty("os.name").startsWith("Windows");

        // onward
        try {
            hostName = InetAddress.getLocalHost().getHostName().split("\\.")[0];
        } catch(UnknownHostException e) {
            hostName = "localhost";
        }

        frame = new JFrame(CCStatus.msgs.getString("CC_Control") + hostName);
        text = new JLabel();

        logItem.addActionListener(new LogViewer());
        closeItem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                System.exit(0);
            }
        });

        appMenu.add(logItem);
        appMenu.addSeparator();
        appMenu.add(closeItem);
        menuBar.add(appMenu);
        frame.setJMenuBar(menuBar);

        String ICON_PATH = "com/capmet/Captivity/CaptiveCollector/images/Control.png";
        URL imageURL = getClass().getClassLoader().getResource(ICON_PATH);
        ImageIcon controlIcon = null;
        if (imageURL != null)
            controlIcon = new ImageIcon(imageURL);
        if (controlIcon != null)
            frame.setIconImage(controlIcon.getImage());

        // running/stopped panels with labels and colors
        runStopLabel = new JLabel();
        runStopPanel = new JPanel();
        startButton = new JButton(CCStatus.msgs.getString("Start"));
        stopButton = new JButton(CCStatus.msgs.getString("Stop"));

        refresh();

        runStopLabel.setForeground(Color.BLACK);
        runStopPanel.add(runStopLabel, BorderLayout.CENTER);

        // start/stop buttons
        startStopBuilder.add(startButton, cc.xy(1, 1));
        startStopBuilder.add(stopButton,  cc.xy(3, 1));

        startButton.setToolTipText(
            CCStatus.msgs.getString("Start_TTT"));
        startButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                Cursor defaultCursor = new Cursor(Cursor.DEFAULT_CURSOR);
                Cursor waitCursor = new Cursor(Cursor.WAIT_CURSOR);

                frame.setCursor(waitCursor);
                try {
                    start();
                } catch(LocalException e) {
                    XEDialogs.warningDialog(frame, e.getMessage());
                    frame.setCursor(defaultCursor);
                    return;
                }
                runStopLabel.setText(CCStatus.msgs.getString("Running"));
                runStopPanel.setBackground(Color.GREEN);
                startButton.setEnabled(false);
                stopButton.setEnabled(true);

                // give the collector a chance to get its act together
                for(int i=0; i<60; i++) {
                    try { Thread.sleep(500); } catch(Exception e) {}
                    refresh();
                    if (cRunning)
                        break;
                }

                if (cRunning)
                    frame.pack();
                else
                    XEDialogs.warningDialog(
                        frame, CCStatus.msgs.getString("No_CC") +
                            startCommand);

                frame.setCursor(defaultCursor);
            }
        });
        stopButton.setToolTipText(
            CCStatus.msgs.getString("Stop_TTT"));
        stopButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                try {
                    stop();
                } catch(LocalException e) {
                    XEDialogs.warningDialog(frame, e.getMessage());
                    return;
                }
                runStopLabel.setText(CCStatus.msgs.getString("Stopped"));
                runStopPanel.setBackground(Color.RED);
                startButton.setEnabled(true);
                stopButton.setEnabled(false);
                cRunning = false;
                ccStatus.removeAccessPoint(new CaptiveConnection("localhost"));
                ccStatus = null;
                text.setText(CCStatus.msgs.getString("CC_Not_Running"));
                frame.pack();
            }
        });

        // status row
        statusBuilder.add(runStopPanel,                cc.xy(1, 1));
        statusBuilder.add(startStopBuilder.getPanel(), cc.xy(1, 3, "c, c"));

        TitledBorder statusBorder = new TitledBorder(
            CCStatus.msgs.getString("Status"));
        statusBorder.setTitleJustification(TitledBorder.LEFT);
        statusBuilder.getPanel().setBorder(statusBorder);

        // stats row
        statsBuilder.add(text,          cc.xy(1, 1));
        statsBuilder.add(refreshButton, cc.xy(1, 3));

        refreshButton.setToolTipText(
            CCStatus.msgs.getString("Refresh_TTT"));
        refreshButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                refresh();
                frame.pack();
            }
        });

        TitledBorder statsBorder = new TitledBorder(
            CCStatus.msgs.getString("Metrics"));
        statsBorder.setTitleJustification(TitledBorder.LEFT);
        statsBuilder.getPanel().setBorder(statsBorder);

        topBuilder.setDefaultDialogBorder();
        topBuilder.add(statusBuilder.getPanel(),    cc.xy(1, 1));
        topBuilder.add(statsBuilder.getPanel(),     cc.xy(1, 3));

        frame.setContentPane(topBuilder.getPanel());

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        // frame.setSize(275, 305);
        frame.setVisible(true);
    }

    /**
     * The main program.
     * @param args No arguments currently defined.
     */
    public static void main(final String[] args) {
        try {
            new CaptiveControl(args);
        } catch(Exception e) {
            e.printStackTrace();
        }
    }
}
