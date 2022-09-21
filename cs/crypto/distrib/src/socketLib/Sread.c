/* Sread.c: */
#include <stdio.h>
#include "sockets.h"

/* Sread: this function performs a read from a Socket */
#ifdef __PROTOTYPE__
int Sread(
  Socket *skt,		/* socket handle			*/
  void   *buf,		/* socket character buffer	*/
  int     buflen)	/* max length of buffer		*/
#else
int Sread(
  skt,				/* socket handle			*/
  buf,				/* socket character buffer	*/
  buflen)			/* max length of buffer		*/
Socket *skt;
void   *buf;
int     buflen;
#endif
{
int cnt;


/* sanity check */
if(!skt) {
	return -1;
	}

/* read bytes from Socket */
cnt  = recv(skt->skt,(void *) buf,(unsigned) buflen,0);

if(cnt > 0) {	/* "cnt" bytes received		*/
	return cnt;
	}

/* error return */
((char *) buf)[0]= '\0';
return 0;
}

/* --------------------------------------------------------------------- */
