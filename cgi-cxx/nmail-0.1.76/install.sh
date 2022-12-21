#!/bin/bash
#
# This is the install script.
#
# If you are looking for a place to configure the installation directories,
#    check 'config.h'


LOCALSTATEDIR=$(grep LOCALSTATEDIR config.h | awk -F \" '{ print $2 }')
PREFDIR=$(grep PREFDIR config.h | awk -F \" '{ print $2 }')
LOCKDIR=$(grep LOCKDIR config.h | awk -F \" '{ print $2 }')
TMPDIR=$(grep TMPDIR config.h | awk -F \" '{ print $2 }')

SBINDIR=$(grep SBINDIR config.h | awk -F \" '{ print $2 }')
HTTPROOT=$(grep HTTPROOT config.h | awk -F \" '{ print $2 }')
CGIBIN=$(grep CGIBIN config.h | awk -F \" '{ print $2 }')
NMHTTP=${HTTPROOT}$(grep NMHTTP config.h | awk -F \" '{ print $2 }')

#create directories

if [ ! -d $LOCALSTATEDIR ]; then
   install -d -o mail -g mail -m 500 $LOCALSTATEDIR
fi

if [ ! -d $PREFDIR ]; then
   install -d -o mail -g mail -m 700 $PREFDIR
fi

if [ ! -d $LOCKDIR ]; then
   install -d -o mail -g mail -m 700 $LOCKDIR
fi

if [ ! -d $TMPDIR ]; then
   install -d -o mail -g mail -m 700 $TMPDIR
fi


if [ ! -d $HTTPROOT ]; then
   echo "Error: $HTTPROOT does not exist"
   exit -1
fi

#copy http directory

if [ ! -d $NMHTTP ]; then
   install -d -o nobody -g nobody -m 555 $NMHTTP
   cp -a images/* $NMHTTP
fi


if [ ! -d $CGIBIN ]; then
   echo "Error: $CGIBIN does not exist"
   exit -1
fi

#install nmail binary
install -o mail -g mail -s -m 6555 nmail $CGIBIN

#install login_validate binary
install -o root -g root -m 4555 login_validate $SBINDIR

if [ ! -f $PREFDIR/default ]; then
   echo "hostname=$(hostname)" > $PREFDIR/default
   chown mail.mail $PREFDIR/default
   chmod 600 $PREFDIR/default
fi


