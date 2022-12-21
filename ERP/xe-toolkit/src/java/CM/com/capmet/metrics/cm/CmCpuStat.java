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
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.PerformanceCategory;
import com.capmet.metrics.rmi.PerformanceCollection;
import com.capmet.metrics.rmi.CollectionItem;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.*;

/**
 * The Captive Metrics class for computing aggregate
 * CPU utilization by communicating with the CaptiveCollector over RMI.
 */
public class CmCpuStat implements CmCategory
{
    /**
     * The percentage of time spent in user time.
     */
    public CmInteger userTimePercent;
    /**
     * The percentage of time spent in system time.
     */
    public CmInteger systemTimePercent;
    /**
     * The percentage of time spent in wait time.
     */
    public CmInteger waitTimePercent;
    /**
     * The percentage of time spent in idle time.
     */
    public CmInteger idleTimePercent;
    
    /**
     * Default constructor for looking at CPU metrics on the local host.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmCpuStat()
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        this("localhost");
    }
    
    // private members in support of the class.
    private PerformanceHost     host;
    private CmOSFlavor          osFlavor;
    private CollectionItem      userCounter;
    private CollectionItem      systemCounter;
    private CollectionItem      waitCounter;
    private CollectionItem      idleCounter;
    private CollectionItem      procCounter;
    private PerformanceCategory category;
    PerformanceCollection       collection;
    
    /**
     * The constructor for monitoring a remote host.
     * @param remoteHost The name of the remote host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmCpuStat(String remoteHost) throws
        NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmNames> gnames = CmGlobal.getNames(remoteHost);
        CmNames names = gnames.getObject();

        host = new PerformanceHost(remoteHost);
        collection = new PerformanceCollection(remoteHost);
        osFlavor = host.getOSFlavor();
        
        if (osFlavor.isUnixHost())
        {
            String u = host.getLocalizedName("Percent User Time");
            String s = host.getLocalizedName("Percent Kernel Time");
            String w = host.getLocalizedName("Percent Wait Time");
            String i = host.getLocalizedName("Percent Idle Time");

            category      = host.getCategory(names.UnixCpuStat);
            userCounter   = collection.put(names.UnixCpuStat, u);
            systemCounter = collection.put(names.UnixCpuStat, s);
            waitCounter   = collection.put(names.UnixCpuStat, w);
            idleCounter   = collection.put(names.UnixCpuStat, i);
        }
        else if (osFlavor.isWindowsHost())
        {
            String u = host.getLocalizedName("% User Time");
            String p = host.getLocalizedName("% Processor Time");

            category    = host.getCategory(names.WindowsProcessor);
            userCounter = collection.put(names.WindowsProcessor, u, "_Total");
            procCounter = collection.put(names.WindowsProcessor, p, "_Total");
        } else
            throw new PerformanceException("CmCpuStat::CmCpuStat: " +
                CmMessages.msgs.getString("unknown_platform"));
        
        collection.queryCollection();

        if (osFlavor.isUnixHost())
        {
            userTimePercent   = new CmInteger(true);
            systemTimePercent = new CmInteger(true);
            waitTimePercent   = new CmInteger(waitCounter.supported);
            idleTimePercent   = new CmInteger(true);
        }
        else if (osFlavor.isWindowsHost())
        {
            userTimePercent   = new CmInteger(true);
            systemTimePercent = new CmInteger(true);
            idleTimePercent   = new CmInteger(true);
            waitTimePercent   = new CmInteger(false);
        }

        refresh(true);
    }
    
    /**
     * Refresh the CPU metrics.
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
        if (!isConstructor)
        {
            category.categoryRefresh();
            collection.queryCollection();
        }

        if (osFlavor.isUnixHost())
        {
            userTimePercent.setValue(
                ((Number) userCounter.value).intValue());
            systemTimePercent.setValue(
                ((Number) systemCounter.value).intValue());
            if (waitTimePercent.isSupported())
                waitTimePercent.setValue(
                    ((Number) waitCounter.value).intValue());
            idleTimePercent.setValue(
                ((Number) idleCounter.value).intValue());
        }
        else if (osFlavor.isWindowsHost())
        {
            Number u = (Number) userCounter.value;
            Number t = (Number) procCounter.value;
            int usr = u.intValue();         // usr is usr
            int sys = t.intValue() - usr;       // sys is total - usr
            int idl = 100 - t.intValue();   // idle is 100 - total
            int total = usr + sys + idl;

            if (total != 100)
            {
                idl += (100 - total);
                if (idl < 0)
                {
                    if (usr > sys)
                        usr += idl;
                    else
                        sys += idl;
                    idl = 0;
                }
            }

            userTimePercent.setValue(usr);
            systemTimePercent.setValue(sys);
            idleTimePercent.setValue(idl);
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
