/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Windows plugin library for CaptiveCollector.
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


#include "Stdafx.h"
#include "sysinfo.h"
#include "plugin_ex.h"
#include <time.h>

sysinfo_t::sysinfo_t()
{
    SYSTEM_INFO si;
    DWORD error;
    HKEY key;
    int i;
    int online = 0;

    GetSystemInfo(&si);
    pageSize = (int) si.dwPageSize;
    cpuCountConfigured = (int) si.dwNumberOfProcessors;

    for (i = 0; i < 32; i++)
        if (si.dwActiveProcessorMask & (1 << i))
            online++;

    cpuCountOnline = online;

    clockTicksPerSecond = CLOCKS_PER_SEC;

    LPCSTR regent = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";

    error =
        RegOpenKeyEx(HKEY_LOCAL_MACHINE, regent, 0, KEY_QUERY_VALUE, &key);

    if (error != ERROR_SUCCESS)
    {
        throw plugin_exception("sysinfo_t::sysinfo_t: RegOpenKeyEx fails");
    }

    char identifier[BUFSIZ];
    void *p =
        read_value(key, "Identifier", REG_SZ, identifier, sizeof identifier);
    if (p == 0)
    {
        RegCloseKey(key);
        throw
            plugin_exception("sysinfo_t::sysinfo_t: cannot read Identifier");
    }

    trimout(p, sizeof identifier);
    cpuArchitecture.assign((const char *) p);

    char processorName[BUFSIZ];
    p = read_value(key, "ProcessorNameString", REG_SZ,
                   processorName, sizeof processorName);
    if (p == 0)
    {
        RegCloseKey(key);
        throw plugin_exception(
            "sysinfo_t::sysinfo_t: cannot read ProcessorNameString");
    }

    trimout(p, sizeof processorName);
    cpuModel.assign((const char *) p);

    char MHz[BUFSIZ];
    p = read_value(key, "~MHz", REG_DWORD, MHz, sizeof MHz);
    if (p == 0)
    {
        RegCloseKey(key);
        throw plugin_exception("sysinfo_t::sysinfo_t: cannot read ~MHz");
    }

    cpuClockSpeed = (int) *((DWORD *) p);

    RegCloseKey(key);


    regent = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";

    error =
        RegOpenKeyEx(HKEY_LOCAL_MACHINE, regent, 0, KEY_QUERY_VALUE, &key);

    if (error != ERROR_SUCCESS)
    {
        throw plugin_exception("sysinfo_t::sysinfo_t: RegOpenKeyEx fails");
    }

    char productName[BUFSIZ];
    p = read_value(key, "ProductName", REG_SZ, productName,
                   sizeof productName);
    if (p == 0)
    {
        RegCloseKey(key);
        throw plugin_exception(
            "sysinfo_t::sysinfo_t: cannot read ProductName");
    }
    trimout(p, sizeof productName);
    osPlatform.assign((const char *) p);

    char currentVersion[BUFSIZ];
    p = read_value(key, "CurrentVersion", REG_SZ,
        currentVersion, sizeof currentVersion);
    if (p == 0)
    {
        RegCloseKey(key);
        throw plugin_exception(
            "sysinfo_t::sysinfo_t: cannot read CurrentVersion");
    }
    trimout(p, sizeof currentVersion);
    osVersion.assign((const char *) p);

    RegCloseKey(key);

    MEMORYSTATUS gms;

    GlobalMemoryStatus(&gms);
    MBytesPagingFileTotal =
        (int) ((gms.dwTotalPageFile - gms.dwTotalPhys) / (1024 * 1024));

    HMODULE hModule = LoadLibrary("ntdll.dll");
    if (hModule == 0)
    {
        throw plugin_exception("sysinfo_t::sysinfo_t: cannot attach ntdll.dll");
    }

    FARPROC nqsi = GetProcAddress(hModule, "NtQuerySystemInformation");
    if (nqsi == 0)
    {
        throw plugin_exception(
            "sysinfo_t::sysinfo_t: cannot attach NtQuerySystemInformation");
    }

    typedef NTSTATUS(NTAPI *NQSI)(
        IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
        OUT PVOID SystemInformation,
        IN ULONG SystemInformationLength,
        OUT PULONG ReturnLength OPTIONAL);

    NQSI func = (NQSI) nqsi;

    SYSTEM_BASIC_INFORMATION sbi;

#define SBI_PHYSPAGES(s) (((ULONG *) &(s))[3])

    (*func) (SystemBasicInformation, &sbi, sizeof sbi, 0);

    double physp = (double) SBI_PHYSPAGES(sbi);
    physp *= (double) si.dwPageSize;

    MBytesMemoryTotal = (int) (ULONG) (physp / (1024 * 1024));
}

void sysinfo_t::refresh(void)
{
    SYSTEM_INFO si;
    int i;
    int online = 0;
    MEMORYSTATUS gms;

    GetSystemInfo(&si);
    for (i = 0; i < 32; i++)
        if (si.dwActiveProcessorMask & (1 << i))
            online++;

    cpuCountOnline = online;

    GlobalMemoryStatus(&gms);
    MBytesPagingFileTotal =
        (int) ((gms.dwTotalPageFile - gms.dwTotalPhys) / (1024 * 1024));
}

void *sysinfo_t::read_value(HKEY key, LPCSTR name, DWORD type, char buf[],
                            DWORD sz)
{
    DWORD error;
    DWORD returnType;

    error = RegQueryValueEx(key, name, 0, &returnType, (BYTE *) buf, &sz);

    if (error != ERROR_SUCCESS && error != ERROR_MORE_DATA)
        return 0;

    if (returnType != type)
        return 0;

    return (void *) buf;
}

void sysinfo_t::trimout(void *s, DWORD sz)
{
    char *p;
    char *q;
    bool altered = false;

    for (p = (char *) s; *p; p++)
        if (*p == ' ' || *p == '\t')
        {
            altered = true;
            p++;
        }
        else
            break;
    q = p;
    p += strlen(p) - 1;
    while (*p == ' ' || *p == '\t')
    {
        altered = true;
        p--;
    }
    *(p + 1) = '\0';

    if (altered)
        strcpy_s((char *) s, sz, q);
}
