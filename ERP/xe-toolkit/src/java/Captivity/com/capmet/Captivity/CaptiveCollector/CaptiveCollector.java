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
import java.io.IOException;
import java.io.FileInputStream;
import java.rmi.Naming;
import java.rmi.registry.*;
import javax.rmi.ssl.*;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.text.MessageFormat;
import java.util.Date;
import java.util.ResourceBundle;
import java.util.Properties;
import java.util.logging.Logger;
import java.util.logging.Handler;
import java.util.logging.FileHandler;
import com.capmet.metrics.rmi.RMIMethods;

/**
 * The CaptiveCollector program from Captivity.
 */
public class CaptiveCollector {
    public static Logger         log;
    public static ResourceBundle msgs;

    private final static String  hostName;
    private final static String  top;

    static {
        msgs = ResourceBundle.getBundle(
            "com.capmet.Captivity.CaptiveCollector.MessagesBundle");

        top = System.getProperty("com.capmet.CaptiveCollector.Top");

        if (top == null || top.length() == 0) {
            System.err.println(msgs.getString("No_Top"));
            System.exit(1);
        }

        String cname = CaptiveCollector.class.getName();
        String fname;
        int n = cname.lastIndexOf('.');

        if (n != -1)
            fname = cname.substring(n + 1, cname.length());
        else
            fname = cname;

        try {
            hostName = InetAddress.getLocalHost().getHostName().split("\\.")[0];
        } catch(UnknownHostException e) {
            throw new RuntimeException(e.getMessage());
        }

        String logFile =
            top + File.separatorChar + "logs" + File.separatorChar + fname +
            File.separatorChar + hostName;
        String dt =
            MessageFormat.format("{0,date,MM-dd-yyyy}::{0,time,HH:mm:ss}",
                new Date());
        String oldLogFile =
            top + File.separatorChar + "logs" + File.separatorChar + fname +
            File.separator + "old" + File.separatorChar + hostName +
            ".thru-" + dt;

        log = Logger.getLogger(cname);
        log.setUseParentHandlers(false);

        Handler h = null;

        try {
            File newFile = new File(logFile);
            File renamedFile = new File(oldLogFile);

            newFile.renameTo(renamedFile);
            h = new FileHandler(logFile, false);
        } catch(IOException e) {
            throw new RuntimeException(e.getMessage());
        }

        h.setFormatter(new CaptiveFormatter());
        log.addHandler(h);
    }

    private static void usage() {
        System.err.println(msgs.getString("Usage"));
        log.severe(msgs.getString("Usage"));
        System.exit(1);
    }

    /**
     * The main program.
     * @param args The argument vector passed in from the environment.
     */
    public static void main(String[] args) {
        try {
            // path to properties file
            String propPath = top + File.separatorChar + "etc"
                            + File.separatorChar
                            + "CaptiveCollector.properties";
            String pwdPath = top + File.separatorChar + "etc"
                            + File.separatorChar
                            + "CaptiveCollector.password";

            // load application properties
            Properties props = new Properties();
            props.load(new FileInputStream(propPath));

            Properties pwdProps = new Properties();
            pwdProps.load(new FileInputStream(pwdPath));

            // lookup wildcard and host specific secure options
            final String hostProp =
                "com.capmet.CaptiveCollector.Secure." + hostName;
            final String allProp = "com.capmet.CaptiveCollector.Secure.*";
            final String hostResult = props.getProperty(hostProp);
            final String allResult = props.getProperty(allProp);
            boolean secure = false;

            // use "all" result first and then veto it with host specific
            if (allResult != null)
                secure = Boolean.parseBoolean(allResult);
            if (hostResult != null)
                secure = Boolean.parseBoolean(hostResult);

            // get the registry port number
            final String portProperty =
                "com.capmet.CaptiveCollector.RegistryPort";
            int registryPort = RMIMethods.REGISTRY_PORT;

            String rport = props.getProperty(portProperty);
            if (rport != null) {
                try {
                    registryPort = Integer.parseInt(rport);
                } catch(NumberFormatException e) {
                    registryPort = RMIMethods.REGISTRY_PORT;
                }
            }

            log.info(msgs.getString("Starting") + registryPort);

            Registry registry;
            RMIServer rmi;

            if (secure) {
                log.info(msgs.getString("Secure"));

                String keystore = top + File.separatorChar + "etc"
                                + File.separatorChar + "Keystore";
                String pwd = pwdProps.getProperty(
                    "com.capmet.CaptiveCollector.password");

                System.setProperty("javax.net.ssl.keyStore", keystore);
                System.setProperty("javax.net.ssl.keyStorePassword", pwd);

                SslRMIClientSocketFactory csf =
                    new SslRMIClientSocketFactory();
                SslRMIServerSocketFactory ssf =
                    new SslRMIServerSocketFactory();

                registry = LocateRegistry.createRegistry(
                    registryPort, csf, ssf);
                rmi = new RMIServer(top, csf, ssf);
            } else {
                log.info(msgs.getString("Not_Secure"));
                registry = LocateRegistry.createRegistry(registryPort);
                rmi = new RMIServer(top);
            }

            registry.bind(RMIMethods.RMI_SIGNATURE, rmi);
        }
        catch(Exception e) {
            log.severe(e.getMessage());
            System.exit(1);
        }
    }
}

