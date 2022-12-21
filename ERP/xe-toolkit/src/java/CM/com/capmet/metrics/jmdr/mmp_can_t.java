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
 * mmp_can_t is the unit container for the mdr_mp_t class
 * which is a mapping of cpu name to mmp_can_t.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */

public class mmp_can_t
{
    /**
     * CPU Instance Number
     */
    public int mmp_cpu_instance;
    /**
     * User CPU Ticks
     */
    public int mmp_user_ticks;
    /**
     * Kernel CPU Ticks
     */
    public int mmp_kernel_ticks;
    /**
     * Idle CPU Ticks
     */
    public int mmp_idle_ticks;
    /**
     * Nice CPU Ticks
     */
    public int mmp_nice_ticks;
    /**
     * Wait CPU Ticks
     */
    public int mmp_wait_ticks;
    /**
     * Hardware Interrupt Ticks
     */
    public int mmp_irq_ticks;
    /**
     * Software Interrupt Ticks
     */
    public int mmp_soft_irq_ticks;

    /**
     * Set all public members to their initial value.
     */
    public void clear()
    {
        mmp_cpu_instance = 0;
        mmp_user_ticks = 0;
        mmp_kernel_ticks = 0;
        mmp_idle_ticks = 0;
        mmp_nice_ticks = 0;
        mmp_wait_ticks = 0;
        mmp_irq_ticks = 0;
        mmp_soft_irq_ticks = 0;
    }

    /**
     * The default constructor.
     */
    public mmp_can_t()
    {
        clear();
    }
}
