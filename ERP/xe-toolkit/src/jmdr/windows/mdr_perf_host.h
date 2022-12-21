#ifndef _MDR_PERF_HOST_H_
#define _MDR_PERF_HOST_H_

#include "mdr.h"
#include "mdr_category.h"
#include "mdr_export.h"
#include <string>

namespace mdr {

class JMDR_API mdr_performance_host_t {
public:
  mdr_performance_host_t(const char *remote_host = 0);

  const mdr_keyset_t &mdr_get_category_names();

  mdr_category_t *mdr_get_category_by_name(const char *category);
  mdr_category_t *mdr_get_category_by_name(const std::string &category)
  {
    return mdr_get_category_by_name(category.c_str());
  }

  std::string &mdr_localized_name(std::string &en_us_name, std::string &localized_name);
  std::string &mdr_en_us_name(std::string &localized_name, std::string &en_us_name);

  void mdr_release_host(void);

private:
  mdr_keyset_t  category_names;
  const char   *remote_host;
};

}
#endif
