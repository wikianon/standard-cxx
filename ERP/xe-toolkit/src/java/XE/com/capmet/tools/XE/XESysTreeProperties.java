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

import java.io.Serializable;

/**
 * The properties of an <CODE>XESystemTree</CODE> object
 * that can be saved and restored to the object later.
 */
public class XESysTreeProperties implements Serializable {
    /**
     * The update interval.
     */
    public int       intervalSeconds;
    /**
     * An indication of which nodes in the tree have been expanded.
     */
    public boolean[] rowExpanded;

    /**
     * Default constructor.
     */
    public XESysTreeProperties() {
        intervalSeconds = 30;
        rowExpanded = null;
    }
}
