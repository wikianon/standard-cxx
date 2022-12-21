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
import com.capmet.metrics.cm.CmMpStat;
import com.capmet.metrics.cm.CmMpMetrics;

/**
 * An application for monitoring the CPUs on a host.<p>
 *
 * Use: java MpStat [-n cpu] [-h remote-host] [interval [iterations]]
 *      -n cpu - show only named CPU<br>
 *      -h remote-host - monitor remote host
 */
public class MpStat
{
    /**
     * The main method of the MpStat application.
     * @param args Arguments passed from the environment
     */
    public static void main(String[] args)
    {
        new MpStat(args);
    }

    // usage blurb
    private void usage()
    {
        System.out.println(XEMessages.msgs.getString("MpStat_Usage")); 
    }

    /**
     * MpStat constructor.
     * @param args Arguments passed from the environment
     */
    public MpStat(String[] args)
    {
        try {
            String namedCpu = null;
            String remoteHost = null;
            int iterations = 1;
            int interval = 1;
            GetOpt opts = new GetOpt(args, "n:h:");
            int c;

            opts.optErr = false;
            while((c = opts.getopt()) != opts.optEOF) {
                switch(c) {
                case 'n':
                    namedCpu = opts.getOptArg();
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

            CmMpStat mpstat = new CmMpStat(remoteHost);

            do {
                System.out.println(XEMessages.msgs.getString("MpStat_Header"));

                for(Map.Entry<String, CmMpMetrics> e : mpstat.entrySet())
                {
                    String name = e.getKey();
                    CmMpMetrics mp = e.getValue();

                    if (namedCpu != null && !namedCpu.equals(name))
                        return;

                    System.out.printf("%-6.6s %4d    %4d  %4d  %4d\n",
                        name,
                        mp.userTimePercent.intValue(),
                        mp.systemTimePercent.intValue(),
                        mp.waitTimePercent.intValue(),
                        mp.idleTimePercent.intValue());
                }

                if (--iterations > 0) {
                    Thread.sleep(interval * 1000);
                    mpstat.refresh();
                }
            } while(iterations > 0);
        } catch(Exception e) {
            e.printStackTrace();
        }
    }
}
