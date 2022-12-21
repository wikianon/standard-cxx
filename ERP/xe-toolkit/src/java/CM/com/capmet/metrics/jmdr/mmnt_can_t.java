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
 * mmnt_can_t is the unit container for the mdr_mnt_t class
 * which is a mapping of mount point to mmnt_can_t.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */

public class mmnt_can_t
{
    /**
     * Special File
     */
    public String mmnt_special;
    /**
     * Mount Point
     */
    public String mmnt_mount_point;
    /**
     * File System Type
     */
    public String mmnt_fs_type;
    /**
     * Mount Options
     */
    public String mmnt_mount_options;
    /**
     * Mount Time
     */
    public long   mmnt_mount_time;
    /**
     * KBytes Space Total
     */
    public long   mmnt_size_K;
    /**
     * KBytes Space Used
     */
    public long   mmnt_used_K;
    /**
     * KBytes Space Free
     */
    public long   mmnt_free_K;
    /**
     * Percent Used
     */
    public double mmnt_percent_used;
    /**
     * Inode Count
     */
    public long   mmnt_files;
    /**
     * Free Inode Count
     */
    public long   mmnt_free_files;
    /**
     * Major Number
     */
    public int    mmnt_major;
    /**
     * Minor Number
     */
    public int    mmnt_minor;
    /**
     * File System ID
     */
    public long   mmnt_file_system_id;

    /**
     * Set all public members to their initial value.
     */
    public void clear()
    {
        mmnt_special = null;
        mmnt_mount_point = null;
        mmnt_fs_type = null;
        mmnt_mount_options = null;
        mmnt_mount_time = 0;
        mmnt_size_K = 0;
        mmnt_used_K = 0;
        mmnt_free_K = 0;
        mmnt_percent_used = 0.0;
        mmnt_files = 0;
        mmnt_free_files = 0;
        mmnt_major = 0;
        mmnt_minor = 0;
        mmnt_file_system_id = 0;
    }

    /**
     * The default constructor.
     */
    public mmnt_can_t()
    {
        clear();
    }
}
