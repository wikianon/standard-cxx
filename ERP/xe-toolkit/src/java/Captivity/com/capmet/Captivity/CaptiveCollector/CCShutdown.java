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

import java.io.File;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.ResourceBundle;
import java.rmi.server.UID;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import com.capmet.metrics.rmi.CaptiveConnection;
import com.capmet.metrics.rmi.RMIAccessPoint;
import com.capmet.metrics.rmi.RMIMethods;

/**
 * The class for shutting down the RMI server.
 */
public class CCShutdown extends RMIAccessPoint
{
    private static ResourceBundle msgs;

    static {
        msgs = ResourceBundle.getBundle("com.capmet.Captivity.CaptiveCollector.MessagesBundle");
    }

    /**
     * Constructor.
     * @param host The name of the host to connect to.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception.
     */
    public CCShutdown(String host, UID challenge) throws
        NotBoundException, MalformedURLException, RemoteException
    {
        super(new CaptiveConnection(host));

        RMIMethods remote = (RMIMethods) getInterface();
        if (!remote.CCShutdown(challenge))
            System.err.println(msgs.getString("Denied"));
    }

    /**
     * Main program for shutting down the server.
     * @param args Argument vector passed from environment.
     */
    public static void main(String[] args)
    {
        String top = System.getProperty("com.capmet.CaptiveCollector.Top");
        UID challenge = null;

        if (top == null || top.length() == 0) {
            System.err.println(msgs.getString("No_Top"));
            System.exit(1);
        }

        String hostName;

        try {
            hostName = InetAddress.getLocalHost().getHostName().split("\\.")[0];
        } catch(UnknownHostException e) {
            throw new RuntimeException(e.getMessage());
        }

        String cFile = top + File.separatorChar + "logs"
            + File.separatorChar + "CaptiveCollector" + File.separatorChar +
            "challenge" + '.' + hostName;

        try {
            FileInputStream fis = new FileInputStream(cFile);
            ObjectInputStream ois = new ObjectInputStream(fis);

            challenge = (UID) ois.readObject();
            ois.close();
        } catch(Exception e) {
            System.err.println(msgs.getString("Denied"));
            System.exit(1);
        }

        try {
            String host = args.length == 1 ? args[0] : "localhost";

            new CCShutdown(host, challenge);
        } catch(Exception e) {
            System.exit(0);
        }
    }
}
