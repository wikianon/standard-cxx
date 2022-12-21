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

import java.util.Vector;
import java.util.Formatter;
import java.io.Serializable;
import java.awt.*;
import java.awt.event.*;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.event.*;
import javax.swing.border.*;
import com.jgoodies.forms.layout.*;
import com.jgoodies.forms.builder.PanelBuilder;
import com.jgoodies.forms.factories.FormFactory;
import com.capmet.tools.XE.XEDialogs;
import com.capmet.tools.XE.XEApplication;
import com.capmet.tools.XE.rules.Rule;
import com.capmet.tools.XE.rules.LiveCPURule;
import com.capmet.tools.XE.rules.LiveDiskRule;
import com.capmet.tools.XE.rules.LiveMemoryRule;
import com.capmet.tools.XE.rules.LiveNICRule;
import com.capmet.tools.XE.rules.LivePageFileRule;
import com.capmet.tools.XE.rules.LiveCacheRule;
import com.capmet.tools.XE.rules.LiveTCPRule;
import com.capmet.tools.XE.rules.AggregateRule;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.rmi.PerformanceException;

public class XERulePanel extends Timer implements MouseListener {
    private Container             RpPanel;
    private Container             RpParent;
    private String                RpRemoteHost;
    private Rule[]                RpRules;
    private Rule                  RpPeakRule;
    private int                   RpInterval;
    private JPopupMenu            RpPop;
    private JMenuItem             RpMenuItem;
    private IntervalListener      RpIList;
    private Vector<RulePanel>     RpRuleVector;
    private RulePanel             RpClickRule;
    private Color                 RpOldBackground;
    private DisksPopup            RpDisksPopup = null;
    private NICsPopup             RpNICsPopup = null;
    private XERulePanelProperties RpProps = null;

    //
    // public methods
    //

    public void erase() {
        RpPanel.removeMouseListener(this);
        RpPanel = null;
        RpParent = null;
        RpRemoteHost = null;
        RpRules = null;
        RpPeakRule = null;
        RpMenuItem.removeActionListener(RpIList);
        RpPop.remove(0);
        RpIList.erase();
        RpPop.setBorder(null);
        RpPop = null;
        RpMenuItem = null;
        RpIList = null;
        if (RpRuleVector != null)
            RpRuleVector.clear();
        RpRuleVector = null;
        if (RpClickRule != null)
            RpClickRule.erase();
        RpClickRule = null;
        RpOldBackground = null;
        if (RpDisksPopup != null)
            RpDisksPopup.erase();
        RpDisksPopup = null;
        if (RpNICsPopup != null)
            RpNICsPopup.erase();
        RpNICsPopup = null;
        RpProps = null;
        for(ActionListener ae : getActionListeners())
            removeActionListener(ae);
    }

    public Rule[] getRules() {
        return RpRules;
    }

    public Container getPane() {
        return RpPanel;
    }

    public Rule getPeakRule() {
        return RpPeakRule;
    }

    public XERulePanelProperties getProperties() {
        RpProps.intervalSeconds = RpInterval / 1000;
        if (RpDisksPopup != null) {
            RpProps.isDisksWindowVisible = RpDisksPopup.isVisible();
            RpProps.disksWindowBounds = RpDisksPopup.getBounds();
        }
        if (RpNICsPopup != null) {
            RpProps.isNICsWindowVisible = RpNICsPopup.isVisible();
            RpProps.nicsWindowBounds = RpNICsPopup.getBounds();
        }
        return RpProps;
    }

    public void setProperties(XERulePanelProperties props) {
        RpInterval = props.intervalSeconds * 1000;
        if (props.disksWindowBounds != null) {
            if (RpDisksPopup == null)
                RpDisksPopup = new DisksPopup();
            RpDisksPopup.setBounds(props.disksWindowBounds);
            if (props.isDisksWindowVisible)
                RpDisksPopup.setVisible(true);
        }
        if (props.nicsWindowBounds != null) {
            if (RpNICsPopup == null)
                RpNICsPopup = new NICsPopup();
            RpNICsPopup.setBounds(props.nicsWindowBounds);
            if (props.isNICsWindowVisible)
                RpNICsPopup.setVisible(true);
        }
    }

    //
    // private methods
    //

    private void mouseDroppings(MouseEvent evt, int clickType) {
        if (evt.isPopupTrigger()) {
            int x = evt.getX();
            int y = evt.getY();
            Point p = new Point(x, y);

            SwingUtilities.convertPointToScreen(p, RpPanel);

            RpIList.setXY(p.x, p.y);
            RpPop.show(RpPanel, x, y);
        } else {
            if (evt.getButton() == MouseEvent.BUTTON1) {
                if (clickType == MouseEvent.MOUSE_PRESSED) {
                    int x = evt.getX();
                    int y = evt.getY();

                    RpClickRule = null;

                    for(RulePanel rp : RpRuleVector) {
                        Rectangle r = rp.getPanel().getBounds();

                        if ((x >= r.x && x <= r.x + r.width) &&
                            (y >= r.y && y <= r.y + r.height)) {
                            RpClickRule = rp;
                            break;
                        }
                    }

                    if (RpClickRule != null && RpClickRule.getHasPopup()) {
                        JPanel p = RpClickRule.getPanel();
                
                        RpOldBackground = p.getBackground();
                        p.setBackground(Color.BLACK);
                    }
                } else if (clickType == MouseEvent.MOUSE_RELEASED) {
                    if (RpClickRule != null && RpClickRule.getHasPopup()) {
                        JPanel p = RpClickRule.getPanel();

                        p.setBackground(RpOldBackground);
                    }
                } else if (clickType == MouseEvent.MOUSE_CLICKED) {
                    if (RpClickRule != null && RpClickRule.getHasPopup()) {
                        Rule rule = RpClickRule.getRule();

                        if (rule instanceof LiveDiskRule) {
                            if (RpDisksPopup == null)
                                RpDisksPopup = new DisksPopup((LiveDiskRule) rule);
                            RpDisksPopup.setVisible(true);
                        } else if (rule instanceof LiveNICRule) {
                            if (RpNICsPopup == null)
                                RpNICsPopup = new NICsPopup((LiveNICRule) rule);
                            RpNICsPopup.setVisible(true);
                        }
                    }
                }
            }
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
        mouseDroppings(evt, MouseEvent.MOUSE_PRESSED);
    }

    /**
     * Mouse button has been released.
     * @param evt The MouseEvent
     */
    public void mouseReleased(MouseEvent evt) {
        mouseDroppings(evt, MouseEvent.MOUSE_RELEASED);
    }

    /**
     * Mouse button has been pushed down and then released.
     * @param evt The MouseEvent
     */
    public void mouseClicked(MouseEvent evt) {
        mouseDroppings(evt, MouseEvent.MOUSE_CLICKED);
    }

    //
    // private classes
    //

    private class RulePanel {
        private Rule                 rule;
        private Vector<SubRulePanel> subRpRuleVector;
        private TitledBorder[]       coloredBorders;
        private JPanel               panel;
        private boolean              hasPopup;

        //
        // public methods
        //

        public void erase() {
            rule = null;
            if (subRpRuleVector != null)
                subRpRuleVector.clear();
            subRpRuleVector = null;
            coloredBorders = null;
            panel = null;
        }

        public JPanel getPanel() {
            return panel;
        }

        public Rule getRule() {
            return rule;
        }

        public boolean getHasPopup() {
            return hasPopup;
        }

        public void updateValues() {
            panel.setBorder(getBorder());
            for(SubRulePanel rr : subRpRuleVector)
                rr.updateValues();
        }

        //
        // private methods
        //

        private TitledBorder getBorder() {
            int n = Rule.colorValue(rule.getState());
            String title = rule.getRuleName();

            if (hasPopup)
                title = title + ' ' + XEMessages.msgs.getString("Press_Popup");

            if (n == -1)
                n = 0;

            if (coloredBorders[n] == null)
                coloredBorders[n] =
                    new TitledBorder(
                        new LineBorder(rule.getState(), 5, true),
                            title, TitledBorder.CENTER,
                                TitledBorder.TOP, null, null);

            return coloredBorders[n];
        }

        //
        // private classes
        //

        private class SubRulePanel {
            private Rule        rule;
            private JPanel      panel;
            private JLabel      actionText;
            private JLabel      explanationText;
            private JLabel      detailText;
            private JLabel      solutionText;
            private HistoryBars historyBars;

            public SubRulePanel(Rule rule, boolean isSubRule) {
                // top level layout and builder
                FormLayout layout = new FormLayout(
                    "l:225dlu:n, 2dlu, r:110dlu:n", "f:p:n");
                PanelBuilder builder = new PanelBuilder(layout);

                // layout and builder for the results
                FormLayout resultsLayout = new FormLayout(
                    "l:d:n, 4dlu, l:d:n",
                    "c:d:n, 1dlu, c:d:n, 1dlu, c:d:n, 1dlu, c:d:n, 1dlu, c:d:n");
                PanelBuilder resultsBuilder = new PanelBuilder(resultsLayout);

                // constraints for everyone
                CellConstraints cc = new CellConstraints();

                // results labels
                JLabel actionLabel = new JLabel(
                    XEMessages.msgs.getString("Action"));
                JLabel explanationLabel = new JLabel(
                    XEMessages.msgs.getString("Explanation"));
                JLabel detailLabel = new JLabel(
                    XEMessages.msgs.getString("Detail"));
                JLabel solutionLabel = new JLabel(
                    XEMessages.msgs.getString("Solution"));

                // save for later
                this.rule = rule;

                // create the results labels
                actionText = new JLabel();
                explanationText = new JLabel();
                detailText = new JLabel();
                solutionText = new JLabel();

                // build the history bars
                historyBars = new HistoryBars(25);
                JPanel history = historyBars.getPanel();

                // titled border for the history
                TitledBorder historyBorder = new TitledBorder(
                    XEMessages.msgs.getString("History_H"));
                history.setBorder(historyBorder);

                // add the builder components
                resultsBuilder.add(actionLabel,      cc.xy(1, 1));
                resultsBuilder.add(explanationLabel, cc.xy(1, 3));
                resultsBuilder.add(detailLabel,      cc.xy(1, 5));
                resultsBuilder.add(solutionLabel,    cc.xy(1, 7));

                resultsBuilder.add(actionText,       cc.xy(3, 1));
                resultsBuilder.add(explanationText,  cc.xy(3, 3));
                resultsBuilder.add(detailText,       cc.xy(3, 5));
                resultsBuilder.add(solutionText,     cc.xy(3, 7));

                // titled border for the results
                TitledBorder resultsBorder = new TitledBorder(
                    XEMessages.msgs.getString("Results"));
                resultsBuilder.getPanel().setBorder(resultsBorder);

                // build the top level panel
                builder.add(resultsBuilder.getPanel(), cc.xy(1, 1));
                builder.add(history,                   cc.xy(3, 1));

                // this is the top level panel
                panel = builder.getPanel();

                // subrules have their own titled border
                if (isSubRule)
                    panel.setBorder(new TitledBorder(
                        null, rule.getRuleName(), TitledBorder.LEFT,
                            TitledBorder.TOP, null, null));

                updateValues();
            }

            public void updateValues() {
                actionText.setText(rule.getAction());
                explanationText.setText(rule.getExplanation());
                detailText.setText(rule.getFurtherDetail());
                solutionText.setText(rule.getSolution());
                historyBars.addBar(rule.getState());
            }

            public JPanel getPanel() {
                return panel;
            }
        }

        private class HistoryBars {
            private JProgressBar[] bars;
            private int            barCount;
            private int            currentBar;
            private JPanel         panel;

            public HistoryBars(int barCount) {
                int i;

                // don't mess with me
                if (barCount < 2)
                    barCount = 2;

                this.barCount = barCount;
                bars = new JProgressBar[barCount];
                for(i=0; i<barCount; i++) {
                    bars[i] = new JProgressBar(JProgressBar.VERTICAL, 0, 100);
                    bars[i].setForeground(Color.WHITE);
                    bars[i].setBackground(Color.WHITE);
                    bars[i].setValue(0);
                    bars[i].setPreferredSize(new Dimension(8, 60));
                }
                currentBar = 0;

                String cols = "f:p:g";
                for(i=0; i<barCount; i++)
                    cols = cols + ", f:p:g";

                FormLayout layout = new FormLayout(cols, "f:p:g");
                PanelBuilder builder = new PanelBuilder(layout);
                CellConstraints cc = new CellConstraints();

                for(i=0; i<barCount; i++)
                    builder.add(bars[i], cc.xy(i+1, 1));

                panel = builder.getPanel();
            }

            public void addBar(Color state) {
                int percent = 0;

                if (state.equals(Color.WHITE))
                    percent = 0;
                else if (state.equals(Color.GREEN))
                    percent = 20;
                else if (state.equals(Color.YELLOW))
                    percent = 40;
                else if (state.equals(Color.ORANGE))
                    percent = 60;
                else if (state.equals(Color.RED))
                    percent = 80;
                else if (state.equals(Color.BLACK))
                    percent = 100;

                if (currentBar == barCount) {
                    for(int i=1; i<barCount; i++) {
                        bars[i-1].setForeground(bars[i].getForeground());
                        bars[i-1].setValue(bars[i].getValue());
                    }
                    currentBar--;
                }

                bars[currentBar].setForeground(state);
                bars[currentBar++].setValue(percent);
            }

            public JPanel getPanel() {
                return panel;
            }
        }

        //
        // Constructors
        //

        public RulePanel(Rule rule, boolean hasPopup) {
            // for later use
            this.rule = rule;
            this.hasPopup = hasPopup;

            subRpRuleVector = new Vector<SubRulePanel>();
            coloredBorders =
                new TitledBorder[Rule.colorValue(Color.BLACK) + 1];

            // aggregate rule needs a panel with multiple rule panels
            if (rule.isAggregateRule()) {
                // get at its aggregateness
                AggregateRule agg = (AggregateRule) rule;
                String rows = "f:p:g";
                int n;

                n = agg.getSupportedSubruleCount();
                if (n == 0) {
                    // this is the type of error that should fail the program
                    panel = null;
                    return;
                }

                // construct the rows string for the builder
                for(int i=1; i<n; i++)
                    rows = rows + ", 1dlu, f:p:g";

                FormLayout srl = new FormLayout("c:p:g", rows);
                PanelBuilder srb = new PanelBuilder(srl);
                CellConstraints cc = new CellConstraints();

                int x = 1;
                for(Object objr : agg.getSubRules()) {
                    Rule r = (Rule) objr;

                    // skip non-supported rules
                    if (!r.isRuleSupported())
                        continue;

                    // build a SubRulePanel for the subrule
                    SubRulePanel rr = new SubRulePanel(r, true);

                    // stash it in the vector
                    subRpRuleVector.addElement(rr);

                    // add it to the builder
                    srb.add(rr.getPanel(), cc.xy(1, x));

                    // increment past the separator
                    x += 2;
                }

                panel = srb.getPanel();
            } else {
                SubRulePanel rr = new SubRulePanel(rule, false);

                panel = rr.getPanel();
                subRpRuleVector.addElement(rr);
            }

            panel.setBorder(getBorder());
        }
    }

    private class RuleUpdater implements ActionListener {
        private Vector<RulePanel> ruleVector;
        private RuleResolution    ruleResolution;

        public void erase() {
            ruleVector = null;
            ruleResolution = null;
        }

        public RuleUpdater(Vector<RulePanel> ruleVector, RuleResolution rr) {
            this.ruleVector = ruleVector;
            this.ruleResolution = rr;
        }

        public void actionPerformed(ActionEvent ae) {
            // anything will be better than pink
            Color state = Color.PINK;
            Rule  peakRule = null;

            try {
                for(RulePanel rulePanel : ruleVector) {
                    Rule rule = rulePanel.getRule();

                    rule.fireLiveRule();
                    rulePanel.updateValues();

                    if (Rule.isWorseThan(rule.getState(), state)) {
                        state = rule.getState();
                        peakRule = rule;
                    }
                }
                XERulePanel.this.RpPeakRule = peakRule;
                if (ruleResolution != null)
                    ruleResolution.resolve(peakRule);
                if (RpDisksPopup != null)
                    RpDisksPopup.refresh();
                if (RpNICsPopup != null)
                    RpNICsPopup.refresh();
            } catch(Exception e) {
                XERulePanel.this.stop();
                XEDialogs.fatalDialog(new CmExceptionParser(e).getMessage());
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
                    JSlider.HORIZONTAL, 0, 300, RpInterval / 1000);
                label = new JLabel(
                    XEMessages.msgs.getString("Int_Secs") + RpInterval / 1000);
                ok = new JButton(XEMessages.msgs.getString("OK"));
                
                slider.setMinorTickSpacing(30);
                slider.setMajorTickSpacing(60);
                slider.setPaintTicks(true);
                slider.setPaintLabels(true);
                slider.setLabelTable(slider.createStandardLabels(60));
                changeListener = new SliderMoved();
                slider.addChangeListener(changeListener);
                
                ok.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent ae) {
                        XERulePanel.this.stop();
                        XERulePanel.this.setDelay(RpInterval);
                        XERulePanel.this.start();
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
                    XEMessages.msgs.getString("Int_Secs") + RpInterval / 1000);
                slider.setValue(RpInterval / 1000);
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
                RpInterval = slider.getValue() * 1000;

                if (RpInterval == 0)
                    RpInterval = 1000;
                
                label.setText(
                    XEMessages.msgs.getString("Int_Secs") + RpInterval / 1000);
            }
        }
    }

    private class PopupModel extends AbstractTableModel {
        private Object[][] rowData;
        private Object[]   columnNames;
        private Class[]    types = {
            String.class, JPanel.class, String.class, String.class,
            String.class, String.class
        };

        public void erase() {
            rowData = null;
            columnNames = null;
            types = null;
        }

        public PopupModel(Object[][] rowData, Object[] columnNames) {
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

        public Class getColumnClass(int col) {
            return types[col];
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

    private class LineRenderer extends JPanel
            implements TableCellRenderer, Serializable {

        private Color[] states;

        public LineRenderer(Color[] states) {
            this.states = states;
            setOpaque(true);
            setPreferredSize(new Dimension(36, 12));
        }

        public Component getTableCellRendererComponent(
                JTable table, Object cell,
                boolean isSelected, boolean hasFocus,
                int row, int column) {

            setBackground(states[row]);
            return this;
        }
    }

    private class DisksPopup extends JFrame {
        private String[]      headings = null;
        private Object[][]    tableData = null;
        private int           tableRows = 0;
        private StringBuilder fmtStr;
        private Formatter     fmt;
        private LiveDiskRule  rule;
        private PopupModel    tableModel;

        public void erase() {
            setVisible(false);
            dispose();
            headings = null;
            tableData = null;
            fmtStr = null;
            fmt = null;
            rule = null;
            if (tableModel != null)
                tableModel.erase();
            tableModel = null;
        }

        public void refresh() {
            makeTableData();
            tableModel.setData(tableData);
        }

        private void makeTableData() {
            if (headings == null) {
                headings = new String[] {
                    XEMessages.msgs.getString("Name"),
                    XEMessages.msgs.getString("State_H"),
                    XEMessages.msgs.getString("Util"),
                    XEMessages.msgs.getString("SVC_T")
                };
            }

            if (tableRows != rule.diskNames.length) {
                tableRows = rule.diskNames.length;
                tableData = new Object[tableRows][4];
            }

            for(int i=0; i<tableRows; i++) {
                tableData[i][0] = rule.diskNames[i];
                // tableData[i][1] = is a JPanel
                fmt.format("%7.2f %%", rule.busyTime[i]);
                tableData[i][2] = fmtStr.toString();
                fmtStr.setLength(0);
                fmt.format("%7.2f ms", rule.serviceTime[i]);
                tableData[i][3] = fmtStr.toString();
                fmtStr.setLength(0);
            }
        }

        public DisksPopup() {
            super(XEMessages.msgs.getString("Disks") +
                ':' + ' ' + RpRemoteHost);
            for(Rule rule : RpRules)
                if (rule instanceof LiveDiskRule) {
                    construction((LiveDiskRule) rule);
                    return;
                }
        }

        public DisksPopup(LiveDiskRule rule) {
            super(XEMessages.msgs.getString("Disks") +
                ':' + ' ' + RpRemoteHost);
            construction(rule);
        }

        private void construction(LiveDiskRule rule) {
            this.rule = rule;

            FormLayout layout = new FormLayout("c:p:g", "f:p:g, 6dlu, c:d:n");
            PanelBuilder builder = new PanelBuilder(layout);
            CellConstraints cc = new CellConstraints();
            JTable table = new JTable();
            JScrollPane scroller = new JScrollPane(table);
            JButton closeButton = new JButton(XEMessages.msgs.getString("Close"));

            fmtStr = new StringBuilder();
            fmt = new Formatter(fmtStr);
            makeTableData();

            // set a custom renderer for JPanel
            tableModel = new PopupModel(tableData, headings);
            table.setDefaultRenderer(JPanel.class,
                new LineRenderer(rule.diskStates));
            table.setModel(tableModel);
            table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
            table.setPreferredScrollableViewportSize(new Dimension(300, 100));

            closeButton.setToolTipText(
                XEMessages.msgs.getString("Close_Window"));
            closeButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    setVisible(false);
                }
            });

            builder.setDefaultDialogBorder();

            builder.add(scroller,    cc.xy(1, 1, "f, f"));
            builder.add(closeButton, cc.xy(1, 3));

            setContentPane(builder.getPanel());
            pack();

            Rectangle pr = new Rectangle();
            Rectangle cr = new Rectangle();

            pr = RpParent.getBounds(pr);
            cr = getBounds(cr);
            pr.x += ((pr.width - cr.width) / 2);
            pr.y += ((pr.height - cr.height) / 2);
            pr.width = cr.width;
            pr.height = cr.height;
            setBounds(pr);
        }
    }

    private class NICsPopup extends JFrame {
        private String[]      headings = null;
        private Object[][]    tableData = null;
        private int           tableRows = 0;
        private StringBuilder fmtStr;
        private Formatter     fmt;
        private LiveNICRule   rule;
        private PopupModel    tableModel;

        public void erase() {
            setVisible(false);
            dispose();
            headings = null;
            tableData = null;
            fmtStr = null;
            fmt = null;
            rule = null;
            if (tableModel != null)
                tableModel.erase();
            tableModel = null;
        }

        public void refresh() {
            makeTableData();
            tableModel.setData(tableData);
        }

        private void makeTableData() {
            if (headings == null) {
                headings = new String[] {
                    XEMessages.msgs.getString("Name"),
                    XEMessages.msgs.getString("State_H"),
                    XEMessages.msgs.getString("Errors"),
                    XEMessages.msgs.getString("Failures"),
                    XEMessages.msgs.getString("Colls"),
                    XEMessages.msgs.getString("Util")
                };
            }

            if (tableRows != rule.ifNames.length) {
                tableRows = rule.ifNames.length;
                tableData = new Object[tableRows][6];
            }

            for(int i=0; i<tableRows; i++) {
                tableData[i][0] = rule.ifNames[i];
                // tableData[i][1] = is a JPanel
                fmt.format("%7.2f/sec", rule.errors[i]);
                tableData[i][2] = fmtStr.toString();
                fmtStr.setLength(0);
                fmt.format("%7.2f/sec", rule.failures[i]);
                tableData[i][3] = fmtStr.toString();
                fmtStr.setLength(0);
                if (rule.colls == null)
                    tableData[i][4] = "   -";
                else {
                    fmt.format("%3d %%", rule.colls[i]);
                    tableData[i][4] = fmtStr.toString();
                    fmtStr.setLength(0);
                }
                if (rule.utils == null)
                    tableData[i][5] = "   -";
                else {
                    fmt.format("%3d %%", rule.utils[i]);
                    tableData[i][5] = fmtStr.toString();
                    fmtStr.setLength(0);
                }
            }
        }

        public NICsPopup() {
            super(XEMessages.msgs.getString("Interfaces") +
                ':' + ' ' + RpRemoteHost);
            for(Rule rule : RpRules) {
                if (rule instanceof LiveNICRule) {
                    construction((LiveNICRule) rule);
                    return;
                }
            }
        }

        public NICsPopup(LiveNICRule rule) {
            super(XEMessages.msgs.getString("Interfaces") +
                ':' + ' ' + RpRemoteHost);
            construction(rule);
        }

        private void construction(LiveNICRule rule) {
            this.rule = rule;

            FormLayout layout = new FormLayout("c:p:g", "f:p:g, 6dlu, c:d:n");
            PanelBuilder builder = new PanelBuilder(layout);
            CellConstraints cc = new CellConstraints();
            JTable table = new JTable();
            JScrollPane scroller = new JScrollPane(table);
            JButton closeButton = new JButton(XEMessages.msgs.getString("Close"));

            fmtStr = new StringBuilder();
            fmt = new Formatter(fmtStr);
            makeTableData();

            // set a custom renderer for JPanel
            tableModel = new PopupModel(tableData, headings);
            table.setDefaultRenderer(JPanel.class,
                new LineRenderer(rule.ifStates));
            table.setModel(tableModel);
            table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
            table.setPreferredScrollableViewportSize(new Dimension(450, 100));

            closeButton.setToolTipText(
                XEMessages.msgs.getString("Close_Window"));
            closeButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    setVisible(false);
                }
            });

            builder.setDefaultDialogBorder();

            builder.add(scroller,    cc.xy(1, 1, "f, f"));
            builder.add(closeButton, cc.xy(1, 3));

            setContentPane(builder.getPanel());
            pack();

            Rectangle pr = new Rectangle();
            Rectangle cr = new Rectangle();

            pr = RpParent.getBounds(pr);
            cr = getBounds(cr);
            pr.x += ((pr.width - cr.width) / 2);
            pr.y += ((pr.height - cr.height) / 2);
            pr.width = cr.width;
            pr.height = cr.height;
            setBounds(pr);
        }
    }

    //
    // Constructors
    //

    public XERulePanel(Container parent, String remoteHost, int msInterval,
                       Object ... args) throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        super(msInterval, null);

        RpParent = parent;
        RpRemoteHost = remoteHost;
        RpInterval = msInterval;

        String rows = "c:p:g";
        Color state = Color.PINK;
        RpRules = new Rule[] {
            new LiveCPURule(remoteHost),
            new LiveDiskRule(remoteHost),
            new LiveMemoryRule(remoteHost),
            new LiveNICRule(remoteHost),
            new LivePageFileRule(remoteHost),
            new LiveCacheRule(remoteHost),
            new LiveTCPRule(remoteHost)
        };

        RpRuleVector = new Vector<RulePanel>();

        for(Rule rule : RpRules) {
            if (!rule.isRuleSupported())
                continue;
            rule.fireLiveRule();
            if (Rule.isWorseThan(rule.getState(), state)) {
                state = rule.getState();
                RpPeakRule = rule;
            }
            boolean hasPopup = ((rule instanceof LiveDiskRule) ||
                                (rule instanceof LiveNICRule));
            RpRuleVector.addElement(new RulePanel(rule, hasPopup));
            rows = rows + ", 1dlu, c:p:g";
        }

        RuleResolution rr = null;

        switch(args.length) {
        case 0:
            // no trailing args
            break;
        case 1:
            // either a RuleResolution or XERulePanelProperties arg
            if (args[0] instanceof RuleResolution)
                rr = (RuleResolution) args[0];
            else if (args[0] instanceof XERulePanelProperties)
                RpProps = (XERulePanelProperties) args[0];
            else
                throw new PerformanceException(
                    XEMessages.msgs.getString("No_RR"));
            break;
        case 2:
            // a RuleResolution arg followed by a properties arg
            if (args[0] instanceof RuleResolution) {
                rr = (RuleResolution) args[0];
                if (args[1] instanceof XERulePanelProperties) {
                    RpProps = (XERulePanelProperties) args[1];
                    if ((RpInterval / 1000) != RpProps.intervalSeconds) {
                        RpInterval = RpProps.intervalSeconds * 1000;
                        super.setDelay(RpInterval);
                    }
                    setProperties(RpProps);
                    break;
                }
            }
            // any way to get here is an error
            throw new PerformanceException(
                XEMessages.msgs.getString("No_RR"));
        }

        if (RpProps == null)
            RpProps = new XERulePanelProperties();

        addActionListener(new RuleUpdater(RpRuleVector, rr));

        FormLayout layout = new FormLayout("c:p:g", rows);
        PanelBuilder builder = new PanelBuilder(layout);
        CellConstraints cc = new CellConstraints();

        int x = 1;
        for(RulePanel rulePanel : RpRuleVector) {
            builder.add(rulePanel.getPanel(), cc.xy(1, x));
            x += 2;
        }

        builder.setDefaultDialogBorder();

        RpPanel = builder.getPanel();

        RpPop = new JPopupMenu();
        RpMenuItem = new JMenuItem(XEMessages.msgs.getString("Interval"), 'I');
        RpIList = new IntervalListener();
        RpMenuItem.addActionListener(RpIList);
        RpPop.add(RpMenuItem);
        RpPop.setBorder(new BevelBorder(BevelBorder.RAISED));
        RpPanel.addMouseListener(this);
    }

    protected void finalize() {
        // System.out.println("XERulePanel.finalize");
    }
}
