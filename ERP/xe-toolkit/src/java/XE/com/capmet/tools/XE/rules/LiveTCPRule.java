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
import com.capmet.metrics.cm.CmTcpStat;
import com.capmet.metrics.rmi.PerformanceException;

public class LiveTCPRule extends PureTCPRule {

    private CmGlobal<CmTcpStat> gtcp;
    private CmTcpStat           tcp;
    private PureTCPSeedValues   sv;

    public LiveTCPRule() throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        this("localhost");
    }

    public LiveTCPRule(String remoteHost) throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        gtcp = CmGlobal.getTcpStat(remoteHost);
        tcp = gtcp.getObject();

        sv = new PureTCPSeedValues();
        sv.osFlavor = tcp.getOSFlavor();
        sv.outputSegs = tcp.tcpOutSegs.intValue();
        sv.retransPercent = tcp.tcpRetransPercent.intValue();
        sv.attemptFails = tcp.tcpAttemptFails.intValue();
        sv.inputSegs = tcp.tcpInSegs.intValue();
        sv.inputErrorsPercent = tcp.tcpInErrsPercent.intValue();
        sv.rstPercent = tcp.tcpOutRstsPercent.intValue();

        // at least get osFlavor in there
        seedValues(sv);
    }

    public void fireLiveRule() throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException {

        gtcp.refresh();

        sv.osFlavor = tcp.getOSFlavor();
        sv.outputSegs = tcp.tcpOutSegs.intValue();
        sv.retransPercent = tcp.tcpRetransPercent.intValue();
        sv.attemptFails = tcp.tcpAttemptFails.intValue();
        sv.inputSegs = tcp.tcpInSegs.intValue();
        sv.inputErrorsPercent = tcp.tcpInErrsPercent.intValue();

        seedValues(sv);
        firePureRule();
    }
}
