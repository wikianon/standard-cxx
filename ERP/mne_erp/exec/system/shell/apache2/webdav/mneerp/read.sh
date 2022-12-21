#!/bin/bash

. exec/system/shell/allg/db.sh

getreleases="SELECT DISTINCT t0.name, t0.location, t0.description FROM mne_application.folder t0 LEFT JOIN  mne_system.shares t1 ON ( t0.folderid = t1.folderid ) WHERE t1.folderid IS NOT NULL;"
get_releases()
{
    par=$1
    name=${par/%%%%*};        par=${par#*%%%%}
    location=${par/%%%%*};    par=${par#*%%%%}
    description=${par/%%%%*}; par=${par#*%%%%}
}
