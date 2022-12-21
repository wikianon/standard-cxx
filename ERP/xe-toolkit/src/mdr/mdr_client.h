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


#ifndef _MDR_CLIENT_H_
#define _MDR_CLIENT_H_

#include <map>
#include <pthread.h>
#include "nilib.h"
#include "nicmd.h"
#include "mdr_value.h"

namespace mdr {

const int MDRD_PORT = 12760;
const int MDRD_QLEN = 25;

const char *const MC_FDELIM  = "\1";
const char *const MC_RDELIM  = "\2";
const char *const MC_KVDELIM = "\3";

// singleton member map; field_id=value
typedef std::map<int, mdr_value_t> mdr_s_mm_t;
typedef mdr_s_mm_t::iterator mdr_s_mm_node_t;

// multi member map; key=singleton_map
typedef std::map<std::string, mdr_s_mm_t> mdr_m_mm_t;
typedef mdr_m_mm_t::iterator mdr_m_mm_node_t;

class mdr_client_t {
public:
  bool   mc_category_supported;
  fd_set mc_support_mask;

  mdr_client_t(const char *class_name, const char *remote_host, int version);
 ~mdr_client_t();

  void mc_refresh(mdr_s_mm_t &s_map, const char *rpc = "mdr_refresh");
  void mc_release(mdr_s_mm_t &s_map);

  void mc_refresh(mdr_m_mm_t &m_map);
  void mc_release(mdr_m_mm_t &m_map);

  int  mc_call(const char *rpc, iovec_t *to = 0, iovec_t *from = 0);

private:
  struct context_t {
    int client_socket;
    int object_count;
  };
  typedef std::map<std::string, context_t> context_map_t;
  typedef context_map_t::iterator context_node_t;

  static context_map_t   host_map;
  static pthread_mutex_t host_mux;

  const char *mc_class_name;
  const char *mc_remote_host;
  int         mc_version;

  void build_singleton_map(char *data, mdr_s_mm_t &s_map);
};

} // end of namespace mdr

#endif
