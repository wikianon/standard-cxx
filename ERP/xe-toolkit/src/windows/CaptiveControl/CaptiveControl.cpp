/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Windows system applications of The XE Toolkit.
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
    char pathbuf[8192];
    std::string classpath;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    if (argv[1] != 0) {
        std::cerr << "Use: CaptiveControl" << std::endl;
        return 1;
    }

    // find the top and the command name
    p = strrchr(argv[0], '\\');
    if (p == 0)
        cmd = argv[0];
    else {
        *p++ = '\0';
        cmd = p;
        top = argv[0];
    }

    // eliminate .exe
    if ((p = strrchr(cmd, '.')) != 0)
        *p = '\0';

    // if top is "." then make it the current directory
    if (strcmp(argv[0], ".") == 0 || top == 0) {
        GetCurrentDirectory(sizeof cwd, cwd);
        top = cwd;
    }

    // I'm looking for the top/.. so lop off the end
    if ((p = strrchr(top, '\\')) != 0)
        *p = '\0';

    // construct a CLASSPATH
    _snprintf_s(cp, sizeof cp, sizeof cp,
        "%s\\jars\\CM.jar;%s\\jars\\Captivity.jar;%s\\jars\\forms.jar;%s\\jars\\looks.jar;%s\\jars\\XE.jar",
            top, top, top, top, top);
    classpath = cp;

    // set it
    SetEnvironmentVariable("CLASSPATH", classpath.c_str());

    // construct a PATH
    _snprintf_s(cp, sizeof cp, sizeof cp,
        ";%s\\lib\\windows\\x86", top);

    // get the current path
    GetEnvironmentVariable("PATH", pathbuf, sizeof pathbuf);

    // stringify it
    std::string path(pathbuf);

    // append our additions
    path += cp;

    // set it
    SetEnvironmentVariable("PATH", path.c_str());

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

    // build the command line
    _snprintf_s(exec, sizeof exec, sizeof exec,
        "java -Dcom.capmet.CaptiveCollector.Top=\"%s\"", top);  

    // stringify it
    std::string cmdLine(exec);

    // add the end
    cmdLine += " com.capmet.Captivity.CaptiveCollector.CaptiveControl";

    // zip
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // go, Jimmy, go
    if (CreateProcess(
        0, (LPSTR) cmdLine.c_str(), 0, 0, FALSE,
            CREATE_NO_WINDOW|CREATE_NEW_PROCESS_GROUP, 0, 0, &si, &pi) == 0) {
                std::cerr << "Cannot start java: error "
                          << GetLastError() << std::endl;
                return 1;
    }

    return 0;
}
