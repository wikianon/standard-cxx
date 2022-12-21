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
 * mci_can_t is the unit container for the mdr_cpuinfo_t class
 * which is a mapping of CPU name to mci_can_t.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mci_can_t
{
    /**
     * The CPU instance number as assigned by the kernel.
     */
    public int    mci_instance;      // CPU Instance Number
    /**
     * CPU architecture name.
     */
    public String mci_architecture;  // CPU Architecture
    /**
     * CPU model description.
     */
    public String mci_model;         // CPU Model Name
    /**
     * CPU clock speed in MHz.
     */
    public int    mci_clock_MHz;     // CPU Speed in Megahertz

    /**
     * The default constructor for mci_can_t.
     */
    public mci_can_t()
    {
        clear();
    }

    /**
     * Set all public members to their initial value.
     */
    public void clear()
    {
        mci_instance = 0;
        mci_architecture = null;
        mci_model = null;
        mci_clock_MHz = 0;
    }
}
