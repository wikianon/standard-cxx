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

import java.io.File;
import java.io.Serializable;
import java.io.IOException;
import java.net.URL;
import java.net.InetAddress;
import java.util.Formatter;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.border.TitledBorder;
import com.capmet.tools.XE.GetOpt;
import com.capmet.tools.XE.XEApplication;
import com.capmet.tools.XE.XEDialogs;
import com.capmet.tools.XE.XEMessages;
import com.capmet.tools.XE.GuruFileReader;
import com.capmet.tools.XE.TimeRecord;
import com.capmet.tools.XE.RuleRecord;
import com.capmet.tools.XE.XEBrowserLauncher;
import com.capmet.metrics.cm.CmExceptionParser;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.CellConstraints;
import com.jgoodies.forms.builder.PanelBuilder;

public class GuruVision extends XEApplication {
    private String      gvTop;
    private JFrame      gvFrame;
    private JTabbedPane gvTabbedPane;

    private static void usage() {
        System.err.println(XEMessages.msgs.getString("GV_Usage"));
    }
    
    public static void main(String[] args) {
        new GuruVision(args);
    }
    
    public GuruVision(String[] args) {
        try {
            String fileNames = null;
            GetOpt opts = new GetOpt(args, "h:");
            int c;

            opts.optErr = false;
            while((c = opts.getopt()) != opts.optEOF) {
                switch(c) {
                    case 'h':
                        fileNames = opts.getOptArg();
                        break;
                    default:
                        usage();
                        return;
                }
            }

            gvTop = System.getProperty("com.capmet.CaptiveCollector.Top");
            if (gvTop == null)
                XEDialogs.fatalDialog(XEMessages.msgs.getString("No_Top"));

            gvFrame = new JFrame("GuruVision");
            gvFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            
            final String ICON_PATH = "com/capmet/tools/XE/images/nerd.png";
            URL imageURL = getClass().getClassLoader().getResource(ICON_PATH);
            ImageIcon cgIcon = null;
            if (imageURL != null)
                cgIcon = new ImageIcon(imageURL);
            if (cgIcon != null)
                gvFrame.setIconImage(cgIcon.getImage());
            
            gvTabbedPane = new JTabbedPane();
            
            if (fileNames != null) {
                String dir = gvTop + File.separatorChar + "logs" +
                             File.separatorChar + "VirtualGuru";
                for(String file : fileNames.split(",")) {
                    String path = dir + File.separatorChar + file;
                    File f = new File(path);

                    if (!f.exists())
                        continue;

                    GuruView gv = null;

                    try {
                        gv = new GuruView(file, f);
                    } catch(IOException e) {
                        XEDialogs.warningDialog(e.getMessage());
                        continue;
                    }
                
                    gvTabbedPane.add(file, gv.getPane());
                }
            }
            
            JMenuBar menuBar = new JMenuBar();
            JMenu gvMenu = new JMenu("GuruVision");
            JMenuItem loadItem = new JMenuItem(
                XEMessages.msgs.getString("Load"), 'L');
            JMenuItem exitItem = new JMenuItem(
                XEMessages.msgs.getString("Exit"), 'X');
            JMenu helpMenu = new JMenu(XEMessages.msgs.getString("Help"));
            JMenuItem aboutItem = new JMenuItem(XEMessages.msgs.getString("About"), 'A');
            JMenuItem helpItem = new JMenuItem(XEMessages.msgs.getString("GV_Help"), 'H');

            LoadAction loadAction = new LoadAction();
            loadItem.addActionListener(loadAction);
            exitItem.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    System.exit(0);
                }
            });

            helpItem.addActionListener(new HelpCallback());
            aboutItem.addActionListener(new AboutCallback());

            gvMenu.add(loadItem);
            gvMenu.addSeparator();
            gvMenu.add(exitItem);

            helpMenu.add(helpItem);
            helpMenu.addSeparator();
            helpMenu.add(aboutItem);

            menuBar.add(gvMenu);
            menuBar.add(helpMenu);

            gvFrame.setJMenuBar(menuBar);
            gvFrame.setContentPane(gvTabbedPane);
            gvFrame.pack();
            gvFrame.setVisible(true);

            if (gvTabbedPane.getTabCount() == 0)
                loadAction.actionPerformed(null);
        } catch(Exception e) {
            System.err.println(new CmExceptionParser(e).getMessage());
        }
    }

    private class LoadAction implements ActionListener {
        public void actionPerformed(ActionEvent ev) {
            String dir = gvTop + File.separatorChar + "logs" +
                         File.separatorChar + "VirtualGuru";
            JFileChooser chooser = new JFileChooser(dir);
            chooser.setMultiSelectionEnabled(true);
            int option = chooser.showOpenDialog(gvFrame);

            if (option == JFileChooser.APPROVE_OPTION) {
                File[] files = chooser.getSelectedFiles();

                for(File f : files) {
                    String file = f.getName();
                    int n = file.indexOf('.');

                    if (n != -1)
                        file = file.substring(0, n);

                    GuruView gv = null;

                    try {
                        gv = new GuruView(file, f);
                    } catch(IOException e) {
                        XEDialogs.warningDialog(e.getMessage());
                        continue;
                    }
        
                    gvTabbedPane.add(file, gv.getPane());
                }
            }

            if (gvTabbedPane.getTabCount() == 1)
                gvFrame.pack();
        }
    }
    
    private class GuruView {
        private Container      pane;
        private Object[][]     tableData = null;
        private JTextArea      textArea;
        private GuruFileReader guruReader;
        private int            firstRecord = 0;
        private int            viewWindow = 10;
        private int            recordCount;
        private JLabel         infoLabel;
        private int            guruFileColor = GuruFileReader.ALL_STATES;
        
        public GuruView(String fileName, File file) throws IOException {
            FormLayout layout = new FormLayout(
                "c:d:g",
// colors       table        info         pr/nxt       text         close
  "c:d:n, 4dlu, f:p:n, 4dlu, c:d:n, 4dlu, c:d:n, 6dlu, c:d:g, 6dlu, c:d:n");
            PanelBuilder builder = new PanelBuilder(layout);
            CellConstraints cc = new CellConstraints();
            final String[] headings = new String[] {
                XEMessages.msgs.getString("Number"),
                XEMessages.msgs.getString("Time"),
                XEMessages.msgs.getString("State_H"),
                XEMessages.msgs.getString("Action_H"),
                ""
            };

            guruReader = new GuruFileReader(file, 0, viewWindow);
            recordCount = guruReader.getRecordCount();
            textArea = new JTextArea(15, 20);
            textArea.setEditable(false);
            makeTableData();
            
            final DefaultTableModel model =
                new DefaultTableModel(tableData, headings);
            final JTable table = new JTable(model) {
                // override types by column number
                public Class getColumnClass(int column) {
                    switch(column) {
                    case 0:
                        return Integer.class;
                    case 1:
                    case 3:
                        return String.class;
                    case 2:
                        return JPanel.class;
                    case 4:
                        return JButton.class;
                    default:
                        return Exception.class;
                    }
                }

                // nip this in the bud
                public boolean isCellEditable(int row, int column) {
                    return (column == 4);
                }
            };

            // cause the Details column to be buttons
            ButtonColumn buttonColumn = new ButtonColumn(table);

            table.setDefaultRenderer(Integer.class, new IntegerRenderer(table));
            table.setDefaultRenderer(JPanel.class, new PanelRenderer());
            table.setDefaultRenderer(JButton.class, buttonColumn);
            table.setDefaultEditor(JButton.class, buttonColumn);
            table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
            table.setPreferredScrollableViewportSize(new Dimension(650, 160));

            // hack the column widths
            final TableColumnModel cm = table.getColumnModel();

            resizeColumns(cm);

            FormLayout colorsLayout = new FormLayout(
                "c:d:n, 2dlu, c:d:n, 2dlu, c:d:n, 2dlu, c:d:n, 2dlu, " +
                "c:d:n, 2dlu, c:d:n, 2dlu, c:d:n, 2dlu, c:d:n",
                "c:d:n");
            PanelBuilder colorsBuilder = new PanelBuilder(colorsLayout);

            JButton allButton = new JButton(
                XEMessages.msgs.getString("All"));
            JButton blueButton = new JButton();
            JButton whiteButton = new JButton();
            JButton greenButton = new JButton();
            JButton yellowButton = new JButton();
            JButton orangeButton = new JButton();
            JButton redButton = new JButton();
            JButton blackButton = new JButton();

            blueButton.setBackground(Color.BLUE);
            whiteButton.setBackground(Color.WHITE);
            greenButton.setBackground(Color.GREEN);
            yellowButton.setBackground(Color.YELLOW);
            orangeButton.setBackground(Color.ORANGE);
            redButton.setBackground(Color.RED);
            blackButton.setBackground(Color.BLACK);

            allButton.setToolTipText(
                XEMessages.msgs.getString("All_TTT"));
            blueButton.setToolTipText(
                XEMessages.msgs.getString("Blue_TTT"));
            whiteButton.setToolTipText(
                XEMessages.msgs.getString("White_TTT"));
            greenButton.setToolTipText(
                XEMessages.msgs.getString("Green_TTT"));
            yellowButton.setToolTipText(
                XEMessages.msgs.getString("Yellow_TTT"));
            orangeButton.setToolTipText(
                XEMessages.msgs.getString("Orange_TTT"));
            redButton.setToolTipText(
                XEMessages.msgs.getString("Red_TTT"));
            blackButton.setToolTipText(
                XEMessages.msgs.getString("Black_TTT"));

            Dimension d = new Dimension(24, 22);
            blueButton.setPreferredSize(d);
            whiteButton.setPreferredSize(d);
            greenButton.setPreferredSize(d);
            yellowButton.setPreferredSize(d);
            orangeButton.setPreferredSize(d);
            redButton.setPreferredSize(d);
            blackButton.setPreferredSize(d);

            ActionListener colorClick = new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    JButton b = (JButton) ae.getSource();
                    String t = b.getText();
                    int currentColor = guruReader.getStateColor();

                    if (t == null || t.length() == 0) {
                        Color c = b.getBackground();

                        if (c.equals(Color.BLUE))
                            guruReader.setStateColor(GuruFileReader.BLUE_STATE);
                        else if (c.equals(Color.WHITE))
                            guruReader.setStateColor(GuruFileReader.WHITE_STATE);
                        else if (c.equals(Color.GREEN))
                            guruReader.setStateColor(GuruFileReader.GREEN_STATE);
                        else if (c.equals(Color.YELLOW))
                            guruReader.setStateColor(GuruFileReader.YELLOW_STATE);
                        else if (c.equals(Color.ORANGE))
                            guruReader.setStateColor(GuruFileReader.ORANGE_STATE);
                        else if (c.equals(Color.RED))
                            guruReader.setStateColor(GuruFileReader.RED_STATE);
                        else if (c.equals(Color.BLACK))
                            guruReader.setStateColor(GuruFileReader.BLACK_STATE);
                    } else
                        guruReader.setStateColor(GuruFileReader.ALL_STATES);

                    guruFileColor = guruReader.getStateColor();

                    if (guruFileColor != currentColor) {
                        try {
                            firstRecord = 0;
                            guruReader.loadRecordVector(firstRecord, viewWindow);
                            recordCount = guruReader.getRecordCount();
                            makeTableData();
                        } catch(IOException e) {
                            // fudge
                            XEDialogs.warningDialog(gvFrame, e.getMessage());
                            return;
                        }
                        model.setDataVector(tableData, headings);
                        model.fireTableDataChanged();
                        resizeColumns(cm);
                        updateInfoLabel();
                    }
                }
            };

            allButton.addActionListener(colorClick);
            blueButton.addActionListener(colorClick);
            whiteButton.addActionListener(colorClick);
            greenButton.addActionListener(colorClick);
            yellowButton.addActionListener(colorClick);
            orangeButton.addActionListener(colorClick);
            redButton.addActionListener(colorClick);
            blackButton.addActionListener(colorClick);

            colorsBuilder.add(allButton,    cc.xy(1, 1));
            colorsBuilder.add(blueButton,   cc.xy(3, 1));
            colorsBuilder.add(whiteButton,  cc.xy(5, 1));
            colorsBuilder.add(greenButton,  cc.xy(7, 1));
            colorsBuilder.add(yellowButton, cc.xy(9, 1));
            colorsBuilder.add(orangeButton, cc.xy(11, 1));
            colorsBuilder.add(redButton,    cc.xy(13, 1));
            colorsBuilder.add(blackButton,  cc.xy(15, 1));

            TitledBorder colorsBorder = new TitledBorder(
                XEMessages.msgs.getString("View"));

            colorsBuilder.getPanel().setBorder(colorsBorder);

            FormLayout prnLayout = new FormLayout(
                "c:p:g, 4dlu, c:p:g, 4dlu, c:p:g, 4dlu, c:p:g, 4dlu, c:p:g",
                "c:p:g");
            PanelBuilder prnBuilder = new PanelBuilder(prnLayout);

            JButton beginButton;
            JButton endButton;
            JButton prevButton;
            JButton nextButton;
            JButton refreshButton;

            String ICON_PATH = "com/capmet/tools/XE/images/up.png";
            URL imageURL = getClass().getClassLoader().getResource(ICON_PATH);
            ImageIcon arrowIcon = null;
            if (imageURL != null)
                arrowIcon = new ImageIcon(imageURL);

            if (arrowIcon == null)
                beginButton = new JButton(
                    XEMessages.msgs.getString("Begin"));
            else
                beginButton = new JButton(
                    XEMessages.msgs.getString("Begin"), arrowIcon);

            beginButton.setToolTipText(
                XEMessages.msgs.getString("Begin_TTT"));
            beginButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    firstRecord = 0;
                    try {
                        guruReader.loadRecordVector(firstRecord, viewWindow);
                        makeTableData();
                    } catch(IOException e) {
                        // fudge
                        XEDialogs.warningDialog(gvFrame, e.getMessage());
                        return;
                    }
                    model.setDataVector(tableData, headings);
                    model.fireTableDataChanged();
                    resizeColumns(cm);
                    updateInfoLabel();
                }
            });

            ICON_PATH = "com/capmet/tools/XE/images/down.png";
            imageURL = getClass().getClassLoader().getResource(ICON_PATH);
            arrowIcon = null;
            if (imageURL != null)
                arrowIcon = new ImageIcon(imageURL);

            if (arrowIcon == null)
                endButton = new JButton(
                    XEMessages.msgs.getString("End"));
            else {
                endButton = new JButton(
                    XEMessages.msgs.getString("End"), arrowIcon);
                endButton.setHorizontalTextPosition(SwingConstants.LEADING);
            }

            endButton.setToolTipText(
                XEMessages.msgs.getString("End_TTT"));
            endButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    firstRecord = recordCount - viewWindow;
                    if (firstRecord < 0)
                        firstRecord = 0;
                    try {
                        guruReader.loadRecordVector(firstRecord, viewWindow);
                        makeTableData();
                    } catch(IOException e) {
                        // fudge
                        XEDialogs.warningDialog(gvFrame, e.getMessage());
                        return;
                    }
                    model.setDataVector(tableData, headings);
                    model.fireTableDataChanged();
                    resizeColumns(cm);
                    updateInfoLabel();
                }
            });

            ICON_PATH = "com/capmet/tools/XE/images/left.png";
            imageURL = getClass().getClassLoader().getResource(ICON_PATH);
            arrowIcon = null;
            if (imageURL != null)
                arrowIcon = new ImageIcon(imageURL);

            if (arrowIcon == null)
                prevButton = new JButton(
                    XEMessages.msgs.getString("Previous"));
            else
                prevButton = new JButton(
                    XEMessages.msgs.getString("Previous"), arrowIcon);

            prevButton.setToolTipText(
                XEMessages.msgs.getString("Prev_TTT"));
            prevButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    firstRecord -= viewWindow;
                    if (firstRecord < 0) {
                        XEDialogs.warningDialog(gvFrame,
                            XEMessages.msgs.getString("First_Page"));
                        firstRecord += viewWindow;
                        return;
                    }
                    try {
                        guruReader.loadRecordVector(firstRecord, viewWindow);
                        makeTableData();
                    } catch(IOException e) {
                        // fudge
                        XEDialogs.warningDialog(gvFrame, e.getMessage());
                        return;
                    }
                    model.setDataVector(tableData, headings);
                    model.fireTableDataChanged();
                    resizeColumns(cm);
                    updateInfoLabel();
                }
            });

            ICON_PATH = "com/capmet/tools/XE/images/right.png";
            imageURL = getClass().getClassLoader().getResource(ICON_PATH);
            arrowIcon = null;
            if (imageURL != null)
                arrowIcon = new ImageIcon(imageURL);

            if (arrowIcon == null)
                nextButton = new JButton(
                    XEMessages.msgs.getString("Next"));
            else {
                nextButton = new JButton(
                    XEMessages.msgs.getString("Next"), arrowIcon);
                nextButton.setHorizontalTextPosition(SwingConstants.LEADING);
            }

            nextButton.setToolTipText(
                XEMessages.msgs.getString("Next_TTT"));
            nextButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    firstRecord += viewWindow;
                    if (firstRecord >= recordCount) {
                        XEDialogs.warningDialog(gvFrame,
                            XEMessages.msgs.getString("Last_Page"));
                        firstRecord -= viewWindow;
                        return;
                    }
                    try {
                        guruReader.loadRecordVector(firstRecord, viewWindow);
                        makeTableData();
                    } catch(IOException e) {
                        // fudge
                        XEDialogs.warningDialog(gvFrame, e.getMessage());
                        return;
                    }
                    model.setDataVector(tableData, headings);
                    model.fireTableDataChanged();
                    resizeColumns(cm);
                    updateInfoLabel();
                }
            });

            refreshButton = new JButton(
                XEMessages.msgs.getString("Refresh"));

            refreshButton.setToolTipText(
                XEMessages.msgs.getString("Refresh_TTT"));
            refreshButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    try {
                        guruReader.refresh();
                        recordCount = guruReader.getRecordCount();
                        guruReader.loadRecordVector(firstRecord, viewWindow);
                        makeTableData();
                    } catch(IOException e) {
                        // fudge
                        XEDialogs.warningDialog(gvFrame, e.getMessage());
                        return;
                    }
                    model.setDataVector(tableData, headings);
                    model.fireTableDataChanged();
                    resizeColumns(cm);
                    updateInfoLabel();
                }
            });

            infoLabel = new JLabel();
            updateInfoLabel();

            prnBuilder.add(beginButton,   cc.xy(1, 1));
            prnBuilder.add(prevButton,    cc.xy(3, 1));
            prnBuilder.add(refreshButton, cc.xy(5, 1));
            prnBuilder.add(nextButton,    cc.xy(7, 1));
            prnBuilder.add(endButton,     cc.xy(9, 1));

            JButton closeButton = new JButton(
                XEMessages.msgs.getString("Close"));

            JScrollPane tablePane = new JScrollPane(table);
            JScrollPane textPane = new JScrollPane(textArea);

            closeButton.setToolTipText(
                XEMessages.msgs.getString("Press_Close"));
            closeButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    gvTabbedPane.remove(pane);
                    if (gvTabbedPane.getTabCount() == 0)
                        System.exit(0);
                }
            });
            
            builder.add(colorsBuilder.getPanel(), cc.xy(1, 1, "l, c"));
            builder.add(tablePane,                cc.xy(1, 3, "f, f"));
            builder.add(infoLabel,                cc.xy(1, 5));
            builder.add(prnBuilder.getPanel(),    cc.xy(1, 7));
            builder.add(textPane,                 cc.xy(1, 9, "f, f"));
            builder.add(closeButton,              cc.xy(1, 11));
            
            builder.setDefaultDialogBorder();
            pane = builder.getPanel();
        }

        private void updateInfoLabel() {
            int pageNo = (firstRecord / viewWindow) + 1;
            int lastRecord = firstRecord + viewWindow;
            if (lastRecord > recordCount)
                lastRecord = recordCount;
            int fr = firstRecord + 1;
            if (recordCount == 0)
              fr = 0;
            String msg = XEMessages.msgs.getString("Page_No") + pageNo + ' ' +
                         XEMessages.msgs.getString("Records") +
                         fr + ' ' + '-' + ' ' + lastRecord + ' ' +
                         XEMessages.msgs.getString("Of") + ' ' + recordCount;

            infoLabel.setText(msg);
        }

        private void resizeColumns(TableColumnModel cm) {
            TableColumn col;
            int w;

            // record number: fixed width
            col = cm.getColumn(0);
            col.setPreferredWidth(10);

            // time: double
            col = cm.getColumn(1);
            w = col.getPreferredWidth();
            col.setPreferredWidth(w * 2);
            
            // state: decrease by 50%
            col = cm.getColumn(2);
            w = col.getPreferredWidth();
            col.setPreferredWidth(w - (w / 2));
            
            // action: triple
            col = cm.getColumn(3);
            w = col.getPreferredWidth();
            col.setPreferredWidth(w * 3);

            // button: decrease by 75%
            col = cm.getColumn(4);
            w = col.getPreferredWidth();
            col.setPreferredWidth(w / 2);
        }
        
        public Container getPane() {
            return pane;
        }
        
        private void makeTableData() throws IOException {
            int toTheEnd = recordCount - firstRecord;
            int count = (recordCount < viewWindow ? recordCount : viewWindow);
            if (toTheEnd < count)
                count = toTheEnd;
            if (tableData == null || tableData.length != count)
                tableData = new Object[count][5];
            
            int i = 0;
            for(TimeRecord tr : guruReader.getRecordVector()) {
                // don't know why yet, but I'm stopping it now
                if (i >= count)
                    break;
                tableData[i][0] = new Integer(firstRecord+i+1);
                tableData[i][1] = tr.timestamp;
                // 2 is a label, pass in the color it should be
                tableData[i][2] = tr.state;
                tableData[i][3] = tr.action;
                // 4 is actually a button.
                tableData[i][4] = null;
                i++;
            }
        }
    
        private class IntegerRenderer extends JLabel
                implements TableCellRenderer, Serializable {
        
            private JTable table;

            public IntegerRenderer(JTable table) {
                setOpaque(true);
                setHorizontalAlignment(SwingConstants.CENTER);
                this.table = table;
            }

            public Component getTableCellRendererComponent(
                    JTable table, Object cell,
                    boolean isSelected, boolean hasFocus,
                    int row, int column) {
            
                if (cell == null)
                    setText("?");
                else
                    setText(((Integer) cell).toString());

                setBackground(table.getBackground());

                return this;
            }
        }
    
        private class PanelRenderer extends JPanel
                implements TableCellRenderer, Serializable {
        
            public PanelRenderer() {
                setOpaque(true);
            }

            public Component getTableCellRendererComponent(
                    JTable table, Object cell,
                    boolean isSelected, boolean hasFocus,
                    int row, int column) {
            
                Color background = (Color) cell;

                setBackground(background);
                return this;
            }
        }

        private class ButtonColumn extends AbstractCellEditor
            implements TableCellRenderer, TableCellEditor, ActionListener
        {
            private JTable  table;
            private JButton renderButton;
            private JButton editButton;
            private Color   background;
            private String  details;
     
            public ButtonColumn(JTable table) {
                super();
                this.table = table;
                renderButton = new JButton();
                renderButton.setToolTipText(
                    XEMessages.msgs.getString("GVDetails_TTT"));
     
                editButton = new JButton();
                editButton.setFocusPainted(false);
                editButton.addActionListener(this);
     
                background = UIManager.getColor("Button.background");
                details = XEMessages.msgs.getString("Details");
            }
     
            public Component getTableCellRendererComponent(
                JTable table, Object value, boolean isSelected,
                    boolean hasFocus, int row, int column)
            {
                if (hasFocus) {
                    renderButton.setForeground(table.getForeground());
                    renderButton.setBackground(background);
                } else if (isSelected) {
                    renderButton.setForeground(table.getSelectionForeground());
                    renderButton.setBackground(table.getSelectionBackground());
                } else {
                    renderButton.setForeground(table.getForeground());
                    renderButton.setBackground(background);
                }
     
                renderButton.setText(details);
                return renderButton;
            }
     
            public Component getTableCellEditorComponent(
                JTable table, Object value, boolean isSelected,
                    int row, int column)
            {
                editButton.setText(details);
                return editButton;
            }
     
            public Object getCellEditorValue() {
                return details;
            }

            public void actionPerformed(ActionEvent ae) {
                int n = table.getSelectionModel().getMinSelectionIndex();

                fireEditingStopped();

                // hmmm
                if (n == -1)
                    return;

                TimeRecord t;

                try {
                    t = GuruView.this
                                .guruReader.getRecordVector().elementAt(n);
                } catch(ArrayIndexOutOfBoundsException e) {
                    // hmmm
                    return;
                }
                String out = XEMessages.msgs.getString("Time_Label") + ' ' +
                             t.timestamp + '\n';
                out += (XEMessages.msgs.getString("State") +
                        t.stateString + '\n');
                if (t.triggerRule != null) {
                    out += (XEMessages.msgs.getString("Trigger") +
                            t.triggerRule + '\n');
                    out += (XEMessages.msgs.getString("Top_Process") +
                            t.topProcess + '\n');
                    out += (XEMessages.msgs.getString("Sort_Key") +
                            t.sortKey + '\n');
                    out += (XEMessages.msgs.getString("Off_Value") +
                            t.keyFieldValue + '\n');
                }
                boolean firstRecord = true;
                for(RuleRecord r : t.ruleVector) {
                    if (!firstRecord)
                        out += '\n';
                    else
                        firstRecord = false;
                    out += ('\t' + r.ruleName + '\n');
                    out += ('\t' + XEMessages.msgs.getString("State") +
                            r.stateString + '\n');
                    out += ('\t' + XEMessages.msgs.getString("Action") +
                            r.action + '\n');
                    out += ('\t' + XEMessages.msgs.getString("Explanation") +
                            r.explanation + '\n');
                    out += ('\t' + XEMessages.msgs.getString("Detail") +
                            r.furtherDetail + '\n');
                    out += ('\t' + XEMessages.msgs.getString("Solution") +
                            r.solution + '\n');
                    boolean firstSubRecord = true;
                    for(RuleRecord sr : r.subruleVector) {
                        if (!firstSubRecord)
                            out += '\n';
                        else
                            firstSubRecord = false;
                        out += ("\t\t" + sr.ruleName + '\n');
                        out += ("\t\t" + XEMessages.msgs.getString("State") +
                                sr.stateString + '\n');
                        out += ("\t\t" + XEMessages.msgs.getString("Action") +
                                sr.action + '\n');
                        out += ("\t\t" + XEMessages.msgs.getString("Explanation") +
                                sr.explanation + '\n');
                        out += ("\t\t" + XEMessages.msgs.getString("Detail") +
                                sr.furtherDetail + '\n');
                        out += ("\t\t" + XEMessages.msgs.getString("Solution") +
                                sr.solution + '\n');
                    }
                }
                GuruView.this.textArea.setText(out);
                GuruView.this.textArea.setCaretPosition(0);
            }
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
            final String imagePath = "com/capmet/tools/XE/images/nerd.png";
            final String msg =
                    "<html><c>GuruVision v1.0<br>by Captive Metrics Software Corporation</c></html>";

            XEDialogs.aboutDialog(gvFrame, msg, imagePath);
        }
    }

    private class HelpCallback implements ActionListener {
        public void actionPerformed(ActionEvent ae) {
            String top = System.getProperty("com.capmet.CaptiveCollector.Top");

            if (top == null) {
                XEDialogs.warningDialog(XEMessages.msgs.getString("No_Top"));
                return;
            }
            String helpURL = "file://" + top + "/docs/User_Manual/XE/GuruVision.html";

            try {
                new XEBrowserLauncher(helpURL);
            } catch(Exception e) {
                XEDialogs.warningDialog(e.getMessage());
                return;
            }
        }
    }
}
