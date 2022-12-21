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

package com.capmet.metrics.rmi;

/**
 * Define the interface for a category that all category subobjects must implement.
 */
public interface CategorySource
{
    /**
     * Return the name of the category.
     * @return The <CODE>String</CODE> name of the category.
     */
    public String  getCategoryName();
    /**
     * Return the explanation of the category.
     * @return The <CODE>String</CODE> explanation of the category.
     */
    public String  getCategoryExplanation()
        throws PerformanceException;
    /**
     * Return indication of whether this category is supported.
     * @return <CODE>true:</CODE> the category is supported - <CODE>false:</CODE> the category is not supported.
     */
    public boolean isCategorySupported();
    /**
     * Return indication of whether the category is a singleton or an n-tuple.
     * @return <CODE>true:</CODE> category is a singleton - <CODE>false:</CODE> category is an n-tuple.
     */
    public boolean isSingleton();
    /**
     * Return the number of fields in this category.
     * @return An <CODE>int</CODE> value representing the cardinal number of fields present in the category.
     */
    public int     getFieldCount();
    /**
     * Return indication of whether the field in question is supported in this category.
     * @param fieldNumber The field number in question.
     * @return <CODE>true:</CODE> the field is supported - <CODE>false:</CODE> the field in not supported.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     */
    public boolean isFieldSupported(int fieldNumber)
        throws PerformanceException;
    /**
     * Return the name of the field given the field number.
     * @param fieldNumber The cardinal number of the field.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return The <CODE>String</CODE> name of the field.
     */
    public String  getFieldName(int fieldNumber)
        throws PerformanceException;
    /**
     * Return the explanation of the field given the field name.
     * @param fieldName The name of the field.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return The <CODE>String</CODE> explanation for the field.
     */
    public String  getFieldExplanation(String fieldName)
        throws PerformanceException;
    /**
     * Return the explanation of the field given the field number.
     * @param fieldNumber The cardinal number of the field.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return The <CODE>String</CODE> explanation for the field.
     */
    public String  getFieldExplanation(int fieldNumber)
        throws PerformanceException;
    /**
     * Return the field number given the field name.
     * @param fieldName The name of the field.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return The cardinal number of the field.
     */
    public int     getFieldNumber(String fieldName)
        throws PerformanceException;
    /**
     * Retrieve a <CODE>FieldSource</CODE> object from the singleton category for the given field number.
     * @param fieldNumber The cardinal number of the field.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return A <CODE>FieldSource</CODE> object for the given field number.
     * @see com.capmet.metrics.rmi.FieldSource
     */
    public FieldSource getField(int fieldNumber)
        throws PerformanceException;
    /**
     * Retrieve a <CODE>FieldSource</CODE> object from the singleton category for the given field name.
     * @param fieldName The <CODE>String</CODE> name of the field.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return A <CODE>FieldSource</CODE> object for the given field name.
     * @see com.capmet.metrics.rmi.FieldSource
     */
    public FieldSource getField(String fieldName)
        throws PerformanceException;
    /**
     * Retrieve a <CODE>FieldSource</CODE> object from the n-tuple category for the given
     * instance name and field number.
     * @param fieldNumber The cardinal field number.
     * @param instanceName The name of the instance in the specified n-tuple.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return A <CODE>FieldSource</CODE> object for the given instance name and field number.
     * @see com.capmet.metrics.rmi.FieldSource
     */
    public FieldSource getField(int fieldNumber, String instanceName)
        throws PerformanceException;
    /**
     * Retrieve a <CODE>FieldSource</CODE> object from the n-tuple category for the given
     * instance name and field name.
     * @param fieldName The <CODE>String</CODE> name of the field.
     * @param instanceName The instance name containing the field in question.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return A <CODE>FieldSource</CODE> object for the given instance name and field name.
     * @see com.capmet.metrics.rmi.FieldSource
     */
    public FieldSource getField(String fieldName, String instanceName)
        throws PerformanceException;
    /**
     * Retrieve the instance names of the n-tuple category.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return An array of <CODE>String</CODE> with the names of the instances of the n-tuple category.
     */
    public String[] getInstanceNames()
        throws PerformanceException;
    /**
     * Retrieve all field names of the n-tuple category.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return An array of <CODE>String</CODE> with the names of the fields of the category.
     */
    public String[] getAllFieldNames()
        throws PerformanceException;
    /**
     * Retrieve the supported field names of the n-tuple category.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     * @return An array of <CODE>String</CODE> with the names of the fields of the category.
     */
    public String[] getSupportedFieldNames()
        throws PerformanceException;
    /**
     * Refresh the entire performance category at once.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     */
    public void categoryRefresh()
        throws PerformanceException;
    /**
     * Refresh the entire performance category and
     * compute the differences from the previous refresh.
     * @throws com.capmet.metrics.rmi.PerformanceException Any possible lower level exception.
     */
    public void categoryRefreshRates()
        throws PerformanceException;
}
