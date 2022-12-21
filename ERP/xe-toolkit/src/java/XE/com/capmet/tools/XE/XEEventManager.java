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

import java.util.TreeMap;
import java.util.Vector;

/**
 * A class that stores event names and the event handlers that are interested
 * in those events.
 */
public class XEEventManager implements XEEventInterface
{
    // private data
    private class HandlerVector extends Vector<XEEventHandler> {}
    private class EventSink extends TreeMap<String, HandlerVector> {}
    private EventSink eventSink;

    /**
     * Default constructor. Allocate the <CODE>EventSink</CODE>.
     */
    public XEEventManager()
    {
        eventSink = new EventSink();
    }

    /**
     * Add an event handler for a specific event name.
     * @param eventName The name of the event.
     * @param eh The event handler for this event.
     */
    public void addEventHandler(String eventName, XEEventHandler eh)
    {
        if (!eventSink.containsKey(eventName)) {
            HandlerVector v = new HandlerVector();
            v.add(eh);
            eventSink.put(eventName, v);
        } else {
            HandlerVector v = eventSink.get(eventName);
            v.add(eh);
        }
    }

    /**
     * Cause the event handlers for this event to be called.
     * @param eventName The name of the event.
     * @param source The object that caused the event.
     */
    public void fireEvent(String eventName, Object source)
    {
        if (!eventSink.containsKey(eventName))
            return;

        HandlerVector v = eventSink.get(eventName);
        for(XEEventHandler h : v)
            h.handleEvent(eventName, source);
    }
}
