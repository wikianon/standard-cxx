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

/**
 * An enumeration defining the operating system platforms supported.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public enum mdr_os_flavor_t
{
    // enum values called out for consistency between Java and C++
    //

    /**
     * The OS is Linux.
     */
    MOS_LINUX            (1),
    /**
     * The OS is Solaris.
     */
    MOS_SOLARIS          (2),
    /**
     * The OS is AIX.
     */
    MOS_AIX              (4),
    /**
     * The OS is HP-UX.
     */
    MOS_HPUX             (8),
    /**
     * The OS is MacOS X.
     */
    MOS_MACOS            (16),
    /**
     * The OS is a BSD derivative.
     */
    MOS_BSD              (32),
    /**
     * The OS is Windows.
     */
    MOS_WINDOWS          (64);

    private final int value;

    private mdr_os_flavor_t(int value)
    {
        this.value = value;
    }

    /**
     * Convert the enum to a <CODE>String</CODE>.
     * @return The <CODE>String</CODE> representation of the enumeration.
     */
    public String toString()
    {
        switch(value) {
        case 1:
            return "Linux";
        case 2:
            return "Solaris";
        case 4:
            return "AIX";
        case 8:
            return "HP-UX";
        case 16:
            return "MacOS";
        case 32:
            return "BSD";
        case 64:
            return "Windows";
        default:
            return mdr_t.msgs.getString("unknown") + ": " + value;
        }
    }
}
