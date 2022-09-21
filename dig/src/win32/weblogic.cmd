@echo off

@rem @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@rem 
@rem ID:        $Id$
@rem 
@rem Author:    $Author$
@rem            $Date$
@rem 
@rem Initial:   paul.x.egan.-nd@dig.com
@rem            2003/06/24
@rem 
@rem Description:
@rem 
@rem            Script to start & stop a weblogic server
@rem 
@rem            weblogic <start|stop|ping|install> server_name domain_name
@rem 
@rem History:   $Log: weblogic.cmd,v $
@rem History:   Revision 1.1  2003/07/03 14:11:39  pegan
@rem History:   Initial revision
@rem History:
@rem
@rem Copyright: (C) Walt Disney Internet Group.  All rights reserved.
@rem
@rem @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SETLOCAL

set APP_ROOT=C:\apps
set PROPS_FILE=properties.cmd


@rem @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@rem check arguments

if "%1"=="" ( goto usage ) else set cmd=%1
if "%2"=="" ( goto usage ) else set SERVER_NAME=%2
if "%3"=="" ( goto usage ) else set DOMAIN_NAME=%3


@rem @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@rem verify existence of domain and server dirs

if not exist %APP_ROOT%\%DOMAIN_NAME% (
  echo %APP_ROOT%\%DOMAIN_NAME% not found
  goto end
)

if not exist %APP_ROOT%\%DOMAIN_NAME%\%PROPS_FILE% (
  echo %APP_ROOT%\%DOMAIN_NAME%\%PROPS_FILE% not found
  goto end
)

if not exist %APP_ROOT%\%DOMAIN_NAME%\%SERVER_NAME% (
  echo %APP_ROOT%\%DOMAIN_NAME%\%SERVER_NAME% not found
  goto end
)


@rem @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@rem load domain & server properties

call %APP_ROOT%\%DOMAIN_NAME%\properties.cmd

if exist %APP_ROOT%\%DOMAIN_NAME%\%SERVER_NAME%\config\%PROPS_FILE% (
  call %APP_ROOT%\%DOMAIN_NAME%\%SERVER_NAME%\config\%PROPS_FILE%
)


@rem @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@rem jump to cmd

if "%cmd%"=="start"    goto start
if "%cmd%"=="stop"     goto admin_cmd
if "%cmd%"=="ping"     goto admin_cmd
if "%cmd%"=="install"  goto install

goto usage


@rem @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@rem start server

:start

cd %WL_HOME%
echo on

java %JAVA_OPTIONS% ^
  -classpath "%CLASSPATH%" ^
  %PROPERTIES% ^
  weblogic.Server
@rem | cronolog logs\stdout\%SERVER_NAME%_%%Y%%m%%d.log

@echo off
goto end


@rem @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@rem run server admin command

:admin_cmd

if "%cmd%"=="stop" set cmd=shutdown
echo on

java -hotspot ^
  -classpath "%CLASSPATH%" ^
  weblogic.Admin ^
  -url t3://%SERVER_HOST%:%SERVER_PORT% ^
  -username system ^
  -password %DOMAIN_PASSWD% ^
  %cmd%

@echo off
goto end


@rem @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@rem install service

:install

set SVC_NAME="Weblogic %DOMAIN_NAME%_%SERVER_NAME%"

beasvc -remove -svcname:%SVC_NAME%

beasvc -install ^
  -svcname:%SVC_NAME% ^
  -javahome:%JAVA_HOME% ^
  -execdir:%WL_HOME% ^
  -log:%WL_HOME%\logs\stdout\%SERVER_NAME%.log ^
  -stopclass:weblogic.Server ^
  -password:%DOMAIN_PASSWD% ^
  -cmdline:"%JAVA_OPTIONS% -cp %CLASSPATH% %PROPERTIES% weblogic.Server"

goto end


@rem @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@rem finish

:usage

echo Usage: weblogic ^<start^|stop^|ping^|install^> server_name domain_name

:end

ENDLOCAL