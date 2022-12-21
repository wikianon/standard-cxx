/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Captive Metrics library for Captivity.
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

package D;

import D.PN;
import java.util.TreeMap;

/**
 * PT is the Process Tree class. It simply extends TreeMap with a String as a key type
 * where the key is the unique name of the process and a PN as the value type containing
 * the process information. The name of the process is made unique in the case of one
 * executable running more than once by appending "#digit" after the executable name where
 * "digit" is a natural number (1, 2, 3, ...).
 * @author Richard Pettit
 * @since JDK 1.5
 * @version 1.0
 */
public class PT extends TreeMap<String, D.PN>
{
    /**
     * An "or"ing of bits represented by the constants in P.java which indicate which members
     * within the PN class are supported on the server platform.
     */
    public long supportFlags;
}
