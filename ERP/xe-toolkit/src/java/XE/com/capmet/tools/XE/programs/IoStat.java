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
import com.capmet.metrics.cm.CmIoStat;
import com.capmet.metrics.cm.CmIoMetrics;
import com.capmet.metrics.cm.CmMounts;

/**
 * An application for monitoring the I/O entities on a host.<p>
 *
 * Use: java IoStat [-dN] [-n disk] [-h remote-host] [interval [iterations]]<br>
 *      -d - show physical disks only<br>
 *      -N - show network disks<br>
 *      -n disk - show only named disk<br>
 *      -h remote-host - monitor remote host
 */
public class IoStat
{
    private final int HEADER_INTERVAL = 20;
    private int lineCount = 0;

    private CmMounts diskMap;

    /**
     * The main method of the IoStat application.
     * @param args Arguments passed from the environment
     */
    public static void main(String[] args)
    {
        new IoStat(args);
    }

    // usage blurb
    private void usage()
    {
        System.out.println(XEMessages.msgs.getString("IoStat_Usage"));
    }

    /**
     * IoStat constructor.
     * @param args Arguments passed from the environment
     */
    public IoStat(String[] args)
    {
        try {
            boolean showDisks = false;
            boolean showNetDisks = false;
            String namedDisk = null;
            String remoteHost = null;
            int iterations = 1;
            int interval = 1;
            GetOpt opts = new GetOpt(args, "dn:Nh:");
            int c;

            opts.optErr = false;
            while((c = opts.getopt()) != opts.optEOF) {
                switch(c) {
                case 'd':
                    showDisks = true;
                    break;
                case 'n':
                    namedDisk = opts.getOptArg();
                    break;
                case 'N':
                    showNetDisks = true;
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

            CmIoStat io = new CmIoStat(remoteHost);
            diskMap = new CmMounts(remoteHost);

            do {
                if (lineCount == 0) {
                    System.out.println(
                        "device           %util   qlen (rio:kB rd  wio:kB wr" +
                        "  ios:kB /sec) svctim (ms)");
                    lineCount = HEADER_INTERVAL;
                }

                for(Map.Entry<String, CmIoMetrics> e : io.entrySet())
                {
                    String name = e.getKey();
                    CmIoMetrics iom = e.getValue();

                    boolean printit = false;
                    boolean mapit = iom.diskType.equals("logical");

                    if (mapit)
                        name = diskMap.toMountPoint(name);

                    // if a disk was named and this is the one
                    if (namedDisk != null) {
                        if (namedDisk.equals(name))
                            printit = true;
                    } else {
                        // print those requested
                        // skip the _Total disk
                        if (name.equals("_Total"))
                            printit = false;
                        else if (iom.diskType.equals("logical"))
                            printit = true;
                        else if (iom.diskType.equals("physical"))
                        {
                            printit = showDisks;
                            int space = name.indexOf(' ');
                            if (space != -1)
                                name = name.substring(0, space);
                        }
                        else if (iom.diskType.equals("network"))
                            printit = showNetDisks;
                    }

                    if (printit) {
                        System.out.printf(
                            "%-14.14s %6.2f%% %6.2f  %3d:%-6d %3d:%-6d %3d:%-6d %8.1f\n",
                                name, iom.utilizationPercent.doubleValue(),
                                iom.averageQueueLength.doubleValue(),
                                iom.readOperations.intValue(), iom.kilobytesRead.intValue(),
                                iom.writeOperations.intValue(), iom.kilobytesWritten.intValue(),
                                iom.readOperations.intValue() + iom.writeOperations.intValue(),
                                iom.kilobytesRead.intValue() + iom.kilobytesWritten.intValue(),
                                iom.averageAccessTime.doubleValue());
                        lineCount--;
                    }
                }

                if (--iterations > 0) {
                    Thread.sleep(interval * 1000);
                    io.refresh();
                }
            } while(iterations > 0);
        } catch(Exception e) {
            e.printStackTrace();
        }
    }
}
