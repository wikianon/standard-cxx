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
 * An abstract implementation of CategorySource that returns values from an
 * underlying CategorySource object. Methods needing more detail can be
 * overridden by the subclass.
 */
public abstract class CategoryClone implements CategorySource
{
    /**
     * The underlying category object given a value by the subclass.
     */
    protected CategorySource categoryObj;

    /**
     * The name of the category.
     */
    protected String         categoryName;

    /**
     * Constructor for the class.
     * @param name The name of the category.
     * @throws PerformanceException Any possible lower level exception.
     */
    public CategoryClone(String name)
        throws PerformanceException
    {
        categoryName = name;
        categoryObj = null;
    }

    /**
     * Return the name of the category.
     * @return The <CODE>String</CODE> name of the category.
     */
    public String getCategoryName()
    {
        return categoryName;
    }

    /**
     * Return the explanation of the category.
     * @throws PerformanceException Any possible lower level exception.
     * @return The <CODE>String</CODE> explanation of the category.
     */
    public String getCategoryExplanation()
        throws PerformanceException
    {
        return categoryObj.getCategoryExplanation();
    }

    /**
     * Return indication of whether this category is supported.
     * @return <CODE>true:</CODE> the category is supported - <CODE>false:</CODE> the category is not supported.
     */
    public boolean isCategorySupported()
    {
        return categoryObj.isCategorySupported();
    }

    /**
     * Return indication of whether the category is a singleton or an n-tuple.
     * @return <CODE>true:</CODE> category is a singleton - <CODE>false:</CODE> category is an n-tuple.
     */
    public boolean isSingleton()
    {
        return categoryObj.isSingleton();
    }

    /**
     * Return the number of fields in this category.
     * @return An <CODE>int</CODE> value representing the cardinal number of fields present in the category.
     */
    public int getFieldCount()
    {
        return categoryObj.getFieldCount();
    }

    /**
     * Return indication of whether the field in question is supported in this category.
     * @param fieldNumber The field number in question.
     * @return <CODE>true:</CODE> the field is supported - <CODE>false:</CODE> the field in not supported.
     * @throws PerformanceException Any possible lower level exception.
     */
    public boolean isFieldSupported(int fieldNumber)
        throws PerformanceException
    {
        return categoryObj.isFieldSupported(fieldNumber);
    }

    /**
     * Return the name of the field given the field number.
     * @param fieldNumber The cardinal number of the field.
     * @throws PerformanceException Any possible lower level exception.
     * @return The <CODE>String</CODE> name of the field.
     */
    public String getFieldName(int fieldNumber)
        throws PerformanceException
    {
        return categoryObj.getFieldName(fieldNumber);
    }

    /**
     * Return the explanation of the field given the field name.
     * @param fieldName The name of the field.
     * @throws PerformanceException Any possible lower level exception.
     * @return The <CODE>String</CODE> explanation for the field.
     */
    public String getFieldExplanation(String fieldName)
        throws PerformanceException
    {
        return getFieldExplanation(getFieldNumber(fieldName));
    }

    /**
     * Return the explanation of the field given the field number.
     * @param fieldNumber The cardinal number of the field.
     * @throws PerformanceException Any possible lower level exception.
     * @return The <CODE>String</CODE> explanation for the field.
     */
    public String getFieldExplanation(int fieldNumber)
        throws PerformanceException
    {
        return categoryObj.getFieldExplanation(fieldNumber);
    }

    /**
     * Return the field number given the field name.
     * @param fieldName The name of the field.
     * @throws PerformanceException Any possible lower level exception.
     * @return The cardinal number of the field.
     */
    public int getFieldNumber(String fieldName)
        throws PerformanceException
    {
        return categoryObj.getFieldNumber(fieldName);
    }

    /**
     * Retrieve a <CODE>FieldSource</CODE> object from the singleton category for the given field number.
     * @param fieldNumber The cardinal number of the field.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>FieldSource</CODE> object for the given field number.
     * @see com.capmet.metrics.rmi.FieldSource
     */
    public FieldSource getField(int fieldNumber)
        throws PerformanceException
    {
        return getField(getFieldName(fieldNumber));
    }

    /**
     * Retrieve a <CODE>FieldSource</CODE> object from the singleton category for the given field name.
     * @param fieldName The <CODE>String</CODE> name of the field.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>FieldSource</CODE> object for the given field name.
     * @see com.capmet.metrics.rmi.FieldSource
     */
    public FieldSource getField(String fieldName)
        throws PerformanceException
    {
        return categoryObj.getField(fieldName);
    }

    /**
     * Retrieve a <CODE>FieldSource</CODE> object from the n-tuple category for the given
     * instance name and field number.
     * @param fieldNumber The cardinal field number.
     * @param instanceName The name of the instance in the specified n-tuple.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>FieldSource</CODE> object for the given instance name and field number.
     * @see com.capmet.metrics.rmi.FieldSource
     */
    public FieldSource getField(int fieldNumber, String instanceName)
        throws PerformanceException
    {
        return getField(getFieldName(fieldNumber), instanceName);
    }

    /**
     * Retrieve a <CODE>FieldSource</CODE> object from the n-tuple category for the given
     * instance name and field name.
     * @param fieldName The <CODE>String</CODE> name of the field.
     * @param instanceName The instance name containing the field in question.
     * @throws PerformanceException Any possible lower level exception.
     * @return A <CODE>FieldSource</CODE> object for the given instance name and field name.
     * @see com.capmet.metrics.rmi.FieldSource
     */
    public FieldSource getField(String fieldName, String instanceName)
        throws PerformanceException
    {
        return categoryObj.getField(fieldName, instanceName);
    }

    /**
     * Retrieve the instance names of the n-tuple category.
     * @throws PerformanceException Any possible lower level exception.
     * @return An array of <CODE>String</CODE> with the names of the instances of the n-tuple category.
     */
    public String[] getInstanceNames()
        throws PerformanceException
    {
        return categoryObj.getInstanceNames();
    }

    /**
     * Retrieve all field names of the n-tuple category.
     * @throws PerformanceException Any possible lower level exception.
     * @return An array of <CODE>String</CODE> with the names of the fields of the category.
     */
    public String[] getAllFieldNames()
        throws PerformanceException
    {
        return categoryObj.getAllFieldNames();
    }

    /**
     * Retrieve the supported field names of the n-tuple category.
     * @throws PerformanceException Any possible lower level exception.
     * @return An array of <CODE>String</CODE> with the names of the fields of the category.
     */
    public String[] getSupportedFieldNames()
        throws PerformanceException
    {
        return categoryObj.getSupportedFieldNames();
    }

    /**
     * Refresh the entire performance category at once.
     * @throws PerformanceException Any possible lower level exception.
     */
    public void categoryRefresh()
        throws PerformanceException
    {
        categoryObj.categoryRefresh();
    }

    /**
     * Refresh the entire performance category and
     * compute the differences from the previous refresh.
     * @throws PerformanceException Any possible lower level exception.
     */
    public void categoryRefreshRates()
        throws PerformanceException
    {
        categoryObj.categoryRefreshRates();
    }
}
