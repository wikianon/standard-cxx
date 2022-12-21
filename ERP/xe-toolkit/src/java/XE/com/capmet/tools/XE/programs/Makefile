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

JFILES  = CpuMeter.java OSFlavor.java IoStat.java Explain.java \
          CpuGraph.java NicStat.java MpStat.java Top.java CpuPie.java \
          SysInfo.java MemoryPie.java SysStat.java PageFilePie.java \
          CacheStat.java Disks.java Counters.java \
          GadgetProperties.java TopTabProperties.java TopProperties.java \
          TopTabMap.java TopWindowProperties.java TWPVector.java \
          CommandVector.java RulePanel.java TcpStat.java VirtualGuru.java \
          GuruVision.java
BCFILES = CpuMeter.class OSFlavor.class IoStat.class Explain.class \
          CpuGraph.class NicStat.class MpStat.class Top.class CpuPie.class \
          SysInfo.class MemoryPie.class SysStat.class PageFilePie.class \
          CacheStat.class Disks.class Counters.class \
          GadgetProperties.class TopTabProperties.class TopProperties.class \
          TopTabMap.class TopWindowProperties.class TWPVector.class \
          CommandVector.class RulePanel.class TcpStat.class VirtualGuru.class \
          GuruVision.class
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
