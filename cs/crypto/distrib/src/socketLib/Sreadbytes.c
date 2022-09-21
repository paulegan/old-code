/* Sreadbytes.c: */
#include <stdio.h>
#include "sockets.h"

/* --------------------------------------------------------------------- */

/* Sreadbytes: this function performs a socket read.
 *  Gets buflen bytes, so long as read is working.
 *  Otherwise, (read fails with a -1), the number
 *  of bytes received will be returned.
 */
#ifdef __PROTOTYPE__
int Sreadbytes(
  Socket *skt,		/* socket handle			*/
  void   *buf,		/* socket character buffer	*/
  int     buflen)	/* max length of buffer		*/
#else
int Sreadbytes(
  skt,				/* socket handle			*/
  buf,				/* socket character buffer	*/
  buflen)			/* max length of buffer		*/
Socket *skt;
void   *buf;
int     buflen;
#endif
{
int cnt;
int rcnt;


/* sanity check */
if(!skt) {
	return -1;
	}

/* get buflen bytes, no matter the wait */
for(cnt= 0; cnt < buflen; cnt+= rcnt) {
	rcnt= recv(skt->skt,(void *) (((char *)buf)+cnt),(unsigned) (buflen-cnt),0);
	if(rcnt < 0) break;
	}

return cnt;
}

/* --------------------------------------------------------------------- */
