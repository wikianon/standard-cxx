#!/bin/bash

. exec/system/shell/allg/db.sh

getreleases="SELECT DISTINCT t0.name, t0.location, t0.description \
           FROM mne_application.folder t0 \
             LEFT JOIN mne_system.shares t1 ON ( t0.folderid = t1.folderid ) \
             LEFT JOIN mne_crm.personowndata t2 ON ( t1.personid = t2.personid ) \
             LEFT JOIN pg_catalog.pg_roles t3 ON ( t2.loginname = t3.rolname)\
             LEFT JOIN mne_catalog.accessgroup t4 ON ( t3.rolname = t4.member AND t4.group = 'erpsmb' )\
              WHERE t1.folderid IS NOT NULL AND t2.loginname != '' AND t4.group IS NOT NULL \
           ORDER BY t0.name"

get_releases()
{
    par=$1
    name=${par/%%%%*};        par=${par#*%%%%}
    location=${par/%%%%*};    par=${par#*%%%%}
    description=${par/%%%%*}; par=${par#*%%%%}
}

getvalidusers="SELECT DISTINCT t2.loginname, CASE WHEN t1.readwrite THEN 'rw' ELSE 'r' END\
           FROM mne_application.folder t0 \
             LEFT JOIN mne_system.shares t1 ON ( t0.folderid = t1.folderid ) \
             LEFT JOIN mne_crm.personowndata t2 ON ( t1.personid = t2.personid ) \
             LEFT JOIN pg_catalog.pg_roles t3 ON ( t2.loginname = t3.rolname)\
             LEFT JOIN mne_catalog.accessgroup t4 ON ( t3.rolname = t4.member AND t4.group = 'erpsmb' )\
           WHERE t0.name = 'par1' AND t1.folderid IS NOT NULL AND t2.loginname != '' AND t4.group IS NOT NULL"

get_validusers()
{
    par=$1
    user=${par/%%%%*};        par=${par#*%%%%}
    readwrite=${par/%%%%*};   par=${par#*%%%%}
}
