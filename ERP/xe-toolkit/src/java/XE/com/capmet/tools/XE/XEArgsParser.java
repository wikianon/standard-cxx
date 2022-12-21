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

import java.util.Hashtable;

/**
 * Build a <CODE>Hashtable</CODE> containing keys and values
 * from one or many comma separated list of key=value strings.
 *
 * <PRE>
 *
 * someFunction("orientation=vertical,maxSize=1024", "isEnabled=true");
 *
 * public void someFunction(Object ... args)
 * {
 *   XEArgsParser ap = new XEArgsParser(args);
 *
 *   for(Map.Entry&lt;String, String&gt; e : ap.entrySet()) {
 *     String key = e.getKey();
 *     String value = e.getValue();
 *
 *     if (key.equals("orientation")) {
 *       if (value.equals("vertical"))
 *     ...
 * </PRE>
 */
public class XEArgsParser extends Hashtable<String, String>
{
    /**
     * Constructor for the class. Pass in a <CODE>varargs</CODE>
     * style argument array which is tokenized and placed into the
     * <CODE>Hashtable</CODE> as key=value pairs.
     * @param args The argument array.
     */
    public XEArgsParser(Object[] args)
    {
        if (args == null || args.length == 0)
            return;

        for(int i=0; i<args.length; i++) {
            for(String keyVal : ((String) args[i]).split(",")) {
                int eq = keyVal.indexOf('=');

                if (eq == -1)
                    continue;

                String key = keyVal.substring(0, eq);
                String value = keyVal.substring(eq+1, keyVal.length());

                put(key, value);
            }
        }
    }

    /**
     * Constructor for the class. Pass in a <CODE>String</CODE>
     * argument which is placed into an array of <CODE>Object</CODE>
     * and passed on to the appropriate constructor.
     * @param argString The "key=value{,key=value}" string.
     */
    public XEArgsParser(String argString)
    {
        this(new Object[] { argString });
    }
}
