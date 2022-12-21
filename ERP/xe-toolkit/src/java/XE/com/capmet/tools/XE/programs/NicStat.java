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

import java.util.Map;
import com.capmet.tools.XE.GetOpt;
import com.capmet.tools.XE.XEMessages;
import com.capmet.metrics.cm.CmNicStat;
import com.capmet.metrics.cm.CmNicMetrics;
import com.capmet.metrics.cm.CmExceptionParser;

/**
 * An application for monitoring the network interfaces on a host.<p>
 *
 * Use: java NicStat [-h remote-host] [interval [iterations]]
 */
public class NicStat
{
    private final int HEADER_INTERVAL = 20;
    private int lineCount = 0;

    /**
     * The main method of the NicStat application.
     * @param args Arguments passed from the environment
     */
    public static void main(String[] args)
    {
        new NicStat(args);
    }


    // usage blurb
    private void usage()
    {
        System.out.println(XEMessages.msgs.getString("NicStat_Usage"));
    }

    /**
     * NicStat constructor.
     * @param args Arguments passed from the environment
     */
    public NicStat(String[] args)
    {
        try {
            boolean showLoopback = false;
            String namedNic = null;
            String remoteHost = null;
            int iterations = 1;
            int interval = 1;
            GetOpt opts = new GetOpt(args, "ln:h:");
            int c;

            opts.optErr = false;
            while((c = opts.getopt()) != opts.optEOF) {
                switch(c) {
                case 'l':
                    showLoopback = true;
                    break;
                case 'n':
                    namedNic = opts.getOptArg();
                    break;
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
                    interval = Integer.parseInt(args[optind]);
                    iterations = 0;
                } catch(NumberFormatException e) {
                    usage();
                    return;
                }
                break;
            case 2:
                try {
                    interval = Integer.parseInt(args[optind++]);
                    iterations = Integer.parseInt(args[optind]);
                } catch(NumberFormatException e) {
                    usage();
                    return;
                }
                break;
            default:
                usage();
                return;
            }

            if (interval == 0)
                interval = 1;
            if (iterations == 0)
                iterations = Integer.MAX_VALUE;

            CmNicStat nic = new CmNicStat(remoteHost);
            boolean firstPass = true;

            do {
                if (lineCount == 0)
                {
                    System.out.println(XEMessages.msgs.getString("NicStat_Header1"));
                    System.out.println(XEMessages.msgs.getString("NicStat_Header2"));
                    lineCount = HEADER_INTERVAL;
                }

                if (firstPass)
                {
                    Thread.sleep(interval * 1000);
                    nic.refresh();
                    firstPass = false;
                    continue;
                }

                for(Map.Entry<String, CmNicMetrics> e : nic.entrySet())
                {
                    String name = e.getKey();
                    CmNicMetrics nm = e.getValue();
                    boolean printit = false;

                    // if a disk was named and this is the one
                    if (namedNic != null) {
                        if (namedNic.equals(name))
                            printit = true;
                    } else {
                        if (nm.isLoopback.booleanValue()) {
                            if (showLoopback)
                                printit = true;
                        } else
                            printit = true;
                    }

                    if (printit)
                    {
                        System.out.printf(
                            "%-6.6s %6d %6d %10d %10d %4d %4d %4d %4d %-4d %9.2f%%\n",
                            name,
                            nm.packetsReceived.intValue(), nm.packetsTransmitted.intValue(),
                            nm.bytesReceived.intValue(), nm.bytesTransmitted.intValue(),
                            nm.inputErrors.intValue(), nm.outputErrors.intValue(),
                            nm.inputFailures.intValue(), nm.outputFailures.intValue(),
                            nm.interfaceSpeed.intValue() / 1000000,
                            nm.utilizationPercent.doubleValue());

                        lineCount--;
                    }
                }

                if (--iterations > 0) {
                    Thread.sleep(interval * 1000);
                    nic.refresh();
                }
            } while(iterations > 0);
        } catch(Exception e) {
            System.err.println(new CmExceptionParser(e).getMessage());
        }
    }
}
