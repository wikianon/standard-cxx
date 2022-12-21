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
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import java.util.Map;
import java.util.TreeSet;
import com.capmet.metrics.CmOSFlavor;
import com.capmet.metrics.cm.CmGlobal;
import com.capmet.metrics.cm.CmNames;
import com.capmet.metrics.cm.CmNicStat;
import com.capmet.metrics.cm.CmNicMetrics;
import com.capmet.metrics.rmi.PerformanceException;

public class LiveNICRule extends PureNICRule {

    private CmGlobal<CmNicStat>  gstat;
    private CmNicStat            stat;
    private CmGlobal<CmNames>    gnames;
    private CmNames              names;
    private PureNICSeedValues    sv;
    private Threshold<String>    excludeListThr;
    private String               excludeList;

    public LiveNICRule() throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        this("localhost");
    }

    public LiveNICRule(String remoteHost) throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        gstat = CmGlobal.getNicStat(remoteHost);
        stat = gstat.getObject();

        gnames = CmGlobal.getNames(remoteHost);
        names = gnames.getObject();

        sv = new PureNICSeedValues();
        sv.osFlavor = stat.getOSFlavor();

        // read exclude list from properties
        excludeListThr = new Threshold<String>("NIC_EXCLUSION_LIST", "");
        excludeList = getThreshString(excludeListThr, sv.osFlavor);

        populateSeedValues();

        seedValues(sv);
    }

    public void print(PrintStream out) {
        out.println(formatString(excludeListThr, sv.osFlavor));
        super.print(out);
    }

    private void populateSeedValues() {
        int ifCount = 0;
        String sampleIfName = null;

        // count the number of non-excluded interfaces
        for(String ifName : stat.keySet()) {
            boolean include = true;

            for(String s : excludeList.split(",")) {
                if (s.length() == 0)
                    continue;
                if (ifName.startsWith(s)) {
                    include = false;
                    break;
                }
            }

            if (!include)
                continue;

            ifCount++;

            if (sampleIfName == null)
                sampleIfName = ifName;
        }

        // get a node to check for supported fields
        CmNicMetrics n = stat.get(sampleIfName);

        // allocate the arrays if necessary
        if (sv.errors == null || sv.errors.length != ifCount)
            sv.errors = new double[ifCount];
        if (sv.failures == null || sv.failures.length != ifCount)
            sv.failures = new double[ifCount];
        if (sv.ifNames == null || sv.ifNames.length != ifCount)
            sv.ifNames = new String[ifCount];

        // set supported values
        if (n.collisionPercent.isSupported()) {
            if (sv.colls == null || sv.colls.length != ifCount)
                sv.colls = new int[ifCount];
            sv.hasCollisionPercent = true;
        } else {
            sv.colls = null;
            sv.hasCollisionPercent = false;
        }

        if (n.utilizationPercent.isSupported()) {
            if (sv.utils == null || sv.utils.length != ifCount)
                sv.utils = new int[ifCount];
            sv.hasUtilizationPercent = true;
        } else {
            sv.utils = null;
            sv.hasUtilizationPercent = false;
        }

        // what time is it?
        sv.currentTime = System.currentTimeMillis();

        // populate values for supported interfaces
        int i = 0;
        for(Map.Entry<String, CmNicMetrics> e : stat.entrySet()) {
            String ifName = e.getKey();
            CmNicMetrics nm = e.getValue();
            boolean include = true;

            for(String s : excludeList.split(",")) {
                if (s.length() == 0)
                    continue;
                if (ifName.startsWith(s)) {
                    include = false;
                    break;
                }
            }

            if (!include)
                continue;

            sv.ifNames[i] = ifName;
            sv.errors[i] = nm.inputErrors.doubleValue() +
                        nm.outputErrors.doubleValue();
            sv.failures[i] = nm.inputFailures.doubleValue() +
                          nm.outputFailures.doubleValue();
            if (sv.hasCollisionPercent)
                sv.colls[i] = nm.collisionPercent.intValue();
            if (sv.hasUtilizationPercent)
                sv.utils[i] = nm.utilizationPercent.intValue();
            i++;
        }
    }

    public void fireLiveRule() throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        gstat.refresh();
        populateSeedValues();
        seedValues(sv);
        firePureRule();
    }
}
