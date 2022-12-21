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

import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import java.net.URL;
import java.awt.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.border.TitledBorder;
import java.awt.event.*;
import java.net.InetAddress;
import java.util.Vector;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.Map;
import java.util.Set;
import java.text.MessageFormat;
import java.io.Serializable;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.io.File;
import D.PN;
import com.capmet.metrics.*;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.rmi.RMIAccessPoint;
import com.capmet.metrics.rmi.CaptiveConnection;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.cm.CmProcessMetrics;
import com.capmet.tools.XE.*;
import com.capmet.tools.XE.rules.*;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.CellConstraints;
import com.jgoodies.forms.builder.PanelBuilder;

/**
 * Top: An application for displaying processes sorted by a field.<p>
 *
 * Use: java Top [-h remote-host[,remote-host] | -p profile-name]<br>
 *      -h remote-host[,remote-host] - specify host[s]<br>
 *      -p profile-name - specify a profile<br>
 * <p>
 * <b>Top</b> is yet another incarnation of the old tried-and-true
 * application for monitoring processes by sorting them according on
 * some display field. This version takes the concept where it's never
 * gone before. This is a Java application that not only runs anywhere
 * Java 5 is supported, but also can monitor any host running the data
 * collection engine <b>CaptiveCollector</b>.
 *
 * <h3>Features</h3>
 *
 * <h4>Multi-Function Tabs</h4>
 *
 * For each host that is monitored, there are 3 tabs and a toolbar.
 * <ul>
 * <li>The <i>Processes</i> tab
 * shows the process display with headings for each field. Any of the heading
 * fields can be clicked to indicate a sort by that field. Clicking again will
 * reverse the order of the sort. A red arrow appears next to the name of the
 * field pointing down for a descending (high to low) sort or up for an
 * ascending (low to high) sort. Any process in the display can be clicked
 * on to mark it for tracking. The line will be redrawn with a light blue
 * background color and will stay that way until unselected by clicking on
 * it again, or until the process exits.</li>
 * <li> The <i>XE Gadgets</i> tab
 * shows the gadgets available for display. Currently, they are a CPU Pie
 * Chart, a Memory Pie Chart and a Page File Pie Chart. Single-clicking on any
 * of these will bring the gadget up in the lower pane and will run according
 * to the interval set for that gadget. To change the interval for a running
 * gadget, place the pointer on the running gadget and click the right button.
 * For Mac users, use control-click. A popup menu will display "Interval..."
 * and when selected, the interval dialog will come up and the interval for
 * that gadget can be selected.</li>
 * <li>The <i>System Info</i> tab
 * shows a tree structure with the configuration of the monitored host
 * split up into functional areas. Any node with a plus sign (+) can be
 * expanded to view more information. This display also updates on an
 * interval, and like the gadgets, a right-click or control-click will
 * bring up an interval dialog.</li>
 * <li>The toolbar has 5 options.
 * <ul>
 * <li>Interval...
 * This will bring up an interval dialog to change the frequency of updates
 * to the Processes display.</li>
 * <li>Visible...
 * This will change the number of processes placed into the viewport for
 * display. The viewport is limited to the size of the screen, so a scrollbar
 * may appear so that the other processes can be viewed.</li>
 * <li>Select Fields...
 * This will pop up a dialog with all of the possible display fields and a
 * check-box indicating whether that field is displayed. Once the desired
 * fields are selected and the close button is pressed, the display will
 * be updated with the selected fields being shown.</li>
 * <li>Watch Command...
 * A dialog will pop up and allow the user to enter the name of a process
 * to track. Once the Apply button is pressed, any process with that name
 * will be highlighted with a light blue background and tracked as long as
 * it exists and is on the screen. Clicking apply again with the same name
 * will unhighlight the processes and will no longer be tracked.</li>
 * <li>Close will close that host tab. If it is the last tab, it will close
 * the window. If that is the last window, the application will exit.</li>
 * </ul></li></ul>
 *
 * <h4>Multi-Host Monitoring</h4>
 *
 * <b>Top</b> can monitor many hosts at once. These hosts can all appear
 * in one window, each host in its own tab, or they can be split up among
 * different windows. Each host can have its own window by selecting the
 * "New Top Instance..." option from the Top menu or hosts can be
 * split up according to their platform, function, membership in a cluster,
 * whatever. If split up according to some creteria, the title in the title-bar
 * can be changed to signify the purpose of the segregation by using the
 * Set Title... option in the Top menu. Examples of titles are
 * "Top: Unix Hosts" or "Top: Database Servers". Each host display can be
 * changed to display different fields, sort ascending or descending on
 * different fields, have different gadgets running, have different nodes
 * open on the System Tree tab, many different options, all unique to the
 * individual host. To add a new host tab to the existing window, select
 * the Monitor... option in the Top menu.
 *
 * <h4>Multi-Process Selection</h4>
 *
 * Instead of marking just one process to watch in the display, like
 * Task Manager in Windows, several processes can be marked for tracking
 * or the name of the process can be entered and all processes with that
 * name will be selected in the display and tracked.
 *
 * <h4>Cluster Definition</h4>
 *
 * Regardless of whether a group of hosts are in a cluster, they can be
 * defined as a logical cluster in <b>Top</b>. Once the definition has
 * been entered, the cluster name can be used as a host name to monitor.
 * A new <b>Top</b> window can be opened with the cluster name and that
 * window will contain one host tab for each host in the cluster. The
 * cluster name can also be used on the command line in place of a remote-host.
 *
 * <h4>Application Profiles</h4>
 *
 * When <b>Top</b> has been modified to display all the information about
 * all the hosts the way the user wants it displayed, the context of the
 * application can be saved as a <i>profile</i>. This will cause <b>Top</b>
 * to take a "snapshot" of itself and save it to the user's home directory
 * in a file called ".xetopconfig". This file a Java serialization data file
 * which contains properties about all of the components of the application.
 * After <b>Top</b> has been started, a profile can be loaded from the Tools
 * menu or the profile can be restarted from the command line by using the
 * -p flag to <b>Top</b>. This functionality allows for a complex set of
 * modifications to be made and then saved for reloading later without the
 * need to revisit all of the actions.
 */
public class Top extends XEApplication {
    private final int SHORT_INTERVAL = 5000;
    private final int MEDIUM_INTERVAL = 15000;
    private final int LONG_INTERVAL = 30000;
    private final int MIN_VISIBLE = 2;
    private final int INITIAL_VISIBLE = 25;
    private final int GADGET_HEIGHT = 150;
    private final int GADGET_WIDTH = 250;
    private final int GADGET_PAD = 10;

    // turn on or off for debugging unintentional object retention
    private final boolean discussFinalization = false;

    private int            topInterval = SHORT_INTERVAL;
    private JFrame         topFrame;
    private JTabbedPane    topTabbedPane;
    private JComboBox      topCmdBox;

    // count how many Top windows are open
    private static class TopVector extends Vector<JFrame> {}
    private static TopVector     topWindows;
    private static TopProperties topProps;
    private static CommandVector topCommands;
    private static ClusterMap    topClusters;

    //
    // Static methods.
    //

    // usage blurb
    private static void usage() {
        System.err.println(XEMessages.msgs.getString("Top_Usage"));
    }

    private static String getXETopConfigFileName() {
        String home = System.getProperty("user.home");
        String file = ".xetopconfig";

        if (home == null)
            return file;

        return home + File.separatorChar + file;
    }

    /**
     * The main method of the Top application.
     * @param args Arguments passed from the environment
     */
    public static void main(String[] args) {
        String remoteHost = null;
        String profileName = null;
        GetOpt opts = new GetOpt(args, "h:p:");
        int c;

        opts.optErr = false;
        while((c = opts.getopt()) != opts.optEOF) {
            switch(c) {
                case 'h':
                    remoteHost = opts.getOptArg();
                    break;
                case 'p':
                    profileName = opts.getOptArg();
                    break;
                default:
                    usage();
                    return;
            }
        }

        topWindows  = new TopVector();

        String configFile = getXETopConfigFileName();
        File f = new File(configFile);

        try {
            if (f.exists()) {
                FileInputStream fis = new FileInputStream(configFile);
                ObjectInputStream ois = new ObjectInputStream(fis);

                topProps = (TopProperties) ois.readObject();
            } else
                topProps = new TopProperties();

            topCommands = topProps.commandNames;
            if (topCommands == null) {
                topCommands = new CommandVector();
                topProps.commandNames = topCommands;
            }

            topClusters = topProps.clusters;
            if (topClusters == null) {
                topClusters = new ClusterMap();
                topProps.clusters = topClusters;
            }

            if (topProps.windowProfiles == null)
                topProps.windowProfiles = new TopPropsMap();
        } catch(Exception e) {
            XEDialogs.warningDialog(new CmExceptionParser(e).getMessage());
        }

        if (profileName == null)
            new Top(remoteHost, null);
        else
            new Top(":" + profileName, null);

        if (topWindows.size() == 0)
            System.exit(0);
    }

    //
    // Public methods.
    //

    /**
     * Retrieve the tabbed pane at the top level of the application frame.
     * @return A <CODE>JTabbedPane</CODE> object.
     */
    public JTabbedPane getTabbedPane() {
        return topTabbedPane;
    }

    public void erase() {
        if (topFrame != null) {
            topFrame.setVisible(false);
            topFrame.dispose();
        }
        topFrame = null;

        topTabbedPane = null;
        topCmdBox = null;

        topWindows.clear();
        topWindows = null;

        topProps = null;

        topCommands.clear();
        topCommands = null;

        topClusters.clear();
        topClusters = null;
    }

    //
    // Private methods.
    //

    private void loadProfile(String pName) {
        if (topProps.windowProfiles == null || !topProps.windowProfiles.containsKey(pName)) {
            XEDialogs.warningDialog(topFrame, pName + ':' + ' ' +
                XEMessages.msgs.getString("No_Profile"));
            return;
        }
        TWPVector tv = topProps.windowProfiles.get(pName);

        // for each top level window
        for(TopWindowProperties twp : tv)
            new Top(null, twp);
    }

    private void saveConfigFile() {
        try {
            String configFile = getXETopConfigFileName();
            FileOutputStream fos = new FileOutputStream(configFile);
            ObjectOutputStream oos = new ObjectOutputStream(fos);

            oos.writeObject(topProps);
        } catch(Exception e) {
            XEDialogs.warningDialog(new CmExceptionParser(e).getMessage());
        }
    }

    //
    // Private classes.
    //

    private class StateIcon implements Icon {
        private Color stateColor;

        public StateIcon() {
            this(Color.WHITE);
        }

        public StateIcon(Color stateColor) {
            this.stateColor = stateColor;
        }

        public void setStateColor(Color stateColor) {
            this.stateColor = stateColor;
        }

        public void paintIcon(Component c, Graphics g, int x, int y) {
            g.setColor(stateColor);
            g.fillRect(x, y, 16, 16);
        }

        public int getIconWidth() {
            return 16;
        }

        public int getIconHeight() {
            return 16;
        }

        public void erase() {
            stateColor = null;
        }

        protected void finalize() {
            if (discussFinalization)
                System.out.println("StateIcon.finalize");
        }
    }

    /*
     * Encapsulated logic for popping up a frame which
     * queries the user for the name of host[s] to monitor.
     */

    private class HostWindow extends JTabbedPane {
        private String         remoteHost;
        private XETopProcesses topProcs;
        private XEGadgets      gadgets;
        private XESystemTree   sysTree;
        private XERulePanel    rulePanel;
        private PeakRule       peakRuleUpdater;
        private JScrollPane    ruleScroller;
        private JToolBar       toolbar;
        private IntervalSlider intervalPopup;
        private ProcSlider     procCountPopup;
        private SelectFields   selectFieldsPopup;
        private WatchCommand   watchPopup;
        private JPanel         statePanel;
        private JLabel         triggerText;
        private JButton        detailsButton;
        private DetailsInfo    detailsInfo;
        private boolean        autoSort = true;
        private boolean        zeroProcs = false;
        private DetailsPopup   detailsPopup;
        private Rule[]         ruleSet;
        private JButton[]      dummyLights;
        private CloseTop       closeAction;

        public void erase() {
            remoteHost = null;

            topProcs.erase();
            topProcs = null;

            gadgets.erase();
            gadgets = null;

            sysTree.erase();
            sysTree = null;

            rulePanel.erase();
            rulePanel = null;

            ruleScroller = null;
            peakRuleUpdater = null;

            // get Interval... button
            JButton tmpButton = (JButton) toolbar.getComponent(0);
            tmpButton.removeActionListener(intervalPopup);

            intervalPopup.erase();
            intervalPopup = null;

            // get Visible... button
            tmpButton = (JButton) toolbar.getComponent(1);
            tmpButton.removeActionListener(procCountPopup);

            procCountPopup.erase();
            procCountPopup = null;

            // get Select Fields... button
            tmpButton = (JButton) toolbar.getComponent(2);
            tmpButton.removeActionListener(selectFieldsPopup);

            selectFieldsPopup = null;

            // get Watch Command... button
            tmpButton = (JButton) toolbar.getComponent(3);
            tmpButton.removeActionListener(watchPopup);

            watchPopup.erase();
            watchPopup = null;

            // get Close... button
            tmpButton = (JButton) toolbar.getComponent(6);
            tmpButton.removeActionListener(closeAction);

            closeAction = null;

            int n = toolbar.getComponentCount();
            for(int i=n-1; i>=0; i--)
                toolbar.remove(i);
            toolbar = null;

            statePanel = null;
            triggerText = null;
            detailsInfo = null;

            detailsButton.removeActionListener(detailsPopup);
            detailsPopup.erase();
            detailsPopup = null;
            detailsButton = null;

            ruleSet = null;
            for(JButton jb : dummyLights) {
                for(ActionListener ae : jb.getActionListeners()) {
                    jb.removeActionListener(ae);
                }
            }
            dummyLights = null;

            for(int i=0; i<this.getTabCount(); i++)
                this.removeTabAt(i);
        }

        public XETopProcesses getProcs() {
            return topProcs;
        }

        public XEGadgets getGadgets() {
            return gadgets;
        }

        public XESystemTree getSysTree() {
            return sysTree;
        }

        public XERulePanel getRulePanel() {
            return rulePanel;
        }

        public JFrame getDetailsFrame() {
            return detailsPopup.getFrame();
        }

        public HostWindow(String remoteHost, TopTabProperties ttp)
            throws Exception {

            FormLayout layout = new FormLayout(
                    "f:p:g",
                    "f:p:n, 6dlu, f:d:g, 2dlu, f:p:n");
            PanelBuilder builder = new PanelBuilder(layout);
            CellConstraints cc = new CellConstraints();
            String dummyCols = "c:p:g";
            int x;
            int i;
            int ruleCount = 0;
            int ruleSubPanelCount = 0;

            this.remoteHost = remoteHost;

            // allocate details structure and setup details button
            detailsButton = new JButton(XEMessages.msgs.getString("Details"));
            detailsInfo = new DetailsInfo();
            detailsPopup = new DetailsPopup();
            detailsButton.addActionListener(detailsPopup);

            // peak rules are updated this way
            peakRuleUpdater = new PeakRule();

            // allocate sub-tabs, from properties or not
            if (ttp == null) {
                topProcs  = new TopProcesses(topFrame, remoteHost, SHORT_INTERVAL,
                    "procCount=" + INITIAL_VISIBLE);
                gadgets   = new XEGadgets(remoteHost, null);
                sysTree   = new XESystemTree(topFrame, remoteHost, LONG_INTERVAL);
                rulePanel = new XERulePanel(topFrame, remoteHost, MEDIUM_INTERVAL,
                    peakRuleUpdater);
            } else {
                topProcs  = new TopProcesses(topFrame, remoteHost, SHORT_INTERVAL,
                    ttp.topProcsProperties);
                gadgets   = new XEGadgets(remoteHost, ttp.gadgetProperties);
                sysTree   = new XESystemTree(topFrame, remoteHost, LONG_INTERVAL,
                    ttp.sysTreeProperties);
                rulePanel = new XERulePanel(topFrame, remoteHost, MEDIUM_INTERVAL,
                    peakRuleUpdater, ttp.rulePanelProperties);
            }

            topInterval = topProcs.getDelay();

            // count the number of rules for allocation
            ruleSet = rulePanel.getRules();
            for(Rule r : ruleSet) {
                if (!r.isRuleSupported())
                    continue;

                // count rule panels
                if (r instanceof AggregateRule) {
                    AggregateRule ar = (AggregateRule) r;

                    ruleSubPanelCount += ar.getSupportedSubruleCount();
                } else
                    ruleSubPanelCount++;

                ruleCount++;
            }

            // state buttons
            dummyLights = new JButton[ruleCount];
            x = 0;
            for(Rule r : ruleSet) {
                if (!r.isRuleSupported())
                    continue;
                dummyLights[x] = new JButton(r.getShortRuleName());
                dummyLights[x].setBackground(r.getState());
                if (x > 0)
                    dummyCols = dummyCols + ", 4dlu, c:p:g";
                x++;
            }

            // update detail info with live data
            Rule peakRule = rulePanel.getPeakRule();

            // add items to the toolbar
            toolbar = new JToolBar();
            JButton tmpButton = new JButton(XEMessages.msgs.getString("Interval"));
            intervalPopup = new IntervalSlider();
            tmpButton.addActionListener(intervalPopup);
            tmpButton.setToolTipText(XEMessages.msgs.getString("Press_Interval"));
            toolbar.add(tmpButton, 0);

            tmpButton = new JButton(XEMessages.msgs.getString("Visible"));
            procCountPopup = new ProcSlider();
            tmpButton.addActionListener(procCountPopup);
            tmpButton.setToolTipText(XEMessages.msgs.getString("Press_Visible"));
            toolbar.add(tmpButton, 1);

            tmpButton = new JButton(XEMessages.msgs.getString("Fields"));
            selectFieldsPopup = new SelectFields();
            tmpButton.addActionListener(selectFieldsPopup);
            tmpButton.setToolTipText(XEMessages.msgs.getString("Press_Fields"));
            toolbar.add(tmpButton, 2);

            tmpButton = new JButton(XEMessages.msgs.getString("Watch_Cmd"));
            watchPopup = new WatchCommand();
            tmpButton.addActionListener(watchPopup);
            tmpButton.setToolTipText(XEMessages.msgs.getString("Press_Command"));
            toolbar.add(tmpButton, 3);

            toolbar.add((JComponent) new AutoSort(XEMessages.msgs.getString("Auto_Sort")), 4);
            toolbar.add((JComponent) new SystemProcs(XEMessages.msgs.getString("Sys_Procs")), 5);

            tmpButton = new JButton(XEMessages.msgs.getString("Close"));
            closeAction = new CloseTop();
            tmpButton.addActionListener(closeAction);
            tmpButton.setToolTipText(XEMessages.msgs.getString("Press_Close"));
            toolbar.add(tmpButton, 6);

            // add sub-tabs to the host tab
            this.addTab(XEMessages.msgs.getString("Processes"),  null, builder.getPanel(),
                XEMessages.msgs.getString("Proc_View"));
            this.addTab(XEMessages.msgs.getString("XE_Gadgets"), null, gadgets.getPane(),
                XEMessages.msgs.getString("Gadget_View"));
            this.addTab(XEMessages.msgs.getString("Sys_Info"),   null, sysTree.getPane(),
                XEMessages.msgs.getString("Sys_View"));

            // build a scroller for the rule tab
            ruleScroller = new JScrollPane(rulePanel.getPane());
            ruleScroller.getVerticalScrollBar().setUnitIncrement(10);

            // add it to the host tab
            this.addTab(XEMessages.msgs.getString("Rules"),      null, ruleScroller,
                XEMessages.msgs.getString("Rule_View"));

            // construct the Last Alert Condition panel
            JLabel stateLabel = new JLabel(
                XEMessages.msgs.getString("State"));
            statePanel = new JPanel();
            statePanel.setPreferredSize(new Dimension(36, 12));
            statePanel.setBackground(peakRule.getState());
            JLabel triggerLabel = new JLabel(
                XEMessages.msgs.getString("Trigger"));
            triggerText = new JLabel(peakRule.getTriggerRuleName());
            detailsButton.setToolTipText(
                XEMessages.msgs.getString("Details_TTT"));

            FormLayout alertLayout = new FormLayout(
                    "l:p:n, 2dlu, l:p:n, 8dlu, l:p:n, 2dlu, l:p:n, 8dlu, l:p:n",
                    "c:d:n");
            PanelBuilder alertBuilder = new PanelBuilder(alertLayout);

            alertBuilder.add(stateLabel,    cc.xy(1, 1));
            alertBuilder.add(statePanel,    cc.xy(3, 1));
            alertBuilder.add(triggerLabel,  cc.xy(5, 1));
            alertBuilder.add(triggerText,   cc.xy(7, 1));
            alertBuilder.add(detailsButton, cc.xy(9, 1));

            TitledBorder alertBorder = new TitledBorder(
                XEMessages.msgs.getString("Last_Alert"));

            alertBuilder.getPanel().setBorder(alertBorder);

            // construct the "dummy lights" rule state buttons
            FormLayout dummyLayout = new FormLayout(dummyCols, "c:p:g");
            PanelBuilder dummyBuilder = new PanelBuilder(dummyLayout);

            // clicking on a rule state button should take you to the
            // rule in question on the rule panel

            int panelsSoFar = 0;

            i = 0;
            x = 1;
            for(Rule r : ruleSet) {
                if (!r.isRuleSupported())
                    continue;

                int howFar = 0;

                if (ruleSubPanelCount > 0)
                    howFar = (panelsSoFar * 100) / ruleSubPanelCount;

                // count rule panels
                if (r instanceof AggregateRule) {
                    AggregateRule ar = (AggregateRule) r;

                    panelsSoFar += ar.getSupportedSubruleCount();
                } else
                    panelsSoFar++;

                dummyLights[i].addActionListener(new DummyAction(howFar));
                dummyBuilder.add(dummyLights[i], cc.xy(x, 1));

                i++;
                x += 2;
            }

            TitledBorder dummyBorder = new TitledBorder(
                XEMessages.msgs.getString("Rule_States"));

            dummyBuilder.getPanel().setBorder(dummyBorder);

            // build the bottom of the host tab
            FormLayout bottomLayout = new FormLayout(
                "l:p:g, 8dlu, l:p:g", "c:p:g");
            bottomLayout.setColumnGroups(new int[][]{{1, 3}});
            PanelBuilder bottomBuilder = new PanelBuilder(bottomLayout);

            bottomBuilder.add(alertBuilder.getPanel(), cc.xy(1, 1));
            bottomBuilder.add(dummyBuilder.getPanel(), cc.xy(3, 1));

            // now put it all together
            builder.setDefaultDialogBorder();
            builder.add(toolbar,                  cc.xy(1, 1));
            builder.add(topProcs.getPane(),       cc.xy(1, 3));
            builder.add(bottomBuilder.getPanel(), cc.xy(1, 5));

            // prime the pump
            setAlertInfo(peakRule, PN.PN_PERCENT_CPU_USE);

            // light em up
            topProcs.start();
            sysTree.start();
            rulePanel.start();

            // open the details window if the profile says so
            if (ttp != null && ttp.detailsWindowBounds != null) {
                detailsPopup.setBounds(ttp.detailsWindowBounds);
                if (ttp.isDetailsWindowVisible)
                    detailsPopup.actionPerformed(null);
            }
        }

        public void setAlertInfo(Rule peakRule, int sortKey) {
            boolean useIntValue = true;
            int intValue = 0;
            double doubleValue = 0.0;
            int problemCode = peakRule.getProblemCode();
            String units = "";

            CmProcessMetrics pm = topProcs.getTopProcess();
            String cl = pm.commandLine.toString();
            if (cl.length() == 0)
                cl = pm.commandName.toString();

            // It's the same problem code but the name of the process has
            // changed. Possible false positive in the works. Return without
            // updating and let the condition settle down if possible and
            // if it comes back again then it's real and it will be updated.

            if (!detailsInfo.stateSettle &&
                (problemCode == detailsInfo.problemCode) &&
                    !cl.equals(detailsInfo.topProcess)) {
                // make note, but don't update
                detailsInfo.stateSettle = true;
                return;
            }

            detailsInfo.time = System.currentTimeMillis();
            detailsInfo.state = peakRule.getState();
            detailsInfo.action = peakRule.getAction();
            detailsInfo.explanation = peakRule.getExplanation();
            detailsInfo.detail = peakRule.getFurtherDetail();
            detailsInfo.solution = peakRule.getSolution();
            detailsInfo.sortKey = topProcs.getSortKeyName();
            detailsInfo.numericSortKey = sortKey;
            detailsInfo.problemCode = problemCode;
            detailsInfo.stateSettle = false;

            switch(sortKey) {
            case PN.PN_CONTEXT_SWITCHES:
                intValue = pm.contextSwitches.intValue();
                break;
            case PN.PN_PERCENT_CPU_USE:
                doubleValue = pm.percentCPUUse.doubleValue();
                useIntValue = false;
                break;
            case PN.PN_PERCENT_KERNEL_TIME:
                doubleValue = pm.percentKernelTime.doubleValue();
                useIntValue = false;
                break;
            case PN.PN_PERCENT_USER_TIME:
                doubleValue = pm.percentUserTime.doubleValue();
                useIntValue = false;
                break;
            case PN.PN_PROCESS_ID:
                intValue = pm.processID.intValue();
                break;
            case PN.PN_RESIDENT_SIZE_KB:
                intValue = pm.residentSizeKB.intValue();
                units = "K";
                break;
            case PN.PN_RESIDENT_SIZE_DELTA_KB:
                intValue = pm.residentSizeDeltaKB.intValue();
                units = "K";
                break;
            case PN.PN_SYSTEM_CALLS:
                intValue = pm.systemCalls.intValue();
                break;
            case PN.PN_TOTAL_IO_KB:
                intValue = pm.totalIOKB.intValue();
                units = "K";
                break;
            case PN.PN_TOTAL_SIZE_KB:
                intValue = pm.totalSizeKB.intValue();
                units = "K";
                break;
            default:
                intValue = -sortKey;
                break;
            }

            detailsInfo.topProcess = cl;
            detailsInfo.topProcessId = pm.processID;

            if (useIntValue)
                detailsInfo.offendingValue = new Integer(intValue);
            else
                detailsInfo.offendingValue = new Double(doubleValue);

            detailsInfo.units = units;

            statePanel.setBackground(peakRule.getState());
            triggerText.setText(peakRule.getTriggerRuleName());
            if (detailsPopup.isVisible())
                detailsPopup.refresh();
        }

        private class TopProcesses extends XETopProcesses {
            public TopProcesses(Container parent, String remoteHost,
                    int msInterval, Object ... args) throws
                        RemoteException, NotBoundException,
                        MalformedURLException, PerformanceException {

                super(parent, remoteHost, msInterval, args);
            }

            public void failureEvent(String message) {
                XEDialogs.warningDialog(message);
                closeAction.actionPerformed(null);
            }
        }

        private class PeakRule implements RuleResolution {
            private int tabIndex;

            private int[] csTries = {
                PN.PN_CONTEXT_SWITCHES,
                PN.PN_SYSTEM_CALLS,
                PN.PN_PERCENT_KERNEL_TIME,
                PN.PN_PERCENT_CPU_USE
            };
            private int[] scTries = {
                PN.PN_SYSTEM_CALLS,
                PN.PN_PERCENT_KERNEL_TIME,
                PN.PN_PERCENT_CPU_USE
            };
            private int[] ktTries = {
                PN.PN_PERCENT_KERNEL_TIME,
                PN.PN_PERCENT_CPU_USE
            };
            private int[] utTries = {
                PN.PN_PERCENT_USER_TIME,
                PN.PN_PERCENT_CPU_USE
            };
            private int[] ioTries = {
                PN.PN_TOTAL_IO_KB,
                PN.PN_SYSTEM_CALLS,
                PN.PN_PERCENT_KERNEL_TIME,
                PN.PN_PERCENT_CPU_USE
            };
            private int[] muxTries = {
                PN.PN_NUMBER_OF_THREADS,
                PN.PN_PERCENT_KERNEL_TIME,
                PN.PN_PERCENT_CPU_USE
            };
            private int[] pagingTries = {
                PN.PN_RESIDENT_SIZE_KB,
                PN.PN_RESIDENT_SIZE_DELTA_KB
            };

            public PeakRule() {
                tabIndex = -1;
            }

            private int cascade(int[] flags) throws PerformanceException {
                XETopProcesses topProcs = HostWindow.this.getProcs();

                for(int i=0; i<flags.length; i++) {
                    if (topProcs.attemptToSortBy(flags[i], true))
                        return flags[i];
                }

                topProcs.attemptToSortBy(PN.PN_PROCESS_ID, true);
                return PN.PN_PROCESS_ID;
            }

            public void resolve(Rule peakRule) {
                XETopProcesses topProcs = HostWindow.this.getProcs();

                if (tabIndex == -1)
                    tabIndex = topTabbedPane.indexOfComponent(HostWindow.this);

                if (tabIndex != -1) {
                    // tab went away but the Timer fired anyway
                    if (tabIndex < 0 || tabIndex >= topTabbedPane.getTabCount()) {

                        // This should not be necessary, but CloseTop asked
                        // it to stop and it apparently didn't feel like stopping
                        // right now. So, it will be asked until it complies.

                        Timer t = HostWindow.this.getRulePanel();

                        if (t.isRunning())
                            t.stop();

                        return;
                    }
                    StateIcon i = (StateIcon) topTabbedPane.getIconAt(tabIndex);
                    i.setStateColor(peakRule.getState());
                    topTabbedPane.repaint();
                }

                if (!autoSort)
                    return;

                int c = peakRule.getProblemCode();
                int sortKey = -1;

                try {
                    switch(c) {
                    case PureCPURule.HIGH_CONTEXT_SWITCHES:
                        sortKey = cascade(csTries);
                        break;
                    case PureCPURule.HIGH_SYSTEM_CALLS:
                        sortKey = cascade(scTries);
                        break;
                    case PureCPURule.HIGH_MUTEX_SPINS:
                        sortKey = cascade(muxTries);
                        break;
                    case PureCPURule.HIGH_SYSTEM_TIME:
                    case PureCPURule.HIGH_INTERRUPTS:
                        sortKey = cascade(ktTries);
                        break;
                    case PureCPURule.HIGH_RUNQ:
                    case PureCPURule.CPU_HOG:
                    case PureCPURule.CPU_SPIKE:
                    case PureCPURule.CPU_IMBALANCE:
                        sortKey = cascade(utTries);
                        break;
                    case PureCPURule.HIGH_WAIT_TIME:
                    case PureDiskRule.HIGH_SERVICE_TIME:
                    case PureDiskRule.HIGH_BUSY_TIME:
                    case PureDiskRule.DISK_IMBALANCE:
                    case PureNICRule.NETIF_IMBALANCE:
                    case PureNICRule.INTERFACE_ERRORS:
                    case PureNICRule.INTERFACE_FAILURES:
                    case PureNICRule.INTERFACE_COLLISIONS:
                    case PureNICRule.INTERFACE_UTILIZATION:
                    case PureNICRule.INTERFACE_IMBALANCE:
                    case PureCacheRule.POOR_CACHE_UTILIZATION:
                    case PureTCPRule.HIGH_SEGMENT_RETRANSMISSION:
                    case PureTCPRule.HIGH_ATTEMPT_FAILURES:
                    case PureTCPRule.HIGH_INPUT_ERRORS:
                    case PureTCPRule.HIGH_RST_SEGMENT_RATE:
                        sortKey = cascade(ioTries);
                        break;
                    case PureMemoryRule.PAGING_OUT:
                    case PureMemoryRule.PAGING_IN:
                        sortKey = cascade(pagingTries);
                        break;
                    case PurePageFileRule.PAGE_SPACE_LOW:
                    case PurePageFileRule.PAGE_SPACE_GONE:
                        sortKey = PN.PN_TOTAL_SIZE_KB;
                        topProcs.attemptToSortBy(sortKey, true);
                        break;
                    case Rule.NO_PROBLEM_DETECTED:
                        sortKey = PN.PN_PERCENT_CPU_USE;
                        topProcs.attemptToSortBy(sortKey, true);
                        topProcs.cancelOffensiveWatch();
                        break;
                    }
                } catch(PerformanceException e) {
                    XEDialogs.warningDialog(e.getMessage());
                    closeAction.actionPerformed(null);
                    return;
                }

                if (c != Rule.NO_PROBLEM_DETECTED) {
                    CmProcessMetrics p = topProcs.getTopProcess();

                    topProcs.watchOffensiveProcess(
                        p.processID.longValue(), peakRule.getState());
                    HostWindow.this.setAlertInfo(peakRule, sortKey);
                }

                int x = 0;
                for(Rule r : ruleSet) {
                    if (!r.isRuleSupported())
                        continue;

                    dummyLights[x].setBackground(r.getState());
                    x++;
                }
            }

            protected void finalize() {
                if (discussFinalization)
                    System.out.println("PeakRule.finalize");
            }
        }

        /*
         * auto sort option on option bar
         */
        private class AutoSort extends JCheckBox implements ActionListener {
            public AutoSort(String text) {
                super(text, autoSort);
                addActionListener(this);
                setToolTipText(XEMessages.msgs.getString("Auto_Sort_TTT"));
            }

            public void actionPerformed(ActionEvent ae) {
                JCheckBox cBox = (JCheckBox) ae.getSource();

                autoSort = cBox.isSelected();
            }

            protected void finalize() {
                if (discussFinalization)
                    System.out.println("AutoSort.finalize");
            }
        }

        /*
         * sys procs option on option bar
         */
        private class SystemProcs extends JCheckBox implements ActionListener {
            public SystemProcs(String text) {
                super(text, zeroProcs);
                addActionListener(this);
                setToolTipText(XEMessages.msgs.getString("Sys_Procs_TTT"));
            }

            public void actionPerformed(ActionEvent ae) {
                JCheckBox cBox = (JCheckBox) ae.getSource();

                zeroProcs = cBox.isSelected();
                topProcs.setViewSystemProcs(zeroProcs);
            }

            protected void finalize() {
                if (discussFinalization)
                    System.out.println("SystemProcs.finalize");
            }
        }

        private class DetailsInfo {
            public long    time;
            public Color   state;
            public String  action;
            public String  explanation;
            public String  detail;
            public String  solution;
            public String  sortKey;
            public String  topProcess;
            public CmLong  topProcessId;
            public Number  offendingValue;
            public String  units;
            public int     numericSortKey;
            public int     problemCode;
            public boolean stateSettle;

            public DetailsInfo() {
                problemCode = -1;
                stateSettle = false;
            }

            protected void finalize() {
                if (discussFinalization)
                    System.out.println("DetailsInfo.finalize");
            }
        }

        private class DetailsPopup implements ActionListener {
            private JFrame      frame = null;
            private JLabel      timeText = null;
            private JPanel      statePanel = null;
            private JLabel      actionText = null;
            private JLabel      explainText = null;
            private JLabel      detailText = null;
            private JLabel      solutionText = null;
            private JLabel      sortKeyText = null;
            private JTextField  topProcessText = null;
            private JLabel      topPidText = null;
            private JLabel      offValueText = null;
            private Rectangle   bounds = null;

            public void erase() {
                if (frame != null) {
                    frame.setVisible(false);
                    frame.dispose();
                }
                frame = null;
                timeText = null;
                statePanel = null;
                actionText = null;
                explainText = null;
                detailText = null;
                solutionText = null;
                sortKeyText = null;
                topProcessText = null;
                topPidText = null;
                offValueText = null;
                bounds = null;
            }

            public void actionPerformed(ActionEvent ae) {
                if (frame == null) {
                    FormLayout layout = new FormLayout(
                        "l:p:g, 4dlu, l:p:n",
                        "c:p:g, 2dlu, c:p:g, 2dlu, c:p:g, 2dlu, " +
                        "c:p:g, 2dlu, c:p:g, 2dlu, c:p:g, 2dlu, " +
                        "c:p:g, 2dlu, c:p:g, 2dlu, c:p:g, 2dlu, " +
                        "c:p:g");
                    PanelBuilder builder = new PanelBuilder(layout);
                    CellConstraints cc = new CellConstraints();

                    frame = new JFrame(
                        XEMessages.msgs.getString("Last_Alert") +
                        ':' + ' ' + remoteHost);
                    JLabel time = new JLabel(
                        XEMessages.msgs.getString("Time_Label"));
                    JLabel state = new JLabel(
                        XEMessages.msgs.getString("State"));
                    JLabel action = new JLabel(
                        XEMessages.msgs.getString("Action"));
                    JLabel explain = new JLabel(
                        XEMessages.msgs.getString("Explanation"));
                    JLabel detail = new JLabel(
                        XEMessages.msgs.getString("Detail"));
                    JLabel solution = new JLabel(
                        XEMessages.msgs.getString("Solution"));
                    JLabel sortKey = new JLabel(
                        XEMessages.msgs.getString("Sort_Key"));
                    JLabel topProcess = new JLabel(
                        XEMessages.msgs.getString("Top_Process"));
                    JLabel topPid = new JLabel(
                        XEMessages.msgs.getString("Top_Pid"));
                    JLabel offValue = new JLabel(
                        XEMessages.msgs.getString("Off_Value"));

                    timeText = new JLabel();
                    statePanel = new JPanel();
                    statePanel.setPreferredSize(new Dimension(72, 12));
                    actionText = new JLabel();
                    explainText = new JLabel();
                    detailText = new JLabel();
                    solutionText = new JLabel();
                    sortKeyText = new JLabel();
                    topProcessText = new JTextField();
                    topProcessText.setEditable(false);
                    topPidText = new JLabel();
                    offValueText = new JLabel();

                    builder.add(time,           cc.xy(1, 1));
                    builder.add(state,          cc.xy(1, 3));
                    builder.add(action,         cc.xy(1, 5));
                    builder.add(explain,        cc.xy(1, 7));
                    builder.add(detail,         cc.xy(1, 9));
                    builder.add(solution,       cc.xy(1, 11));
                    builder.add(sortKey,        cc.xy(1, 13));
                    builder.add(offValue,       cc.xy(1, 15));
                    builder.add(topProcess,     cc.xy(1, 17));
                    builder.add(topPid,         cc.xy(1, 19));

                    builder.add(timeText,       cc.xy(3, 1));
                    builder.add(statePanel,     cc.xy(3, 3));
                    builder.add(actionText,     cc.xy(3, 5));
                    builder.add(explainText,    cc.xy(3, 7));
                    builder.add(detailText,     cc.xy(3, 9));
                    builder.add(solutionText,   cc.xy(3, 11));
                    builder.add(sortKeyText,    cc.xy(3, 13));
                    builder.add(offValueText,   cc.xy(3, 15));
                    builder.add(topProcessText, cc.xy(3, 17));
                    builder.add(topPidText,     cc.xy(3, 19));

                    JButton closeButton = new JButton(XEMessages.msgs.getString("Close"));
                    closeButton.setToolTipText(
                        XEMessages.msgs.getString("Close_Window"));
                    closeButton.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent ae) {
                            frame.setVisible(false);
                        }
                    });

                    FormLayout popLayout = new FormLayout(
                        "c:p:g", "c:p:g, 4dlu, c:p:n");
                    PanelBuilder popBuilder = new PanelBuilder(popLayout);

                    popBuilder.add(builder.getPanel(), cc.xy(1, 1));
                    popBuilder.add(closeButton,        cc.xy(1, 3));

                    popBuilder.setDefaultDialogBorder();
                    frame.setContentPane(popBuilder.getPanel());
                }

                refresh(true);
                if (bounds != null) {
                    frame.setBounds(bounds);
                    bounds = null;
                }
                frame.setVisible(true);
            }

            private void refresh(boolean recompute) {
                timeText.setText(
                    MessageFormat.format(
                       "{0,date} {0,time}", detailsInfo.time));
                statePanel.setBackground(detailsInfo.state);
                actionText.setText(detailsInfo.action);
                explainText.setText(detailsInfo.explanation);
                detailText.setText(detailsInfo.detail);
                solutionText.setText(detailsInfo.solution);
                sortKeyText.setText(detailsInfo.sortKey);
                topProcessText.setText(detailsInfo.topProcess);
                int n = detailsInfo.topProcess.length();
                topProcessText.setColumns(n < 32 ? n : 32);
                topProcessText.setCaretPosition(0);
                topPidText.setText(detailsInfo.topProcessId.toString());
                offValueText.setText(
                    detailsInfo.offendingValue.toString() +
                    detailsInfo.units);

                if (recompute) {
                    frame.pack();

                    Rectangle pr = new Rectangle();
                    Rectangle cr = new Rectangle();

                    pr = topFrame.getBounds(pr);
                    cr = frame.getBounds(cr);
                    pr.x += ((pr.width - cr.width) / 2);
                    pr.y += ((pr.height - cr.height) / 2);
                    pr.width = cr.width;
                    pr.height = cr.height;
                    frame.setBounds(pr);
                }
            }

            public void refresh() {
                refresh(false);
            }

            public boolean isVisible() {
                if (frame == null)
                    return false;
                return frame.isVisible();
            }

            public JFrame getFrame() {
                return frame;
            }

            public void setBounds(Rectangle r) {
                bounds = r;
            }

            protected void finalize() {
                if (discussFinalization)
                    System.out.println("DetailsPopup.finalize");
            }
        }

        private class DummyAction implements ActionListener {
            private int percent;

            public DummyAction(int percent) {
                this.percent = percent;
            }

            public void actionPerformed(ActionEvent ae) {
                HostWindow.this.setSelectedIndex(3);
                JScrollBar sb = ruleScroller.getVerticalScrollBar();
                double max = (double) sb.getMaximum();
                double pct = (double) percent / 100.0;

                max *= pct;
                sb.setValue((int) max);
            }

            protected void finalize() {
                if (discussFinalization)
                    System.out.println("DummyAction.finalize");
            }
        }

        protected void finalize() {
            if (discussFinalization)
                System.out.println("HostWindow.finalize");
        }

        /*
         * Encapsulated logic for popping up a frame with a slider
         * control within it for changing the update interval.
         */
        private class IntervalSlider implements ActionListener {
            /**
             * The <code>JFrame</code> container which is popped up and down.
             */
            private JFrame sliderFrame = null;

            /**
             * The text "Interval in seconds: " under the slider control.
             */
            private JLabel label;

            public void erase() {
                if (sliderFrame != null) {
                    sliderFrame.setVisible(false);
                    sliderFrame.dispose();
                }
                sliderFrame = null;
                label = null;
            }

            /**
             * The callback to process action events.
             * @param ae The action event in question.
             */
            public void actionPerformed(ActionEvent ae) {
                if (sliderFrame == null) {
                    JSlider slider;
                    JButton ok;
                    FormLayout layout = new FormLayout(
                            "c:p:g",
                            "t:p:g, 6dlu, c:p:g, 6dlu, b:p:g");
                    PanelBuilder builder = new PanelBuilder(layout);
                    CellConstraints cc = new CellConstraints();

                    builder.setDefaultDialogBorder();

                    sliderFrame = new JFrame(XEMessages.msgs.getString("Set_Interval"));
                    slider = new JSlider(JSlider.HORIZONTAL, 0, 30, topInterval / 1000);
                    label = new JLabel(XEMessages.msgs.getString("Int_Secs") + topInterval / 1000);
                    ok = new JButton("OK");

                    slider.setMinorTickSpacing(1);
                    slider.setMajorTickSpacing(5);
                    slider.setPaintTicks(true);
                    slider.setPaintLabels(true);
                    slider.setLabelTable(slider.createStandardLabels(5));
                    slider.addChangeListener(new SliderMoved());

                    ok.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent ae) {
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

                Rectangle pr = new Rectangle();
                Rectangle cr = new Rectangle();
                pr = topFrame.getBounds(pr);
                cr = sliderFrame.getBounds(cr);
                pr.x += ((pr.width - cr.width) / 2);
                pr.y += ((pr.height - cr.height) / 2);
                pr.width = cr.width;
                pr.height = cr.height;
                sliderFrame.setBounds(pr);

                sliderFrame.setVisible(true);
            }

            /**
             * The handler subclass for dealing with slider events.
             */
            class SliderMoved implements ChangeListener {
                /**
                 * The callback method for dealing with state changes in the
                 * slider control.
                 * @param e The event which caused the state change.
                 */
                public void stateChanged(ChangeEvent e) {
                    JSlider slider = (JSlider) e.getSource();

                    topInterval = slider.getValue() * 1000;
                    if (topInterval == 0)
                        topInterval = 1000;

                    topProcs.stop();
                    topProcs.setDelay(topInterval);
                    topProcs.start();

                    label.setText(XEMessages.msgs.getString("Int_Secs") + topInterval / 1000);
                }
            }

            protected void finalize() {
                if (discussFinalization)
                    System.out.println("IntervalSlider.finalize");
            }
        }

        /*
         * Encapsulated logic for popping up a frame with a slider
         * control within it for changing the number of processes to view.
         */
        private class ProcSlider implements ActionListener {
            /**
             * The <code>JFrame</code> container which is popped up and down.
             */
            private JFrame sliderFrame = null;

            /**
             * The text under the slider control.
             */
            private JLabel label;

            private int visibleProcs = INITIAL_VISIBLE;

            public void erase() {
                if (sliderFrame != null) {
                    sliderFrame.setVisible(false);
                    sliderFrame.dispose();
                }
                sliderFrame = null;
                label = null;
            }

            /**
             * The callback to process action events.
             * @param ae The action event in question.
             */
            public void actionPerformed(ActionEvent ae) {
                if (sliderFrame == null) {
                    JSlider slider;
                    JButton ok;
                    FormLayout layout = new FormLayout(
                            "c:p:g",
                            "t:p:g, 6dlu, c:p:g, 6dlu, b:p:g");

                    PanelBuilder builder = new PanelBuilder(layout);
                    CellConstraints cc = new CellConstraints();
                    int procCount = topProcs.getProcessCount();

                    builder.setDefaultDialogBorder();

                    sliderFrame = new JFrame(XEMessages.msgs.getString("Set_Proc_Count"));
                    slider = new JSlider(JSlider.HORIZONTAL, MIN_VISIBLE, procCount, visibleProcs);
                    label = new JLabel(XEMessages.msgs.getString("Procs_Display") + visibleProcs);
                    ok = new JButton(XEMessages.msgs.getString("OK"));

                    slider.setPaintTicks(false);
                    slider.setPaintLabels(false);
                    slider.setLabelTable(slider.createStandardLabels(5));
                    slider.addChangeListener(new SliderMoved());

                    ok.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent ae) {
                            topProcs.setVisibleProcs(visibleProcs);
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

                Rectangle pr = new Rectangle();
                Rectangle cr = new Rectangle();
                pr = topFrame.getBounds(pr);
                cr = sliderFrame.getBounds(cr);
                pr.x += ((pr.width - cr.width) / 2);
                pr.y += ((pr.height - cr.height) / 2);
                pr.width = cr.width;
                pr.height = cr.height;
                sliderFrame.setBounds(pr);

                sliderFrame.setVisible(true);
            }

            /**
             * The handler subclass for dealing with slider events.
             */
            class SliderMoved implements ChangeListener {
                /**
                 * The callback method for dealing with state changes in the
                 * slider control.
                 * @param e The event which caused the state change.
                 */
                public void stateChanged(ChangeEvent e) {
                    JSlider slider = (JSlider) e.getSource();

                    visibleProcs = slider.getValue();
                    if (visibleProcs == 0)
                        visibleProcs = MIN_VISIBLE;

                    label.setText(XEMessages.msgs.getString("Procs_Display") + visibleProcs);
                }
            }

            protected void finalize() {
                if (discussFinalization)
                    System.out.println("ProcSlider.finalize");
            }
        }

        /*
         * Encapsulated logic for popping up a frame with many
         * check boxes to select which fields to display.
         */
        private class SelectFields implements ActionListener {
            public void actionPerformed(ActionEvent ae) {
                JFrame fieldsPopup = topProcs.getFieldsBoxFrame();

                Rectangle pr = new Rectangle();
                Rectangle tr = new Rectangle();
                tr = topFrame.getBounds(tr);
                pr = fieldsPopup.getBounds(pr);
                pr.x = (tr.x + ((tr.width - pr.width) / 2));
                pr.y = (tr.y + ((tr.height - pr.height) / 2));
                fieldsPopup.setBounds(pr);
                fieldsPopup.setVisible(true);
            }

            protected void finalize() {
                if (discussFinalization)
                    System.out.println("SelectFields.finalize");
            }
        }

        /*
         * Encapsulated logic for popping up a frame with to
         * enter the name of a process to watch.
         */
        private class WatchCommand implements ActionListener {
            private JFrame frame = null;

            public void erase() {
                if (frame != null) {
                    frame.setVisible(false);
                    frame.dispose();
                }
                frame = null;
            }

            private class CmdBoxListener implements ActionListener {
                public void actionPerformed(ActionEvent ev) {
                    String newCommand = (String) topCmdBox.getSelectedItem();
                    String action = ev.getActionCommand();

                    if (action.equals("comboBoxEdited")) {
                        if (newCommand.length() == 0)
                            return;
                        boolean already = false;
                        int n = topCmdBox.getItemCount();

                        for(int i=0; i<n; i++) {
                            if (((String) topCmdBox.getItemAt(i)).equals(newCommand)) {
                                already = true;
                                break;
                            }
                        }
                        if (!already) {
                            topCommands.addElement(newCommand);
                            topCmdBox.addItem(newCommand);
                        }
                    }
                }
            }

            public void actionPerformed(ActionEvent ae) {
                if (frame == null) {
                    FormLayout layout = new FormLayout(
                            "10dlu, c:p:g, 6dlu, c:p:g, 6dlu, c:p:n, 10dlu",
                            "t:p:n, 6dlu, c:p:g, 6dlu, b:p:g");
                    PanelBuilder builder = new PanelBuilder(layout);
                    CellConstraints cc = new CellConstraints();
                    JLabel label = new JLabel(XEMessages.msgs.getString("Cmd_Name"));
                    JButton apply = new JButton(XEMessages.msgs.getString("Apply"));
                    JButton close = new JButton(XEMessages.msgs.getString("Close"));

                    apply.setToolTipText(XEMessages.msgs.getString("Press_Apply"));
                    close.setToolTipText(XEMessages.msgs.getString("Close_Window"));

                    if (topCommands.size() > 0) {
                        String[] cmds = new String[topCommands.size()];

                        cmds = topCommands.toArray(cmds);
                        topCmdBox = new JComboBox(cmds);
                    } else
                        topCmdBox = new JComboBox();

                    topCmdBox.setEditable(true);
                    topCmdBox.addActionListener(new CmdBoxListener());

                    apply.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent ae) {
                            String cmd = (String) topCmdBox.getSelectedItem();

                            if (cmd.length() > 0) {
                                topProcs.watchProcessName(cmd);
                                saveConfigFile();
                            }
                        }
                    });
                    close.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent ae) {
                            frame.setVisible(false);
                        }
                    });

                    frame = new JFrame(XEMessages.msgs.getString("Cmd_Watch"));

                    builder.setDefaultDialogBorder();

                    builder.add(label, cc.xyw(2, 1, 6));
                    builder.add(topCmdBox, cc.xyw(1, 3, 7));
                    builder.add(apply, cc.xy(4, 5));
                    builder.add(close, cc.xy(6, 5));

                    frame.setContentPane(builder.getPanel());
                    frame.pack();
                }

                Rectangle pr = new Rectangle();
                Rectangle tr = new Rectangle();
                tr = topFrame.getBounds(tr);
                pr = frame.getBounds(pr);
                pr.x = (tr.x + ((tr.width - pr.width) / 2));
                pr.y = (tr.y + ((tr.height - pr.height) / 2));
                frame.setBounds(pr);
                frame.setVisible(true);
            }

            protected void finalize() {
                if (discussFinalization)
                    System.out.println("WatchCommand.finalize");
            }
        }

        /*
         * close option on menu which closes this window.
         */
        private class CloseTop implements ActionListener {
            public void actionPerformed(ActionEvent ae) {
                boolean closeWindow = false;

                getProcs().stop();
                getGadgets().stop();
                getSysTree().stop();
                getRulePanel().stop();

                topTabbedPane.remove(HostWindow.this);
                if (topTabbedPane.getTabCount() == 0) {
                    closeWindow = true;
                    for(int i=0; i<topWindows.size(); i++) {
                        if (topWindows.elementAt(i) == topFrame) {
                            topWindows.removeElementAt(i);
                            break;
                        }
                    }
                }

                if (closeWindow) {
                    topFrame.setVisible(false);
                    topFrame.dispose();
                    topFrame = null;
                }

                // force a recursive decent clean-up until out of Swing
                HostWindow.this.erase();
                RMIAccessPoint.removeAccessPoint(new CaptiveConnection(remoteHost));

                if (topWindows.size() == 0)
                    System.exit(0);
            }

            protected void finalize() {
                if (discussFinalization)
                    System.out.println("CloseTop.finalize");
            }
        }

        /*
         * Construct the XE Gadgets tab.
         */
        private class XEGadgets {
            //
            // Private members
            //

            private final JDesktopPane desktop = new JDesktopPane();
            private String           remoteHost;
            private GadgetProperties gp;
            private Container        pane;
            private JButton          cpuPie = null;
            private JButton          memPie = null;
            private JButton          pfPie = null;
            private GadgetListener   gadgetListener;

            //
            // Public methods
            //

            public void erase() {
                pane = null;
                for(ActionListener ae : cpuPie.getActionListeners())
                    cpuPie.removeActionListener(ae);
                for(ActionListener ae : memPie.getActionListeners())
                    memPie.removeActionListener(ae);
                for(ActionListener ae : pfPie.getActionListeners())
                    pfPie.removeActionListener(ae);
                cpuPie = null;
                memPie = null;
                pfPie = null;
                if (gadgetListener != null)
                    gadgetListener.erase();
                gadgetListener = null;
            }

            public GadgetProperties getProperties() {
                return gadgetListener.getProperties();
            }

            public void start() {
                gadgetListener.start();
            }

            public void stop() {
                gadgetListener.stop();
            }

            private class GadgetListener implements ActionListener {
                private JInternalFrame     cpuChartFrame = null;
                private JInternalFrame     memChartFrame = null;
                private JInternalFrame     pfChartFrame = null;
                private Rectangle          cpuChartBounds;
                private Rectangle          memChartBounds;
                private Rectangle          pfChartBounds;
                private XECpuPieChart      cpuChart;
                private XEMemoryPieChart   memChart;
                private XEPageFilePieChart pfChart;
                private GadgetProperties   props;
                private GadgetProperties   initialProps;

                public void erase() {
                    if (cpuChartFrame != null) {
                        cpuChartFrame.setVisible(false);
                        cpuChartFrame.dispose();
                    }
                    cpuChartFrame = null;

                    if (memChartFrame != null) {
                        memChartFrame.setVisible(false);
                        memChartFrame.dispose();
                    }
                    memChartFrame = null;

                    if (pfChartFrame != null) {
                        pfChartFrame.setVisible(false);
                        pfChartFrame.dispose();
                    }
                    pfChartFrame = null;

                    cpuChartBounds = null;
                    memChartBounds = null;
                    pfChartBounds = null;

                    if (cpuChart != null)
                        cpuChart.erase();
                    cpuChart = null;

                    if (memChart != null)
                        memChart.erase();
                    memChart = null;

                    if (pfChart != null)
                        pfChart.erase();
                    pfChart = null;

                    props = null;
                    initialProps = null;
                }

                public void start() {
                    if (cpuChart != null)
                        cpuChart.start();
                    if (memChart != null)
                        memChart.start();
                    if (pfChart != null)
                        pfChart.start();
                }

                public void stop() {
                    if (cpuChart != null)
                        cpuChart.stop();
                    if (memChart != null)
                        memChart.stop();
                    if (pfChart != null)
                        pfChart.stop();
                }

                public GadgetProperties getProperties() {
                    if (cpuChartFrame != null) {
                        props.cpuPieCreated = true;
                        props.cpuPieClosed = cpuChartFrame.isClosed();
                        props.cpuPieBounds = new Rectangle();
                        cpuChartFrame.getBounds(props.cpuPieBounds);
                        props.cpuProperties = cpuChart.getProperties();
                    }
                    if (memChartFrame != null) {
                        props.memPieCreated = true;
                        props.memPieClosed = memChartFrame.isClosed();
                        props.memPieBounds = new Rectangle();
                        memChartFrame.getBounds(props.memPieBounds);
                        props.memProperties = memChart.getProperties();
                    }
                    if (pfChartFrame != null) {
                        props.pfPieCreated = true;
                        props.pfPieClosed = pfChartFrame.isClosed();
                        props.pfPieBounds = new Rectangle();
                        pfChartFrame.getBounds(props.pfPieBounds);
                        props.pfProperties = pfChart.getProperties();
                    }
                    return props;
                }

                public GadgetListener() {
                    props = new GadgetProperties();
                    initialProps = gp;
                    if (initialProps != null) {
                        if (initialProps.cpuPieCreated)
                            doCpu();
                        if (initialProps.memPieCreated)
                            doMem();
                        if (initialProps.pfPieCreated)
                            doPf();
                    }
                    initialProps = null;
                }

                public void actionPerformed(ActionEvent ev) {
                    JButton source = (JButton) ev.getSource();

                    if (source == cpuPie)
                        doCpu();
                    else if (source == memPie)
                        doMem();
                    else if (source == pfPie)
                        doPf();
                }

                private void setBounds(JInternalFrame frame) {
                    JInternalFrame[] frames = desktop.getAllFrames();
                    int n = frames.length + 1;
                    int ym = (n-1) / 3;
                    int xm = (n-1) % 3;
                    int y = ym * (GADGET_HEIGHT + GADGET_PAD);
                    int x = xm * (GADGET_WIDTH + GADGET_PAD);

                    frame.setBounds(x, y, GADGET_WIDTH, GADGET_HEIGHT);
                }

                private abstract class InternalClosing implements InternalFrameListener {
                    public void internalFrameOpened(InternalFrameEvent e) {}
                    public void internalFrameClosed(InternalFrameEvent e) {}
                    public void internalFrameIconified(InternalFrameEvent e) {}
                    public void internalFrameDeiconified(InternalFrameEvent e) {}
                    public void internalFrameActivated(InternalFrameEvent e) {}
                    public void internalFrameDeactivated(InternalFrameEvent e) {}
                }

                private void doCpu() {
                    if (cpuChartFrame == null) {
                        try {
                            cpuChartFrame = new JInternalFrame(
                                    XEMessages.msgs.getString("CPU"), true, true, true, true);
                            if (initialProps == null)
                                cpuChart = new XECpuPieChart(cpuChartFrame, remoteHost, SHORT_INTERVAL);
                            else
                                cpuChart = new XECpuPieChart(cpuChartFrame, remoteHost, SHORT_INTERVAL,
                                    initialProps.cpuProperties);
                            cpuChartFrame.addInternalFrameListener(new InternalClosing() {
                                public void internalFrameClosing(InternalFrameEvent e) {
                                    cpuChartBounds = cpuChartFrame.getBounds();
                                    cpuChart.stop();
                                }
                            });
                            cpuChartFrame.setContentPane(cpuChart.getPane());
                            cpuChartFrame.setVisible(true);
                            if (initialProps == null)
                                setBounds(cpuChartFrame);
                            else {
                                cpuChartFrame.setBounds(initialProps.cpuPieBounds);
                                if (initialProps.cpuPieClosed) {
                                    try {
                                        cpuChartFrame.setClosed(true);
                                    } catch(Exception e) {
                                    }
                                }
                            }
                            desktop.add(cpuChartFrame, new Integer(1));
                            cpuChart.start();
                        } catch(Exception e) {
                            XEDialogs.fatalDialog(new CmExceptionParser(e).getMessage());
                        }
                    } else if (cpuChartFrame.isClosed()) {
                        cpuChartFrame.setBounds(cpuChartBounds);
                        desktop.add(cpuChartFrame, new Integer(1));
                        cpuChart.start();
                        cpuChartFrame.setVisible(true);
                    }
                }

                private void doMem() {
                    if (memChartFrame == null) {
                        try {
                            memChartFrame = new JInternalFrame(
                                    XEMessages.msgs.getString("Memory"), true, true, true, true);
                            if (initialProps == null)
                                memChart = new XEMemoryPieChart(topFrame, remoteHost, LONG_INTERVAL);
                            else
                                memChart = new XEMemoryPieChart(topFrame, remoteHost, LONG_INTERVAL,
                                    initialProps.memProperties);
                            memChartFrame.addInternalFrameListener(new InternalClosing() {
                                public void internalFrameClosing(InternalFrameEvent e) {
                                    memChartBounds = memChartFrame.getBounds();
                                    memChart.stop();
                                }
                            });
                            memChartFrame.setContentPane(memChart.getPane());
                            memChartFrame.setVisible(true);
                            if (initialProps == null) {
                                setBounds(memChartFrame);
                                memChartFrame.setVisible(true);
                            } else {
                                memChartFrame.setBounds(initialProps.memPieBounds);
                                if (initialProps.memPieClosed) {
                                    try {
                                        memChartFrame.setClosed(true);
                                    } catch(Exception e) {
                                    }
                                }
                            }
                            desktop.add(memChartFrame, new Integer(1));
                            memChart.start();
                        } catch(Exception e) {
                            memChart = null;
                            XEDialogs.fatalDialog(new CmExceptionParser(e).getMessage());
                        }
                    } else if (memChartFrame.isClosed()) {
                        memChartFrame.setBounds(memChartBounds);
                        desktop.add(memChartFrame, new Integer(1));
                        memChart.start();
                        memChartFrame.setVisible(true);
                    }
                }

                private void doPf() {
                    if (pfChartFrame == null) {
                        try {
                            pfChartFrame = new JInternalFrame(
                                    XEMessages.msgs.getString("Page_File"), true, true, true, true);
                            if (initialProps == null)
                                pfChart = new XEPageFilePieChart(topFrame, remoteHost, LONG_INTERVAL);
                            else
                                pfChart = new XEPageFilePieChart(topFrame, remoteHost, LONG_INTERVAL,
                                    initialProps.pfProperties);
                            pfChartFrame.addInternalFrameListener(new InternalClosing() {
                                public void internalFrameClosing(InternalFrameEvent e) {
                                    pfChartBounds = pfChartFrame.getBounds();
                                    pfChart.stop();
                                }
                            });
                            pfChartFrame.setContentPane(pfChart.getPane());
                            pfChartFrame.setVisible(true);
                            if (initialProps == null) {
                                setBounds(pfChartFrame);
                                pfChartFrame.setVisible(true);
                            } else {
                                pfChartFrame.setBounds(initialProps.pfPieBounds);
                                if (initialProps.pfPieClosed) {
                                    try {
                                        pfChartFrame.setClosed(true);
                                    } catch(Exception e) {
                                    }
                                }
                            }
                            desktop.add(pfChartFrame, new Integer(1));
                            pfChart.start();
                        } catch(Exception e) {
                            pfChart = null;
                            XEDialogs.fatalDialog(new CmExceptionParser(e).getMessage());
                        }
                    } else if (pfChartFrame.isClosed()) {
                        pfChartFrame.setBounds(pfChartBounds);
                        desktop.add(pfChartFrame, new Integer(1));
                        pfChart.start();
                        pfChartFrame.setVisible(true);
                    }
                }
            }

            public XEGadgets(final String remoteHost, GadgetProperties gp) {
                this.remoteHost = remoteHost;
                this.gp = gp;

                desktop.setOpaque(true);

                String imgPath = "com/capmet/tools/XE/images/CpuPie.png";
                URL imageURL = getClass().getClassLoader().getResource(imgPath);
                ImageIcon iIcon = null;
                if (imageURL != null)
                    iIcon = new ImageIcon(imageURL);

                if (iIcon != null)
                    cpuPie = new JButton(iIcon);
                else
                    cpuPie = new JButton(XEMessages.msgs.getString("CPU_Pie"));

                cpuPie.setToolTipText(XEMessages.msgs.getString("CPU_Pie_TTT"));

                gadgetListener = new GadgetListener();
                cpuPie.addActionListener(gadgetListener);

                imgPath = "com/capmet/tools/XE/images/MemoryPie.png";
                imageURL = getClass().getClassLoader().getResource(imgPath);
                iIcon = null;
                if (imageURL != null)
                    iIcon = new ImageIcon(imageURL);

                if (iIcon != null)
                    memPie = new JButton(iIcon);
                else
                    memPie = new JButton(XEMessages.msgs.getString("Memory_Pie"));

                memPie.setToolTipText(XEMessages.msgs.getString("Memory_Pie_TTT"));
                memPie.addActionListener(gadgetListener);

                imgPath = "com/capmet/tools/XE/images/PageFilePie.png";
                imageURL = getClass().getClassLoader().getResource(imgPath);
                iIcon = null;
                if (imageURL != null)
                    iIcon = new ImageIcon(imageURL);

                if (iIcon != null)
                    pfPie = new JButton(iIcon);
                else
                    pfPie = new JButton(XEMessages.msgs.getString("PF_Pie"));

                pfPie.setToolTipText(XEMessages.msgs.getString("PF_Pie_TTT"));
                pfPie.addActionListener(gadgetListener);

                JPanel menuPanel = new JPanel();
                menuPanel.add(cpuPie,   BorderLayout.WEST);
                menuPanel.add(memPie,   BorderLayout.CENTER);
                menuPanel.add(pfPie,    BorderLayout.EAST);

                JSplitPane jsp = new JSplitPane(JSplitPane.VERTICAL_SPLIT, menuPanel, desktop);
                JScrollPane scroller = new JScrollPane(jsp);

                pane = scroller;
            }

            /**
             * Retrieve the <CODE>Container</CODE> holding all of the entities.
             * @return The <CODE>Container</CODE> used by the layout manager where
             * all of the graphical entities are located.
             */
            public Container getPane() {
                return pane;
            }

            protected void finalize() {
                if (discussFinalization)
                    System.out.println("XEGadgets.finalize");
            }
        }
    }

    private String expandClusterNames(String input) {
        String result = null;

        for(String hostName : input.split(",")) {
            if (topClusters.containsKey(hostName)) {
                CommandVector cv = topClusters.get(hostName);
                String hosts = null;

                for(String h : cv)
                    if (hosts == null)
                        hosts = h;
                    else
                        hosts = hosts + "," + h;

                hostName = expandClusterNames(hosts);
            }

            if (result == null)
                result = hostName;
            else
                result = result + "," + hostName;
        }

        return result;
    }

    private class AddHost implements ActionListener {
        private boolean newFrame;

        public AddHost(boolean newFrame) {
            this.newFrame = newFrame;
        }

        public void actionPerformed(ActionEvent ev) {
            String input = XEDialogs.inputDialog(topFrame,
                XEMessages.msgs.getString("Enter_Host"),
                    XEMessages.msgs.getString("New_Host"));

            if (input == null)
                return;

            if (newFrame)
                new Top(input, null);
            else {
                for(String hostName : expandClusterNames(input).split(",")) {
                    try {
                        HostWindow w = new HostWindow(hostName, null);
                        Color state = w.getRulePanel().getPeakRule().getState();
                        topTabbedPane.addTab(hostName, new StateIcon(state), w);
                    } catch(Exception e) {
                        XEDialogs.warningDialog(new CmExceptionParser(e).getMessage());
                    }
                }
            }
        }
    }

    private class AddTitle implements ActionListener {
        public void actionPerformed(ActionEvent ev) {
            String title = XEDialogs.inputDialog(topFrame,
                XEMessages.msgs.getString("Enter_Title"),
                    XEMessages.msgs.getString("New_Title_Title"));

            if (title == null)
                return;

            topFrame.setTitle(title);
        }
    }

    private class HelpCallback implements ActionListener {
        public void actionPerformed(ActionEvent ae) {
            String top = System.getProperty("com.capmet.CaptiveCollector.Top");

            if (top == null) {
                XEDialogs.warningDialog(XEMessages.msgs.getString("No_Top"));
                return;
            }
            String helpURL = "file://" + top + "/docs/User_Manual/XE/Top.html";

            try {
                new XEBrowserLauncher(helpURL);
            } catch(Exception e) {
                XEDialogs.warningDialog(e.getMessage());
                return;
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
            final String imagePath = "com/capmet/tools/XE/images/top.png";
            final String msg =
                    "<html><c>Top v1.2<br>by Captive Metrics Software Corporation</c></html>";

            XEDialogs.aboutDialog(topFrame, msg, imagePath);
        }
    }

    private class AddProfile implements ActionListener {
        public void actionPerformed(ActionEvent ev) {
            try {
                String profName = XEDialogs.inputDialog(topFrame,
                    XEMessages.msgs.getString("Enter_Prof_Name"),
                        XEMessages.msgs.getString("Save_Profile"));

                if (profName == null)
                    return;

                if (topProps.windowProfiles.containsKey(profName)) {
                    int result = XEDialogs.questionDialog(
                        topFrame, profName + ':' + ' ' +
                            XEMessages.msgs.getString("Already_Exists"));
                    if (result == JOptionPane.NO_OPTION)
                        return;
                    topProps.windowProfiles.remove(profName);
                }

                TWPVector tv = new TWPVector();

                for(JFrame frame : topWindows) {
                    // the tab for each host with the multiple tabs under it
                    JTabbedPane hostTab = (JTabbedPane) frame.getContentPane();
        
                    // bounds of the JFrame
                    TopWindowProperties twp = new TopWindowProperties();
                    twp.windowTitle = frame.getTitle();
                    twp.windowBounds = new Rectangle();
                    frame.getBounds(twp.windowBounds);
        
                    // a map of host = host-tab properties
                    twp.tabMap = new TopTabMap();
        
                    // for each host in the host-tab
                    for(int i=0, n=hostTab.getTabCount(); i<n; i++) {
                        // the name of the host
                        String hostName = hostTab.getTitleAt(i);
        
                        // the JTabbedPane for the specific host
                        HostWindow hostWindow = (HostWindow) hostTab.getComponentAt(i);
        
                        // get the properties for the tabs on this host-tab
                        TopTabProperties ttp = new TopTabProperties();
                        ttp.topProcsProperties = hostWindow.getProcs().getProperties();
                        ttp.gadgetProperties   = hostWindow.getGadgets().getProperties();
                        ttp.sysTreeProperties  = hostWindow.getSysTree().getProperties();
                        ttp.rulePanelProperties = hostWindow.getRulePanel().getProperties();
                        JFrame df = hostWindow.getDetailsFrame();

                        if (df != null) {
                            ttp.isDetailsWindowVisible = df.isVisible();
                            ttp.detailsWindowBounds = df.getBounds();
                        }
        
                        twp.tabMap.put(hostName, ttp);
                    }
        
                    // get a new top-level windows properties
                    tv.addElement(twp);
                }

                topProps.windowProfiles.put(profName, tv);

                saveConfigFile();

                XEDialogs.infoDialog(topFrame, XEMessages.msgs.getString("Profile_Saved"));

                for(JFrame frame : topWindows) {
                    JMenuBar mb = frame.getJMenuBar();
                    JMenu tm = (JMenu) mb.getComponent(1);
                    JMenu lm = (JMenu) tm.getMenuComponent(1);
                    int n = lm.getComponentCount();
                    JMenuItem mi = new JMenuItem(profName);

                    if (n == 0) {
                        ActionListener lp = new ActionListener() {
                            public void actionPerformed(ActionEvent ae) {
                                loadProfile(ae.getActionCommand());
                            }
                        };

                        lm.setEnabled(true);
                        mi.addActionListener(lp);
                    } else {
                        JMenuItem emi = (JMenuItem) lm.getComponent(0);
                        ActionListener[] al = emi.getActionListeners();

                        mi.addActionListener(al[0]);
                    }
                    lm.add(mi);
                }
            } catch(Exception e) {
                XEDialogs.warningDialog(new CmExceptionParser(e).getMessage());
            }
        }
    }

    private class ClusterEdit implements ActionListener {
        private boolean    CBSurgury;
        private JComboBox  clusterBox;
        private JList      hostList;
        private JTextField hostField;
        private JFrame     frame;
        private JButton    deleteButton;
        private JButton    clusterDelete;
        private JLabel     msgBar;

        public void erase() {
            clusterBox = null;
            hostList = null;
            hostField = null;
            if (frame != null) {
                frame.setVisible(false);
                frame.dispose();
            }
            frame = null;
            deleteButton = null;
            clusterDelete = null;
            msgBar = null;
        }

        public ClusterEdit() {
            frame = null;
        }

        private boolean hasRecursiveCall(String clusterName, CommandVector hostList) {
            for(String host : hostList) {
                if (host.equals(clusterName))
                    return true;
                if (topClusters.containsKey(host))
                    return hasRecursiveCall(clusterName, topClusters.get(host));
            }
            return false;
        }

        public void actionPerformed(ActionEvent ev) {
            if (frame == null) {
                CellConstraints cc = new CellConstraints();
                JButton applyButton = new JButton(XEMessages.msgs.getString("Apply"));
                JButton closeButton = new JButton(XEMessages.msgs.getString("Close"));
                hostList = new JList();
                JScrollPane hostPane = new JScrollPane(hostList);
                hostField = new JTextField();

                applyButton.setToolTipText(XEMessages.msgs.getString("Press_Apply"));
                closeButton.setToolTipText(XEMessages.msgs.getString("Close_Window"));

                hostList.setVisibleRowCount(7);
                hostList.addListSelectionListener(new HostListListener());
                hostField.setEditable(true);

                clusterBox = new JComboBox();
                clusterBox.setEditable(true);
                clusterBox.addActionListener(new ClusterBoxListener());

                applyButton.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent ae) {
                        String clusterName = (String) clusterBox.getSelectedItem();

                        if (clusterName == null || clusterName.length() == 0) {
                            XEDialogs.warningDialog(frame,
                                XEMessages.msgs.getString("Enter_CName"));
                            return;
                        }

                        // get the list model and it's size
                        ListModel lm = hostList.getModel();
                        int size = lm.getSize();

                        if (size == 0) {
                            XEDialogs.warningDialog(frame,
                                XEMessages.msgs.getString("Enter_CHosts"));
                            return;
                        }

                        // put the elements in a dynamic quick lookup structure
                        TreeSet<String> list = new TreeSet<String>();

                        for(int i=0; i<size; i++)
                            list.add((String) lm.getElementAt(i));

                        CommandVector hosts = new CommandVector(list);

                        // no loops in the graph
                        if (hasRecursiveCall(clusterName, hosts)) {
                            XEDialogs.warningDialog(frame,
                                XEMessages.msgs.getString("Recursion"));
                            return;
                        }

                        if (topClusters.containsKey(clusterName))
                            msgBar.setText(
                                XEMessages.msgs.getString("Updated_Cluster") + clusterName);
                        else
                            msgBar.setText(
                                XEMessages.msgs.getString("Added_Cluster") + clusterName);

                        topClusters.put(clusterName, hosts);

                        hostList.setListData(new Object[0]);
                        hostField.setText("");

                        saveConfigFile();
                    }
                });
                closeButton.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent ae) {
                        frame.setVisible(false);
                    }
                });

                frame = new JFrame(XEMessages.msgs.getString("Clusters"));

                // cluster label and combo box
                FormLayout clusterLayout = new FormLayout(
                    "6dlu, c:p:g, 6dlu", "6dlu, t:p:g, 6dlu, c:p:g, 6dlu");
                PanelBuilder clusterBuilder = new PanelBuilder(clusterLayout);
                clusterDelete = new JButton(XEMessages.msgs.getString("Delete"));

                clusterDelete.setEnabled(false);
                clusterDelete.addActionListener(new DeleteCluster());

                clusterBuilder.add(clusterBox, cc.xy(2, 2));
                clusterBuilder.add(clusterDelete, cc.xy(2, 4));

                TitledBorder clusterBorder = new TitledBorder(XEMessages.msgs.getString("Cluster"));
                clusterBorder.setTitleJustification(TitledBorder.CENTER);
                clusterBuilder.getPanel().setBorder(clusterBorder);

                // add/delete buttons
                FormLayout adLayout = new FormLayout("c:p:g, 4dlu, c:p:g", "c:p:g");
                PanelBuilder adBuilder = new PanelBuilder(adLayout);

                JButton addButton = new JButton(XEMessages.msgs.getString("Add"));
                deleteButton = new JButton(XEMessages.msgs.getString("Delete"));

                AddClusterHost ach = new AddClusterHost();
                addButton.addActionListener(ach);
                hostField.addActionListener(ach);
                deleteButton.addActionListener(new DeleteClusterHost());

                addButton.setToolTipText(XEMessages.msgs.getString("Press_Add"));
                deleteButton.setToolTipText(XEMessages.msgs.getString("Press_Delete"));
                deleteButton.setEnabled(false);

                adBuilder.add(addButton,    cc.xy(1, 1, "r, c"));
                adBuilder.add(deleteButton, cc.xy(3, 1, "l, c"));

                // all the host objects
                FormLayout hostLayout = new FormLayout(
                        "6dlu, f:min(pref;65dlu):g, 6dlu",
                        "6dlu, t:p:n, 6dlu, c:p:g, 6dlu, c:p:g, 6dlu, b:d:g, 6dlu");
                PanelBuilder hostBuilder = new PanelBuilder(hostLayout);

                // add the objects to the host panel
                hostBuilder.add(hostPane,       cc.xy(2, 2));
                hostBuilder.add(hostField,      cc.xy(2, 4));
                hostBuilder.add(adBuilder.getPanel(), cc.xy(2, 8));

                TitledBorder hostBorder = new TitledBorder(XEMessages.msgs.getString("Hosts"));
                hostBorder.setTitleJustification(TitledBorder.CENTER);
                hostBuilder.getPanel().setBorder(hostBorder);

                // the top half consisting of the cluster stuff and host stuff
                FormLayout topHalfLayout = new FormLayout("c:p:g, 6dlu, c:p:g", "t:p:g");
                PanelBuilder topHalfBuilder = new PanelBuilder(topHalfLayout);

                topHalfBuilder.add(clusterBuilder.getPanel(), cc.xy(1, 1));
                topHalfBuilder.add(hostBuilder.getPanel(),    cc.xy(3, 1));

                // the top of the mess where it all comes together
                FormLayout topLayout = new FormLayout(
                    "c:p:g", "t:p:g, 10dlu, c:p:g, 4dlu, f:p:g, f:p:g");
                PanelBuilder topBuilder = new PanelBuilder(topLayout);

                topBuilder.setDefaultDialogBorder();

                // the apply/close buttons
                FormLayout acLayout = new FormLayout("c:p:g, 4dlu, c:p:g", "c:p:g");
                PanelBuilder acBuilder = new PanelBuilder(acLayout);

                msgBar = new JLabel(" ");
                JSeparator sep = new JSeparator();
                
                acBuilder.add(applyButton, cc.xy(1, 1, "r, c"));
                acBuilder.add(closeButton, cc.xy(3, 1, "l, c"));

                topBuilder.add(topHalfBuilder.getPanel(), cc.xy(1, 1));
                topBuilder.add(acBuilder.getPanel(),      cc.xy(1, 3));
                topBuilder.add(sep,                       cc.xy(1, 5, "f, f"));
                topBuilder.add(msgBar,                    cc.xy(1, 6, "f, f"));

                frame.setContentPane(topBuilder.getPanel());
                frame.pack();
            }

            if (topClusters.size() > 0) {
                Set<String> keys = topClusters.keySet();
                Object[] array = keys.toArray();

                clusterBox.setModel(new DefaultComboBoxModel(array));
                hostList.setListData(topClusters.get((String) array[0]));
                clusterDelete.setEnabled(true);
            } else {
                clusterBox.setModel(new DefaultComboBoxModel(new Object[0]));
                hostList.setListData(new Object[0]);
                hostField.setText("");
            }

            Rectangle pr = new Rectangle();
            Rectangle cr = new Rectangle();
            pr = topFrame.getBounds(pr);
            cr = frame.getBounds(cr);
            pr.x += ((pr.width - cr.width) / 2);
            pr.y += ((pr.height - cr.height) / 2);
            pr.width = cr.width;
            pr.height = cr.height;
            frame.setBounds(pr);

            frame.setVisible(true);
        }

        private class ClusterBoxListener implements ActionListener {
            public void actionPerformed(ActionEvent ev) {
                // additions, deletions to the box by another
                if (CBSurgury)
                    return;

                String newCluster = (String) clusterBox.getSelectedItem();
                String action = ev.getActionCommand();

                if (action.equals("comboBoxEdited")) {
                    if (newCluster == null || newCluster.length() == 0)
                        return;
                    boolean already = false;
                    int n = clusterBox.getItemCount();

                    for(int i=0; i<n; i++) {
                        if (((String) clusterBox.getItemAt(i)).equals(newCluster)) {
                            already = true;
                            break;
                        }
                    }
                    if (!already)
                        clusterBox.addItem(newCluster);
                    hostList.setListData(new Object[0]);
                    clusterDelete.setEnabled(true);
                } else if (action.equals("comboBoxChanged")) {
                    String name = (String) clusterBox.getSelectedItem();

                    if (name == null || !topClusters.containsKey(name))
                        return;
                    hostList.setListData(topClusters.get(name).toArray());
                    clusterDelete.setEnabled(true);
                }
            }
        }

        private class HostListListener implements ListSelectionListener {
            public void valueChanged(ListSelectionEvent le) {
                // if it's the first of many events
                if (le.getValueIsAdjusting())
                    return;

                // the point is to select a host
                if (hostList.getSelectedValues().length == 0)
                    return;

                deleteButton.setEnabled(true);
            }
        }

        private class AddClusterHost implements ActionListener {
            public void actionPerformed(ActionEvent ev) {
                // grab string
                String newHost = hostField.getText();

                // none present
                if (newHost.length() == 0)
                    return;

                // get the list model and it's size
                ListModel lm = hostList.getModel();
                int size = lm.getSize();

                // put the elements in a dynamic quick lookup structure
                TreeSet<String> list = new TreeSet<String>();

                for(int i=0; i<size; i++)
                    list.add((String) lm.getElementAt(i));

                // gimme comma separated list
                boolean hostsAdded = false;

                // for each host
                for(String h : newHost.split(",")) {
                    // no duplicates, please
                    if (list.contains(h))
                        continue;

                    list.add(h);
                    hostsAdded = true;
                }

                if (hostsAdded)
                    hostList.setListData(list.toArray());
            }
        }

        private class DeleteClusterHost implements ActionListener {
            public void actionPerformed(ActionEvent ev) {
                Object[] selections = hostList.getSelectedValues();
                int n = selections.length;

                // that was easy.
                if (n == 0)
                    return;

                // get the list model and it's size
                ListModel lm = hostList.getModel();
                int size = lm.getSize();

                // put the elements in a dynamic quick lookup structure
                TreeSet<String> list = new TreeSet<String>();
                int i;

                for(i=0; i<size; i++)
                    list.add((String) lm.getElementAt(i));

                for(i=0; i<n; i++)
                    list.remove((String) selections[i]);

                hostList.setListData(list.toArray());

                deleteButton.setEnabled(false);
            }
        }

        private class DeleteCluster implements ActionListener {
            public void actionPerformed(ActionEvent ev) {
                String name = (String) clusterBox.getSelectedItem();

                if (name == null)
                    return;
                CBSurgury = true;
                clusterBox.removeItem(name);
                CBSurgury = false;
                hostList.setListData(new Object[0]);
                if (topClusters.containsKey(name))
                    topClusters.remove(name);
                clusterDelete.setEnabled(false);
                saveConfigFile();
                msgBar.setText(
                    XEMessages.msgs.getString("Deleted_Cluster") + name);
            }
        }
    }

    //
    // Constructors.
    //

    /**
     * Top constructor.
     * @param remoteHost The host to monitor
     * @param twp Properties for creating windows from a profile
     */
    public Top(String remoteHost, TopWindowProperties twp) {
        // static main, non-static loadProfile...
        if (remoteHost != null && remoteHost.charAt(0) == ':') {
            loadProfile(remoteHost.substring(1, remoteHost.length()));
            return;
        }

        try {
            // for JFrame
            String title;

            // title
            if (twp == null)
                title = XEMessages.msgs.getString("Top");
            else
                title = twp.windowTitle;

            // the application frame
            topFrame = new JFrame(title);

            // icon
            String ICON_PATH = "com/capmet/tools/XE/images/top.png";
            URL imageURL = getClass().getClassLoader().getResource(ICON_PATH);
            ImageIcon top = null;
            if (imageURL != null)
                top = new ImageIcon(imageURL);
            if (top != null)
                topFrame.setIconImage(top.getImage());

            // menu bar
            JMenuBar menuBar = new JMenuBar();

            // Top menu
            JMenu topMenu = new JMenu(XEMessages.msgs.getString("Top"));
            JMenuItem newTitleItem = new JMenuItem(XEMessages.msgs.getString("New_Title"), 'H');
            JMenuItem newTopItem = new JMenuItem(XEMessages.msgs.getString("New_Top"), 'T');
            JMenuItem monitorItem = new JMenuItem(XEMessages.msgs.getString("Monitor"), 'M');
            JMenuItem exitItem = new JMenuItem(XEMessages.msgs.getString("Exit"), 'X');

            // actions for them
            newTitleItem.addActionListener(new AddTitle());
            newTopItem.addActionListener(new AddHost(true));
            monitorItem.addActionListener(new AddHost(false));
            exitItem.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    System.exit(0);
                }
            });

            // insert
            topMenu.add(newTitleItem);
            topMenu.add(newTopItem);
            topMenu.add(monitorItem);
            topMenu.addSeparator();
            topMenu.add(exitItem);

            // Tools menu
            JMenu toolsMenu = new JMenu(XEMessages.msgs.getString("Tools"));
            JMenuItem addProfile = new JMenuItem(XEMessages.msgs.getString("Add_Profile"), 'A');
            JMenu loadProfile = new JMenu(XEMessages.msgs.getString("Load_Profile"));
            JMenuItem clusters = new JMenuItem(XEMessages.msgs.getString("Define_Clusters"), 'D');

            // action
            addProfile.addActionListener(new AddProfile());
            clusters.addActionListener(new ClusterEdit());

            // add load entries if any
            if (topProps.windowProfiles.size() > 0) {
                ActionListener lp = new ActionListener() {
                    public void actionPerformed(ActionEvent ae) {
                        loadProfile(ae.getActionCommand());
                    }
                };
                for(String pName : topProps.windowProfiles.keySet()) {
                    JMenuItem pi = new JMenuItem(pName);

                    pi.addActionListener(lp);
                    loadProfile.add(pi);
                }
            } else
                loadProfile.setEnabled(false);

            // populate
            toolsMenu.add(addProfile);
            toolsMenu.add(loadProfile);
            toolsMenu.addSeparator();
            toolsMenu.add(clusters);

            // the help menu
            JMenu helpMenu = new JMenu(XEMessages.msgs.getString("Help"));
            JMenuItem helpItem = new JMenuItem(XEMessages.msgs.getString("Top_Help"), 'H');
            JMenuItem aboutItem = new JMenuItem(XEMessages.msgs.getString("About"), 'A');
            helpItem.addActionListener(new HelpCallback());
            aboutItem.addActionListener(new AboutCallback());

            // populate
            helpMenu.add(helpItem);
            helpMenu.addSeparator();
            helpMenu.add(aboutItem);

            // add the menus
            menuBar.add(topMenu);
            menuBar.add(toolsMenu);
            menuBar.add(helpMenu);

            // configure the menu bar into the frame
            topFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            topFrame.setJMenuBar(menuBar);

            // build the tabbed pane of hosts
            topTabbedPane = new JTabbedPane();

            // if they're both null, it's the simplest case; local host, no profile
            if (remoteHost == null && twp == null) {
                remoteHost = InetAddress.getLocalHost().getHostName();
                int dot = remoteHost.indexOf(".");
                if (dot != -1)
                    remoteHost = remoteHost.substring(0, dot);
                HostWindow w = new HostWindow(remoteHost, null);
                Color state = w.getRulePanel().getPeakRule().getState();
                topTabbedPane.addTab(remoteHost, new StateIcon(state), w);
            } else {
                // either remoteHost is not null or twp is not null
                if (remoteHost != null) {
                    // it's remoteHost; monitor a remoteHost without a profile
                    for(String hostName : expandClusterNames(remoteHost).split(",")) {
                        HostWindow w;

                        try {
                            w = new HostWindow(hostName, null);
                        } catch(Exception e) {
                            XEDialogs.warningDialog(hostName + ':' + ' ' +
                                new CmExceptionParser(e).getMessage());
                            continue;
                        }

                        Color state = w.getRulePanel().getPeakRule().getState();
                        topTabbedPane.addTab(hostName, new StateIcon(state), w);
                    }
                } else {
                    // twp is not null; load a profile and go

                    // for each host, make a tab with its properties
                    for(Map.Entry<String, TopTabProperties> e : twp.tabMap.entrySet()) {
                        String hostName = e.getKey();
                        TopTabProperties ttp = e.getValue();
                        HostWindow w;

                        try {
                            w = new HostWindow(hostName, ttp);
                        } catch(Exception ex) {
                            XEDialogs.warningDialog(hostName + ':' + ' ' +
                                new CmExceptionParser(ex).getMessage());
                            continue;
                        }

                        Color state = w.getRulePanel().getPeakRule().getState();
                        topTabbedPane.addTab(hostName, new StateIcon(state), w);
                    }
                }
            }

            if (topTabbedPane.getTabCount() == 0) {
                // all of the hosts exceptioned out
                topFrame.setVisible(false);
                topFrame.dispose();
                topFrame = null;
                return;
            }

            // pack, pack
            topFrame.setContentPane(topTabbedPane);
            topFrame.pack();

            // keep track of the upper level windows
            topWindows.addElement(topFrame);

            if (twp == null)
                topFrame.setSize(866, 657);
            else
                topFrame.setBounds(twp.windowBounds);

            // lay a patch, man
            topFrame.setVisible(true);
        } catch(Exception e) {
            XEDialogs.warningDialog(new CmExceptionParser(e).getMessage());
        }
    }
}
