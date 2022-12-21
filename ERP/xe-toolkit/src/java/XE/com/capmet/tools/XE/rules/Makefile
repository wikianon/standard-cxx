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

JFILES  = RuleMessages.java Threshold.java Rule.java AggregateRule.java \
          PureCPURule.java LiveCPURule.java PureDiskRule.java \
          PureCPUSeedValues.java PureDiskSeedValues.java LiveDiskRule.java \
          PureMemoryRule.java PureMemorySeedValues.java LiveMemoryRule.java \
          PurePageFileRule.java PurePageFileSeedValues.java \
          LivePageFileRule.java PureNICRule.java PureNICSeedValues.java \
          LiveNICRule.java PureCacheRule.java PureCacheSeedValues.java \
          LiveCacheRule.java PureTCPRule.java PureTCPSeedValues.java \
          LiveTCPRule.java
BCFILES = RuleMessages.class Threshold.class Rule.class AggregateRule.class \
          PureCPURule.class LiveCPURule.class PureDiskRule.class \
          PureCPUSeedValues.class PureDiskSeedValues.class LiveDiskRule.class \
          PureMemoryRule.class PureMemorySeedValues.class LiveMemoryRule.class \
          PurePageFileRule.class PurePageFileSeedValues.class \
          LivePageFileRule.class PureNICRule.class PureNICSeedValues.class \
          LiveNICRule.class PureCacheRule.class PureCacheSeedValues.class \
          LiveCacheRule.class PureTCPRule.class PureTCPSeedValues.class \
          LiveTCPRule.class
TOP     = ../../../../..
JARS    = /CM/product/jars
JF      = $(JARS)/forms.jar:$(JARS)/looks.jar:$(JARS)/Chart2D.jar
OPTIM   = -g
.SUFFIXES: .java .class

.java.class:
	CLASSPATH=$(TOP):$(JF) javac $(OPTIM) $*.java

default:	$(BCFILES)

all:
	CLASSPATH=$(TOP):$(JF) javac $(OPTIM) $(JFILES)

clean:
	/bin/rm -fr *.class
