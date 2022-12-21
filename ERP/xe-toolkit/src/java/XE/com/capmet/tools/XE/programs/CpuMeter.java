/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the XE programs for The XE Toolkit.
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

package com.capmet.tools.XE.programs;

import java.net.URL;
import javax.swing.JFrame;
import javax.swing.ImageIcon;
import java.net.InetAddress;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.tools.XE.GetOpt;
import com.capmet.tools.XE.XEMessages;
import com.capmet.tools.XE.XEApplication;
import com.capmet.tools.XE.XECpuMeter;

/**
 * An application for monitoring the CPUs on a host.<p>
 *
 * Use: java CpuMeter [-v] [-h remote-host]<br>
 *      -v vertical orientation
 */
public class CpuMeter extends XEApplication
{
    /**
     * The main method of the CpuMeter application.
     * @param args Arguments passed from the environment
     */
    public static void main(String[] args)
    {
        new CpuMeter(args);
    }

    // usage blurb
    private void usage()
    {
        System.out.println(XEMessages.msgs.getString("CpuMeter_Usage")); 
    }

    /**
     * CpuMeter constructor.
     * @param args Arguments passed from the environment
     */
    public CpuMeter(String[] args)
    {
        try {
            // whether localhost or remotehost
            String remoteHost = null;
            String hostname = null;
            // orientation: vertical or horizontal
            String orientation = null;
            GetOpt opts = new GetOpt(args, "h:v");
            int c;

            opts.optErr = false;
            while((c = opts.getopt()) != opts.optEOF) {
                switch(c) {
                case 'h':
                    remoteHost = opts.getOptArg();
                    break;
                case 'v':
                    orientation = "orientation=vertical";
                    break;
                default:
                    usage();
                    return;
                }
            }
            
            if (orientation == null)
                orientation = "orientation=horizontal";
            
            // if localhost, use the actual name of the host
            if (remoteHost == null) {
                hostname = InetAddress.getLocalHost().getHostName();
                int dot = hostname.indexOf(".");
                if (dot != -1)
                    hostname = hostname.substring(0, dot);
            } else
                hostname = remoteHost;
            
            // for JFrame
            String title = "CpuMeter: " + hostname;
            
            JFrame frame = new JFrame(title);

            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

            String ICON_PATH = null;
            URL imageURL = null;
            ImageIcon cpuMeter = null;

            ICON_PATH = "com/capmet/tools/XE/images/ktimemon.png";
            imageURL = getClass().getClassLoader().getResource(ICON_PATH);
            if (imageURL != null)
                cpuMeter = new ImageIcon(imageURL);

            if (cpuMeter != null)
                frame.setIconImage(cpuMeter.getImage());

            // let the constructor do the heavy lifting
            XECpuMeter cm = new XECpuMeter(frame, remoteHost, 2000, orientation);
            
            frame.setContentPane(cm.getPane());
            
            // reorganize
            frame.pack();
            
            // start the clock
            cm.start();
            
            // go
            frame.setVisible(true);
        } catch(Exception e) {
            System.err.println(new CmExceptionParser(e).getMessage());
        }
    }
}
