#!/bin/bash

. exec/system/shell/allg/db.sh

bs='\\'
getallsites="SELECT t0.apachesiteid AS apachesiteid,
                 t0.enabled AS enabled,
                 t0.domain AS domain,
                 t0.alias AS alias,
                 CASE WHEN substring(t0.documentroot from 1 for 1 ) = '/' THEN t0.documentroot ELSE t2.location || '/' || t0.documentroot END AS documentroot,
                 t0.email AS email,
                 t0.httpsonly AS httpsonly,
                 regexp_replace(regexp_replace(t0.conftext, '$bs', '$bs$bs', 'g'), '\n', '$bs""\n', 'g')  AS conftext,
                 t1.port AS port,
                 t1.sport AS sport
          FROM ( mne_system.apachesite t0 LEFT JOIN mne_system.apache t1 ON ( true )  LEFT JOIN mne_application.folder t2 ON ( t2.name = 'webcontent' ) )"

getsites="$getallsites
          WHERE t0.lastread < t0.modifydate OR t0.lastread IS NULL"

get_sites()
{
    par=$1
    name=${par/%%%%*};         par=${par#*%%%%}
    enabled=${par/%%%%*};      par=${par#*%%%%}
    domain=${par/%%%%*};       par=${par#*%%%%}
    aliases=${par/%%%%*};      par=${par#*%%%%}
    documentroot=${par/%%%%*}; par=${par#*%%%%}
    email=${par/%%%%*};        par=${par#*%%%%}
    httpsonly=${par/%%%%*};    par=${par#*%%%%}
    conftext="${par/%%%%*}";     par=${par#*%%%%}
    port=${par/%%%%*};         par=${par#*%%%%}
    sport=${par/%%%%*};        par=${par#*%%%%}
}

getports="SELECT port, sport FROM mne_system.apache"
get_ports()
{
    par=$1
    port=${par/%%%%*};         par=${par#*%%%%}
    sport=${par/%%%%*};        par=${par#*%%%%}
}

getrootfolder="SELECT location FROM mne_application.folder WHERE name ='webcontent'"
get_rootfolder()
{
    par=$1
    location=${par/%%%%*};     par=${par#*%%%%}
}
