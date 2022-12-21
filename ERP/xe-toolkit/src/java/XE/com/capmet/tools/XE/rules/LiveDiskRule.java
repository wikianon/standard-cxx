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
import com.capmet.metrics.cm.CmIoStat;
import com.capmet.metrics.cm.CmIoMetrics;
import com.capmet.metrics.rmi.PerformanceException;

public class LiveDiskRule extends PureDiskRule {

    private CmGlobal<CmIoStat> gio;
    private CmIoStat           io;
    private PureDiskSeedValues sv;
    private Threshold<String>  excludeDisksThr;
    private String             excludeDisks;

    public LiveDiskRule() throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        this("localhost");
    }

    public LiveDiskRule(String remoteHost) throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        gio = CmGlobal.getIoStat(remoteHost);
        io = gio.getObject();

        CmOSFlavor osFlavor = io.getOSFlavor();

        excludeDisksThr = new Threshold<String>("DISK_EXCLUSION_LIST", "");
        excludeDisks = getThreshString(excludeDisksThr, osFlavor);

        sv = new PureDiskSeedValues();
        sv.osFlavor = osFlavor;

        populateSeedValues();
        seedValues(sv);
    }

    public void print(PrintStream out) {
        out.println(formatString(excludeDisksThr, sv.osFlavor));
        super.print(out);
    }

    private void populateSeedValues() {
        int diskCount = 0;

        // count the number of non-excluded disks
        for(Map.Entry<String, CmIoMetrics> me : io.entrySet()) {
            CmIoMetrics m = me.getValue();
            String diskName = me.getKey();

            if (!m.diskType.equals("physical"))
                continue;

            boolean include = true;
            for(String s : excludeDisks.split(",")) {
                if (s.length() == 0)
                    continue;
                if (diskName.startsWith(s)) {
                    include = false;
                    break;
                }
            }

            if (!include)
                continue;

            diskCount++;
        }

        // allocate arrays if necessary
        if (sv.busyTime == null || sv.busyTime.length != diskCount)
            sv.busyTime = new double[diskCount];
        if (sv.serviceTime == null || sv.serviceTime.length != diskCount)
            sv.serviceTime = new double[diskCount];
        if (sv.diskNames == null || sv.diskNames.length != diskCount)
            sv.diskNames = new String[diskCount];

        // populate values for supported interfaces
        int i = 0;
        for(Map.Entry<String, CmIoMetrics> me : io.entrySet()) {
            CmIoMetrics m = me.getValue();
            String diskName = me.getKey();

            if (!m.diskType.equals("physical"))
                continue;

            boolean include = true;
            for(String s : excludeDisks.split(",")) {
                if (s.length() == 0)
                    continue;
                if (diskName.startsWith(s)) {
                    include = false;
                    break;
                }
            }

            if (!include)
                continue;

            sv.busyTime[i] = m.utilizationPercent.doubleValue();
            sv.serviceTime[i] = m.averageAccessTime.doubleValue();
            sv.diskNames[i] = diskName;

            i++;
        }
    }

    public void fireLiveRule() throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        gio.refresh();

        populateSeedValues();
        seedValues(sv);
        firePureRule();
    }
}
