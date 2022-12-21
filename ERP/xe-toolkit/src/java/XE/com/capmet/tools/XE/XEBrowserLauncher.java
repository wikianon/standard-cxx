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

import java.net.URL;
import java.io.IOException;
import java.io.File;
import java.io.FileInputStream;
import java.util.Properties;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.cm.CmExceptionParser;

public class XEBrowserLauncher {
    private static Properties properties = null;
    private static String     browserCommand = null;

    static {
        String top = System.getProperty("com.capmet.CaptiveCollector.Top");

        if (top == null || top.length() == 0)
            throw new RuntimeException(XEMessages.msgs.getString("No_Top"));

        String browserDefs = top + File.separatorChar + "etc"
                          + File.separatorChar + "XE_Browser.properties";

        properties = new Properties();
        File browserFile = new File(browserDefs);
        if (browserFile.exists()) {
            try {
                properties.load(new FileInputStream(browserFile));
            } catch(IOException e) {
                // oh well
            }
        }

        // cannot use PerformanceHost(); may be a client-only install
        String osName = System.getProperties().getProperty("os.name");
        String osFlavor = "Unknown";

        if (osName.equals("SunOS"))
            osFlavor = "Solaris";
        else if (osName.equals("Linux"))
            osFlavor = "Linux";
        else if (osName.equals("FreeBSD"))
            osFlavor = "FreeBSD";
        else if (osName.startsWith("Windows"))
            osFlavor = "Windows";
        else if (osName.startsWith("Mac"))
            osFlavor = "MacOS";

        String browserNames = properties.getProperty(
            "BrowserNames." + osFlavor);

        if (browserNames != null) {
            for(String browser : browserNames.split(",")) {
                String path = properties.getProperty(
                    "Path." + browser + "." + osFlavor);

                if (path == null)
                    continue;

                File executableFile = new File(path);
                if (!executableFile.exists())
                    continue;

                browserCommand = properties.getProperty(
                    "Invoke." + browser + "." + osFlavor);

                if (browserCommand == null)
                    continue;

                break;
            }
        }
    }

    public XEBrowserLauncher(String url) throws Exception {
        if (browserCommand == null)
            throw new PerformanceException(
                XEMessages.msgs.getString("No_Browser"));

        String command = browserCommand + " " + url;

        Runtime.getRuntime().exec(command);
    }
}
