:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::
:: Copyright (c) 2006-2007 Captive Metrics Software Corporation.
::                    All rights reserved.
::
:: This file is part of the configuration files of Captivity.
::
:: This file may be used under the terms of the GNU General Public
:: License version 2.0 as published by the Free Software Foundation
:: and appearing in the file LICENSE-GPL.txt contained within the
:: same package as this file. This software is subject to a
:: dual-licensing mechanism, the details of which are outlined in
:: file LICENSE-DUAL.txt, also contained within this package. Be sure
:: to use the correct license for your needs. To view the commercial
:: license, read LICENSE-COMMERCIAL.txt also contained within this
:: package.
::
:: If you are unsure which license is appropriate for your use, please
:: contact the sales department at sales@captivemetrics.com.
::
:: This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
:: WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
::
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

::
:: This script controls the CaptiveCollector process. The script must
:: be called with one argument, either "start", "stop" or "report".
::
:: @ECHO OFF
SETLOCAL
::
:: get top level directory and set classpath
::
:: incantations in the following lines:
:: %0 is argv[0]
:: %~s0 is the short (8.3) version of the script path
:: %~d0 is the drive letter of %0
:: %~p0 is the path of %0 (adds trailing \)
::
:: (taking the .. of a file works on dos. hmm.)
::
:: so TOP ends up like C:\Progra~1\Captiv~1\XETool~1\System~1\Captiv~2.bat\..\..
::
SET TOP=%~s0\..\..
SET D="-Dcom.capmet.CaptiveCollector.Top=%TOP%"
SET CLASSPATH=%TOP%\jars\CM.jar;%TOP%\jars\Captivity.jar;.
::
:: determine if using v4 IP stack
::
SET V4FLAG=false
FOR /F "eol=# delims== tokens=1,2" %%i IN ("%TOP%\etc\CaptiveCollector.properties") DO IF %%i==java.net.preferIPv4Stack SET V4FLAG=%%j
IF %V4FLAG%==FALSE SET V4FLAG=false
IF %V4FLAG%==false (SET V4=) ELSE (SET V4=-Djava.net.preferIPv4Stack=%V4FLAG%)
::
::
SET C=NO
::
IF "%1%"=="" GOTO ERROR
IF %1%==start SET C=com.capmet.Captivity.CaptiveCollector.CaptiveCollector
IF %1%==stop SET C=com.capmet.Captivity.CaptiveCollector.CCShutdown
IF %1%==report SET C=com.capmet.Captivity.CaptiveCollector.CCStatus
IF %C%==NO GOTO ERROR
::
:: If you need to change the PATH, do it here
::
PATH=%TOP%\lib\windows\x86;%PATH%
::
::
java %D% %V4% %C%
GOTO END

:ERROR
ECHO Use: CaptiveCollector {start OR stop OR report}

:END
