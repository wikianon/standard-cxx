#!/bin/bash

. exec/system/shell/allg/db.sh

getcarddav="SELECT zpushid, addressbookname FROM mne_system.zpush;"
get_carddav()
{
	line=$1
          carddavid=${line/%%%%*}; line=${line#*%%%%}
    addressbookname=${line/%%%%*}; 
}
