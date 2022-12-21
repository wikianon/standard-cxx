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


#include "stdafx.h"
#include "diskinfo.h"

void diskinfo_t::refresh(void)
{
    int i;
    HANDLE handle;
    char diskName[BUFSIZ];
    DWORD dwBytesReturned;
    DWORD dwRetCode;
    DISK_GEOMETRY dg;
    char c;
    char root[16];
    ULARGE_INTEGER total;
    DWORD logicalDrives = GetLogicalDrives();
    DWORD type;
    const char *driveType;
    dinfo_can_t can;
    char name[128];

    // start over
    clear();

    for (i = 0;; i++) {
        _snprintf_s(diskName, sizeof diskName, "\\\\.\\PHYSICALDRIVE%d", i);

        handle = CreateFile(diskName,
                            GENERIC_READ,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            NULL, OPEN_EXISTING, 0, NULL);
        if (handle == INVALID_HANDLE_VALUE)
            break;
        else {
            dwRetCode = DeviceIoControl(handle,
                                        IOCTL_DISK_GET_DRIVE_GEOMETRY,
                                        NULL,
                                        0,
                                        &dg,
                                        sizeof(dg), &dwBytesReturned, NULL);
            CloseHandle(handle);

            LONGLONG bytes = dg.BytesPerSector;
            bytes *= dg.SectorsPerTrack;
            bytes *= dg.TracksPerCylinder;
            bytes *= dg.Cylinders.QuadPart;

            can.clear();
            can.diskType = "physical";
            can.capacityMB = (ULONG) (bytes / (1024LL * 1024LL));

            _snprintf_s(name, sizeof name, sizeof name, "%d", i);
            (*this)[name] = can;
        }
    }

    SetErrorMode(SEM_FAILCRITICALERRORS);

    strcpy_s(root, sizeof root, "C:\\");

    for (c = 'C'; c <= 'Z'; c++) {
        if ((logicalDrives & (1UL << (c - 'A'))) == 0)
            continue;

        *root = c;

        if (!GetDiskFreeSpaceEx(root, 0, &total, 0))
            memset(&total, '\0', sizeof total);

        type = GetDriveType(root);
        switch (type) {
        case DRIVE_UNKNOWN:
        case DRIVE_NO_ROOT_DIR:
            driveType = "unknown";
            break;
        case DRIVE_CDROM:
        case DRIVE_FIXED:
        case DRIVE_REMOVABLE:
            driveType = "logical";
            break;
        case DRIVE_REMOTE:
            driveType = "network";
            break;
        case DRIVE_RAMDISK:
            driveType = "ram";
            break;
        }

        can.clear();
        can.diskType = driveType;
        can.capacityMB = (ULONG) (total.QuadPart / (1024ULL*1024ULL));

        std::string key(root);

        (*this)[key.substr(0, key.length() - 1)] = can;
    }
}

mdr::mdr_keyset_t &diskinfo_t::getInstanceNames(mdr::mdr_keyset_t &keyset)
{
    dbox_node_t node;
    int i;

    keyset.clear();
    keyset.resize(size());
    for(i=0, node=begin(); node != end(); node++, i++)
        keyset[i] = node->first;

    return keyset;
}
