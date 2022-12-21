/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector plugin for Solaris.
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

package com.capmet.plugins.CaptiveCollector.Solaris;

/**
 * The mdr_sun_cpu_t class provides data about the performance of the CPUs
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
 * @date July 2005
 * @version 1.0
 */

import java.util.TreeMap;
import com.capmet.metrics.jmdr.mdr_t;
import com.capmet.metrics.jmdr.mdr_tuple_t;
import com.capmet.metrics.jmdr.mdr_string_t;
import com.capmet.metrics.jmdr.mdr_exception_t;
import com.capmet.plugins.CaptiveCollector.Solaris.msc_can_t;

public class mdr_sun_cpu_t extends mdr_t implements mdr_tuple_t
{
    // fields by the number
    public static final int MSC_CPU_INSTANCE   = 0;
    public static final int MSC_BREAD          = 1;
    public static final int MSC_BWRITE         = 2;
    public static final int MSC_LREAD          = 3;
    public static final int MSC_LWRITE         = 4;
    public static final int MSC_PHREAD         = 5;
    public static final int MSC_PHWRITE        = 6;
    public static final int MSC_PSWITCH        = 7;
    public static final int MSC_TRAP           = 8;
    public static final int MSC_INTR           = 9;
    public static final int MSC_SYSCALL        = 10;
    public static final int MSC_SYSREAD        = 11;
    public static final int MSC_SYSWRITE       = 12;
    public static final int MSC_SYSFORK        = 13;
    public static final int MSC_SYSVFORK       = 14;
    public static final int MSC_SYSEXEC        = 15;
    public static final int MSC_READCH         = 16;
    public static final int MSC_WRITECH        = 17;
    public static final int MSC_RAWCH          = 18;
    public static final int MSC_CANCH          = 19;
    public static final int MSC_OUTCH          = 20;
    public static final int MSC_MSG            = 21;
    public static final int MSC_SEMA           = 22;
    public static final int MSC_NAMEI          = 23;
    public static final int MSC_UFSIGET        = 24;
    public static final int MSC_UFSDIRBLK      = 25;
    public static final int MSC_UFSIPAGE       = 26;
    public static final int MSC_UFSINOPAGE     = 27;
    public static final int MSC_INODEOVF       = 28;
    public static final int MSC_FILEOVF        = 29;
    public static final int MSC_PROCOVF        = 30;
    public static final int MSC_INTRTHREAD     = 31;
    public static final int MSC_INTRBLK        = 32;
    public static final int MSC_IDLETHREAD     = 33;
    public static final int MSC_INV_SWTCH      = 34;
    public static final int MSC_NTHREADS       = 35;
    public static final int MSC_CPUMIGRATE     = 36;
    public static final int MSC_XCALLS         = 37;
    public static final int MSC_MUTEX_ADENTERS = 38;
    public static final int MSC_RW_RDFAILS     = 39;
    public static final int MSC_RW_WRFAILS     = 40;
    public static final int MSC_MODLOAD        = 41;
    public static final int MSC_MODUNLOAD      = 42;
    public static final int MSC_BAWRITE        = 43;
    public static final int MSC_IOWAIT         = 44;
    public static final int MSC_PGREC          = 45;
    public static final int MSC_PGFREC         = 46;
    public static final int MSC_PGIN           = 47;
    public static final int MSC_PGPGIN         = 48;
    public static final int MSC_PGOUT          = 49;
    public static final int MSC_PGPGOUT        = 50;
    public static final int MSC_SWAPIN         = 51;
    public static final int MSC_PGSWAPIN       = 52;
    public static final int MSC_SWAPOUT        = 53;
    public static final int MSC_PGSWAPOUT      = 54;
    public static final int MSC_ZFOD           = 55;
    public static final int MSC_DFREE          = 56;
    public static final int MSC_SCAN           = 57;
    public static final int MSC_REV            = 58;
    public static final int MSC_HAT_FAULT      = 59;
    public static final int MSC_AS_FAULT       = 60;
    public static final int MSC_MAJ_FAULT      = 61;
    public static final int MSC_COW_FAULT      = 62;
    public static final int MSC_PROT_FAULT     = 63;
    public static final int MSC_SOFTLOCK       = 64;
    public static final int MSC_KERNEL_ASFLT   = 65;
    public static final int MSC_PGRRUN         = 66;
    public static final int MSC_EXECPGIN       = 67;
    public static final int MSC_EXECPGOUT      = 68;
    public static final int MSC_EXECFREE       = 69;
    public static final int MSC_ANONPGIN       = 70;
    public static final int MSC_ANONPGOUT      = 71;
    public static final int MSC_ANONFREE       = 72;
    public static final int MSC_FSPGIN         = 73;
    public static final int MSC_FSPGOUT        = 74;
    public static final int MSC_FSFREE         = 75;
    public static final int MSC_FIELD_COUNT    = 76;

    /**
     * Call the constructor for the C++ class
     */
    private native boolean sun_cpu_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean sun_cpu_refresh();
    
    /**
     * Call the rates method in the C++ class
     */
    private native boolean sun_cpu_rates();
    
    /**
     * mdr_sun_cpu_t constructor.
     */
    public mdr_sun_cpu_t(
        TreeMap<String, String> localToEnUsMap,
        TreeMap<String, String> enUsToLocalMap)
            throws mdr_exception_t
    {
        if (!sun_cpu_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    protected native void finalize();
    
    /**
     * Update the variable with the latest values.
     */
    public void mdr_refresh() throws mdr_exception_t
    {
        if (!sun_cpu_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     */
    public void mdr_rates() throws mdr_exception_t
    {
        if (!sun_cpu_rates())
            throw new mdr_exception_t(reason);
    }
    
    // mdr_tuple_t methods
    public native int mdr_size();
    public native String[] mdr_keyset();
    
    // to be used in conjunction with mdr_keyset()
    public native boolean mdr_get(String key, msc_can_t can);

    // for exclusive use by mdr_field_value
    private native boolean mdr_get_field
        (String key, msc_can_t can, int field_number);

    public Object mdr_field_value(String key, int field_number)
        throws mdr_exception_t
    {
        msc_can_t can = new msc_can_t();

        if (!mdr_get_field(key, can, field_number))
            throw new mdr_exception_t("mdr_sun_cpu_t::mdr_field_value: " +
                msgs.getString("instance") + ": " + key + " " +
                msgs.getString("not_found"));

        switch(field_number)
        {
        case MSC_CPU_INSTANCE:
            return new Integer(can.msc_cpu_instance);
        case MSC_BREAD:
            return new Integer(can.msc_bread);
        case MSC_BWRITE:
            return new Integer(can.msc_bwrite);
        case MSC_LREAD:
            return new Integer(can.msc_lread);
        case MSC_LWRITE:
            return new Integer(can.msc_lwrite);
        case MSC_PHREAD:
            return new Integer(can.msc_phread);
        case MSC_PHWRITE:
            return new Integer(can.msc_phwrite);
        case MSC_PSWITCH:
            return new Integer(can.msc_pswitch);
        case MSC_TRAP:
            return new Integer(can.msc_trap);
        case MSC_INTR:
            return new Integer(can.msc_intr);
        case MSC_SYSCALL:
            return new Integer(can.msc_syscall);
        case MSC_SYSREAD:
            return new Integer(can.msc_sysread);
        case MSC_SYSWRITE:
            return new Integer(can.msc_syswrite);
        case MSC_SYSFORK:
            return new Integer(can.msc_sysfork);
        case MSC_SYSVFORK:
            return new Integer(can.msc_sysvfork);
        case MSC_SYSEXEC:
            return new Integer(can.msc_sysexec);
        case MSC_READCH:
            return new Integer(can.msc_readch);
        case MSC_WRITECH:
            return new Integer(can.msc_writech);
        case MSC_RAWCH:
            return new Integer(can.msc_rawch);
        case MSC_CANCH:
            return new Integer(can.msc_canch);
        case MSC_OUTCH:
            return new Integer(can.msc_outch);
        case MSC_MSG:
            return new Integer(can.msc_msg);
        case MSC_SEMA:
            return new Integer(can.msc_sema);
        case MSC_NAMEI:
            return new Integer(can.msc_namei);
        case MSC_UFSIGET:
            return new Integer(can.msc_ufsiget);
        case MSC_UFSDIRBLK:
            return new Integer(can.msc_ufsdirblk);
        case MSC_UFSIPAGE:
            return new Integer(can.msc_ufsipage);
        case MSC_UFSINOPAGE:
            return new Integer(can.msc_ufsinopage);
        case MSC_INODEOVF:
            return new Integer(can.msc_inodeovf);
        case MSC_FILEOVF:
            return new Integer(can.msc_fileovf);
        case MSC_PROCOVF:
            return new Integer(can.msc_procovf);
        case MSC_INTRTHREAD:
            return new Integer(can.msc_intrthread);
        case MSC_INTRBLK:
            return new Integer(can.msc_intrblk);
        case MSC_IDLETHREAD:
            return new Integer(can.msc_idlethread);
        case MSC_INV_SWTCH:
            return new Integer(can.msc_inv_swtch);
        case MSC_NTHREADS:
            return new Integer(can.msc_nthreads);
        case MSC_CPUMIGRATE:
            return new Integer(can.msc_cpumigrate);
        case MSC_XCALLS:
            return new Integer(can.msc_xcalls);
        case MSC_MUTEX_ADENTERS:
            return new Integer(can.msc_mutex_adenters);
        case MSC_RW_RDFAILS:
            return new Integer(can.msc_rw_rdfails);
        case MSC_RW_WRFAILS:
            return new Integer(can.msc_rw_wrfails);
        case MSC_MODLOAD:
            return new Integer(can.msc_modload);
        case MSC_MODUNLOAD:
            return new Integer(can.msc_modunload);
        case MSC_BAWRITE:
            return new Integer(can.msc_bawrite);
        case MSC_IOWAIT:
            return new Integer(can.msc_iowait);
        case MSC_PGREC:
            return new Integer(can.msc_pgrec);
        case MSC_PGFREC:
            return new Integer(can.msc_pgfrec);
        case MSC_PGIN:
            return new Integer(can.msc_pgin);
        case MSC_PGPGIN:
            return new Integer(can.msc_pgpgin);
        case MSC_PGOUT:
            return new Integer(can.msc_pgout);
        case MSC_PGPGOUT:
            return new Integer(can.msc_pgpgout);
        case MSC_SWAPIN:
            return new Integer(can.msc_swapin);
        case MSC_PGSWAPIN:
            return new Integer(can.msc_pgswapin);
        case MSC_SWAPOUT:
            return new Integer(can.msc_swapout);
        case MSC_PGSWAPOUT:
            return new Integer(can.msc_pgswapout);
        case MSC_ZFOD:
            return new Integer(can.msc_zfod);
        case MSC_DFREE:
            return new Integer(can.msc_dfree);
        case MSC_SCAN:
            return new Integer(can.msc_scan);
        case MSC_REV:
            return new Integer(can.msc_rev);
        case MSC_HAT_FAULT:
            return new Integer(can.msc_hat_fault);
        case MSC_AS_FAULT:
            return new Integer(can.msc_as_fault);
        case MSC_MAJ_FAULT:
            return new Integer(can.msc_maj_fault);
        case MSC_COW_FAULT:
            return new Integer(can.msc_cow_fault);
        case MSC_PROT_FAULT:
            return new Integer(can.msc_prot_fault);
        case MSC_SOFTLOCK:
            return new Integer(can.msc_softlock);
        case MSC_KERNEL_ASFLT:
            return new Integer(can.msc_kernel_asflt);
        case MSC_PGRRUN:
            return new Integer(can.msc_pgrrun);
        case MSC_EXECPGIN:
            return new Integer(can.msc_execpgin);
        case MSC_EXECPGOUT:
            return new Integer(can.msc_execpgout);
        case MSC_EXECFREE:
            return new Integer(can.msc_execfree);
        case MSC_ANONPGIN:
            return new Integer(can.msc_anonpgin);
        case MSC_ANONPGOUT:
            return new Integer(can.msc_anonpgout);
        case MSC_ANONFREE:
            return new Integer(can.msc_anonfree);
        case MSC_FSPGIN:
            return new Integer(can.msc_fspgin);
        case MSC_FSPGOUT:
            return new Integer(can.msc_fspgout);
        case MSC_FSFREE:
            return new Integer(can.msc_fsfree);
        default:
            throw new mdr_exception_t(
                "mdr_sun_cpu_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }
    
    static
    {
        System.loadLibrary("CCsolplugin");
    }
}
