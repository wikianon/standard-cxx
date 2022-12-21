#include "snmp_netif.h"

namespace mib {

static const char *netif_oid = "1.3.6.1.2.1.2.2.1";

snmp_netif_t::snmp_netif_t(const char *hostname,
                           int version,
                           const char *community) :
    snmp_tuple_t(netif_oid, hostname, version, community)
{
    if (has_snmpd(hostname))
        netif_refresh(true);
}

void snmp_netif_t::netif_refresh(bool constructor)
{
    if (!constructor)
        tuple_refresh();

    snmp_aggregate_t &agg = get_instances();
    snmp_agg_node_t anode;
    snmp_netif_can_t can;

    // just start over
    clear();

    // for each interface
    for(anode=agg.begin(); anode != agg.end(); anode++) {
        snmp_var_list_t &vlist = *anode;

        // deque resized according to the last octet, which number
        // starting at 1, not 0. Therefore, the first element will
        // have nothing in it.

        if (vlist.size() == 0)
            continue;

        snmp_var_node_t vnode;
        std::string ifDescr;
        int n;

        // get ready for more data
        can.clear();

        // for each counter of the interface
        for(vnode=vlist.begin(); vnode != vlist.end(); vnode++) {
            snmp_var_t &var = *vnode;

            // deque resized according to the next to last octet, which
            // number starting at 1, not 0. Therefore, the first element
            // will have nothing in it.

            if (var.name_length == 0)
                continue;

            int next_to_last_octet = var.name[var.name_length - 2];

            switch(next_to_last_octet) {
            case 1:
                can.ifIndex = var.val.integer;
                break;
            case 2:
                ifDescr = var.string;
                // XXX: gak! hack alert!
                // AIX reports ent0; blah blah blah ... and then ... blah blah
                n = ifDescr.find(';');
                if (n != -1)
                    ifDescr = ifDescr.substr(0, n);
                break;
            case 3:
                can.ifType = var.val.integer;
                break;
            case 4:
                can.ifMtu = var.val.integer;
                break;
            case 5:
                can.ifSpeed = var.val.integer;
                break;
            case 6:
                can.ifPhysAddress = var.string;
                break;
            case 7:
                can.ifAdminStatus = var.val.integer;
                break;
            case 8:
                can.ifOperStatus = var.val.integer;
                break;
            case 9:
                can.ifLastChange = var.val.integer;
                break;
            case 10:
                can.ifInOctets = var.val.integer;
                break;
            case 11:
                can.ifInUcastPkts = var.val.integer;
                break;
            case 12:
                can.ifInNUcastPkts = var.val.integer;
                break;
            case 13:
                can.ifInDiscards = var.val.integer;
                break;
            case 14:
                can.ifInErrors = var.val.integer;
                break;
            case 15:
                can.ifInUnknownProtos = var.val.integer;
                break;
            case 16:
                can.ifOutOctets = var.val.integer;
                break;
            case 17:
                can.ifOutUcastPkts = var.val.integer;
                break;
            case 18:
                can.ifOutNUcastPkts = var.val.integer;
                break;
            case 19:
                can.ifOutDiscards = var.val.integer;
                break;
            case 20:
                can.ifOutErrors = var.val.integer;
                break;
            case 21:
                can.ifOutQLen = var.val.integer;
                break;
            }
        }

        (*this)[ifDescr] = can;
    }
}

}
