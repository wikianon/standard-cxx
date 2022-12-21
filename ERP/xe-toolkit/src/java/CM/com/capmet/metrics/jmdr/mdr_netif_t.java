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
import com.capmet.metrics.jmdr.mnet_can_t;


/**
 * The mdr_netif_t class provides data about the performance of the CPUs
 * in the machine. This data is broken down into user time, kernel time,
 * idle time and wait time. The hardware instance number of the CPU is
 * also provided.
 * <p>
 * Methods in this class are <code>refresh, rates</code> and
 * <code>refresh_rates</code> for updating the variable, computing rates
 * for those values that are rates and both respectively.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_netif_t extends mdr_t implements mdr_tuple_t
{
    // fields by the number
    public static final int MNET_IP_ADDRESS        = 0;
    public static final int MNET_NETMASK           = 1;
    public static final int MNET_BROADCAST_ADDRESS = 2;
    public static final int MNET_MTU               = 3;
    public static final int MNET_INPUT_BYTES       = 4;
    public static final int MNET_OUTPUT_BYTES      = 5;
    public static final int MNET_INPUT_PACKETS     = 6;
    public static final int MNET_OUTPUT_PACKETS    = 7;
    public static final int MNET_INPUT_ERRORS      = 8;
    public static final int MNET_OUTPUT_ERRORS     = 9;
    public static final int MNET_INPUT_FAILURES    = 10;
    public static final int MNET_OUTPUT_FAILURES   = 11;
    public static final int MNET_COLLISIONS        = 12;
    public static final int MNET_INTERFACE_SPEED   = 13;
    public static final int MNET_COLLISION_PERCENT = 14;
    public static final int MNET_FULL_DUPLEX       = 15;
    public static final int MNET_FIELD_COUNT       = 16;

    /**
     * Call the constructor for the C++ class
     */
    private native boolean netif_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean netif_refresh();
    
    /**
     * Call the rates method in the C++ class
     */
    private native boolean netif_rates();
    
    /**
     * mdr_netif_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_netif_t() throws mdr_exception_t
    {
        if (!netif_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    /**
     * mdr_netif_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_netif_t(String remote_host) throws mdr_exception_t
    {
        if (!netif_constructor(remote_host))
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
        if (!netif_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
        if (!netif_rates())
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
    public native boolean mdr_get(String key, mnet_can_t can);

    // for exclusive use by mdr_field_value
    private native boolean mdr_get_field
        (String key, mnet_can_t can, int field_number);

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
        mnet_can_t can = new mnet_can_t();

        if (!mdr_get_field(key, can, field_number))
            throw new mdr_exception_t("mdr_netif_t::mdr_field_value: " +
                msgs.getString("instance") + ": " + key + " " +
                msgs.getString("not_found"));

        switch(field_number)
        {
        case MNET_IP_ADDRESS:
            return new String(can.mnet_ip_address);
        case MNET_NETMASK:
            return new String(can.mnet_netmask);
        case MNET_BROADCAST_ADDRESS:
            return new String(can.mnet_broadcast_address);
        case MNET_MTU:
            return new Integer(can.mnet_mtu);
        case MNET_INPUT_BYTES:
            return new Long(can.mnet_input_bytes);
        case MNET_OUTPUT_BYTES:
            return new Long(can.mnet_output_bytes);
        case MNET_INPUT_PACKETS:
            return new Long(can.mnet_input_packets);
        case MNET_OUTPUT_PACKETS:
            return new Long(can.mnet_output_packets);
        case MNET_INPUT_ERRORS:
            return new Long(can.mnet_input_errors);
        case MNET_OUTPUT_ERRORS:
            return new Long(can.mnet_output_errors);
        case MNET_INPUT_FAILURES:
            return new Long(can.mnet_input_failures);
        case MNET_OUTPUT_FAILURES:
            return new Long(can.mnet_output_failures);
        case MNET_COLLISIONS:
            return new Long(can.mnet_collisions);
        case MNET_INTERFACE_SPEED:
            return new Long(can.mnet_interface_speed);
        case MNET_COLLISION_PERCENT:
            return new Double(can.mnet_collision_percent);
        case MNET_FULL_DUPLEX:
            return new Boolean(can.mnet_full_duplex);
        default:
            throw new mdr_exception_t("mdr_netif_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }

    public native void mdr_full_duplex(String name, boolean is_full);

    static
    {
        System.loadLibrary("jmdr");
    }
}
