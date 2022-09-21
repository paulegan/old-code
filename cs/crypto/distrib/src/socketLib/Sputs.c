/* Sputs.c: this function "puts" a string which Sgets can receive */
#include <stdio.h>
#include "sockets.h"

/* --------------------------------------------------------------------- */

/* Sputs: */
#ifdef __PROTOTYPE__
void Sputs(
  char   *buf,
  Socket *skt)
#else
void Sputs(
  buf,
  skt)
char   *buf;
Socket *skt;
#endif
{

/* write out buf and the null byte */
Swrite(skt,buf,strlen(buf)+1);

}

/* --------------------------------------------------------------------- */
