/* Stest.c: this file contains code which allows one to test if data
 *  is present via the socket and not be blocked (not held, not made to sleep)
 *
 *  Returns: number of bytes of data awaiting perusal (which may be 0)
 *	         or EOF if unable to "select" the socket
 *                  or select returns 1 but nothing is on socket
 */
#include <stdio.h>
#define SSLNEEDTIME
#include "sockets.h"

/* ---------------------------------------------------------------------
 * Source Code:
 */

#ifdef __PROTOTYPE__
int Stest(Socket *skt)
#else
int Stest(skt)
Socket *skt;
#endif
{
fd_set rmask,wmask,emask;
short  result;
int  ret;
struct timeval timeout;
static char buf[PM_BIGBUF];


/* sanity check */
if(!skt) {
	return -1;
	}

FD_ZERO(&rmask);
FD_SET(skt->skt,&rmask);
FD_ZERO(&wmask);
FD_ZERO(&emask);

timeout.tv_sec = 0;
timeout.tv_usec= 0;

#ifdef SSLNOPEEK
result = select(skt->skt+1,
         rmask.fds_bits,wmask.fds_bits,emask.fds_bits,&timeout);
#else
result = select(skt->skt+1,&rmask,&wmask,&emask,&timeout);
#endif	/* SSLNOPEEK */

if(result < 0) {
	return EOF;
	}

/* server sockets return the select result (client awaiting connection) */
if(skt->type == PM_SERVER) {
	return result;
	}

#ifdef SSLNOPEEK

if(FD_ISSET(skt->skt,&rmask)) {
	return 1;
	}

#else	/* #ifdef SSLNOPEEK ... #else ... #endif */

/* test if message available from socket, return qty bytes avail */

if(FD_ISSET(skt->skt,&rmask)) {
	ret= recv(skt->skt,buf,PM_BIGBUF-1,MSG_PEEK);

	/* return error indication when select returned a result of 1
	 * (indicating that *something* is on the socket)
	 * and recv indicates that *nothing* is on the socket
	 */
	if(result == 1 && ret == 0) ret= EOF;
	return ret;
	}

#endif	/* #ifdef SSLNOPEEK ... #else ... #endif	*/

/* socket is empty */
return 0;
}

/* --------------------------------------------------------------------- */
