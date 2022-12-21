#!/bin/sh

echo "You must run install.sh from your data directory"
echo "(set in Config.local).   You are now in "
echo
pwd
echo
echo "If this is correct enter Y to continue, else exit"
read OK

if [ -z $OK ] ; then
  exit
fi

umask 077

mkdir messages &&
mkdir deliveries &&

echo "Now use 'adduser.sh' to create user postmaster (required)" &&
echo "and any other users you wish to create right now." &&
echo "You can of course use adduser later whenever yoy wish." ||

echo install FAILED - please check your permissions
