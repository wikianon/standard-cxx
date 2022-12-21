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

import com.capmet.metrics.jmdr.mdr_exception_t;

/**
 * A generic interface to a category of performance data found on an
 * <CODE>mdr_performance_host_t</CODE>.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_category_t
{
    /**
     * Is this category supported?
     */
    public boolean mdr_category_supported;
    /**
     * Is this category a singleton?
     */
    public boolean mdr_is_singleton;
    /**
     * The number of fields in this category.
     */
    public int     mdr_field_count;

    private long handle;
    private String reason;

    private native boolean  category_constructor(
        String category, String hostname);
    private native Boolean  is_supported(int field_number);
    private native String   field_name(int field_number);
    private native String   field_explanation(int field_number);
    private native int      field_name_to_number(String field_name);
    private native String   category_explanation();

    /**
     * Constructor for a category on the local host.
     *
     * @param category The name of the performance category.
     * @throws mdr_exception_t Thrown from JNI code.
     */
    public mdr_category_t(String category)
        throws mdr_exception_t
    {
        this(category, null);
    }

    /**
     * Constructor for a category on the specified machine.
     *
     * @param category The name of the performance category.
     * @param hostname The name of the remote host or null for localhost.
     * @throws mdr_exception_t Thrown from JNI code.
     */
    public mdr_category_t(String category, String hostname)
        throws mdr_exception_t
    {
        if (!category_constructor(category, hostname))
            throw new mdr_exception_t(reason);
    }

    /**
     * Determine if a field is supported.
     *
     * @param field_number The numeric field number of the category.
     * @throws mdr_exception_t Thrown from JNI code.
     * @return True - field is supported; false - it is not.
     */
    public boolean mdr_is_supported(int field_number)
        throws mdr_exception_t
    {
        Boolean b = is_supported(field_number);

        if (b == null)
            throw new mdr_exception_t(reason);

        return b.booleanValue();
    }

    /**
     * Retrieve the name of a field.
     *
     * @param field_number The numeric field number of the category.
     * @throws mdr_exception_t Thrown from JNI code.
     * @return The field name of the given field number.
     */
    public String mdr_field_name(int field_number)
        throws mdr_exception_t
    {
        String retVal = field_name(field_number);

        if (retVal == null)
            throw new mdr_exception_t(reason);

        return retVal;
    }

    /**
     * Retrieve the explanation text of a field.
     *
     * @param field_number The numeric field number of the category.
     * @throws mdr_exception_t Thrown from JNI code.
     * @return The field explanation of the given field number.
     */
    public String mdr_field_explanation(int field_number)
        throws mdr_exception_t
    {
        String retVal = field_explanation(field_number);

        if (retVal == null)
            throw new mdr_exception_t(reason);

        return retVal;
    }

    /**
     * Retrieve the explanation text of the category.
     *
     * @throws mdr_exception_t Thrown from JNI code.
     * @return The explanation for the category.
     */
    public String mdr_category_explanation()
        throws mdr_exception_t
    {
        String s = category_explanation();

        if (s == null)
            throw new mdr_exception_t(reason);

        return s;
    }

    /**
     * Convert a field name to its numeric field value.
     *
     * @return The numeric value of the field name or -1 if it is not
     * a valid field name.
     */
    public int mdr_field_name_to_number(String field_name)
    {
        return field_name_to_number(field_name);
    }

    private native String[] all_field_names();
    private native String[] supported_field_names();

    /**
     * Retrieve the names of all of the fields of the category.
     *
     * @throws mdr_exception_t Thrown from JNI code.
     * @return An array of <CODE>String</CODE> containing the field names.
     */
    public String[] mdr_all_field_names() throws mdr_exception_t
    {
        String[] retVal = all_field_names();

        if (retVal == null)
            throw new mdr_exception_t(reason);

        return retVal;
    }

    /**
     * Retrieve the names of the supported fields of the category.
     *
     * @throws mdr_exception_t Thrown from JNI code.
     * @return An array of <CODE>String</CODE> containing the field names.
     */
    public String[] mdr_supported_field_names() throws mdr_exception_t
    {
        String[] retVal = supported_field_names();

        if (retVal == null)
            throw new mdr_exception_t(reason);

        return retVal;
    }

    private native String[] get_instance_names();

    /**
     * Retrieve all the instance names of the catetory. Note: the category
     * must be an n-tuple or an exception will be thrown.
     *
     * @throws mdr_exception_t Thrown from JNI code.
     * @return An array of <CODE>String</CODE> containing the instance names.
     */
    public String[] mdr_get_instance_names() throws mdr_exception_t
    {
        String[] result = get_instance_names();

        if (result == null)
            throw new mdr_exception_t(reason);

        return result;
    }

    private native boolean refresh();

    /**
     * Refresh the values of the fields of the category.
     *
     * @throws mdr_exception_t Thrown from JNI code.
     */
    public void mdr_refresh() throws mdr_exception_t
    {
        if (!refresh())
            throw new mdr_exception_t(reason);
    }

    private native boolean refresh_rates();

    /**
     * Retrieve all the field names of the catetory and compute the difference
     * for all rate fields of the category.
     *
     * @throws mdr_exception_t Thrown from JNI code.
     */
    public void mdr_refresh_rates() throws mdr_exception_t
    {
        if (!refresh_rates())
            throw new mdr_exception_t(reason);
    }

    private static native String release_host(String host);

    /**
     * Cause the underlying JNI code to release all category resources
     * related to the specified host.
     *
     * @param host The name of the host whose resources should be released.
     * @throws mdr_exception_t Thrown from JNI code.
     */
    public static void mdr_release_host(String host)
        throws mdr_exception_t
    {
        String why = release_host(host);

        if (why != null)
            throw new mdr_exception_t(why);
    }

    /**
     * Wrap things up in the JNI code.
     */
    protected native void finalize();

    static
    {
        System.loadLibrary("jmdr");
    }
}
