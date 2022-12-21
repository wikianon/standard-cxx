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

import java.awt.Color;
import java.net.InetAddress;
import java.io.File;
import java.io.IOException;
import java.util.Date;
import java.util.Calendar;
import java.util.logging.Logger;
import java.util.logging.Handler;
import java.util.logging.FileHandler;
import java.util.logging.Formatter;
import java.util.logging.LogRecord;
import java.text.MessageFormat;
import com.capmet.tools.XE.GetOpt;
import com.capmet.tools.XE.XEDialogs;
import com.capmet.tools.XE.XEMessages;
import com.capmet.tools.XE.rules.*;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.cm.CmProcesses;
import com.capmet.metrics.cm.CmProcessMetrics;
import com.capmet.metrics.rmi.CaptiveCollectorStats;
import com.capmet.metrics.rmi.CaptiveConnection;
import com.capmet.metrics.rmi.RMIAccessPoint;
import com.capmet.metrics.rmi.RMIMethods;
import D.PN;

public class VirtualGuru {
    // default interval is 60 seconds
    private int VgInterval = 60000;

    public static void main(String[] args) {
        new VirtualGuru(args);
    }

    // usage blurb
    private void usage() {
        System.out.println(XEMessages.msgs.getString("VirtualGuru_Usage"));
    }

    public VirtualGuru(String[] args) {
        try {
            String remoteHost = null;
            int iterations = 1;
            int interval = 1;
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

            int optind = opts.getOptIndex();
            int argCount = args.length - optind;

            switch(argCount) {
            case 0:
                break;
            case 1:
                try {
                    VgInterval = Integer.parseInt(args[optind]) * 1000;
                } catch(NumberFormatException e) {
                    usage();
                    return;
                }
                break;
            default:
                usage();
                return;
            }

            if (remoteHost == null) {
                remoteHost = InetAddress.getLocalHost().getHostName();
                int dot = remoteHost.indexOf(".");
                if (dot != -1)
                    remoteHost = remoteHost.substring(0, dot);
            }

            String top = System.getProperty("com.capmet.CaptiveCollector.Top");
            if (top == null || top.length() == 0)
                XEDialogs.fatalDialog(XEMessages.msgs.getString("No_Top"));

            // for each host in the comma-separated string
            for(String host : remoteHost.split(",")) {
                MonitoredHost mh = new MonitoredHost(top, host);

                // light up a new thread to monitor it
                mh.start();
            }

        } catch(Exception e) {
            System.err.println((new CmExceptionParser(e)).getMessage());
        }
    }

    private class MonitoredHost extends Thread {
        private String       top;
        private String       hostName;
        private Rule[]       ruleSet;
        private Handler      handler = null;
        private Logger       log;
        private Calendar     cal = Calendar.getInstance();
        private int          dayOfTheYear;
        private RMIMethods   accessInterface;
        private CmProcesses  procs;
        private FieldPair[] fieldPairs = {
            new FieldPair("PID",       D.PN.PN_PROCESS_ID),
            new FieldPair("PPID",      D.PN.PN_PARENT_PROCESS_ID),
            new FieldPair("USERNAME",  D.PN.PN_USER_NAME),
            new FieldPair("SIZE",      D.PN.PN_TOTAL_SIZE_KB),
            new FieldPair("RSS",       D.PN.PN_RESIDENT_SIZE_KB),
            new FieldPair("SZD",       D.PN.PN_TOTAL_SIZE_DELTA_KB),
            new FieldPair("RSSD",      D.PN.PN_RESIDENT_SIZE_DELTA_KB),
            new FieldPair("STATE",     D.PN.PN_PROCESS_STATE),
            new FieldPair("TIME",      D.PN.PN_TOTAL_CPU_SECONDS),
            new FieldPair("USER_TIME", D.PN.PN_USER_CPU_SECONDS),
            new FieldPair("SYS_TIME",  D.PN.PN_KERNEL_CPU_SECONDS),
            new FieldPair("USER_PCT",  D.PN.PN_PERCENT_USER_TIME),
            new FieldPair("SYS_PCT",   D.PN.PN_PERCENT_KERNEL_TIME),
            new FieldPair("IO_sec",    D.PN.PN_TOTAL_IO_KB),
            new FieldPair("MEM_PCT",   D.PN.PN_PERCENT_PHYSMEM_USE),
            new FieldPair("CPU_PCT",   D.PN.PN_PERCENT_CPU_USE),
            new FieldPair("PG_FAULTS", D.PN.PN_PAGE_FAULTS),
            new FieldPair("SYS_CALLS", D.PN.PN_SYSTEM_CALLS),
            new FieldPair("CSWITCHES", D.PN.PN_CONTEXT_SWITCHES),
            new FieldPair("PRIO",      D.PN.PN_GLOBAL_PRIORITY),
            new FieldPair("THREADS",   D.PN.PN_NUMBER_OF_THREADS),
            new FieldPair("COMMAND",   D.PN.PN_COMMAND_NAME),
            new FieldPair("FULL_CMD",  D.PN.PN_COMMAND_LINE)
        };

        private void rotateLogFile(String top, String hostName, Date date) {
            String fname = "VirtualGuru";
            String logFile =
                top + File.separatorChar + "logs" + File.separatorChar +
                fname + File.separatorChar + hostName; 
            String dt =
                MessageFormat.format("{0,date,MM-dd-yyyy}::{0,time,HH:mm:ss}",
                    date);
            String oldLogFile =
                top + File.separatorChar + "logs" + File.separatorChar +
                fname + File.separator + "old" + File.separatorChar +
                hostName + ".thru-" + dt;
            
            log = Logger.getAnonymousLogger();
            log.setUseParentHandlers(false);

            // 86 the old handler
            if (handler != null) {
                for(Handler h : log.getHandlers()) {
                    if (h == handler) {
                        log.removeHandler(h);
                        break;
                    }
                }
            }

            try { 
                File newFile = new File(logFile); 
                File renamedFile = new File(oldLogFile);
        
                newFile.renameTo(renamedFile);
                handler = new FileHandler(logFile, false);
            } catch(IOException e) {
                throw new RuntimeException(e.getMessage());
            }

            handler.setFormatter(new Formatter() {
                public synchronized String format(LogRecord record) {
                    return formatMessage(record) + '\n';
                }
            });

            log.addHandler(handler);
        }

        public MonitoredHost(String top, String hostName) throws Exception {
            this.top = top;
            this.hostName = hostName;

            RMIAccessPoint accessPoint;
            CaptiveCollectorStats stats = null;

            accessPoint = new RMIAccessPoint(new CaptiveConnection(hostName));
            accessInterface = (RMIMethods) accessPoint.getInterface();
            stats = accessInterface.CCStatus();
            cal.setTime(stats.currentTime);
            dayOfTheYear = cal.get(Calendar.DAY_OF_YEAR);

            rotateLogFile(top, hostName, stats.currentTime);

            ruleSet = new Rule[] {
                new LiveCPURule(hostName),
                new LiveDiskRule(hostName),
                new LiveMemoryRule(hostName),
                new LiveNICRule(hostName),
                new LivePageFileRule(hostName),
                new LiveCacheRule(hostName),
                new LiveTCPRule(hostName)
            };

            procs = new CmProcesses(hostName);
        }

        private int cascade(int[] flags) {
            for(int i=0; i<flags.length; i++) {
                if (procs.isSupported(flags[i])) {
                    procs.rsort(flags[i]);
                    return flags[i];
                }
            }

            procs.rsort(PN.PN_PROCESS_ID);
            return PN.PN_PROCESS_ID;
        }

        private String getFieldName(int index) {
            for(FieldPair fp : fieldPairs)
                if (fp.index == index)
                    return fp.name;

            return (new Integer(index)).toString();
        }

        public void run() {
            int[] csTries = {
                PN.PN_CONTEXT_SWITCHES,
                PN.PN_SYSTEM_CALLS,
                PN.PN_PERCENT_KERNEL_TIME,
                PN.PN_PERCENT_CPU_USE
            };
            int[] scTries = {
                PN.PN_SYSTEM_CALLS,
                PN.PN_PERCENT_KERNEL_TIME,
                PN.PN_PERCENT_CPU_USE
            };
            int[] ktTries = {
                PN.PN_PERCENT_KERNEL_TIME,
                PN.PN_PERCENT_CPU_USE
            };
            int[] utTries = {
                PN.PN_PERCENT_USER_TIME,
                PN.PN_PERCENT_CPU_USE
            };
            int[] ioTries = {
                PN.PN_TOTAL_IO_KB,
                PN.PN_SYSTEM_CALLS,
                PN.PN_PERCENT_KERNEL_TIME,
                PN.PN_PERCENT_CPU_USE
            };
            int[] muxTries = {
                PN.PN_NUMBER_OF_THREADS,
                PN.PN_PERCENT_KERNEL_TIME,
                PN.PN_PERCENT_CPU_USE
            };
            int[] pagingTries = {
                PN.PN_RESIDENT_SIZE_KB,
                PN.PN_RESIDENT_SIZE_DELTA_KB
            };
            CaptiveCollectorStats stats = null;
            Color state = Color.PINK;
            Rule peakRule = null;
            int sortKey = PN.PN_PERCENT_CPU_USE;
            CmProcessMetrics topProc;

            try {
                for(;;) {
                    // start at a state less than white
                    state = Color.PINK;

                    Thread.sleep(VgInterval);
                    stats = accessInterface.CCStatus();
                    cal.setTime(stats.currentTime);
                    int today = cal.get(Calendar.DAY_OF_YEAR);
                    if (today != dayOfTheYear) {
                        rotateLogFile(top, hostName, stats.currentTime);
                        dayOfTheYear = today;
                    }

                    // run through, fire the rules and find the peak rule
                    for(Rule rule : ruleSet) {
                        if (!rule.isRuleSupported())
                            continue;

                        rule.fireLiveRule();

                        if (Rule.isWorseThan(rule.getState(), state)) {
                            state = rule.getState();
                            peakRule = rule;
                        }
                    }

                    log.info("timestamp " +
                        MessageFormat.format(
                            "{0,date} {0,time,hh:mm:ss a z}", stats.currentTime));

                    log.info("    state " + Rule.stateString(state) + " " +
                                            Rule.stateExplain(state));
                    log.info("    action " + peakRule.getAction());

                    if (Rule.isWorseThan(state, Color.GREEN)) {
                        int intValue = 0;
                        double doubleValue = 0.0;
                        boolean useIntValue = true;
                        Number offValue = null;

                        procs.refresh();

                        switch(peakRule.getProblemCode()) {
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
                            sortKey = PN.PN_TOTAL_SIZE_KB;
                            cascade(new int[] { PN.PN_TOTAL_SIZE_KB });
                            break;
                        case Rule.NO_PROBLEM_DETECTED:
                            sortKey = PN.PN_PERCENT_CPU_USE;
                            cascade(new int[] { PN.PN_PERCENT_CPU_USE });
                            break;
                        }

                        topProc = procs.sortedValues().firstElement();

                        String command = topProc.commandLine.toString();
                        if (command.length() == 0)
                            command = topProc.commandName.toString();

                        switch(sortKey) {
                        case PN.PN_CONTEXT_SWITCHES:
                            intValue = topProc.contextSwitches.intValue();
                            break;
                        case PN.PN_PERCENT_CPU_USE:
                            doubleValue = topProc.percentCPUUse.doubleValue();
                            useIntValue = false;
                            break;
                        case PN.PN_PERCENT_KERNEL_TIME:
                            doubleValue = topProc.percentKernelTime.doubleValue();
                            useIntValue = false;
                            break;
                        case PN.PN_PERCENT_USER_TIME:
                            doubleValue = topProc.percentUserTime.doubleValue();
                            useIntValue = false;
                            break;
                        case PN.PN_PROCESS_ID:
                            intValue = topProc.processID.intValue();
                            break;
                        case PN.PN_RESIDENT_SIZE_DELTA_KB:
                            intValue = topProc.residentSizeDeltaKB.intValue();
                            break;
                        case PN.PN_SYSTEM_CALLS:
                            intValue = topProc.systemCalls.intValue();
                            break;
                        case PN.PN_TOTAL_IO_KB:
                            intValue = topProc.totalIOKB.intValue();
                            break;
                        case PN.PN_TOTAL_SIZE_KB:
                            intValue = topProc.totalSizeKB.intValue();
                            break;
                        default:
                            intValue = -sortKey;
                            break;
                        }

                        if (useIntValue)
                            offValue = new Integer(intValue);
                        else
                            offValue = new Double(doubleValue);

                        String fieldName = getFieldName(sortKey);

                        log.info("    trigger " + peakRule.getTriggerRuleName());
                        log.info("    top-process " + command);
                        log.info("    sort-key " + fieldName);
                        log.info("    key-field-value " + offValue);
                    }

                    // run through again and dump the rules
                    for(Rule rule : ruleSet) {
                        if (!rule.isRuleSupported())
                            continue;

                        dumpRule(rule, 1, false);
                    }

                    log.info("end");
                }
            } catch(Exception e) {
                CmExceptionParser ep = new CmExceptionParser(e);

                log.severe(ep.getMessage());
                return;
            }
        }

        private void dumpRule(Rule rule, int indent, boolean isSubRule) {
            Color state = rule.getState();
            String one = "    ";
            String two = "        ";
            String three = "            ";
            String firstTab = null;
            String secondTab = null;

            switch(indent) {
            case 1:
                firstTab = one;
                secondTab = two;
                break;
            case 2:
                firstTab = two;
                secondTab = three;
                break;
            default:
                break;
            }

            String ruleType = (isSubRule ? "sub-rule " : "rule ");

            log.info(firstTab + ruleType + rule.getRuleName());
            log.info(secondTab + "state " + Rule.stateString(state) + " " +
                                        Rule.stateExplain(state));
            log.info(secondTab + "action " + rule.getAction());
            log.info(secondTab + "explanation " + rule.getExplanation());
            log.info(secondTab + "furtherDetail " + rule.getFurtherDetail());
            log.info(secondTab + "solution " + rule.getSolution());

            if (rule.isAggregateRule()) {
                AggregateRule agg = (AggregateRule) rule;

                for(Object objr : agg.getSubRules()) {
                    Rule subrule = (Rule) objr;

                    if (!subrule.isRuleSupported())
                        continue;

                    dumpRule(subrule, indent + 1, true);
                }
            }

            log.info(firstTab + "end");
        }

        private class FieldPair {
            public String name;
            public int    index;

            public FieldPair(String n, int i) {
                name = XEMessages.msgs.getString(n);
                index = i;
            }
        }
    }
}
