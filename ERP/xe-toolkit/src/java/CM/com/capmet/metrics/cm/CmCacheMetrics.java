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

import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import com.capmet.metrics.rmi.PerformanceCollection;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.rmi.PerformanceCategory;
import com.capmet.metrics.rmi.PerformanceField;
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.CollectionItem;
import com.capmet.metrics.*;

/**
 * This class is the data contained in the value part
 * of the TreeMap used in the <CODE>CmCaches</CODE> class. For each
 * cache name as the key, this class contains the values.
 */
public class CmCacheMetrics
{
    /**
     * The number of hits on this cache over the interval.
     */
    public CmInteger hits;
    /**
     * The number of misses on this cache over the interval.
     */
    public CmInteger misses;
    /**
     * The percentage of the total attempts represented by the number of hits.
     */
    public CmInteger hitPercent;

    // private pointers into the collection for the public fields
    private CollectionItem hitsField;
    private CollectionItem missesField;
    private CollectionItem hitPercentField;

    private CmOSFlavor            osFlavor;
    private PerformanceCollection collection;

    /**
     * Lone constructor for the class.
     * @param host The <CODE>PerformanceHost</CODE> object for the remote host.
     * @param pcat The <CODE>PerformanceCategory</CODE> object for caches.
     * @param instance The name of the cache instance.
     * @param coll The <CODE>PerformanceCollection</CODE> holding all the values.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmCacheMetrics(PerformanceHost host, PerformanceCategory pcat,
        String instance, PerformanceCollection coll)
            throws RemoteException, NotBoundException,
                MalformedURLException, PerformanceException
    {
        String cacheCat = pcat.getCategoryName();

        this.osFlavor = host.getOSFlavor();
        this.collection = coll;

        if (osFlavor.isUnixHost()) {
            String cacheHits   = host.getLocalizedName("Cache Hits");
            String cacheMisses = host.getLocalizedName("Cache Misses");
            String hitPercent  = host.getLocalizedName("Hit Percentage");

            hitsField = coll.put(cacheCat, cacheHits, instance);
            missesField = coll.put(cacheCat, cacheMisses, instance);
            hitPercentField = coll.put(cacheCat, hitPercent, instance);
        } else if (osFlavor.isWindowsHost()) {
            String copyReads = host.getLocalizedName("Copy Reads/sec");
            String dataMaps  = host.getLocalizedName("Data Maps/sec");
            String mdlReads  = host.getLocalizedName("MDL Reads/sec");
            String pinReads  = host.getLocalizedName("Pin Reads/sec");
            String copyHits  = host.getLocalizedName("Copy Read Hits %");
            String dataHits  = host.getLocalizedName("Data Map Hits %");
            String mdlHits   = host.getLocalizedName("MDL Read Hits %");
            String pinHits   = host.getLocalizedName("Pin Read Hits %");

            if (instance.equals(CmCaches.CopyReadName))
            {
                hitsField = coll.put(cacheCat, copyReads);
                hitPercentField = coll.put(cacheCat, copyHits);
            }
            else if (instance.equals(CmCaches.DataMapName))
            {
                hitsField = coll.put(cacheCat, dataMaps);
                hitPercentField = coll.put(cacheCat, dataHits);
            }
            else if (instance.equals(CmCaches.MDLReadName))
            {
                hitsField = coll.put(cacheCat, mdlReads);
                hitPercentField = coll.put(cacheCat, mdlHits);
            }
            else if (instance.equals(CmCaches.PinReadName))
            {
                hitsField = coll.put(cacheCat, pinReads);
                hitPercentField = coll.put(cacheCat, pinHits);
            }
        }

        hits = new CmInteger(true);
        misses = new CmInteger(true);
        hitPercent = new CmInteger(true);
    }

    /**
     * Resync the values from the <CODE>PerformanceCollection</CODE> to this node.
     */
    public void resync()
    {
        if (osFlavor.isUnixHost())
        {
            hits.setValue(((Number) hitsField.value).intValue());
            misses.setValue(((Number) missesField.value).intValue());
            hitPercent.setValue(((Number) hitPercentField.value).intValue());
        }
        else if (osFlavor.isWindowsHost())
        {
            int total = ((Number) hitsField.value).intValue();
            int hitPct = ((Number) hitPercentField.value).intValue();
            double pct = (double) hitPct / 100.0;
            int hitInt = (int) (total * pct);

            hits.setValue(hitInt);
            misses.setValue(total - hitInt);
            hitPercent.setValue(hitPct);
        }

        // No requests is a perfect score.
        if (hits.intValue() == 0)
            hitPercent.setValue(100);
    }

    /**
     * Remove the values from the <CODE>PerformanceCollection</CODE>.
     * @throws RemoteException RMI exception
     */
    public void remove()
        throws RemoteException
    {
        if (osFlavor.isUnixHost())
        {
            collection.remove(hitsField);
            collection.remove(missesField);
            collection.remove(hitPercentField);
        }
        else if (osFlavor.isWindowsHost())
        {
            collection.remove(hitsField);
            collection.remove(hitPercentField);
        }
    }
}
