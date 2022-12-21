/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector application of Captivity.
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

package com.capmet.Captivity.CaptiveCollector;

import java.io.StringWriter;
import java.io.PrintWriter;
import java.util.Date;
import java.text.MessageFormat;
import java.util.logging.*;

/**
 * The line formatter class used by the <CODE>Logger</CODE>
 * code to format lines in the log file.
 */
public class CaptiveFormatter extends Formatter {
    private Date date;

    /**
     * Default constructor.
     */
    public CaptiveFormatter() {
        date = new Date();
    }

    /**
     * Format the log record passed from <CODE>Logger</CODE> to
     * <CODE>FileHandler</CODE> and then here.
     * @param record The <CODE>LogRecord</CODE> passed down.
     * @return A formatted log record string.
     */
    public synchronized String format(LogRecord record) {
        String message = formatMessage(record);
        String level = record.getLevel().getLocalizedName();
        String source;
        String method;
        String other = null;

        date.setTime(record.getMillis());

        source = record.getSourceClassName();
        if (source == null)
            source = record.getLoggerName();
        int n = source.lastIndexOf('.');
        if (n != -1)
            source = source.substring(n+1, source.length());

        method = record.getSourceMethodName();
        if (method == null)
            method = "<?>";

        Throwable t = record.getThrown();
        if (t != null) {
            StringWriter sw = new StringWriter();
            PrintWriter pw = new PrintWriter(sw);

            record.getThrown().printStackTrace(pw);
            pw.close();
            other = sw.toString();
        }

        String result = MessageFormat.format(
            "{0,date,MM/dd/yy}~~{0,time,HH:mm:ss}~~{1}~~{2}~~{3}~~{4}~~{5}\n",
                date, level, source, method, message,
                    (other != null ? other : ""));

        return result;
    }
}
