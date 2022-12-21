/****************************************************************************
**
** Copyright (c) 2006-2008 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the mib library of Captivity.
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

#ifndef _SNMP_BASE_H_
#define _SNMP_BASE_H_

#ifdef _SOLARIS
# include "snmp_solaris.h"
#endif
#ifdef _LINUX
# include "snmp_linux.h"
#endif
#ifdef _AIX
# include "snmp_aix.h"
#endif
#ifdef _DARWIN
# include "snmp_darwin.h"
#endif
#ifdef _FREEBSD
# include "snmp_freebsd.h"
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include <sys/types.h>
#if HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif
#if TIME_WITH_SYS_TIME
# ifdef WIN32
#  include <sys/timeb.h>
# else
#  include <sys/time.h>
# endif
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#include <stdio.h>
#if HAVE_WINSOCK_H
#include <winsock.h>
#endif
#if HAVE_NETDB_H
#include <netdb.h>
#endif
#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#include <pthread.h>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include <string>
#include <deque>
#include <map>

namespace mib {

class snmp_var_t {
public:
    u_char      type;
    std::string string;
    oid         name[MAX_OID_LEN];
    size_t      name_length;
    union {
        long        integer;
        counter64   integer64;
        float       floatVal;
        double      doubleVal;
    } val;
    size_t      val_length;

    snmp_var_t() {
        clear();
    }

    void clear(void) {
        type = 0;
        string.clear();
        memset(&name, '\0', sizeof name);
        name_length = 0;
        val.doubleVal = 0.0;
    }
};

typedef std::deque<snmp_var_t>       snmp_var_list_t;
typedef snmp_var_list_t::iterator    snmp_var_node_t;
typedef std::deque<snmp_var_list_t>  snmp_aggregate_t;
typedef snmp_aggregate_t::iterator   snmp_agg_node_t;
typedef std::map<std::string, bool>  snmpd_map_t;

class snmp_base_t {
public:
    snmp_base_t(const char *oid_string,
                const char *hostname = "localhost",
                int version = SNMP_VERSION_1,
                const char *community = "public");
    snmp_base_t(const char *const oid_string[],
                const char *hostname = "localhost",
                int version = SNMP_VERSION_1,
                const char *community = "public");
   ~snmp_base_t();

    snmp_aggregate_t &get_oid_list(void)
    {
        return oid_list;
    }

    static bool has_snmpd(const char *hostname = "localhost");
protected:
    void snmp_refresh(void);

    uint_t wrap_diff_32(uint_t a, uint_t b)
    {
        if (a >= b)
            return a - b;

        return (uint_t) (((double) a + 4294967296.0) - b);
    }
private:
    netsnmp_session       *ss;
    snmp_aggregate_t       oid_list;
    char *const            *oid_string;
    const char *oid_storage[2]; // for constructor with 1 oid
    char                   *remote_host;
    static snmpd_map_t     snmpd_map;
    static pthread_mutex_t snmpd_mutex;

    void common(const char *const oid_string[],
                const char *hostname,
                int version,
                const char *community);
    void convert_var(netsnmp_variable_list *src, snmp_var_t *dest);
};

}

#endif
