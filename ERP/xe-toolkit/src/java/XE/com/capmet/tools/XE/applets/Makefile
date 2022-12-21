#############################################################################
##
## Copyright (c) 2006-2007 Captive Metrics Software Corporation.
##                    All rights reserved.
##
## This file is part of the XE library for The XE Toolkit.
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
## If you do not have access to these files or are unsure which license
## is appropriate for your use, please contact the sales department at
## sales@captivemetrics.com.
##
## This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
## WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
#############################################################################

JFILES  = CpuMeterApplet.java
BCFILES = CpuMeterApplet.class
TOP     = ../../../../..
JARS    = /CM/product/jars
TARGET  = $(JARS)/XEApplets.jar

# this does work, but is slow
.SUFFIXES:	.java .class

.java.class:
	CLASSPATH=$(TOP):$(JARS)/forms.jar javac -O $*.java

$(TARGET):	$(BCFILES)
	/opt/java/bin/jar cvf $(JARS)/XEApplets.jar $(BCFILES)

all:
	CLASSPATH=$(TOP):$(JARS)/forms.jar javac -O $(JFILES)
	/opt/java/bin/jar cvf $(JARS)/XEApplets.jar $(BCFILES)

clean:
	/bin/rm -fr *.class
