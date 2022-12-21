/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector application of Captivity.
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

package com.capmet.Captivity.CaptiveCollector;

import D.PN;
import java.util.Map;
import java.util.TreeMap;
import com.capmet.metrics.rmi.PerformanceException;

/**
 * Access a remote PROCess Serialized Large OBject.
 */
public class ProcSlob extends TreeMap<String, D.PN>
{
    /**
     * The support mask for the fields in <CODE>D.PN</CODE>.
     */
    public long supportFlags;

    /**
     * The native process handle.
     */
    protected long handle;

    /**
     * A reason for failure.
     */
    protected String reason;

    // private store for nodes
    private TreeMap<String, D.PN> nodeStore;

    // the native side.
    private native boolean procSlobConstructor();

    /**
     * Construct the ProcSlob.
     * @throws PerformanceException Any lower level exception.
     */
    public ProcSlob() throws PerformanceException
    {
        nodeStore = new TreeMap<String, D.PN>();
        if (!procSlobConstructor())
            throw new PerformanceException(reason);
    }

    /*
     * Empty the nodes from the TreeMap and hang on to them for later use
     */
    private void vacate()
    {
        // pluck the fruit from this tree and hang on to it
        for(Map.Entry<String, D.PN> e : entrySet())
            nodeStore.put(e.getKey(), e.getValue());

        // now clear "this"
        clear();
    }

    /*
     * Grab a node from nodeStore and return it
     */
    private D.PN newNode()
    {
        // nodeStore depleted; allocate new
        if (nodeStore.size() == 0)
            return new D.PN();

        String key = nodeStore.firstKey();
        D.PN value = nodeStore.get(key);

        // delete it from nodeStore
        nodeStore.remove(key);

        // return it
        return value;
    }

    // the native side.
    private native boolean procSlobRefreshRates(TreeMap<String, D.PN> bt);

    /**
     * Get a new sample and compute rates where appropriate.
     * @throws PerformanceException Any lower level exception.
     */
    public void refreshRates() throws PerformanceException
    {
        vacate();
        if (!procSlobRefreshRates(this))
            throw new PerformanceException(reason);
    }

    /**
     * Finalize the object when done.
     */
    protected native void finalize();

    // static initialization
    static
    {
        System.loadLibrary("CCdirect");
    }
}
