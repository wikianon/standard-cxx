#ifndef _MDR_SINGLE_H_
#define _MDR_SINGLE_H_

#include <windows.h>
#include <map>
#include <string>
#include "mdr.h"
#include "reg_query.h"

namespace mdr {

//
// A registry singleton is a non-repeating object, such as Server.
//

typedef std::map<int, PDH_HCOUNTER> cnt_list_t;
typedef cnt_list_t::iterator        cnt_node_t;

typedef std::map<int, mdr_value_t> counter_list_t;
typedef counter_list_t::iterator   counter_node_t;

class mdr_single_t : public mdr_t, public counter_list_t
{
public:
  int        mdr_instance_count;   // for duplicate instance names in tuple
  cnt_list_t mdr_cnt_list;

  // need default constructor, at least, for tuple's map of singles
  mdr_single_t(const char *cat = 0, const char *counter_list = 0, const char *reg_host = 0);
 ~mdr_single_t();

  void clear(void)
  {
    mdr_instance_count = 1;
    query = 0;
  }

  const std::string &mdr_field_explanation(int field_number, std::string &explanation);
  const mdr_value_t &mdr_field_value(int field_number);
               void  mdr_refresh(void);

private:
  reg_query_t    *query;
};

} // end of namespace mdr

#endif
