#!/bin/bash

export PATH=/sbin:/usr/sbin:$PATH

psql=/usr/bin/psql
pgversion="$(pg_config --version | egrep -o '[0-9]{1,}\.[0-9]{1,}')"
pgconf="/etc/postgresql/$pgversion/main/"

apache2rootdir=/etc/apache2

apache2confdir=/etc/apache2/conf-available
apache2sitedir=/etc/apache2/sites-available

apache2ensite=a2ensite
apache2enconf=a2enconf

apache2reload="systemctl reload apache2.service"
apache2restart="systemctl restart apache2.service"

netinterfaces=/etc/network/interfaces

scriptdir=`dirname $0`
basedir=/opt/mne_sys

certbasedir=$DATAROOT/cert
  certcadir=$certbasedir/ca
 certkeydir=$certbasedir/key
certcertdir=$certbasedir/cert
 certcsrdir=$certbasedir/csr

certscriptdir=exec/system/shell/cert

dhcpconf=/etc/dhcp
dhcpconfig=/etc/dhcp/dhclient.conf

bindconf=/etc/bind
apparmorconf=/etc/apparmor.d

sambaroot=/opt/mne/samba
sambabin=$sambaroot/bin
sambasbin=$sambaroot/sbin
sambaconf=/etc/mne/samba
sambavar=/var/lib/mne/samba

ldapconf=/etc/ldap

kerberosconfig=/etc/krb5.conf

dovecotconf=/etc/dovecot
postfixconf=/etc/postfix
