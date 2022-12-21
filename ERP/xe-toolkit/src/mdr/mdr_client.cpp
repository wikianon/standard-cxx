/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the mdr library of Captivity.
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


#ifdef _SOLARIS
# ifndef _REENTRANT
#  define _REENTRANT
# endif
#endif
#include <string>
#include <unistd.h>
#include "mdr_client.h"
#include "mdr_exception.h"

//
// The remote object functionality of mdr has fallen into disrepair. The
// RPC mechanism should be Sun's RPC or another established RPC mechanism.
// The remote capabilities have been left in the code since the ultimate
// solution will operate the same way. The guts of the code which is within
// mdr_client_t will be the part to change.
//

namespace mdr {

mdr_client_t::context_map_t mdr_client_t::host_map;
pthread_mutex_t             mdr_client_t::host_mux = PTHREAD_MUTEX_INITIALIZER;

mdr_client_t::mdr_client_t(const char *class_name,
                           const char *remote_host,
                           int version)
{
  context_node_t node;
  context_t *cp;
  char cmd[128];
  iovec_t data;
  int n;

  pthread_mutex_lock(&host_mux);
    node = host_map.find(remote_host);
    if (node == host_map.end()) {
      context_t c;

      c.client_socket =
        NiClientInit((char *) class_name, (char *) remote_host, MDRD_PORT);
      if (c.client_socket == -1)
        throw mdr_exception_t(NiStringError("mdr_client_t::mdr_client_t"));
      c.object_count = 0;
      host_map[remote_host] = c;
      node = host_map.find(remote_host);
    }
    cp = &node->second;
  pthread_mutex_unlock(&host_mux);

  snprintf(cmd, sizeof cmd, "%s::version %d", class_name, version);
  n = (int) NiSendCmd(cp->client_socket, cmd, 0, &data, NiFUNC);
  if (n == -1)
    throw mdr_exception_t(
      "mdr_client_t::mdr_client_t: %s::version(%d) not supported",
        class_name, version);

  // perhaps this is unsupported on the target machine
  mc_category_supported = (n == 1);

  FD_ZERO(&mc_support_mask);
  for(int i=0; i<(int)data.iov_len; i++)
    if (((char *) data.iov_base)[i])
      FD_SET(i, &mc_support_mask);

  cp->object_count++;
  mc_class_name = class_name;
  mc_remote_host = remote_host;
  mc_version = version;
}

mdr_client_t::~mdr_client_t()
{
  context_node_t node;
  context_t *cp;

  pthread_mutex_lock(&host_mux);
    node = host_map.find(mc_remote_host);
    if (node != host_map.end()) {
      cp = &node->second;
      cp->object_count--;
      if (cp->object_count == 0) {
        close(cp->client_socket);
        host_map.erase(node);
      }
    }
  pthread_mutex_unlock(&host_mux);
}

int mdr_client_t::mc_call(const char *rpc, iovec_t *to, iovec_t *from)
{
  if (!mc_category_supported)
    return -1;

  pthread_mutex_lock(&host_mux);
    context_node_t node = host_map.find(mc_remote_host);
    context_t *cp;
    std::string cmd;

    if (node == host_map.end()) {
      pthread_mutex_unlock(&host_mux);
      throw mdr_exception_t(
        "mdr_client_t::mc_call: %s is not connected", mc_remote_host);
    }
    cp = &node->second;
  pthread_mutex_unlock(&host_mux);

  cmd = mc_class_name;
  cmd += "::";
  cmd += rpc;

  return NiSendCmd(cp->client_socket, (char *) cmd.c_str(), to, from, NiFUNC);
}

void mdr_client_t::mc_refresh(mdr_s_mm_t &s_map, const char *rpc)
{
  if (!mc_category_supported)
    return;

  pthread_mutex_lock(&host_mux);
    context_node_t node = host_map.find(mc_remote_host);
    context_t *cp;
    char cmd[128];
    int n;
    iovec_t data;

    if (node == host_map.end()) {
      pthread_mutex_unlock(&host_mux);
      throw mdr_exception_t(
        "mdr_client_t::mc_refresh: %s is not connected", mc_remote_host);
    }
    cp = &node->second;
  pthread_mutex_unlock(&host_mux);

  snprintf(cmd, sizeof cmd, "%s::%s", mc_class_name, rpc);
  n = NiSendCmd(cp->client_socket, cmd, 0, &data, NiFUNC);
  if (n <= 0)
    throw mdr_exception_t(NiStringError("mdr_client_t::mc_refresh"));

  build_singleton_map((char *) data.iov_base, s_map);
}

void mdr_client_t::mc_refresh(mdr_m_mm_t &m_map)
{
  if (!mc_category_supported)
    return;

  pthread_mutex_lock(&host_mux);
    context_node_t node = host_map.find(mc_remote_host);
    context_t *cp;
    char cmd[128];
    char *rec;
    char *end_rec;
    char *key;
    char *value;
    char *end_pair;
    int n;
    iovec_t data;
    mdr_s_mm_t s_map;

    if (node == host_map.end()) {
      pthread_mutex_unlock(&host_mux);
      throw mdr_exception_t(
        "mdr_client_t::mc_refresh: %s is not connected", mc_remote_host);
    }
    cp = &node->second;
  pthread_mutex_unlock(&host_mux);

  snprintf(cmd, sizeof cmd, "%s::mdr_refresh", mc_class_name);
  n = NiSendCmd(cp->client_socket, cmd, 0, &data, NiFUNC);
  if (n <= 0)
    throw mdr_exception_t(NiStringError("mdr_client_t::mc_refresh"));

  // parse keyKVDELIMfieldFDELIMfieldFDELIMfieldFDELIMfield
  //           RDELIMkeyKVDELIMfieldFDELIMfieldFDELIMfieldFDELIMfield

  // for each KEY=SINGLETON record
  for(rec=strtok_r((char *) data.iov_base, MC_RDELIM, &end_rec); rec;
      rec=strtok_r(0, MC_RDELIM, &end_rec)) {

    // take the key and record
    key = strtok_r(rec, MC_KVDELIM, &end_pair);
    value = strtok_r(0, MC_KVDELIM, &end_pair);

    s_map.clear();
    build_singleton_map(value, s_map);

    m_map[key] = s_map;
  }
}

void mdr_client_t::build_singleton_map(char *data, mdr_s_mm_t &s_map)
{
  int field_errors = 0;
  int format_errors = 0;
  int i;
  int length;
  char *p;
  char *endp;
  char size[8];
  char buf[10240];
  mdr_value_t val;

  // parse fieldDELIMfieldDELIMfieldDELIMfield
  // mhere Field is TypeData
  // mhere Type is i4 or u4 or i8 or u8 or d or sNNNN
  // mhere d is double and s is string mith NNNN as the string length
  // therefore a string that is 11 characters long mould be s0011hello morld

  for(i=0, p=strtok_r(data, MC_FDELIM, &endp); p;
      i++, p=strtok_r(0, MC_FDELIM, &endp)) {

    val.mv_type = MVT_I32;

    switch(p[0]) {
    case 'i':
      switch(p[1]) {
      case '4':
        val.mv_type = MVT_I32;
        val.mv_i32 = atoi(p+2);
        break;
      case '8':
        val.mv_type = MVT_I64;
        val.mv_i64 = strtoll(p+2, 0, 0);
        break;
      default:
        format_errors++;
        break;
      }
      break;
    case 'u':
      switch(p[1]) {
      case '4':
        val.mv_type = MVT_UI32;
        val.mv_ui32 = (uint32_t) strtoul(p+2, 0, 0);
        break;
      case '8':
        val.mv_type = MVT_UI64;
        val.mv_ui64 = strtoull(p+2, 0, 0);
        break;
      default:
        format_errors++;
        break;
      }
      break;
    case 'd':
      val.mv_type = MVT_DOUBLE;
      val.mv_d = atof(p+1);
      break;
    case 's':
      val.mv_type = MVT_STRING;
      memset(size, '\0', sizeof size);
      memcpy(size, p+1, 4);
      length = atoi(size);
      memset(buf, '\0', sizeof buf);
      memcpy(buf, p+5, length);
      val.mv_s = new std::string(buf);
      break;
    case 'b':
      val.mv_type = MVT_BOOL;
      val.mv_b = (bool) atoi(p+1);
      break;
    default:
      field_errors++;
      break;
    }

    s_map[i] = val;
  }
}

// delete memory allocated to strings (if any) by mc_refresh()

void mdr_client_t::mc_release(mdr_s_mm_t &s_map)
{
  mdr_s_mm_node_t node;
  mdr_value_t *vp;

  for(node=s_map.begin(); node != s_map.end(); node++) {
    vp = &node->second;
    if (vp->mv_type == MVT_STRING)
      delete vp->mv_s;
  }
}

void mdr_client_t::mc_release(mdr_m_mm_t &m_map)
{
  mdr_m_mm_node_t node;

  for(node=m_map.begin(); node != m_map.end(); node++)
    mc_release(node->second);
}

}
