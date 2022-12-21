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

import javax.swing.JApplet;
import com.capmet.tools.XE.XECpuMeter;

/**
 * CpuMeterApplet is a JApplet that displays the XE Toolkit
 * CpuMeter.
 */
public class CpuMeterApplet extends JApplet
{
    /**
     * This is the "main" function for the JApplet code. The
     * boolean parameter "vertical" can be specified within the
     * applet tag.
     */
    public void init()
    {
        try
        {
            boolean vertical = false;
            String vertParam = getParameter("vertical");
            String hostname = getCodeBase().getHost();
            String orientation = null;
            
            if (vertParam != null)
                vertical = Boolean.parseBoolean(vertParam);

            if (vertical)
                orientation = "orientation=vertical";
            else
                orientation = "orientation=horizontal";
            
            XECpuMeter cm = new XECpuMeter(this, hostname, 2000, orientation);
            
            setContentPane(cm.getPane());
            
            cm.start();
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
    }
}
