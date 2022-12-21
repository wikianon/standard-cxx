/****************************************************************************
**
** Copyright (c) 2006-2008 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE-GPL.txt contained within the
** same package as this file. This software is subject to a
** dual-licensing mechanism, the details of which are outlined in
** file LICENSE-DUAL.txt, also contained within this package. Be sure
** to use the correct license for your needs. To view the commercial
** license, read LICENSE-COMMERCIAL.txt also contained within this
** package.
**
** If you do not have access to these files or are unsure which license
** is appropriate for your use, please contact the sales department at
** sales@captivemetrics.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

import com.capmet.metrics.cm.CmCpus;
import com.capmet.metrics.cm.CmCpuInfo;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.metrics.rmi.PerformanceHost;

class Cpus {
  static void main(args) {
    try {
      def remoteHost = (args.length == 1 ? args[0] : "localhost");
      PerformanceHost host = new PerformanceHost(remoteHost);
      CmCpus cpus = new CmCpus(remoteHost);

      if (!cpus.isSupported()) {
        println("Cpus category not supported on " +
           host.getOSFlavor());
        return;
      }

      println(host.getHostName() + " : " + host.getOSFlavor());
      cpus.each { e ->
        def name = e.key;
        CmCpuInfo cm = e.value;

        println("--------------------");
        println("name : " + name);
        println("inst#: " + cm.cpuInstance);
        println("arch : " + cm.cpuArchitecture);
        println("model: " + cm.cpuModel);
        println("clock: " + cm.clockSpeedMHz + " MHz");
      }
    } catch(Exception e) {
        println((new CmExceptionParser(e)).getMessage());
    }
  }
}
