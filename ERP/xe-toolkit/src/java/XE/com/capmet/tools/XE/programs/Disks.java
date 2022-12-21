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
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.rmi.PerformanceHost;

/**
 * An application for viewing the I/O entities known by the collector.<p>
 *
 * Use: java Disks [-h remote-host]
 */
public class Disks
{
    /**
     * The main method of the Disks application.
     * @param args Arguments passed from the environment
     */
    public static void main(String[] args)
    {
        new Disks(args);
    }

    // usage blurb
    private void usage()
    {
        System.out.println(XEMessages.msgs.getString("Disks_Usage")); 
    }

    /**
     * Disks constructor.
     * @param args Arguments passed from the environment
     */
    public Disks(String[] args)
    {
        try {
            String namedCache = null;
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

            CmIoStat io = new CmIoStat(remoteHost);

            System.out.printf("%10.10s %10.10s %10.10s\n", "Name", "Type", "Capacity");

            for(Map.Entry<String, CmIoMetrics> e : io.entrySet()) {
                String name = e.getKey();
                CmIoMetrics im = e.getValue();

                if (im.capacityMB.intValue() == 0)
                    continue;

                System.out.printf("%10.10s %10.10s %10d\n",
                                    name, im.diskType, im.capacityMB.intValue());
            }
        } catch(Exception e) {
            System.err.println(new CmExceptionParser(e).getMessage());
        }
    }
}
