#!/bin/bash

. exec/system/shell/allg/db.sh

getinterfaces="SELECT networkid, typ, addr, mask, broadcast, gateway, nameserver, domain, search FROM mne_system.network ORDER BY networkid;"
get_interfaces()
{
    par=$1
    device=${par/%%%%*};     par=${par#*%%%%}
    typ=${par/%%%%*};        par=${par#*%%%%}
    addr=${par/%%%%*};       par=${par#*%%%%}
    mask=${par/%%%%*};       par=${par#*%%%%}
    bcast=${par/%%%%*};      par=${par#*%%%%}
    gw=${par/%%%%*};         par=${par#*%%%%}
    nameserver=${par/%%%%*}; par=${par#*%%%%}
    domain=${par/%%%%*};     par=${par#*%%%%}
    search=${par/%%%%*};     par=${par#*%%%%}
}
