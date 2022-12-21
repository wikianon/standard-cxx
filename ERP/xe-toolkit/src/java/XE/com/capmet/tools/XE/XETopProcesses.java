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
import java.net.URL;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.border.BevelBorder;
import javax.swing.plaf.UIResource;
import java.io.Serializable;
import java.util.Map;
import java.util.Formatter;
import java.util.Comparator;
import java.util.TreeSet;
import com.capmet.tools.XE.XEDialogs;
import com.capmet.metrics.cm.CmGlobal;
import com.capmet.metrics.cm.CmProcesses;
import com.capmet.metrics.cm.CmProcessMetrics;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.rmi.PerformanceException;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.CellConstraints;
import com.jgoodies.forms.builder.PanelBuilder;

/**
 * <CODE>XETopProcesses</CODE> is the base graphical entity for displaying
 * the processes on a host sorted ascending or descending by any of the
 * presented fields. <CODE>XETopProcesses</CODE> makes no assumptions about
 * whether it is an application or applet. It constructs
 * the graphical entity and makes it available from the
 * <CODE>getPane()</CODE> method.  This object extends <CODE>Timer</CODE>,
 * so after instantiation, a call to the <CODE>start</CODE> method
 * is necessary.
 *
 * <PRE>
 *   XETopProcesses procs = new XETopProcesses(parent, "humpty", 5000);
 *
 *   procs.start();
 * </PRE>
 */
public class XETopProcesses extends Timer implements XEFailureMethod {

    // turn on or off for debugging unintentional object retention
    private final boolean       discussFinalization = false;

    //
    // Private class members
    //

    private Container           TpParent;
    private ProcessTable        TpProcesses;
    private int                 TpInterval;
    private Container           TpPane;
    private int                 TpVisibleProcs = -1;
    private int                 TpProcessCount;
    private TreeSet<Long>       TpSelectedPids = null;
    private FieldsFrame         TpFieldsFrame;
    private TableColumnModel    TpColumnModel = null;
    private XETopProcProperties TpProps;
    private int                 TpSortKey;
    private boolean             TpViewSystemProcs = false;
    private CmProcessMetrics    TpTopProcess = null;
    private long                TpOffensivePid = -1;
    private Color               TpOffensiveColor;
    private TitleMap[]          titleStrings = new TitleMap[]
    {
        new TitleMap("PID",       D.PN.PN_PROCESS_ID,             Number.class, true),
        new TitleMap("PPID",      D.PN.PN_PARENT_PROCESS_ID,      Number.class, false),
        new TitleMap("USERNAME",  D.PN.PN_USER_NAME,              String.class, true),
        new TitleMap("SIZE",      D.PN.PN_TOTAL_SIZE_KB,          Number.class, false),
        new TitleMap("RSS",       D.PN.PN_RESIDENT_SIZE_KB,       Number.class, true),
        new TitleMap("SZD",       D.PN.PN_TOTAL_SIZE_DELTA_KB,    Number.class, false),
        new TitleMap("RSSD",      D.PN.PN_RESIDENT_SIZE_DELTA_KB, Number.class, false),
        new TitleMap("STATE",     D.PN.PN_PROCESS_STATE,          String.class, true),
        new TitleMap("TIME",      D.PN.PN_TOTAL_CPU_SECONDS,      Number.class, true),
        new TitleMap("USER_TIME", D.PN.PN_USER_CPU_SECONDS,       Number.class, false),
        new TitleMap("SYS_TIME",  D.PN.PN_KERNEL_CPU_SECONDS,     Number.class, false),
        new TitleMap("USER_PCT",  D.PN.PN_PERCENT_USER_TIME,      Number.class, false),
        new TitleMap("SYS_PCT",   D.PN.PN_PERCENT_KERNEL_TIME,    Number.class, false),
        new TitleMap("IO_sec",    D.PN.PN_TOTAL_IO_KB,            Number.class, true),
        new TitleMap("MEM_PCT",   D.PN.PN_PERCENT_PHYSMEM_USE,    Number.class, false),
        new TitleMap("CPU_PCT",   D.PN.PN_PERCENT_CPU_USE,        Number.class, true),
        new TitleMap("PG_FAULTS", D.PN.PN_PAGE_FAULTS,            Number.class, false),
        new TitleMap("SYS_CALLS", D.PN.PN_SYSTEM_CALLS,           Number.class, false),
        new TitleMap("CSWITCHES", D.PN.PN_CONTEXT_SWITCHES,       Number.class, false),
        new TitleMap("PRIO",      D.PN.PN_GLOBAL_PRIORITY,        Number.class, false),
        new TitleMap("THREADS",   D.PN.PN_NUMBER_OF_THREADS,      Number.class, false),
        new TitleMap("COMMAND",   D.PN.PN_COMMAND_NAME,           String.class, true),
        new TitleMap("FULL_CMD",  D.PN.PN_COMMAND_LINE,           String.class, false)
    };
    
    //
    // Public methods
    //
    
    /**
     * Format a long "size" into a string with K, MB or GB suffix.
     * @param size The long as source for formatting.
     * @return A string representation of the long value with suffix.
     */
    public static String formatSz(long size) {
        final long one_meg = 1024 * 1024;
        final long ten_meg = 10 * one_meg;
        final long one_gig = 1024 * one_meg;
        final long ten_gig = 10 * one_gig;
        long value;
        String neg = "";
        
        if (size < 0) {
            size *= -1;
            neg = "-";
        }
        if (size >= ten_gig) {
            value = size / one_gig;
            if ((size % one_gig) != 0)
                value++;
            return neg + value + "G";
        }
        if (size >= ten_meg) {
            value = size / one_meg;
            if ((size % one_meg) != 0)
                value++;
            return neg + value + "M";
        }
        if (size >= 1024) {
            value = size / 1024;
            if ((size % 1024) != 0)
                value++;
            return neg + value + "K";
        }
        return neg + size;
    }
    
    /**
     * Disconnect references to objects for better memory management.
     */
    public void erase() {
        for(ActionListener ae : getActionListeners())
            removeActionListener(ae);

        TpParent = null;

        if (TpProcesses != null)
            TpProcesses.erase();
        TpProcesses = null;

        ((JScrollPane) TpPane).getViewport().setView(null);
        TpPane = null;

        if (TpSelectedPids != null)
            TpSelectedPids.clear();
        TpSelectedPids = null;

        if (TpFieldsFrame != null)
            TpFieldsFrame.erase();
        TpFieldsFrame = null;

        TpColumnModel = null;
        TpProps = null;
        TpTopProcess = null;
        TpOffensiveColor = null;

        titleStrings = null;
    }

    /**
     * Default method for handling failure. Application should extend
     * this class and override this method for a more graceful end.
     * @param message The failure message.
     */
    public void failureEvent(String message) {
        XETopProcesses.this.stop();
        XEDialogs.fatalDialog(message);
    }

    /**
     * Retrieve the <CODE>Container</CODE> holding all of the entities.
     * @return The <CODE>Container</CODE> used by the layout manager where
     * all of the graphical entities are located.
     */
    public Container getPane() {
        return TpPane;
    }
    
    /**
     * Change the number of processes to display.
     * @param count The process count.
     */
    public void setVisibleProcs(int count) {
        TpVisibleProcs = count;
        if (TpProcesses == null)
            return;
        ProcModel pm = TpProcesses.getProcModel();
        if (pm == null)
            return;
        pm.redraw();
        pm.fireTableDataChanged();
    }
    
    public CmProcessMetrics getProcess(long pid) {
        return TpProcesses.getProcModel().getProcess(pid);
    }

    /**
     * Get the current process count.
     * @return The process count.
     */
    public int getProcessCount() {
        return TpProcessCount;
    }
    
    /**
     * Get the <CODE>JFrame</CODE> containing the field check boxes.
     * @return The <CODE>JFrame</CODE> object.
     */
    public JFrame getFieldsBoxFrame() {
        return TpFieldsFrame;
    }
    
    /**
     * Override the <CODE>setDelay</CODE> method of <CODE>Timer</CODE>
     * so this object can track the interval changes. Sets the
     * <CODE>Timer</CODE> delay, the number of milliseconds
     * between successive action events.
     * @param delay The delay in milliseconds.
     */
    public void setDelay(int delay) {
        TpInterval = delay;
        super.setDelay(delay);
    }
    
    /**
     * Returns the delay, in milliseconds,
     * between firings of action events.
     * @return The number of milliseconds of delay.
     */
    public int getDelay() {
        TpInterval = super.getDelay();
        return TpInterval;
    }

    /**
     * Retrieve the properties structure for this object.
     * @return The <CODE>XEPieChartProperties</CODE> containing the
     * properties of the pie chart.
     */
    public XETopProcProperties getProperties() {
        TpProps.visibleProcs = TpVisibleProcs;
        TpProps.selectedPids = TpSelectedPids;
        TpProps.intervalSeconds = TpInterval / 1000;
        TpProcesses.getProcModel().getProperties();
        return TpProps;
    }

    /**
     * Add pids of processes with specified process name
     * to the list of selected processes to be highlighted.
     * @param processName The name of the processes whose pids are highlighted.
     */
    public void watchProcessName(String processName) {
        TpProcesses.getProcModel().watchProcessName(processName);
    }

    public CmProcessMetrics getTopProcess() {
        return TpTopProcess;
    }

    public void watchOffensiveProcess(long pid, Color state) {
        TpOffensivePid = pid;
        TpOffensiveColor = state;
    }

    public void cancelOffensiveWatch() {
        TpOffensivePid = -1;
    }

    public boolean attemptToSortBy(int index, boolean descending)
        throws PerformanceException {

        // already doing it; move along...
        if (descending && (-index == TpSortKey))
            return true;

        if (!descending && (index == TpSortKey))
            return true;

        // hunt down the index the old fashioned way
        for(int i=0; i<titleStrings.length; i++) {
            if (titleStrings[i].index == index) {
                // if it's not supported, we're done
                if (!titleStrings[i].supported)
                    return false;

                if (!titleStrings[i].visible) {
                    titleStrings[i].visible = true;
                    TpFieldsFrame.setChecked(i);
                }

                // all set, do it
                break;
            }
        }

        ProcModel pm = TpProcesses.getProcModel();

        TpSortKey = index;
        if (descending)
            TpSortKey *= -1;

        try {
            pm.refresh();
            pm.redraw();
            pm.fireTableStructureChanged();
            resizeStringCells(pm);
        } catch(Exception e) {
            throw new PerformanceException(
                new CmExceptionParser(e).getMessage());
        }

        return true;
    }

    public boolean getViewSystemProcs() {
        return TpViewSystemProcs;
    }

    public void setViewSystemProcs(boolean view) {
        TpViewSystemProcs = view;
    }

    public int getSortKey() {
        return TpSortKey;
    }

    public String getSortKeyName() {
        return getSortKeyName(TpSortKey);
    }

    public String getSortKeyName(int sortKey) {
        int sk = (sortKey < 0 ? -sortKey : sortKey);

        for(int i=0; i<titleStrings.length; i++)
            if (titleStrings[i].index == sk)
                return titleStrings[i].title;
        return null;
    }
    
    //
    // Private class methods
    //

    // resize certain columns that are String types
    private void resizeStringCells(ProcModel pm) {
        TableColumn col;
        int index;
        int w;
        
        // User Name column
        index = pm.getHeaderNameIndex(D.PN.PN_USER_NAME);
        if (index != -1) {
            col = TpColumnModel.getColumn(index);
            
            // get the width and increase it by one half
            w = col.getPreferredWidth();
            col.setPreferredWidth(w + (w / 2));
        }
        
        // Command column
        index = pm.getHeaderNameIndex(D.PN.PN_COMMAND_NAME);
        if (index != -1) {
            col = TpColumnModel.getColumn(index);
            
            // get the width and double it
            col.setPreferredWidth(col.getPreferredWidth() * 2);
        }
        
        // State column
        index = pm.getHeaderNameIndex(D.PN.PN_PROCESS_STATE);
        if (index != -1) {
            col = TpColumnModel.getColumn(index);
            
            // get the width and increase by one quarter
            w = col.getPreferredWidth();
            col.setPreferredWidth(w + (w / 4));
        }
        
        // Command Line column
        index = pm.getHeaderNameIndex(D.PN.PN_COMMAND_LINE);
        if (index != -1) {
            col = TpColumnModel.getColumn(index);
            
            // get the width and increase by 400%
            w = col.getPreferredWidth();
            col.setPreferredWidth(w * 4);
        }
    }
    
    //
    // Private classes
    //

    // This is the collection of columns and heading names
    private class TitleMap {
        /**
         * The heading.
         */
        public String  title;
        /**
         * The index from D.PN
         */
        public int     index;
        /**
         * The class (for formatting).
         */
        public Class   clazz;
        /**
         * Whether this is a selected column to display.
         */
        public boolean visible;
        /**
         * Whether this field is supported on the target host.
         */
        public boolean supported;
        /**
         * The preferred width of the field.
         */
        public int preferredWidth;
        
        /**
         * Construct the TitleMap.
         * @param title The title
         * @param index The index
         * @param clazz The class
         * @param visible Is visible
         */
        public TitleMap(String title, int index, Class clazz, boolean visible) {
            this.title     = XEMessages.msgs.getString(title);
            this.index     = index;
            this.clazz     = clazz;
            this.visible   = visible;
            this.supported = false; // set by ProcModel constructor
            this.preferredWidth = -1;
        }
    }
    
    // extend the AbstractTableModel to provide additional functionality
    private class ProcModel extends AbstractTableModel {
        //
        // Private members
        //
        private CmGlobal<CmProcesses> gproc;
        private CmProcesses   proc;
        private StringBuilder stringBuilder;
        private Formatter     fmt;
        private String[]      titles = null;
        private Class[]       types = null;
        private Object[][]    processData = null;
        private String        remoteHost;
        private boolean       reverseSort;
        private int           procCount;
        
        //
        // Public methods
        //
        
        public void erase() {
            gproc = null;
            proc = null;
            stringBuilder = null;
            fmt = null;
            titles = null;
            types = null;
            processData = null;
            remoteHost = null;
        }

        /**
         * Make a contribution to the <CODE>XETopProcProperties</CODE>
         * properties structure regarding values in our purview.
         */
        public void getProperties() {
            boolean[] v = new boolean[titleStrings.length];
            int[] w = new int[titleStrings.length];
            int index;
            
            for(int i=0; i<titleStrings.length; i++) {
                v[i] = titleStrings[i].visible;
                w[i] = -1;
                index = -1;
                for(int j=0; j<titles.length; j++) {
                    if (titles[j].equals(titleStrings[i].title)) {
                        index = j;
                        break;
                    }
                }
                if (index == -1)
                    continue;
                TableColumn col = TpColumnModel.getColumn(index);
                w[i] = col.getPreferredWidth();
            }
            TpProps.visibleFields = v;
            TpProps.fieldWidths = w;
            TpProps.sortKey = TpSortKey;
            TpProps.reverseSort = reverseSort;
        }
        
        /**
         * Set the values in our purview from the supplied properties structure.
         * @param props The properties structure.
         */
        public void setProperties() {
            int i;
            int j;

            TpSortKey = TpProps.sortKey;
            reverseSort = TpProps.reverseSort;

            for(i=0; i<titleStrings.length; i++) {
                titleStrings[i].visible = TpProps.visibleFields[i];
                titleStrings[i].preferredWidth = TpProps.fieldWidths[i];
            }
        }
        
        /**
         * Get the column index for the D.PN index.
         * @param index The D.PN index.
         * @return The column index.
         */
        public int getHeaderNameIndex(int index) {
            for(int i=0; i<titleStrings.length; i++)
                if (titleStrings[i].index == index) {
                    for(int j=0; j<titles.length; j++)
                        if (titles[j].equals(titleStrings[i].title))
                            return j;
                    break;
                }
            return -1;
        }
        
        /**
         * Get the D.PN index number for the string title.
         * @param title The header title
         * @return The D.PN constant representing that column.
         */
        public int getHeaderIndexByTitle(String title) {
            for(int i=0; i<titleStrings.length; i++)
                if (titleStrings[i].title.equals(title))
                    return titleStrings[i].index;
            return -1;
        }

        /**
         * Set the preferred field widths of the table columns.
         */
        public void setWidths() {
            int index;

            for(int i=0; i<titleStrings.length; i++) {
                index = -1;
                for(int j=0; j<titles.length; j++) {
                    if (titles[j].equals(titleStrings[i].title)) {
                        index = j;
                        break;
                    }
                }
                if (index == -1 || TpProps.fieldWidths[i] == -1)
                    continue;
                TableColumn col = TpColumnModel.getColumn(index);
                col.setPreferredWidth(TpProps.fieldWidths[i]);
            }
        }
        
        /**
         * Get the number of rows.
         * @return The number of rows.
         */
        public int getRowCount() {
            return processData.length;
        }
        
        /**
         * Get the number of columns.
         * @return The number of columns.
         */
        public int getColumnCount() {
            return titles.length;
        }
        
        /**
         * Get the name of the column.
         * @param c The column number.
         * @return The name in the header of that column.
         */
        public String getColumnName(int c) {
            return titles[c];
        }
        
        /**
         * Get the class of the column.
         * @param c The column number.
         * @return The class of that column.
         */
        public Class getColumnClass(int c) {
            return types[c];
        }
        
        /**
         * Get the value for the specified row and column.
         * @param r The row number.
         * @param c The column number.
         * @return The object at those coordinates.
         */
        public Object getValueAt(int r, int c) {
            return processData[r][c];
        }
        
        /**
         * Sort the processes by the string from the header.
         * @param title The header title.
         */
        public void sortByTitle(String title) {
            int index = getHeaderIndexByTitle(title);
            
            if (index == -1)
                return;

            if ((index == TpSortKey) || (index == -TpSortKey))
                TpSortKey = -TpSortKey;
            else {
                if (TpSortKey < 0)
                    TpSortKey = -index;
                else
                    TpSortKey = index;
            }
            
            if (TpSortKey < 0) {
                proc.rsort(-TpSortKey);
                reverseSort = true;
            } else {
                proc.sort(TpSortKey);
                reverseSort = false;
            }
            
            redraw();
        }
        
        /**
         * Get a new data collection and build the necessary arrays for the AbstractTableModel.
         */
        public void refresh() throws
                RemoteException, NotBoundException,
                MalformedURLException, PerformanceException {
            int i;
            int j;
            int fieldCount = 0;
            
            if (proc == null) {
                gproc = CmGlobal.getProcesses(remoteHost);
                proc = gproc.getObject();
                proc.rsort(D.PN.PN_PERCENT_CPU_USE);
                TpSortKey = -D.PN.PN_PERCENT_CPU_USE;
                for(i=0; i<titleStrings.length; i++)
                    titleStrings[i].supported = proc.isSupported(titleStrings[i].index);
                TpFieldsFrame = new FieldsFrame();
            } else {
                proc.refresh();
                if (TpSortKey < 0) {
                    proc.rsort(-TpSortKey);
                    reverseSort = true;
                } else {
                    proc.sort(TpSortKey);
                    reverseSort = false;
                }
            }
            
            // count supported fields to be displayed
            for(i=0; i<titleStrings.length; i++)
                if (titleStrings[i].visible && titleStrings[i].supported)
                    fieldCount++;
            
            // allocate
            if (titles == null || titles.length != fieldCount)
                titles = new String[fieldCount];
            if (types == null || types.length != fieldCount)
                types = new Class[fieldCount];
            
            // assign values
            for(i=j=0; i<titleStrings.length; i++) {
                if (titleStrings[i].visible && titleStrings[i].supported) {
                    titles[j] = titleStrings[i].title;
                    types[j] = titleStrings[i].clazz;
                    j++;
                }
            }
            
            TpProcessCount = procCount = proc.size();
        }
        
        /**
         * Redraw the table.
         */
        public void redraw() {
            if (TpVisibleProcs != -1)
                procCount = TpVisibleProcs;
            
            // try to cut down on superfluous allocations
            if (processData == null || processData.length != procCount ||
                    processData[0].length != titles.length)
                processData = new Object[procCount][titles.length];
            
            int i = 0;
            double total;
            int mins;
            int secs;
            
            TpTopProcess = null;
            for(CmProcessMetrics pm : proc.sortedValues()) {
                if (!TpViewSystemProcs && pm.isSystemProcess.booleanValue())
                    continue;
                if (TpTopProcess == null)
                    TpTopProcess = pm;
                for(int x=0, n=0; x<titleStrings.length; x++) {
                    if (!titleStrings[x].visible || !titleStrings[x].supported)
                        continue;
                    switch(titleStrings[x].index) {
                        case D.PN.PN_PROCESS_ID:
                            processData[i][n++] = new Long(pm.processID.longValue());
                            break;
                        case D.PN.PN_PARENT_PROCESS_ID:
                            processData[i][n++] = new Long(pm.parentProcessID.longValue());
                            break;
                        case D.PN.PN_USER_NAME:
                            processData[i][n++] = pm.userName.toString();
                            break;
                        case D.PN.PN_TOTAL_SIZE_KB:
                            processData[i][n++] = formatSz(pm.totalSizeKB.intValue() * 1024L);
                            break;
                        case D.PN.PN_RESIDENT_SIZE_KB:
                            processData[i][n++] = formatSz(pm.residentSizeKB.intValue() * 1024L);
                            break;
                        case D.PN.PN_TOTAL_SIZE_DELTA_KB:
                            processData[i][n++] = formatSz(pm.totalSizeDeltaKB.intValue() * 1024L);
                            break;
                        case D.PN.PN_RESIDENT_SIZE_DELTA_KB:
                            processData[i][n++] = formatSz(pm.residentSizeDeltaKB.intValue() * 1024L);
                            break;
                        case D.PN.PN_PROCESS_STATE:
                            processData[i][n++] = pm.processState.toString();
                            break;
                        case D.PN.PN_TOTAL_CPU_SECONDS:
                            total = pm.totalCPUSeconds.doubleValue();
                            mins = (int) (total / 60.0);
                            total -= (mins * 60.0);
                            secs = (int) total;
                            fmt.format("%3d:%02d", mins, secs);
                            processData[i][n++] = stringBuilder.toString();
                            stringBuilder.setLength(0);
                            break;
                        case D.PN.PN_USER_CPU_SECONDS:
                            total = pm.userCPUSeconds.doubleValue();
                            mins = (int) (total / 60.0);
                            total -= (mins * 60.0);
                            secs = (int) total;
                            fmt.format("%3d:%02d", mins, secs);
                            processData[i][n++] = stringBuilder.toString();
                            stringBuilder.setLength(0);
                            break;
                        case D.PN.PN_KERNEL_CPU_SECONDS:
                            total = pm.kernelCPUSeconds.doubleValue();
                            mins = (int) (total / 60.0);
                            total -= (mins * 60.0);
                            secs = (int) total;
                            fmt.format("%3d:%02d", mins, secs);
                            processData[i][n++] = stringBuilder.toString();
                            stringBuilder.setLength(0);
                            break;
                        case D.PN.PN_PERCENT_USER_TIME:
                            fmt.format("%6.2f", pm.percentUserTime.doubleValue());
                            processData[i][n++] = stringBuilder.toString();
                            stringBuilder.setLength(0);
                            break;
                        case D.PN.PN_PERCENT_KERNEL_TIME:
                            fmt.format("%6.2f", pm.percentKernelTime.doubleValue());
                            processData[i][n++] = stringBuilder.toString();
                            stringBuilder.setLength(0);
                            break;
                        case D.PN.PN_TOTAL_IO_KB:
                            processData[i][n++] = formatSz(pm.totalIOKB.intValue() * 1024L);
                            break;
                        case D.PN.PN_PERCENT_PHYSMEM_USE:
                            fmt.format("%6.2f", pm.percentPhysmemUse.doubleValue());
                            processData[i][n++] = stringBuilder.toString();
                            stringBuilder.setLength(0);
                            break;
                        case D.PN.PN_PERCENT_CPU_USE:
                            fmt.format("%6.2f", pm.percentCPUUse.doubleValue());
                            processData[i][n++] = stringBuilder.toString();
                            stringBuilder.setLength(0);
                            break;
                        case D.PN.PN_PAGE_FAULTS:
                            processData[i][n++] = new Integer(pm.pageFaults.intValue());
                            break;
                        case D.PN.PN_SYSTEM_CALLS:
                            processData[i][n++] = new Integer(pm.systemCalls.intValue());
                            break;
                        case D.PN.PN_CONTEXT_SWITCHES:
                            processData[i][n++] = new Integer(pm.contextSwitches.intValue());
                            break;
                        case D.PN.PN_GLOBAL_PRIORITY:
                            processData[i][n++] = new Integer(pm.globalPriority.intValue());
                            break;
                        case D.PN.PN_NUMBER_OF_THREADS:
                            processData[i][n++] = new Integer(pm.numberOfThreads.intValue());
                            break;
                        case D.PN.PN_COMMAND_NAME:
                            processData[i][n++] = pm.commandName.toString();
                            break;
                        case D.PN.PN_COMMAND_LINE:
                            processData[i][n++] = pm.commandLine.toString();
                            break;
                        default:
                            // hmmm
                            continue;
                    }
                }
                
                if (++i == TpVisibleProcs)
                    break;
            }
            
            // if TpViewSystemProcs chopped off some processes
            if (processData.length > i) {
                Object[][] tmp = new Object[i][titles.length];
                
                System.arraycopy(processData, 0, tmp, 0, i);
                
                processData = tmp;
            }
        }

        /**
         * Add pids of processes with specified process name
         * to the list of selected processes to be highlighted.
         * @param processName The name of the processes whose pids are highlighted.
         */
        public void watchProcessName(String processName) {
            boolean changed = false;

            for(CmProcessMetrics pm : proc.values()) {
                if (pm.commandName.toString().equals(processName)) {
                    long pid = pm.processID.longValue();

                    if (TpSelectedPids.contains(pid))
                        TpSelectedPids.remove(pid);
                    else
                        TpSelectedPids.add(pid);

                    changed = true;
                }
            }

            if (changed) {
                redraw();
                fireTableDataChanged();
            }
        }

        public CmProcessMetrics getProcess(long pid) {
            for(CmProcessMetrics pm : proc.values())
                if (pm.processID.longValue() == pid)
                    return pm;
            return null;
        }
        
        //
        // Private methods
        //
        
        //
        // Public constructors
        //
        
        /**
         * Construct the ProcModel for the local host.
         * @throws NotBoundException Service is not bound on server machine.
         * @throws MalformedURLException The service signature is malformed.
         * @throws RemoteException RMI exception
         * @throws PerformanceException Any possible lower level exception.
         */
        public ProcModel() throws
                RemoteException, NotBoundException,
                MalformedURLException, PerformanceException {
            this("localhost", false);
        }
        
        /**
         * Construct the ProcModel for the remote host.
         * @param remoteHost The remote host.
         * @throws NotBoundException Service is not bound on server machine.
         * @throws MalformedURLException The service signature is malformed.
         * @throws RemoteException RMI exception
         * @throws PerformanceException Any possible lower level exception.
         */
        public ProcModel(String remoteHost, boolean setProperties) throws
                RemoteException, NotBoundException,
                MalformedURLException, PerformanceException {
            if (setProperties)
                this.setProperties();
            this.remoteHost = remoteHost;
            stringBuilder = new StringBuilder();
            fmt = new Formatter(stringBuilder);
            refresh();
            redraw();
        }
    }

    // extend the JTableHeader to make the headings "clickable"
    private class ClickableHeader extends JTableHeader implements MouseListener {
        public static final int COLUMN_HEADER_HEIGHT = 30;

        private HeaderRenderer headerRenderer;

        public void erase() {
            if (headerRenderer != null)
                headerRenderer.erase();
            headerRenderer = null;
            setDefaultRenderer(null);
            removeMouseListener(this);
        }

        private class HeaderRenderer implements TableCellRenderer {
            private JLabel   upArrow;
            private JLabel   downArrow;
            private JLabel   blank;
            private JPanel[] headings = null;

            public void erase() {
                upArrow = null;
                downArrow = null;
                blank = null;
                headings = null;
            }

            public HeaderRenderer() {
                final String upPath = "com/capmet/tools/XE/images/up.png";
                final String downPath = "com/capmet/tools/XE/images/down.png";
                final String blankPath = "com/capmet/tools/XE/images/blank.png";
                URL upURL = ClassLoader.getSystemClassLoader().getResource(upPath);
                URL downURL = ClassLoader.getSystemClassLoader().getResource(downPath);
                URL blankURL = ClassLoader.getSystemClassLoader().getResource(blankPath);
                Icon upIcon = new ImageIcon(upURL);
                Icon downIcon = new ImageIcon(downURL);
                Icon blankIcon = new ImageIcon(blankURL);

                upArrow = new JLabel(upIcon);
                downArrow = new JLabel(downIcon);
                blank = new JLabel(blankIcon);

                int n = titleStrings.length;
                String ttt = XEMessages.msgs.getString("Press_Sort");

                headings = new JPanel[n];
                for(int i=0; i<n; i++) {
                    headings[i] = new JPanel();
                    headings[i].setBorder(new BevelBorder(BevelBorder.RAISED));
                    headings[i].setToolTipText(ttt);
                }
            }

            public Component getTableCellRendererComponent(JTable table, Object value,
                                            boolean isSelected, boolean hasFocus,    
                                            int row, int column) {

                // get the panel for this column
                int index = 0;
                int count = 0;
                int sortKey = (TpSortKey < 0 ? -TpSortKey : TpSortKey);
                int sortIndex = -1;
                int matchCount = 0;

                for(int i=0; i<titleStrings.length; i++) {
                    if (titleStrings[i].title.equals((String) value)) {
                        index = i;
                        matchCount++;
                    }
                    if (titleStrings[i].index == sortKey) {
                        sortIndex = i;
                        matchCount++;
                    }
                    if (matchCount == 2)
                        break;
                }

                JPanel p = headings[index];
                JLabel arrow = blank;

                // remove is broken. screw it, remove everything.
                p.removeAll();

                if (index == sortIndex)
                    if (TpSortKey < 0)
                        arrow = downArrow;
                    else
                        arrow = upArrow;

                String t = titleStrings[index].title;
                JLabel text = new JLabel(t);
                p.add(text, BorderLayout.WEST);
                p.add(arrow, BorderLayout.EAST);

                return p;
            }
        }

        public Object getHeaderPanel(String title) {
            return headerRenderer.getTableCellRendererComponent(
                TpProcesses, title, false, false, 0, 0);
        }

        /**
         * Constructor.
         * @param cm The TableColumnModel
         */
        public ClickableHeader(TableColumnModel cm) {
            super(cm);
            addMouseListener(this);
            headerRenderer = new HeaderRenderer();
            setDefaultRenderer(headerRenderer);
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
            ProcModel pm = (ProcModel) getTable().getModel();
            int n = pm.getColumnCount();
            Point p = evt.getPoint();
            Rectangle r = null;
            int i;
            
            // find the heading in question
            for(i=0; i<n; i++) {
                r = getHeaderRect(i);
                if (p.x >= r.x && p.x <= (r.x + r.width))
                    break;
            }
            
            // if found, draw a black line around it
            if (i != n) {
                Graphics g = getGraphics();
                int width = r.width - 2;
                
                g.setColor(Color.BLACK);
                g.drawRect(r.x, 0, width, COLUMN_HEADER_HEIGHT - 2);
                pm.sortByTitle(getTable().getColumnName(i));
            }
        }
        
        /**
         * Mouse button has been released.
         * @param evt The MouseEvent
         */
        public void mouseReleased(MouseEvent evt) {
        }
        
        /**
         * Mouse button has been pushed down and then released.
         * @param evt The MouseEvent
         */
        public void mouseClicked(MouseEvent evt) {
        }
    }

    private class FieldsFrame extends JFrame {
        private FieldBox       fb;
        private JCheckBox[]    cBoxes;
        private JButton        apply;
        private JButton        close;
        
        public void erase() {
            dispose();
            if (fb != null)
                fb.erase();
            fb = null;

            for(JCheckBox cb : cBoxes)
                for(ActionListener ae : cb.getActionListeners())
                    cb.removeActionListener(ae);
            cBoxes = null;

            for(ActionListener ae : apply.getActionListeners())
                apply.removeActionListener(ae);
            for(ActionListener ae : close.getActionListeners())
                close.removeActionListener(ae);
        }

        // The field selection check boxes for choosing which columns to view.
        private class FieldBox implements ActionListener {
            private boolean[] checkBoxes;
            
            public void erase() {
                checkBoxes = null;
            }

            /**
             * Constructor.
             */
            public FieldBox() {
                checkBoxes = new boolean[titleStrings.length];
                for(int i=0; i<titleStrings.length; i++)
                    checkBoxes[i] = titleStrings[i].visible;
            }
            
            /**
             * When this menu item is selected.
             * @param ae The action event.
             */
            public void actionPerformed(ActionEvent ae) {
                JCheckBox cBox = (JCheckBox) ae.getSource();
                String title = cBox.getText();
                int index = -1;
                
                for(int i=0; i<titleStrings.length; i++) {
                    if (titleStrings[i].title.equals(title)) {
                        index = i;
                        break;
                    }
                }
                
                checkBoxes[index] = cBox.isSelected();
            }
            
            /**
             * Sync the local booleans to the titleStrings member
             * and cause the JTable to restructure itself.
             */
            public boolean sync() throws PerformanceException {
                boolean changed = false;
                ProcModel pm = TpProcesses.getProcModel();
                
                for(int i=0; i<checkBoxes.length; i++) {
                    if (titleStrings[i].visible != checkBoxes[i]) {
                        titleStrings[i].visible = checkBoxes[i];
                        changed = true;
                    }
                }
                
                if (!changed)
                    return false;
                
                try {
                    pm.refresh();
                    pm.redraw();
                } catch(Exception e) {
                    throw new PerformanceException(
                        new CmExceptionParser(e).getMessage());
                }
                pm.fireTableStructureChanged();
                return true;
            }

            public void setChecked(int index) {
                if (index < 0 || index >= checkBoxes.length)
                    return;
                checkBoxes[index] = true;
            }
        }

        // build the JFrame with the check boxes in it.
        public FieldsFrame() {
            super(XEMessages.msgs.getString("Select_Fields"));

            FormLayout layout = new FormLayout( // 5x5 grid
                    "l:p:g, 4dlu, l:p:g, 4dlu, l:p:g, 4dlu, l:p:g, 4dlu, l:p:g",  // cols
                    "c:p:g, 4dlu, c:p:g, 4dlu, c:p:g, 4dlu, c:p:g, 4dlu, c:p:g, 10dlu, c:p:g"); // rows
            PanelBuilder builder = new PanelBuilder(layout);
            CellConstraints cc = new CellConstraints();
            int x = 1;
            int y = 1;
            
            fb = new FieldBox();
            cBoxes = new JCheckBox[titleStrings.length];
            
            for(int i=0; i<titleStrings.length; i++) {
                JCheckBox cBox = new JCheckBox(titleStrings[i].title,
                        titleStrings[i].visible && titleStrings[i].supported);

                cBoxes[i] = cBox;
                
                if (i > 0 && (i % 5) == 0) {
                    x = 1;
                    y += 2;
                }
                
                if (i > 0 && titleStrings[i].supported)
                    cBox.addActionListener(fb);
                else
                    cBox.setEnabled(false);
                
                builder.add(cBox, cc.xy(x, y));
                x += 2;
            }
            
            apply = new JButton(XEMessages.msgs.getString("Apply"));
            close = new JButton(XEMessages.msgs.getString("Close"));
            apply.setToolTipText(XEMessages.msgs.getString("Apply_Changes"));
            close.setToolTipText(XEMessages.msgs.getString("Close_Window"));

            FormLayout acButtons = new FormLayout("c:p:g, 4dlu, c:p:g", "c:p:g");
            PanelBuilder acBuilder = new PanelBuilder(acButtons);
            acBuilder.add(apply, cc.xy(1, 1));
            acBuilder.add(close, cc.xy(3, 1));
            
            builder.setDefaultDialogBorder();
            builder.add(acBuilder.getPanel(), cc.xy(5, 11));
            
            apply.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    try {
                        if (fb.sync())
                            resizeStringCells(TpProcesses.getProcModel());
                    } catch(Exception e) {
                        failureEvent(e.getMessage());
                    }
                }
            });
            
            close.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent ae) {
                    try {
                        if (fb.sync())
                            resizeStringCells(TpProcesses.getProcModel());
                    } catch(Exception e) {
                        failureEvent(e.getMessage());
                    }
                    setVisible(false);
                }
            });
            
            setContentPane(builder.getPanel());
            pack();
        }

        public void setChecked(int index) {
            if (index < 0 || index >= cBoxes.length)
                return;
            cBoxes[index].setSelected(true);
            fb.setChecked(index);
        }
    }
    
    // The JTable filled with processes.
    private class ProcessTable extends JTable {
        private ProcModel             pm;
        private ClickableHeader       ch;
        private LineRenderer          lineR;
        private ListSelectionModel    rowSM;
        private ListSelectionListener listSelectionListener;
        
        public void erase() {
            if (pm != null)
                pm.erase();
            pm = null;
            if (ch != null)
                ch.erase();
            ch = null;
            setTableHeader(null);
            setModel(new DefaultTableModel());
            lineR = null;
            setDefaultRenderer(Number.class, null);
            setDefaultRenderer(String.class, null);
            rowSM.removeListSelectionListener(listSelectionListener);
            rowSM = null;
            listSelectionListener = null;
        }

        // This class will highlight a row that has been selected by the user.
        private class LineRenderer extends JLabel
                implements TableCellRenderer, Serializable {
            /**
             * The constructor.
             */
            public LineRenderer() {
                setOpaque(true);
            }
            
            /**
             * Get the cell after modification.
             * @param table The JTable.
             * @param cell The cell being rendered.
             * @param isSelected Whether this cell has been selected.
             * @param hasFocus Whether this cell has focus.
             * @param row The row number.
             * @param column The column number.
             * @return The component to be rendered.
             */
            public Component getTableCellRendererComponent(
                    JTable table, Object cell,
                    boolean isSelected, boolean hasFocus,
                    int row, int column) {
                Number pid = (Number) pm.getValueAt(row, 0);
                
                if (pid == null)
                    return this; // gak
                
                if (TpSelectedPids.contains(pid.longValue()))
                    setBackground(getSelectionBackground());
                else if (TpOffensivePid != -1 && pid.longValue() == TpOffensivePid)
                    setBackground(TpOffensiveColor);
                else
                    setBackground(ProcessTable.this.getBackground());

                // black on black is hard to read
                if (getBackground().equals(Color.BLACK))
                    setForeground(Color.WHITE);

                setFont(table.getFont());
                setText(cell.toString());

                return this;
            }
        }
        
        /**
         * The constructor.
         * @param host The host whose processes are to be monitored.
         * @throws NotBoundException Service is not bound on server machine.
         * @throws MalformedURLException The service signature is malformed.
         * @throws RemoteException RMI exception
         * @throws PerformanceException Any possible lower level exception.
         */
        public ProcessTable(String host, boolean setProperties) throws
                RemoteException, NotBoundException,
                MalformedURLException, PerformanceException {
            super(new ProcModel(host, setProperties));
            pm = (ProcModel) getModel();
            
            // get the column model
            TpColumnModel = getColumnModel();
            
            // make a new JTableHeader with this column model
            ch = new ClickableHeader(TpColumnModel);
            
            resizeStringCells(pm);
            
            // set the column height
            Dimension d = ch.getPreferredSize();
            d.height = ClickableHeader.COLUMN_HEADER_HEIGHT;
            ch.setPreferredSize(d);
            
            // set this table header
            setTableHeader(ch);
            
            // only allow one row selected at once
            setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
            
            // Ask to be notified of selection changes.
            rowSM = getSelectionModel();
            listSelectionListener = new ListSelectionListener() {
                public void valueChanged(ListSelectionEvent e) {
                    // Ignore extra messages.
                    if (e.getValueIsAdjusting())
                        return;
                    
                    ListSelectionModel lsm =
                            (ListSelectionModel) e.getSource();
                    if (! lsm.isSelectionEmpty()) {
                        int selectedRow = lsm.getMinSelectionIndex();
                        Number pid = (Number) pm.getValueAt(selectedRow, 0);
                        
                        // if the pid is in the set, remove it; if it's not, add it.
                        long value = pid.longValue();

                        if (TpSelectedPids.contains(value))
                            TpSelectedPids.remove(value);
                        else
                            TpSelectedPids.add(value);
                        pm.fireTableDataChanged();
                    }
                }
            };
            rowSM.addListSelectionListener(listSelectionListener);
            
            // set a custom renderer for the 2 types in the table
            lineR = new LineRenderer();
            setDefaultRenderer(Number.class, lineR);
            setDefaultRenderer(String.class, lineR);
        }
        
        /**
         * Refresh the process data, update the JTable and cause a repaint.
         */
        public void refresh() throws
                RemoteException, NotBoundException,
                MalformedURLException, PerformanceException {
            pm.refresh();
            pm.redraw();
            pm.fireTableDataChanged();
        }
        
        public ProcModel getProcModel() {
            return pm;
        }
    }
    
    //
    // Public constructors
    //
    
    /**
     * Constructor for the top processes object.
     * @param parent The object that this object will be subordinate to.
     * @param remoteHost The name of the host to monitor.
     * @param msInterval The interval between updates.
     * @param args Optional list of key=value strings for intializing process list.
     * <p>Possible key=value pairs are:
     * <ul>
     *   <li>procCount=
     *     <ul>
     *       <li>An integer value for how many processes to display.</li>
     *     </ul>
     *   </li>
     * </ul>
     * <p>Current there are no key=value pairs defined for this class.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public XETopProcesses(Container parent, String remoteHost,
            int msInterval, Object ... args) throws
            RemoteException, NotBoundException,
            MalformedURLException, PerformanceException {

        super(msInterval, null);

        TpParent = parent;
        TpInterval = msInterval;
        TpProps = null;

        boolean initializeWidths = false;

        if (args.length > 0) {
            if (args[0] instanceof XETopProcProperties) {
                TpProps = (XETopProcProperties) args[0];
                TpVisibleProcs = TpProps.visibleProcs;
                TpSelectedPids = TpProps.selectedPids;
                if ((TpInterval / 1000) != TpProps.intervalSeconds) {
                    TpInterval = TpProps.intervalSeconds * 1000;
                    super.setDelay(TpInterval);
                }
                initializeWidths = true;
            } else {
                XEArgsParser ap = new XEArgsParser(args);
            
                for(Map.Entry<String, String> e : ap.entrySet()) {
                    String key = e.getKey();
                    String value = e.getValue();
                
                    if (key.equals("procCount"))
                        TpVisibleProcs = Integer.parseInt(value);
                }
            }
        }

        addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                try {
                    TpProcesses.refresh();
                } catch(Exception e) {
                    failureEvent(new CmExceptionParser(e).getMessage());
                }
            }
        });
        
        TpProcesses = new ProcessTable(remoteHost, TpProps != null);
        TpProcesses.setShowGrid(false);
        TpProcesses.getColumnModel().setColumnMargin(10);
        JScrollPane jsp = new JScrollPane(TpProcesses);

        if (TpProps == null)
            TpProps = new XETopProcProperties();
        if (TpSelectedPids == null)
            TpSelectedPids = new TreeSet<Long>();

        TpPane = jsp;

        if (initializeWidths)
            TpProcesses.getProcModel().setWidths();
    }

    protected void finalize() {
        if (discussFinalization)
            System.out.println("XETopProcesses.finalize");
    }
}
