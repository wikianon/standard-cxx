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
 * mcpu_can_t is the unit container for the mdr_cpu_t class
 * which is a mapping of CPU name to mcpu_can_t.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mcpu_can_t
{
    /**
     * Number of clock ticks accumulated in non-nice user space.
     */
    public int mcpu_user_ticks;          // User CPU Ticks
    /**
     * Number of clock ticks accumulated in kernel space.
     */
    public int mcpu_kernel_ticks;        // Kernel CPU Ticks
    /**
     * Number of clock ticks accumulated with requests pending in iowait.
     */
    public int mcpu_wait_ticks;          // Wait CPU Ticks
    /**
     * Number of clock ticks accumulated in nice user space.
     */
    public int mcpu_nice_ticks;          // Nice CPU Ticks
    /**
     * Number of clock ticks accumulated by the idle task.
     */
    public int mcpu_idle_ticks;          // Idle CPU Ticks
    /**
     * Number of clock ticks accumulated by IRQ handlers.
     */
    public int mcpu_irq_ticks;           // Hardware Interrupt Ticks
    /**
     * Number of clock ticks accumulated by software interrupt handlers.
     */
    public int mcpu_soft_irq_ticks;      // Software Interrupt Ticks

    /**
     * The default constructor for mcpu_can_t.
     */
    public mcpu_can_t()
    {
        clear();
    }

    /**
     * Set all public members to their initial value.
     */
    public void clear()
    {
        mcpu_user_ticks = 0;
        mcpu_kernel_ticks = 0;
        mcpu_wait_ticks = 0;
        mcpu_nice_ticks = 0;
        mcpu_idle_ticks = 0;
        mcpu_irq_ticks = 0;
        mcpu_soft_irq_ticks = 0;
    }
}
