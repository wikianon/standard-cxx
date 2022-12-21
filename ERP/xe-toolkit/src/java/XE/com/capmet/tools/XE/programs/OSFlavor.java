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

import com.capmet.tools.XE.GetOpt;
import com.capmet.tools.XE.XEMessages;
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.cm.CmExceptionParser;

/**
 * An application for displaying the operating system "flavor".<p>
 *
 * Use: java OSFlavor [-h remote-host]
 */
public class OSFlavor
{
    // usage blurb
    private static void usage()
    {
        System.out.println(XEMessages.msgs.getString("OSFlavor_Usage")); 
    }

    /**
     * The main method of the OSFlavor application.
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

            PerformanceHost host = new PerformanceHost(remoteHost);

            System.out.println(host.getOSFlavor());
        } catch(Exception e) {
            System.err.println(new CmExceptionParser(e).getMessage());
        }
    }
}
