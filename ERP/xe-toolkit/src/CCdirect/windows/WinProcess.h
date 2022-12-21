/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CCdirect library of Captivity.
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

#pragma once

#include "Stdafx.h"
#include <time.h>
#include <map>
#include <string>

// pilfered from mdr_ps.h
const int MPS_RUNNABLE    = 0;
const int MPS_SLEEPING    = 1;
const int MPS_STOPPED     = 2;
const int MPS_BLOCKED     = 3;
const int MPS_ZOMBIE      = 4;
const int MPS_SPECIFIC    = 5;
const int MPS_PROC_STATES = 6;

class ProcInfo {
public:
    time_t      startTime;              // StartTime (Unix epoch format, but in milliseconds)
    long        processID;              // Id
    long        parentProcessID;        // reg:Creating Process ID
    std::string userName;               // GetSecurityInfo(), et. al.
    std::string commandName;            // ProcessName
    std::string commandLine;            // QueryFullProcessImageName() (Vista/Longhorn)
    double      totalCPUSeconds;        // TotalProcessorTime
    double      userCPUSeconds;         // UserProcessorTime
    double      kernelCPUSeconds;       // Difference between previous two
    double      percentUserTime;        // compute
    double      percentKernelTime;      // compute
    int         totalSizeKB;            // VirtualMemorySize
    int         residentSizeKB;         // WorkingSet
    int         totalIOKB;              // reg:IO Data Bytes/sec
    double      percentCPUUse;          // compute
    double      percentPhysmemUse;      // compute
    int         pageFaults;             // reg:Page Faults/sec
    int         processState;           // compute
    int         globalPriority;         // BasePriority
    int         numberOfThreads;        // Threads->Count
    int         totalSizeDeltaKB;       // compute
    int         residentSizeDeltaKB;    // compute
    int         contextSwitches;        // ContextSwitches
    bool        isSystemProcess;        // Size == 0

    ProcInfo()
    {
        clear();
    }

    void clear(void)
    {
        startTime = 0;
        processID = 0;
        parentProcessID = 0;
        userName.clear();
        commandName.clear();
        commandLine.clear();
        totalCPUSeconds = 0;
        userCPUSeconds = 0;
        kernelCPUSeconds = 0;
        percentUserTime = 0;
        percentKernelTime = 0;
        totalSizeKB = 0;
        residentSizeKB = 0;
        totalIOKB = 0;
        percentCPUUse = 0;
        percentPhysmemUse = 0;
        pageFaults = 0;
        processState = 0;
        globalPriority = 0;
        numberOfThreads = 0;
        totalSizeDeltaKB = 0;
        residentSizeDeltaKB = 0;
        contextSwitches = 0;
        isSystemProcess = false;
    }
};

typedef std::map<std::string, ProcInfo> ProcMap;
typedef ProcMap::iterator ProcMapNode;

typedef NTSTATUS (NTAPI *NQSI)(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL);
typedef BOOL (NTAPI *QFPIN)(
    IN     HANDLE hProcess,
    IN     DWORD  dwFlags,
    OUT    LPTSTR lpExeName,
    IN OUT PDWORD lpdwSize);

class WinProcess : public ProcMap
{
public:
    WinProcess();
   ~WinProcess();

    void refresh(void);
    void rates(void);
    void refreshRates(void)
    {
        refresh();
        rates();
    }
private:
    char   *buf;
    int     bufsiz;

    HMODULE hNtDll;
    HMODULE hK32Dll;
    NQSI    NtQuerySystemInformation;
    QFPIN   QueryFullProcessImageName;
    time_t  msPerTick;
    time_t  hundredthNano;
    time_t  hundredthNanoPerMilli;
    time_t  epochDiff;
    time_t  bootTime;
    int     totalPhysmemKB;
    int     cpusOnline;
    time_t  lastTime;
    DWORD   lastUpdate;

    ProcMap old;
};
