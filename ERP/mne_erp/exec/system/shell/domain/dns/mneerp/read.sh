#!/bin/bash

. exec/system/shell/allg/db.sh

getalladdress="SELECT dnsaddressid, name, address, record, fix, mac FROM mne_system.dnsaddress;"
getfixaddress="SELECT dnsaddressid, name, address, record, fix, mac FROM mne_system.dnsaddress WHERE fix = true;"
getdnsaddress="SELECT dnsaddressid, name, address, record, fix, mac FROM mne_system.dnsaddress WHERE dnsaddressid = 'par1';"
get_dnsaddress()
{
    par=$1
    dnsaddressid=${par/%%%%*};  par=${par#*%%%%}
    name=${par/%%%%*};  par=${par#*%%%%}
    address=${par/%%%%*}; par=${par#*%%%%}
    record=${par/%%%%*}; par=${par#*%%%%}
    fix=${par/%%%%*}; par=${par#*%%%%}
    macaddr=${par/%%%%*}; par=${par#*%%%%}
}

cleardnsaddress="DELETE FROM mne_system.dnsaddress WHERE modifydate < par1;"


