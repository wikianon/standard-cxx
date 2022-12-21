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
 * A singleton class that presents information much like
 * the vmstat command on Unix.
 */
public class CmSysStat implements CmCategory
{
    /**
     * The number of processes in the system marked in the "runnable" state.
     */
    public CmInteger runnableProcesses;
    /**
     * The number of processes in the system marked in the "blocked" state.
     */
    public CmInteger blockedProcesses;
    /**
     * The percentage of total page file space that is unused.
     */
    public CmInteger pagingFileFreePercent;
    /**
     * The percentage of total physical memory residing on the free list.
     */
    public CmInteger memoryFreePercent;
    /**
     * The number of pages scanned by the page management thread.
     */
    public CmInteger pagesScanned;
    /**
     * The total number of page faults per second.
     */
    public CmInteger pageFaults;
    /**
     * The number of major page faults per second.
     */
    public CmInteger majorPageFaults;
    /**
     * The kilobytes of pages paged in from secondary storage per second.
     */
    public CmInteger KBytesPagedIn;
    /**
     * The kilobytes of pages paged out to secondary storage per second.
     */
    public CmInteger KBytesPagedOut;
    /**
     * The kilobytes of input from secondary storage per second.
     */
    public CmInteger KBytesRead;
    /**
     * The kilobytes of output to secondary storage per second.
     */
    public CmInteger KBytesWritten;
    /**
     * The total number of hardware interrupts received per second.
     */
    public CmInteger interrupts;
    /**
     * The total number of context switches per second.
     */
    public CmInteger contextSwitches;
    /**
     * The total number of system calls per second.
     */
    public CmInteger systemCalls;
    /**
     * The percentage of the elapsed time spent executing code in user space.
     */
    public CmInteger userTimePercent;
    /**
     * The percentage of the elapsed time spent executing
     * priviledged code within the operating system kernel.
     */
    public CmInteger systemTimePercent;
    /**
     * The percentage of the elapsed time while there was at least one
     * pending I/O request charged to this CPU.
     */
    public CmInteger waitTimePercent;
    /**
     * The percentage of elapsed time that the operating system kernel
     * was executing the idle thread.
     */
    public CmInteger idleTimePercent;

    // private members in support of the class
    private PerformanceHost       host;
    private CmOSFlavor            osFlavor;
    private CmGlobal<CmProcesses> gprocs;
    private CmProcesses           procs;
    private CmGlobal<CmSysInfo>   gsysinfo;
    private CmSysInfo             sysinfo;
    private CmGlobal<CmCpuStat>   gcpu;
    private CmCpuStat             cpu;
    private PerformanceCollection collection;
    private PerformanceCategory   memory;
    private PerformanceCategory   disk;
    private PerformanceCategory   processor;
    private PerformanceCategory   system;
    private int                   pageSize;

    private CollectionItem runProcItem;
    private CollectionItem blockProcItem;
    private CollectionItem pfFreeItem;
    private CollectionItem memFreeItem;
    private CollectionItem scannedItem;
    private CollectionItem faultsItem;
    private CollectionItem majorItem;
    private CollectionItem pagedInItem;
    private CollectionItem pagedOutItem;
    private CollectionItem readItem;
    private CollectionItem writtenItem;
    private CollectionItem intrItem;
    private CollectionItem cswItem;
    private CollectionItem sysCallItem;
    private CollectionItem usrItem;
    private CollectionItem sysItem;
    private CollectionItem waitItem;
    private CollectionItem idleItem;

    /**
     * Default constructor for looking at system information on the local host.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmSysStat()
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
    public CmSysStat(String remoteHost)
        throws NotBoundException, MalformedURLException,
            RemoteException, PerformanceException
    {
        CmGlobal<CmNames> gnames = CmGlobal.getNames(remoteHost);
        CmNames names = gnames.getObject();
        int i;

        host = new PerformanceHost(remoteHost);
        osFlavor = host.getOSFlavor();
        collection = new PerformanceCollection(remoteHost);

        if (osFlavor.isUnixHost())
        {
            String sysStat = names.UnixSysStat;
            String run     = host.getLocalizedName("Runnable Processes");
            String block   = host.getLocalizedName("Blocked Processes");
            String pgFree  = host.getLocalizedName("Percent Paging File Free");
            String memFree = host.getLocalizedName("Percent Memory Free");
            String scan    = host.getLocalizedName("Pages Scanned");
            String faults  = host.getLocalizedName("Total Page Faults");
            String major   = host.getLocalizedName("Major Page Faults");
            String pgIn    = host.getLocalizedName("KBytes Paged in/sec");
            String pgOut   = host.getLocalizedName("KBytes Paged out/sec");
            String read    = host.getLocalizedName("KBytes Read/sec");
            String write   = host.getLocalizedName("KBytes Written/sec");
            String intr    = host.getLocalizedName("Interrupts/sec");
            String csw     = host.getLocalizedName("Context Switches/sec");
            String sysc    = host.getLocalizedName("System Calls/sec");
            String usr     = host.getLocalizedName("Percent User Time");
            String sys     = host.getLocalizedName("Percent Kernel Time");
            String wait    = host.getLocalizedName("Percent Wait Time");
            String idle    = host.getLocalizedName("Percent Idle Time");

            system         = host.getCategory(sysStat);
            runProcItem    = collection.put(sysStat, run);
            blockProcItem  = collection.put(sysStat, block);
            pfFreeItem     = collection.put(sysStat, pgFree);
            memFreeItem    = collection.put(sysStat, memFree);
            scannedItem    = collection.put(sysStat, scan);
            faultsItem     = collection.put(sysStat, faults);
            majorItem      = collection.put(sysStat, major);
            pagedInItem    = collection.put(sysStat, pgIn);
            pagedOutItem   = collection.put(sysStat, pgOut);
            readItem       = collection.put(sysStat, read);
            writtenItem    = collection.put(sysStat, write);
            intrItem       = collection.put(sysStat, intr);
            cswItem        = collection.put(sysStat, csw);
            sysCallItem    = collection.put(sysStat, sysc);
            usrItem        = collection.put(sysStat, usr);
            sysItem        = collection.put(sysStat, sys);
            waitItem       = collection.put(sysStat, wait);
            idleItem       = collection.put(sysStat, idle);

            collection.queryCollection();

            runnableProcesses = new CmInteger(
                Int(runProcItem.value), runProcItem.supported);
            blockedProcesses = new CmInteger(
                Int(blockProcItem.value), blockProcItem.supported);
            pagingFileFreePercent = new CmInteger(
                Int(pfFreeItem.value), pfFreeItem.supported);
            memoryFreePercent = new CmInteger(
                Int(memFreeItem.value), memFreeItem.supported);
            pagesScanned = new CmInteger(
                Int(scannedItem.value), scannedItem.supported);
            pageFaults = new CmInteger(
                Int(faultsItem.value), faultsItem.supported);
            majorPageFaults = new CmInteger(
                Int(majorItem.value), majorItem.supported);
            KBytesPagedIn = new CmInteger(
                Int(pagedInItem.value), pagedInItem.supported);
            KBytesPagedOut = new CmInteger(
                Int(pagedOutItem.value), pagedOutItem.supported);
            KBytesRead = new CmInteger(
                Int(readItem.value), readItem.supported);
            KBytesWritten = new CmInteger(
                Int(writtenItem.value), writtenItem.supported);
            interrupts = new CmInteger(
                Int(intrItem.value), intrItem.supported);
            contextSwitches = new CmInteger(
                Int(cswItem.value), cswItem.supported);
            systemCalls = new CmInteger(
                Int(sysCallItem.value), sysCallItem.supported);
            userTimePercent = new CmInteger(
                Int(usrItem.value), usrItem.supported);
            systemTimePercent = new CmInteger(
                Int(sysItem.value), sysItem.supported);
            waitTimePercent = new CmInteger(
                Int(waitItem.value), waitItem.supported);
            idleTimePercent = new CmInteger(
                Int(idleItem.value), idleItem.supported);
        }
        else if (osFlavor.isWindowsHost())
        {
            gprocs   = CmGlobal.getProcesses(remoteHost);
            procs    = gprocs.getObject();
            gsysinfo = CmGlobal.getSysInfo(remoteHost);
            sysinfo  = gsysinfo.getObject();
            gcpu     = CmGlobal.getCpuStat(remoteHost);
            cpu      = gcpu.getObject();

            String memCat  = names.WindowsMemory;
            String diskCat = names.WindowsPhysDisk;
            String procCat = names.WindowsProcessor;
            String sysCat  = names.WindowsSystem;

            memory    = host.getCategory(memCat);
            disk      = host.getCategory(diskCat);
            processor = host.getCategory(procCat);
            system    = host.getCategory(sysCat);

            String pgIn   = host.getLocalizedName("Pages Input/sec");
            String pgOut  = host.getLocalizedName("Pages Output/sec");
            String faults = host.getLocalizedName("Page Faults/sec");
            String major  = host.getLocalizedName("Page Reads/sec");
            String read   = host.getLocalizedName("Disk Read Bytes/sec");
            String write  = host.getLocalizedName("Disk Write Bytes/sec");
            String intr   = host.getLocalizedName("Interrupts/sec");
            String csw    = host.getLocalizedName("Context Switches/sec");
            String sysc   = host.getLocalizedName("System Calls/sec");

            pagedInItem  = collection.put(memCat,  pgIn);
            pagedOutItem = collection.put(memCat,  pgOut);
            faultsItem   = collection.put(memCat,  faults);
            majorItem    = collection.put(memCat,  major);
            readItem     = collection.put(diskCat, read, "_Total");
            writtenItem  = collection.put(diskCat, write, "_Total");
            intrItem     = collection.put(procCat, intr, "_Total");
            cswItem      = collection.put(sysCat,  csw);
            sysCallItem  = collection.put(sysCat,  sysc);

            collection.queryCollection();

            runnableProcesses = new CmInteger();
            blockedProcesses = new CmInteger();

            doProcCounting();

            pagingFileFreePercent = sysinfo.pagingFileFreePercent;
            memoryFreePercent = sysinfo.memoryFreePercent;
            pageSize = sysinfo.pageSize.intValue();
            pagesScanned = new CmInteger(false);
            pageFaults = new CmInteger(
                Int(faultsItem.value), faultsItem.supported);
            majorPageFaults = new CmInteger(
                Int(majorItem.value), majorItem.supported);
            KBytesPagedIn = new CmInteger(
                (int) ((BigInt(pagedInItem.value) * pageSize) / 1024),
                    pagedInItem.supported);
            KBytesPagedOut = new CmInteger(
                (int) ((BigInt(pagedOutItem.value) * pageSize) / 1024),
                    pagedOutItem.supported);
            KBytesRead = new CmInteger(
                Int(readItem.value) / 1024, readItem.supported);
            KBytesWritten = new CmInteger(
                Int(writtenItem.value) / 1024, writtenItem.supported);
            interrupts = new CmInteger(
                Int(intrItem.value), intrItem.supported);
            contextSwitches = new CmInteger(
                Int(cswItem.value), cswItem.supported);
            systemCalls = new CmInteger(
                Int(sysCallItem.value), sysCallItem.supported);
            userTimePercent   = cpu.userTimePercent;
            systemTimePercent = cpu.systemTimePercent;
            waitTimePercent   = cpu.waitTimePercent;
            idleTimePercent   = cpu.idleTimePercent;
        }
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
        if (osFlavor.isUnixHost())
        {
            // refresh PerformanceCategory objects
            system.categoryRefresh();

            // query the collection
            collection.queryCollection();

            runnableProcesses.setValue(Int(runProcItem.value));
            blockedProcesses.setValue(Int(blockProcItem.value));
            pagingFileFreePercent.setValue(Int(pfFreeItem.value));
            memoryFreePercent.setValue(Int(memFreeItem.value));
            pagesScanned.setValue(Int(scannedItem.value));
            pageFaults.setValue(Int(faultsItem.value));
            majorPageFaults.setValue(Int(majorItem.value));
            KBytesPagedIn.setValue(Int(pagedInItem.value));
            KBytesPagedOut.setValue(Int(pagedOutItem.value));
            KBytesRead.setValue(Int(readItem.value));
            KBytesWritten.setValue(Int(writtenItem.value));
            interrupts.setValue(Int(intrItem.value));
            contextSwitches.setValue(Int(cswItem.value));
            if (sysCallItem.supported)
              systemCalls.setValue(Int(sysCallItem.value));
            userTimePercent.setValue(Int(usrItem.value));
            systemTimePercent.setValue(Int(sysItem.value));
            if (waitItem.supported)
              waitTimePercent.setValue(Int(waitItem.value));
            idleTimePercent.setValue(Int(idleItem.value));
        }
        else if (osFlavor.isWindowsHost())
        {
            // refresh CmGlobal objects
            gprocs.refresh();
            gsysinfo.refresh();
            gcpu.refresh();

            // refresh PerformanceCategory objects
            memory.categoryRefresh();
            disk.categoryRefresh();
            processor.categoryRefresh();
            system.categoryRefresh();

            // query the collection
            collection.queryCollection();

            // count the runnable and blocked procs
            doProcCounting();

            KBytesPagedIn.setValue(
                (int) ((BigInt(pagedInItem.value) * pageSize) / 1024));
            KBytesPagedOut.setValue(
                (int) ((BigInt(pagedOutItem.value) * pageSize) / 1024));
            pageFaults.setValue(Int(faultsItem.value));
            majorPageFaults.setValue(Int(majorItem.value));
            KBytesRead.setValue(Int(readItem.value) / 1024);
            KBytesWritten.setValue(Int(writtenItem.value) / 1024);
            interrupts.setValue(Int(intrItem.value));
            contextSwitches.setValue(Int(cswItem.value));
            systemCalls.setValue(Int(sysCallItem.value));
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

    private int Int(Object o)
    {
        if (o == null)
            return 0;
        return ((Number) o).intValue();
    }

    private long BigInt(Object o)
    {
        if (o == null)
            return 0;
        return ((Number) o).longValue();
    }

    private void doProcCounting()
    {
        int runnable = 0;
        int blocked = 0;

        for(CmProcessMetrics pm : procs.values())
        {
            String state = pm.processState.stringValue();

            if (state.equals("Runnable"))
                runnable++;
            else if (state.equals("Blocked"))
                blocked++;
        }

        runnableProcesses.setValue(runnable);
        blockedProcesses.setValue(blocked);
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
