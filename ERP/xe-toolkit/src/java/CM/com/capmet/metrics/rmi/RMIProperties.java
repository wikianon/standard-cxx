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

/**
 * Properties needed by <CODE>RMIAccessPoint</CODE> to make a correct
 * connection to an RMI service.
 */
public class RMIProperties {

    // The difference between connectionHost and hostName is this:
    // If the client is running on a host named "boo" and a constructor
    // indicates its desire to connect to "localhost", the connectionHost
    // will be "localhost" and hostName will be "boo".

    protected String  connectionHost;
    protected String  hostName;
    protected int     registryPort;
    protected String  service;
    protected boolean secure;
    protected String  trustStorePath;

    /**
     * Default constructor.
     */
     public RMIProperties() {
        this.connectionHost = null;
        this.hostName = null;
        this.registryPort = -1;
        this.service = null;
        this.secure = false;
        this.trustStorePath = null;
     }

    /**
     * Constructor providing the property values at instantiation time.
     * @param connectionHost The host name to use for the connection.
     * @param hostName The name of the server host.
     * @param registryPort The port number of the service.
     * @param service The RMI signature of the service.
     * @param secure Whether the service requires a secure connection.
     * @param trustStorePath The path to the TrustStore file for the client.
     */
    public RMIProperties(String connectionHost, String hostName,
                         int registryPort, String service,
                         boolean secure, String trustStorePath) {
        this.connectionHost = connectionHost;
        this.hostName = hostName;
        this.registryPort = registryPort;
        this.service = service;
        this.secure = secure;
        this.trustStorePath = trustStorePath;
    }

    /**
     * Retrieve the name of the host to make the connection to.
     * @return A string that is the name of the host running the service.
     */
    public String getConnectionHost() {
        return connectionHost;
    }

    /**
     * Retrieve the name of the server host.
     * @return A string that is the name of the host running the service.
     */
    public String getHostName() {
        return hostName;
    }

    /**
     * Retrieve the service registry port number.
     * @return The TCP port number that the RMI service is listening on.
     */
    public int getRegistryPort() {
        return registryPort;
    }

    /**
     * Retrieve the service name.
     * @return The RMI signature of the RMI service.
     */
    public String getService() {
        return service;
    }

    /**
     * Retrieve an indication of whether the connection should be secure.
     * @return True: it is secure; False: it is not.
     */
    public boolean isSecure() {
        return secure;
    }

    /**
     * Retrieve the path of the TrustStore file.
     * @return A string that is the path leading to the TrustStore file.
     */
    public String getTrustStorePath() {
        return trustStorePath;
    }
}
