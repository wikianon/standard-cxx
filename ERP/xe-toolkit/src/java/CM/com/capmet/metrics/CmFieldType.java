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

package com.capmet.metrics;

/**
 * An enumeration of the possible data types of collected metrics.
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public enum CmFieldType
{
    /**
     * The metric is an <CODE>Integer</CODE>.
     */
    PF_INT, 
    /**
     * The metric is a <CODE>Long</CODE>.
     */
    PF_LONG, 
    /**
     * The metric is a <CODE>Double</CODE>.
     */
    PF_DOUBLE, 
    /**
     * The metric is a <CODE>String</CODE>.
     */
    PF_STRING, 
    /**
     * The metric is a <CODE>Boolean</CODE>.
     */
    PF_BOOLEAN
}
