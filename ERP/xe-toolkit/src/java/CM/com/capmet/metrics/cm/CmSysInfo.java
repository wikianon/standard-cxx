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
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.CollectionItem;
import com.capmet.metrics.*;

/**
 * A singleton class that provides miscellaneous system information.
 */
public class CmSysInfo implements CmCategory
{
    /**
     * The percentage of total page file storage represented by the 
     * amount of free page file storage.
     */
    public CmInteger pagingFileFreePercent;
    /**
     * The percentage of physical memory represented by the amount
     * of free memory available.
     */
    public CmInteger memoryFreePercent;
    /**
     * The total number of megabytes of page file storage.
     */
    public CmInteger pagingFileTotalMB;
    /**
     * The total number of megabytes of physical memory.
     */
    public CmInteger memoryTotalMB;
    /**
     * The number of interrupts delivered to the CPU from the
     * clock generator every second.
     */
    public CmInteger clockTicksPerSecond;
    /**
     * The name of the operating system product.
     */
    public CmString  osPlatform;
    /**
     * The version string of the currently running operating system.
     */
    public CmString  osVersion;
    /**
     * Size of a page of physical memory in bytes.
     */
    public CmInteger pageSize;
    /**
     * Architecture name of the CPU.
     */
    public CmString  cpuArchitecture;
    /**
     * Model name of the CPU.
     */
    public CmString  cpuModel;
    /**
     * Number of CPUs present in the system.
     */
    public CmInteger cpuCountConfigured;
    /**
     * Number of CPUs that are configured for use by the operating system.
     */
    public CmInteger cpuCountOnline;
    /**
     * Clock speed of the CPU in megahertz.
     */
    public CmInteger cpuClockSpeed;
    /**
     * The elapsed time in seconds that the computer has been running.
     */
    public CmInteger systemUpTime;

    // private members in support of the class
    private PerformanceHost       host;
    private CmOSFlavor            osFlavor;
    private PerformanceCollection collection;

    private CmGlobal<CmPageFiles> gPageFiles;
    private CmPageFiles           pageFiles;
    private PerformanceCategory   memory;
    private PerformanceCategory   misc;
    private PerformanceCategory   cpu;
    private PerformanceCategory   system;

    private CollectionItem pagingUsedItem;
    private CollectionItem pagingTotalItem;
    private CollectionItem memoryFreeItem;
    private CollectionItem memoryTotalItem;
    private CollectionItem clockTicksItem;
    private CollectionItem osPlatformItem;
    private CollectionItem osVersionItem;
    private CollectionItem pageSizeItem;
    private CollectionItem cpuArchItem;
    private CollectionItem cpuModelItem;
    private CollectionItem cpuConfigItem;
    private CollectionItem cpuOnlineItem;
    private CollectionItem cpuSpeedItem;
    private CollectionItem uptimeItem;
    private CollectionItem nowItem;

    /**
     * Default constructor for looking at system information on the local host.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmSysInfo()
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        this("localhost");
    }

    /**
     * Constructor for looking at system information on a remote host.
     * @param remoteHost The name of the host to monitor.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmSysInfo(String remoteHost)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmNames> gnames = CmGlobal.getNames(remoteHost);
        CmNames names = gnames.getObject();
        String memoryCategory;
        String miscCategory;
        String cpuCategory;
        int i;

        host = new PerformanceHost(remoteHost);
        osFlavor = host.getOSFlavor();
        collection = new PerformanceCollection(remoteHost);

        if (osFlavor.isUnixHost())
        {
            // category names on Unix
            memoryCategory = names.UnixMemory;
            miscCategory   = names.UnixMisc;
            cpuCategory    = names.UnixCpuInfo;

            // get categories
            memory      = host.getCategory(memoryCategory);
            misc        = host.getCategory(miscCategory);
            cpu         = host.getCategory(cpuCategory);
            system      = null;

            // get the page files
            gPageFiles = CmGlobal.getPageFiles(remoteHost);
            pageFiles = gPageFiles.getObject();

            // get the rest
            String[] instances = cpu.getInstanceNames();
            String freeMem   = host.getLocalizedName("KBytes Free Memory");
            String physPages = host.getLocalizedName("Physical Pages");
            String clkTicks  = host.getLocalizedName("Clock Ticks/sec");
            String osName    = host.getLocalizedName("OS Name");
            String osVersion = host.getLocalizedName("OS Version");
            String pageSz    = host.getLocalizedName("Page Size");
            String cpuArch   = host.getLocalizedName("CPU Architecture");
            String cpuModel  = host.getLocalizedName("CPU Model Name");
            String cpuConf   = host.getLocalizedName("CPUs Configured");
            String cpuOnln   = host.getLocalizedName("CPUs On-line");
            String cpuSpeed  = host.getLocalizedName("CPU Speed in Megahertz");
            String uptime    = host.getLocalizedName("Boot Time");
            String nowTime   = host.getLocalizedName("Current Time");

            memoryFreeItem  = collection.put(memoryCategory, freeMem);
            memoryTotalItem = collection.put(miscCategory,   physPages);
            clockTicksItem  = collection.put(miscCategory,   clkTicks);
            osPlatformItem  = collection.put(miscCategory,   osName);
            osVersionItem   = collection.put(miscCategory,   osVersion);
            pageSizeItem    = collection.put(miscCategory,   pageSz);
            cpuArchItem     = collection.put(cpuCategory,    cpuArch, instances[0]);
            cpuModelItem    = collection.put(cpuCategory,    cpuModel, instances[0]);
            cpuConfigItem   = collection.put(miscCategory,   cpuConf);
            cpuOnlineItem   = collection.put(miscCategory,   cpuOnln);
            cpuSpeedItem    = collection.put(cpuCategory,    cpuSpeed, instances[0]);
            uptimeItem      = collection.put(miscCategory,   uptime);
            nowItem         = collection.put(miscCategory,   nowTime);
        }
        else if (osFlavor.isWindowsHost())
        {
            String pagingCategory;
            String systemCategory;

            // category names on Windows
            pagingCategory = names.WindowsPageFiles;
            memoryCategory = names.WindowsMemory;
            miscCategory   = names.WindowsSystemInfo;
            systemCategory = names.WindowsSystem;
            cpuCategory    = null;

            // get categories
            memory      = host.getCategory(memoryCategory);
            misc        = host.getCategory(miscCategory);
            system      = host.getCategory(systemCategory);
            cpu         = null;

            // get the rest
            String pgUsed    = host.getLocalizedName("% Usage");
            String pgTotal   = host.getLocalizedName("Total Paging File Space in MB");
            String freeMem   = host.getLocalizedName("Available MBytes");
            String physPages = host.getLocalizedName("Physical Memory Total in MB");
            String clkTicks  = host.getLocalizedName("CPU Clock Ticks/sec");
            String osName    = host.getLocalizedName("Operating System Platform");
            String osVersion = host.getLocalizedName("Operating System Version");
            String pageSz    = host.getLocalizedName("Native Page Size");
            String cpuArch   = host.getLocalizedName("CPU Architecture");
            String cpuModel  = host.getLocalizedName("CPU Model");
            String cpuConf   = host.getLocalizedName("CPU Count Configured");
            String cpuOnln   = host.getLocalizedName("CPU Count Online");
            String cpuSpeed  = host.getLocalizedName("CPU Speed in MHz");
            String uptime    = host.getLocalizedName("System Up Time");

            pagingUsedItem  = collection.put(pagingCategory, pgUsed, "_Total");
            pagingTotalItem = collection.put(miscCategory,   pgTotal);
            memoryFreeItem  = collection.put(memoryCategory, freeMem);
            memoryTotalItem = collection.put(miscCategory,   physPages);
            clockTicksItem  = collection.put(miscCategory,   clkTicks);
            osPlatformItem  = collection.put(miscCategory,   osName);
            osVersionItem   = collection.put(miscCategory,   osVersion);
            pageSizeItem    = collection.put(miscCategory,   pageSz);
            cpuArchItem     = collection.put(miscCategory,   cpuArch);
            cpuModelItem    = collection.put(miscCategory,   cpuModel);
            cpuConfigItem   = collection.put(miscCategory,   cpuConf);
            cpuOnlineItem   = collection.put(miscCategory,   cpuOnln);
            cpuSpeedItem    = collection.put(miscCategory,   cpuSpeed);
            uptimeItem      = collection.put(systemCategory, uptime);
        }

        // query collection
        collection.queryCollection();

        // collect constants
        clockTicksPerSecond = new CmInteger(((Number) clockTicksItem.value).intValue());
        osPlatform = new CmString((String) osPlatformItem.value);
        osVersion = new CmString((String) osVersionItem.value);
        pageSize = new CmInteger(((Number) pageSizeItem.value).intValue());
        cpuArchitecture = new CmString((String) cpuArchItem.value);
        cpuModel = new CmString((String) cpuModelItem.value);
        cpuCountConfigured = new CmInteger(((Number) cpuConfigItem.value).intValue());
        cpuClockSpeed = new CmInteger(((Number) cpuSpeedItem.value).intValue());

        // allocate these now
        pagingFileTotalMB = new CmInteger(true);
        pagingFileFreePercent = new CmInteger(true);
        memoryFreePercent = new CmInteger(true);
        memoryTotalMB = new CmInteger(true);
        cpuCountOnline = new CmInteger(true);
        systemUpTime = new CmInteger(true);

        // These are not needed any longer. Remove them.
        collection.remove(clockTicksItem);
        collection.remove(osPlatformItem);
        collection.remove(osVersionItem);
        collection.remove(pageSizeItem);
        collection.remove(cpuArchItem);
        collection.remove(cpuModelItem);
        collection.remove(cpuConfigItem);
        collection.remove(cpuSpeedItem);

        refresh(false);
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
        refresh(true);
    }

    // The grunt of the refresh method with indication of whether a
    // category refresh needs to be performed.

    private void refresh(boolean needsRefreshing)
        throws RemoteException, NotBoundException,
            MalformedURLException, PerformanceException
    {
        double free = 0.0;
        double total = 0.0;

        if (needsRefreshing)
        {
            if (gPageFiles != null)
                gPageFiles.refresh();
            memory.categoryRefresh();
            misc.categoryRefresh();
            if (cpu != null)
                cpu.categoryRefresh();
            if (system != null)
                system.categoryRefresh();
            collection.queryCollection();
        }

        if (osFlavor.isUnixHost())
        {
            for(CmPFMetrics pf : pageFiles.values())
            {
                // these are in KBytes
                free += pf.freeMB.doubleValue();
                total += pf.sizeMB.doubleValue();
            }
            pagingFileFreePercent.setValue((int) ((free * 100.0) / total));
            pagingFileTotalMB.setValue((int) total);
            free = ((Number) memoryFreeItem.value).doubleValue();
            total = (((Number) memoryTotalItem.value).doubleValue() *
                        (double) pageSize.intValue()) / 1024.0;
            memoryFreePercent.setValue((int) ((free * 100.0) / total));
            memoryTotalMB.setValue((int) (total / 1024.0));
            cpuCountOnline.setValue(((Number) cpuOnlineItem.value).intValue());
            long now = ((Number) nowItem.value).longValue();
            long ago = ((Number) uptimeItem.value).longValue();
            systemUpTime.setValue((int) (now - ago));
        }
        else if (osFlavor.isWindowsHost())
        {
            // this is in MBytes
            total = ((Number) pagingTotalItem.value).doubleValue();
            pagingFileTotalMB.setValue((int) total);

            // this is a %
            free = 100.0 - ((Number) pagingUsedItem.value).doubleValue(); 
            pagingFileFreePercent.setValue((int) free);

            // these are in MBytes
            total = ((Number) memoryTotalItem.value).doubleValue();
            free = ((Number) memoryFreeItem.value).doubleValue();

            memoryFreePercent.setValue((int) ((free * 100.0) / total));
            memoryTotalMB.setValue((int) total);

            cpuCountOnline.setValue(((Number) cpuOnlineItem.value).intValue());
            systemUpTime.setValue(((Number) uptimeItem.value).intValue());
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
     * Utility method for converting uptime seconds to a string spelling out
     * the number of years, days, hours, minutes and seconds of uptime.
     * @param total The number of seconds of uptime.
     * @return A nicely formatted string representing time since boot.
     */
    public static String formatUpTime(int total)
    {
        int secs = 1;
        int mins = 60 * secs;
        int hours = 60 * mins;
        int days = 24 * hours;
        int years = 365 * days;
        int eSecs = 0;
        int eMins = 0;
        int eHours = 0;
        int eDays = 0;
        int eYears = 0;

        if (total >= years) {
            eYears = total / years;
            total -= (eYears * years);
        }
        if (total >= days) {
            eDays = total / days;
            total -= (eDays * days);
        }
        if (total >= hours) {
            eHours = total / hours;
            total -= (eHours * hours);
        }
        if (total >= mins) {
            eMins = total / mins;
            total -= (eMins * mins);
        }
        if (total >= secs) {
            eSecs = total / secs;
            total -= (eSecs * secs);
        }

        String result = null;
        if (eYears > 0)
            result = eYears + " " +
                (eYears == 1 ?
                    CmMessages.msgs.getString("year")
                :
                    CmMessages.msgs.getString("years"));
        if (eDays > 0) {
            if (result == null)
                result = "";
            else
                result += " ";
            result = result + eDays + " " +
                (eDays == 1 ?
                    CmMessages.msgs.getString("day")
                :
                    CmMessages.msgs.getString("days"));
        }
        if (eHours > 0) {
            if (result == null)
                result = "";
            else
                result += " ";
            result = result + eHours + " " +
                (eHours == 1 ?
                    CmMessages.msgs.getString("hour")
                :
                    CmMessages.msgs.getString("hours"));
        }
        if (eMins > 0) {
            if (result == null)
                result = "";
            else
                result += " ";
            result = result + eMins + " " +
                (eMins == 1 ?
                    CmMessages.msgs.getString("min")
                :
                    CmMessages.msgs.getString("mins"));
        }
        if (eSecs > 0) {
            if (result == null)
                result = "";
            else
                result += " ";
            result = result + eSecs + " " +
                (eSecs == 1 ?
                    CmMessages.msgs.getString("sec")
                :
                    CmMessages.msgs.getString("secs"));
        }

        if (result == null)
            result = "0 " + CmMessages.msgs.getString("secs");

        return result;
    }

    /**
     * Determine if this host's connection to the remote host is valid.
     * @return true - valid connection; false - connection is broken
     */
    public boolean hasValidConnection() {
        return host.hasValidConnection();
    }
}
