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

import com.capmet.metrics.jmdr.mdr_t;
import com.capmet.metrics.jmdr.mdr_tuple_t;


/**
 * The mdr_cpuinfo_t class provides data about the CPUs in the machine. This
 * data is not performance related, but descriptive.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_cpuinfo_t extends mdr_t implements mdr_tuple_t
{
    // fields by the number
    public static final int MCI_INSTANCE     = 0;
    public static final int MCI_ARCHITECTURE = 1;
    public static final int MCI_MODEL        = 2;
    public static final int MCI_CLOCK_MHZ    = 3;
    public static final int MCI_FIELD_COUNT  = 4;

    /**
     * Call the constructor for the C++ class
     */
    private native boolean cpuinfo_constructor(String remote_host);
    
    /**
     * mdr_cpuinfo_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_cpuinfo_t() throws mdr_exception_t
    {
        if (!cpuinfo_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    /**
     * mdr_cpuinfo_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_cpuinfo_t(String remote_host) throws mdr_exception_t
    {
        if (!cpuinfo_constructor(remote_host))
            throw new mdr_exception_t(reason);
    }

    /**
     * Wrap things up in the JNI code.
     */
    protected native void finalize();
    
    /**
     * Update the variable with the latest values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_refresh() throws mdr_exception_t
    {
    }

    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
    }

    /**
     * Get the number of entries in this tuple.
     * @return The number of entries in this tuple.
     */
    public native int mdr_size();

    /**
     * Get all the instance names in this tuple.
     * @return An array of <CODE>String</CODE> containing the instance names.
     */
    public native String[] mdr_keyset();

    /**
     * Convenience method used with mdr_keyset() to retrieve instances.
     * @param key The instance name.
     * @param can Can to place the results in.
     * @return True - get succeeded; False - get failed, see reason member.
     */
    public native boolean mdr_get(String key, mci_can_t can);

    // for exclusive use by mdr_field_value
    private native boolean mdr_get_field
        (String key, mci_can_t can, int field_number);

    /**
     * Get the current value of the performance field.
     * @param key The instance name to retrieve the field from.
     * @param field_number The numeric field ID of the field to retrieve.
     * @throws mdr_exception_t Thrown from JNI code
     * @return <CODE>Object</CODE> value of the performance field.
     */
    public Object mdr_field_value(String key, int field_number)
        throws mdr_exception_t
    {
        mci_can_t can = new mci_can_t();

        if (!mdr_get_field(key, can, field_number))
            throw new mdr_exception_t("mdr_cpuinfo_t::mdr_field_value: " +
                msgs.getString("instance") + ": " + key + " " +
                msgs.getString("not_found"));

        switch(field_number)
        {
        case MCI_INSTANCE:
            return new Integer(can.mci_instance);
        case MCI_ARCHITECTURE:
            return new String(can.mci_architecture);
        case MCI_MODEL:
            return new String(can.mci_model);
        case MCI_CLOCK_MHZ:
            return new Integer(can.mci_clock_MHz);
        default:
            throw new mdr_exception_t("mdr_cpuinfo_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }

    static
    {
        System.loadLibrary("jmdr");
    }
}
