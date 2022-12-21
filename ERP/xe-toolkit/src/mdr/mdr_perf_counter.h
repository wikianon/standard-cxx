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


#ifndef _MDR_PERF_COUNTER_H_
#define _MDR_PERF_COUNTER_H_

#include <string>
#include <map>
#include <vector>
#include "mdr_category.h"
#include "mdr_perf_host.h"

namespace mdr {

class mdr_performance_counter_t {
public:
  mdr_performance_counter_t(
    const char *category,
    const char *field,
    const char *instance = 0,
    const char *remote_host = 0);
 ~mdr_performance_counter_t();

        bool         mdr_is_supported(void);
  const mdr_value_t &mdr_value(mdr_value_t &value);
  const mdr_value_t &mdr_next_value(mdr_value_t &value);

private:
  int             field_number;
  std::string     instance_name;
  mdr_category_t *mdr_obj;
};

}

#endif
