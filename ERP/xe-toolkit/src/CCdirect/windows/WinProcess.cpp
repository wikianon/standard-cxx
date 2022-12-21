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

//
// If you find a memory leak in this code, I'll send you a coffee mug.
//

#include <Stdafx.h>
#include <iostream>
#include <mdr_exception.h>
#include <sysinfo.h>
#include <AccCtrl.h>
#include <AclAPI.h>
#include <set>
#include "WinProcess.h"
#include "ProcStructs.h"

// #define _CM_DEBUG_ 1

using namespace mdr;

#define STATUS_SUCCESS              ((NTSTATUS) 0)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)

typedef std::set<std::string> DelSet;
typedef DelSet::iterator      DelNode;

WinProcess::WinProcess()
{
    sysinfo_t sys;
    STOD_INFO tod;

    bufsiz = 65536;
    buf = new char[bufsiz];
    memset(buf, '\0', bufsiz);

    // open ntdll.dll dll
    hNtDll = LoadLibrary("ntdll.dll");
    if (hNtDll == 0)
        throw mdr_exception_t("WinProcess::WinProcess: cannot open ntdll.dll");

    // get the NtQuerySystemInformation function pointer
    FARPROC nqsi = GetProcAddress(hNtDll, "NtQuerySystemInformation");
    if (nqsi == 0)
        throw mdr_exception_t("WinProcess::WinProcess: cannot find NQSI");

    NtQuerySystemInformation = (NQSI) nqsi;

    // null by default; should only be found on Vista and newer
    QueryFullProcessImageName = 0;

    hK32Dll = LoadLibrary("kernel32.dll");
    if (hK32Dll != 0) {
        FARPROC qfpin = GetProcAddress(hK32Dll, "QueryFullProcessImageNameA");

        if (qfpin != 0)
            QueryFullProcessImageName = (QFPIN) qfpin;
    }

    // computed constants that are needed later
    totalPhysmemKB = sys.MBytesMemoryTotal * 1024;
    cpusOnline = sys.cpuCountOnline;
    lastTime = 0;

    // constants
    msPerTick = 1000 / CLOCKS_PER_SEC;
    hundredthNano = 1000000000LL / 100;
    hundredthNanoPerMilli = hundredthNano / 1000;
    epochDiff = 11644473600000LL;

    // get the hi-res boot time
    DWORD status = NtQuerySystemInformation(SystemTimeOfDayInformation,
                                        (void *) &tod, sizeof tod, 0);
    if (status != STATUS_SUCCESS)
        throw mdr_exception_t("WinProcess::WinProcess: cannot get boot time");

    bootTime = (tod.BootTime.QuadPart / hundredthNanoPerMilli) - epochDiff;

    // The following magic will allow this process to open other processes
    // for the purpose of querying the security info (the user name).
    // The bottom line: this process makes itself look like a debugger.

    HANDLE hProcess;
    HANDLE hToken;

    // first, open myself
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
    if (hProcess) {
        // now get the adjust privileges token
        if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken)) {
            TOKEN_PRIVILEGES tp;
            LUID luid;

            // lookup the privilege token for debugging
            status = LookupPrivilegeValue(0, SE_DEBUG_NAME, &luid);
            if (status == 0)
                status = GetLastError();

            // turn it on
            tp.PrivilegeCount = 1;
            tp.Privileges[0].Luid = luid;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

            // adjust the privileges
            status = AdjustTokenPrivileges(
                   hToken, 
                   FALSE, 
                   &tp, 
                   sizeof(TOKEN_PRIVILEGES), 
                   (PTOKEN_PRIVILEGES) 0, 
                   (PDWORD) 0);
            if (status == 0)
                status = GetLastError();

            CloseHandle(hToken);
        }
        CloseHandle(hProcess);
    }

    lastUpdate = 0;
    refreshRates();
}

WinProcess::~WinProcess()
{
    if (buf)
        delete[] buf;
    if (hNtDll != 0)
        FreeLibrary(hNtDll);
    if (hK32Dll != 0)
        FreeLibrary(hK32Dll);
}

void WinProcess::refresh(void)
{
    DWORD status;
    SYSTEM_PROCESS *pp;
    char name[128];
    char key[128];
    char *p;
    DWORD me = GetCurrentProcessId();
    HANDLE hProcess;
    int procStates[MPS_PROC_STATES];
    int i;
    ProcMapNode node;
    DelSet delSet;
    DelNode sn;
    DelSet currentSet;

#ifdef _CM_DEBUG_
    std::cerr << "Initial NtQSI" << std::endl;
#endif

    memset(buf, '\0', bufsiz);

    for (;;)
    {
        status = NtQuerySystemInformation(SystemProcessInformation,
                                          (void *) buf, bufsiz, 0);
        if (status == STATUS_INFO_LENGTH_MISMATCH)
        {
            delete[] buf;
            bufsiz *= 2;
            buf = new char[bufsiz];
            memset(buf, '\0', bufsiz);
            continue;
        }
        break;
    }

#ifdef _CM_DEBUG_
std::cerr << "NtQSI done; bufsiz=" << bufsiz << std::endl;
#endif

    pp = (SYSTEM_PROCESS *) buf;
    for (;;)
    {
        // pid 0 is the Idle Process
        if (pp->ProcessId == 0)
            pp->ProcessName.Buffer = L"Idle";

        // If the process has no name, skip it.
        if (pp->ProcessName.Buffer != 0)
        {
            // de-unicode the name
            _snprintf_s(name, sizeof name, sizeof name, "%S",
                        pp->ProcessName.Buffer);
            p = strrchr(name, '.');
            if ((p != 0) &&
              ((strcmp(p, ".exe") == 0) || (strcmp(p, ".EXE") == 0)))
                *p = '\0';

            _snprintf_s(key, sizeof key, sizeof key, "%s[%d]",
                name, pp->ProcessId);

            // if it's not already there then make a new one
            node = find(key);
            if (node == end()) {
#ifdef _CM_DEBUG_
std::cerr << "New Process: " << key << std::endl;
#endif
                // load up pi with all of the information that does not change

                ProcInfo pi;
                time_t fileTime = pp->CreateTime.QuadPart;

                // If the creation time is zero then, it happened at system boot.
                if (fileTime == 0)
                    fileTime = (time_t) bootTime;
                else
                    fileTime = (time_t) ((fileTime / hundredthNanoPerMilli) - epochDiff);

                pi.startTime = fileTime;
                pi.processID = pp->ProcessId;
                pi.commandName = name;

#ifdef _CM_DEBUG_
std::cerr << "Process <" << name << ">" << std::endl;
#endif

                // If it's my own process, get the command line.
                if (me == pi.processID)
                    pi.commandLine = GetCommandLineA();

                // Open the process.
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pi.processID);
                if (hProcess != 0) {
                    PSID pOwner;
                    PSECURITY_DESCRIPTOR pSD = 0;

                    // Get the security info.
                    status = GetSecurityInfo(hProcess, SE_KERNEL_OBJECT,
                        OWNER_SECURITY_INFORMATION,
                            (PSID *) &pOwner, NULL, NULL, NULL, &pSD);
                    if (status == ERROR_SUCCESS) {
                        static const DWORD S_NAME_MAX = 32;
                        DWORD cbName = S_NAME_MAX;
                        DWORD cbDomain = S_NAME_MAX;
                        char szName[S_NAME_MAX];
                        char szDomain[S_NAME_MAX];
                        SID_NAME_USE m_sidnameuse;

                        // Lookup the user name.
                        status = LookupAccountSidA(0, pOwner, szName, &cbName,
                            szDomain, &cbDomain, &m_sidnameuse);
                        if (status) {
                            pi.userName = szName;
                        }
                    }
#ifdef _CM_DEBUG_
else std::cerr << "Cannot GetSecurityInfo(" << pi.processID << ")" << std::endl;
#endif
                    if (pSD)
                        LocalFree(pSD);

                    // smoke 'em if you got 'em
                    if (pi.commandLine.length() == 0 &&
                            QueryFullProcessImageName != 0) {
                        char name[4096];
                        DWORD size = sizeof(name);

                        //
                        // This is not a useful comment, but here it is.
                        // It would be nice if I could get a handle to the
                        // open process and call into that process' address
                        // space and call GetCommandLineA() to get ITS' full
                        // command name. This function is really not all that
                        // useful. It just gives the full path of argv[0], not
                        // the actual full command line.
                        //
                        if (QueryFullProcessImageName(hProcess, 0, name, &size)) {
                            name[size] = '\0';
                            pi.commandLine = name;
                        }
                    }

                    CloseHandle(hProcess);
                }
#ifdef _CM_DEBUG_
else std::cerr << "Cannot OpenProcess(" << pi.processID << ")" << std::endl;
#endif

                // Punt.
                if (pi.userName.size() == 0)
                    pi.userName = "SYSTEM";

                (*this)[key] = pi;
                node = find(key);
            }

            ProcInfo *pip = &node->second;

            // Compute these.
            pip->userCPUSeconds =
                (double) pp->UserTime.QuadPart / (double) hundredthNano;
            pip->kernelCPUSeconds =
                (double) pp->KernelTime.QuadPart / (double) hundredthNano;
            pip->totalCPUSeconds =
                pip->userCPUSeconds + pip->kernelCPUSeconds;
            pip->percentUserTime =
                (pip->userCPUSeconds * 100.0) /
                    pip->totalCPUSeconds;
            pip->percentKernelTime =
                (pip->kernelCPUSeconds * 100.0) /
                    pip->totalCPUSeconds;
            pip->totalSizeKB =
                pp->VmCounters.VirtualSize / 1024;
            pip->residentSizeKB =
                pp->VmCounters.WorkingSetSize / 1024;
            pip->percentPhysmemUse =
                (pip->residentSizeKB * 100.0) / totalPhysmemKB;
            pip->totalIOKB =
                (int) ((pp->IoCounters.ReadTransferCount +
                        pp->IoCounters.WriteTransferCount +
                        pp->IoCounters.OtherTransferCount) / 1024);


            for(i=0; i<MPS_PROC_STATES; i++)
                procStates[i] = 0;

            DWORD contextSwitches = 0;

#ifdef _CM_DEBUG_
std::cerr << "Process has " << pp->ThreadCount << " threads" << std::endl;
#endif

            for(i=0; i<(int) pp->ThreadCount; i++) {
                contextSwitches += pp->Threads[i].ContextSwitches;

                switch(pp->Threads[i].ThreadState) {
                    // A state that indicates the thread has been initialized
                    // but has not yet started.
                case THREAD_INITIALIZED:
                    // The state of the thread is unknown. 
                case THREAD_UNKNOWN:
                    procStates[MPS_SPECIFIC]++;
                    break;
                    // A state that indicates the thread is waiting to use a
                    // processor because no processor is free. The thread is
                    // prepared to run on the next available processor. 
                case THREAD_READY:
                    // A state that indicates the thread is currently using a processor. 
                case THREAD_RUNNING:
                    // A state that indicates the thread is about to use a processor.
                    // Only one thread can be in this state at a time. 
                case THREAD_STANDBY:
                    procStates[MPS_RUNNABLE]++;
                    break;
                    // A state that indicates the thread has finished executing and has exited. 
                case THREAD_TERMINATED:
                    procStates[MPS_ZOMBIE]++;
                    break;
                    // A state that indicates the thread is not ready to use the processor
                    // because it is waiting for a peripheral operation to complete or a
                    // resource to become free. When the thread is ready, it will be rescheduled. 
                case THREAD_WAITING:
                    switch(pp->Threads[i].WaitReason) {
                        // The thread is waiting for event pair high. 
                    case WAIT_EventPairHigh:
                        // The thread is waiting for event pair low. 
                    case WAIT_EventPairLow:
                        // Thread execution is delayed. 
                    case WAIT_ExecutionDelay1:
                    case WAIT_ExecutionDelay2:
                        // The thread is waiting for the scheduler. 
                    case WAIT_Executive1:
                    case WAIT_Executive2:
                        // The thread is waiting for a local procedure call to arrive. 
                    case WAIT_LpcReceive:
                        // The thread is waiting for reply to a local procedure call to arrive. 
                    case WAIT_LpcReply:
                        // Thread execution is suspended. 
                    case WAIT_Suspended1:
                    case WAIT_Suspended2:
                        // The thread is waiting for a user request. 
                    case WAIT_UserRequest1:
                    case WAIT_UserRequest2:
                        // The thread is waiting for an unknown reason. 
                    default:
                        procStates[MPS_SLEEPING]++;
                        break;
                        // The thread is waiting for a free virtual memory page. 
                    case WAIT_FreePage1:
                    case WAIT_FreePage2:
                        // The thread is waiting for a virtual memory page to arrive in memory. 
                    case WAIT_PageIn1:
                    case WAIT_PageIn2:
                        // The thread is waiting for a virtual memory page to be written to disk. 
                    case WAIT_PageOut:
                        // The thread is waiting for system allocation. 
                    case WAIT_SystemAllocation1:
                    case WAIT_SystemAllocation2:
                        // The thread is waiting for the system to allocate virtual memory. 
                    case WAIT_VirtualMemory:
                        procStates[MPS_BLOCKED]++;
                        break;
                    }
                    break;
                    // A state that indicates the thread is waiting for a resource, other than
                    // the processor, before it can execute. For example, it might be waiting
                    // for its execution stack to be paged in from disk. 
                case THREAD_TRANSITION:
                    procStates[MPS_BLOCKED]++;
                    break;
                default:
                    break;
                }
            }

            pip->contextSwitches = contextSwitches;
            pip->processState = MPS_SLEEPING;
            if (procStates[MPS_RUNNABLE] > 0)
                pip->processState = MPS_RUNNABLE;
            else {
                for(i=0; i<MPS_PROC_STATES; i++) {
                    if (procStates[i] == pp->ThreadCount) {
                        pip->processState = i;
                        break;
                    }
                }
            }

            pip->globalPriority = pp->BasePriority;
            pip->numberOfThreads = pp->ThreadCount;
            pip->isSystemProcess = (pip->totalSizeKB == 0);

            // keep track of what procs we know are there
            currentSet.insert(key);
        }

        if (pp->Next == 0)
            break;

#ifdef _CM_DEBUG_
std::cerr << "pp->Next is " << pp->Next << std::endl;
#endif

        pp = (SYSTEM_PROCESS *) (((char *) pp) + pp->Next);
    }

    // We know what *is* there. Now remove any procs in the current set
    // that are *not* there.

    // if it's not in currentSet, then mark it for deletion
    for(node=begin(); node != end(); node++) {
        if (currentSet.find(node->first) == currentSet.end())
            delSet.insert(node->first);
        else
            // it exists. make sure there's an old node for it
            if (old.find(node->first) == old.end())
                old[node->first].clear();
    }

    // traverse the delete set and remove anything that is not there
    for(sn=delSet.begin(); sn != delSet.end(); sn++) {
        node = find(*sn);
        erase(node);
#ifdef _CM_DEBUG_
std::cerr << "deleting process " << *sn << std::endl;
#endif
    }

    // clear deletion set
    delSet.clear();

    // now find "old" nodes that no longer exist and delete them
    for(node=old.begin(); node != old.end(); node++)
        if (find(node->first) == end())
            delSet.insert(node->first);

    for(sn=delSet.begin(); sn != delSet.end(); sn++) {
        node = old.find(*sn);
        old.erase(node);
    }
}

void WinProcess::rates(void)
{
    ProcMapNode n;
    ProcMapNode ncn;
    ProcInfo *pip;
    ProcInfo *opip;
    time_t now;
    time_t nowTime;
    double elapsed;
    double tmp_userCPUSeconds;
    double tmp_kernelCPUSeconds;
    int tmp_totalIOKB;
    bool firstPass = false;
    STOD_INFO tod;
    DWORD status;

    status = NtQuerySystemInformation(SystemTimeOfDayInformation,
                                        (void *) &tod, sizeof tod, 0);
    now = tod.CurrentTime.QuadPart;
    nowTime = (now / hundredthNanoPerMilli) - epochDiff;

    // Constructor sets lastTime to zero. This is the first pass.
    if (lastTime == 0)
        firstPass = true;
    else {
        // elapsed seconds, whole and fractional
        elapsed = (double) (now - lastTime) / (double) hundredthNano;
        if (elapsed == 0.0)
            return; // no time has passed. everything is the same
    }

    for(n=begin(); n != end(); n++) {
        pip = &n->second;
        ncn = old.find(n->first);

        // should not happen
        if (ncn == old.end())
            old[n->first].clear();

        opip = &ncn->second;

        // First pass. Elapsed time is seconds since process was started.
        long pid = pip->processID;
        if (firstPass) {
            elapsed = (double) ((nowTime - pip->startTime) / 1000.0);
            if (elapsed == 0.0)
                elapsed = 1.0 / hundredthNano; // a 100ns unit
        }

        // how many processors can run for the process
        int engines = (cpusOnline < pip->numberOfThreads) ?
            cpusOnline : pip->numberOfThreads;

        // the amount of time that could be used is engines * elapsed time
        double potential = elapsed * engines;

        tmp_userCPUSeconds   = pip->userCPUSeconds;
        tmp_kernelCPUSeconds = pip->kernelCPUSeconds;
        tmp_totalIOKB        = pip->totalIOKB;

        pip->userCPUSeconds   -= opip->userCPUSeconds;
        pip->kernelCPUSeconds -= opip->kernelCPUSeconds;

        pip->percentUserTime = (pip->userCPUSeconds * 100.0) / potential;
        pip->percentKernelTime = (pip->kernelCPUSeconds * 100.0) / potential;
        pip->percentCPUUse = (pip->percentUserTime + pip->percentKernelTime);

        pip->percentPhysmemUse = (pip->residentSizeKB * 100.0) / totalPhysmemKB;

        pip->totalSizeDeltaKB = pip->totalSizeKB - opip->totalSizeKB;
        pip->residentSizeDeltaKB = pip->residentSizeKB - opip->residentSizeKB;

        pip->totalIOKB = (int) ((pip->totalIOKB - opip->totalIOKB) / elapsed);

        opip->userCPUSeconds = tmp_userCPUSeconds;
        opip->kernelCPUSeconds = tmp_kernelCPUSeconds;
        opip->totalSizeKB = pip->totalSizeKB;
        opip->residentSizeKB = pip->residentSizeKB;
        opip->totalIOKB = tmp_totalIOKB;
    }

    lastTime = now;
}
