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
import java.net.MalformedURLException;
import java.rmi.RemoteException;
import com.capmet.metrics.CmFieldType;
import com.capmet.metrics.rmi.RMIAccessPoint;

/**
 * A generic interface to a performance metric, part of a
 * <CODE>PerformanceCategory</CODE>, found on a <CODE>PerformanceHost</CODE>.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class PerformanceField extends RMIAccessPoint
{
    /**
     * The properties of the field retrieved from the RMI server.
     */
    private PerformanceFieldInfo pfi;
    /**
     * The <CODE>Remote</CODE> access point to the RMI server.
     */
    private RMIMethods           remote;
    /**
     * The data type of the field as defined by <CODE>CmFieldType</CODE>.
     */
    private CmFieldType fieldType;

    /**
     * Fully qualified constructor to identify a performance metric
     * on a remote host.
     * @param category The name of the performance category.
     * @param field The name of the field.
     * @param instance The name of the instance if the category is an n-tuple.
     * @param host The name of the remote host.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Field is of unknown type.
     * @see com.capmet.metrics.rmi.PerformanceCategory
     */
    public PerformanceField(
        String category, String field, String instance, String host) throws
            NotBoundException, MalformedURLException, RemoteException, PerformanceException
    {
        super(new CaptiveConnection(host));

        remote = (RMIMethods) getInterface();
        pfi = remote.getPerformanceFieldInfo(category, field, instance);

        RemoteJanitor.put(pfi.objectId, remote);

        Object o = getValue();
        if (o instanceof Integer)
            fieldType = CmFieldType.PF_INT;
        else if (o instanceof Long)
            fieldType = CmFieldType.PF_LONG;
        else if (o instanceof Double)
            fieldType = CmFieldType.PF_DOUBLE;
        else if (o instanceof String)
            fieldType = CmFieldType.PF_STRING;
        else if (o instanceof Boolean)
            fieldType = CmFieldType.PF_BOOLEAN;
        else
            throw new PerformanceException(
                "PerformanceField::PerformanceField: " +
                    RMIMessages.msgs.getString("unknown_type"));
    }

    /**
     * Fully qualified constructor to identify a performance metric
     * on the local host.
     * @param category The name of the performance category.
     * @param field The name of the field.
     * @param instance The name of the instance if the category is an n-tuple.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Field is of unknown type.
     * @see com.capmet.metrics.rmi.PerformanceCategory
     */
    public PerformanceField(
        String category, String field, String instance) throws
            NotBoundException, MalformedURLException, RemoteException, PerformanceException
    {
        this(category, field, instance, "localhost");
    }

    /**
     * Constructor to identify a singleton performance metric
     * on the local host.
     * @param category The name of the performance category.
     * @param field The name of the field.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @throws PerformanceException Field is of unknown type.
     * @see com.capmet.metrics.rmi.PerformanceCategory
     */
    public PerformanceField(String category, String field) throws
            NotBoundException, MalformedURLException, RemoteException, PerformanceException
    {
        this(category, field, null, "localhost");
    }

    /**
     * Return indication of whether this field is supported.
     * @return <CODE>true</CODE>: it is supported - <CODE>false</CODE>: it is not supported.
     */
    public boolean isSupported()
    {
        return pfi.supported;
    }

    /**
     * Get the current value of the performance field.
     * @throws RemoteException RMI exception
     * @return <CODE>Object</CODE> value of the performance field.
     */
    public Object getValue()
        throws RemoteException
    {
        return remote.getValue(pfi.objectId);
    }

    /**
     * Re-read the metric and return the new value of the performance field.
     * @throws RemoteException RMI exception
     * @return <CODE>Object</CODE> value of the performance field.
     */
    public Object getNextValue()
        throws RemoteException
    {
        return remote.getNextValue(pfi.objectId);
    }

    /**
     * Return the data type of the field as a <CODE>CmFieldType</CODE>.
     * @return A <CODE>CmFieldType</CODE> indicating the
     * data type of the field.
     * @see <CODE>CmFieldType</CODE>
     */
    public CmFieldType getFieldType()
    {
        return fieldType;
    }
}
