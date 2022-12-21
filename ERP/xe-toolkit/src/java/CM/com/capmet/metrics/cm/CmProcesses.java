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

package com.capmet.metrics.cm;

import java.util.Map;
import java.util.TreeMap;
import java.util.Vector;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import com.capmet.metrics.CmOSFlavor;
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.PerformanceException;

/**
 * An n-tuple class that presents statistics on processes in the system.
 *
 * <PRE>
 * import java.util.Map;
 * import com.capmet.metrics.cm.CmProcesses;
 * import com.capmet.metrics.cm.CmProcessMetrics;
 * import com.capmet.metrics.rmi.PerformanceHost;
 * 
 * public class Procs {
 *   public Procs(Object ... args) throws Exception {
 *    String remoteHost = (args.length == 1 ? (String) args[0] : "localhost");
 *    PerformanceHost host = new PerformanceHost(remoteHost);
 *    CmProcesses ps = new CmProcesses(remoteHost);
 * 
 *    System.out.printf("%-20.20s %20.20s %8.8s\n",
 *      "Name", "Owner", "State");
 * 
 *    for(Map.Entry&lt;String, CmProcessMetrics&gt; e : ps.entrySet()) {
 *      String name = e.getKey();
 *      CmProcessMetrics pm = e.getValue();
 * 
 *      System.out.printf("%-20.20s %20.20s %8.8s\n",
 *        pm.commandName, pm.userName, pm.processState);
 *     }
 *   }
 * 
 *   public static void main(String[] args) {
 *     try {
 *         String host = args.length == 1 ? args[0] : null;
 *         new Procs(host);
 *     } catch(Exception e) {
 *         System.err.println(e);
 *     }
 *   }
 * }
 * </PRE>
 */
public class CmProcesses extends TreeMap<String, CmProcessMetrics>
    implements CmCategory
{
    // private members in support of the class
    private PerformanceHost          host;
    private D.P                      processes;
    private long                     supportFlags;
    private Sort<SortContainer>      sorter;
    private Vector<CmProcessMetrics> sortedVector;
    private CmOSFlavor               osFlavor;

    /**
     * Default constructor for looking at processes on the local host.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmProcesses()
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        this("localhost");
    }

    /**
     * Constructor for looking at processes on a remote host.
     * @param remoteHost The name of the host to monitor.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmProcesses(String remoteHost)
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        host = new PerformanceHost(remoteHost);
        osFlavor = host.getOSFlavor();
        processes = new D.P(remoteHost);
        sorter = new Sort<SortContainer>();
        sortedVector = new Vector<CmProcessMetrics>();

        refresh();
    }

    /**
     * Refresh the process metrics.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public void refresh()
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        D.PT ptree = processes.Pget();
        CmInstanceSet deletions = new CmInstanceSet();

        this.supportFlags = ptree.supportFlags;

        // for each entry in processes
        for(Map.Entry<String, D.PN> e : ptree.entrySet())
        {
            String key = e.getKey();
            D.PN value = e.getValue();

            // if it's not in this then add it
            if (!containsKey(key))
                put(key, new CmProcessMetrics(value, ptree.supportFlags));
            else
                get(key).update(value);
        }

        // for every entry in this
        for(Map.Entry<String, CmProcessMetrics> e : entrySet())
        {
            String key = e.getKey();

            // if it's not in processes then mark it for deletion
            if (!ptree.containsKey(key))
                deletions.add(key);
        }

        // delete the deleteables
        for(String key : deletions)
            remove(key);
    }

    /**
     * Indicate whether this category is supported.
     * @return A boolean indication: true - the category is supported; false - it is not.
     */
    public boolean isSupported()
    {
        return true;
    }

    /**
     * Indicate whether a specific field is supported.
     * @param fieldNumber The field number as described in class <CODE>D.PN</CODE>.
     * @return A boolean indication: true - the field is supported; false - it is not.
     */
    public boolean isSupported(int fieldNumber)
    {
        return ((1 << fieldNumber) & supportFlags) != 0;
    }

    // sort: forward or reverse

    private void sort(int fieldIndex, boolean reverse)
    {
        int n = size();
        SortContainer[] array = new SortContainer[n];
        int i = 0;

        for(CmProcessMetrics m : values())
            array[i++] = new SortContainer(m, fieldIndex);

        if (reverse)
            sorter.rsort(array);
        else
            sorter.sort(array);

        sortedVector.clear();

        for(i=0; i<n; i++)
            sortedVector.addElement(array[i].getMetrics());
    }

    /**
     * Perform an ascending sort of the processes by a specific field.
     * @param fieldIndex The field number as described in class <CODE>D.PN</CODE>.
     */
    public void sort(int fieldIndex)
    {
        sort(fieldIndex, false);
    }

    /**
     * Perform a descending sort of the processes by a specific field.
     * @param fieldIndex The field number as described in class <CODE>D.PN</CODE>.
     */
    public void rsort(int fieldIndex)
    {
        sort(fieldIndex, true);
    }

    /**
     * Obtain the vector of sorted processes.
     * @return A Vector&lt;CmProcessMetrics&gt; of <CODE>CmProcessMetrics</CODE>.
     */
    public Vector<CmProcessMetrics> sortedValues()
    {
        return sortedVector;
    }

    /**
     * Retrieve the OS flavor according to <CODE>CmOSFlavor</CODE>.
     * @return A <CODE>CmOSFlavor</CODE> object.
     */
    public CmOSFlavor getOSFlavor()
    {
        return osFlavor;
    }

    /**
     * Determine if this host's connection to the remote host is valid.
     * @return true - valid connection; false - connection is broken
     */
    public boolean hasValidConnection() {
        return host.hasValidConnection();
    }
}
