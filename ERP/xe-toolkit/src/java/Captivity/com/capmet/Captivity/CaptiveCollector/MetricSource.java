/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector application of Captivity.
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

package com.capmet.Captivity.CaptiveCollector;

import java.util.TreeMap;
import com.capmet.metrics.rmi.CategorySource;
import com.capmet.metrics.rmi.PerformanceException;

/**
 * The template definition of an object that provides performance metrics.
 */
public interface MetricSource
{
    /**
     * Retrieve a <CODE>CategorySource</CODE> object for the
     * specified category.
     * @param categoryName The name of the category in question.
     * @throws PerformanceException Any lower level exception.
     * @return A <CODE>CategorySource</CODE> object.
     */
    public CategorySource getCategory(String categoryName)
        throws PerformanceException;
    /**
     * Get the names of the categories available on the host.
     * @throws PerformanceException Any lower level exception.
     * @return An array of <CODE>String</CODE> containing the names of
     * the performance categories available.
     */
    public String[] getCategoryNames() throws PerformanceException;

    /**
     * Deliver to the object the mappings of en_us to localized
     * strings and vice-versa.
     * @param localToEnUsMap A mapping of localized strings to en_us strings.
     * @param enUsToLocalMap A mapping of en_us strings to localized strings.
     */
    public void setNamesMaps(
        TreeMap<String, String> localToEnUsMap,
        TreeMap<String, String> enUsToLocalMap);
}
