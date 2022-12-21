/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector application of Captivity.
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

package com.capmet.Captivity.CaptiveCollector;

import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import java.util.Date;
import java.util.Formatter;
import java.util.ResourceBundle;
import java.text.MessageFormat;
import com.capmet.metrics.cm.CmSysInfo;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.rmi.CaptiveConnection;
import com.capmet.metrics.rmi.RMIAccessPoint;
import com.capmet.metrics.rmi.RMIMethods;
import com.capmet.metrics.rmi.CaptiveCollectorStats;

/**
 * The status printer class for checking up on CaptiveCollector.
 */
public class CCStatus extends RMIAccessPoint
{
    private CaptiveCollectorStats ccStats;
    private RMIMethods            remote;

    // make these public so CCControl can use them...

    /**
     * The messages catalog.
     */
    public static ResourceBundle  msgs;
    /**
     * The tag strings that describe the status data.
     */
    public static String[]        tags;
    /**
     * Nicely formatted status data with indexes that match <CODE>tags</CODE>.
     */
    public Object[]               values;

    static {
        msgs = ResourceBundle.getBundle(
            "com.capmet.Captivity.CaptiveCollector.MessagesBundle");
        tags = new String[] {
            msgs.getString("Version"),
            msgs.getString("Start_Time"),
            msgs.getString("Current_Time"),
            msgs.getString("Run_Time"),
            msgs.getString("RMC_Count"),
            msgs.getString("Objs_Cache"),
            msgs.getString("Nproc"),
            msgs.getString("Total_Mem"),
            msgs.getString("Free_Mem")
        };
    }

    /**
     * Constructor.
     * @param host The name of the host to check on.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception.
     */
    public CCStatus(String host) throws
        NotBoundException, MalformedURLException, RemoteException
    {
        super(new CaptiveConnection(host));

        remote = (RMIMethods) getInterface();
        ccStats = remote.CCStatus();
    }

    public CaptiveCollectorStats getStats() {
        return ccStats;
    }

    public void refresh(StringBuilder out) throws RemoteException {
        int length = 0;

        ccStats = remote.CCStatus();

        long uptimeMs = ccStats.currentTime.getTime() -
                        ccStats.startTime.getTime();

        values = new Object[] {
            ccStats.version,
            MessageFormat.format(
                "{0,date} {0,time,hh:mm:ss a z}", ccStats.startTime),
            MessageFormat.format(
                "{0,date} {0,time,hh:mm:ss a z}", ccStats.currentTime),
            CmSysInfo.formatUpTime((int) (uptimeMs / 1000)),
            new Long(ccStats.rmcCount),
            new Integer(ccStats.objectsInCache),
            new Integer(ccStats.jvmProcessors),
            formatSz((int) (ccStats.jvmTotalMemory / 1024)),
            formatSz((int) (ccStats.jvmFreeMemory / 1024))
        };

        // status only
        if (out == null)
            return;

        Formatter f = new Formatter(out);

        // find the lhs with the greatest length
        for(String s : tags) {
            int l = s.length();

            if (l > length)
                length = l;
        }

        // construct a format string (%-X.Xs = %s\n)
        String format = "%-" + length + "." + length + "s = %s\n";

        for(int i=0; i<tags.length; i++)
            f.format(format, tags[i], values[i].toString());
    }

    private String formatSz(int sz) {
        final long one_meg = 1024 * 1024;
        final long ten_meg = 10000 * 1024;
        final long one_gig = 1024 * one_meg;
        final long ten_gig = 10000 * one_meg;
        long size;
        long value;
        String neg = "";
        
        // all sizes passed in are in KB
        size = (long) sz * 1024;
        
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
     * The main program.
     * @param args The argument vector passed from the environment.
     */
    public static void main(String[] args)
    {
        boolean quiet = false;
        String host = null;

        if (args.length > 0 && args[0].equals("-q")) {
            quiet = true;
            host = args.length == 2 ? args[1] : "localhost";
        } else
            host = args.length == 1 ? args[0] : "localhost";

        try
        {
            CCStatus ccs = new CCStatus(host);

            if (quiet) {
                ccs.refresh(null);
                System.exit(0);
            }

            StringBuilder sb = new StringBuilder();
            ccs.refresh(sb);
            System.out.print(sb);
        }
        catch(Exception e)
        {
            if (!quiet)
                System.out.println(new CmExceptionParser(e).getMessage());
            System.exit(1);
        }
    }
}
