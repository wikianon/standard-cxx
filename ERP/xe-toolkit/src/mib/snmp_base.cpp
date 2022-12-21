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

#include <iostream>
#include "snmp_base.h"
#include "snmp_exception.h"

static const class sock_stuff {
public:
    sock_stuff() {
        init_snmp("cmsnmp");
        SOCK_STARTUP;
    }
   ~sock_stuff() {
        SOCK_CLEANUP;
    }
} global_sock_initializer;

namespace mib {

//
// snmp_base_t constructor with one OID
//

snmp_base_t::snmp_base_t(const char *oid,
                         const char *hostname,
                         int version,
                         const char *community)
{
  oid_storage[0] = oid;
  oid_storage[1] = 0;
  common(oid_storage, hostname, version, community);
}

//
// snmp_base_t constructor with an array of OIDs
//

snmp_base_t::snmp_base_t(const char *const oids[],
                         const char *hostname,
                         int version,
                         const char *community)
{
  common(oids, hostname, version, community);
}

//
// snmp_base_t common code
//

void snmp_base_t::common(const char *const oids[],
                         const char *hostname,
                         int version,
                         const char *community)
{
    netsnmp_session session;

    if (oids == 0)
        throw snmp_exception_t("snmp_base_t::snmp_base_t: null oids");

    // If there's no snmpd, then go no further...
    if (has_snmpd(hostname)) {

        // configure session parameters
        snmp_sess_init(&session);

        session.version = version;
        session.community = (unsigned char *) community;
        session.community_len = strlen(community);
        session.peername = (char *) hostname;

        // open an SNMP session 
        ss = snmp_open(&session);
        if (ss == 0) {
            // diagnose snmp_open errors with the input netsnmp_session pointer 
            const char *msg = snmp_api_errstring(session.s_errno);
            throw snmp_exception_t("snmp_base_t::snmp_base_t: %s", msg);
        }

        oid_string = (char *const *) oids;
        remote_host = (char *) hostname;

        snmp_refresh();
    }
}

void snmp_base_t::snmp_refresh(void)
{
    netsnmp_pdu     *pdu;
    netsnmp_variable_list *vars;
    netsnmp_pdu     *response;
    int             i;
    oid             name[MAX_OID_LEN];
    size_t          name_length;
    oid             root[MAX_OID_LEN];
    size_t          rootlen;
    bool            running = true;
    int             status;

    // count oids
    for(i=0; oid_string[i]; i++)
        ;

    // verify adequate storage
    if (oid_list.size() <= i)
        oid_list.resize(i);

    // build a new var list for each oid
    for(i=0; oid_string[i]; i++) {

        rootlen = MAX_OID_LEN;
        if (snmp_parse_oid(oid_string[i], root, &rootlen) == 0) {
            const char *msg = snmp_api_errstring(snmp_errno);
            throw snmp_exception_t("snmp_base_t::snmp_base_t: %s", msg);
        }

        memmove(name, root, rootlen * sizeof(oid));
        name_length = rootlen;

        oid_list[i].clear();
        running = true;

        while(running) {
            // create PDU for GETNEXT request and add object name to request 
            pdu = snmp_pdu_create(SNMP_MSG_GETNEXT);
            snmp_add_null_var(pdu, name, name_length);

            // do the request 
            response = 0;
            status = snmp_synch_response(ss, pdu, &response);
            if (status == STAT_SUCCESS) {
                if (response->errstat == SNMP_ERR_NOERROR) {
                    // check resulting variables 
                    for (vars = response->variables; vars;
                         vars = vars->next_variable) {
                        if ((vars->name_length < rootlen)
                            || (memcmp(root, vars->name, rootlen * sizeof(oid))
                                != 0)) {
                            // not part of this subtree 
                            running = false;
                            if (response)
                                snmp_free_pdu(response);
                            continue;
                        }
                        snmp_var_t new_var;

                        convert_var(vars, &new_var);
                        oid_list[i].push_back(new_var);

                        if ((vars->type != SNMP_ENDOFMIBVIEW) &&
                            (vars->type != SNMP_NOSUCHOBJECT) &&
                            (vars->type != SNMP_NOSUCHINSTANCE)) {
                            // not an exception value 
                            if (snmp_oid_compare(name, name_length,
                                                vars->name,
                                                vars->name_length) >= 0) {
                                running = false;
                                if (response)
                                    snmp_free_pdu(response);
                                continue;
                            }
                            memmove((char *) name, (char *) vars->name,
                                    vars->name_length * sizeof(oid));
                            name_length = vars->name_length;
                        } else {
                            // an exception value, so stop 
                            running = false;
                            if (response)
                                snmp_free_pdu(response);
                            continue;
                        }
                    }
                } else {
                    std::string msg("snmp_base_t::snmp_base_t: ");

                    // error in response, print it 
                    if (response->errstat == SNMP_ERR_NOSUCHNAME) {
                        msg += "MIB name not found";
                    } else {
                        msg += "Error in packet; reason: ";
                        msg += snmp_errstring(response->errstat);
                        if (response->errindex != 0) {
                            int count;

                            msg += "Failed object: ";
                            for (count = 1, vars = response->variables;
                                 vars && count != response->errindex;
                                 vars = vars->next_variable, count++)
                                /*EMPTY*/;
                            if (vars)
                                msg += (char *) vars->name;
                        }
                    }
                    throw snmp_exception_t(msg.c_str());
                }
            } else if (status == STAT_TIMEOUT) {
                throw snmp_exception_t(
                    "snmp_base_t::snmp_base_t: Timeout: No Response from %s",
                        remote_host);
            } else {                /* status == STAT_ERROR */
                throw snmp_exception_t(
                    "snmp_base_t::snmp_base_t: %s",
                         snmp_api_errstring(snmp_errno));
            }

            if (response)
                snmp_free_pdu(response);
        }
    }
}

void snmp_base_t::convert_var(netsnmp_variable_list *src, snmp_var_t *dest)
{
    dest->type = src->type;
    memcpy(dest->name, src->name, src->name_length * sizeof(oid));
    dest->name_length = src->name_length;

    switch(dest->type) {
    // 32 bit integer
    case ASN_INTEGER:
    case ASN_COUNTER:
    case ASN_GAUGE:
    case ASN_TIMETICKS:
        dest->val.integer = (long) *src->val.integer;
        break;
    // 64 bit integer
    case ASN_COUNTER64:
        dest->val.integer64 = *src->val.counter64;
        break;
    // string
    case ASN_OCTET_STR:
        dest->string = (char *) src->val.string;
        dest->string = dest->string.substr(0, src->val_len);
        dest->val_length = src->val_len;
        break;
    default:
        dest->type = ASN_OPAQUE;
        break;
    }
}

// destroyer
snmp_base_t::~snmp_base_t()
{
    if (ss)
        snmp_close(ss);
}

snmpd_map_t snmp_base_t::snmpd_map;
pthread_mutex_t snmp_base_t::snmpd_mutex = PTHREAD_MUTEX_INITIALIZER;

bool snmp_base_t::has_snmpd(const char *hostname)
{
    pthread_mutex_lock(&snmpd_mutex);
    if (snmpd_map.find(hostname) == snmpd_map.end()) {
        netsnmp_session session;

        snmp_sess_init(&session);

        session.version = 1;
        session.community = (unsigned char *) "public";
        session.community_len = 6;
        session.peername = (char *) hostname;
        session.retries = 0;
        session.timeout = 2000000; // 2 seconds

        snmp_session *ss = snmp_open(&session);
        if (ss == 0) {
            snmpd_map[hostname] = false;
            pthread_mutex_unlock(&snmpd_mutex);
            return false;
        }

        oid    name[MAX_OID_LEN];
        size_t name_length = MAX_OID_LEN;

        if (snmp_parse_oid("1.3.6.1.2.1.1.5.0", name, &name_length) == 0) {
            snmp_close(ss);
            snmpd_map[hostname] = false;
            pthread_mutex_unlock(&snmpd_mutex);
            return false;
        }

        netsnmp_pdu *pdu = snmp_pdu_create(SNMP_MSG_GET);
        int          status;
        netsnmp_pdu *response;

        snmp_add_null_var(pdu, name, name_length);
        status = snmp_synch_response(ss, pdu, &response);
        snmpd_map[hostname] = (status == STAT_SUCCESS);

        snmp_free_pdu(response);
        snmp_close(ss);
    }

    bool result = snmpd_map[hostname];
    pthread_mutex_unlock(&snmpd_mutex);
    return result;
}

} // end of namespace
