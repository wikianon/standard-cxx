#!/bin/bash

scriptdir=$(dirname ${BASH_SOURCE[1]})

if [ "$ALLGREADED" = "" ]; then
        
    ALLGREADED=1
    
    function mne_error_handler()
    {
        # Aufruf mit: trap 'mne_error_handler $BASH_COMMAND $LINENO $?' ERR
    
        if [ "`type -t script_error_handler`" = "function" ]; then
             script_error_handler $1 $2 $3
        fi
    
    	if [ "$mne_error_ignore" = "1" ]; then
    		return;
    	fi
    
        if [ "$mne_error_need" = "" ]; then
            cmd=`echo $1 | cut "-d " -f 1`
            if [ "$cmd" = "grep" ] || [ "$cmd" = "fgrep" ] || [ "$cmd" = "egrep" ]; then
                return;
            fi
        fi
      
        echo "error found: " 1>&$stderr
        echo "error in: $scriptdir" 1>&$logfile
        echo "     cmd: $1" 1>&2
        echo "    line: $2" 1>&2
        echo "  status: $3" 1>&2
    
            if [ "$noexit" = "1" ]; then
            noexit=0
            return;
        fi
        exit 1
    }
    
    trap 'mne_error_handler "$BASH_COMMAND" $LINENO $?' ERR
    exit_status=0
    
    function mne_checksys()
    {
    	for i in "${sysok[@]}"
    	do
    		if [ "$SYSVERSION" = "$i" ]; then
    			return;
    		fi
    	done
    	
    	echo "system not supported" 1>&2
    	exit 2;
    }
    
    function save_file()
    {
        savedfile=
        if [ -f "$1" ]; then
            cp $1 $1""_$actdate
            savedfile=$1""_$actdate
        fi
    }
    
    function toupper()
    {
        echo "$1"| awk '{print toupper($0)}'
    }
    
    function tolower()
    {
        echo "$1"| awk '{print tolower($0)}'
    }
    
    function Tolower()
    {
        echo "$1"| awk '{printf("%s%s", toupper(substr($0,0,1)), tolower(substr($0,2))) }'
    }
    
    function pg_adduser()
    {
        su - postgres -c "echo \"CREATE ROLE $1; ALTER ROLE $1 LOGIN;\" | psql >$logfile 2>&1; exit 0"; 
    }
    
    function pg_addaccess()
    {
  
      egrep $1 $pgconf/pg_hba.conf >$logfile 2>&1
      if [ ! "$?" = "0" ]; then
        mv  $pgconf/pg_hba.conf $pgconf/pg_hba.conf.orig 

        echo "$2"                      > $pgconf/pg_hba.conf
        cat $pgconf/pg_hba.conf.orig  >> $pgconf/pg_hba.conf
        chown postgres:postgres $pgconf/pg_hba.conf
        su - postgres -c "$(pg_config --bindir)/pg_ctl reload -D $pgconf" >$logfile 2>&1
      fi
    }

    export LANG=C
    umask 0022
    
    bs='\\'
    
    DBUSER=
    DB=
    DATAROOT=
    
    actdate=$(date +%d.%m.%Y_%H:%M:%S)

    stdout=3
    stderr=4
    logfile=5
    log=/var/log/mne/system.log
    
    exec 3>&1
    exec 4>&2
    
    exec 2> >( tee -ia ${log} 1>&2 )
    exec 1> >( tee -ia ${log} )
    exec 5> >( tee -ia ${log} 1>/dev/null )
    
    echo `date` ===================================================================== 1>&$logfile
    echo $0 1>&$logfile
    
    function cleanup
    {
      echo End ===================================================================== 1>&$logfile
      sleep 0
    }
    
    trap cleanup EXIT
    
    phkomma="'"'\"'"'"'\"'"'"
    prog='/'"'"'/ { gsub(/'"'"'/, "'$phkomma'") } { print$0 }'
    
    while [ $# -gt 0 ] ; do
      case $1 in
        -project) project=$2; shift 2;;
           -user) daemonuser=$2; shift 2;;
         -locale) deamonlocale=$2; shift 2;;
            -va*) p=${1//\./_};
                  v=`echo "$2" | awk "$prog"`;
                  eval "${p:1}='$v'";
                  i="${p:1}ignore"
                  if [ "${!i}" = "" ]; then
                    echo "${p:1}=$v" 1>&$logfile
                  else
                    echo "${p:1}=********" 1>&$logfile
                  fi
                  shift 2 ;;
         *)       shift 1 ;;
      esac
    done
    
    . $project/exec/system/config.sh
    
    UNAME=`uname`
    SYSVERSION=default
    
    if [ "$UNAME" = "Linux" ]; then
      fgrep 12.04 /etc/issue 1>&$logfile 2>&1
      if [ "$?" = "0" ]; then
          SYSVERSION=12_04
       fi
    
      fgrep 14.04 /etc/issue 1>&$logfile 2>&1
      if [ "$?" = "0" ]; then
        SYSVERSION=14_04
      fi
    
      fgrep 15.04 /etc/issue 1>&$logfile 2>&1
      if [ "$?" = "0" ]; then
        SYSVERSION=15_04
      fi
    
      fgrep 16.04 /etc/issue 1>&$logfile 2>&1
      if [ "$?" = "0" ]; then
        SYSVERSION=16_04
      fi
    fi
    
    if [ "$UNAME" = "Darwin" ]; then
        sw_vers -productVersion | fgrep 10.8 1>&$logfile
        if [ "$?" = "0" ]; then
          SYSVERSION=mac_10_8
        fi
    
        sw_vers -productVersion | fgrep 10.11 1>&$logfile
        if [ "$?" = "0" ]; then
          SYSVERSION=mac_10_11
        fi
    fi
    
      mne_error_ignore=1
      . exec/system/config/config.sh >/dev/null 2>&1
      mne_error_ignore=
    
    if [ -f "exec/local/system/config/config.sh" ]; then
      . exec/local/system/config/config.sh
    fi
    
    if [ -f "exec/system/config/$SYSVERSION/config.sh" ]; then
      . exec/system/config/$SYSVERSION/config.sh
    fi
    
    if [ -f "exec/local/system/config/$SYSVERSION/config.sh" ]; then
      . exec/local/system/config/$SYSVERSION/config.sh
    fi

fi

if [ -f "$scriptdir/check.sh" ]; then
    . $scriptdir/check.sh
fi

