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

import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import com.capmet.metrics.CmOSFlavor;
import com.capmet.metrics.cm.CmGlobal;
import com.capmet.metrics.cm.CmMpStat;
import com.capmet.metrics.cm.CmMpMetrics;
import com.capmet.metrics.cm.CmSysStat;
import com.capmet.metrics.cm.CmSysInfo;
import com.capmet.metrics.rmi.PerformanceField;
import com.capmet.metrics.rmi.PerformanceCategory;
import com.capmet.metrics.rmi.PerformanceException;

public class LiveCPURule extends PureCPURule {

    private CmGlobal<CmSysStat> gsys;
    private CmSysStat           sys;
    private CmGlobal<CmMpStat>  gmp;
    private CmMpStat            mp;
    private PureCPUSeedValues   sv;
    private PerformanceField[]  mux = null;
    private long                oldMutexSpins = 0;

    public LiveCPURule() throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        this("localhost");
    }

    public LiveCPURule(String remoteHost) throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        CmGlobal<CmSysInfo> ginfo = CmGlobal.getSysInfo(remoteHost);
        CmSysInfo info = ginfo.getObject();

        gsys = CmGlobal.getSysStat(remoteHost);
        sys = gsys.getObject();

        gmp = CmGlobal.getMpStat(remoteHost);
        mp = gmp.getObject();

        sv = new PureCPUSeedValues();
        sv.currentTime = System.currentTimeMillis();
        sv.osFlavor = sys.getOSFlavor();
        sv.ncpus = info.cpuCountOnline.intValue();
        sv.cpuPct = new int[sv.ncpus];
        sv.clockMHz = info.cpuClockSpeed.intValue();

        if (sv.osFlavor == CmOSFlavor.MOS_SOLARIS) {
            PerformanceCategory sun = new PerformanceCategory(
                "SunOS Per-CPU Data", remoteHost);
            if (sun.isCategorySupported()) {
                String[] cpuNames = sun.getInstanceNames();

                mux = new PerformanceField[cpuNames.length];
                for(int i=0; i<cpuNames.length; i++)
                    mux[i] = sun.getField(
                        "failed mutex enters (adaptive)", cpuNames[i]);
            }
        }

        // at least get osFlavor in there
        seedValues(sv);
    }

    public void fireLiveRule() throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        int count = 0;

        gsys.refresh();
        gmp.refresh();

        sv.currentTime = System.currentTimeMillis();
        sv.usrPct = sys.userTimePercent.intValue();
        sv.sysPct = sys.systemTimePercent.intValue();
        sv.waitPct = sys.waitTimePercent.intValue();
        sv.systemCalls = sys.systemCalls.intValue();
        sv.contextSwitches = sys.contextSwitches.intValue();
        if (mux != null) {
            sv.mutexSpins = 0;
            for(int i=0; i<mux.length; i++) {
                Number v = (Number) mux[i].getNextValue();

                sv.mutexSpins += v.intValue();
            }
            long ms = sv.mutexSpins;
            sv.mutexSpins -= oldMutexSpins;
            oldMutexSpins = ms;
        }
        sv.interrupts = sys.interrupts.intValue();
        sv.runnableProcesses = sys.runnableProcesses.intValue();
        for(CmMpMetrics m : mp.values())
            sv.cpuPct[count++] = m.userTimePercent.intValue() +
                                 m.systemTimePercent.intValue();

        seedValues(sv);
        firePureRule();
    }
}
