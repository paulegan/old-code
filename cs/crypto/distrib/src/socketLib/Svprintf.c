/* Svprintf.c: */
#include <stdio.h>
#include "sockets.h"

/* --------------------------------------------------------------------- */

/* Svprintf:
 *
 *   Returns: count of bytes sent, which may be 0
 *
 *   This function is like vprintf; it takes a Socket pointer, a format
 *   string, and an argument list.  Note that it actually "prints" the
 *   string into a local buffer first of PM_BIGBUF bytes (originally 1024
 *   bytes).  So, please insure that you don't put more info than will fit
 *   into PM_BIGBUF bytes!  It then squirts the resulting string through
 *   a call to Swrite.
 */
#ifdef __PROTOTYPE__
int Svprintf(
  Socket *skt,
  char   *fmt,
  void   *args)
#else
int Svprintf(skt,fmt,args)
Socket *skt;
char   *fmt;
void   *args;
#endif
{
int ret;
static char buf[PM_BIGBUF];


/* sanity check */
if(!skt) {
	return 0;
	}

ret= vsprintf(buf,fmt,(char *) args);
Swrite(skt,buf,strlen(buf)+1);	/* send the null byte, too */

return ret;
}

/* --------------------------------------------------------------------- */
