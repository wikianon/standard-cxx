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

package com.capmet.metrics.rmi;

import java.util.Map;
import java.util.Hashtable;
import java.rmi.server.UID;
import com.capmet.metrics.rmi.RMIMethods;

/**
 * A static class for collecting unique ids of remote objects so the
 * collector can be told to delete them when the client process exits.
 */
public class RemoteJanitor
{
    /**
     * The static data structure holding the hash code to RMI access point mappings.
     */
    private static class ObjectMap extends Hashtable<UID, RMIMethods> {}

    /**
     * A map of hash code to Remote access point used by the shutdown thread to instruct the RMI server
     * to release memory representing the remote copy of the performance metric.
     */
    private static ObjectMap objectMap;

    static
    {
        objectMap = new ObjectMap();

        try
        {
            // config shutdown hook to remote-delete performance field objects
            Runtime.getRuntime().addShutdownHook(new Thread() {
                public void run() {
                    try
                    {
                        for(Map.Entry<UID, RMIMethods> e : objectMap.entrySet())
                        {
                            UID objectId = e.getKey();
                            RMIMethods r = e.getValue();

                            r.deleteObject(objectId);
                        }
                    }
                    catch(Exception e)
                    {
                        ; // tough luck, I guess
                    }
                }
            });
        }
        catch(Exception e)
        {
            // No shutdown hooks allowed in untrusted applets.
        }
    }

    /**
     * Add an object to the collection of items to be deleted at exit.
     * @param key The unique id of the object to be deleted.
     * @param value The connection to the remote host.
     */
    public static void put(UID key, RMIMethods value)
    {
        synchronized(objectMap) {
            objectMap.put(key, value);
        }
    }
}
