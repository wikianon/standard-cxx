#!/bin/bash

. exec/system/shell/allg/db.sh

apacheconf="SELECT port, sport FROM mne_system.apache;"
apache_conf()
{
    par=$1
    port=${par/%%%%*};  par=${par#*%%%%}
    sport=${par/%%%%*}; par=${par#*%%%%}
    
    if [ "$port" = "" ]; then
      port=80;
    fi

    if [ "$sport" = "" ]; then
      sport=443;
    fi
   
}
