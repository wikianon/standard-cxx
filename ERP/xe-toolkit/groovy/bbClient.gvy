/****************************************************************************
**
** Copyright (c) 2006-2008 Captive Metrics Software Corporation.
**                    All rights reserved.
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

import java.awt.Color;
import java.util.Date;
import java.text.MessageFormat;
import com.capmet.tools.XE.rules.*;
import com.capmet.metrics.cm.CmExceptionParser;

//
// Client application for BigBrother.
//    Use: bbClient bb-server [-i interval] [-h host{,host}]
//
//    Example:
//        bbClient bbserver.bb4.com -i 120 -h abbott.bb4.com,costello.bb4.com
//

class bbClient {

  static usage() {
    println("Use: bbClient bb-server [-i interval] [-h host{,host}]");
  }

  // main method; parse args, launch monitors
  static void main(args) {
    def DEFAULT_INTERVAL = 120;  // 120 second default interval
    try {
      def bbHost = null;
      def interval = DEFAULT_INTERVAL;
      def bbName = null;
      def n = args.size();

      // first arg
      if (args.length == 0) {
        usage();
        return;
      }
      bbHost = args[0];

      // parse command line args
      for(int i=1; i<n; i++) {
        switch(args[i]) {
        case "-i":
          interval = Integer.parseInt(args[++i]);
          break;
        case "-h":
          bbName = args[++i];
          break;
        default:
          usage();
          return;
        }
      }

      // must specify server
      if (bbHost == null) {
        usage();
        return;
      }

      // if not monitoring a remote host
      if (bbName == null) {
        File alias = new File("/opt/bb/etc/bbaliasname");

        // extract one line of text
        if (alias.exists()) {
          bbName = alias.text;
        } else
          bbName = InetAddress.getLocalHost().getHostName();
      }

      // hostname can be a comma separated list of hostnames
      bbName.split(",").each { host ->
        new bbMonitor(bbHost, host, interval).start();
      }

    } catch(Exception e) {
      println(new CmExceptionParser(e).getMessage());
      return;
    }
  }
}

// launch a new thread for each host monitored
class bbMonitor extends Thread {
  private bbHost;
  private bbName;
  private interval;

  public bbMonitor(bbHost, bbName, interval) {
    this.bbHost = bbHost;
    this.bbName = bbName;
    this.interval = interval;
  }

  public void run() {
    try {
      // groovy list of rules
      def ruleSet = [
        new LiveCPURule(bbName),      new LiveDiskRule(bbName),
        new LiveMemoryRule(bbName),   new LiveNICRule(bbName),
        new LivePageFileRule(bbName), new LiveCacheRule(bbName),
        new LiveTCPRule(bbName)
      ];
      def max = 0;

      // for neatness, find the max length of the rule name
      ruleSet.each { rule ->
        if (rule.isRuleSupported()) {
          def s = rule.getShortRuleName();
          if (s.length() > max)
            max = s.length() + 1; // add 1 for the :
        }
      }

      // build the format string passed to printf
      String format = "%-" + max.toString() + "." + max.toString() + "s";
      def command = [
        "/opt/bb/bin/bb",
        bbHost
      ];

      // for(ever)
      while(true) {
        // pause first
        Thread.sleep(interval * 1000);

        // start lower than white
        Color state = Color.PINK;
        Rule peakRule = null;
        String one_token = "status " + bbName + ".XE_Client";
        String details = "";

        // run through, fire the rules and find the peak rule
        ruleSet.each { rule ->
          // only deal with supported rules, obviously
          if (rule.isRuleSupported()) {

            // fire the rule
            rule.fireLiveRule();

            // track the worse state
            if (Rule.isWorseThan(rule.getState(), state)) {
              state = rule.getState();
              peakRule = rule;
            }

            // append the details string
            if (details.length() > 0)
              details += "\n";

            details = details +
              "&" + bbstate(rule.getState()) + " " + 
              String.format(format,
                (rule.getShortRuleName().replace(' ', '_') + ":")) +
              " " + rule.getFurtherDetail();
          }
        }

        // build the next argument of the command vector as one token
        one_token = one_token + " " + bbstate(state) + " " +
          MessageFormat.format("{0,date,E MMM d HH:mm:ss yyyy}", new Date()) +
          "\n" + details;

        // this completes the command vector
        command[2] = one_token;

        // run bb to put the data into BigBrother
        run_bb(bbName, command);
      }
    } catch(Exception e) {
      println(new CmExceptionParser(e).getMessage());
      return;
    }
  }

  // map XE colors into BB colors
  String bbstate(Color c) {
    def colorMap = [(Color.WHITE):"green ",   (Color.BLUE):"green ",
                    (Color.GREEN):"green ",   (Color.YELLOW):"green ",
                    (Color.ORANGE):"yellow", (Color.RED):"red   ",
                    (Color.BLACK):"red   "];

    if (colorMap.containsKey(c))
      return colorMap[c];

    return "clear";
  }

  // run the bb command, wait for completion and print any errors
  synchronized void run_bb(host, command) {
    def proc = command.execute();
    proc.waitFor();
    def t = proc.in.text + proc.err.text;
    if (t != '')
      println(t);

    // else
    //   println(host + " done " +
    //     MessageFormat.format("{0,date,HH:mm:ss}", new Date()));
  }
}
