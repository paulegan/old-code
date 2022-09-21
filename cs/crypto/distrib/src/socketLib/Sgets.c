/* Sgets.c
 *
 * Syntax :  Sgets(buf,maxbuf,skt)
 * Returns: buf if it worked, a NULL pointer otherwise
 *
 * buf[maxbuf]
 */
#include <stdio.h>
#include "sockets.h"

/* ---------------------------------------------------------------------------
 * Local Definitions:
 */

/* ---------------------------------------------------------------------------
 * Function Declarations:
 */

/* ---------------------------------------------------------------------------
 * Source Code:
 */

/* Sgets:
 * Gets a null-byte terminated string from the specified Socket.
 * Note that the maxbuf *must be* >= 2 (one byte needed for the
 * null byte termination, and one byte for a minimal amount of data).
 * This function blocks and will fill the buffer "buf" with up
 * to maxbuf characters.  The output from this function will
 * be null byte terminated when successful, even on a full buffer condition.
 *
 * Returns: buf     : on success
 *          null ptr: on failure
 */
#ifdef __PROTOTYPE__
char *Sgets(
  char *buf,
  int maxbuf,
  Socket *skt)
#else
char *Sgets(
  buf,
  maxbuf,
  skt)
char *buf;
int maxbuf;
Socket *skt;
#endif
{
int   buflen;
char *b=NULL;
char *nb=NULL;
int   mesglen;


/* error or whatever, until the buffer actually gets something its empty */
buf[0]= '\0';

/* sanity check */
if(!skt) {
	return (char *) NULL;
	}

#ifdef SSLNOPEEK	/* stupid, so it receives one byte at a time */
for(b= buf, buflen= maxbuf-1; buflen; --buflen, ++b) {
	if(recv(skt->skt,b,(unsigned) 1,0) <= 0) {
		return (char *) NULL;
		}
	if(*b == '\0') break;
	}

#else

/* receive bytes until a nullbyte is received */
buflen       = maxbuf - 2;	/* leave space for trailing null byte			*/
buf[buflen+1]= '\0';		/* so strchr will find a null byte somewhere	*/
for(b= buf, mesglen= 0; buflen > 0; b+= mesglen, buflen-= mesglen) {

	/* block until something shows up on the Socket */
	if(!Swait(skt)) {    /* null == error - should be at least 1 null byte */
		return (char *) NULL;
		}

	/* peek at contents of socket queue */
	mesglen= recv(skt->skt,b,buflen,MSG_PEEK);

	if(mesglen <= 0) {	/* socket error - bad receive */
		return (char *) NULL;
		}
	else if(mesglen > 0) { /* check if null byte received yet */
		nb= strchr(b,'\0');			/* find null byte in buffer				*/

		if(nb < b + mesglen) {		/* null byte inside peeked-at message	*/
			mesglen= nb - b + 1;	/* read up to and including null byte	*/
			if(Sreadbytes(skt,b,mesglen) <= 0) {
				return buf;
				}
			break;
			}
		else if(mesglen >= buflen) { /* whoops - full buffer				*/
			if(Sreadbytes(skt,b,buflen) <= 0) {
				return buf;
				}
			b[buflen]= '\0';		/* end-of-buffer termination			*/
			break;
			}
		else if(Sreadbytes(skt,b,mesglen) <= 0) {	/* partial read			*/
			return buf;
			}
		}
	}

#endif	/* #ifdef SSLNOPEEK ... #else ... #endif */

/* return message */
return buf;
}

/* --------------------------------------------------------------------- */
