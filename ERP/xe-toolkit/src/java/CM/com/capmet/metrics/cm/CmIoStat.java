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
 * An n-tuple class that presents statistics on I/O devices in the system.
 *
 * <PRE>
 * import java.util.Map;
 * import com.capmet.metrics.cm.CmIoStat;
 * import com.capmet.metrics.cm.CmIoMetrics;
 * import com.capmet.metrics.rmi.PerformanceHost;
 * 
 * public class Disks {
 *   public Disks(Object ... args) throws Exception {
 *    String remoteHost = (args.length == 1 ? (String) args[0] : "localhost");
 *    PerformanceHost host = new PerformanceHost(remoteHost);
 *    CmIoStat io = new CmIoStat(remoteHost);
 * 
 *    if (!io.isSupported()) {
 *      System.out.println("IoStat category not supported on " + host.getOSFlavor());
 *      return;
 *    }
 * 
 *    System.out.printf("%10.10s %10.10s %10.10s\n", "Name", "Type", "Capacity");
 * 
 *    for(Map.Entry&lt;String, CmIoMetrics&gt; e : io.entrySet()) {
 *      String name = e.getKey();
 *      CmIoMetrics im = e.getValue();
 * 
 *      if (im.capacityMB.intValue() == 0)
 *        continue;
 * 
 *      System.out.printf("%10.10s %10.10s %10d\n",
 *        name, im.diskType, im.capacityMB.intValue());
 *     }
 *   }
 * 
 *   public static void main(String[] args) {
 *     try {
 *         String host = args.length == 1 ? args[0] : null;
 *         new Disks(host);
 *     } catch(Exception e) {
 *         System.err.println(e);
 *     }
 *   }
 * }
 * </PRE>
 */
public class CmIoStat extends TreeMap<String, CmIoMetrics>
    implements CmCategory
{
    // private data in support of the class
    private PerformanceCategory     unixCat;
    private PerformanceCategory     physCat;
    private PerformanceCategory     logCat;
    private PerformanceCategory     infoCat;
    private PerformanceCollection   collection;
    private CmOSFlavor              osFlavor;
    private PerformanceHost         host;

    /**
     * Default constructor for looking at I/O devices on the local host.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmIoStat()
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        this("localhost");
    }

    /**
     * Constructor for looking at I/O devices on a remote host.
     * @param remoteHost The name of the host to monitor.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmIoStat(String remoteHost)
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
            unixCat = host.getCategory(names.UnixDiskStat);

        // if it's Windows
        else if (osFlavor.isWindowsHost())
        {
            physCat = host.getCategory(names.WindowsPhysDisk);
            if (host.containsCategory(names.WindowsLogicDisk))
                logCat = host.getCategory(names.WindowsLogicDisk);
            else
                logCat = null;

            infoCat = host.getCategory(names.WindowsDiskInfo);
        }

        else
            throw new PerformanceException(
                "CmIoStat::CmIoStat: " +
                    CmMessages.msgs.getString("unknown_platform"));

        // create the PerformanceCollection
        collection = new PerformanceCollection(remoteHost);

        // refresh will populate the collection
        refresh(true);
    }

    /**
     * Refresh the I/O metrics.
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
        if (osFlavor.isUnixHost())
        {
            // do a category refresh so that the instance names
            // can be updated and we can report on resources that
            // are still there or newly appeared; or don't report
            // on those that have gone away

            if (!isConstructor)
                unixCat.categoryRefresh();

            // get the instances names from the newly refreshed category
            String[] instances;

            instances = unixCat.getInstanceNames();

            // need a fast lookup structure
            CmInstanceSet instanceSet = new CmInstanceSet();

            // place instances in an CmInstanceSet
            for(int i=0; i<instances.length; i++)
                if (!instances[i].equals("_Total"))
                    instanceSet.add(instances[i]);

            // foreach key in instanceSet
            CmInstanceSet addable = new CmInstanceSet();
            CmInstanceSet deletions = new CmInstanceSet();

            for(String key : instanceSet)
            {
                // if key is not in this
                    // add key to addable

                if (!this.containsKey(key))
                    addable.add(key);
            }

            // foreach key in this
            for(Map.Entry<String, CmIoMetrics> e : entrySet())
            {
                String key = e.getKey();
                CmIoMetrics value = e.getValue();

                // if the key is not in instanceSet
                    // CmIoMetrics node deletes associated counters
                    // delete node associated with key from this

                if (!instanceSet.contains(key))
                    deletions.add(key);
            }

            // foreach key in addable
            for(String key : addable)
            {
                // add a new node to this
                put(key, new CmIoMetrics(host, unixCat, null, key, collection));
            }

            // foreach key in deletions
            for(String key : deletions)
            {
                // cause CmIoMetrics to delete counters
                get(key).remove();

                // remove instance from this
                remove(key);
            }

            // now that everything is synced up, query the counter collection
            collection.queryCollection();

            // now traverse the nodes in this and update the values
            for(CmIoMetrics iom : values())
            {
                iom.resync();
            }
        }
        else if (osFlavor.isWindowsHost())
        {
            if (!isConstructor)
            {
                physCat.categoryRefresh();
                if (logCat != null)
                    logCat.categoryRefresh();
                infoCat.categoryRefresh();
            }

            // get the instances names from the newly refreshed category
            String[] physInstances;
            String[] logInstances;

            physInstances = physCat.getInstanceNames();
            logInstances = (logCat == null ? null : logCat.getInstanceNames());

            // need a fast lookup structure
            CmInstanceSet physSet = new CmInstanceSet();
            CmInstanceSet logSet = (logInstances == null ? null : new CmInstanceSet());

            // place instances in a CmInstanceSet
            for(int i=0; i<physInstances.length; i++)
                if (!physInstances[i].equals("_Total"))
                    physSet.add(physInstances[i]);

            if (logInstances != null)
            {
                // place instances in an CmInstanceSet
                for(int i=0; i<logInstances.length; i++)
                    if (!logInstances[i].equals("_Total") &&
                        !logInstances[i].startsWith("HarddiskVolume"))
                        logSet.add(logInstances[i]);
            }

            CmInstanceSet addable = new CmInstanceSet();
            CmInstanceSet deletions = new CmInstanceSet();

            // foreach key in physSet
            for(String key : physSet)
            {
                // if key is not in this
                    // add key to addable

                if (!this.containsKey(key))
                    addable.add(key);
            }

            // foreach key in logSet
            if (logSet != null)
            {
                for(String key : logSet)
                {
                    // if key is not in this
                        // add key to addable

                    if (!this.containsKey(key))
                        addable.add(key);
                }
            }

            // foreach key in this
            for(Map.Entry<String, CmIoMetrics> e : entrySet())
            {
                String key = e.getKey();
                CmIoMetrics value = e.getValue();

                // if the key is not in either set
                    // CmIoMetrics node deletes associated counters
                    // delete node associated with key from this

                if (!physSet.contains(key) &&
                    (logSet != null && !logSet.contains(key)))
                        deletions.add(key);
            }

            // foreach key in addable
            for(String key : addable)
            {
                // add a new node to this
                if (physSet.contains(key))
                    put(key, new CmIoMetrics(host, physCat, infoCat, key, collection));
                else if (logSet != null && logSet.contains(key))
                    put(key, new CmIoMetrics(host, logCat, infoCat, key, collection));
            }

            // foreach key in deletions
            for(String key : deletions)
            {
                // cause CmIoMetrics to delete counters
                get(key).remove();

                // remove instance from this
                remove(key);
            }
        }

        // now that everything is synced up, query the counter collection
        collection.queryCollection();

        // now traverse the nodes in this and update the values
        for(CmIoMetrics iom : values())
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
        return true;
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
