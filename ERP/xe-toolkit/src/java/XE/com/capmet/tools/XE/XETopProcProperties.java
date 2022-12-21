/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the XE library for The XE Toolkit.
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

package com.capmet.tools.XE;

import java.util.TreeSet;
import java.io.Serializable;

/**
 * The properties of an <CODE>XETopProcesses</CODE> object
 * that can be saved and restored to the object later.
 */
public class XETopProcProperties implements Serializable {
    /**
     * The number of processes visible in the viewport.
     */
    public int           visibleProcs;
    /**
     * The list of process ids that have been highlighted.
     */
    public TreeSet<Long> selectedPids;
    /**
     * The update interval.
     */
    public int           intervalSeconds;
    /**
     * The field that the processes are sorted on.
     */
    public int           sortKey;
    /**
     * Whether the sort is ascending or descending.
     */
    public boolean       reverseSort;
    /**
     * Which fields are visible; array index is the field number.
     */
    public boolean[]     visibleFields;
    /**
     * The widths of the fields; array index is the field number.
     */
    public int[]         fieldWidths;

    /**
     * Default constructor.
     */
    public XETopProcProperties() {
        visibleProcs = 10;
        selectedPids = null;
        intervalSeconds = 5;
        sortKey = D.PN.PN_PERCENT_CPU_USE;
        reverseSort = true;
        visibleFields = null;
        fieldWidths = null;
    }
}
