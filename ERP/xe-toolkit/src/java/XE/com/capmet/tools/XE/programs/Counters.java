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

import java.awt.event.*;
import javax.swing.JFrame;
import com.capmet.metrics.cm.CmExceptionParser;
import com.capmet.tools.XE.XEDialogs;
import com.capmet.tools.XE.XEApplication;
import com.capmet.tools.XE.XECounterData;
import com.capmet.tools.XE.XEPerformanceFieldSelector;

/**
 * The main program with which to view the
 * <CODE>XEPerformanceFieldSelector</CODE> dialog.
 *
 * Usage: java Counters [host-name].
 */
public class Counters extends XEApplication
{
    private static XEPerformanceFieldSelector pfs;

    /**
     * Test main program for the dialog.
     * @param args Passed from environment.
     */
    public static void main(String[] args) {
        new Counters(args);
    }

    /**
     * The single constructor.
     * @param args The argument vector passed from the environment.
     * Possibly contains the name of a host to communicate with.
     */
    public Counters(String[] args) {
        try {
            pfs = new XEPerformanceFieldSelector(
                (args.length == 1 ? args[0] : null)) {
                    public void actionPerformed(ActionEvent ev) {
                        XECounterData[] cd = pfs.getSelectedCounters();
                        for(int i=0; i<cd.length; i++)
                            System.out.println(cd[i].counter);
                    }
                };

            pfs.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            pfs.setVisible(true);
        } catch(Exception e) {
            XEDialogs.fatalDialog(new CmExceptionParser(e).getMessage());
        }
    }
}
