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
 * of the TreeMap used in the <CODE>CmIoStat</CODE> class. For each
 * I/O source name as the key, this class contains the values.
 */
public class CmIoMetrics
{
    /**
     * A string representation of the type of disk. Possible values
     * are "physical", "logical", "network", "ram", "loop" and "meta".
     */
    public CmString  diskType;
    /**
     * The run time divided by the elapsed time.
     */
    public CmDouble  utilizationPercent;
    /**
     * The average length of the pending request queue.
     */
    public CmDouble  averageQueueLength;
    /**
     * The number of read operations per second.
     */
    public CmInteger readOperations;
    /**
     * The number of write operations per second.
     */
    public CmInteger writeOperations;
    /**
     * The number of kilobytes of data read per second.
     */
    public CmInteger kilobytesRead;
    /**
     * The number of kilobytes of data written per second.
     */
    public CmInteger kilobytesWritten;
    /**
     * The average amount of elapsed time in milliseconds between
     * the time that an operation is accepted by the device and when the
     * reply from the device for that operation is returned.
     */
    public CmDouble  averageAccessTime;
    /**
     * The total size of the device represented in megabytes.
     */
    public CmInteger capacityMB;

    // private members in support of the class.
    private CollectionItem utilField;
    private CollectionItem queueField;
    private CollectionItem readsField;
    private CollectionItem writesField;
    private CollectionItem kBReadField;
    private CollectionItem kBWrittenField;
    private CollectionItem avgAccTimeField;
    private CollectionItem capField;

    private CmOSFlavor            osFlavor;
    private PerformanceCollection collection;

    /**
     * Lone constructor for the class.
     * @param host The <CODE>PerformanceHost</CODE> object for the remote host.
     * @param pcat The <CODE>PerformanceCategory</CODE> object for I/O devices.
     * @param info The <CODE>PerformanceCategory</CODE> object for Windows disk info.
     * @param instance The name of the I/O device instance.
     * @param coll The <CODE>PerformanceCollection</CODE> holding all the values.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmIoMetrics(PerformanceHost host, PerformanceCategory pcat,
        PerformanceCategory info, String instance, PerformanceCollection coll)
            throws RemoteException, NotBoundException,
                MalformedURLException, PerformanceException
    {
        String category = pcat.getCategoryName();
        CmGlobal<CmNames> gnames = CmGlobal.getNames(host.getHostName());
        CmNames names = gnames.getObject();
        boolean isPhys;

        this.osFlavor = host.getOSFlavor();
        this.collection = coll;

        if (osFlavor.isUnixHost()) {
            String dt   = host.getLocalizedName("Disk Type");
            String uf   = host.getLocalizedName("Percent Time Busy");
            String qf   = host.getLocalizedName("Average Request Queue Length");
            String rf   = host.getLocalizedName("Read Operations/sec");
            String wf   = host.getLocalizedName("Write Operations/sec");
            String krf  = host.getLocalizedName("KBytes Read/sec");
            String kwf  = host.getLocalizedName("KBytes Written/sec");
            String aatf = host.getLocalizedName("Average Access Time");
            String cf   = host.getLocalizedName("Megabytes of Capacity");

            PerformanceField typeField = pcat.getField(dt, instance);

            diskType        = new CmString((String) typeField.getValue());
            utilField       = coll.put(category, uf,   instance);
            queueField      = coll.put(category, qf,   instance);
            readsField      = coll.put(category, rf,   instance);
            writesField     = coll.put(category, wf,   instance);
            kBReadField     = coll.put(category, krf,  instance);
            kBWrittenField  = coll.put(category, kwf,  instance);
            avgAccTimeField = coll.put(category, aatf, instance);
            capField        = coll.put(category, cf,   instance);
        } else if (osFlavor.isWindowsHost()) {
            diskType        = new CmString();
            if (category.equals(names.WindowsPhysDisk))
            {
                diskType.setValue("physical");
                isPhys = true;
            }
            else
            {
                diskType.setValue("logical");
                isPhys = false;
            }

            String dt   = host.getLocalizedName("% Disk Time");
            String qf   = host.getLocalizedName("Current Disk Queue Length");
            String rf   = host.getLocalizedName("Disk Reads/sec");
            String wf   = host.getLocalizedName("Disk Writes/sec");
            String krf  = host.getLocalizedName("Disk Read Bytes/sec");
            String kwf  = host.getLocalizedName("Disk Write Bytes/sec");
            String aatf = host.getLocalizedName("Avg. Disk sec/Transfer");

            utilField       = coll.put(category, dt,   instance);
            queueField      = coll.put(category, qf,   instance);
            readsField      = coll.put(category, rf,   instance);
            writesField     = coll.put(category, wf,   instance);
            kBReadField     = coll.put(category, krf,  instance);
            kBWrittenField  = coll.put(category, kwf,  instance);
            avgAccTimeField = coll.put(category, aatf, instance);

            String infoInst = instance;
            if (isPhys)
            {
                int space = instance.indexOf(' ');

                if (space != -1)
                    infoInst = instance.substring(0, space);
            }

            String cf = host.getLocalizedName("Megabytes of Capacity");

            capField = coll.put(info.getCategoryName(), cf, infoInst);
        }

        // must wait for a queryCollection to find out if fields are supported
        utilizationPercent = null;
    }

    /**
     * Resync the values from the <CODE>PerformanceCollection</CODE> to this node.
     */
    public void resync()
    {
        if (utilizationPercent == null)
        {
            utilizationPercent = new CmDouble(utilField.supported);
            averageQueueLength = new CmDouble(queueField.supported);
            readOperations = new CmInteger(readsField.supported);
            writeOperations = new CmInteger(writesField.supported);
            kilobytesRead = new CmInteger(kBReadField.supported);
            kilobytesWritten = new CmInteger(kBWrittenField.supported);
            averageAccessTime = new CmDouble(avgAccTimeField.supported);
            capacityMB = new CmInteger(capField.supported);
        }
        if (utilizationPercent.isSupported())
            utilizationPercent.setValue(((Number) utilField.value).doubleValue());
        if (averageQueueLength.isSupported())
            averageQueueLength.setValue(((Number) queueField.value).doubleValue());
        if (readOperations.isSupported())
            readOperations.setValue(((Number) readsField.value).intValue());
        if (writeOperations.isSupported())
            writeOperations.setValue(((Number) writesField.value).intValue());
        if (osFlavor.isUnixHost())
        {
            if (kilobytesRead.isSupported())
                kilobytesRead.setValue(((Number) kBReadField.value).intValue());
            if (kilobytesWritten.isSupported())
                kilobytesWritten.setValue(((Number) kBWrittenField.value).intValue());
            if (averageAccessTime.isSupported())
                averageAccessTime.setValue(((Number) avgAccTimeField.value).doubleValue());
        }
        else if (osFlavor.isWindowsHost())
        {
            if (kilobytesRead.isSupported())
                kilobytesRead.setValue(((Number) kBReadField.value).intValue() / 1024);
            if (kilobytesWritten.isSupported())
                kilobytesWritten.setValue(((Number) kBWrittenField.value).intValue() / 1024);
            if (averageAccessTime.isSupported())
                averageAccessTime.setValue(
                    ((Number) avgAccTimeField.value).doubleValue() * 1000);
        }
        if (capacityMB.isSupported())
            capacityMB.setValue(((Number) capField.value).intValue());
    }

    /**
     * Remove the values from the <CODE>PerformanceCollection</CODE>.
     * @throws RemoteException RMI exception
     */
    public void remove()
        throws RemoteException
    {
        collection.remove(utilField);
        collection.remove(queueField);
        collection.remove(readsField);
        collection.remove(writesField);
        collection.remove(kBReadField);
        collection.remove(kBWrittenField);
        collection.remove(avgAccTimeField);
        collection.remove(capField);
    }
}
