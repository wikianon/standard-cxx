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

import java.io.InputStreamReader;
import java.io.BufferedReader;
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.PerformanceCategory;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.tools.XE.GetOpt;
import com.capmet.tools.XE.XEMessages;

/**
 * An application for retrieving counter explanations for a host.<p>
 *
 * Use: java Explain { [-l(ist) host] | host category field }
 */
public class Explain
{
    // usage blurb
    private static void usage()
    {
        System.err.println(XEMessages.msgs.getString("Explain_Usage"));
        System.exit(1);
    }

    /**
     * The main method of the Explain application.
     * @param args Arguments passed from the environment
     */
    public static void main(String[] args)
    {
        try
        {
            GetOpt opts = new GetOpt(args, "l:");
            String hostName = null;
            int c;

            opts.optErr = false;
            while((c = opts.getopt()) != opts.optEOF) {
                switch(c) {
                case 'l':
                    hostName = opts.getOptArg();
                    break;
                default:
                    usage();
                    break;
                }
            }

            if (hostName != null)
            {
                // list option
                PerformanceHost host = new PerformanceHost(hostName);

                if (host.getOSFlavor().isWindowsHost())
                {
                    System.out.println(hostName + " " + XEMessages.msgs.getString("Explain_W1"));
                    System.out.println(XEMessages.msgs.getString("Explain_W2")); 
                    System.out.print(XEMessages.msgs.getString("Explain_W3"));
                    InputStreamReader ir = new InputStreamReader(System.in);
                    BufferedReader br = new BufferedReader(ir);
                    String ans = br.readLine();
                    if (ans == null || ans.length() == 0)
                        System.exit(0);
                    char c0 = ans.charAt(0);
                    if (c0 == 'n' || c0 == 'N')
                        System.exit(0);
                }

                String[] categories = host.getCategoryNames();

                for(String catName : categories)
                {
                    System.out.println(catName);
                    PerformanceCategory cat = host.getCategory(catName);
                    String[] fields = cat.getSupportedFieldNames();
                    for(String fieldName : fields)
                        System.out.println("\t" + fieldName);
                }
                System.exit(0);
            }

            int optind = opts.getOptIndex();
            int argCount = args.length - optind;
            String categoryName = null;
            String fieldName = null;

            if (argCount != 3)
                usage();

            hostName = args[optind++];
            categoryName = args[optind++];
            fieldName = args[optind];

            PerformanceHost host = new PerformanceHost(hostName);
            PerformanceCategory cat = host.getCategory(categoryName);
            int fieldNumber = cat.getFieldNumber(fieldName);
            String explanation = cat.getFieldExplanation(fieldNumber);

            System.out.println(explanation);
        }
        catch(Exception e)
        {
            System.err.println(new CmExceptionParser(e).getMessage());
        }
    }
}
