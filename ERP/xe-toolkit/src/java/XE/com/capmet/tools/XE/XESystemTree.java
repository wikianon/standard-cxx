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

import java.util.Map;
import java.util.Hashtable;
import java.net.URL;
import java.net.InetAddress;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.event.*;
import javax.swing.border.BevelBorder;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import java.net.UnknownHostException;
import com.capmet.metrics.cm.CmNames;
import com.capmet.metrics.cm.CmGlobal;
import com.capmet.metrics.cm.CmSysInfo;
import com.capmet.metrics.cm.CmNicStat;
import com.capmet.metrics.cm.CmNicMetrics;
import com.capmet.metrics.cm.CmIoStat;
import com.capmet.metrics.cm.CmIoMetrics;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.cm.CmMounts;
import com.capmet.metrics.cm.CmPageFiles;
import com.capmet.metrics.cm.CmPFMetrics;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.PerformanceCategory;
import com.capmet.metrics.rmi.PerformanceField;
import com.capmet.metrics.*;
import com.capmet.tools.XE.XEDialogs;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.CellConstraints;
import com.jgoodies.forms.builder.PanelBuilder;

/**
 * A <CODE>JTree</CODE> representation of a system, its physical
 * resources and logical entities. <CODE>XESystemTree</CODE> makes
 * no assumptions about whether it is an application or applet. It
 * constructs the graphical entity and makes it available from the
 * <CODE>getPane()</CODE> method.  This object extends <CODE>Timer</CODE>,
 * so after instantiation, a call to the <CODE>start</CODE> method
 * is necessary for continuous updating.
 *
 * <PRE>
 *   XESystemTree st = new XESystemTree(parent, "tweedledum", 30000);
 *
 *   st.start();
 * </PRE>
 */
public class XESystemTree extends Timer
{
    //
    // Private members
    //

    private ClickableTree       StTree;
    private Container           StTreePane;
    private CmMounts            StDiskMap;
    private NodeHash            StUpdateItems;
    private int                 StInterval;
    private XESysTreeProperties StProps;

    //
    // Public methods
    //

    public void erase() {
        StTree.erase();
        StTree = null;
        StTreePane = null;
        StDiskMap = null;
        StUpdateItems = null;
        StProps = null;
    }

    /**
     * Retrieve the <CODE>Container</CODE> holding all of the entities.
     * @return The <CODE>Container</CODE> used by the layout manager where
     * all of the graphical entities are located.
     */
    public Container getPane()
    {
        return StTreePane;
    }

    /**
     * Override the <CODE>setDelay</CODE> method of <CODE>Timer</CODE>
     * so this object can track the interval changes. Sets the
     * <CODE>Timer</CODE> delay, the number of milliseconds
     * between successive action events.
     * @param delay The delay in milliseconds.
     */
    public void setDelay(int delay)
    {
        StInterval = delay;
        super.setDelay(delay);
    }

    /**
     * Returns the delay, in milliseconds, 
     * between firings of action events.
     * @return The number of milliseconds of delay.
     */
    public int getDelay()
    {
        StInterval = super.getDelay();
        return StInterval;
    }

    /**
     * Retrieve the properties structure for this object.
     * @return The <CODE>XESysTreeProperties</CODE> containing the
     * properties of the system tree.
     */
    public XESysTreeProperties getProperties()
    {
        int z = StTree.getRowCount();

        StProps.intervalSeconds = StInterval / 1000;
        StProps.rowExpanded = new boolean[z];

        for(int q=0; q<z; q++)
            StProps.rowExpanded[q] = StTree.isExpanded(q);

        return StProps;
    }

    /**
     * Set the system tree properties.
     * @param props An <CODE>XESysTreeProperties</CODE> structure containing
     * the properties to be set.
     */
    public void setProperties(XESysTreeProperties props)
    {
        StInterval = props.intervalSeconds * 1000;
        if (props.rowExpanded != null)
            for(int q=0; q<props.rowExpanded.length; q++)
                if (props.rowExpanded[q])
                    StTree.expandRow(q);
    }

    //
    // Private methods
    //

    // traverse the updatable items hash and cause the node to be updated
    private void refresh()
        throws PerformanceException
    {
        for(Map.Entry<DefaultMutableTreeNode, UpdatableItem> e : StUpdateItems.entrySet())
        {
            DefaultMutableTreeNode node = e.getKey();
            UpdatableItem item = e.getValue();

            node.setUserObject(item.getValue());
        }
    }

    //
    // Private classes
    //

    // the interface implemented by any updatable object placed in the hash
    private interface UpdatableItem
    {
        public String getValue() throws PerformanceException;
    }

    private class NodeHash extends Hashtable<DefaultMutableTreeNode, UpdatableItem> {}

    // update the "CPUs online" value
    private class CPUsOnlineUpdater implements UpdatableItem
    {
        private CmGlobal<CmSysInfo> gsysInfo;

        /**
         * Constructor.
         * @param gsysInfo A <CODE>CmGlobal&lt;CmSysInfo&gt;</CODE> object.
         */
        public CPUsOnlineUpdater(CmGlobal<CmSysInfo> gsysInfo)
        {
            this.gsysInfo = gsysInfo;
        }

        /**
         * Return the text to be placed in the <CODE>JTree</CODE>.
         * @throws PerformanceException Any lower level exception.
         * @return The formatted string for the <CODE>Jtree</CODE>.
         */
        public String getValue()
            throws PerformanceException
        {
            try {
                gsysInfo.refresh();
            } catch(Exception e) {
                throw new PerformanceException(e.getMessage());
            }
            return XEMessages.msgs.getString("CPUs_online") +
                gsysInfo.getObject().cpuCountOnline.intValue();
        }
    }

    // update the "System Up Time" value
    private class UptimeUpdater implements UpdatableItem
    {
        private CmGlobal<CmSysInfo> gsysInfo;

        /**
         * Constructor.
         * @param gsysInfo A <CODE>CmGlobal&lt;CmSysInfo&gt;</CODE> object.
         */
        public UptimeUpdater(CmGlobal<CmSysInfo> gsysInfo)
        {
            this.gsysInfo = gsysInfo;
        }

        /**
         * Return the text to be placed in the <CODE>JTree</CODE>.
         * @throws PerformanceException Any lower level exception.
         * @return The formatted string for the <CODE>Jtree</CODE>.
         */
        public String getValue()
            throws PerformanceException
        {
            try {
                gsysInfo.refresh();
            } catch(Exception e) {
                throw new PerformanceException(e.getMessage());
            }
            return XEMessages.msgs.getString("Sys_UpTime") +
                CmSysInfo.formatUpTime(
                    gsysInfo.getObject().systemUpTime.intValue());
        }
    }

    // update the "memory %" value
    private class MemoryPercentUpdater implements UpdatableItem
    {
        private CmGlobal<CmSysInfo> gsysInfo;

        /**
         * Constructor.
         * @param gsysInfo A <CODE>CmGlobal&lt;CmSysInfo&gt;</CODE> object.
         */
        public MemoryPercentUpdater(CmGlobal<CmSysInfo> gsysInfo)
        {
            this.gsysInfo = gsysInfo;
        }

        /**
         * Return the text to be placed in the <CODE>JTree</CODE>.
         * @throws PerformanceException Any lower level exception.
         * @return The formatted string for the <CODE>Jtree</CODE>.
         */
        public String getValue()
            throws PerformanceException
        {
            try {
                gsysInfo.refresh();
            } catch(Exception e) {
                throw new PerformanceException(e.getMessage());
            }
            return XEMessages.msgs.getString("Mem_Percent") +
                gsysInfo.getObject().memoryFreePercent.intValue();
        }
    }

    // update the "page file %" value
    private class PageFilePercentUpdater implements UpdatableItem
    {
        private CmGlobal<CmPageFiles> gpf;
        private String                pfName;

        /**
         * Constructor.
         * @param gpf A <CODE>CmGlobal&lt;CmPageFiles&gt;</CODE> object.
         * @param pfName The name of the page file instance.
         */
        public PageFilePercentUpdater(CmGlobal<CmPageFiles> gpf, String pfName)
        {
            this.gpf = gpf;
            this.pfName = pfName;
        }

        /**
         * Return the text to be placed in the <CODE>JTree</CODE>.
         * @throws PerformanceException Any lower level exception.
         * @return The formatted string for the <CODE>Jtree</CODE>.
         */
        public String getValue()
            throws PerformanceException
        {
            try {
                gpf.refresh();
            } catch(Exception e) {
                throw new PerformanceException(e.getMessage());
            }
            CmPFMetrics pfm = gpf.getObject().get(pfName);
            int pctFree = 100 - pfm.percentUsed.intValue();

            return XEMessages.msgs.getString("Pct_Free") + pctFree;
        }
    }

    // update the global "page file %" value
    private class PageSpacePercentUpdater implements UpdatableItem
    {
        private CmGlobal<CmSysInfo> gsysInfo;

        /**
         * Constructor.
         * @param gsysInfo A <CODE>CmGlobal&lt;CmSysInfo&gt;</CODE> object.
         */
        public PageSpacePercentUpdater(CmGlobal<CmSysInfo> gsysInfo)
        {
            this.gsysInfo = gsysInfo;
        }

        /**
         * Return the text to be placed in the <CODE>JTree</CODE>.
         * @throws PerformanceException Any lower level exception.
         * @return The formatted string for the <CODE>Jtree</CODE>.
         */
        public String getValue()
            throws PerformanceException
        {
            try {
                gsysInfo.refresh();
            } catch(Exception e) {
                throw new PerformanceException(e.getMessage());
            }
            return XEMessages.msgs.getString("Pct_Free") +
                gsysInfo.getObject().pagingFileFreePercent.intValue();
        }
    }

    /**
     * Encapsulated logic for popping up a frame with a slider
     * control within it for changing the update interval.
     */
    private class IntervalListener implements ActionListener
    {
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
                    JSlider.HORIZONTAL, 0, 60, StInterval / 1000);
                label = new JLabel(
                    XEMessages.msgs.getString("Int_Secs") + StInterval / 1000);
                ok = new JButton(XEMessages.msgs.getString("OK"));
                
                slider.setMinorTickSpacing(1);
                slider.setMajorTickSpacing(10);
                slider.setPaintTicks(true);
                slider.setPaintLabels(true);
                slider.setLabelTable(slider.createStandardLabels(10));
                changeListener = new SliderMoved();
                slider.addChangeListener(changeListener);
                
                ok.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent ae) {
                        XESystemTree.this.stop();
                        XESystemTree.this.setDelay(StInterval);
                        XESystemTree.this.start();
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
                    XEMessages.msgs.getString("Int_Secs") + StInterval / 1000);
                slider.setValue(StInterval / 1000);
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
                StInterval = slider.getValue() * 1000;

                if (StInterval == 0)
                    StInterval = 1000;
                
                label.setText(
                    XEMessages.msgs.getString("Int_Secs") + StInterval / 1000);
            }
        }
    }

    // a JTree that can be right-clicked on
    private class ClickableTree extends JTree implements MouseListener
    {
        private JPopupMenu       pop;
        private JMenuItem        menuItem;
        private IntervalListener iList;

        public void erase() {
            menuItem.removeActionListener(iList);
            pop.remove(0);
            iList.erase();
            pop.setBorder(null);
            pop = null;
            menuItem = null;
            iList = null;
        }

        public ClickableTree(DefaultTreeModel m)
        {
            super(m);
            addMouseListener(this);
            pop = new JPopupMenu();
            menuItem = new JMenuItem(XEMessages.msgs.getString("Interval"), 'I');
            iList = new IntervalListener();
            menuItem.addActionListener(iList);
            pop.add(menuItem);
            pop.setBorder(new BevelBorder(BevelBorder.RAISED));
        }

        private void mouseDroppings(MouseEvent evt)
        {
            if (evt.isPopupTrigger()) {
                int x = evt.getX();
                int y = evt.getY();
                Point p = new Point(x, y);

                SwingUtilities.convertPointToScreen(p, this);

                iList.setXY(p.x, p.y);
                pop.show(this, x, y);
            }
        }

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
            mouseDroppings(evt);
        }

        /**
         * Mouse button has been released.
         * @param evt The MouseEvent
         */
        public void mouseReleased(MouseEvent evt)
        {
            mouseDroppings(evt);
        }

        /**
         * Mouse button has been pushed down and then released.
         * @param evt The MouseEvent
         */
        public void mouseClicked(MouseEvent evt)
        {
            mouseDroppings(evt);
        }
    }

    //
    // Public constructors
    //

    /**
     * Constructor for the system tree object.
     * @param parent The object that this object will be subordinate to.
     * @param remoteHost The name of the host to monitor.
     * @param msInterval The interval between updates.
     * @param args Optional list of name=value strings for intializing the area graph.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws UnknownHostException The specified host is unknown.
     * @throws PerformanceException Any possible lower level exception.
     */
    public XESystemTree(Container parent, String remoteHost, int msInterval, Object ... args)
        throws NotBoundException, MalformedURLException,
            RemoteException, UnknownHostException, PerformanceException
    {
        super(msInterval, null);

        StInterval = msInterval;
        StProps = null;
        StUpdateItems = new NodeHash();

        CmGlobal<CmSysInfo> gsysInfo = CmGlobal.getSysInfo(remoteHost);
        CmSysInfo sysInfo = gsysInfo.getObject();
        String hostName = null;

        if (remoteHost == null) {
            hostName = InetAddress.getLocalHost().getHostName();
            int dot = hostName.indexOf(".");
            if (dot != -1)
                hostName = hostName.substring(0, dot);
        } else
            hostName = remoteHost;

        CmGlobal<CmMounts> gdiskMap = CmGlobal.getMounts(remoteHost);
        StDiskMap = gdiskMap.getObject();

        // root node
        DefaultMutableTreeNode root = new DefaultMutableTreeNode(hostName);

        // the tree its own bad self
        DefaultTreeModel treeModel = new DefaultTreeModel(root);
        StTree = new ClickableTree(treeModel);

        this.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                try {
                    refresh();
                    StTree.repaint();
                } catch(PerformanceException e) {
                    // game over
                    XESystemTree.this.stop();
                    XEDialogs.fatalDialog(new CmExceptionParser(e).getMessage());
                }
            }
        });

        // identification node
        DefaultMutableTreeNode id = new DefaultMutableTreeNode(XEMessages.msgs.getString("Id"));
        DefaultMutableTreeNode platform = new DefaultMutableTreeNode(
            XEMessages.msgs.getString("OS_Plat") + sysInfo.osPlatform.toString());
        DefaultMutableTreeNode version = new DefaultMutableTreeNode(
            XEMessages.msgs.getString("OS_Vers") + sysInfo.osVersion.toString());
        DefaultMutableTreeNode pgsz = new DefaultMutableTreeNode(
            XEMessages.msgs.getString("Pg_Sz") + sysInfo.pageSize.intValue());
        DefaultMutableTreeNode uptime = new DefaultMutableTreeNode(
            XEMessages.msgs.getString("Sys_UpTime") +
                CmSysInfo.formatUpTime(sysInfo.systemUpTime.intValue()) +
                    "        "); // JTree won't make the text longer once set

        // add leaves to the id node
        id.add(platform);
        id.add(version);
        id.add(pgsz);
        id.add(uptime);

        StUpdateItems.put(uptime, new UptimeUpdater(gsysInfo));

        // add id node to root
        root.add(id);

        // processors node
        DefaultMutableTreeNode proc = new DefaultMutableTreeNode(XEMessages.msgs.getString("Proc"));
        DefaultMutableTreeNode arch = new DefaultMutableTreeNode(
            XEMessages.msgs.getString("CPU_Arch") + sysInfo.cpuArchitecture.toString());
        DefaultMutableTreeNode model = new DefaultMutableTreeNode(
            XEMessages.msgs.getString("CPU_Model") + sysInfo.cpuModel.toString());
        DefaultMutableTreeNode config = new DefaultMutableTreeNode(
            XEMessages.msgs.getString("CPUs_config") + sysInfo.cpuCountConfigured.intValue());
        DefaultMutableTreeNode online = new DefaultMutableTreeNode(
            XEMessages.msgs.getString("CPUs_online") + sysInfo.cpuCountOnline.intValue());
        DefaultMutableTreeNode clock = new DefaultMutableTreeNode(
            XEMessages.msgs.getString("Clock_Speed") + sysInfo.cpuClockSpeed.intValue() + " MHz");

        StUpdateItems.put(online, new CPUsOnlineUpdater(gsysInfo));

        // add leaves to proc node
        proc.add(arch);
        proc.add(model);
        proc.add(config);
        proc.add(online);
        proc.add(clock);

        // add proc to root
        root.add(proc);

        // memory node
        DefaultMutableTreeNode memory = new DefaultMutableTreeNode(XEMessages.msgs.getString("Mem"));
        DefaultMutableTreeNode memTot = new DefaultMutableTreeNode(
            XEMessages.msgs.getString("Mem_Tot") +
                XETopProcesses.formatSz(
                    sysInfo.memoryTotalMB.longValue() * 1024L * 1024L));
        DefaultMutableTreeNode memPercent = new DefaultMutableTreeNode(
            XEMessages.msgs.getString("Mem_Percent") + sysInfo.memoryFreePercent.intValue());

        StUpdateItems.put(memPercent, new MemoryPercentUpdater(gsysInfo));

        // add leaves to memory node
        memory.add(memTot);
        memory.add(memPercent);

        // add memory to root
        root.add(memory);

        // page files node
        DefaultMutableTreeNode pgFile =
            new DefaultMutableTreeNode(XEMessages.msgs.getString("Page_Files"));

        PerformanceHost host = new PerformanceHost(remoteHost);
        CmOSFlavor osFlavor = host.getOSFlavor();

        CmGlobal<CmPageFiles> gpf = CmGlobal.getPageFiles(remoteHost);
        CmPageFiles pf = gpf.getObject();

        for(Map.Entry<String, CmPFMetrics> e : pf.entrySet())
        {
            String pfName = e.getKey();
            CmPFMetrics pfm = e.getValue();
            int pctFree = 100 - pfm.percentUsed.intValue();
            DefaultMutableTreeNode pfNode = new DefaultMutableTreeNode(pfName);
            DefaultMutableTreeNode pfUsed =
                new DefaultMutableTreeNode(XEMessages.msgs.getString("Pct_Free") + pctFree);

            StUpdateItems.put(pfUsed, new PageFilePercentUpdater(gpf, pfName));

            pfNode.add(pfUsed);
            pgFile.add(pfNode);
        }

        DefaultMutableTreeNode pfTotal =
            new DefaultMutableTreeNode(XEMessages.msgs.getString("Total"));
        DefaultMutableTreeNode pfSize =
            new DefaultMutableTreeNode(XEMessages.msgs.getString("Size") +
                XETopProcesses.formatSz(
                    sysInfo.pagingFileTotalMB.longValue() * 1024L * 1024L));
        DefaultMutableTreeNode pfPctUsed =
            new DefaultMutableTreeNode(XEMessages.msgs.getString("Pct_Free") +
                sysInfo.pagingFileFreePercent.intValue());

        StUpdateItems.put(pfPctUsed, new PageSpacePercentUpdater(gsysInfo));

        // add leaves to total node
        pfTotal.add(pfSize);
        pfTotal.add(pfPctUsed);

        // add total node to page file
        pgFile.add(pfTotal);

        // add page files to root
        root.add(pgFile);

        // network interfaces node
        DefaultMutableTreeNode nics =
            new DefaultMutableTreeNode(XEMessages.msgs.getString("NICs"));

        CmGlobal<CmNicStat> nicGlobal = CmGlobal.getNicStat(remoteHost);
        CmNicStat nic = nicGlobal.getObject();

        for(Map.Entry<String, CmNicMetrics> e : nic.entrySet())
        {
            String inst = e.getKey();
            CmNicMetrics nm = e.getValue();

            if (nm.isLoopback.booleanValue())
                continue;

            DefaultMutableTreeNode nicName = new DefaultMutableTreeNode(inst);
            int speed = nm.interfaceSpeed.intValue();
            DefaultMutableTreeNode ifSpeed =
                new DefaultMutableTreeNode(XEMessages.msgs.getString("IF_Speed") +
                    (speed == 0 ? XEMessages.msgs.getString("unknown") :
                        (speed / 1000000 + " Mb/sec")));
            DefaultMutableTreeNode ipAddr =
                new DefaultMutableTreeNode(XEMessages.msgs.getString("IP_Addr") +
                    nm.ipAddress.toString());
            DefaultMutableTreeNode subnetMask =
                new DefaultMutableTreeNode(XEMessages.msgs.getString("Subnet_Mask") +
                    nm.subnetMask.toString());

            nicName.add(ifSpeed);
            nicName.add(ipAddr);
            nicName.add(subnetMask);
            nics.add(nicName);
        }

        // add nics to root
        root.add(nics);

        // disks node
        DefaultMutableTreeNode disks =
            new DefaultMutableTreeNode(XEMessages.msgs.getString("Disks"));
        DefaultMutableTreeNode physical =
            new DefaultMutableTreeNode(XEMessages.msgs.getString("Physical"));
        DefaultMutableTreeNode logical =
            new DefaultMutableTreeNode(XEMessages.msgs.getString("Logical"));
        int logicalCount = 0;

        CmGlobal<CmIoStat> gio = CmGlobal.getIoStat(remoteHost);
        CmIoStat io = gio.getObject();

        // make physical nodes
        for(Map.Entry<String, CmIoMetrics> e : io.entrySet())
        {
            String ioName = e.getKey();
            CmIoMetrics iom = e.getValue();

            if (iom.capacityMB.intValue() == 0)
                continue;

            DefaultMutableTreeNode diskName = null;

            if (iom.diskType.equals("physical"))
            {
                diskName = new DefaultMutableTreeNode(ioName);
                physical.add(diskName);
            }
            else if (iom.diskType.equals("logical"))
            {
                String name;

                // no-op on Windows
                if (osFlavor.isUnixHost())
                {
                    name = StDiskMap.toMountPoint(ioName);
                    if (name == ioName)
                    {
                        for(String s : pf.keySet())
                        {
                            int slash = s.lastIndexOf('/');

                            if (slash != -1)
                                s = s.substring(slash + 1, s.length());

                            if (ioName.equals(s))
                            {
                                name = "swap";
                                break;
                            }
                        }
                    }

                    if (name.equals("swap"))
                        name = name + ":" + ioName;
                } else
                    name = ioName;

                diskName = new DefaultMutableTreeNode(name);
                logical.add(diskName);
                logicalCount++;
            }

            if (diskName == null)
                continue;

            DefaultMutableTreeNode space =
                new DefaultMutableTreeNode(XEMessages.msgs.getString("Space") +
                    XETopProcesses.formatSz(
                        iom.capacityMB.longValue() * 1024L * 1024L));

            diskName.add(space);
        }

        // add nodes to disks
        disks.add(physical);
        if (logicalCount > 0)
            disks.add(logical);

        root.add(disks);

        // open the top node
        StTree.expandRow(0);

        JScrollPane jsp = new JScrollPane(StTree);

        StTreePane = jsp;

        if (args.length > 0) {
            if (args[0] instanceof XESysTreeProperties) {
                StProps = (XESysTreeProperties) args[0];
                if ((StInterval / 1000) != StProps.intervalSeconds) {
                    StInterval = StProps.intervalSeconds * 1000;
                    super.setDelay(StInterval);
                }
                setProperties(StProps);
            }
        }

        if (StProps == null)
            StProps = new XESysTreeProperties();
    }

    protected void finalize() {
        // System.out.println("XESystemTree.finalize");
    }
}
