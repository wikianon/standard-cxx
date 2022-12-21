/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the XE library for The XE Toolkit.
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

package com.capmet.tools.XE.rules;

import java.io.PrintStream;
import java.util.Map;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import com.capmet.metrics.CmOSFlavor;
import com.capmet.metrics.cm.CmGlobal;
import com.capmet.metrics.cm.CmCaches;
import com.capmet.metrics.cm.CmCacheMetrics;
import com.capmet.metrics.rmi.PerformanceException;

public class LiveCacheRule extends PureCacheRule {

    private CmGlobal<CmCaches>  gcache;
    private CmCaches            cache;
    private PureCacheSeedValues sv;
    private Threshold<String>   excludeCachesThr;
    private String              excludeCaches;

    public LiveCacheRule() throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        this("localhost");
    }

    public LiveCacheRule(String remoteHost) throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        gcache = CmGlobal.getCaches(remoteHost);
        cache = gcache.getObject();

        CmOSFlavor osFlavor = cache.getOSFlavor();

        excludeCachesThr = new Threshold<String>("CACHE_EXCLUSION_LIST", "");
        excludeCaches = getThreshString(excludeCachesThr, osFlavor);

        sv = new PureCacheSeedValues();
        sv.osFlavor = osFlavor;

        populateSeedValues();
        seedValues(sv);
    }

    public void print(PrintStream out) {
        out.println(formatString(excludeCachesThr, sv.osFlavor));
        super.print(out);
    }

    private void populateSeedValues() {
        int cacheCount = 0;

        // count the number of non-excluded disks
        for(Map.Entry<String, CmCacheMetrics> me : cache.entrySet()) {
            String cacheName = me.getKey();
            CmCacheMetrics m = me.getValue();

            boolean include = true;
            for(String s : excludeCaches.split(",")) {
                if (s.length() == 0)
                    continue;
                if (cacheName.startsWith(s)) {
                    include = false;
                    break;
                }
            }

            if (!include)
                continue;

            cacheCount++;
        }

        // allocate arrays if necessary
        if (sv.cacheSwings == null || sv.cacheSwings.length != cacheCount)
            sv.cacheSwings = new int[cacheCount];
        if (sv.cacheUtils == null || sv.cacheUtils.length != cacheCount)
            sv.cacheUtils = new int[cacheCount];
        if (sv.cacheNames == null || sv.cacheNames.length != cacheCount)
            sv.cacheNames = new String[cacheCount];

        // populate values for supported caches
        int i = 0;
        for(Map.Entry<String, CmCacheMetrics> me : cache.entrySet()) {
            String cacheName = me.getKey();
            CmCacheMetrics m = me.getValue();

            boolean include = true;
            for(String s : excludeCaches.split(",")) {
                if (s.length() == 0)
                    continue;
                if (cacheName.startsWith(s)) {
                    include = false;
                    break;
                }
            }

            if (!include)
                continue;

            sv.cacheSwings[i] = m.hits.intValue() + m.misses.intValue();
            sv.cacheUtils[i]  = m.hitPercent.intValue();
            sv.cacheNames[i]  = cacheName;

            i++;
        }
    }

    public void fireLiveRule() throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        gcache.refresh();
        populateSeedValues();
        seedValues(sv);
        firePureRule();
    }
}
