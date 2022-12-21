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
import com.capmet.metrics.jmdr.mdr_exception_t;

/**
 * A class that counts the processes for what state they are in.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_proc_states_t extends mdr_t
{
    // fields by the number
    public static final int MPST_RUNNABLE    = 0;
    public static final int MPST_SLEEPING    = 1;
    public static final int MPST_STOPPED     = 2;
    public static final int MPST_BLOCKED     = 3;
    public static final int MPST_ZOMBIE      = 4;
    public static final int MPST_SPECIFIC    = 5;
    public static final int MPST_FIELD_COUNT = 6;

    /**
     * runnable processes
     */
    public int mpst_procs_runnable;
    /**
     * sleeping processes
     */
    public int mpst_procs_sleeping;
    /**
     * stopped processes
     */
    public int mpst_procs_stopped;
    /**
     * blocked processes
     */
    public int mpst_procs_blocked;
    /**
     * zombie processes
     */
    public int mpst_procs_zombie;
    /**
     * specific processes
     */
    public int mpst_procs_specific;

    /**
     * Call the constructor for the C++ class
     */
    private native boolean proc_states_constructor(String remote_host);
    
    /**
     * Call the refresh method in the C++ class
     */
    private native boolean proc_states_refresh();
    
    /**
     * mdr_proc_states_t constructor for localhost.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_proc_states_t() throws mdr_exception_t
    {
        if (!proc_states_constructor(null))
            throw new mdr_exception_t(reason);
    }
    
    /**
     * mdr_proc_states_t constructor for remote hosts.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_proc_states_t(String remote_host) throws mdr_exception_t
    {
        if (!proc_states_constructor(remote_host))
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
        if (!proc_states_refresh())
            throw new mdr_exception_t(reason);
    }
    
    /**
     * Compute rates from the current values and the previously stored values.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public void mdr_rates() throws mdr_exception_t
    {
        // no rates for this class
    }
    
    /**
     * Get the current value of the performance field.
     * @param field_number The numeric field ID of the field to retrieve.
     * @throws mdr_exception_t Thrown from JNI code
     * @return <CODE>Object</CODE> value of the performance field.
     */
    public Object mdr_field_value(int field_number) throws mdr_exception_t
    {
        switch(field_number)
        {
        case MPST_RUNNABLE:
            return new Integer(mpst_procs_runnable);
        case MPST_SLEEPING:
            return new Integer(mpst_procs_sleeping);
        case MPST_STOPPED:
            return new Integer(mpst_procs_stopped);
        case MPST_BLOCKED:
            return new Integer(mpst_procs_blocked);
        case MPST_ZOMBIE:
            return new Integer(mpst_procs_zombie);
        case MPST_SPECIFIC:
            return new Integer(mpst_procs_specific);
        default:
            throw new mdr_exception_t("mdr_proc_states_t::mdr_field_value: " +
                msgs.getString("field") + ": " + field_number + " " +
                msgs.getString("not_found"));
        }
    }
    
    static
    {
        System.loadLibrary("jmdr");
    }
}
