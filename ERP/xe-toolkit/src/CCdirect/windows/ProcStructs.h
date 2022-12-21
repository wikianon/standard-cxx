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

#ifndef _PROC_STRUCTS_H_
#define _PROC_STRUCTS_H_

// definitions lifted from awprofessional.com

#include "Stdafx.h"

typedef struct
{
	DWORD PeakVirtualSize;
	DWORD VirtualSize;
	DWORD PageFaultCount;
	DWORD PeakWorkingSetSize;
	DWORD WorkingSetSize;
	DWORD QuotaPeakPagedPoolUsage;
	DWORD QuotaPagedPoolUsage;
	DWORD QuotaPeakNonPagedPoolUsage;
	DWORD QuotaNonPagedPoolUsage;
	DWORD PagefileUsage;
	DWORD PeakPagefileUsage;
} VM_COUNTERS;

typedef struct
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID;

typedef enum
{
	THREAD_INITIALIZED = 0,
	THREAD_READY       = 1,
	THREAD_RUNNING     = 2,
	THREAD_STANDBY     = 3,
	THREAD_TERMINATED  = 4,
	THREAD_WAITING     = 5,
	THREAD_TRANSITION  = 6,
	THREAD_UNKNOWN     = 7
} THREAD_STATE;

typedef enum
{
	WAIT_Executive1        = 0,
	WAIT_FreePage1         = 1,
	WAIT_PageIn1           = 2,
	WAIT_SystemAllocation1 = 3,
	WAIT_ExecutionDelay1   = 4,
	WAIT_Suspended1        = 5,
	WAIT_UserRequest1      = 6,
	WAIT_Executive2        = 7,
	WAIT_FreePage2         = 8,
	WAIT_PageIn2           = 9,
	WAIT_SystemAllocation2 = 10,
	WAIT_ExecutionDelay2   = 11,
	WAIT_Suspended2        = 12,
	WAIT_UserRequest2      = 13,
	WAIT_EventPairHigh     = 14,
	WAIT_EventPairLow      = 15,
	WAIT_LpcReceive        = 16,
	WAIT_LpcReply          = 17,
	WAIT_VirtualMemory     = 18,
	WAIT_PageOut           = 19
} WAIT_REASON;

typedef struct
{
	LARGE_INTEGER  KernelTime;
	LARGE_INTEGER  UserTime;
	LARGE_INTEGER  CreateTime;
	DWORD          WaitTime;
	PVOID          StartAddress;
	CLIENT_ID      Cid;
	DWORD          Priority;
	DWORD          BasePriority;
	DWORD          ContextSwitches;
	THREAD_STATE   ThreadState;
	WAIT_REASON    WaitReason;
	DWORD          Reserved01;
} SYSTEM_THREAD;

typedef struct
{
	DWORD          Next;
	DWORD          ThreadCount;
	DWORD          Reserved1[6];
	LARGE_INTEGER  CreateTime;
	LARGE_INTEGER  UserTime;
	LARGE_INTEGER  KernelTime;
	UNICODE_STRING ProcessName;
	LONG           BasePriority;
	DWORD          ProcessId;
	DWORD          InheritedFromProcessId;
	DWORD          HandleCount;
	DWORD          Reserved2[2];
	VM_COUNTERS    VmCounters;
	DWORD          CommitCharge;
	IO_COUNTERS    IoCounters;
	SYSTEM_THREAD  Threads[1];
} SYSTEM_PROCESS;

typedef struct
{
	LARGE_INTEGER BootTime;
	LARGE_INTEGER CurrentTime;
	LARGE_INTEGER TimeZoneBias;
	DWORD         CurrentTimeZoneId;
} STOD_INFO;

#endif

#if _ORIGINAL_VERSION_
typedef struct _SYSTEM_THREAD
    {
/*000*/ FILETIME   ftKernelTime;   // 100 nsec units
/*008*/ FILETIME   ftUserTime;    // 100 nsec units
/*010*/ FILETIME   ftCreateTime;   // relative to 01-01-1601
/*018*/ DWORD    dWaitTime;
/*01C*/ PVOID    pStartAddress;
/*020*/ CLIENT_ID  Cid;        // process/thread ids
/*028*/ DWORD    dPriority;
/*02C*/ DWORD    dBasePriority;
/*030*/ DWORD    dContextSwitches;
/*034*/ DWORD    dThreadState;   // 2=running, 5=waiting
/*038*/ KWAIT_REASON WaitReason;
/*03C*/ DWORD    dReserved01;
/*040*/ }
    SYSTEM_THREAD, *PSYSTEM_THREAD;
#define SYSTEM_THREAD_ sizeof (SYSTEM_THREAD)
// -----------------------------------------------------------------
typedef struct _SYSTEM_PROCESS     // common members
    {
/*000*/ DWORD     dNext;      // relative offset
/*004*/ DWORD     dThreadCount;
/*008*/ DWORD     dReserved01;
/*00C*/ DWORD     dReserved02;
/*010*/ DWORD     dReserved03;
/*014*/ DWORD     dReserved04;
/*018*/ DWORD     dReserved05;
/*01C*/ DWORD     dReserved06;
/*020*/ FILETIME    ftCreateTime;  // relative to 01-01-1601
/*028*/ FILETIME    ftUserTime;   // 100 nsec units
/*030*/ FILETIME    ftKernelTime;  // 100 nsec units
/*038*/ UNICODE_STRING usName;
/*040*/ KPRIORITY   BasePriority;
/*044*/ DWORD     dUniqueProcessId;
/*048*/ DWORD     dInheritedFromUniqueProcessId;
/*04C*/ DWORD     dHandleCount;
/*050*/ DWORD     dReserved07;
/*054*/ DWORD     dReserved08;
/*058*/ VM_COUNTERS  VmCounters;   // see ntddk.h
/*084*/ DWORD     dCommitCharge;  // bytes
/*088*/ }
    SYSTEM_PROCESS, *PSYSTEM_PROCESS;
#define SYSTEM_PROCESS_ sizeof (SYSTEM_PROCESS)
// -----------------------------------------------------------------
typedef struct _SYSTEM_PROCESS_NT4   // Windows NT 4.0
    {
/*000*/ SYSTEM_PROCESS Process;     // common members
/*088*/ SYSTEM_THREAD aThreads [];   // thread array
/*088*/ }
    SYSTEM_PROCESS_NT4, *PSYSTEM_PROCESS_NT4;
#define SYSTEM_PROCESS_NT4_ sizeof (SYSTEM_PROCESS_NT4)
// -----------------------------------------------------------------
typedef struct _SYSTEM_PROCESS_NT5   // Windows 2000
    {
/*000*/ SYSTEM_PROCESS Process;     // common members
/*088*/ IO_COUNTERS  IoCounters;   // see ntddk.h
/*0B8*/ SYSTEM_THREAD aThreads [];   // thread array
/*0B8*/ }
    SYSTEM_PROCESS_NT5, *PSYSTEM_PROCESS_NT5;
#define SYSTEM_PROCESS_NT5_ sizeof (SYSTEM_PROCESS_NT5)
// -----------------------------------------------------------------
typedef union _SYSTEM_PROCESS_INFORMATION
    {
/*000*/ SYSTEM_PROCESS   Process;
/*000*/ SYSTEM_PROCESS_NT4 Process_NT4;
/*000*/ SYSTEM_PROCESS_NT5 Process_NT5;
/*0B8*/ }
    SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;
#endif
