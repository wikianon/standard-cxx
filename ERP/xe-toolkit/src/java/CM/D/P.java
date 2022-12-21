/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Captive Metrics library for Captivity.
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

package D;

import java.rmi.server.UID;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import com.capmet.metrics.CmOSFlavor;
import com.capmet.metrics.rmi.RMIMethods;
import com.capmet.metrics.rmi.RemoteJanitor;
import com.capmet.metrics.rmi.RMIAccessPoint;
import com.capmet.metrics.rmi.CaptiveConnection;

/**
 * P is the process class in the D package. The brevity in the names is for reducing the
 * amount of text transmitted over the RMI connection as serialization includes the fully
 * qualified names of the objects being transmitted. Since the processes and threads
 * collections are passed as a SLOB (Serialized Large OBject) the goal was to get the
 * amount of data being transmitted down to the smallest amount possible without
 * compression.
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class P extends RMIAccessPoint
{
    /**
     * The unique id for the server-side object.
     */
    private UID objectId;
    /**
     * The <CODE>Remote</CODE> access point to the RMI server.
     */
    private RMIMethods  remote;

    /**
     * The default constructor for the P class. No parameter is equivalent to connecting the
     * CaptiveCollector on "localhost".
     * @throws java.rmi.NotBoundException The CaptiveCollector is not bound on the target host.
     * @throws java.net.MalformedURLException The RMI URL contains a syntax error.
     * @throws java.rmi.RemoteException An exception occurred on the server side and was propagated to the client.
     */
    public P() throws
        NotBoundException, MalformedURLException, RemoteException
    {
        this("localhost");
    }

    /**
     * The constructor for the P class which connects to a specified host.
     * @param host The name of the host running the CaptiveCollector which is to be connected to.
     * @throws java.rmi.NotBoundException The CaptiveCollector is not bound on the target host.
     * @throws java.net.MalformedURLException The RMI URL contains a syntax error.
     * @throws java.rmi.RemoteException An exception occurred on the server side and was propagated to the client.
     */
    public P(String host) throws
        NotBoundException, MalformedURLException, RemoteException
    {
        super(new CaptiveConnection(host));

        remote = (RMIMethods) getInterface();
        objectId = remote.PCons();

        RemoteJanitor.put(objectId, remote);
    }

    /**
     * Retrive a PT object from the server containing information about all of the running
     * processes on the target host.
     * @throws java.rmi.RemoteException An exception occurred on the server side and was propagated to the client.
     * @return A PT object from CaptiveCollector.
     */
    public PT Pget()
        throws RemoteException
    {
        return remote.Pget(objectId);
    }
}
