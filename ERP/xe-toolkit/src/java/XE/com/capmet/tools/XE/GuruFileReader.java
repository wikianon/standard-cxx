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

import java.awt.Color;
import java.util.Vector;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.IOException;
import com.capmet.tools.XE.rules.Rule;

public class GuruFileReader {
    public static final int ALL_STATES   = 0;
    public static final int BLUE_STATE   = 1;
    public static final int WHITE_STATE  = 2;
    public static final int GREEN_STATE  = 3;
    public static final int YELLOW_STATE = 4;
    public static final int ORANGE_STATE = 5;
    public static final int RED_STATE    = 6;
    public static final int BLACK_STATE  = 7;

    private File               logFile;
    private BufferedReader     inputFile = null;
    private Vector<TimeRecord> recordVector = null;
    private int                currentRecord = 0;
    private int                stateColor = ALL_STATES;

    public Vector<TimeRecord> getRecordVector() {
        return recordVector;
    }

    public int getRecordCount() throws IOException {
        FileInputStream fis = new FileInputStream(logFile);
        InputStreamReader isr = new InputStreamReader(fis);
        BufferedReader inputFile = new BufferedReader(isr);
        int recordCount = 0;
        String line;

        while((line = inputFile.readLine()) != null) {
            if (line.startsWith("timestamp")) {
                if (stateColor == ALL_STATES) {
                    recordCount++;
                } else {
                    if ((line = inputFile.readLine()) != null) {
                        String[] token = line.trim().split(" ");
                        Color state = Rule.parseColor(token[1]);

                        switch(stateColor) {
                        case BLUE_STATE:
                            if (state.equals(Color.BLUE))
                                recordCount++;
                            break;
                        case WHITE_STATE:
                            if (state.equals(Color.WHITE))
                                recordCount++;
                            break;
                        case GREEN_STATE:
                            if (state.equals(Color.GREEN))
                                recordCount++;
                            break;
                        case YELLOW_STATE:
                            if (state.equals(Color.YELLOW))
                                recordCount++;
                            break;
                        case ORANGE_STATE:
                            if (state.equals(Color.ORANGE))
                                recordCount++;
                            break;
                        case RED_STATE:
                            if (state.equals(Color.RED))
                                recordCount++;
                            break;
                        case BLACK_STATE:
                            if (state.equals(Color.BLACK))
                                recordCount++;
                            break;
                        }
                    }
                }
            }
        }

        inputFile.close();
        return recordCount;
    }

    public GuruFileReader(String logFile) throws IOException {
        loadRecordVector(new File(logFile), 0, -1);
    }

    public GuruFileReader(File logFile) throws IOException {
        loadRecordVector(logFile, 0, -1);
    }

    public GuruFileReader(String logFile,
                          int    firstRecordNumber,
                          int    recordsToRead) throws IOException {
        loadRecordVector(new File(logFile), firstRecordNumber, recordsToRead);
    }

    public GuruFileReader(File logFile,
                          int  firstRecordNumber,
                          int  recordsToRead) throws IOException {
        loadRecordVector(logFile, firstRecordNumber, recordsToRead);
    }

    private void loadRecordVector(File logFile,
                                  int  firstRecordNumber,
                                  int  recordsToRead) throws IOException {

        FileInputStream fis = new FileInputStream(logFile);
        InputStreamReader isr = new InputStreamReader(fis);

        inputFile = new BufferedReader(isr);
        loadRecordVector(firstRecordNumber, recordsToRead);
        this.logFile = logFile;
    }

    public void refresh() throws IOException {
        inputFile.close();
        FileInputStream fis = new FileInputStream(logFile);
        InputStreamReader isr = new InputStreamReader(fis);

        inputFile = new BufferedReader(isr);
        currentRecord = 0;
    }

    public int getStateColor() {
        return stateColor;
    }

    public void setStateColor(int stateColor) {
        this.stateColor = stateColor;
    }

    public void loadRecordVector(int firstRecordNumber,
                                 int recordsToRead) throws IOException {
        String line;
        int recordCount = 0;

        if (recordVector == null)
            recordVector = new Vector<TimeRecord>();
        else
            recordVector.clear();

        if (firstRecordNumber < currentRecord) {
            // I don't care much for this.
            FileInputStream fis = new FileInputStream(logFile);
            InputStreamReader isr = new InputStreamReader(fis);

            inputFile.close();
            inputFile = new BufferedReader(isr);
            currentRecord = 0;
        }

        // reset the count of records to the current record number
        recordCount = currentRecord;

        while((line = inputFile.readLine()) != null) {
            // look for the beginning of the record
            if (!line.startsWith("timestamp"))
                continue;

            // tick
            currentRecord++;

            // look for the first record of interest
            // only account for recordCount if looking at all records
            if ((stateColor == ALL_STATES) &&
                (recordCount++ < firstRecordNumber))
                    continue;

            String[] token = line.trim().split(" ");
            TimeRecord timeRecord = new TimeRecord();
            boolean skipRecord = false;

            timeRecord.timestamp = restOfTheLine(token, 1);
            while((line = inputFile.readLine()) != null) {
                token = line.trim().split(" ");
                if (token[0].equals("state")) {
                    timeRecord.state = Rule.parseColor(token[1]);
                    if (stateColor != ALL_STATES) {
                        // skip records of any color not being sought
                        switch(stateColor) {
                        case BLUE_STATE:
                            if (!timeRecord.state.equals(Color.BLUE))
                                skipRecord = true;
                            break;
                        case WHITE_STATE:
                            if (!timeRecord.state.equals(Color.WHITE))
                                skipRecord = true;
                            break;
                        case GREEN_STATE:
                            if (!timeRecord.state.equals(Color.GREEN))
                                skipRecord = true;
                            break;
                        case YELLOW_STATE:
                            if (!timeRecord.state.equals(Color.YELLOW))
                                skipRecord = true;
                            break;
                        case ORANGE_STATE:
                            if (!timeRecord.state.equals(Color.ORANGE))
                                skipRecord = true;
                            break;
                        case RED_STATE:
                            if (!timeRecord.state.equals(Color.RED))
                                skipRecord = true;
                            break;
                        case BLACK_STATE:
                            if (!timeRecord.state.equals(Color.BLACK))
                                skipRecord = true;
                            break;
                        }

                        // sought record; bump recordCount and see if we're
                        // beyond the firstRecordNumber yet

                        if (!skipRecord)
                            if (recordCount++ < firstRecordNumber)
                                skipRecord = true;
                    }
                    if (skipRecord)
                        break;
                    timeRecord.stateString = restOfTheLine(token, 2);
                } else if (token[0].equals("trigger"))
                    timeRecord.triggerRule = restOfTheLine(token, 1);
                else if (token[0].equals("action"))
                    timeRecord.action = restOfTheLine(token, 1);
                else if (token[0].equals("top-process"))
                    timeRecord.topProcess = restOfTheLine(token, 1);
                else if (token[0].equals("sort-key"))
                    timeRecord.sortKey = restOfTheLine(token, 1);
                else if (token[0].equals("key-field-value"))
                    timeRecord.keyFieldValue = restOfTheLine(token, 1);
                else if (token[0].equals("rule"))
                    timeRecord.ruleVector.addElement(readRule(line));
                else if (token[0].equals("end"))
                    break;
            }
            if (skipRecord)
                continue;

            recordCount++;
            recordVector.addElement(timeRecord);
            if (recordVector.size() == recordsToRead)
                break;
        }
    }

    private String restOfTheLine(String[] token, int start) {
        String result = null;

        for(int i=start; i<token.length; i++) {
            if (result == null)
                result = token[i];
            else
                result = result + " " + token[i];
        }

        return result;
    }

    private RuleRecord readRule(String firstLine) throws IOException {
        RuleRecord ruleRecord = new RuleRecord();
        String[] token = firstLine.trim().split(" ");
        String line;

        ruleRecord.isSubRule = token[0].equals("sub-rule");
        ruleRecord.ruleName = restOfTheLine(token, 1);

        while((line = inputFile.readLine()) != null) {
            token = line.trim().split(" ");
            if (token[0].equals("state")) {
                ruleRecord.state = Rule.parseColor(token[1]);
                ruleRecord.stateString = restOfTheLine(token, 2);
            } else if (token[0].equals("action"))
                ruleRecord.action = restOfTheLine(token, 1);
            else if (token[0].equals("explanation"))
                ruleRecord.explanation = restOfTheLine(token, 1);
            else if (token[0].equals("furtherDetail"))
                ruleRecord.furtherDetail = restOfTheLine(token, 1);
            else if (token[0].equals("solution"))
                ruleRecord.solution = restOfTheLine(token, 1);
            else if (token[0].equals("sub-rule"))
                ruleRecord.subruleVector.addElement(readRule(line));
            else if (token[0].equals("end"))
                break;
        }

        return ruleRecord;
    }
}
