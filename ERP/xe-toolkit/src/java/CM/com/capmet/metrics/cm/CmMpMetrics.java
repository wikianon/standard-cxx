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
 * of the TreeMap used in the <CODE>CmMpStat</CODE> class. For each
 * CPU name as the key, this class contains the values.
 */
public class CmMpMetrics
{
    /**
     * The percentage of the elapsed time spent executing code
     * in user space.
     */
    public CmInteger userTimePercent;
    /**
     * The percentage of the elapsed time spent executing
     * priviledged code within the operating system kernel.
     */
    public CmInteger systemTimePercent;
    /**
     * The percentage of the elapsed time while there was at least one
     * pending I/O request charged to this CPU.
     */
    public CmInteger waitTimePercent;
    /**
     * The percentage of elapsed time that the operating system kernel
     * was executing the idle thread.
     */
    public CmInteger idleTimePercent;

    // private members in support of the class
    private CollectionItem userField;
    private CollectionItem kernField;
    private CollectionItem waitField;
    private CollectionItem idleField;
    private CollectionItem intrField;   // Windoze
    private CollectionItem procField;   // Windoze

    private CmOSFlavor            osFlavor;
    private PerformanceCollection collection;
    private boolean               initialized;

    /**
     * Lone constructor for the class.
     * @param host The <CODE>PerformanceHost</CODE> object for the remote host.
     * @param pcat The <CODE>PerformanceCategory</CODE> object for CPU.
     * @param instance The name of the CPU instance.
     * @param coll The <CODE>PerformanceCollection</CODE> holding all the values.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Any possible lower level exception.
     */
    public CmMpMetrics(PerformanceHost host, PerformanceCategory pcat,
        String instance, PerformanceCollection coll)
            throws RemoteException, NotBoundException,
                MalformedURLException, PerformanceException
    {
        String category = pcat.getCategoryName();

        this.osFlavor = host.getOSFlavor();
        this.collection = coll;

        if (osFlavor.isUnixHost()) {
            String u = host.getLocalizedName("Percent User Time");
            String k = host.getLocalizedName("Percent Kernel Time");
            String w = host.getLocalizedName("Percent Wait Time");
            String i = host.getLocalizedName("Percent Idle Time");

            userField = coll.put(category, u, instance);
            kernField = coll.put(category, k, instance);
            waitField = coll.put(category, w, instance);
            idleField = coll.put(category, i, instance);
        } else if (osFlavor.isWindowsHost()) {
            String u = host.getLocalizedName("% User Time");
            String p = host.getLocalizedName("% Privileged Time");
            String i = host.getLocalizedName("% Interrupt Time");
            String idleName = host.getLocalizedName("% Idle Time");

            userField = coll.put(category, u, instance);
            kernField = coll.put(category, p, instance);
            intrField = coll.put(category, i, instance);
            if (pcat.getFieldNumber(idleName) != -1)
                idleField  = coll.put(category, idleName, instance);
            else
            {
                String pp = host.getLocalizedName("% Processor Time");
                procField  = coll.put(category, pp,  instance);
            }
        }

        // flag initialization needed to resync
        initialized = false;
    }

    /**
     * Resync the values from the <CODE>PerformanceCollection</CODE> to this node.
     */
    public void resync()
    {
        if (!initialized)
        {
            userTimePercent = new CmInteger(true);
            systemTimePercent = new CmInteger(true);
            if (waitField == null)
                waitTimePercent = new CmInteger(false);
            else
                waitTimePercent = new CmInteger(waitField.supported);
            idleTimePercent = new CmInteger(true);
        }

        if (osFlavor.isUnixHost())
        {
            userTimePercent.setValue(((Number) userField.value).intValue());
            systemTimePercent.setValue(((Number) kernField.value).intValue());
            if (waitTimePercent.isSupported())
                waitTimePercent.setValue(((Number) waitField.value).intValue());
            idleTimePercent.setValue(((Number) idleField.value).intValue());
        }
        else if (osFlavor.isWindowsHost())
        {
            Number u = (Number) userField.value;
            Number p = (Number) kernField.value;
            Number r = (Number) intrField.value;
            Number i;
            int idl;

            // Win2K does not have % Idle Time
            if (idleField != null)
            {
                i = (Number) idleField.value;
                idl = i.intValue();
            }
            else
            {
                i = (Number) procField.value;
                idl = 100 - i.intValue();
            }

            int usr = u.intValue();
            int sys = p.intValue() + r.intValue();
            int total = usr + sys + idl;

            if (total != 100)
            {
                idl += (100 - total);
                if (idl < 0)
                {
                    if (usr > sys)
                        usr += idl;
                    else
                        sys += idl;
                    idl = 0;
                }
            }

            userTimePercent.setValue(usr);
            systemTimePercent.setValue(sys);
            idleTimePercent.setValue(idl);
        }
    }

    /**
     * Remove the values from the <CODE>PerformanceCollection</CODE>.
     * @throws RemoteException RMI exception
     */
    public void remove()
        throws RemoteException
    {
        collection.remove(userField);
        collection.remove(kernField);
        if (waitField != null)
            collection.remove(waitField);
        collection.remove(idleField);
        if (intrField != null)
            collection.remove(intrField);
        if (procField != null)
            collection.remove(procField);
    }
}
