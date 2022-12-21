#!/bin/bash

. exec/system/shell/allg/db.sh

getmods="SELECT apachemodid, enabled FROM mne_system.apachemod;"
get_mods()
{
    par=$1
    name=${par/%%%%*};    par=${par#*%%%%}
    enabled=${par/%%%%*}; par=${par#*%%%%}
}
