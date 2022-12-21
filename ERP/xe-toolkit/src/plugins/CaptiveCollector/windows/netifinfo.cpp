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
#include "netifinfo.h"

static const int MAX_KEY_LENGTH = 255;
static const int MAX_VALUE_NAME = 16383;

void *netif_info_t::read_value(HKEY key, LPCSTR name, DWORD type, PBYTE buf, DWORD sz)
{
    DWORD error;
    DWORD returnType;

    error = RegQueryValueEx(key, name, 0, &returnType, buf, &sz);

    if (error != ERROR_SUCCESS && error != ERROR_MORE_DATA)
        return 0;

    if (returnType != type)
        return 0;

    return (void *) buf;
}

void netif_info_t::query_key(HKEY hKey, mdr::mdr_keyset_t *subkeys, mdr::mdr_keyset_t *values) 
{ 
    char     achKey[MAX_KEY_LENGTH]; // buffer for subkey name
    DWORD    cbName;                 // size of name string 
    DWORD    cSubKeys=0;             // number of subkeys 
    DWORD    cValues;                // number of values for key 
    DWORD    i;
    DWORD    retCode; 
    char     achValue[MAX_VALUE_NAME]; 
    DWORD    cchValue = MAX_VALUE_NAME; 
    int      sk = 0;
    int      v = 0;

    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,      // key handle 
        0,         // buffer for class name 
        0,         // size of class string 
        0,         // reserved 
        &cSubKeys, // number of subkeys 
        0,         // longest subkey size 
        0,         // longest class string 
        &cValues,  // number of values for this key 
        0,         // longest value name 
        0,         // longest value data 
        0,         // security descriptor 
        0);        // last write time 

    // Enumerate the subkeys, until RegEnumKeyEx fails.

    if (cSubKeys && subkeys)
    {
        subkeys->resize(cSubKeys);
        for (i=0; i<cSubKeys; i++) 
        { 
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(hKey, i,
                achKey, 
                &cbName, 
                0, 
                0, 
                0, 
                0);
            if (retCode == ERROR_SUCCESS) 
                (*subkeys)[sk++] = achKey;
        }
    } 

    // Enumerate the key values. 

    if (cValues && values) 
    {
        values->resize(cValues);
        for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
        { 
            cchValue = MAX_VALUE_NAME; 
            achValue[0] = '\0'; 
            retCode = RegEnumValue(hKey, i, 
                achValue, 
                &cchValue, 
                0, 
                0,
                0,
                0);

            if (retCode == ERROR_SUCCESS ) 
                (*values)[v++] = achValue;
        }
    }
}

static void transmogrify(char *s)
{
    // if there are other rules to apply to interface names, they should
    // be placed here.

    char *p;

    // Rule 1:
    //
    // substitution:
    //   a) slash for underscore

    for(p=s; *p; p++)
        if (*p == '/')
            *p = '_';
}

void netif_info_t::refresh(void)
{
    HKEY key;
    const char *regent = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards";
    ibox_node_t node;

    // gather up the interface names and their associated service names

    // open the network cards key
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, regent, 0, KEY_READ, &key) == ERROR_SUCCESS)
    {
        mdr::mdr_keyset_t subkeys;
        mdr::mdr_keyset_t values;
        size_t i;
        size_t n;

        // now query the key for its subkeys
        query_key(key, &subkeys, 0);

        // for each subkey
        for(n=subkeys.size(), i=0; i<n; i++)
        {
            std::string newRegent(regent);
            HKEY newKey;
            BYTE svcName[BUFSIZ];
            BYTE description[BUFSIZ];

            // build a new name with a subkey
            newRegent += "\\";
            newRegent += subkeys[i];

            // open that key
            if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, newRegent.c_str(),
                                0, KEY_READ, &newKey) == ERROR_SUCCESS)
            {
                // gather the desired fields
                const char *s = (const char *)
                    read_value(newKey, "ServiceName", REG_SZ, svcName, sizeof(svcName));
                char *d = (char *)
                    read_value(newKey, "Description", REG_SZ, description, sizeof(description));

                // yet another hack for M$'s mistake
                transmogrify(d);

                // subscript (key) is the interface name
                (*this)[d].serviceName = s;

                RegCloseKey(newKey);
            }
        }

        RegCloseKey(key);
    }

    // now move on to the interfaces key
    regent = "SYSTEM\\ControlSet001\\Services\\Tcpip\\Parameters\\Interfaces\\";

    // for each interface found
    for(node=begin(); node != end(); node++)
    {
        iinfo_can_t *ip = &node->second;
        std::string newRegent(regent);
        BYTE dhcpAddress[BUFSIZ];
        BYTE dhcpMask[BUFSIZ];
        BYTE staticAddress[BUFSIZ];
        BYTE staticMask[BUFSIZ];

        // build a new name with the service name
        newRegent += ip->serviceName;

        // open the key
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, newRegent.c_str(),
                            0, KEY_READ, &key) == ERROR_SUCCESS)
        {
            DWORD enableDHCP = 0;
            const char *da = 0;
            const char *dm = 0;
            const char *sa = 0;
            const char *sm = 0;

            read_value(key, "EnableDHCP", REG_DWORD, (PBYTE) &enableDHCP, sizeof(enableDHCP));

            if (enableDHCP) {
              da = (const char *)
                read_value(key, "DhcpIPAddress", REG_SZ,
                  dhcpAddress, sizeof(dhcpAddress));
              dm = (const char *)
                read_value(key, "DhcpSubnetMask", REG_SZ,
                  dhcpMask, sizeof(dhcpMask));
            } else {
              sa = (const char *)
                read_value(key, "IPAddress", REG_MULTI_SZ,
                  staticAddress, sizeof(staticAddress));
              sm = (const char *)
                read_value(key, "SubnetMask", REG_MULTI_SZ,
                  staticMask, sizeof(staticMask));
            }

            // dhcp address is present and not equal to 0.0.0.0
            if (da && strcmp(da, "0.0.0.0")) {
              ip->ipAddress = da;
              ip->subnetMask = (dm ? dm : "???");
            } else if (sa && strcmp(sa, "0.0.0.0")) {
              ip->ipAddress = sa;
              ip->subnetMask = (sm ? sm : "???");
            } else {
              ip->ipAddress = "???";
              ip->subnetMask = "???";
            }

            RegCloseKey(key);
        }
    }
}

mdr::mdr_keyset_t &netif_info_t::getInstanceNames(mdr::mdr_keyset_t &keyset)
{
    ibox_node_t node;
    int i;

    keyset.clear();
    keyset.resize(size());
    for(i=0, node=begin(); node != end(); node++, i++)
        keyset[i] = node->first;

    return keyset;
}