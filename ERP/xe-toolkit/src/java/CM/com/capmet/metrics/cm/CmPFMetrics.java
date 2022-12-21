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

package com.capmet.metrics.cm;

import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import com.capmet.metrics.rmi.PerformanceCollection;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.rmi.PerformanceCategory;
import com.capmet.metrics.rmi.PerformanceField;
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.CollectionItem;
import com.capmet.metrics.*;

/**
 * This class is the data contained in the value part
 * of the TreeMap used in the <CODE>CmPageFiles</CODE> class. For each
 * page file name as the key, this class contains the values.
 */
public class CmPFMetrics
{
    /**
     * The size of the page file in megabytes.
     */
    public CmInteger sizeMB;
    /**
     * The amount of free space in the page file in megabytes.
     */
    public CmInteger freeMB;
    /**
     * The amount of used space in the page file in megabytes.
     */
    public CmInteger usedMB;
    /**
     * The amount of used space as a percentage of the total size.
     */
    public CmInteger percentUsed;

    // private members in support of the class
    private CollectionItem sizeField;
    private CollectionItem usedField;
    private CollectionItem percField;
    private boolean        firstTime;

    private CmOSFlavor              osFlavor;
    private PerformanceCollection collection;

    /**
     * Lone constructor for the class.
     * @param host The <CODE>PerformanceHost</CODE> object for the remote host.
     * @param pcat The <CODE>PerformanceCategory</CODE> object for page files.
     * @param instance The name of the page file instance.
     * @param coll The <CODE>PerformanceCollection</CODE> holding all the values.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmPFMetrics(PerformanceHost host, PerformanceCategory pcat,
        String instance, PerformanceCollection coll)
            throws RemoteException, NotBoundException,
                MalformedURLException, PerformanceException
    {
        String pfCat = pcat.getCategoryName();

        this.osFlavor = host.getOSFlavor();
        this.collection = coll;

        if (osFlavor.isUnixHost()) {
            String sz = host.getLocalizedName("KBytes Total");
            String uz = host.getLocalizedName("KBytes Used");

            sizeField = coll.put(pfCat, sz, instance);
            usedField = coll.put(pfCat, uz, instance);
        } else if (osFlavor.isWindowsHost()) {
            String pc = host.getLocalizedName("% Usage");

            sizeField = null;
            percField = coll.put(pfCat, pc, instance);
        }

        // must wait for a queryCollection to find out if fields are supported
        firstTime = true;
    }

    /**
     * Resync the values from the <CODE>PerformanceCollection</CODE> to this node.
     */
    public void resync()
    {
        if (firstTime)
        {
            if (osFlavor.isUnixHost())
            {
                sizeMB        = new CmInteger(true);
                freeMB        = new CmInteger(true);
                usedMB        = new CmInteger(true);
                percentUsed   = new CmInteger(true);
            }
            else if (osFlavor.isWindowsHost())
            {
                sizeMB        = new CmInteger(false);
                freeMB        = new CmInteger(false);
                usedMB        = new CmInteger(false);
                percentUsed   = new CmInteger(true);
            }

            firstTime = false;
        }

        if (osFlavor.isUnixHost())
        {
            sizeMB.setValue(((Number) sizeField.value).intValue() / 1024);
            usedMB.setValue(((Number) usedField.value).intValue() / 1024);
            freeMB.setValue(sizeMB.intValue() - usedMB.intValue());
            percentUsed.setValue((usedMB.intValue() * 100) / sizeMB.intValue());
        }
        else if (osFlavor.isWindowsHost())
        {
            percentUsed.setValue(((Number) percField.value).intValue());
        }
    }

    /**
     * Remove the values from the <CODE>PerformanceCollection</CODE>.
     * @throws RemoteException RMI exception
     */
    public void remove()
        throws RemoteException
    {
        if (osFlavor.isUnixHost())
        {
            collection.remove(sizeField);
            collection.remove(usedField);
        }
        else if (osFlavor.isWindowsHost())
        {
            collection.remove(percField);
        }
    }
}
