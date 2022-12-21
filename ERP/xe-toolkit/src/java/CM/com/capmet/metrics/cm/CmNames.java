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

import java.util.Locale;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import com.capmet.metrics.CmOSFlavor;
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.PerformanceException;

/**
 * A collection of localized category names.
 */
public class CmNames implements CmCategory
{
    public String UnixCaches     = "Caches";
    public String UnixCpus       = "CPU Utilization Data";
    public String UnixCpuInfo    = "CPU Information";
    public String UnixCpuStat    = "Global CPU Metrics";
    public String UnixCtlrs      = "Controllers";
    public String UnixCtlrStat   = "Controller Metrics";
    public String UnixDisks      = "Disks";
    public String UnixICMP       = "ICMP Protocol";
    public String UnixDiskStat   = "Disk Metrics";
    public String UnixIP         = "IP Protocol";
    public String UnixMemory     = "Memory Data";
    public String UnixMisc       = "Miscellaneous Data";
    public String UnixMounts     = "Mount Points";
    public String UnixMp         = "Per-CPU Data";
    public String UnixMpStat     = "Per-CPU Metrics";
    public String UnixNetif      = "Network Interfaces";
    public String UnixPageFiles  = "Paging Files";
    public String UnixProcesses  = "Processes";
    public String UnixSysStat    = "System Metrics";
    public String UnixTCP        = "TCP Protocol";
    public String UnixThreads    = "Threads";
    public String UnixUDP        = "UDP Protocol";
    public String UnixProcStates = "Process States";
    public String UnixNetStat    = "Interface Metrics";

    public String WindowsDnClrExc       = ".NET CLR Exceptions";
    public String WindowsDnClrIop       = ".NET CLR Interop";
    public String WindowsDnClrJit       = ".NET CLR Jit";
    public String WindowsDnClrLoad      = ".NET CLR Loading";
    public String WindowsDnClrLnT       = ".NET CLR LocksAndThreads";
    public String WindowsDnClrMem       = ".NET CLR Memory";
    public String WindowsDnClrRemote    = ".NET CLR Remoting";
    public String WindowsDnClrSecurity  = ".NET CLR Security";
    public String WindowsAspDn          = "ASP.NET";
    public String WindowsAspDnApps      = "ASP.NET Applications";
    public String WindowsAspDnSS        = "ASP.NET State Service";
    public String WindowsBattery        = "BatteryStatus";
    public String WindowsBrowser        = "Browser";
    public String WindowsCache          = "Cache";
    public String WindowsDiskInfo       = "Disk Information";
    public String WindowsDTC            = "Distributed Transaction Coordinator";
    public String WindowsICMP           = "ICMP";
    public String WindowsIP             = "IP";
    public String WindowsJobObj         = "Job Object";
    public String WindowsJobObjDetails  = "Job Object Details";
    public String WindowsLogicDisk      = "LogicalDisk";
    public String WindowsMemory         = "Memory";
    public String WindowsNbtConn        = "NBT Connection";
    public String WindowsNetif          = "Network Interface";
    public String WindowsNICInfo        = "NIC Information";
    public String WindowsObjs           = "Objects";
    public String WindowsPsPipe         = "PSched Pipe";
    public String WindowsPageFiles      = "Paging File";
    public String WindowsPhysDisk       = "PhysicalDisk";
    public String WindowsPrintQ         = "Print Queue";
    public String WindowsProcess        = "Process";
    public String WindowsProcessor      = "Processor";
    public String WindowsCpuPerf        = "ProcessorPerformance";
    public String WindowsRSVP           = "RSVP Service";
    public String WindowsRedirector     = "Redirector";
    public String WindowsServer         = "Server";
    public String WindowsServerWorkQ    = "Server Work Queues";
    public String WindowsSystem         = "System";
    public String WindowsSystemInfo     = "System Information";
    public String WindowsTCP            = "TCP";
    public String WindowsTelephony      = "Telephony";
    public String WindowsTermSvc        = "Terminal Services";
    public String WindowsTermSvcSession = "Terminal Services Session";
    public String WindowsThread         = "Thread";
    public String WindowsUDP            = "UDP";
    public String WindowsWMIObjs        = "WMI Objects";
    public String WindowsTCPMib         = "TCP MIB";

    private PerformanceHost host;
    private CmOSFlavor      osFlavor;

    /**
     * The constructor which will either leave the names as is (en_US locale)
     * or localize the names to the local host's locale.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     */
    public CmNames()
        throws NotBoundException, MalformedURLException, RemoteException
    {
        this("localhost");
    }

    /**
     * The constructor which will either leave the names as is (en_US locale)
     * or localize the names to the remote host's locale.
     * @param remoteHost The name of the remote host to determine locale.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     */
    public CmNames(String remoteHost)
        throws NotBoundException, MalformedURLException, RemoteException
    {
        host = new PerformanceHost(remoteHost);

        Locale locale = host.getLocale();

        osFlavor = host.getOSFlavor();

        // all is as it should be given this case
        if (locale.equals(Locale.US))
            return;

        if (osFlavor.isUnixHost())
        {
            UnixCaches     = host.getLocalizedName(UnixCaches);
            UnixCpus       = host.getLocalizedName(UnixCpus);
            UnixCpuInfo    = host.getLocalizedName(UnixCpuInfo);
            UnixCpuStat    = host.getLocalizedName(UnixCpuStat);
            UnixCtlrs      = host.getLocalizedName(UnixCtlrs);
            UnixCtlrStat   = host.getLocalizedName(UnixCtlrStat);
            UnixDisks      = host.getLocalizedName(UnixDisks);
            UnixICMP       = host.getLocalizedName(UnixICMP);
            UnixDiskStat   = host.getLocalizedName(UnixDiskStat);
            UnixIP         = host.getLocalizedName(UnixIP);
            UnixMemory     = host.getLocalizedName(UnixMemory);
            UnixMisc       = host.getLocalizedName(UnixMisc);
            UnixMounts     = host.getLocalizedName(UnixMounts);
            UnixMp         = host.getLocalizedName(UnixMp);
            UnixMpStat     = host.getLocalizedName(UnixMpStat);
            UnixNetif      = host.getLocalizedName(UnixNetif);
            UnixPageFiles  = host.getLocalizedName(UnixPageFiles);
            UnixProcesses  = host.getLocalizedName(UnixProcesses);
            UnixSysStat    = host.getLocalizedName(UnixSysStat);
            UnixTCP        = host.getLocalizedName(UnixTCP);
            UnixThreads    = host.getLocalizedName(UnixThreads);
            UnixUDP        = host.getLocalizedName(UnixUDP);
            UnixProcStates = host.getLocalizedName(UnixProcStates);
            UnixNetStat    = host.getLocalizedName(UnixNetStat);
        }
        else if (osFlavor.isWindowsHost())
        {
            WindowsDnClrExc       = host.getLocalizedName(WindowsDnClrExc);
            WindowsDnClrIop       = host.getLocalizedName(WindowsDnClrIop);
            WindowsDnClrJit       = host.getLocalizedName(WindowsDnClrJit);
            WindowsDnClrLoad      = host.getLocalizedName(WindowsDnClrLoad);
            WindowsDnClrLnT       = host.getLocalizedName(WindowsDnClrLnT);
            WindowsDnClrMem       = host.getLocalizedName(WindowsDnClrMem);
            WindowsDnClrRemote    = host.getLocalizedName(WindowsDnClrRemote);
            WindowsDnClrSecurity  = host.getLocalizedName(WindowsDnClrSecurity);
            WindowsAspDn          = host.getLocalizedName(WindowsAspDn);
            WindowsAspDnApps      = host.getLocalizedName(WindowsAspDnApps);
            WindowsAspDnSS        = host.getLocalizedName(WindowsAspDnSS);
            WindowsBattery        = host.getLocalizedName(WindowsBattery);
            WindowsBrowser        = host.getLocalizedName(WindowsBrowser);
            WindowsCache          = host.getLocalizedName(WindowsCache);
            WindowsDiskInfo       = host.getLocalizedName(WindowsDiskInfo);
            WindowsDTC            = host.getLocalizedName(WindowsDTC);
            WindowsICMP           = host.getLocalizedName(WindowsICMP);
            WindowsIP             = host.getLocalizedName(WindowsIP);
            WindowsJobObj         = host.getLocalizedName(WindowsJobObj);
            WindowsJobObjDetails  = host.getLocalizedName(WindowsJobObjDetails);
            WindowsLogicDisk      = host.getLocalizedName(WindowsLogicDisk);
            WindowsMemory         = host.getLocalizedName(WindowsMemory);
            WindowsNbtConn        = host.getLocalizedName(WindowsNbtConn);
            WindowsNetif          = host.getLocalizedName(WindowsNetif);
            WindowsNICInfo        = host.getLocalizedName(WindowsNICInfo);
            WindowsObjs           = host.getLocalizedName(WindowsObjs);
            WindowsPsPipe         = host.getLocalizedName(WindowsPsPipe);
            WindowsPageFiles      = host.getLocalizedName(WindowsPageFiles);
            WindowsPhysDisk       = host.getLocalizedName(WindowsPhysDisk);
            WindowsPrintQ         = host.getLocalizedName(WindowsPrintQ);
            WindowsProcess        = host.getLocalizedName(WindowsProcess);
            WindowsProcessor      = host.getLocalizedName(WindowsProcessor);
            WindowsCpuPerf        = host.getLocalizedName(WindowsCpuPerf);
            WindowsRSVP           = host.getLocalizedName(WindowsRSVP);
            WindowsRedirector     = host.getLocalizedName(WindowsRedirector);
            WindowsServer         = host.getLocalizedName(WindowsServer);
            WindowsServerWorkQ    = host.getLocalizedName(WindowsServerWorkQ);
            WindowsSystem         = host.getLocalizedName(WindowsSystem);
            WindowsSystemInfo     = host.getLocalizedName(WindowsSystemInfo);
            WindowsTCP            = host.getLocalizedName(WindowsTCP);
            WindowsTelephony      = host.getLocalizedName(WindowsTelephony);
            WindowsTermSvc        = host.getLocalizedName(WindowsTermSvc);
            WindowsTermSvcSession = host.getLocalizedName(WindowsTermSvcSession);
            WindowsThread         = host.getLocalizedName(WindowsThread);
            WindowsUDP            = host.getLocalizedName(WindowsUDP);
            WindowsWMIObjs        = host.getLocalizedName(WindowsWMIObjs);
            WindowsTCPMib         = host.getLocalizedName(WindowsTCPMib);
        }
    }

    /**
     * Stub refresh for satisfaction of CmCategory interface.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public void refresh()
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        // no refresh here, but needed for CmCategory
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
