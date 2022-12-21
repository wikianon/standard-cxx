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

import java.io.CharArrayWriter;
import java.io.PrintWriter;

/**
 * A convenience class for determining the cause of the thrown
 * exception and converting it to a readable text message. If
 * it is not a message worth parsing, the stack trace is generated.
 */
public class CmExceptionParser
{
    // the constructed message
    private String message;

    /**
     * The lone constructor.
     * @param e The thrown exception for parsing.
     */
    public CmExceptionParser(Exception e)
    {
        message = null;

        if (e instanceof java.rmi.NotBoundException)
            message = CmMessages.msgs.getString("no_CC");
        else if (e instanceof java.net.MalformedURLException)
            message = CmMessages.msgs.getString("bad_URL");
        else if (e instanceof java.rmi.UnknownHostException)
            message = CmMessages.msgs.getString("unknown_host");
        else if (e instanceof java.awt.HeadlessException)
            message = CmMessages.msgs.getString("No_Display");
        else if (e instanceof java.rmi.RemoteException)
        {
            Throwable cause = e.getCause();

            if (cause == null)
                message = "RMI " + CmMessages.msgs.getString("failure") +
                            ": " + e.getMessage();
            else if (cause instanceof java.net.ConnectException)
                message = CmMessages.msgs.getString("no_rmiregistry");
            else if (cause instanceof java.net.NoRouteToHostException)
                message = CmMessages.msgs.getString("host_unreachable");
        }

        if (message == null)
        {
            CharArrayWriter caw = new CharArrayWriter();
            PrintWriter out = new PrintWriter(caw);
            e.printStackTrace(out);
            message = caw.toString();
        }
    }

    /**
     * Return the generated text message of the exception.
     * @return A nice message or a stack trace listing.
     */
    public String getMessage()
    {
        return message;
    }
}
