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
 * mc_can_t is the unit container for the mdr_cache_t class
 * which is a mapping of cache name to mc_can_t.
 * <p>
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class mc_can_t
{
    /**
     * The number of lookups that matched an existing item in the cache.
     */
    public int    mc_hits;         // Cache Hits
    /**
     * The number of lookups that did not match an existing item in the cache.
     */
    public int    mc_misses;       // Cache Misses
    /**
     * The percentage of lookups that resulted in a cache hit.
     */
    public double mc_hit_percent;  // Hit Percentage

    /**
     * Set all public members to their initial value.
     */
    public void clear()
    {
        mc_hits = 0;
        mc_misses = 0;
        mc_hit_percent = 0.0;
    }

    /**
     * The default constructor for mc_can_t.
     */
    public mc_can_t()
    {
        clear();
    }
}
