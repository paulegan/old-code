#include <stdio.h>
#include <ctype.h>
#include "xtdio.h"


/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */

/* stpnxt: returns a pointer to the next argument in the given string,
 *	assuming a %d, %s, %f, etc. code was used to interpret the
 *	present argument.
 *  s  : source string
 *  fmt: control string (ie. format control codes go here)
 */
#ifdef __PROTOTYPE__
char *stpnxt(
  char *s,
  char *fmt)
#else	/* __PROTOTYPE__ */
char *stpnxt(s,fmt)
char *s,*fmt;
#endif	/* __PROTOTYPE__ */
{
char *ss;
int cnt,cmax;
#define MAXCNT 65535


/* if s is NULL return NULL */
if(s == NULL) {
	return (char *) NULL;
	}

/* process control string */
for(; *s && *fmt ; ++fmt) {

	ss= s;

	/* scan code processing */
	if(*fmt == '%') {
		++fmt;

		/* %[-]###.: handle predecessor stuff */
		if(*fmt == '-') ++fmt;	
		if(*fmt && isdigit(*fmt)) {
			sscanf(fmt,"%d",&cmax);
			while(*fmt && isdigit(*fmt)) ++fmt;
			}
		else cmax= MAXCNT;

		/* %...l.: remove the 'l' from consideration */
		if(*fmt == 'l') ++fmt;

		/* handle the format code character */
		switch(*fmt) {

		/* %s: string processing */
		case 's':
			while(*s && isspace(*s)) ++s;
			while(*s && !isspace(*s)) ++s;
			break;

		/* %c: character processing */
		case 'c':
			if(*s) ++s;
			break;

		/* %x: hexadecimal processing */
		case 'x':
			while(*s && isspace(*s)) ++s;
			ss= s;
			if(*s == '+' || *s == '-') ++s;
			while(*s && (isdigit(*s) || (*s >= 'a' && *s <= 'f') ||
			  (*s >= 'A' && *s <= 'F'))) ++s;
			break;

		/* %o: octal processing */
		case 'o':
			while(*s && isspace(*s)) ++s;
			ss= s;
			if(*s == '+' || *s == '-') ++s;
			while(*s && *s >= '0' && *s <= '7') ++s;
			break;

		/* %d, %e, %f, %g: integer and floating point number processing */
		case 'd':
		case 'e':
		case 'f':
		case 'g':
			while(*s && isspace(*s)) ++s;
			ss= s;
			if(*s == '-' || *s == '+') ++s;
			while(*s && isdigit(*s)) ++s;
			if(*fmt == 'd') break;
			if(*s == '.') ++s;
			while(*s && isdigit(*s)) ++s;
			if(*s == 'e') { /* exponent handling */
				++s;
				if(*s == '-' || *s == '+') ++s;
				while(isdigit(*s) && *s) ++s;
				}
			break;

		/* %%: single character processing */
		case '%':
			while(*s && isspace(*s)) ++s;
			if(*s != *fmt) {
				return s;
				}
			break;

		/* %b: slight extension, skips over blanks */
		case 'b':
			while(*s && isspace(*s)) ++s;
			break;

		/* unknown */
		default:
			error(WARNING,"unknown stpnxt code <%s%c%s>\n",MAGENTA,*fmt,GREEN);
			break;
			}
		}

	/* white space in control string skipping */
	else if(*fmt && isspace(*fmt)) continue;

	/* non-white space: must match string */
	else {
		while(*s && isspace(*s)) ++s;
		if(*s != *fmt) {
			return s;
			}
		else {	/* s and c match, so continue forth */
			++s;
			continue;
			}
		}
	if(s <= ss) {
		return ss;
		}
	}	/* c: loop */

return s;
}
