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


#ifndef _MDR_PERF_HOST_H_
#define _MDR_PERF_HOST_H_

#include "mdr.h"
#include "mdr_category.h"
#include <string>

namespace mdr {

// version 1
const int MPH_VERSION = 1;

class mdr_performance_host_t {
public:
  enum mdr_os_flavor_t {
    MOS_LINUX     = 1,
    MOS_SOLARIS   = 2,
    MOS_AIX       = 4,
    MOS_HPUX      = 8,
    MOS_MACOS     = 16,
    MOS_BSD       = 32
  };

  mdr_os_flavor_t mdr_os_flavor;

  mdr_performance_host_t(const char *remote_host = 0);
 ~mdr_performance_host_t();

  const mdr_keyset_t &mdr_get_category_names(void);

  mdr_category_t *mdr_get_category_by_name(const char *category);
  mdr_category_t *mdr_get_category_by_name(const std::string &category)
  {
    return mdr_get_category_by_name(category.c_str());
  }

  std::string &mdr_localized_name(std::string &en_us_name, std::string &localized_name);
  std::string &mdr_en_us_name(std::string &localized_name, std::string &en_us_name);

  void mdr_release_host(void);

private:
  mdr_keyset_t category_names;
  mdr_client_t *net_client;
  const char   *remote_host;
};

extern std::ostream &operator<<
  (std::ostream &out, const mdr_performance_host_t::mdr_os_flavor_t v);

}
#endif
