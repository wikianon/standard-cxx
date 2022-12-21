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

import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import java.net.UnknownHostException;
import java.util.TreeMap;
import java.util.Set;
import java.util.Map;
import java.util.Arrays;
import java.util.Iterator;
import java.awt.*;
import java.awt.event.*;
import java.net.InetAddress;
import javax.swing.*;
import javax.swing.event.*;
import com.jgoodies.forms.layout.*;
import com.jgoodies.forms.builder.PanelBuilder;
import com.jgoodies.forms.factories.FormFactory;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.cm.CmGlobal;
import com.capmet.metrics.cm.CmNames;
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.PerformanceCategory;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.tools.XE.XEDialogs;

/**
 * A dialog that allows the user to select one or more
 * counters from a category of counters. The category
 * is first selected and then the counters of that category
 * are selected. Explanations for the counters can also be
 * viewed.
 */
public abstract class XEPerformanceFieldSelector extends JFrame implements ActionListener
{
    private enum CounterMode
    {
        CM_ALL, CM_SELECT
    }

    private CounterMode         counterMode = CounterMode.CM_SELECT;
    private CounterMode         instanceMode = CounterMode.CM_SELECT;
    private String              monitoredHost;
    private String              selectedCategory;
    private PerformanceHost     performanceHost;
    private PerformanceCategory category;
    private boolean             CBSurgery = false;
    private JComboBox           hostBox;
    private JComboBox           categoryBox;
    private JList               counterList;
    private JScrollPane         counterPane;
    private JRadioButton        localCounters;
    private JRadioButton        remoteCounters;
    private JRadioButton        allInstances;
    private JRadioButton        selectInstances;
    private JButton             explainButton;
    private JList               instanceList;
    private JScrollPane         instancePane;
    private XECounterData[]     selectedCounters;
    private JFrame              explainFrame;
    private JTextArea           explainText;
    private CmNames             names;

    private TreeMap<String, PerformanceHost> hostsMap;
    private TreeMap<String, CmNames>         namesMap;

    // set monitoredHost to the local host name.
    private void setLocalHost()
    {
        monitoredHost = getLocalHost();
    }

    // get the name of the local host.
    private String getLocalHost()
    {
        String local;

        try {
            local = InetAddress.getLocalHost().getHostName();
        } catch(UnknownHostException e) {
            local = "localhost";
        }
        return local;
    }

    // set the default category.
    private void setDefaultCategory()
    {
        if (performanceHost.getOSFlavor().isUnixHost())
            selectedCategory = names.UnixCpuStat;
        else if (performanceHost.getOSFlavor().isWindowsHost())
            selectedCategory = names.WindowsProcessor;
    }

    // get the category names, sort them and put them in the category box.
    private void updateCategories()
        throws PerformanceException
    {
        int i;
        int n;
        String[] cats;

        try {
            cats = performanceHost.getCategoryNames();
            Arrays.sort(cats);
        } catch(Exception e) {
            XEDialogs.warningDialog(new CmExceptionParser(e).getMessage());
            throw new PerformanceException("XEPerformanceFieldSelector failed.");
        }

        CBSurgery = true;
        categoryBox.removeAllItems();
        for(i=0, n=cats.length; i<n; i++)
            categoryBox.addItem(cats[i]);
        categoryBox.setSelectedItem(selectedCategory);
        CBSurgery = false;
    }

    // get the counters for the selected category, sort them and
    // place them into the counter list.

    private void updateCounters()
        throws PerformanceException
    {
        String[] fields = null;

        try {
            category = new PerformanceCategory(selectedCategory, monitoredHost);
            fields = category.getSupportedFieldNames();
            Arrays.sort(fields);
        } catch(Exception e) {
            CmExceptionParser ep = new CmExceptionParser(e);
            XEDialogs.warningDialog(ep.getMessage());
            throw new PerformanceException("XEPerformanceFieldSelector failed.");
        }

        counterList.setListData(fields);
        counterList.setVisibleRowCount(7);
    }

    // get the instances for the selected category, sort them and
    // place them into the instance list.

    private void updateInstances()
        throws PerformanceException
    {
        if (category.isSingleton()) {
            instanceList.setListData(new String[0]);
            instanceList.setEnabled(false);
            allInstances.setEnabled(false);
            selectInstances.setEnabled(false);
        } else {
            instanceList.setEnabled(true);
            allInstances.setEnabled(true);
            selectInstances.setEnabled(true);
            try {
                String[] instances = category.getInstanceNames();
                Arrays.sort(instances);
                instanceList.setListData(instances);
            } catch(Exception e) {
                CmExceptionParser ep = new CmExceptionParser(e);
                XEDialogs.warningDialog(ep.getMessage());
                throw new PerformanceException("XEPerformanceFieldSelector failed.");
            }
        }
    }

    // add a new host to the list of hosts in the host box.
    private void addNewHost(String newHost)
    {
        boolean already = false;
        int n = hostBox.getItemCount();

        for(int i=0; i<n; i++) {
            if (((String) hostBox.getItemAt(i)).equals(newHost)) {
                already = true;
                break;
            }
        }
        if (!already)
            hostBox.addItem(newHost);
    }

    // encapsulated logic for handling the host box.
    private class HostBoxListener implements ActionListener
    {
        /**
         * Handle host box events.
         * @param ev The event.
         */
        public void actionPerformed(ActionEvent ev)
        {
            String newHost = (String) hostBox.getSelectedItem();
            String command = ev.getActionCommand();

            if (monitoredHost.equals(newHost))
                return;

            try {
                switchHosts(newHost);
                if (command.equals("comboBoxEdited"))
                    addNewHost(newHost);
            } catch(PerformanceException e) {
                localCounters.setSelected(true);
                remoteCounters.setSelected(false);
                hostBox.setEnabled(false);
                hostBox.setSelectedItem(getLocalHost());
            }
        }
    }

    // switch from the current host to another host.
    private void switchHosts(String newHost)
        throws PerformanceException
    {
        if (monitoredHost.equals(newHost))
            return;
        try {
            if (hostsMap.containsKey(newHost))
              performanceHost = hostsMap.get(newHost);
            else {
              performanceHost = new PerformanceHost(newHost);
              hostsMap.put(newHost, performanceHost);
            }
            if (namesMap.containsKey(newHost))
              names = namesMap.get(newHost);
            else {
              CmGlobal<CmNames> gnames = CmGlobal.getNames(newHost);

              names = gnames.getObject();
              namesMap.put(newHost, names);
            }
            monitoredHost = newHost;
            setDefaultCategory();
            updateCategories();
            updateCounters();
            updateInstances();
        } catch(Exception e) {
            XEDialogs.warningDialog(new CmExceptionParser(e).getMessage());
            throw new PerformanceException("XEPerformanceFieldSelector failed.");
        }
    }

    // encapsulated logic for dealing with category box events.
    private class CategoryBoxListener implements ActionListener
    {
        /**
         * The category box event handler.
         * @param ev The event.
         */
        public void actionPerformed(ActionEvent ev)
        {
            // don't try to react to manual modification
            if (CBSurgery)
                return;
            selectedCategory = (String) categoryBox.getSelectedItem();
            try {
                updateCounters();
                updateInstances();
                if (explainFrame != null)
                    explainCategory();
            } catch(Exception e) {
                CmExceptionParser ep = new CmExceptionParser(e);
                XEDialogs.fatalDialog(ep.getMessage());
            }
        }
    }

    // encapsulated logic for dealing with the "local computer" button.
    private class LocalButtonListener implements ActionListener
    {
        /**
         * The button event handler.
         * @param ev The event.
         */
        public void actionPerformed(ActionEvent ev)
        {
            hostBox.setEnabled(false);
            try {
                switchHosts(getLocalHost());
            } catch(Exception e) {
                CmExceptionParser ep = new CmExceptionParser(e);
                XEDialogs.fatalDialog(ep.getMessage());
            }
        }
    }

    // encapsulated logic for dealing with the "remote computer" button.
    private class RemoteButtonListener implements ActionListener
    {
        /**
         * The button event handler.
         * @param ev The event.
         */
        public void actionPerformed(ActionEvent ev)
        {
            hostBox.setEnabled(true);
            String newHost = (String) hostBox.getSelectedItem();
            if (monitoredHost.equals(newHost))
                return;
            try {
                switchHosts(newHost);
                setDefaultCategory();
                updateCategories();
                updateCounters();
                updateInstances();
            } catch(Exception e) {
                CmExceptionParser ep = new CmExceptionParser(e);
                XEDialogs.fatalDialog(ep.getMessage());
            }
        }
    }

    // encapsulated logic for dealing with the "all counters" button.
    private class AllCountersListener implements ActionListener
    {
        /**
         * The button event handler.
         * @param ev The event.
         */
        public void actionPerformed(ActionEvent ev)
        {
            counterMode = CounterMode.CM_ALL;
            counterList.setEnabled(false);
        }
    }

    // encapsulated logic for dealing with the "select counters" button.
    private class SelectCountersListener implements ActionListener
    {
        /**
         * The button event handler.
         * @param ev The event.
         */
        public void actionPerformed(ActionEvent ev)
        {
            counterMode = CounterMode.CM_SELECT;
            counterList.setEnabled(true);
            explainButton.setEnabled(true);
        }
    }

    // encapsulated logic for dealing with the "all instances" button.
    private class AllInstancesListener implements ActionListener
    {
        /**
         * The button event handler.
         * @param ev The event.
         */
        public void actionPerformed(ActionEvent ev)
        {
            instanceMode = CounterMode.CM_ALL;
            instanceList.setEnabled(false);
        }
    }

    // encapsulated logic for dealing with the "select instances" button.
    private class SelectInstancesListener implements ActionListener
    {
        /**
         * The button event handler.
         * @param ev The event.
         */
        public void actionPerformed(ActionEvent ev)
        {
            instanceMode = CounterMode.CM_SELECT;
            instanceList.setEnabled(true);
        }
    }

    // encapsulated logic for dealing with the add button.
    private class AddButtonListener implements ActionListener
    {
        /**
         * The button event handler.
         * @param ev The event.
         */
        public void actionPerformed(ActionEvent ev)
        {
            String[] counters = null;
            String[] instances = null;

            // first: see how we did with the counters
            if (counterMode == CounterMode.CM_ALL) {
                // selected all counters
                ListModel lm = counterList.getModel(); int n = lm.getSize();

                counters = new String[n];
                for(int i=0; i<n; i++)
                    counters[i] = (String) lm.getElementAt(i);
            } else {
                // selected specific counters
                Object[] selections = counterList.getSelectedValues();
                int n = selections.length;

                if (n > 0) {
                    counters = new String[n];
                    for(int i=0; i<n; i++)
                        counters[i] = (String) selections[i];
                }
            }

            if (!category.isSingleton()) {
                // if it's not a singleton, then we have to inventory the instances
                if (instanceMode == CounterMode.CM_ALL) {
                    // selected all the instances
                    ListModel lm = instanceList.getModel();
                    int n = lm.getSize();

                    instances = new String[n];
                    for(int i=0; i<n; i++)
                        instances[i] = (String) lm.getElementAt(i);
                } else {
                    // selected specific instances
                    Object[] selections = instanceList.getSelectedValues();
                    int n = selections.length;

                    if (n > 0) {
                        instances = new String[n];
                        for(int i=0; i<n; i++)
                            instances[i] = (String) selections[i];
                    }
                }
            }

            // ok, we're done with that
            if (instances == null) {
                if (counters != null) {
                    // singleton
                    selectedCounters = new XECounterData[counters.length];
                    for(int i=0; i<counters.length; i++) {
                        XECounterData c = new XECounterData();

                        c.host = monitoredHost;
                        c.category = selectedCategory;
                        c.counter = counters[i];

                        selectedCounters[i] = c;
                    }
                }
            } else {
                if (counters != null) {
                    int i;
                    int j;
                    int n = 0;

                    // n-tuple
                    selectedCounters = new XECounterData[instances.length * counters.length];
                    for(i=0; i<instances.length; i++) {
                        for(j=0; j<counters.length; j++) {
                            XECounterData c = new XECounterData();

                            c.host = monitoredHost;
                            c.category = selectedCategory;
                            c.counter = counters[j];
                            c.instance = instances[i];

                            selectedCounters[n++] = c;
                        }
                    }
                }
            }

            // call the add event handler from the customer
            XEPerformanceFieldSelector.this.actionPerformed(ev);

            // all done; hope you got your stuff
            selectedCounters = null;
        }
    }

    // encapsulated logic for dealing with the explain button.
    private class ExplainButtonListener implements ActionListener
    {
        private boolean categoryExplanation;

        /**
         * The constructor.
         */
        public ExplainButtonListener()
        {
            explainFrame = null;
            explainText = new JTextArea(10, 40);
            categoryExplanation = false;
        }

        public ExplainButtonListener(boolean isCategory)
        {
            explainFrame = null;
            explainText = new JTextArea(10, 40);
            categoryExplanation = isCategory;
        }

        /**
         * The button event handler.
         * @param ev The event.
         */
        public void actionPerformed(ActionEvent ev)
        {
            if (explainFrame == null) {
                explainFrame = new JFrame(XEMessages.msgs.getString("Explain_Text"));
                JButton button = new JButton(XEMessages.msgs.getString("OK"));
                JScrollPane sp = new JScrollPane(explainText);
                FormLayout layout = new FormLayout(
                        "c:p:g",
                        "f:p:g, 8dlu, f:p:n");

                PanelBuilder builder = new PanelBuilder(layout);
                CellConstraints cc = new CellConstraints();

                builder.setDefaultDialogBorder();

                button.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent ae) {
                        explainFrame.setVisible(false);
                    }
                });
                button.setToolTipText(XEMessages.msgs.getString("Press_Dismiss"));
                explainFrame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);

                explainText.setEditable(false);
                explainText.setLineWrap(true);
                explainText.setWrapStyleWord(true);
                sp.setHorizontalScrollBarPolicy(
                    ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);

                builder.add(sp,     cc.xy(1, 1));
                builder.add(button, cc.xy(1, 3));

                explainFrame.setContentPane(builder.getPanel());
                explainFrame.pack();
            }

            Rectangle r = new Rectangle();
            r = XEPerformanceFieldSelector.this.getBounds(r);
            r.y += r.height;
            r.height = explainFrame.getHeight();
            r.width = explainFrame.getWidth();
            explainFrame.setBounds(r);

            if (categoryExplanation)
                explainCategory();
            else
                explainCounters();
        }
    }

    // collect the counter names that need explanation and explain them.
    private void explainCounters()
    {
        String[] counters;
        int n;

        if (counterMode == CounterMode.CM_ALL) {
            // selected all counters
            ListModel lm = counterList.getModel();
            n = lm.getSize();

            counters = new String[n];
            for(int i=0; i<n; i++)
                counters[i] = (String) lm.getElementAt(i);
        } else {
            // selected specific counters
            Object[] selections = counterList.getSelectedValues();
            n = selections.length;

            // that was easy.
            if (n == 0)
                return;

            counters = new String[n];
            for(int i=0; i<n; i++)
                counters[i] = (String) selections[i];
        }

        String explanation = null;

        for(int i=0; i<n; i++) {
            String ex = null;

            try {
                ex = category.getFieldExplanation(counters[i]);
            } catch(Exception e) {
                XEDialogs.fatalDialog(new CmExceptionParser(e).getMessage());
            }

            if (i == 0) {
                explanation = counters[i];
                explanation += "\n\n";
                explanation += ex;
            } else {
                explanation += "\n\n";
                explanation += counters[i];
                explanation += "\n\n";
                explanation += ex;
            }
        }

        explainText.setText(explanation);
        explainText.setCaretPosition(0);

        if (!explainFrame.isVisible())
            explainFrame.setVisible(true);
    }

    private void explainCategory()
    {
        String explanation = selectedCategory + "\n\n" +
            category.getCategoryExplanation();

        explainText.setText(explanation);
        explainText.setCaretPosition(0);

        if (!explainFrame.isVisible())
            explainFrame.setVisible(true);
    }

    // encapsulated logic for dealing with selections from the counter list.
    private class CounterListListener implements ListSelectionListener
    {
        /**
         * The value changed event handler.
         * @param le The event.
         */
        public void valueChanged(ListSelectionEvent le) {
            // if it's the first of many events
            if (le.getValueIsAdjusting())
                return;

            // if the explain frame is not up yet
            if (explainFrame == null)
                return;

            // it's been up but is not right now
            if (!explainFrame.isVisible())
                return;

            // go explain it.
            explainCounters();
        }
    }

    /**
     * Retrieve an array of selected counters.
     * @return The <CODE>XECounterData</CODE> array of selected counters.
     */
    public XECounterData[] getSelectedCounters()
    {
        return selectedCounters;
    }

    /**
     * The default constructor for selecting counters from the local host.
     * @throws PerformanceException Any possible lower level exception.
     */
    public XEPerformanceFieldSelector()
        throws PerformanceException
    {
        this(null);
    }

    /**
     * The constructor for selecting counters from the specified host.
     * @param host The host to select counters from.
     * @throws PerformanceException Any possible lower level exception.
     */
    public XEPerformanceFieldSelector(String host)
        throws PerformanceException
    {
        // make the frame
        super(XEMessages.msgs.getString("Add_Counters"));

        // setLocalHost will set monitoredHost to the local host name
        setLocalHost();

        // if a remote host was specified
        if (host != null) {
            // if localhost was specified, make it look that way
            if (monitoredHost.equals(host))
                host = null;
            else
                monitoredHost = host;
        }

        try
        {
            CmGlobal<CmNames> gnames = CmGlobal.getNames(monitoredHost);

            names = gnames.getObject();
            namesMap = new TreeMap<String, CmNames>();
            namesMap.put(monitoredHost, names);
        }
        catch(Exception e)
        {
            throw new PerformanceException(e.getMessage());
        }

        //
        // Step 1: build the GUI framework
        //

        // make the layout manager
        FormLayout layout = new FormLayout(
            // cols
            "f:p:g, 7pt, f:d:g",
            // rows
            "p, 0pt, p, 0pt, p, 7dlu, p, 5pt, p, 5pt, p, 0pt, p, 2pt, f:d:g, 7pt, p, 7pt, p");

        // make the panel builder
        PanelBuilder builder = new PanelBuilder(layout);
        CellConstraints cc = new CellConstraints();

        builder.setDefaultDialogBorder();

        //
        // Step 2: build the local/remote computer buttons with combo box
        //

        try {
            hostsMap = new TreeMap<String, PerformanceHost>();
            performanceHost = new PerformanceHost(monitoredHost);
            hostsMap.put(monitoredHost, performanceHost);
        } catch(Exception e) {
            CmExceptionParser ep = new CmExceptionParser(e);
            XEDialogs.warningDialog(ep.getMessage());
            throw new PerformanceException("XEPerformanceFieldSelector failed.");
        }

        // build the local or remote counters radio buttons
        localCounters =
            new JRadioButton(XEMessages.msgs.getString("Use_Local"), (host == null));
        remoteCounters =
            new JRadioButton(XEMessages.msgs.getString("Select_Counters"), (host != null));

        localCounters.addActionListener(new LocalButtonListener());
        remoteCounters.addActionListener(new RemoteButtonListener());

        // build the remote host combo box
        String[] hosts = new String[1];
        hosts[0] = monitoredHost;

        hostBox = new JComboBox(hosts);
        hostBox.setEditable(true);
        hostBox.addActionListener(new HostBoxListener());

        // default is local host; disable remote hosts list
        hostBox.setEnabled(false);

        // build a button group for the local/remote radio buttons
        ButtonGroup counterSource = new ButtonGroup();

        // add the radio buttons
        counterSource.add(localCounters);
        counterSource.add(remoteCounters);

        // use a form to build the indent for the host box
        FormLayout cbLayout = new FormLayout("10dlu, f:p:g", "f:p:g");
        PanelBuilder cbBuilder = new PanelBuilder(cbLayout);
        cbBuilder.add(hostBox, cc.xy(2, 1));

        // place the local/remote host objects into the builder
        builder.add(localCounters,        cc.xy(1, 1));
        builder.add(remoteCounters,       cc.xy(1, 3));
        builder.add(cbBuilder.getPanel(), cc.xy(1, 5));

        // remote host was requested right out of the box
        if (host != null) {
            hostBox.setEnabled(true);
            addNewHost(host);
        }

        //
        // Step 3: build the category label and combo box
        //

        // build the label
        builder.add(new JLabel(XEMessages.msgs.getString("Perf_Obj")), cc.xy(1, 7));

        // add the category combo box
        categoryBox = new JComboBox();
        categoryBox.addActionListener(new CategoryBoxListener());

        // update the category box with the initial category
        setDefaultCategory();
        updateCategories();

        JButton categoryExplain = new JButton(
            XEMessages.msgs.getString("Explain"));
        categoryExplain.addActionListener(new ExplainButtonListener(true));

        // add to builder
        builder.add(categoryBox,     cc.xy(1, 9));
        builder.add(categoryExplain, cc.xy(3, 9, "left,center"));

        //
        // Step 4: build the all/select counters buttons and counters list box
        //

        // make the which-counters radio buttons
        JRadioButton allCounters =
            new JRadioButton(XEMessages.msgs.getString("All_Counters"), false);
        JRadioButton selectCounters =
            new JRadioButton(XEMessages.msgs.getString("Select_List"), true);

        // update the counter list for the given category
        counterList = new JList();
        counterPane = new JScrollPane(counterList);
        counterList.addListSelectionListener(new CounterListListener());
        updateCounters();

        allCounters.addActionListener(new AllCountersListener());
        selectCounters.addActionListener(new SelectCountersListener());

        // make the button group for the radio controls
        ButtonGroup whichCounters = new ButtonGroup();

        // add the radio buttons
        whichCounters.add(allCounters);
        whichCounters.add(selectCounters);

        // add to builder
        builder.add(allCounters,    cc.xy(1, 11));
        builder.add(selectCounters, cc.xy(1, 13));
        builder.add(counterPane,    cc.xy(1, 15));

        //
        // Step 5: build the all/select instances buttons and instances list box
        //

        // make the radio buttons
        allInstances =
            new JRadioButton(XEMessages.msgs.getString("All_Instances"), false);
        selectInstances =
            new JRadioButton(XEMessages.msgs.getString("Sel_Inst_List"), true);

        // update the instance list for the given category
        instanceList = new JList();
        instanceList.setVisibleRowCount(7);
        instancePane = new JScrollPane(instanceList);
        updateInstances();

        // add the listeners
        allInstances.addActionListener(new AllInstancesListener());
        selectInstances.addActionListener(new SelectInstancesListener());

        // make the button group
        ButtonGroup whichInstances = new ButtonGroup();

        // add the radio buttons
        whichInstances.add(allInstances);
        whichInstances.add(selectInstances);

        // add to builder
        builder.add(allInstances,    cc.xy(3, 11));
        builder.add(selectInstances, cc.xy(3, 13));
        builder.add(instancePane,    cc.xy(3, 15));

        //
        // Step 6: build the Add, Explain and Close buttons
        //

        // use a form to build the fixed size buttons
        FormLayout aeLayout = new FormLayout("c:p:n, 10pt, c:p:n", "c:p:n");
        PanelBuilder aeBuilder = new PanelBuilder(aeLayout);

        // Add button
        JButton addButton = new JButton(XEMessages.msgs.getString("Add"));
        explainButton = new JButton(XEMessages.msgs.getString("Explain"));

        addButton.addActionListener(new AddButtonListener());
        explainButton.addActionListener(new ExplainButtonListener());

        addButton.setToolTipText(XEMessages.msgs.getString("Press_Counters"));
        explainButton.setToolTipText(XEMessages.msgs.getString("Press_Explain"));

        aeBuilder.add(addButton,     cc.xy(1, 1));
        aeBuilder.add(explainButton, cc.xy(3, 1));

        builder.add(aeBuilder.getPanel(), cc.xy(1, 17));

        JButton closeButton = new JButton(XEMessages.msgs.getString("Close"));

        closeButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ev) {
                // setVisible(false);
                System.exit(0);
            }
        });

        closeButton.setToolTipText(XEMessages.msgs.getString("Close_Dialog"));

        builder.add(closeButton, cc.xy(3, 19, "right, bottom"));

        setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
        setContentPane(builder.getPanel());
        pack();
        // setResizable(false);
    }
}
