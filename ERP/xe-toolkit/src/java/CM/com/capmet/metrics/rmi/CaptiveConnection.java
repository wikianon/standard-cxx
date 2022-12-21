/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Captive Metrics library of Captivity.
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

package com.capmet.metrics.rmi;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.util.Properties;

/**
 * Load all of the CaptiveCollector properties from the properties file
 * and make them available to <CODE>RMIAccessPoint</CODE> via a
 * <CODE>RMIProperties</CODE> structure.
 */
public class CaptiveConnection extends RMIProperties {

    private static String     top;
    private static Properties props;

    // load the properties file statically
    static {
        top = System.getProperty("com.capmet.CaptiveCollector.Top");
        if (top == null)
            throw new RuntimeException(RMIMessages.msgs.getString("No_Top"));

        String path = top + File.separatorChar + "etc"
                    + File.separatorChar + "CaptiveCollector.properties";

        props = new Properties();

        try {
            props.load(new FileInputStream(path));
        } catch(IOException e) {
          // skip it, the properties will be null
        }
    }

    /**
     * Build an RMIProperties structure for later use based on values
     * found in the CaptiveCollector properties file.
     * @param remoteHost The server host being connected to.
     */
    public CaptiveConnection(String remoteHost) {
        final String service = RMIMethods.RMI_SIGNATURE;
        String hostName;

        // retrieve the registry port number
        int registryPort = RMIMethods.REGISTRY_PORT;
        final String portProperty =
                "com.capmet.CaptiveCollector.RegistryPort";
        final String rport = props.getProperty(portProperty);
        if (rport != null) {
            try {
                registryPort = Integer.parseInt(rport);
            } catch(NumberFormatException e) {
                registryPort = RMIMethods.REGISTRY_PORT;
            }
        }

        // ascertain whether the connection is secure
        if (remoteHost == null || remoteHost.equals("localhost")) {
            remoteHost = "localhost";
            try {
                hostName = InetAddress.getLocalHost().getHostName();
            } catch(Exception e) {
                hostName = "localhost";
            }
        } else
            hostName = remoteHost;

        final String hostProp =
            "com.capmet.CaptiveCollector.Secure." + hostName;
        final String allProp = "com.capmet.CaptiveCollector.Secure.*";
        final String hostResult = props.getProperty(hostProp);
        final String allResult = props.getProperty(allProp);
        boolean secure = false;

        if (allResult != null)
            secure = Boolean.parseBoolean(allResult);
        if (hostResult != null)
            secure = Boolean.parseBoolean(hostResult);

        final String path = top + File.separatorChar + "etc"
             + File.separatorChar + "Truststore";

        this.connectionHost = remoteHost;
        this.hostName = hostName;
        this.registryPort = registryPort;
        this.service = service;
        this.secure = secure;
        this.trustStorePath = path;
    }
}
