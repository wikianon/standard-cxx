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
 * of the TreeMap used in the <CODE>CmNicStat</CODE> class. For each
 * NIC name as the key, this class contains the values.
 */
public class CmNicMetrics
{
    /**
     * Is this device the loopback device?
     */
    public CmBoolean isLoopback;
    /**
     * The number of bytes received per second.
     */
    public CmInteger bytesReceived;
    /**
     * The number of bytes transmitted per second.
     */
    public CmInteger bytesTransmitted;
    /**
     * The number of packets received per second.
     */
    public CmInteger packetsReceived;
    /**
     * The number of packets transmitted per second.
     */
    public CmInteger packetsTransmitted;
    /**
     * The number of input errors per second.
     */
    public CmDouble inputErrors;
    /**
     * The number of output errors per second.
     */
    public CmDouble outputErrors;
    /**
     * The number of receive failures per second.
     */
    public CmDouble inputFailures;
    /**
     * The number of transmit failures per second.
     */
    public CmDouble outputFailures;
    /**
     * The speed of the interface in bits per second.
     */
    public CmInteger interfaceSpeed;
    /**
     * The number of bits received and transmitted per second
     * as a percentage of the interface speed.
     */
    public CmDouble  utilizationPercent;
    /**
     * The number of collisions on transmit attempt per second
     * as a percentage of the number of packets transmitted.
     */
    public CmDouble  collisionPercent;
    /**
     * The IP address of the interface.
     */
    public CmString  ipAddress;
    /**
     * The subnet mask of the interface.
     */
    public CmString  subnetMask;
    /**
     * Whether the interface is full duplex or not.
     */
    public CmBoolean isFullDuplex;

    // private members in support of the class
    private CollectionItem brField;
    private CollectionItem btField;
    private CollectionItem prField;
    private CollectionItem ptField;
    private CollectionItem ieField;
    private CollectionItem oeField;
    private CollectionItem ifField;
    private CollectionItem ofField;
    private CollectionItem isField;
    private CollectionItem iaField;
    private CollectionItem smField;
    private CollectionItem fdField;
    private CollectionItem cpField;

    // Windoze does not compute rate for these
    private int oldIErrors;
    private int oldOErrors;
    private int oldIFailures;
    private int oldOFailures;

    private CmOSFlavor              osFlavor;
    private PerformanceCollection collection;

    /**
     * Lone constructor for the class.
     * @param host The <CODE>PerformanceHost</CODE> object for the remote host.
     * @param pcat The <CODE>PerformanceCategory</CODE> object for interfaces.
     * @param info The <CODE>PerformanceCategory</CODE> object for NIC Information (Windows).
     * @param instance The name of the NIC instance.
     * @param coll The <CODE>PerformanceCollection</CODE> holding all the values.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmNicMetrics(PerformanceHost host, PerformanceCategory pcat,
        PerformanceCategory info, String instance, PerformanceCollection coll)
            throws RemoteException, NotBoundException,
                MalformedURLException, PerformanceException
    {
        String netifCat = pcat.getCategoryName();
        String netInfoCat = info.getCategoryName();

        this.osFlavor = host.getOSFlavor();
        this.collection = coll;
        isLoopback = new CmBoolean(true);

        if (osFlavor.isUnixHost()) {
            String br = host.getLocalizedName("Bytes Received/sec");
            String bt = host.getLocalizedName("Bytes Transmitted/sec");
            String pr = host.getLocalizedName("Packets Received/sec");
            String pt = host.getLocalizedName("Packets Transmitted/sec");
            String er = host.getLocalizedName("Errors on Receipt/sec");
            String et = host.getLocalizedName("Errors on Transmission/sec");
            String fr = host.getLocalizedName("Failures on Receipt/sec");
            String ft = host.getLocalizedName("Failures on Transmission/sec");
            String is = host.getLocalizedName("Interface Speed (b/s)");
            String ip = host.getLocalizedName("IP Address");
            String nm = host.getLocalizedName("Network Mask");
            String fd = host.getLocalizedName("Interface is Full Duplex");
            String cp = host.getLocalizedName("Collision Percent");

            isLoopback.setValue(instance.equals("lo") || instance.equals("lo0"));
            brField = coll.put(netifCat, br, instance);
            btField = coll.put(netifCat, bt, instance);
            prField = coll.put(netifCat, pr, instance);
            ptField = coll.put(netifCat, pt, instance);
            ieField = coll.put(netifCat, er, instance);
            oeField = coll.put(netifCat, et, instance);
            ifField = coll.put(netifCat, fr, instance);
            ofField = coll.put(netifCat, ft, instance);
            cpField = coll.put(netifCat, cp, instance);
            isField = coll.put(netInfoCat, is, instance);
            iaField = coll.put(netInfoCat, ip, instance);
            smField = coll.put(netInfoCat, nm, instance);
            fdField = coll.put(netInfoCat, fd, instance);
        } else if (osFlavor.isWindowsHost()) {
            String br = host.getLocalizedName("Bytes Received/sec");
            String bt = host.getLocalizedName("Bytes Sent/sec");
            String pr = host.getLocalizedName("Packets Received/sec");
            String pt = host.getLocalizedName("Packets Sent/sec");
            String er = host.getLocalizedName("Packets Received Errors");
            String et = host.getLocalizedName("Packets Outbound Errors");
            String fr = host.getLocalizedName("Packets Received Discarded");
            String ft = host.getLocalizedName("Packets Outbound Discarded");
            String is = host.getLocalizedName("Current Bandwidth");

            isLoopback.setValue(instance.equals("MS TCP Loopback interface"));
            brField = coll.put(netifCat, br, instance);
            btField = coll.put(netifCat, bt, instance);
            prField = coll.put(netifCat, pr, instance);
            ptField = coll.put(netifCat, pt, instance);
            ieField = coll.put(netifCat, er, instance);
            oeField = coll.put(netifCat, et, instance);
            ifField = coll.put(netifCat, fr, instance);
            ofField = coll.put(netifCat, ft, instance);
            isField = coll.put(netifCat, is, instance);
            fdField = null;
            cpField = null;

            if (!isLoopback.booleanValue())
            {
                // More Windows pain. The performance registry adds tidbits to names.
                // In the case of my notebook, HKEY_PERFORMANCE_DATA says the interface
                // is "SiS 900-Based PCI Fast Ethernet Adapter - Packet Scheduler Miniport".
                // But, HKEY_LOCAL_MACHINE claims the name of this interface to be
                // "SiS 900-Based PCI Fast Ethernet Adapter", deleting the extra trailing
                // stuff. Now, who knows? Perhaps there will be cases where HPD calls it
                // "Chicken Little" and HLM calls it "Buffalo Wings Deluxe". This is just
                // the extra pleasure we can all derive from Microsoft and it's view of
                // the world through the Redmond Prism(tm). This bit of code makes the
                // best of an absolutely absurd situation.
                
                String[] iin = info.getInstanceNames();
                boolean namesMatch = false;

                for(String ii : iin)
                {
                    if (instance.equals(ii))
                    {
                        namesMatch = true;
                        break;
                    }
                }

                if (!namesMatch)
                {
                    for(String ii : iin)
                    {
                        if (instance.startsWith(ii, 0))
                        {
                            // close enough for Redmond work
                            instance = ii;
                            namesMatch = true;
                            break;
                        }
                    }
                }

                if (!namesMatch)
                {
                    // oh well.
                    iaField = null;
                    smField = null;
                }
                else
                {
                    String ip = host.getLocalizedName("IP Address");
                    String nm = host.getLocalizedName("Subnet Mask");

                    iaField = coll.put(netInfoCat, ip, instance);
                    smField = coll.put(netInfoCat, nm, instance);
                }
            }
            else
            {
                iaField = null;
                smField = null;
            }
        }

        // must wait for a queryCollection to find out if fields are supported
        utilizationPercent = null;

        oldIErrors = 0;
        oldOErrors = 0;
        oldIFailures = 0;
        oldOFailures = 0;
    }

    /**
     * Resync the values from the <CODE>PerformanceCollection</CODE> to this node.
     */
    public void resync()
    {
        if (utilizationPercent == null)
        {
            bytesReceived = new CmInteger(brField.supported);
            bytesTransmitted = new CmInteger(btField.supported);
            packetsReceived = new CmInteger(prField.supported);
            packetsTransmitted = new CmInteger(ptField.supported);
            inputErrors = new CmDouble(ieField.supported);
            outputErrors = new CmDouble(oeField.supported);
            inputFailures = new CmDouble(ifField.supported);
            outputFailures = new CmDouble(ofField.supported);
            interfaceSpeed = new CmInteger(isField.supported);
            utilizationPercent = new CmDouble(isField.supported);
            ipAddress = new CmString(iaField == null ? false : iaField.supported);
            subnetMask = new CmString(smField == null ? false : smField.supported);
            if (fdField == null || !fdField.supported)
                isFullDuplex = new CmBoolean(false);
            else
                isFullDuplex = new CmBoolean(new Boolean(true));

            collisionPercent = new CmDouble(cpField != null && cpField.supported);
        }
        if (bytesReceived.isSupported())
            bytesReceived.setValue(((Number) brField.value).intValue());
        if (bytesTransmitted.isSupported())
            bytesTransmitted.setValue(((Number) btField.value).intValue());
        if (packetsReceived.isSupported())
            packetsReceived.setValue(((Number) prField.value).intValue());
        if (packetsTransmitted.isSupported())
            packetsTransmitted.setValue(((Number) ptField.value).intValue());
        if (osFlavor.isUnixHost()) {
            // Unix has already computed rates
            if (inputErrors.isSupported())
                inputErrors.setValue(((Number) ieField.value).doubleValue());
            if (outputErrors.isSupported())
                outputErrors.setValue(((Number) oeField.value).doubleValue());
            if (inputFailures.isSupported())
                inputFailures.setValue(((Number) ifField.value).doubleValue());
            if (outputFailures.isSupported())
                outputFailures.setValue(((Number) ofField.value).doubleValue());
        } else if (osFlavor.isWindowsHost()) {
            // Windows must compute its own
            if (inputErrors.isSupported()) {
                int tmpIErrors = ((Number) ieField.value).intValue();

                inputErrors.setValue(tmpIErrors - oldIErrors);
                oldIErrors = tmpIErrors;
            }
            if (outputErrors.isSupported()) {
                int tmpOErrors = ((Number) oeField.value).intValue();

                outputErrors.setValue(tmpOErrors - oldOErrors);
                oldOErrors = tmpOErrors;
            }
            if (inputFailures.isSupported()) {
                int tmpIFailures = ((Number) ifField.value).intValue();

                inputFailures.setValue(tmpIFailures - oldIFailures);
                oldIFailures = tmpIFailures;
            }
            if (outputFailures.isSupported()) {
                int tmpOFailures = ((Number) ofField.value).intValue();

                outputFailures.setValue(tmpOFailures - oldOFailures);
                oldOFailures = tmpOFailures;
            }
        }
        if (interfaceSpeed.isSupported())
        {
            int iobits = (bytesReceived.intValue() + bytesTransmitted.intValue()) * 8;
            int ifspeed = ((Number) isField.value).intValue();
            double util = (ifspeed == 0) ? 0.0 : ((double) iobits * 100.0) / (double) ifspeed;

            // full duplex divides the utilization in half
            if (isFullDuplex.booleanValue())
                util /= 2.0;

            interfaceSpeed.setValue(ifspeed);
            utilizationPercent.setValue(util);
        }
        if (ipAddress.isSupported())
            ipAddress.setValue((String) iaField.value);
        if (subnetMask.isSupported())
            subnetMask.setValue((String) smField.value);
        if (collisionPercent.isSupported())
            collisionPercent.setValue(((Number) cpField.value).doubleValue());
    }

    /**
     * Remove the values from the <CODE>PerformanceCollection</CODE>.
     * @throws RemoteException RMI exception
     */
    public void remove()
        throws RemoteException
    {
        collection.remove(brField);
        collection.remove(btField);
        collection.remove(prField);
        collection.remove(ptField);
        collection.remove(ieField);
        collection.remove(oeField);
        collection.remove(ifField);
        collection.remove(ofField);
        collection.remove(isField);
        collection.remove(iaField);
        collection.remove(smField);
        collection.remove(fdField);
        collection.remove(cpField);
    }
}
