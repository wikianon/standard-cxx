#include "snmp_tuple.h"

namespace mib {

snmp_tuple_t::snmp_tuple_t(const char *oid_base,
                           const char *hostname,
                           int version,
                           const char *community) :
    snmp_base_t(oid_base, hostname, version, community)
{
    if (has_snmpd(hostname))
        tuple_refresh(true);
}

void snmp_tuple_t::tuple_refresh(bool constructor)
{
    if (!constructor)
        snmp_refresh();

    // there is no list of oids, just one, so get it from begin()
    snmp_aggregate_t &agg = get_oid_list();
    snmp_agg_node_t an = agg.begin();
    snmp_var_list_t &list = *an;
    snmp_var_node_t vn;

    // since the one oid represents a tuple, traverse the vars of the oid
    for(vn=list.begin(); vn != list.end(); vn++) {
        snmp_var_t &var = *vn;
        int next_to_last_octet = var.name[var.name_length - 2];
        int last_octet = var.name[var.name_length - 1];

        // The last octet repeats as 1,2,3,1,2,3, etc. It's the instance #.
        if (instances.size() <= last_octet)
            instances.resize(last_octet + 1);

        // Grab the deque of variables for this instance.
        snmp_var_list_t &vlist = instances[last_octet];

        // Add this var to this instance.
        if (vlist.size() <= next_to_last_octet)
            vlist.resize(next_to_last_octet + 1);
        vlist[next_to_last_octet] = var;
    }
}

}
