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
import com.capmet.metrics.rmi.CategoryClone;
import com.capmet.metrics.rmi.CategorySource;
import com.capmet.metrics.rmi.PerformanceException;

/**
 * The category source for the local computer.
 */
public class LocalCategory extends CategoryClone
{
    // A category and its timestamp.
    private class CategoryStuff
    {
        public CategorySource category; // the object
        public long           time;     // the last time refresh was called

        public CategoryStuff(CategorySource c, long t)
        {
            category = c;
            time = t;
        }
    }

    // private members in support of the class
    private static LocalHost                      hostObj;
    private static TreeMap<String, CategoryStuff> categoryMap;

    // static initialization of static members.
    static
    {
        try
        {
            // Must insure that the static block in LocalHost is called
            // to gather all the plugins before going on.
            hostObj = new LocalHost();
            categoryMap = new TreeMap<String, CategoryStuff>();
        }
        catch(Exception e)
        {
            throw new RuntimeException(e.getMessage());
        }
    }

    /**
     * The constructor.
     * @param name The name of the category.
     * @throws PerformanceException Any lower level exception.
     */
    public LocalCategory(String name)
        throws PerformanceException
    {
        super(name);

        try
        {
            synchronized(categoryMap)
            {
                // see if the category is already in the cat map
                if (categoryMap.containsKey(name))
                {
                    categoryObj = categoryMap.get(name).category;
                    categoryRefresh();
                }
                else
                {
                    // Negative. Make a new one and put it in.
                    categoryObj = hostObj.getCategory(name);
                    categoryMap.put(name, new CategoryStuff(categoryObj, 0));
                    categoryObj.categoryRefreshRates();
                }
            }
        }
        catch(Exception e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    // check to see if enough time has elapsed before refreshing again.
    private boolean enoughTimeHasElapsed()
    {
        CategoryStuff cs = categoryMap.get(categoryName);
        long now = System.currentTimeMillis();
        String threshEnv = System.getenv("MDR_INTERVAL_THRESHOLD");
        long threshold = 1000;

        if (threshEnv != null)
            threshold = Long.parseLong(threshEnv);

        if ((now - cs.time) >= threshold)
        {
            cs.time = now;
            return true;
        }

        return false;
    }

    /**
     * Refresh the entire category.
     * @throws PerformanceException Any lower level exception.
     */
    public void categoryRefresh()
        throws PerformanceException
    {
        if (enoughTimeHasElapsed())
            categoryObj.categoryRefresh();
    }

    /**
     * Refresh the entire category and calculate rates to boot.
     * @throws PerformanceException Any lower level exception.
     */
    public void categoryRefreshRates()
        throws PerformanceException
    {
        if (enoughTimeHasElapsed())
            categoryObj.categoryRefreshRates();
    }
}
