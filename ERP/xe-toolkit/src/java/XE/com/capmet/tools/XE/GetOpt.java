package com.capmet.tools.XE;

/**
 * Lifted from the web without any copyright from:
 * http://www.cs.wmich.edu/~parint/jli/count/visual/advos/ConcProgJava/lib/Utilities/GetOpt.java<br>
 *

 * <h3>OVERVIEW:</h3>
 *
 * GetOpt provides a general means for a Java program to parse command
 * line arguments in accordance with the standard Unix conventions;
 * it is analogous to, and based on, getopt(3) for C programs.
 * (The following documentation is based on the man page for getopt(3).)

 * <h3>DESCRIPTION:</h3>
 *
 * GetOpt is a Java class that provides one method, getopt,
 * and some variables that control behavior of or return additional
 * information from getopt.
 * <p>
 * GetOpt interprets command arguments in accordance with the standard
 * Unix conventions: option arguments of a command are introduced by "-"
 * followed by a key character, and a non-option argument terminates
 * the processing of options.  GetOpt's option interpretation is controlled
 * by its parameter optString, which specifies what characters designate
 * legal options and which of them require associated values.
 * <p>
 * The getopt method returns the next, moving left to right, option letter
 * in the command line arguments that matches a letter in optString.
 * optString must contain the option letters the command using getopt
 * will recognize.  For example, getopt("ab") specifies that the command
 * line should contain no options, only "-a", only "-b", or both "-a" and
 * "-b" in either order.  (The command line can also contain non-option
 * arguments after any option arguments.)  Multiple options per argument
 * are allowed, e.g., "-ab" for the last case above.
 * <p>
 * If a letter in optString is followed by a colon, the option is expected
 * to have an argument.  The argument may or may not be separated by
 * whitespace from the option letter.  For example, getopt("w:") allows
 * either "-w 80" or "-w80".  The variable optArg is set to the option
 * argument, e.g., "80" in either of the previous examples.  Conversion
 * functions such as Integer.parseInt(), etc., can then be applied to
 * optArg.
 * <p>
 * getopt places in the variable optIndex the index of the next command
 * line argument to be processed; optIndex is automatically initialized
 * to 1 before the first call to getopt.
 * <p>
 * When all options have been processed (that is, up to the first
 * non-option argument), getopt returns optEOF (-1).  getopt recognizes the
 * command line argument "--" (i.e., two dashes) to delimit the end of
 * the options; getopt returns optEOF and skips "--".  Subsequent,
 * non-option arguments can be retrieved using the String array passed to
 * main(), beginning with argument number optIndex.

 * <h3>DIAGNOSTICS:</h3>
 *
 * getopt prints an error message on System.stderr and returns a question
 * mark ('?') when it encounters an option letter in a command line argument
 * that is not included in optString.  Setting the variable optErr to
 * false disables this error message.

 * <h3>NOTES:</h3>
 *
 * The following notes describe GetOpt's behavior in a few interesting
 * or special cases; these behaviors are consistent with getopt(3)'s
 * behaviors.
 * <ul>
 * <li>A '-' by itself is treated as a non-option argument.</li>
 * <li>If optString is "a:" and the command line arguments are "-a -x",
 *    then "-x" is treated as the argument associated with the "-a".
 * <li>Duplicate command line options are allowed; it is up to user to
 *    deal with them as appropriate.</li>
 * <li>A command line option like "-b-" is considered as the two options
 *    "b" and "-" (so "-" should appear in option string); this differs
 *    from "-b --".</li>
 * <li>Sun and DEC getopt(3)'s differ w.r.t. how "---" is handled.
 *    Sun treats "---" (or anything starting with "--") the same as "--"
 *    DEC treats "---" as two separate "-" options
 *    (so "-" should appear in option string).
 *    Java GetOpt follows the DEC convention.</li>
 * <li>An option `letter' can be a letter, number, or most special character.
 *    Like getopt(3), GetOpt disallows a colon as an option letter.</li>
 * </ul>
 */
public class GetOpt {

   private String[] theArgs = null;
   private int argCount = 0;
   private String optString = null;

   /**
    * Create a new arg parser.
    * @param args Argument array from main method.
    * @param opts The option string.
    */
   public GetOpt(String[] args, String opts) {
      theArgs = args;
      argCount = theArgs.length;
      optString = opts;
   }

   /**
    * user can toggle this to control printing of error messages
    */
   public boolean optErr = false;

   /**
    * Process an integer argument.
    * @param arg The string argument.
    * @param n A value to return upon failure.
    * @return The processed arg or the default value.
    */
   public int processArg(String arg, int n) {
      int value;
      try {
         value = Integer.parseInt(arg);
      } catch (NumberFormatException e) {
         if (optErr)
            System.err.println("processArg cannot process " + arg
               + " as an integer");
         return n;
      }
      return value;
   }

   /**
    * Try to process an integer argument.
    * @param k The index of the arg array.
    * @param n A value to return upon failure.
    * @return The processed arg or the default value.
    */
   public int tryArg(int k, int n) {
      int value;
      try {
         value = processArg(theArgs[k], n);
      } catch (ArrayIndexOutOfBoundsException e) {
         if (optErr)
            System.err.println("tryArg: no theArgs[" + k + "]");
            return n;
      }
      return value;
   }

   /**
    * Process a long argument.
    * @param arg The string argument.
    * @param n A value to return upon failure.
    * @return The processed arg or the default value.
    */
   public long processArg(String arg, long n) {
      long value;
      try {
         value = Long.parseLong(arg);
      } catch (NumberFormatException e) {
         if (optErr)
            System.err.println("processArg cannot process " + arg
               + " as a long");
         return n;
      }
      return value;
   }

   /**
    * Try to process a long argument.
    * @param k The index of the arg array.
    * @param n A value to return upon failure.
    * @return The processed arg or the default value.
    */
   public long tryArg(int k, long n) {
      long value;
      try {
         value = processArg(theArgs[k], n);
      } catch (ArrayIndexOutOfBoundsException e) {
         if (optErr)
            System.err.println("tryArg: no theArgs[" + k + "]");
            return n;
      }
      return value;
   }

   /**
    * Process a double argument.
    * @param arg The string argument.
    * @param d A value to return upon failure.
    * @return The processed arg or the default value.
    */
   public double processArg(String arg, double d) {
      double value;
      try {
         value = Double.valueOf(arg).doubleValue();
      } catch (NumberFormatException e) {
         if (optErr)
            System.err.println("processArg cannot process " + arg
               + " as a double");
         return d;
      }
      return value;
   }

   /**
    * Try to process a double argument.
    * @param k The index of the arg array.
    * @param d A value to return upon failure.
    * @return The processed arg or the default value.
    */
   public double tryArg(int k, double d) {
      double value;
      try {
         value = processArg(theArgs[k], d);
      } catch (ArrayIndexOutOfBoundsException e) {
         if (optErr)
            System.err.println("tryArg: no theArgs[" + k + "]");
            return d;
      }
      return value;
   }

   /**
    * Process a float argument.
    * @param arg The string argument.
    * @param f A value to return upon failure.
    * @return The processed arg or the default value.
    */
   public float processArg(String arg, float f) {
      float value;
      try {
         value = Float.valueOf(arg).floatValue();
      } catch (NumberFormatException e) {
         if (optErr)
            System.err.println("processArg cannot process " + arg
               + " as a float");
         return f;
      }
      return value;
   }

   /**
    * Try to process a float argument.
    * @param k The index of the arg array.
    * @param f A value to return upon failure.
    * @return The processed arg or the default value.
    */
   public float tryArg(int k, float f) {
      float value;
      try {
         value = processArg(theArgs[k], f);
      } catch (ArrayIndexOutOfBoundsException e) {
         if (optErr)
            System.err.println("tryArg: no theArgs[" + k + "]");
            return f;
      }
      return value;
   }

   /**
    * Process a boolean argument.
    * @param arg The string argument.
    * @param b A value to return upon failure.
    * @return The processed arg or the default value.
    */
   public boolean processArg(String arg, boolean b) {
      // `true' in any case mixture is true; anything else is false
      return Boolean.valueOf(arg).booleanValue();
   }

   /**
    * Try to process a boolean argument.
    * @param k The index of the arg array.
    * @param b A value to return upon failure.
    * @return The processed arg or the default value.
    */
   public boolean tryArg(int k, boolean b) {
      boolean value;
      try {
         value = processArg(theArgs[k], b);
      } catch (ArrayIndexOutOfBoundsException e) {
         if (optErr)
            System.err.println("tryArg: no theArgs[" + k + "]");
            return b;
      }
      return value;
   }

   /**
    * Try to process a String argument.
    * @param k The index of the arg array.
    * @param s A value to return upon failure.
    * @return The processed arg or the default value.
    */
   public String tryArg(int k, String s) {
      String value;
      try {
         value = theArgs[k];
      } catch (ArrayIndexOutOfBoundsException e) {
         if (optErr)
            System.err.println("tryArg: no theArgs[" + k + "]");
            return s;
      }
      return value;
   }

   private static void writeError(String msg, char ch) {
      System.err.println("GetOpt: " + msg + " -- " + ch);
   }

   /**
    * Constant value representing the end of argument processing.
    */
   public static final int optEOF = -1;

   private int optIndex = 0;

   /**
    * Get the current option index.
    * @return The option index.
    */
   public int getOptIndex() {return optIndex;}

   private String optArg = null;

   /**
    * Get the current option argument.
    * @return The option argument.
    */
   public String getOptArg() {return optArg;}

   private int optPosition = 1;

   /**
    * The method that drives argument processing.
    * @return The character of the arg just parsed or optEOF at end.
    */
   public int getopt() {
      optArg = null;
      if (theArgs == null || optString == null) return optEOF;
      if (optIndex < 0 || optIndex >= argCount) return optEOF;
      String thisArg = theArgs[optIndex];
      int argLength = thisArg.length();
      // handle special cases
      if (argLength <= 1 || thisArg.charAt(0) != '-') {
         // e.g., "", "a", "abc", or just "-"
         return optEOF;
      } else if (thisArg.equals("--")) { // end of non-option args
         optIndex++;
         return optEOF;
      }
      // get next "letter" from option argument
      char ch = thisArg.charAt(optPosition);
      // find this option in optString
      int pos = optString.indexOf(ch);
      if (pos == -1 || ch == ':') {
         if (optErr) writeError("illegal option", ch);
         ch = '?';
      } else { // handle colon, if present
         if (pos < optString.length()-1 && optString.charAt(pos+1) == ':') {
            if (optPosition != argLength-1) {
               // take rest of current arg as optArg
               optArg = thisArg.substring(optPosition+1);
               optPosition = argLength-1; // force advance to next arg below
            } else { // take next arg as optArg
               optIndex++;
               if (optIndex < argCount
                     && (theArgs[optIndex].charAt(0) != '-' ||
                     theArgs[optIndex].length() >= 2 &&
                     (optString.indexOf(theArgs[optIndex].charAt(1)) == -1
                     || theArgs[optIndex].charAt(1) == ':'))) {
                  optArg = theArgs[optIndex];
               } else {
                  if (optErr) writeError("option requires an argument", ch);
                  optArg = null;
                  ch = ':'; // Linux man page for getopt(3) says : not ?
               }
            }
         }
      }
      // advance to next option argument,
      // which might be in thisArg or next arg
      optPosition++;
      if (optPosition >= argLength) {
         optIndex++;
         optPosition = 1;
      }
      return ch;
   }
}
