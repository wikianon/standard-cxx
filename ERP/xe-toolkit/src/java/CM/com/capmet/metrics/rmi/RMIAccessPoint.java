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
import java.rmi.NotBoundException;
import java.rmi.registry.*;
import javax.rmi.ssl.*;
import java.net.InetAddress;
import java.net.MalformedURLException;
import java.rmi.RemoteException;
import java.util.TreeMap;
import java.util.Properties;
import java.rmi.Naming;
import java.rmi.Remote;

/**
 * A map of hostname to <CODE>Remote</CODE> access points.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
class RMIHostMap extends TreeMap<String, Remote> {}

/**
 * A common access mechanism so that all objects using RMI will use
 * the same <CODE>Remote</CODE> object.
 * <p>
 * @author Richard Pettit
 * @since JDK1.5
 */
public class RMIAccessPoint
{
    /**
     * The map of hostname to <CODE>Remote</CODE> access points.
     */
    private static RMIHostMap RMIHosts;

    static {
        RMIHosts = new RMIHostMap();
    }

    /**
     * Local storage for the <CODE>Remote</CODE> access point.
     */
    private Remote accessPoint;

    /**
     * Build a client/server RMI connection based on connection properties.
     * @param props The properties needed to build a correct connection.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     */
    public RMIAccessPoint(RMIProperties props) throws
        NotBoundException, MalformedURLException, RemoteException
    {
        String connectionHost = props.getConnectionHost();
        String hostName       = props.getHostName();
        int registryPort      = props.getRegistryPort();
        String service        = props.getService();
        boolean secure        = props.isSecure();
        String trustStore     = props.getTrustStorePath();

        // build a key for lookup in the map
        final String target =
            "rmi://" + hostName + ":" + registryPort + "/" + service;

        accessPoint = null;

        synchronized(RMIHosts) {
            if (RMIHosts.containsKey(target))
                accessPoint = RMIHosts.get(target);

            // XXX: someday when there are cycles, this needs fixing.
            //      RMIAccessPoint is not supposed to know what it will
            //      be cast to nor the methods contained in that class.

            if (accessPoint != null) {
                try {
                    if (((RMIMethods) accessPoint).hasValidConnection())
                        return;
                } catch(Exception e) {
                    accessPoint = null;
                    RMIHosts.remove(target);
                }
            }

            if (accessPoint == null) {
                Registry registry;

                if (secure) {
                    System.setProperty("javax.net.ssl.trustStore", trustStore);
                    registry = LocateRegistry.getRegistry(
                        connectionHost, registryPort,
                            new SslRMIClientSocketFactory());
                } else
                    registry = LocateRegistry.getRegistry(
                        connectionHost, registryPort);

                accessPoint = registry.lookup(service);
                RMIHosts.put(target, (Remote) accessPoint);
            } else
                accessPoint = RMIHosts.get(target);
        }
    }

    /**
     * Remove a cached access point. This is needed for specific cases
     * where a client connects to a server, the server goes away, comes
     * back and the client knows it and wants to reestablish a connection
     * with the new server.
     * @param props The properties needed to build a correct connection.
     */
    public static void removeAccessPoint(RMIProperties props)
    {
        String hostName   = props.getHostName();
        int registryPort  = props.getRegistryPort();
        String service    = props.getService();

        final String target =
            "rmi://" + hostName + ":" + registryPort + "/" + service;

        synchronized(RMIHosts) {
            if (!RMIHosts.containsKey(target))
                return;

            RMIHosts.remove(target);
        }
    }

    /**
     * Return the <CODE>Remote</CODE> access point.
     * @return The RMI <CODE>Remote</CODE> access point.
     */
    public Remote getInterface()
    {
        return accessPoint;
    }
}
