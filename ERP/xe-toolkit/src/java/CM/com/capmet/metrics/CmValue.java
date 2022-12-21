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

import com.capmet.metrics.CmFieldType;

/**
 * The base class of the Cm types containing the basic notion of support.
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public abstract class CmValue {
    /**
     * The support indication.
     */
    protected boolean supported;

    /**
     * The default constructor indicating non-support.
     */
    public CmValue()
    {
        this.supported = false;
    }

    /**
     * The constructor with indication of support.
     * @param supported true or false indication of support.
     */
    public CmValue(boolean supported) {
        this.supported = supported;
    }

    /**
     * Ascertain whether value is supported.
     * @return true - value is supported; false - it is not.
     */
    public boolean isSupported() {
        return supported;
    }

    /**
     * Retrieve field type as indicated by <CODE>CmFieldType</CODE>.
     * @return The <CODE>CmFieldType</CODE> representation of type.
     */
    public abstract CmFieldType getType();

    /**
     * Generate a <CODE>String</CODE> representation of the value.
     * @return The <CODE>String</CODE> representation of the value.
     */
    public abstract String toString();
}
