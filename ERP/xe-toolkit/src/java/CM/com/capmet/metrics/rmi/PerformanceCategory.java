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
import java.rmi.RemoteException;
import java.net.MalformedURLException;
import com.capmet.metrics.rmi.RMIAccessPoint;

/**
 * A generic interface to a category of performance data found on a
 * <CODE>PerformanceHost</CODE>.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class PerformanceCategory extends RMIAccessPoint
{
    /**
     * The <CODE>Remote</CODE> access point to the RMI server.
     */
    private RMIMethods              remote;
    /**
     * The properties of the category retrieved from the RMI server.
     */
    private PerformanceCategoryInfo pci;
    /**
     * The name of the remote host.
     */
    private String                  hostName;

    /**
     * The constructor to obtain a category of data on the local host.
     * @param category The name of the category.
     * @throws java.rmi.NotBoundException Service is not bound on server machine.
     * @throws java.net.MalformedURLException The service signature is malformed.
     * @throws java.rmi.RemoteException RMI exception
     */
    public PerformanceCategory(String category) throws
        NotBoundException, MalformedURLException, RemoteException
    {
        this(category, "localhost");
    }

    /**
     * The constructor to obtain a category of data on a remote host.
     * @param category The name of the category to retrieve.
     * @param host The host to retrieve the category from.
     * @throws java.rmi.NotBoundException Service is not bound on server machine.
     * @throws java.net.MalformedURLException The service signature is malformed.
     * @throws java.rmi.RemoteException RMI exception
     */
    public PerformanceCategory(String category, String host) throws
        NotBoundException, MalformedURLException, RemoteException
    {
        super(new CaptiveConnection(host));

        remote = (RMIMethods) getInterface();
        pci = remote.getPerformanceCategoryInfo(category);
        hostName = host;
        RemoteJanitor.put(pci.objectId, remote);
    }

    /**
     * Return the name of the category.
     * @return The <CODE>String</CODE> name of the category.
     */
    public String getCategoryName()
    {
        return pci.categoryName;
    }

    /**
     * Return the explanation of the category.
     * @return The <CODE>String</CODE> explanation of the category.
     */
    public String getCategoryExplanation()
    {
        return pci.categoryExplanation;
    }

    /**
     * Return indication of whether this category is supported.
     * @return <CODE>true:</CODE> the category is supported - <CODE>false:</CODE> the category is not supported.
     */
    public boolean isCategorySupported()
    {
        return pci.categorySupported;
    }

    /**
     * Return indication of whether the category is a singleton or an n-tuple.
     * @return <CODE>true:</CODE> category is a singleton - <CODE>false:</CODE> category is an n-tuple.
     */
    public boolean isSingleton()
    {
        return pci.singleton;
    }

    /**
     * Return the number of fields in this category.
     * @return An <CODE>int</CODE> value representing the cardinal number of fields present in the category.
     */
    public int getFieldCount()
    {
        return pci.fieldCount;
    }

    /**
     * Return indication of whether the field in question is supported in this category.
     * @param fieldNumber The field number in question.
     * @return <CODE>true:</CODE> the field is supported - <CODE>false:</CODE> the field in not supported.
     * @throws java.rmi.RemoteException RMI exception
     */
    public boolean isFieldSupported(int fieldNumber)
        throws RemoteException
    {
        return remote.isFieldSupported(pci.objectId, fieldNumber);
    }

    /**
     * Return the name of the field given the field number.
     * @param fieldNumber The cardinal number of the field.
     * @throws java.rmi.RemoteException RMI exception
     * @return The <CODE>String</CODE> name of the field.
     */
    public String getFieldName(int fieldNumber)
        throws RemoteException
    {
        return remote.getFieldName(pci.objectId, fieldNumber);
    }

    /**
     * Return the explanation of the field given the field name.
     * @param fieldName The name of the field.
     * @return The <CODE>String</CODE> explanation for the field.
     * @throws java.rmi.RemoteException RMI exception
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     */
    public String getFieldExplanation(String fieldName)
        throws RemoteException, PerformanceException
    {
        int fieldNumber = getFieldNumber(fieldName);

        if (fieldNumber == -1)
            throw new PerformanceException(
                "PerformanceCategory::getFieldExplanation: " +
                RMIMessages.msgs.getString("field") + ": " + fieldName + " " +
                RMIMessages.msgs.getString("not_found"));

        return getFieldExplanation(fieldNumber);
    }

    /**
     * Return the explanation of the field given the field number.
     * @param fieldNumber The cardinal number of the field.
     * @throws java.rmi.RemoteException RMI exception
     * @return The <CODE>String</CODE> explanation for the field.
     */
    public String getFieldExplanation(int fieldNumber)
        throws RemoteException
    {
        return remote.getFieldExplanation(pci.objectId, fieldNumber);
    }

    /**
     * Return the field number given the field name.
     * @param fieldName The name of the field.
     * @throws java.rmi.RemoteException RMI exception
     * @return The cardinal number of the field.
     */
    public int getFieldNumber(String fieldName)
        throws RemoteException
    {
        return remote.getFieldNumber(pci.objectId, fieldName);
    }

    /**
     * Retrieve a <CODE>PerformanceField</CODE> object from the singleton category for the given field number.
     * @param fieldNumber The cardinal number of the field.
     * @throws PerformanceException The generic com.capmet.metrics.rmi exception type
     * @throws java.rmi.NotBoundException Service is not bound on server machine.
     * @throws java.net.MalformedURLException The service signature is malformed.
     * @throws java.rmi.RemoteException RMI exception
     * @return A <CODE>PerformanceField</CODE> object for the given field number.
     * @see com.capmet.metrics.rmi.PerformanceField
     */
    public PerformanceField getField(int fieldNumber)
        throws PerformanceException,
            NotBoundException, MalformedURLException, RemoteException
    {
        return getField(getFieldName(fieldNumber));
    }

    /**
     * Retrieve a <CODE>PerformanceField</CODE> object from the singleton category for the given field name.
     * @param fieldName The <CODE>String</CODE> name of the field.
     * @throws PerformanceException The generic com.capmet.metrics.rmi exception type
     * @throws java.rmi.NotBoundException Service is not bound on server machine.
     * @throws java.net.MalformedURLException The service signature is malformed.
     * @throws java.rmi.RemoteException RMI exception
     * @return A <CODE>PerformanceField</CODE> object for the given field name.
     * @see com.capmet.metrics.rmi.PerformanceField
     */
    public PerformanceField getField(String fieldName)
        throws PerformanceException,
            NotBoundException, MalformedURLException, RemoteException
    {
        if (!pci.singleton)
            throw new PerformanceException("PerformanceCategory::getField: " +
                RMIMessages.msgs.getString("category") + ": " +
                pci.categoryName + " " +
                RMIMessages.msgs.getString("req_inst"));

        return new PerformanceField(
            pci.categoryName, fieldName, null, hostName);
    }

    /**
     * Retrieve a <CODE>PerformanceField</CODE> object from the n-tuple category for the given
     * instance name and field number.
     * @param fieldNumber The cardinal field number.
     * @param instance The name of the instance in the specified n-tuple.
     * @throws PerformanceException The generic com.capmet.metrics.rmi exception type
     * @throws java.rmi.NotBoundException Service is not bound on server machine.
     * @throws java.net.MalformedURLException The service signature is malformed.
     * @throws java.rmi.RemoteException RMI exception
     * @return A <CODE>PerformanceField</CODE> object for the given instance name and field number.
     * @see com.capmet.metrics.rmi.PerformanceField
     */
    public PerformanceField getField(int fieldNumber, String instance)
        throws PerformanceException,
            NotBoundException, MalformedURLException, RemoteException
    {
        return getField(getFieldName(fieldNumber), instance);
    }

    /**
     * Retrieve a <CODE>PerformanceField</CODE> object from the n-tuple category for the given
     * instance name and field name.
     * @param fieldName The <CODE>String</CODE> name of the field.
     * @param instanceName The instance name containing the field in question.
     * @throws PerformanceException The generic com.capmet.metrics.rmi exception type
     * @throws java.rmi.NotBoundException Service is not bound on server machine.
     * @throws java.net.MalformedURLException The service signature is malformed.
     * @throws java.rmi.RemoteException RMI exception
     * @return A <CODE>PerformanceField</CODE> object for the given instance name and field name.
     * @see com.capmet.metrics.rmi.PerformanceField
     */
    public PerformanceField getField(String fieldName, String instanceName)
        throws PerformanceException,
            NotBoundException, MalformedURLException, RemoteException
    {
        if (pci.singleton)
            throw new PerformanceException("PerformanceCategory::getField: " +
                RMIMessages.msgs.getString("category") + ": " +
                pci.categoryName + " " +
                RMIMessages.msgs.getString("not_tuple"));

        return new PerformanceField(
            pci.categoryName, fieldName, instanceName, hostName);
    }

    /**
     * Retrieve the instance names of the n-tuple category.
     * @throws java.rmi.RemoteException RMI exception
     * @return An array of <CODE>String</CODE> with the names of the instances of the n-tuple category.
     */
    public String[] getInstanceNames()
        throws RemoteException
    {
        return remote.getInstanceNames(pci.objectId);
    }

    /**
     * Retrieve all field names of the n-tuple category.
     * @throws java.rmi.RemoteException RMI exception
     * @return An array of <CODE>String</CODE> with the names of the fields of the category.
     */
    public String[] getAllFieldNames()
        throws RemoteException
    {
        return remote.getAllFieldNames(pci.objectId);
    }

    /**
     * Retrieve the supported field names of the n-tuple category.
     * @throws java.rmi.RemoteException RMI exception
     * @return An array of <CODE>String</CODE> with the names of the fields of the category.
     */
    public String[] getSupportedFieldNames()
        throws RemoteException
    {
        return remote.getSupportedFieldNames(pci.objectId);
    }

    /**
     * Refresh the entire performance category at once.
     * @throws java.rmi.RemoteException RMI exception
     */
    public void categoryRefresh()
        throws RemoteException
    {
        remote.categoryRefresh(pci.objectId);
    }

    /**
     * Refresh the entire performance category and
     * compute the differences from the previous refresh.
     * @throws java.rmi.RemoteException RMI exception
     */
    public void categoryRefreshRates()
        throws RemoteException
    {
        remote.categoryRefreshRates(pci.objectId);
    }
}
