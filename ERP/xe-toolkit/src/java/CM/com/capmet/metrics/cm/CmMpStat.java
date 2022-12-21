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

import java.util.TreeMap;
import java.util.Collection;
import java.util.Set;
import java.util.Map;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.rmi.PerformanceCategory;
import com.capmet.metrics.rmi.PerformanceCollection;
import com.capmet.metrics.CmOSFlavor;

/**
 * An n-tuple class that presents statistics on CPUs in the system.
 *
 * <PRE>
 * import java.util.Map;
 * import com.capmet.metrics.cm.CmMpStat;
 * import com.capmet.metrics.cm.CmMpMetrics;
 * import com.capmet.metrics.rmi.PerformanceHost;
 * 
 * public class Mp {
 *   public Mp(Object ... args) throws Exception {
 *    String remoteHost = (args.length == 1 ? (String) args[0] : "localhost");
 *    PerformanceHost host = new PerformanceHost(remoteHost);
 *    CmMpStat mp = new CmMpStat(remoteHost);
 * 
 *    if (!mp.isSupported()) {
 *      System.out.println("MpStat not supported on " + host.getOSFlavor());
 *      return;
 *    }
 * 
 *    System.out.printf("%6.6s %6.6s\n", "CPU", "Idle %");
 * 
 *    for(Map.Entry&lt;String, CmMpMetrics&gt; e : mp.entrySet()) {
 *      String name = e.getKey();
 *      CmMpMetrics mm = e.getValue();
 * 
 *      System.out.printf("%6.6s %6d\n", name, mm.idleTimePercent.intValue());
 *     }
 *   }
 * 
 *   public static void main(String[] args) {
 *     try {
 *         String host = args.length == 1 ? args[0] : null;
 *         new Mp(host);
 *     } catch(Exception e) {
 *         System.err.println(e);
 *     }
 *   }
 * }
 * </PRE>
 */
public class CmMpStat extends TreeMap<String, CmMpMetrics>
    implements CmCategory
{
    // private members in support of the class
    private PerformanceCategory     category;
    private PerformanceCollection   collection;
    private CmOSFlavor              osFlavor;
    private boolean                 supported;
    private PerformanceHost         host;

    /**
     * Default constructor for looking at CPUs on the local host.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmMpStat()
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        this("localhost");
    }

    /**
     * Constructor for looking at CPUs on a remote host.
     * @param remoteHost The name of the host to monitor.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmMpStat(String remoteHost)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        // grab the PerformanceHost
        host = new PerformanceHost(remoteHost);
        CmGlobal<CmNames> gnames = CmGlobal.getNames(remoteHost);
        CmNames names = gnames.getObject();

        // what OS flavor is it?
        osFlavor = host.getOSFlavor();

        // if it's Unix
        if (osFlavor.isUnixHost())
        {
            if (!host.containsCategory(names.UnixMpStat))
            {
                supported = false;
                return;
            }

            supported = true;
            category = host.getCategory(names.UnixMpStat);
        }

        // if it's Windows
        else if (osFlavor.isWindowsHost())
            category = host.getCategory(names.WindowsProcessor);

        else
            throw new PerformanceException(
                "CmMpStat::CmMpStat: unknown host flavor");

        // create the PerformanceCollection
        collection = new PerformanceCollection(remoteHost);

        // refresh will populate the collection
        refresh(true);
    }

    /**
     * Refresh the CPU metrics.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public void refresh()
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        refresh(false);
    }

    // The grunt of the refresh method with indication of whether a
    // category refresh needs to be performed.

    private void refresh(boolean isConstructor)
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        // do a category refresh so that the instance names
        // can be updated and we can report on resources that
        // are still there or newly appeared; or don't report
        // on those that have gone away

        if (!isConstructor)
            category.categoryRefreshRates();

        // get the instances names from the newly refreshed category
        String[] instances = category.getInstanceNames();

        // need a fast lookup structure
        CmInstanceSet instanceSet = new CmInstanceSet();

        // place instances in an CmInstanceSet
        for(int i=0; i<instances.length; i++)
            if (!instances[i].equals("_Total"))
                instanceSet.add(instances[i]);

        // foreach key in instanceSet
        CmInstanceSet addable = new CmInstanceSet();

        for(String key : instanceSet)
        {
            // if key is not in this
                // add key to addable

            if (!this.containsKey(key))
                addable.add(key);
        }

        // foreach key in this
        for(Map.Entry<String, CmMpMetrics> e : entrySet())
        {
            String key = e.getKey();
            CmMpMetrics value = e.getValue();

            // if the key is not in instanceSet
                // CmMpMetrics node deletes associated counters
                // delete node associated with key from this

            if (!instanceSet.contains(key))
            {
                // cause CmMpMetrics to delete counters
                value.remove();

                // remove instance from this
                remove(key);
            }
        }

        // foreach key in addable
        for(String key : addable)
        {
            // add a new node to this
            put(key, new CmMpMetrics(host, category, key, collection));
        }

        // now that everything is synced up, query the counter collection
        collection.queryCollection();

        // now traverse the nodes in this and update the values
        for(CmMpMetrics iom : values())
        {
            iom.resync();
        }
    }

    /**
     * Indicate whether this category is supported.
     * @return A boolean indication: true - the category is supported; false - it is not.
     */
    public boolean isSupported()
    {
        return supported;
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
