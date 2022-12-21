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

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.UID;
import java.util.Locale;
import com.capmet.metrics.CmOSFlavor;
import com.capmet.metrics.rmi.PerformanceHost;
import com.capmet.metrics.rmi.PerformanceCategoryInfo;

/**
 * These are the remote method prototypes for the CaptiveCollector RMI server.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public interface RMIMethods extends Remote
{
    /**
     * The signature used by the client and server sides of the RMI to identify this service.
     */
    public final static String RMI_SIGNATURE = "CaptiveCollector";

    /**
     * The TCP port number for the RMI Registry.
     */
    public final static int REGISTRY_PORT = 12760;

    /**
     * Get the OS platform designation as specified by the <code>CmOSFlavor</code> data type.
     * This is a <CODE>PerformanceHost</CODE> remote method.
     * @throws java.rmi.RemoteException RMI exception
     * @return The OS platform designation.
     * @see <CODE>com.capmet.metrics.CmOSFlavor</CODE>
     */
    public CmOSFlavor getOSFlavor()
        throws RemoteException;
    /**
     * Get the names of the categories available on this platform.
     * This is a <CODE>PerformanceHost</CODE> remote method.
     * @throws java.rmi.RemoteException RMI exception
     * @return An array of <CODE>String</CODE> containing the names of the performance categories available.
     */
    public String[] getCategoryNames()
        throws RemoteException;

    /**
     * Return the instance names of an n-tuple category.
     * This is a <CODE>PerformanceCategory</CODE> remote method.
     * @param objectId The unique id of the remote category object.
     * @throws RemoteException RMI exception
     * @return An array of <CODE>String</CODE> containing the instance names with the category.
     */
    public String[] getInstanceNames(UID objectId)
        throws RemoteException;

    /**
     * Retrive the class properties for the <CODE>PerformanceCategory</CODE> class given the category name.
     * This is called from the constructor of the <CODE>PerformanceCategory</CODE> class.
     * @param category The name of the category whose properties are retrieved.
     * @throws RemoteException RMI exception
     * @return A PerformanceCategoryInfo structure containing the properties.
     * @see <CODE>com.capmet.metrics.rmi.PerformanceCategoryInfo</CODE>
     */
    public PerformanceCategoryInfo getPerformanceCategoryInfo(String category)
        throws RemoteException;

    /**
     * Return boolean indication of whether the field in question is supported.
     * This is a <CODE>PerformanceCategory</CODE> remote method.
     * @param objectId The hash code of the <CODE>PerformanceCategory</CODE> object.
     * @param fieldNumber The integer field number.
     * @throws RemoteException RMI exception
     * @return <CODE>true</CODE>: the field is supported - <CODE>false</CODE>: the field is not supported
     */
    public boolean isFieldSupported(UID objectId, int fieldNumber)
        throws RemoteException;

    /**
     * Return the field name given the field number.
     * This is a <CODE>PerformanceCategory</CODE> remote method.
     * @param objectId The hash code of the <CODE>PerformanceCategory</CODE> object.
     * @param fieldNumber The integer field number.
     * @throws RemoteException RMI exception
     * @return The <CODE>String</CODE> name of the field.
     */
    public String getFieldName(UID objectId, int fieldNumber)
        throws RemoteException;

    /**
     * Return the field explanation given the field number.
     * This is a <CODE>PerformanceCategory</CODE> remote method.
     * @param objectId The hash code of the <CODE>PerformanceCategory</CODE> object.
     * @param fieldNumber The integer field number.
     * @throws RemoteException RMI exception
     * @return The <CODE>String</CODE> explanation for the field.
     */
    public String getFieldExplanation(UID objectId, int fieldNumber)
        throws RemoteException;

    /**
     * Return the field number given the field name.
     * This is a <CODE>PerformanceCategory</CODE> remote method.
     * @param objectId The hash code of the <CODE>PerformanceCategory</CODE> object.
     * @param fieldName The <CODE>String</CODE> field name.
     * @throws RemoteException RMI exception
     * @return The integer field number.
     */
    public int getFieldNumber(UID objectId, String fieldName)
        throws RemoteException;

    /**
     * Return a <CODE>String</CODE> array containing the names of all the fields.
     * This is a <CODE>PerformanceCategory</CODE> remote method.
     * @param objectId The hash code of the <CODE>PerformanceCategory</CODE> object.
     * @throws RemoteException RMI exception
     * @return The <CODE>String</CODE> names of all of the fields in the category.
     */
    public String[] getAllFieldNames(UID objectId)
        throws RemoteException;

    /**
     * Return a <CODE>String</CODE> array containing the names of the supported fields.
     * This is a <CODE>PerformanceCategory</CODE> remote method.
     * @param objectId The hash code of the <CODE>PerformanceCategory</CODE> object.
     * @throws RemoteException RMI exception
     * @return The <CODE>String</CODE> names of the supported fields in the category.
     */
    public String[] getSupportedFieldNames(UID objectId)
        throws RemoteException;

    /**
     * Retrive the class properties for the <CODE>PerformanceField</CODE> class given the category name,
     * field name and instance name.
     * This is called from the constructor of the <CODE>PerformanceField</CODE> class.
     * @param category The performance category name.
     * @param field The performance field name.
     * @param instance The instance name.
     * @throws RemoteException RMI exception
     * @return A <CODE>PerformanceFieldInfo</CODE> class containing the properties.
     * @see <CODE>com.capmet.metrics.rmi.PerformanceFieldInfo</CODE>
     */
    public PerformanceFieldInfo getPerformanceFieldInfo(
        String category, String field, String instance)
            throws RemoteException;

    /**
     * Get the current value of the <CODE>PerformanceField</CODE> referenced by the hash code.
     * @param objectId The hash code of the <CODE>PerformanceField</CODE> object.
     * @throws RemoteException RMI exception
     * @return An <CODE>Object</CODE> which will be either an <CODE>Integer, Long, Double, String</CODE> or <CODE>Boolean</CODE>.
     */
    public Object getValue(UID objectId)
        throws RemoteException;

    /**
     * Get the next value of the <CODE>PerformanceField</CODE> referenced by the hash code.
     * @param objectId The hash code of the <CODE>PerformanceField</CODE> object.
     * @throws RemoteException RMI exception
     * @return An <CODE>Object</CODE> which will be either an <CODE>Integer, Long, Double, String</CODE> or <CODE>Boolean</CODE>.
     */
    public Object getNextValue(UID objectId)
        throws RemoteException;

    /**
     * Release <CODE>PerformanceField</CODE> object memory associated with the hash code.
     * @param objectId The hash code of the <CODE>PerformanceField</CODE> object.
     * @throws RemoteException RMI exception
     */
    public void deleteObject(UID objectId)
        throws RemoteException;

    /**
     * Refresh the entire performance category at once.
     * @param objectId The hash code of the <CODE>PerformanceCategory</CODE> object.
     * @throws RemoteException RMI exception
     */
    public void categoryRefresh(UID objectId)
        throws RemoteException;

    /**
     * Refresh the entire performance category and
     * compute the differences from the previous refresh.
     * @param objectId The hash code of the <CODE>PerformanceCategory</CODE> object.
     * @throws RemoteException RMI exception
     */
    public void categoryRefreshRates(UID objectId)
        throws RemoteException;

    /**
     * Retrieve a hash code for a new server-side FieldCollection.
     * @return The hash code of the <CODE>FieldCollection</CODE> object.
     * @throws RemoteException RMI exception
     */
    public UID getCCInstance()
        throws RemoteException;

    /**
     * Query the collection of counters specified by <CODE>coll</CODE>.
     * @param objectId The unique id of the server-side <CODE>FieldCollection</CODE> object.
     * @return A <CODE>FieldArray</CODE> object with the updated query objects.
     * @throws RemoteException RMI exception
     */
    public FieldArray queryCollection(UID objectId)
        throws RemoteException;

    /**
     * Send a newly constructed <CODE>CounterCollection</CODE> to the server and
     * retrieve a unique id for the remote object for further reference.
     * @param objectId The unique id of the server-side <CODE>FieldCollection</CODE> object.
     * @param coll The <CODE>CounterCollection</CODE> to process.
     * @return A <CODE>FieldArray</CODE> object with the query objects.
     * @throws RemoteException RMI exception
     */
    public FieldArray processCollection(UID objectId, CounterCollection coll)
        throws RemoteException;

    /**
     * Add a <CODE>CollectionItem</CODE> to a <CODE>FieldCollection</CODE> that has
     * already been created.
     * @param objectId The unique id of the server-side <CODE>FieldCollection</CODE> object.
     * @param ci The <CODE>CollectionItem</CODE> to add.
     * @throws RemoteException RMI exception
     */
    public void putCollectionItem(UID objectId, CollectionItem ci)
        throws RemoteException;

    /**
     * Remove a <CODE>CollectionItem</CODE> from a <CODE>FieldCollection</CODE> that has
     * already been created.
     * @param objectId The unique id of the server-side <CODE>FieldCollection</CODE> object.
     * @param ciObjectId The unique id of the <CODE>CollectionItem</CODE> to delete.
     * @throws RemoteException RMI exception
     */
    public void removeCollectionItem(UID objectId, UID ciObjectId)
        throws RemoteException;

    /**
     * Get the localized version of the en_US string.
     * @param enUsName The en_US string.
     * @throws RemoteException RMI exception
     * @return The localized version of the string if available.
     */
    public String getLocalizedName(String enUsName)
        throws RemoteException;

    /**
     * Get the en_US version of the localized string.
     * @param localizedName The localized string.
     * @throws RemoteException RMI exception
     * @return The en_US version of the string if available.
     */
    public String getEnUsName(String localizedName)
        throws RemoteException;

    /**
     * Get the locale of the machine running the collector.
     * @throws RemoteException RMI exception
     * @return A <CODE>Locale</CODE> structure from the server.
     */
    public Locale getLocale()
        throws RemoteException;

    //
    // Direct access methods
    //

    /**
     * A remote constructor for the <CODE>P</CODE> class of the <CODE>D</CODE> library.
     * @throws RemoteException RMI exception
     * @return A unique id of the remote process collection.
     */
    public UID PCons() throws RemoteException;

    /**
     * A remote update method to gather information about all processes.
     * @param objectId The unique id of the remote process collection.
     * @throws RemoteException RMI exception
     * @return A <CODE>PT</CODE> object containing all of the processes.
     */
    public D.PT Pget(UID objectId) throws RemoteException;

    //
    // Admin methods
    //

    /**
     * Cause the collector process to exit.
     * @throws RemoteException RMI exception
     */
    public boolean CCShutdown(UID challenge) throws RemoteException;

    /**
     * Retieve the status of the RMI server.
     * @throws RemoteException RMI exception
     * @return Stats about the collector.
     */
    public CaptiveCollectorStats CCStatus() throws RemoteException;

    /**
     * Determine if this host's connection to the remote host is valid.
     * @return true - valid connection; false - connection is broken
     */
    public boolean hasValidConnection() throws RemoteException;
}
