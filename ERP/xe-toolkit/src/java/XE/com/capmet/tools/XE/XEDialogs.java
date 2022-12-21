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

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.net.URL;

/**
 * A convenience class that holds static dialog methods.
 */
public class XEDialogs {
    /**
     * Popup a fatal dialog with an EXIT button that will exit the application.
     * @param msg A message to display in the dialog.
     */
    public static void fatalDialog(String msg) {
        fatalDialog(null, msg);
    }
    
    /**
     * Popup a fatal dialog with an EXIT button that will exit the application.
     * @param parent The parent container to center the dialog in.
     * @param msg A message to display in the dialog.
     */
    public static void fatalDialog(Container parent, String msg) {
        JOptionPane.showMessageDialog(parent, msg,
                XEMessages.msgs.getString("Fatal_Error"),
                JOptionPane.ERROR_MESSAGE);
        System.exit(0);
    }
    
    /**
     * Popup a warning dialog with a CLOSE button that will dismiss the dialog.
     * @param msg A message to display in the dialog.
     */
    public static void warningDialog(String msg) {
        warningDialog(null, msg);
    }
    
    /**
     * Popup a warning dialog with a CLOSE button that will dismiss the dialog.
     * @param parent The parent container to center the dialog in.
     * @param msg A message to display in the dialog.
     */
    public static void warningDialog(Container parent, String msg) {
        JOptionPane.showMessageDialog(parent, msg,
                XEMessages.msgs.getString("Warning"), JOptionPane.WARNING_MESSAGE);
    }
    
    /**
     * Popup a question dialog with YES and NO buttons that will dismiss the dialog.
     * @param msg A question to ask in the dialog.
     */
    public static int questionDialog(String msg) {
        return questionDialog(null, msg);
    }
    
    /**
     * Popup a question dialog with YES and NO buttons that will dismiss the dialog.
     * @param parent The parent container to center the dialog in.
     * @param msg A question to ask in the dialog.
     */
    public static int questionDialog(Container parent, String msg) {
        return JOptionPane.showConfirmDialog(parent, msg,
                XEMessages.msgs.getString("Question"), JOptionPane.YES_NO_OPTION);
    }
    
    /**
     * Popup an info dialog with an OK button that will dismiss the dialog.
     * @param msg A message to display in the dialog.
     */
    public static void infoDialog(String msg) {
        infoDialog(null, msg);
    }
    
    /**
     * Popup a info dialog with an OK button that will dismiss the dialog.
     * @param parent The parent container to center the dialog in.
     * @param msg A message to display in the dialog.
     */
    public static void infoDialog(Container parent, String msg) {
        JOptionPane.showMessageDialog(parent, msg,
                XEMessages.msgs.getString("Information"), JOptionPane.INFORMATION_MESSAGE);
    }
    
    /**
     * Popup an input dialog with an OK and Cancel button that will dismiss the dialog.
     * @param msg A prompt to display in the dialog.
     * @param title A title for the frame.
     */
    public static String inputDialog(String msg, String title) {
        return inputDialog(null, msg, title);
    }
    
    /**
     * Popup an input dialog with an OK and Cancel button that will dismiss the dialog.
     * @param parent The parent container to center the dialog in.
     * @param msg A prompt to display in the dialog.
     * @param title A title for the frame.
     */
    public static String inputDialog(Container parent, String msg, String title) {
        return JOptionPane.showInputDialog(parent, msg, title, JOptionPane.QUESTION_MESSAGE);
    }
    
    /**
     * Popup a dialog with the "about" information.
     * @param parent The parent container to center the dialog in.
     * @param msg The "about" information.
     * @param imagePath A path to an image representing the application icon.
     */
    public static void aboutDialog(Container parent, String msg, String imagePath) {
        // need a reference to getClass()
        if (parent == null)
            return;
        URL imageURL = ClassLoader.getSystemClassLoader().getResource(imagePath);
        Icon theIcon = new ImageIcon(imageURL);
        final String title = XEMessages.msgs.getString("About");
        
        JOptionPane.showMessageDialog(parent, msg, title, JOptionPane.PLAIN_MESSAGE, theIcon);
    }
}
