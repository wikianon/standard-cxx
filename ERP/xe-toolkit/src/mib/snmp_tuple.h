#ifndef _SNMP_TUPLE_T_
#define _SNMP_TUPLE_T_

#include "snmp_base.h"

namespace mib {

class snmp_tuple_t : public snmp_base_t {
public:
    snmp_tuple_t(const char *oid_base,
                 const char *hostname = "localhost",
                 int version = SNMP_VERSION_1,
                 const char *community = "public");

    snmp_aggregate_t &get_instances(void)
    {
        return instances;
    }
protected:
    void tuple_refresh(bool constructor);
    void tuple_refresh(void)
    {
        tuple_refresh(false);
    }
private:
    snmp_aggregate_t instances;
};

}

#endif
