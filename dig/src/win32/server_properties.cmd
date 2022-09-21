
@rem $Id$
@rem Properties file for IUR
@rem paul.x.egan.-nd@dig.com 20030624

set SERVER_NAME=iur
set SERVER_PORT=8001

@rem override domain classpath
set CLASSPATH=%BEA_HOME%;%BEA_HOME%\wlserver6.1\lib\weblogic.jar;%BEA_HOME%\wlserver6.1\lib\mssqlserver4v65.jar;%JAVA_HOME%\lib\tools.jar

@rem override default options
set JAVA_OPTIONS=-server -ms64m -mx128m

@rem add to system properties
set PROPERTIES=^
 -Dweblogic.Name="%SERVER_NAME%" ^
 -Dmy.prop=test ^
 %PROPERTIES%
