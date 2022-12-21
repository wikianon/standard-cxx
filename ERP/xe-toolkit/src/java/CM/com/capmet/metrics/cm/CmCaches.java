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
 * An n-tuple class that presents statistics on caches in the system.
 *
 * <PRE>
 * import java.util.Map;
 * import com.capmet.metrics.cm.CmCaches;
 * import com.capmet.metrics.cm.CmCacheMetrics;
 * import com.capmet.metrics.rmi.PerformanceHost;
 * 
 * public class Caches {
 *   public Caches(Object ... args) throws Exception {
 *    String remoteHost = (args.length == 1 ? (String) args[0] : "localhost");
 *    PerformanceHost host = new PerformanceHost(remoteHost);
 *    CmCaches caches = new CmCaches(remoteHost);
 * 
 *    if (!caches.isSupported()) {
 *      System.out.println("Caches category not supported on " + host.getOSFlavor());
 *      return;
 *    }
 * 
 *    for(Map.Entry&lt;String, CmCacheMetrics&gt; e : caches.entrySet()) {
 *      String name = e.getKey();
 *      CmCacheMetrics cm = e.getValue();
 * 
 *      System.out.println(name + ": " + cm.hitPercent);
 *     }
 *   }
 * 
 *   public static void main(String[] args) {
 *     try {
 *         String host = args.length == 1 ? args[0] : null;
 *         new Caches(host);
 *     } catch(Exception e) {
 *         System.err.println(e);
 *     }
 *   }
 * }
 * </PRE>
 */

public class CmCaches extends TreeMap<String, CmCacheMetrics>
    implements CmCategory
{
    /**
     * The manufactured cache name for the Copy Read cache on Windows.
     */
    public final static String CopyReadName = CmMessages.msgs.getString("Copy_Read");
    /**
     * The manufactured cache name for the Data Map cache on Windows.
     */
    public final static String DataMapName  = CmMessages.msgs.getString("Data_Map");
    /**
     * The manufactured cache name for the MDL Read cache on Windows.
     */
    public final static String MDLReadName  = CmMessages.msgs.getString("MDL_Read");
    /**
     * The manufactured cache name for the Pin Read cache on Windows.
     */
    public final static String PinReadName  = CmMessages.msgs.getString("Pin_Read");

    // private data in support of the class
    private PerformanceHost         host;
    private PerformanceCategory     category;
    private PerformanceCollection   collection;
    private CmOSFlavor              osFlavor;
    private boolean                 supported;

    /**
     * Default constructor for looking at caches on the local host.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmCaches()
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        this("localhost");
    }

    /**
     * Constructor for looking at caches on a remote host.
     * @param remoteHost The name of the host to monitor.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmCaches(String remoteHost)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmNames> gnames = CmGlobal.getNames(remoteHost);
        CmNames names = gnames.getObject();

        // grab the PerformanceHost
        host = new PerformanceHost(remoteHost);

        // what OS flavor is it?
        osFlavor = host.getOSFlavor();

        // if it's Unix
        if (osFlavor.isUnixHost())
        {
            if (!host.containsCategory(names.UnixCaches))
            {
                supported = false;
                return;
            }

            supported = true;
            category = host.getCategory(names.UnixCaches);
        }
        // if it's Windows
        else if (osFlavor.isWindowsHost())
        {
            category = host.getCategory(names.WindowsCache);
            supported = true;
        }
        else
            throw new PerformanceException(
                "CmCaches::CmCaches: " +
                    CmMessages.msgs.getString("unknown_platform"));

        // create the PerformanceCollection
        collection = new PerformanceCollection(remoteHost);

        // refresh will populate the collection
        refresh(true);
    }

    /**
     * Refresh the cache metrics.
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


    /**
     * The grunt of the refresh method with indication of whether a
     * category refresh needs to be performed.
     *
     * @param isConstructor Whether this was called by the constructor.
     * @throws java.rmi.RemoteException RMI exception
     * @throws java.rmi.NotBoundException Service is not bound on server machine.
     * @throws java.net.MalformedURLException  The service signature is malformed.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     */
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

        if (osFlavor.isUnixHost())
        {
            // Caches on Unix is an n-tuple. Not so on Windows.

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
            CmInstanceSet deletions = new CmInstanceSet();

            for(String key : instanceSet)
            {
                // if key is not in this
                    // add key to addable

                if (!this.containsKey(key))
                    addable.add(key);
            }

            // foreach key in this
            for(Map.Entry<String, CmCacheMetrics> e : entrySet())
            {
                String key = e.getKey();
                CmCacheMetrics value = e.getValue();

                // if the key is not in instanceSet
                    // CmCacheMetrics node deletes associated counters
                    // delete node associated with key from this

                if (!instanceSet.contains(key))
                    deletions.add(key);
            }

            // foreach key in addable
            for(String key : addable)
            {
                // add a new node to this
                put(key, new CmCacheMetrics(host, category, key, collection));
            }

            for(String key : deletions)
            {
                // cause CmCacheMetrics to delete counters
                get(key).remove();

                // remove instance from this
                remove(key);
            }
        }
        else if (osFlavor.isWindowsHost())
        {
            // Windows has a singleton with all the cache stats in it.

            put(CopyReadName, new CmCacheMetrics(host, category, CopyReadName, collection));
            put(DataMapName,  new CmCacheMetrics(host, category, DataMapName,  collection));
            put(MDLReadName,  new CmCacheMetrics(host, category, MDLReadName,  collection));
            put(PinReadName,  new CmCacheMetrics(host, category, PinReadName,  collection));
        }

        // now that everything is synced up, query the counter collection
        collection.queryCollection();

        // now traverse the nodes in this and update the values
        for(CmCacheMetrics iom : values())
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
