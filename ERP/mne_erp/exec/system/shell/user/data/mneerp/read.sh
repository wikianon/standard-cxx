#!/bin/bash

. exec/system/shell/allg/db.sh

getuserdata="SELECT \
                t0.\"loginname\" AS loginname,\
                t1.firstname || E' ' || t1.lastname AS fullname,\
                t0.unixid,\
                t0.unixgrp\
              FROM\
                mne_crm.personowndata t0 LEFT JOIN mne_crm.person t1 ON ( t0.personid = t1.personid) \
              WHERE t0.\"loginname\" = 'par1'"

get_userdata()
{
    par=$1
    user=${par/%%%%*};  par=${par#*%%%%}
    fullname=${par/%%%%*}; par=${par#*%%%%}
    uid=${par/%%%%*}; par=${par#*%%%%}
    gid=${par/%%%%*}; par=${par#*%%%%}
}
              