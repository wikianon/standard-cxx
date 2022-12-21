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

package com.capmet.metrics.jmdr;

import com.capmet.metrics.jmdr.mdr_category_t;
import com.capmet.metrics.jmdr.mdr_exception_t;
import com.capmet.metrics.CmOSFlavor;
import java.nio.*;
import java.nio.charset.*;

/**
 * Interface to the MDR JNI library on a host.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_performance_host_t
{
    public CmOSFlavor mdr_os_flavor;

    private long handle;
    private String reason;
    private String remote_host;
    private int os_flavor;

    private native boolean cat_constructor(String host_name);
    private native void convert_strings_to_utf8(String callback);

    /**
     * The default constructor for the class. Retrieve metrics from localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_performance_host_t() throws mdr_exception_t
    {
        this("localhost");
    }

    /**
     * The constructor for accessing a remote host.
     * NOTE: This relies on the mdrd service which has fallen into a state
     * of disrepair. Use of mdrd is not recommended since it is not secure.
     * @param host_name The name of the remote host to contact.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_performance_host_t(String host_name) throws mdr_exception_t
    {
        boolean localhost =
            (host_name == null || host_name.equals("localhost"));

        remote_host = host_name;

        if (!cat_constructor(localhost ? null : remote_host))
            throw new mdr_exception_t(reason);

        mdr_os_flavor = CmOSFlavor.toOSFlavor(os_flavor);

        if (localhost)
            remote_host = null;

        // Windows must be coerced into cooperating.
        if (mdr_os_flavor == CmOSFlavor.MOS_WINDOWS)
            convert_strings_to_utf8("string_converter");
    }

    // Called back from C++land. Convert windows-1252 (or whatever) to UTF-8.
    // See matching comment in src/jmdr/windows/mdr_perf_host.cpp.

    private void string_converter(String[] localStrings)
    {
        String charSet = Charset.defaultCharset().toString();
        int length = localStrings.length;

        for(int i=0; i<length; i++) {
            String fixed = localStrings[i];

            try {
                fixed = new String(fixed.getBytes(), charSet);
            } catch(Exception e) {
                fixed = localStrings[i];
            }

            localStrings[i] = fixed;
        }
    }

    /**
     * Wrap things up in the JNI code.
     */
    protected native void finalize();

    private native String[] get_category_names();

    /**
     * Get the names of the categories available on the host.
     * @throws mdr_exception_t Thrown from JNI code
     * @return An array of <CODE>String</CODE> containing the names of the performance categories available.
     */
    public String[] mdr_get_category_names() throws mdr_exception_t
    {
        String[] result = get_category_names();

        if (result == null)
            throw new mdr_exception_t(reason);

        return result;
    }

    /**
     * Retrieve a <CODE>mdr_category_t</CODE> object for the specified category.
     * @param name The category to retrieve an object for.
     * @return a <CODE>mdr_category_t</CODE> object.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_category_t mdr_get_category_by_name(String name)
        throws mdr_exception_t
    {
        return new mdr_category_t(name, remote_host);
    }

    private native String localized_name(String en_us_name);

    /**
     * Get the localized version of the en_US string.
     * @param en_us_name The en_US string.
     * @throws mdr_exception_t Thrown from JNI code
     * @return The localized version of the string if available.
     */
    public String mdr_localized_name(String en_us_name)
        throws mdr_exception_t
    {
        String lname = localized_name(en_us_name);

        if (lname == null)
            throw new mdr_exception_t(reason);

        return lname;
    }

    private native String en_us_name(String localized_name);

    /**
     * Get the en_US version of the localized string.
     * @param localized_name The localized string.
     * @throws mdr_exception_t Thrown from JNI code
     * @return The en_US version of the string if available.
     */
    public String mdr_en_us_name(String localized_name)
        throws mdr_exception_t
    {
        String us = en_us_name(localized_name);

        if (us == null)
            throw new mdr_exception_t(reason);

        return us;
    }

    static
    {
        System.loadLibrary("jmdr");
    }
}
