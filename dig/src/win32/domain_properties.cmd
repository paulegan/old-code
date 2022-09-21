
@rem $Id$
@rem Common properties for the test domain
@rem paul.x.egan.-nd@dig.com 20030624

set SERVER_HOST=app01

set DOMAIN_NAME=test
set DOMAIN_PASSWD=weblogic

set JAVA_HOME=C:\java
set BEA_HOME=C:\bea

set WL_HOME=C:\apps\%DOMAIN_NAME%\%SERVER_NAME%
set PATH=%JAVA_HOME%\bin;%BEA_HOME%\bin;%BEA_HOME%\wlserver6.1\bin;%PATH%

set CLASSPATH=%BEA_HOME%;%BEA_HOME%\wlserver6.1\lib\weblogic.jar;%BEA_HOME%\wlserver6.1\lib\mssqlserver4v65.jar;%JAVA_HOME%\lib\tools.jar

set JAVA_OPTIONS=-server -ms64m -mx64m

set PROPERTIES=^
  -Dweblogic.Domain=%DOMAIN_NAME% ^
  -Dweblogic.management.password=%DOMAIN_PASSWD% ^
  -Dweblogic.management.forceApplicationCopy=false ^
  -Dweblogic.management.discover=false ^
  -Dweblogic.ProductionModeEnabled=true ^
  -Djava.security.policy=%BEA_HOME%/wlserver6.1/lib/weblogic.policy
