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

package com.capmet.metrics.cm;

/**
 * A sort class implementing a very simple sort algorithm.
 * Provides for forward and reverse sorting of an array of
 * any type that extends Comparable.
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */

public class Sort<T extends Comparable<T>>
{
    /**
     * A multiplier for the comparison. If a reverse sort
     * is required, polarity will be set to -1, otherwise 1.
     */
    private int polarity;

    /**
     * Simple sort. Find the highest, swap it with the last
     * array element, reduce the size of the array by one.
     * Stop when there is only one element left in the array.
     * @param collection The array to sort.
     */
    public void sort(T[] collection)
    {
        int i;
        int n = collection.length;
        int highest;

        // while there's more than one element to sort
        while(n > 1) {
            // start with the first element as highest
            highest = 0;
            
            // compare first element with second through last
            for(i=1; i<n; i++) {
                int result = collection[i].compareTo(collection[highest]) * polarity;

                // track the highest element
                if (result > 0)
                    highest = i;
            }
            
            // if the highest is not the last, swap the two.
            if (highest != (n - 1)) {
                T tmp = collection[n-1];
                collection[n-1] = collection[highest];
                collection[highest] = tmp;
            }

            // decrease the size of the array by one
            n--;
        }
    }

    /**
     * A reverse sort. <CODE>polarity</CODE> is set to -1 and <CODE>sort</CODE> is called.
     * @param collection The array to sort.
     */
    public void rsort(T[] collection)
    {
        polarity = -1;
        sort(collection);
        polarity = 1;
    }

    /**
     * Default constructor. Forward sort is default.
     */
    public Sort()
    {
        polarity = 1;
    }
}
