
# paul@hr.emea.mot.com	19990824

# Uses GNU make specific commands

project		= coap
output		= ${project}.web
mainFiles	= ${wildcard *.html}
jsFiles		= ${wildcard js/*.js}
allFiles	= ${mainFiles} ${jsFiles}
JSC		= /www/suitespot/bin/https/jsac
restartURL	= "http://localhost:8000/appmgr/control.html?name=${project}&cmd=restart"
appmgrID	= "local:appmgr"

${output}: ${allFiles}
	${JSC} -o $@ $^
	lwp-request -PC ${appmgrID} ${restartURL} > /dev/null

${allFiles}:
	echo $@
