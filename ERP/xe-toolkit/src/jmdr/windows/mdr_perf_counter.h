#ifndef _MDR_PERF_COUNTER_H_
#define _MDR_PERF_COUNTER_H_

#include <string>
#include <map>
#include <vector>
#include "mdr_category.h"
#include "mdr_perf_host.h"
#include "mdr_export.h"

namespace mdr {

class JMDR_API mdr_performance_counter_t {
public:
  mdr_performance_counter_t(
    const char *category,
    const char *field,
    const char *instance = 0,
    const char *remote_host = 0);
 ~mdr_performance_counter_t();

        bool         mdr_is_supported(void);
  const mdr_value_t &mdr_value(void);
  const mdr_value_t &mdr_next_value(void);

private:
  int             field_number;
  std::string     instance_name;
  mdr_category_t *mdr_obj;
  mdr_value_t     field_value;
};

}

#endif
