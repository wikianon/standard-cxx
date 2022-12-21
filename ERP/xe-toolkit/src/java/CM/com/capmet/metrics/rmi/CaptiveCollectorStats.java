/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Captive Metrics library of Captivity.
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

package com.capmet.metrics.rmi;

import java.util.Date;
import java.io.Serializable;

/**
 * Runtime stats about CaptiveCollector.
 */
public class CaptiveCollectorStats implements Serializable {
    /**
     * The version string of the collector.
     */
    public String version;
    /**
     * Start time in milliseconds of the collector.
     */
    public Date startTime;
    /**
     * Current time in milliseconds of the collector.
     */
    public Date currentTime;
    /**
     * Number of remote method calls made to the collector.
     */
    public long rmcCount;
    /**
     * Number of objects in the object cache.
     */
    public int  objectsInCache;
    /**
     * The number of processors available to the Java virtual machine.
     */
    public int  jvmProcessors;
    /**
     * The amount of free memory in the Java Virtual Machine.
     */
    public long jvmFreeMemory;
    /**
     * The total amount of memory in the Java virtual machine.
     */
    public long jvmTotalMemory;

    /**
     * Constructor. Just initialized members to zero.
     */
    public CaptiveCollectorStats() {
        version = null;
        startTime = null;
        currentTime = null;
        rmcCount = 0;
        objectsInCache = 0;
        jvmProcessors = 0;
        jvmTotalMemory = 0;
        jvmFreeMemory = 0;
    }
}
