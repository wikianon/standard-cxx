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

import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.net.MalformedURLException;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.Locale;
import com.capmet.metrics.CmOSFlavor;
import com.capmet.metrics.rmi.RMIAccessPoint;

/**
 * A generic interface to a host running the CaptiveCollector RMI service.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class PerformanceHost extends RMIAccessPoint
{
    /**
     * The <CODE>Remote</CODE> access point to the RMI server.
     */
    private RMIMethods  remote;

    /**
     * The name of the remote host connected.
     */
    private String      hostName;

    /**
     * The platform designation of the remote host.
     */
    private CmOSFlavor  osFlavor;

    /**
     * The locale of the remote host.
     */
    private Locale locale;

    /**
     * Quick indication of en_US locale
     */
    private boolean enUsLocale;

    /**
     * The names of all the categories returned by the remote host.
     */
    private TreeSet<String> categorySet;
    /**
     * The names of all the categories returned by the remote host.
     */
    private String[] categoryArray;

    private TreeMap<String, String> localToEnUsNames;
    private TreeMap<String, String> enUsToLocalNames;

    /**
     * The default constructor for the class. A connection will be
     * made to "localhost".
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     */
    public PerformanceHost() throws
        NotBoundException, MalformedURLException, RemoteException
    {
        this(null);
    }

    /**
     * The constructor for accessing a remote host.
     * @param host The name of the remote host to contact.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     */
    public PerformanceHost(String host) throws
        NotBoundException, MalformedURLException, RemoteException
    {
        super(new CaptiveConnection(host));

        remote = (RMIMethods) getInterface();
        osFlavor = remote.getOSFlavor();
        locale = remote.getLocale();
        if (locale.equals(Locale.US))
            enUsLocale = true;
        else
        {
            localToEnUsNames = new TreeMap<String, String>();
            enUsToLocalNames = new TreeMap<String, String>();
        }
        hostName = host;
        categoryArray = remote.getCategoryNames();
        categorySet = new TreeSet<String>();
        for(String s : categoryArray)
            categorySet.add(s);
    }

    /**
     * Return the OS platform designation as specified by the <code>CmOSFlavor</code> data type.
     * @return The OS platform designation.
     * @see <CODE>com.capmet.metrics.CmOSFlavor</CODE>
     */
    public CmOSFlavor getOSFlavor()
    {
        return osFlavor;
    }

    /**
     * Return the name of the host connected to.
     * @return The <CODE>String</CODE> name of the monitored host.
     */
    public String getHostName()
    {
        return hostName;
    }

    /**
     * Retrieve a <CODE>PerformanceCategory</CODE> object for the specified category.
     * @param categoryName The category to retrieve an object for.
     * @return a <CODE>PerformanceCategory</CODE> object.
     * @throws NotBoundException Service is not bound on server machine.
     * @throws MalformedURLException The service signature is malformed.
     * @throws RemoteException RMI exception
     * @see <CODE>com.capmet.metrics.rmi.PerformanceCategory</CODE>
     */
    public PerformanceCategory getCategory(String categoryName) throws
        NotBoundException, MalformedURLException, RemoteException
    {
        return new PerformanceCategory(categoryName, hostName);
    }

    /**
     * Get the names of the categories available on the remote host.
     * @return An array of <CODE>String</CODE> containing the names of the performance categories available.
     */
    public String[] getCategoryNames()
    {
        return categoryArray;
    }

    /**
     * Indicates whether the remote host contains the specified category.
     * @param category The category in question.
     * @return A boolean indication of whether the category is supported.
     */
    public boolean containsCategory(String category)
    {
        return categorySet.contains(category);
    }

    /**
     * Get the localized version of the en_US string.
     * @param enUsName The en_US string.
     * @throws RemoteException RMI exception
     * @return The localized version of the string if available.
     */
    public String getLocalizedName(String enUsName)
        throws RemoteException
    {
        if (enUsLocale)
            return enUsName;

        if (enUsToLocalNames.containsKey(enUsName))
            return enUsToLocalNames.get(enUsName);

        String lname = remote.getLocalizedName(enUsName);

        enUsToLocalNames.put(enUsName, lname);

        return lname;
    }

    /**
     * Get the en_US version of the localized string.
     * @param localizedName The localized string.
     * @throws RemoteException RMI exception
     * @return The en_US version of the string if available.
     */
    public String getEnUsName(String localizedName)
        throws RemoteException
    {
        if (enUsLocale)
            return localizedName;

        if (localToEnUsNames.containsKey(localizedName))
            return localToEnUsNames.get(localizedName);

        String usName = remote.getEnUsName(localizedName);

        localToEnUsNames.put(localizedName, usName);

        return usName;
    }

    /**
     * Get the locale of the machine running the collector.
     * @return A <CODE>Locale</CODE> structure from the server.
     */
    public Locale getLocale()
    {
        return locale;
    }

    /**
     * Determine if this host's connection to the remote host is valid.
     * @return true - valid connection; false - connection is broken
     */
    public boolean hasValidConnection() {
        try {
            return remote.hasValidConnection();
        } catch(Exception e) {
            // if it exceptions out then it's not valid
            return false;
        }
    }
}
