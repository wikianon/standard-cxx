#!/bin/bash

psql=/usr/bin/psql

apache2rootdir=/etc/apache2

apache2confdir=/etc/apache2/conf-available
apache2sitedir=/etc/apache2/sites-available

apache2ensite=a2ensite
apache2enconf=a2enconf

apache2reload="service apache2 reload"
apache2restart="service apache2 restart"

netinterfaces=/etc/network/interfaces
