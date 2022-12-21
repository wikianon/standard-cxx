#!/bin/sh

echo "You must run adduser.sh from your data directory"
echo "(set in Config.local).   You are now in "
echo
pwd
echo
echo "If this is correct enter Y to continue; else exit"
read OK

if [ -z $OK ] ; then
  exit
fi

umask 077

while [ 1 ] ; do
  USER=""
  echo
  echo "Username ? "
  read USER
  if [ -z $USER ] ; then
    echo Done
    exit
  fi
  mkdir $USER &&
  mkdir $USER/folders &&
  mkdir $USER/messages &&
  mkdir $USER/templates &&
  echo "Created directories for $USER." ||
  echo "FAILED to create directories for $USER (maybe you already have $USER?)"
done
