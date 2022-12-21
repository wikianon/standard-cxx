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

import java.rmi.NotBoundException;
import java.rmi.server.UID;
import java.net.MalformedURLException;
import java.rmi.RemoteException;
import com.capmet.metrics.rmi.RMIAccessPoint;
import com.capmet.metrics.rmi.CounterCollection;

/**
 * A class for collecting many performance fields into a group
 * for mass collection.
 */
public class PerformanceCollection extends RMIAccessPoint
{
    /**
     * The connection to the server.
     */
    private RMIMethods        remote;
    /**
     * The unique id of the remote object.
     */
    private UID               objectId;
    /**
     * Whether the initialization step has been taken.
     */
    private boolean           initialized;
    /**
     * The collection of counters.
     */
    private CounterCollection collection;

    /**
     * The default constructor for collecting data from the local host.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     */
    public PerformanceCollection() throws
        NotBoundException, MalformedURLException,
        RemoteException, PerformanceException
    {
        this("localhost");
    }

    /**
     * The constructor for collecting data from a remote host.
     * @param remoteHost The remote host to connect to.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     */
    public PerformanceCollection(String remoteHost) throws
        NotBoundException, MalformedURLException,
        RemoteException, PerformanceException
    {
        super(new CaptiveConnection(remoteHost));

        remote = (RMIMethods) getInterface();
        collection = new CounterCollection();
        initialized = false;
    }

    /**
     * Add a singleton field to the collection.
     * @param category The category name of the field.
     * @param field The field name.
     * @throws RemoteException RMI exception
     * @return A <CODE>CollectionItem</CODE> object.
     */
    public CollectionItem put(String category, String field)
        throws RemoteException
    {
        return put(category, field, null);
    }

    /**
     * Add an n-tuple field to the collection.
     * @param category The category name of the field.
     * @param field The field name.
     * @param instance The instance name.
     * @throws RemoteException RMI exception
     * @return A <CODE>CollectionItem</CODE> object.
     */
    public CollectionItem put(String category, String field, String instance)
        throws RemoteException
    {
        CollectionItem item = new CollectionItem();
        UID ciObjectId = new UID();

        item.category = category;
        item.field = field;
        item.instance = instance;
        item.objectId = ciObjectId;

        collection.put(ciObjectId, item);

        if (initialized)
            remote.putCollectionItem(objectId, item);

        return item;
    }

    /**
     * Remove a <CODE>CollectionItem</CODE> object from the collection.
     * @param ci The collection item to remove.
     * @throws RemoteException RMI exception
     */
    public void remove(CollectionItem ci)
        throws RemoteException
    {
        if (ci == null)
            return;
        collection.remove(ci.objectId);
        if (initialized)
            remote.removeCollectionItem(objectId, ci.objectId);
    }

    /**
     * Once the collection has been assembled, query the collection.
     * @throws RemoteException RMI exception
     */
    public void queryCollection()
        throws RemoteException
    {
        FieldArray fieldArray;

        if (!initialized)
        {
            objectId = remote.getCCInstance();
            fieldArray = remote.processCollection(objectId, collection);
            RemoteJanitor.put(objectId, remote);
            initialized = true;
        }
        else
        {
            fieldArray = remote.queryCollection(objectId);
        }

        for(FieldCollectionPair pair : fieldArray)
        {
            UID ciObjectId = pair.ciObjectId;
            CollectionItem ci = collection.get(ciObjectId);

            ci.supported = (pair.value != null);
            ci.value = pair.value;
        }
    }
}
