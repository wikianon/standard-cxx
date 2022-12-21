#ifndef _MDR_TUPLE_H_
#define _MDR_TUPLE_H_

#include <map>
#include <string>
#include "mdr_single.h"

namespace mdr {

//
// Define a mapping of instance name to an instance of an n-tuple.
//

typedef std::map<std::string, mdr_single_t> tuple_map_t;
typedef tuple_map_t::iterator tuple_node_t;

//
// A registry n-tuple is a repeating object, such as PhysicalDisk.
//

class mdr_tuple_t : public mdr_t, public tuple_map_t
{
public:
  mdr_tuple_t(const char *category, const char *instance_list,
              const char *counter_list, const char *reg_host = 0);
 ~mdr_tuple_t();

  const mdr_value_t  &mdr_field_value(const char *key, int field_number);
  const mdr_value_t  &mdr_field_value(const std::string &key, int field_number)
  {
    return mdr_field_value(key.c_str(), field_number);
  }
  const mdr_keyset_t &mdr_keyset(mdr_keyset_t &name_set);
        void          mdr_refresh(void);
		const std::string  &mdr_field_explanation(int field_number, std::string &explanation);

private:
  reg_query_t *query;
  mdr_keyset_t field_names;
};

} // end of namespace pdhcl

#endif
