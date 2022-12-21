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
:: This script starts the CaptiveControl GUI which controls and reports
:: on the status of CaptiveCollector. There are no arguments to this program.
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
IF NOT "%1%"=="" GOTO ERROR
SET TOP=%~s0\..\..
SET D=-Dcom.capmet.CaptiveCollector.Top=%TOP%
SET CLASSPATH=%TOP%\jars\CM.jar;%TOP%\jars\Captivity.jar;%TOP%\jars\XE.jar;%TOP%\jars\forms.jar;%TOP%\jars\looks.jar;.
SET C=com.capmet.Captivity.CaptiveCollector.CaptiveControl
::
:: If you need to change the PATH, do it here
::
PATH=%TOP%\lib\windows\x86;%PATH%
::
::
java %D% %C%
GOTO END

:ERROR
ECHO Use: CaptiveControl

:END
