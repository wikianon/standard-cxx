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
import com.capmet.metrics.*;
import com.capmet.metrics.cm.CmNames;
import com.capmet.metrics.rmi.CollectionItem;
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.PerformanceCategory;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.rmi.PerformanceCollection;

/**
 * A singleton class that presents information about the TCP stack.
 */
public class CmTcpStat implements CmCategory
{
    /**
     * Opens initiated by this entity per second.
     */
    public CmInteger tcpActiveOpens;
    /**
     * Opens received by this entity per second.
     */
    public CmInteger tcpPassiveOpens;
    /**
     * Passive open failures per second.
     */
    public CmInteger tcpAttemptFails;
    /**
     * Connection resets per second.
     */
    public CmInteger tcpEstabResets;
    /**
     * Newly established connections per second.
     */
    public CmInteger tcpCurrEstab;
    /**
     * Segments received per second.
     */
    public CmInteger tcpInSegs;
    /**
     * Segments transmitted per second.
     */
    public CmInteger tcpOutSegs;
    /**
     * Segments retransmitted per second.
     */
    public CmInteger tcpRetransSegs;
    /**
     * Percentage of output segments that required retransmission.
     */
    public CmInteger tcpRetransPercent;
    /**
     * Segments received with error per second.
     */
    public CmInteger tcpInErrs;
    /**
     * Percentage of input segments that were in error.
     */
    public CmInteger tcpInErrsPercent;
    /**
     * RST segments transmitted per second.
     */
    public CmInteger tcpOutRsts;
    /**
     * Percentage of output segments with the RST bit on.
     */
    public CmInteger tcpOutRstsPercent;

    // private members in support of the class
    private PerformanceHost       host;
    private CmOSFlavor            osFlavor;
    private PerformanceCollection collection;
    private PerformanceCategory   tcp;

    private CollectionItem tcpActiveOpensItem;
    private CollectionItem tcpPassiveOpensItem;
    private CollectionItem tcpAttemptFailsItem;
    private CollectionItem tcpEstabResetsItem;
    private CollectionItem tcpCurrEstabItem;
    private CollectionItem tcpInSegsItem;
    private CollectionItem tcpOutSegsItem;
    private CollectionItem tcpRetransSegsItem;
    private CollectionItem tcpInErrsItem;
    private CollectionItem tcpOutRstsItem;

    private long lastTime;

    /**
     * Default constructor for looking at tcp information on the local host.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmTcpStat()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        this("localhost");
    }

    /**
     * Constructor for looking at tcp information on a remote host.
     * @param remoteHost The name of the host to monitor.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmTcpStat(String remoteHost)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmNames> gnames = CmGlobal.getNames(remoteHost);
        CmNames names = gnames.getObject();
        int i;

        host = new PerformanceHost(remoteHost);
        osFlavor = host.getOSFlavor();
        collection = new PerformanceCollection(remoteHost);

        String tcpCat = null;

        if (osFlavor.isUnixHost())
            tcpCat = names.UnixTCP;
        else if (osFlavor.isWindowsHost())
            tcpCat = names.WindowsTCPMib;

        tcp = host.getCategory(tcpCat);

        String tcpActiveOpensName  = host.getLocalizedName("Active Opens");
        String tcpPassiveOpensName = host.getLocalizedName("Passive Opens");
        String tcpAttemptFailsName = host.getLocalizedName("Passive Open Failures");
        String tcpEstabResetsName  = host.getLocalizedName("Connection Resets");
        String tcpCurrEstabName    = host.getLocalizedName("Established Connections");
        String tcpInSegsName       = host.getLocalizedName("Segments Received");
        String tcpOutSegsName      = host.getLocalizedName("Segments Transmitted");
        String tcpRetransSegsName  = host.getLocalizedName("Segments Retransmitted");
        String tcpInErrsName       = host.getLocalizedName("Segments Received mith Error");
        String tcpOutRstsName      = host.getLocalizedName("RST Segments Transmitted");

        tcpActiveOpensItem  = collection.put(tcpCat, tcpActiveOpensName);
        tcpPassiveOpensItem = collection.put(tcpCat, tcpPassiveOpensName);
        tcpAttemptFailsItem = collection.put(tcpCat, tcpAttemptFailsName);
        tcpEstabResetsItem  = collection.put(tcpCat, tcpEstabResetsName);
        tcpCurrEstabItem = null;
        if (tcp.isFieldSupported(tcp.getFieldNumber(tcpCurrEstabName)))
            tcpCurrEstabItem = collection.put(tcpCat, tcpCurrEstabName);
        tcpInSegsItem       = collection.put(tcpCat, tcpInSegsName);
        tcpOutSegsItem      = collection.put(tcpCat, tcpOutSegsName);
        tcpRetransSegsItem  = collection.put(tcpCat, tcpRetransSegsName);
        tcpInErrsItem       = collection.put(tcpCat, tcpInErrsName);
        tcpOutRstsItem      = collection.put(tcpCat, tcpOutRstsName);

        collection.queryCollection();

        CmSysInfo si = (CmGlobal.getSysInfo(remoteHost)).getObject();
        int elapsed = si.systemUpTime.intValue();
        si = null;

        tcpActiveOpens  = new CmInteger(
            ((Number) tcpActiveOpensItem.value).intValue() / elapsed,
            tcpActiveOpensItem.supported);
        tcpPassiveOpens = new CmInteger(
            ((Number) tcpPassiveOpensItem.value).intValue() / elapsed,
            tcpPassiveOpensItem.supported);
        tcpAttemptFails = new CmInteger(
            ((Number) tcpAttemptFailsItem.value).intValue() / elapsed,
            tcpAttemptFailsItem.supported);
        tcpEstabResets  = new CmInteger(
            ((Number) tcpEstabResetsItem.value).intValue() / elapsed,
            tcpEstabResetsItem.supported);
        if (tcpCurrEstabItem != null)
            tcpCurrEstab    = new CmInteger(
                ((Number) tcpCurrEstabItem.value).intValue() / elapsed,
                tcpCurrEstabItem.supported);
        else
            tcpCurrEstab    = new CmInteger(false);
        tcpInSegs       = new CmInteger(
            ((Number) tcpInSegsItem.value).intValue() / elapsed,
            tcpInSegsItem.supported);
        tcpOutSegs      = new CmInteger(
            ((Number) tcpOutSegsItem.value).intValue() / elapsed,
            tcpOutSegsItem.supported);
        tcpRetransSegs  = new CmInteger(
            ((Number) tcpRetransSegsItem.value).intValue() / elapsed,
            tcpRetransSegsItem.supported);
        tcpRetransPercent = new CmInteger(
            segmentPercent(tcpRetransSegs.intValue(), tcpOutSegs.intValue()),
            tcpOutSegsItem.supported && tcpRetransSegsItem.supported);
        tcpInErrs       = new CmInteger(
            ((Number) tcpInErrsItem.value).intValue() / elapsed,
            tcpInErrsItem.supported);
        tcpInErrsPercent = new CmInteger(
            segmentPercent(tcpInErrs.intValue(), tcpInSegs.intValue()),
            tcpInSegsItem.supported && tcpInErrsItem.supported);
        tcpOutRsts      = new CmInteger(
            ((Number) tcpOutRstsItem.value).intValue() / elapsed,
            tcpOutRstsItem.supported);
        tcpOutRstsPercent = new CmInteger(
            segmentPercent(tcpOutRsts.intValue(), tcpOutSegs.intValue()),
            tcpOutSegsItem.supported && tcpOutRstsItem.supported);

        lastTime = System.currentTimeMillis();
    }

    /**
     * Refresh the system information.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public void refresh()
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        long now = System.currentTimeMillis();
        long elapsed = now - lastTime;

        // close enough for fudge
        if (elapsed < 900)
            return;

        // between 900 and 1000
        if (elapsed < 1000)
            elapsed = 1000;

        elapsed /= 1000;

        lastTime = now;

        // refresh PerformanceCategory object
        tcp.categoryRefreshRates();

        // query the collection
        collection.queryCollection();

        // assign values
        int tmpActiveOpens = ((Number) tcpActiveOpensItem.value).intValue();
        int tmpPassiveOpens = ((Number) tcpPassiveOpensItem.value).intValue();
        int tmpAttemptFails = ((Number) tcpAttemptFailsItem.value).intValue();
        int tmpEstabResets = ((Number) tcpEstabResetsItem.value).intValue();
        int tmpCurrEstab = 0;
        if (tcpCurrEstabItem != null)
            tmpCurrEstab = ((Number) tcpCurrEstabItem.value).intValue();
        int tmpInSegs = ((Number) tcpInSegsItem.value).intValue();
        int tmpOutSegs = ((Number) tcpOutSegsItem.value).intValue();
        int tmpRetransSegs = ((Number) tcpRetransSegsItem.value).intValue();
        int tmpInErrs = ((Number) tcpInErrsItem.value).intValue();
        int tmpOutRsts = ((Number) tcpOutRstsItem.value).intValue();

        tcpActiveOpens.setValue(tmpActiveOpens / (int) elapsed);
        tcpPassiveOpens.setValue(tmpPassiveOpens / (int) elapsed);
        tcpAttemptFails.setValue(tmpAttemptFails / (int) elapsed);
        tcpEstabResets.setValue(tmpEstabResets / (int) elapsed);
        tcpCurrEstab.setValue(tmpCurrEstab / (int) elapsed);
        tcpInSegs.setValue(tmpInSegs / (int) elapsed);
        tcpOutSegs.setValue(tmpOutSegs / (int) elapsed);
        tcpRetransSegs.setValue(tmpRetransSegs / (int) elapsed);
        tcpRetransPercent.setValue(segmentPercent(tmpRetransSegs, tmpOutSegs));
        tcpInErrs.setValue(tmpInErrs / (int) elapsed);
        tcpInErrsPercent.setValue(segmentPercent(tmpInErrs, tmpInSegs));
        tcpOutRsts.setValue(tmpOutRsts / (int) elapsed);
        tcpOutRstsPercent.setValue(segmentPercent(tmpOutRsts, tmpOutSegs));
    }

    private int segmentPercent(int piece, int total) {
        if (total == 0)
            return 0;

        return (piece * 100) / total;
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
