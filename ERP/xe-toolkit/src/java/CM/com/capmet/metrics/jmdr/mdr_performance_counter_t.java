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

import com.capmet.metrics.jmdr.mdr_exception_t;

/**
 * A generic interface to a performance metric, part of a
 * <CODE>mdr_category_t</CODE>, found on a <CODE>mdr_performance_host_t</CODE>.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mdr_performance_counter_t
{
    private native boolean perf_counter_constructor(
        String category,
        String field,
        String instance,
        String remote_host);

    private long handle;
    private String reason;

    /**
     * Constructor used to identify a singleton performance field on localhost.
     * @param category The name of the performance category.
     * @param field The name of the field.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_performance_counter_t(
        String category,
        String field) throws mdr_exception_t
    {
        if (!perf_counter_constructor(category, field, null, null))
            throw new mdr_exception_t(reason);
    }

    /**
     * Fully qualified constructor to identify a performance metric
     * on the local host.
     * @param category The name of the performance category.
     * @param field The name of the field.
     * @param instance The name of the instance if the category is an n-tuple.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_performance_counter_t(
        String category,
        String field,
        String instance) throws mdr_exception_t
    {
        if (!perf_counter_constructor(category, field, instance, null))
            throw new mdr_exception_t(reason);
    }

    /**
     * Fully qualified constructor to identify a performance metric
     * on a remote host.
     * @param category The name of the performance category.
     * @param field The name of the field.
     * @param instance The name of the instance if the category is an n-tuple.
     * @param remote_host The name of the remote host.
     * @throws mdr_exception_t Thrown from JNI code
     */
    public mdr_performance_counter_t(
        String category,
        String field,
        String instance,
        String remote_host) throws mdr_exception_t
    {
        if (!perf_counter_constructor(category, field, instance, remote_host))
            throw new mdr_exception_t(reason);
    }

    /**
     * Wrap things up in the JNI code.
     */
    protected native void finalize();

    private native Object perf_value();
    private native Object perf_next_value();
    private native Boolean is_supported();

    /**
     * Return indication of whether this field is supported.
     * @throws mdr_exception_t Thrown from JNI code
     * @return <CODE>true</CODE>: it is supported - <CODE>false</CODE>: it is not supported.
     */
    public boolean mdr_is_supported() throws mdr_exception_t
    {
        Boolean b = is_supported();

        if (b == null)
            throw new mdr_exception_t(reason);

        return b.booleanValue();
    }

    /**
     * Get the current value of the performance field.
     * @throws mdr_exception_t Thrown from JNI code
     * @return <CODE>Object</CODE> value of the performance field.
     */
    public Object mdr_value() throws mdr_exception_t
    {
        Object o = perf_value();

        if (o == null)
            throw new mdr_exception_t(reason);

        return o;
    }

    /**
     * Re-read the metric and return the new value of the performance field.
     * @throws mdr_exception_t Thrown from JNI code
     * @return <CODE>Object</CODE> value of the performance field.
     */
    public Object mdr_next_value() throws mdr_exception_t
    {
        Object o = perf_next_value();

        if (o == null)
            throw new mdr_exception_t(reason);

        return o;
    }

    static
    {
        System.loadLibrary("jmdr");
    }
}
