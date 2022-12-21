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
import java.net.MalformedURLException;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.PerformanceCategory;
import com.capmet.metrics.rmi.PerformanceCollection;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.rmi.CollectionItem;
import com.capmet.metrics.CmOSFlavor;

/**
 * An n-tuple class that maps Unix mount points from the
 * special file name to the directory path where the resource
 * is mounted.
 *
 * <PRE>
 * import java.util.Map;
 * import com.capmet.metrics.cm.CmMounts;
 * import com.capmet.metrics.rmi.PerformanceHost;
 * 
 * public class Mounts {
 *   public Mounts(Object ... args) throws Exception {
 *    String remoteHost = (args.length == 1 ? (String) args[0] : "localhost");
 *    PerformanceHost host = new PerformanceHost(remoteHost);
 *    CmMounts mnt = new CmMounts(remoteHost);
 * 
 *    if (!mnt.isSupported()) {
 *      System.out.println("Mounts category not supported on " + host.getOSFlavor());
 *      return;
 *    }
 * 
 *    System.out.printf("%-20.20s %-20.20s\n", "Special File", "Mount Point");
 * 
 *    for(Map.Entry&lt;String, String&gt; e : mnt.entrySet()) {
 *      String special = e.getKey();
 *      String mntPoint = e.getValue();
 * 
 *      if (!special.equals(mntPoint))
 *        System.out.printf("%-20.20s %-20.20s\n", special, mntPoint);
 *     }
 *   }
 * 
 *   public static void main(String[] args) {
 *     try {
 *         String host = args.length == 1 ? args[0] : null;
 *         new Mounts(host);
 *     } catch(Exception e) {
 *         System.err.println(e);
 *     }
 *   }
 * }
 * </PRE>
 */
public class CmMounts extends TreeMap<String, String>
    implements CmCategory
{
    // private members in support of the class
    private String              remoteHost;
    private PerformanceHost     host;
    private CmOSFlavor          osFlavor;
    private CmNames             names;
    private PerformanceCategory mnt;

    /**
     * Default constructor for looking at mount points on the local host.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmMounts()
        throws RemoteException, MalformedURLException,
            NotBoundException, PerformanceException
    {
        this(null);
    }

    /**
     * Constructor for looking at mount points on a remote host.
     * @param remoteHost The name of the host to monitor.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmMounts(String remoteHost)
        throws RemoteException, MalformedURLException,
            NotBoundException, PerformanceException
    {
        host = new PerformanceHost(remoteHost);
        osFlavor = host.getOSFlavor();

        // No mount point mapping on Windows
        if (osFlavor.isWindowsHost())
            return;

        this.remoteHost = remoteHost;
        CmGlobal<CmNames> gnames = CmGlobal.getNames(remoteHost);
        names = gnames.getObject();
        mnt = host.getCategory(names.UnixMounts);

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
        throws RemoteException, MalformedURLException,
            NotBoundException, PerformanceException
    {
        refresh(false);
    }

    // The grunt of the refresh method with indication of whether a
    // category refresh needs to be performed.

    private void refresh(boolean isConstructor)
        throws RemoteException, MalformedURLException,
            NotBoundException, PerformanceException
    {
        if (osFlavor.isUnixHost())
        {
            if (!isConstructor)
                mnt.categoryRefresh();

            PerformanceCollection collection = new PerformanceCollection(remoteHost);
            String[] instances = mnt.getInstanceNames();
            String mntPoint = host.getLocalizedName("Mount Point");

            // a place to store the instance name to CollectionItem mappings
            TreeMap<String, CollectionItem> pairs = new TreeMap<String, CollectionItem>();

            // for each instance, add a CollectionItem
            for(String inst : instances)
                pairs.put(inst, collection.put(names.UnixMounts, mntPoint, inst));

            // query the whole mess
            collection.queryCollection();

            // now traverse the intermediate and build the final result
            this.clear();
            for(Map.Entry<String, CollectionItem> e : pairs.entrySet())
                this.put(e.getKey(), (String) e.getValue().value);
        }
        // The concept of mapping a special file to its mount
        // point is not a Windows-ism. 0 is 0 and C: is C:. For now, anyway.
    }

    /**
     * Convenience method for mapping a special file to its mount point.
     * @param specialFile The special file whose mount point is desired.
     * @return The mount point associated with the special file or the special
     * file itself if no mapping can be found.
     */
    public String toMountPoint(String specialFile)
    {
        String mountPoint = this.get(specialFile);

        return (mountPoint == null) ? specialFile : mountPoint;
    }

    /**
     * Indicate whether this category is supported.
     * @return A boolean indication: true - the category is supported; false - it is not.
     */
    public boolean isSupported()
    {
        return osFlavor.isUnixHost();
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
