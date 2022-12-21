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

import com.capmet.metrics.cm.CmSysInfo;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.tools.XE.GetOpt;
import com.capmet.tools.XE.XEMessages;

/**
 * An application for displaying information about a host.<p>
 *
 * Use: java SysInfo [-h remote-host]
 */
public class SysInfo
{
    // usage blurb
    private static void usage()
    {
        System.out.println(XEMessages.msgs.getString("SysInfo_Usage")); 
    }

    private static String uptime(int total)
    {
        return total + ": (" + CmSysInfo.formatUpTime(total) + ")";
    }

    /**
     * The main method of the SysInfo application.
     * @param args Arguments passed from the environment
     */
    public static void main(String[] args)
    {
        try {
            String remoteHost = null;
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

            CmSysInfo si = new CmSysInfo(remoteHost);
            String[] tags = new String[] {
                XEMessages.msgs.getString("PF_Free"),
                XEMessages.msgs.getString("Mem_Free"),
                XEMessages.msgs.getString("PF_Size"),
                XEMessages.msgs.getString("Mem_Size"),
                XEMessages.msgs.getString("Clock_Tick"),
                XEMessages.msgs.getString("OS_Platform"),
                XEMessages.msgs.getString("OS_Version"),
                XEMessages.msgs.getString("Page_Size"),
                XEMessages.msgs.getString("CPU_Architect"),
                XEMessages.msgs.getString("CPU_ModelName"),
                XEMessages.msgs.getString("CPUs_Conf"),
                XEMessages.msgs.getString("CPUs_Online"),
                XEMessages.msgs.getString("CPU_Clock"),
                XEMessages.msgs.getString("Uptime")
            };
            Object[] values = new Object[] {
                si.pagingFileFreePercent,
                si.memoryFreePercent,
                si.pagingFileTotalMB,
                si.memoryTotalMB,
                si.clockTicksPerSecond,
                si.osPlatform,
                si.osVersion,
                si.pageSize,
                si.cpuArchitecture,
                si.cpuModel,
                si.cpuCountConfigured,
                si.cpuCountOnline,
                si.cpuClockSpeed,
                uptime(si.systemUpTime.intValue())
            };
            int length = 0;

            // find the lhs with the greatest length
            for(String s : tags) {
                int l = s.length();

                if (l > length)
                    length = l;
            }

            // construct a format string (%-X.Xs = %s\n)
            String format = "%-" + length + "." + length + "s = %s\n";

            for(int i=0; i<tags.length; i++)
                System.out.printf(format, tags[i], values[i].toString());
        } catch(Exception e) {
            System.err.println(new CmExceptionParser(e).getMessage());
        }
    }
}
