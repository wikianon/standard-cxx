#############################################################################
##
## Copyright (c) 2006-2007 Captive Metrics Software Corporation.
##                    All rights reserved.
##
## This file is part of the configuration files of Captivity.
##
## This file may be used under the terms of the GNU General Public
## License version 2.0 as published by the Free Software Foundation
## and appearing in the file LICENSE-GPL.txt contained within the
## same package as this file. This software is subject to a
## dual-licensing mechanism, the details of which are outlined in
## file LICENSE-DUAL.txt, also contained within this package. Be sure
## to use the correct license for your needs. To view the commercial
## license, read LICENSE-COMMERCIAL.txt also contained within this
## package.
##
## If you are unsure which license is appropriate for your use, please
## contact the sales department at sales@captivemetrics.com.
##
## This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
## WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
#############################################################################

ALL      = Keystore CaptiveCollector.cer Truststore
#
# This is the default password. Change, rebuild and redistribute
# Keystore, Truststore and CaptiveCollector.cer files if you wish
#
PASSWORD = CAPTIVITY

all:	$(ALL)

Keystore:
	keytool -genkey -keyalg RSA -alias SecureCaptiveCollector -keystore Keystore -dname "CN=CaptiveCollector, OU=Engineering, O=Captive Metrics Software Corporation, L=Johnstown, ST=New York, C=US" -storepass $(PASSWORD) -keypass $(PASSWORD)

Truststore:
	keytool -import -noprompt -alias SecureCaptiveCollector -file CaptiveCollector.cer -keystore Truststore -keypass $(PASSWORD) -storepass $(PASSWORD) -dname "CN=CaptiveCollector, OU=Engineering, O=Captive Metrics Software Corporation, L=Johnstown, ST=New York, C=US"

CaptiveCollector.cer:
	keytool -export -alias SecureCaptiveCollector -keystore Keystore -rfc -file CaptiveCollector.cer -keypass $(PASSWORD) -storepass $(PASSWORD)

clean:
	/bin/rm -f $(ALL)
