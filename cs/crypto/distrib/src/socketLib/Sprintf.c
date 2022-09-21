/* Sprintf.c: */
#include <stdio.h>
#include "sockets.h"

/* Sprintf: */
/*VARARGS*/
#ifdef __PROTOTYPE__
int Sprintf(
  Socket *skt,
  char *fmt,
   ...)
#else
int Sprintf(skt,va_alist)
Socket *skt;
va_dcl
#endif
{
va_list args;
int ret;
static char buf[PM_BIGBUF];

#ifndef __PROTOTYPE__
char *fmt=NULL;
#endif


/* sanity check */
if(!skt) {
	return 0;
	}

#ifdef __PROTOTYPE__
va_start(args,fmt);
#else
va_start(args);
fmt = va_arg(args,char *);
#endif
fflush(stdout);

ret= vsprintf(buf,fmt,args);
Swrite(skt,buf,strlen(buf)+1);	/* send the null byte, too */
va_end(args);

return ret;
}

/* --------------------------------------------------------------------- */
