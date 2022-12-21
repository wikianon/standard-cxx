#!/bin/bash

. exec/system/shell/allg/db.sh

updateuser="UPDATE mne_crm.personowndata \
              SET unixid = par2, unixgrp = par3 \
            WHERE loginname = 'par1'"

getuser="SELECT \
                t0.\"loginname\" AS loginname,\
                t1.firstname || E' ' || t1.lastname AS fullname,\
                t0.unixid, t0.unixgrp\
              FROM\
                mne_crm.personowndata t0 LEFT JOIN mne_crm.person t1 ON ( t0.personid = t1.personid) \
              WHERE t0.\"loginname\" != ''"

get_user()
{
    par=$1
    user=${par/%%%%*};  par=${par#*%%%%}
    fullname=${par/%%%%*}; par=${par#*%%%%}
    uid=${par/%%%%*}; par=${par#*%%%%}
    gid=${par/%%%%*}; par=${par#*%%%%}
}

getvaliduser="SELECT \
                t0.\"loginname\" AS loginname,\
                t1.firstname || E' ' || t1.lastname AS fullname\
              FROM\
                mne_crm.personowndata t0 LEFT JOIN mne_crm.person t1 ON ( t0.personid = t1.personid) \
                LEFT JOIN pg_catalog.pg_roles t3 ON ( t0.loginname = t3.rolname)\
                LEFT JOIN mne_catalog.accessgroup t5 ON ( t3.rolname = t5.member AND t5.group = 'erpsmb' )\
              WHERE t0.\"loginname\" != '' AND t5.group IS NOT NULL"

get_validuser()
{
    par=$1
    user=${par/%%%%*};  par=${par#*%%%%}
    fullname=${par/%%%%*}; par=${par#*%%%%}
}
