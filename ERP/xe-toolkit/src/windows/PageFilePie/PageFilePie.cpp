/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Windows applications of The XE Toolkit.
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


// PageFilePie.cpp : Launch the PageFilePie application.
//

#include "stdafx.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    char *p;
    char *cmd = 0;
    char *top = 0;
    char cp[BUFSIZ];
    char exec[BUFSIZ];
    char cwd[BUFSIZ];
    std::string classpath;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    fclose(stdout);
    fclose(stderr);

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    p = strrchr(argv[0], '\\');
    if (p == 0)
        cmd = argv[0];
    else {
        *p++ = '\0';
        cmd = p;
        top = argv[0];
    }

    if ((p = strrchr(cmd, '.')) != 0)
        *p = '\0';

    if (strcmp(argv[0], ".") == 0 || top == 0) {
        GetCurrentDirectory(sizeof cwd, cwd);
        top = cwd;
    }

    if ((p = strrchr(top, '\\')) != 0)
        *p = '\0';

    _snprintf_s(cp, sizeof cp, sizeof cp,
        "%s\\jars\\CM.jar;%s\\jars\\XE.jar;%s\\jars\\Captivity.jar",
            top, top, top);
    classpath = cp;
    _snprintf_s(cp, sizeof cp, sizeof cp, ";%s\\jars\\Chart2D.jar", top);
    classpath += cp;
    _snprintf_s(cp, sizeof cp, sizeof cp, ";%s\\jars\\forms.jar", top);
    classpath += cp;
    _snprintf_s(cp, sizeof cp, sizeof cp, ";%s\\jars\\looks.jar", top);
    classpath += cp;

    SetEnvironmentVariable("CLASSPATH", classpath.c_str());

    // build the path to the environment file
    std::string envpath(top);

    envpath += "\\etc\\Windows.env";

    // open the file
    std::ifstream input(envpath.c_str());

    // process it
    if (input.is_open()) {
        char buf[BUFSIZ];

        while(input.getline(buf, sizeof buf)) {
            if (buf[0] == '\n' || buf[0] == '#')
                continue;

            char *context = 0;
            char *tag = strtok_s(buf, "=", &context);
            char *value = strtok_s(0, "=", &context);

            if (tag == 0 || value == 0)
                continue;

            SetEnvironmentVariable(tag, value);
        }
    }

    _snprintf_s(exec, sizeof exec, sizeof exec,
        "java \"-Dcom.capmet.CaptiveCollector.Top=%s\" com.capmet.tools.XE.programs.%s", top, cmd);

    std::string cmdLine(exec);
    for(int i=1; i<argc; i++) {
        cmdLine += " ";
        cmdLine += argv[i];
    }

    if( !CreateProcess(
        NULL,             // No module name (use command line)
        (LPSTR) cmdLine.c_str(),  // Command line
        NULL,             // Process handle not inheritable
        NULL,             // Thread handle not inheritable
        FALSE,            // Set handle inheritance to FALSE
        CREATE_NO_WINDOW, // Do NOT pop up a cmd window
        NULL,             // Use parent's environment block
        NULL,             // Use parent's starting directory 
        &si,              // Pointer to STARTUPINFO structure
        &pi )             // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        std::cerr << "CreateProcess failed, error: " << GetLastError() << std::endl;
        return -1;
    }

    return 0;
}
