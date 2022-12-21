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

package com.capmet.metrics;

import com.capmet.metrics.rmi.RMIMessages;

/**
 * An enumeration defining the operating system platforms supported.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public enum CmOSFlavor
{
    // enum values called out for consistency between Java and C++
    //

    /**
     * The OS is Linux.
     */
    MOS_LINUX   (1),
    /**
     * The OS is Solaris.
     */
    MOS_SOLARIS (2),
    /**
     * The OS is AIX.
     */
    MOS_AIX     (4),
    /**
     * The OS is HP-UX.
     */
    MOS_HPUX    (8),
    /**
     * The OS is MacOS X.
     */
    MOS_MACOS   (16),
    /**
     * The OS is a BSD derivative.
     */
    MOS_BSD     (32),
    /**
     * The OS is Windows.
     */
    MOS_WINDOWS (64),
    /**
     * The OS is something else (for use by toOSFlavor()).
     */
    MOS_UNKNOWN (128);

    /**
     * The integer value of the enumeration which matches the underlying C++ code.
     */
    private final int value;

    /**
     * The constructor for giving the enumerated specific values.
     * @param value The integer value of the enumerated value.
     */
    private CmOSFlavor(int value)
    {
        this.value = value;
    }

    /**
     * Convert an integer value to the <CODE>CmOSFlavor</CODE> equivalent.
     * @param value The integer value.
     * @return The <CODE>CmOSFlavor</CODE> value representing the integer value or MOS_UNKNOWN if not onto.
     */
    public static CmOSFlavor toOSFlavor(int value)
    {
        switch(value) {
        case 1:
            return MOS_LINUX;
        case 2:
            return MOS_SOLARIS;
        case 4:
            return MOS_AIX;
        case 8:
            return MOS_HPUX;
        case 16:
            return MOS_MACOS;
        case 32:
            return MOS_BSD;
        case 64:
            return MOS_WINDOWS;
        default:
            return MOS_UNKNOWN;
        }
    }

    /**
     * Return a pleasant <CODE>String</CODE> value of the enumeration value.
     * @return A <CODE>String</CODE> such as "Linux" or "Solaris".
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
        case 128:
            return RMIMessages.msgs.getString("unknown");
        default:
            return RMIMessages.msgs.getString("unknown") + ": " + value;
        }
    }

    /**
     * Return an indication of whether the enumeration value is a Unix variant or not.
     * @return <CODE>true:</CODE> the value is a Unix variant - <CODE>false:</CODE> the value is not a Unix variant.
     */
    public boolean isUnixHost()
    {
        return (value < 64);
    }

    /**
     * Return an indication of whether the enumeration value is a Windows variant or not.
     * @return <CODE>true:</CODE> the value is a Windows variant - <CODE>false:</CODE> the value is not a Windows variant.
     */
    public boolean isWindowsHost()
    {
        return (value == 64);
    }
}
