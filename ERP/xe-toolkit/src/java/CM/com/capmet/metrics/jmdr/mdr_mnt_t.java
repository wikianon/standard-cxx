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
import com.capmet.metrics.jmdr.mdr_string_t;
import com.capmet.metrics.jmdr.mdr_exception_t;
import com.capmet.metrics.jmdr.mmnt_can_t;


/**
 * The mdr_mnt_t class provides data about the mnts in the operating
 * system that report data about their effectiveness.
 * <p>
 * Methods in this class are <code>refresh, rates and refresh_rates</code>
 * for updating the variable, computing rates for those values that are
 * rates and both respectively.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_mnt_t extends mdr_t implements mdr_tuple_t
{
    // fields by the number
    public static final int MMNT_SPECIAL        = 0;
    public static final int MMNT_MOUNT_POINT    = 1;
    public static final int MMNT_FS_TYPE        = 2;
    public static final int MMNT_MOUNT_OPTIONS  = 3;
    public static final int MMNT_MOUNT_TIME     = 4;
    public static final int MMNT_SIZE           = 5;
    public static final int MMNT_USED_K         = 6;
    public static final int MMNT_FREE_K         = 7;
    public static final int MMNT_PERCENT_USED   = 8;
    public static final int MMNT_FILES          = 9;
    public static final int MMNT_FREE_FILES     = 10;
    public static final int MMNT_MAJOR          = 11;
    public static final int MMNT_MINOR          = 12;
    public static final int MMNT_FILE_SYSTEM_ID = 13;
    public static final int MMNT_FIELD_COUNT    = 14;

    /**
     * Call the constructor for the C++ class
     */
    private native boolean mnt_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean mnt_refresh();
    
    /**
     * Call the rates method in the C++ class
     */
    private native boolean mnt_rates();
    
    /**
     * mdr_mnt_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_mnt_t() throws mdr_exception_t
    {
        if (!mnt_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    /**
     * mdr_mnt_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_mnt_t(String remote_host) throws mdr_exception_t
    {
        if (!mnt_constructor(remote_host))
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
        if (!mnt_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
        if (!mnt_rates())
            throw new mdr_exception_t(reason);
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
    public native boolean mdr_get(String key, mmnt_can_t can);

    // for exclusive use by mdr_field_value
    private native boolean mdr_get_field
        (String key, mmnt_can_t can, int field_number);

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
        mmnt_can_t can = new mmnt_can_t();

        if (!mdr_get_field(key, can, field_number))
            throw new mdr_exception_t("mdr_mnt_t::mdr_field_value: " +
                msgs.getString("instance") + ": " + key + " " +
                msgs.getString("not_found"));

        switch(field_number)
        {
        case MMNT_SPECIAL:
            return new String(can.mmnt_special);
        case MMNT_MOUNT_POINT:
            return new String(can.mmnt_mount_point);
        case MMNT_FS_TYPE:
            return new String(can.mmnt_fs_type);
        case MMNT_MOUNT_OPTIONS:
            return new String(can.mmnt_mount_options);
        case MMNT_MOUNT_TIME:
            return new Long(can.mmnt_mount_time);
        case MMNT_SIZE:
            return new Long(can.mmnt_size_K);
        case MMNT_USED_K:
            return new Long(can.mmnt_used_K);
        case MMNT_FREE_K:
            return new Long(can.mmnt_free_K);
        case MMNT_PERCENT_USED:
            return new Double(can.mmnt_percent_used);
        case MMNT_FILES:
            return new Long(can.mmnt_files);
        case MMNT_FREE_FILES:
            return new Long(can.mmnt_free_files);
        case MMNT_MAJOR:
            return new Integer(can.mmnt_major);
        case MMNT_MINOR:
            return new Integer(can.mmnt_minor);
        case MMNT_FILE_SYSTEM_ID:
            return new Long(can.mmnt_file_system_id);
        default:
            throw new mdr_exception_t("mdr_mnt_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }
    
    static
    {
        System.loadLibrary("jmdr");
    }
}
