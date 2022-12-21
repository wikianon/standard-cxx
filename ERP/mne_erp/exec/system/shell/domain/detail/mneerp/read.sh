#!/bin/bash

. exec/system/shell/allg/db.sh

getdomain="SELECT domain, workgroup, typ, description, netdevice, primaryname, primaryaddr, dnsforwarder, dhcpstart, dhcpend FROM mne_system.domain;"
get_domain()
{
    par=$1
    domain=${par/%%%%*};  par=${par#*%%%%}
    workgroup=${par/%%%%*}; par=${par#*%%%%}
    typ=${par/%%%%*}; par=${par#*%%%%}
    description=${par/%%%%*}; par=${par#*%%%%}
    netdevice=${par/%%%%*}; par=${par#*%%%%}
    primaryname=${par/%%%%*}; par=${par#*%%%%}
    primaryaddr=${par/%%%%*}; par=${par#*%%%%}
    dnsforwarder=${par/%%%%*}; par=${par#*%%%%}
    dhcpstart=${par/%%%%*}; par=${par#*%%%%}
    dhcpend=${par/%%%%*}; par=${par#*%%%%}
}
