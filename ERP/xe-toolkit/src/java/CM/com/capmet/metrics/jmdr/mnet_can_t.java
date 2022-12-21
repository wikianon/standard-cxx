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

/**
 * mnet_can_t is the unit container for the mdr_netif_t class
 * which is a mapping of interface name to mnet_can_t.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mnet_can_t
{
    /**
     * IP Address
     */
    public String  mnet_ip_address;
    /**
     * Network Mask
     */
    public String  mnet_netmask;
    /**
     * Broadcast Address
     */
    public String  mnet_broadcast_address;
    /**
     * Maximum Transfer Unit
     */
    public int     mnet_mtu;
    /**
     * Bytes Received
     */
    public long    mnet_input_bytes;
    /**
     * Bytes Transmitted
     */
    public long    mnet_output_bytes;
    /**
     * Packets Received
     */
    public long    mnet_input_packets;
    /**
     * Packets Transmitted
     */
    public long    mnet_output_packets;
    /**
     * Errors on Receipt
     */
    public long    mnet_input_errors;
    /**
     * Errors on Transmission
     */
    public long    mnet_output_errors;
    /**
     * Failures on Receipt
     */
    public long    mnet_input_failures;
    /**
     * Failures on Transmission
     */
    public long    mnet_output_failures;
    /**
     * Collisions on Transmission
     */
    public long    mnet_collisions;
    /**
     * Interface Speed (b/s)
     */
    public long    mnet_interface_speed;
    /**
     * Collision Percent
     */
    public double  mnet_collision_percent;
    /**
     * Interface is Full Duplex
     */
    public boolean mnet_full_duplex;

    /**
     * Set all public members to their initial value.
     */
    public void clear()
    {
        mnet_ip_address = null;
        mnet_netmask = null;
        mnet_broadcast_address = null;
        mnet_mtu = 0;
        mnet_input_bytes = 0;
        mnet_output_bytes = 0;
        mnet_input_packets = 0;
        mnet_output_packets = 0;
        mnet_input_errors = 0;
        mnet_output_errors = 0;
        mnet_input_failures = 0;
        mnet_output_failures = 0;
        mnet_collisions = 0;
        mnet_interface_speed = 0;
        mnet_collision_percent = 0.0;
        mnet_full_duplex = false;
    }

    /**
     * The default constructor.
     */
    public mnet_can_t()
    {
        clear();
    }

    /**
     * Indicate whether this interface is full duplex.
     * @param is_full Indication of whether the interface is full duplex.
     */
    public native void is_full_duplex(boolean is_full);
}
