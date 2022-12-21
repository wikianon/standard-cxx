#ifndef _SNMP_NETIF_H_
#define _SNMP_NETIF_H_

#include "snmp_tuple.h"

namespace mib {

class snmp_netif_can_t {
public:
    int         ifIndex;            // interface slot
    int         ifType;             // csmacd, etc.
    int         ifMtu;              // maximum transfer unit
    uint32_t    ifSpeed;            // in bits per second
    std::string ifPhysAddress;      // MAC address
    int         ifAdminStatus;      // desired: up(1) down(2) testing(3)
    int         ifOperStatus;       // current: ditto
    int         ifLastChange;       // time entered current state
    uint32_t    ifInOctets;         // bytes in
    uint32_t    ifInUcastPkts;      // unicast packets in
    uint32_t    ifInNUcastPkts;     // non-unicast packets in
    uint32_t    ifInDiscards;       // non-error in-packets discarded
    uint32_t    ifInErrors;         // error in-packets discarded
    uint32_t    ifInUnknownProtos;  // unsupp-proto in-packets discarded
    uint32_t    ifOutOctets;        // bytes out
    uint32_t    ifOutUcastPkts;     // unicast packets out
    uint32_t    ifOutNUcastPkts;    // non-unicast packets out
    uint32_t    ifOutDiscards;      // non-error out-packets discarded
    uint32_t    ifOutErrors;        // error out-packets discarded
    uint32_t    ifOutQLen;          // length of output packet queue

    snmp_netif_can_t() {
        clear();
    }

    void clear(void) {
        ifIndex = 0;
        ifType = 0;
        ifMtu = 0;
        ifSpeed = 0;
        ifPhysAddress.clear();
        ifAdminStatus = 0;
        ifOperStatus = 0;
        ifLastChange = 0;
        ifInOctets = 0;
        ifInUcastPkts = 0;
        ifInNUcastPkts = 0;
        ifInDiscards = 0;
        ifInErrors = 0;
        ifInUnknownProtos = 0;
        ifOutOctets = 0;
        ifOutUcastPkts = 0;
        ifOutNUcastPkts = 0;
        ifOutDiscards = 0;
        ifOutErrors = 0;
        ifOutQLen = 0;
    }
};

typedef std::map<std::string, snmp_netif_can_t> snmp_netif_box_t;
typedef snmp_netif_box_t::iterator              snmp_netif_node_t;

class snmp_netif_t : public snmp_tuple_t, public snmp_netif_box_t {
public:
    snmp_netif_t(const char *hostname = "localhost",
                 int version = SNMP_VERSION_1,
                 const char *community = "public");

    void netif_refresh()
    {
        netif_refresh(false);
    }
private:
    void netif_refresh(bool constructor);
};

}

#endif
