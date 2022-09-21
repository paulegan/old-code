
# paul@hr.emea.mot.com	20.10.1998

# Uses GNU make specific commands

output		= god.web
mainFiles	= ${wildcard *.html}
jsFiles		= ${wildcard js/*.js}
allFiles	= ${mainFiles} ${jsFiles}
JSC		= jsac

${output}: ${allFiles}
	${JSC} -o $@ $^

${allFiles}:
	echo $@
