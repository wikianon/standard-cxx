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
import java.security.AccessControlException;
import java.util.TreeMap;
import com.capmet.metrics.rmi.PerformanceException;

/**
 * A public place for the Cm classes to congregate so that
 * applications will not use copious amounts of resources by
 * redefining the Cm classes over and over.
 */
public final class CmGlobal<GlobalType extends CmCategory>
{
    // private members in support of the class
    private GlobalType globalObject;
    private long       globalTime;

    // a veritable forest
    private static TreeMap<String, CmGlobal<CmCpuStat>>   cpustat;
    private static TreeMap<String, CmGlobal<CmIoStat>>    iostat;
    private static TreeMap<String, CmGlobal<CmMpStat>>    mpstat;
    private static TreeMap<String, CmGlobal<CmNicStat>>   nicstat;
    private static TreeMap<String, CmGlobal<CmSysInfo>>   sysinfo;
    private static TreeMap<String, CmGlobal<CmProcesses>> procs;
    private static TreeMap<String, CmGlobal<CmPageFiles>> pagefiles;
    private static TreeMap<String, CmGlobal<CmCaches>>    caches;
    private static TreeMap<String, CmGlobal<CmNames>>     names;
    private static TreeMap<String, CmGlobal<CmMounts>>    mounts;
    private static TreeMap<String, CmGlobal<CmSysStat>>   sysstat;
    private static TreeMap<String, CmGlobal<CmTcpStat>>   tcpstat;

    static
    {
        cpustat   = new TreeMap<String, CmGlobal<CmCpuStat>>();
        iostat    = new TreeMap<String, CmGlobal<CmIoStat>>();
        mpstat    = new TreeMap<String, CmGlobal<CmMpStat>>();
        nicstat   = new TreeMap<String, CmGlobal<CmNicStat>>();
        sysinfo   = new TreeMap<String, CmGlobal<CmSysInfo>>();
        procs     = new TreeMap<String, CmGlobal<CmProcesses>>();
        pagefiles = new TreeMap<String, CmGlobal<CmPageFiles>>();
        caches    = new TreeMap<String, CmGlobal<CmCaches>>();
        names     = new TreeMap<String, CmGlobal<CmNames>>();
        mounts    = new TreeMap<String, CmGlobal<CmMounts>>();
        sysstat   = new TreeMap<String, CmGlobal<CmSysStat>>();
        tcpstat   = new TreeMap<String, CmGlobal<CmTcpStat>>();
    }

    // private constructor: the get<GlobalType> functions will new
    // the class and return the value.

    private CmGlobal(GlobalType o)
    {
        globalObject = o;
        globalTime = System.currentTimeMillis();
    }

    /**
     * Return the object of the type specified as the type parameter of the class.
     * @return The allocated object.
     */
    public GlobalType getObject()
    {
        return globalObject;
    }

    /**
     * Refresh the metrics of the underlying object. The refresh has a
     * throttle value controlled by the environment variable MDR_INTERVAL_THRESHOLD
     * which is the number of milliseconds to wait between refreshes. If the
     * environment variable does not exist, the default is one second between
     * refreshes.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     */
    public synchronized void refresh()
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        long now = System.currentTimeMillis();
        String threshEnv = null;
        long threshold = 1000;

        try {
            threshEnv = System.getenv("MDR_INTERVAL_THRESHOLD");
        } catch(AccessControlException e) {
            // applet
        }

        if (threshEnv != null)
            threshold = Long.parseLong(threshEnv);

        if ((now - globalTime) >= threshold)
        {
            globalTime = now;
            globalObject.refresh();
        }
    }

    /**
     * The static maker of <CODE>CmCpuStat</CODE> global objects for the local host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmCpuStat&gt;</CODE> object.
     */
    public static CmGlobal<CmCpuStat> getCpuStat()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        return getCpuStat("localhost");
    }

    /**
     * The static maker of <CODE>CmCpuStat</CODE> global objects for remote hosts.
     * @param host The remote host from which to gather metrics.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmCpuStat&gt;</CODE> object.
     */
    public static synchronized CmGlobal<CmCpuStat> getCpuStat(String host)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmCpuStat> cg = null;

        if (host == null)
            host = "localhost";

        if (cpustat.containsKey(host)) {
            cg = cpustat.get(host);
            CmCpuStat cs = cg.getObject();
            if (!cs.hasValidConnection()) {
                cpustat.remove(host);
                cg = null;
            }
        }

        if (cg == null) {
            CmCpuStat cs = new CmCpuStat(host);

            cg = new CmGlobal<CmCpuStat>(cs);
            cpustat.put(host, cg);
        }

        return cg;
    }


    /**
     * The static maker of <CODE>CmIoStat</CODE> global objects for the local host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmIoStat&gt;</CODE> object.
     */
    public static CmGlobal<CmIoStat> getIoStat()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        return getIoStat("localhost");
    }

    /**
     * The static maker of <CODE>CmIoStat</CODE> global objects for remote hosts.
     * @param host The remote host from which to gather metrics.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmIoStat&gt;</CODE> object.
     */
    public static synchronized CmGlobal<CmIoStat> getIoStat(String host)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmIoStat> cg = null;

        if (host == null)
            host = "localhost";

        if (iostat.containsKey(host)) {
            cg = iostat.get(host);
            CmIoStat io = cg.getObject();
            if (!io.hasValidConnection()) {
                iostat.remove(host);
                cg = null;
            }
        }

        if (cg == null) {
            CmIoStat io = new CmIoStat(host);

            cg = new CmGlobal<CmIoStat>(io);
            iostat.put(host, cg);
        }

        return cg;
    }

    /**
     * The static maker of <CODE>CmMpStat</CODE> global objects for the local host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmMpStat&gt;</CODE> object.
     */
    public static CmGlobal<CmMpStat> getMpStat()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        return getMpStat("localhost");
    }

    /**
     * The static maker of <CODE>CmMpStat</CODE> global objects for remote hosts.
     * @param host The remote host from which to gather metrics.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmMpStat&gt;</CODE> object.
     */
    public static synchronized CmGlobal<CmMpStat> getMpStat(String host)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmMpStat> cg = null;

        if (host == null)
            host = "localhost";

        if (mpstat.containsKey(host)) {
            cg = mpstat.get(host);
            CmMpStat mp = cg.getObject();
            if (!mp.hasValidConnection()) {
                mpstat.remove(host);
                cg = null;
            }
        }

        if (cg == null) {
            CmMpStat mp = new CmMpStat(host);

            cg = new CmGlobal<CmMpStat>(mp);
            mpstat.put(host, cg);
        }

        return cg;
    }

    /**
     * The static maker of <CODE>CmNicStat</CODE> global objects for the local host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmNicStat&gt;</CODE> object.
     */
    public static CmGlobal<CmNicStat> getNicStat()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        return getNicStat("localhost");
    }

    /**
     * The static maker of <CODE>CmNicStat</CODE> global objects for remote hosts.
     * @param host The remote host from which to gather metrics.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmNicStat&gt;</CODE> object.
     */
    public static synchronized CmGlobal<CmNicStat> getNicStat(String host)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmNicStat> cg = null;

        if (host == null)
            host = "localhost";

        if (nicstat.containsKey(host)) {
            cg = nicstat.get(host);
            CmNicStat nic = cg.getObject();
            if (!nic.hasValidConnection()) {
                nicstat.remove(host);
                cg = null;
            }
        }

        if (cg == null) {
            CmNicStat nic = new CmNicStat(host);

            cg = new CmGlobal<CmNicStat>(nic);
            nicstat.put(host, cg);
        }

        return cg;
    }

    /**
     * The static maker of <CODE>CmSysInfo</CODE> global objects for the local host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmSysInfo&gt;</CODE> object.
     */
    public static CmGlobal<CmSysInfo> getSysInfo()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        return getSysInfo("localhost");
    }

    /**
     * The static maker of <CODE>CmSysInfo</CODE> global objects for remote hosts.
     * @param host The remote host from which to gather metrics.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmSysInfo&gt;</CODE> object.
     */
    public static synchronized CmGlobal<CmSysInfo> getSysInfo(String host)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmSysInfo> cg = null;

        if (host == null)
            host = "localhost";

        if (sysinfo.containsKey(host)) {
            cg = sysinfo.get(host);
            CmSysInfo si = cg.getObject();
            if (!si.hasValidConnection()) {
                sysinfo.remove(host);
                cg = null;
            }
        }

        if (cg == null) {
            CmSysInfo si = new CmSysInfo(host);

            cg = new CmGlobal<CmSysInfo>(si);
            sysinfo.put(host, cg);
        }

        return cg;
    }

    /**
     * The static maker of <CODE>CmProcesses</CODE> global objects for the local host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmProcesses&gt;</CODE> object.
     */
    public static CmGlobal<CmProcesses> getProcesses()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        return getProcesses("localhost");
    }

    /**
     * The static maker of <CODE>CmProcesses</CODE> global objects for remote hosts.
     * @param host The remote host from which to gather metrics.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmProcesses&gt;</CODE> object.
     */
    public static synchronized CmGlobal<CmProcesses> getProcesses(String host)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmProcesses> cg = null;

        if (host == null)
            host = "localhost";

        if (procs.containsKey(host)) {
            cg = procs.get(host);
            CmProcesses p = cg.getObject();
            if (!p.hasValidConnection()) {
                procs.remove(host);
                cg = null;
            }
        }

        if (cg == null) {
            CmProcesses p = new CmProcesses(host);

            cg = new CmGlobal<CmProcesses>(p);
            procs.put(host, cg);
        }

        return cg;
    }

    /**
     * The static maker of <CODE>CmPageFiles</CODE> global objects for the local host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmPageFiles&gt;</CODE> object.
     */
    public static CmGlobal<CmPageFiles> getPageFiles()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        return getPageFiles("localhost");
    }

    /**
     * The static maker of <CODE>CmPageFiles</CODE> global objects for remote hosts.
     * @param host The remote host from which to gather metrics.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmPageFiles&gt;</CODE> object.
     */
    public static synchronized CmGlobal<CmPageFiles> getPageFiles(String host)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmPageFiles> cg = null;

        if (host == null)
            host = "localhost";

        if (pagefiles.containsKey(host)) {
            cg = pagefiles.get(host);
            CmPageFiles pf = cg.getObject();
            if (!pf.hasValidConnection()) {
                pagefiles.remove(host);
                cg = null;
            }
        }

        if (cg == null) {
            CmPageFiles p = new CmPageFiles(host);

            cg = new CmGlobal<CmPageFiles>(p);
            pagefiles.put(host, cg);
        }

        return cg;
    }

    /**
     * The static maker of <CODE>CmCaches</CODE> global objects for the local host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmCaches&gt;</CODE> object.
     */
    public static CmGlobal<CmCaches> getCaches()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        return getCaches("localhost");
    }

    /**
     * The static maker of <CODE>CmCaches</CODE> global objects for remote hosts.
     * @param host The remote host from which to gather metrics.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmCaches&gt;</CODE> object.
     */
    public static synchronized CmGlobal<CmCaches> getCaches(String host)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmCaches> cg = null;

        if (host == null)
            host = "localhost";

        if (caches.containsKey(host)) {
            cg = caches.get(host);
            CmCaches c = cg.getObject();
            if (!c.hasValidConnection()) {
                caches.remove(host);
                cg = null;
            }
        }

        if (cg == null) {
            CmCaches p = new CmCaches(host);

            cg = new CmGlobal<CmCaches>(p);
            caches.put(host, cg);
        }

        return cg;
    }

    /**
     * The static maker of <CODE>CmNames</CODE> global objects for the local host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmNames&gt;</CODE> object.
     */
    public static CmGlobal<CmNames> getNames()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        return getNames("localhost");
    }

    /**
     * The static maker of <CODE>CmNames</CODE> global objects for remote hosts.
     * @param host The remote host from which to gather metrics.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmNames&gt;</CODE> object.
     */
    public static synchronized CmGlobal<CmNames> getNames(String host)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmNames> cg = null;

        if (host == null)
            host = "localhost";

        if (names.containsKey(host)) {
            cg = names.get(host);
            CmNames n = cg.getObject();
            if (!n.hasValidConnection()) {
                names.remove(host);
                cg = null;
            }
        }

        if (cg == null) {
            CmNames p = new CmNames(host);

            cg = new CmGlobal<CmNames>(p);
            names.put(host, cg);
        }

        return cg;
    }

    /**
     * The static maker of <CODE>CmMounts</CODE> global objects for the local host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmMounts&gt;</CODE> object.
     */
    public static CmGlobal<CmMounts> getMounts()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        return getMounts("localhost");
    }

    /**
     * The static maker of <CODE>CmMounts</CODE> global objects for remote hosts.
     * @param host The remote host from which to gather metrics.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmMounts&gt;</CODE> object.
     */
    public static synchronized CmGlobal<CmMounts> getMounts(String host)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmMounts> cg = null;

        if (host == null)
            host = "localhost";

        if (mounts.containsKey(host)) {
            cg = mounts.get(host);
            CmMounts m = cg.getObject();
            if (!m.hasValidConnection()) {
                mounts.remove(host);
                cg = null;
            }
        }

        if (cg == null) {
            CmMounts p = new CmMounts(host);

            cg = new CmGlobal<CmMounts>(p);
            mounts.put(host, cg);
        }

        return cg;
    }

    /**
     * The static maker of <CODE>CmSysStat</CODE> global objects for the local host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmSysStat&gt;</CODE> object.
     */
    public static CmGlobal<CmSysStat> getSysStat()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        return getSysStat("localhost");
    }

    /**
     * The static maker of <CODE>CmSysStat</CODE> global objects for remote hosts.
     * @param host The remote host from which to gather metrics.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmSysStat&gt;</CODE> object.
     */
    public static synchronized CmGlobal<CmSysStat> getSysStat(String host)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmSysStat> cg = null;

        if (host == null)
            host = "localhost";

        if (sysstat.containsKey(host)) {
            cg = sysstat.get(host);
            CmSysStat ss = cg.getObject();
            if (!ss.hasValidConnection()) {
                sysstat.remove(host);
                cg = null;
            }
        }

        if (cg == null) {
            CmSysStat p = new CmSysStat(host);

            cg = new CmGlobal<CmSysStat>(p);
            sysstat.put(host, cg);
        }

        return cg;
    }

    /**
     * The static maker of <CODE>CmTcpStat</CODE> global objects for the local host.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmSysStat&gt;</CODE> object.
     */
    public static CmGlobal<CmTcpStat> getTcpStat()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        return getTcpStat("localhost");
    }

    /**
     * The static maker of <CODE>CmTcpStat</CODE> global objects for remote hosts.
     * @param host The remote host from which to gather metrics.
     * @throws NotBoundException RMI server is not bound on the target host.
     * @throws MalformedURLException Bad URL syntax in RMI bind.
     * @throws RemoteException RMI exception.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>CmGlobal&lt;CmSysStat&gt;</CODE> object.
     */
    public static synchronized CmGlobal<CmTcpStat> getTcpStat(String host)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmTcpStat> cg = null;

        if (host == null)
            host = "localhost";

        if (tcpstat.containsKey(host)) {
            cg = tcpstat.get(host);
            CmTcpStat ts = cg.getObject();
            if (!ts.hasValidConnection()) {
                tcpstat.remove(host);
                cg = null;
            }
        }

        if (cg == null) {
            CmTcpStat p = new CmTcpStat(host);

            cg = new CmGlobal<CmTcpStat>(p);
            tcpstat.put(host, cg);
        }

        return cg;
    }
}
