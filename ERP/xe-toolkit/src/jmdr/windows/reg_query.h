#ifndef _REG_QUERY_H_
#define _REG_QUERY_H_

#include <Pdh.h>
#include "mdr_value.h"
#include "mdr_export.h"

namespace mdr {

class JMDR_API reg_query_t
{
public:
  reg_query_t();
 ~reg_query_t();

  void add_counter(const char *full_name, PDH_HCOUNTER &counter_handle);
  void remove_counter(PDH_HCOUNTER &counter_handle);
  void perform_query(void);
  void get_counter_data(PDH_HCOUNTER counter_handle, mdr_value_t &rval);
private:
  int obj_index;
  PDH_HQUERY qh;

  PDH_STATUS get_counter_value(
    PDH_HCOUNTER counter_handle,
    DWORD format, DWORD *counter_type,
    PDH_FMT_COUNTERVALUE *formatted_value,
    bool is_rate);
};

}
#endif
