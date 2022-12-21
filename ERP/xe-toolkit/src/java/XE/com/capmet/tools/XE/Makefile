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

JFILES  = GetOpt.java XECpuMeter.java XEGraph.java \
          XEGraphProperties.java XECpuGraph.java XEGraphPoint.java \
          XEPerformanceFieldSelector.java XEDialogs.java XETopProcesses.java \
          XEPieChartProperties.java XEPieChart.java XECpuPieChart.java \
          XEMemoryPieChart.java XEPageFilePieChart.java XEMessages.java \
          XESystemTree.java XEArgsParser.java XEApplication.java \
          XEEventInterface.java XEEventManager.java XEEventHandler.java \
          XETopProcProperties.java XESysTreeProperties.java \
          XERulePanel.java RuleResolution.java XERulePanelProperties.java \
          XEFailureMethod.java XEBrowserLauncher.java GuruFileReader.java \
          TimeRecord.java RuleRecord.java
BCFILES = GetOpt.class XECpuMeter.class XECpuMeter.class XEGraph.class \
          XEGraphProperties.class XECpuGraph.class XEGraphPoint.class \
          XEPerformanceFieldSelector.class XEDialogs.class XETopProcesses.class\
          XEPieChartProperties.class XEPieChart.class XECpuPieChart.class \
          XEMemoryPieChart.class XEPageFilePieChart.class XEMessages.class \
          XESystemTree.class XEArgsParser.class XEApplication.class \
          XEEventInterface.class XEEventManager.class XEEventHandler.class \
          XETopProcProperties.class XESysTreeProperties.class \
          XERulePanel.class RuleResolution.class XERulePanelProperties.class \
          XEFailureMethod.class XEBrowserLauncher.class GuruFileReader.class \
          TimeRecord.class RuleRecord.class
TOP     = ../../../..
JARS    = /CM/product/jars
TARGET  = $(JARS)/XE.jar
ALL     = com/capmet/tools/XE/*.class com/capmet/tools/XE/*.properties \
          com/capmet/tools/XE/images/* com/capmet/tools/XE/programs/*.class \
          com/capmet/tools/XE/rules/*.class \
          com/capmet/tools/XE/rules/*.properties
JF      = $(JARS)/forms.jar:$(JARS)/looks.jar:$(JARS)/Chart2D.jar
OPTIM   = -g
.SUFFIXES: .java .class

.java.class:
	CLASSPATH=$(TOP):$(JF) javac $(OPTIM) $*.java

$(TARGET):	$(BCFILES) always
	(cd $(TOP) ; jar-if-newer $(JARS)/XE.jar $(ALL))

always:
	(cd programs ; make $(MFLAGS))
	(cd rules ; make $(MFLAGS))

all:
	CLASSPATH=$(TOP):$(JF) javac $(OPTIM) $(JFILES)
	(cd programs ; make $(MFLAGS) all)
	(cd rules ; make $(MFLAGS) all)
	(cd $(TOP) ; jar-if-newer $(JARS)/XE.jar $(ALL))

rejar:
	(cd $(TOP) ; jar cvf $(JARS)/XE.jar $(ALL))

clean:
	/bin/rm -fr *.class
	(cd programs ; make clean)
	(cd rules ; make clean)
