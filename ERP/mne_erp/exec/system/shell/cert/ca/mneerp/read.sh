#!/bin/bash

. exec/system/shell/allg/db.sh

getcertconf="SELECT country,state,city,org,orgunit,email FROM mne_system.certca;"
get_certconf()
{
    par=$1
    country=${par/%%%%*};  par=${par#*%%%%}
    state=${par/%%%%*}; par=${par#*%%%%}
    city=${par/%%%%*}; par=${par#*%%%%}
    org=${par/%%%%*}; par=${par#*%%%%}
    orgunit=${par/%%%%*}; par=${par#*%%%%}
    email=${par/%%%%*}; par=${par#*%%%%}
}
