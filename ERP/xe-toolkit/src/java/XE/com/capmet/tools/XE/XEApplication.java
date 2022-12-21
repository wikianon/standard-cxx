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

import javax.swing.*;
import java.awt.Dimension;
import com.jgoodies.looks.LookUtils;
import com.jgoodies.looks.Options;
import com.jgoodies.looks.BorderStyle;
import com.jgoodies.looks.HeaderStyle;
import com.jgoodies.looks.plastic.PlasticLookAndFeel;
import com.jgoodies.looks.plastic.PlasticTheme;
import com.jgoodies.looks.plastic.PlasticXPLookAndFeel;
import com.jgoodies.looks.plastic.theme.SkyKrupp;

/**
 * A class for XE applications to extend so that the look-and-feel
 * can be implemented and any other global application-common functionality.
 */
public class XEApplication {
    /**
     * The lone constructor. Apply global effects, etc.
     */
    public XEApplication() {
        Options.setDefaultIconSize(new Dimension(18, 18));
        Options.setUseNarrowButtons(true);
        
        // Global options
        Options.setTabIconsEnabled(true);
        UIManager.put(Options.POPUP_DROP_SHADOW_ENABLED_KEY, null);
        
        // Swing Settings
        LookAndFeel selectedLaf = new PlasticXPLookAndFeel();
        PlasticLookAndFeel.setPlasticTheme(new SkyKrupp());
        PlasticLookAndFeel.setTabStyle(PlasticLookAndFeel.TAB_STYLE_DEFAULT_VALUE);
        PlasticLookAndFeel.setHighContrastFocusColorsEnabled(false);
        
        // Work around caching in MetalRadioButtonUI
        JRadioButton radio = new JRadioButton();
        radio.getUI().uninstallUI(radio);
        JCheckBox checkBox = new JCheckBox();
        checkBox.getUI().uninstallUI(checkBox);
        
        try {
            UIManager.setLookAndFeel(selectedLaf);
        } catch(UnsupportedLookAndFeelException e) {
            // game over.
            System.err.println(e.getMessage());
            System.exit(0);
        }
    }
}
