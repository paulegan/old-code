/* Just put in another line */
/* Saccept.c: this file contains the Socket accept support */
#include <stdio.h>
#include <errno.h>
#include "sockets.h"

/* ------------------------------------------------------------------------- */

/* Saccept: this routine uses a server Socket to accept connections
 *  The accept() function clones a socket for use with a client connect.
 *  One may close the Saccept generated socket without affecting the
 *  server socket.
 */
#ifdef __PROTOTYPE__
Socket *Saccept(Socket *skt)
#else
Socket *Saccept(skt)
Socket *skt;
#endif
{
int             addrlen;
int             status=1;
struct sockaddr addr;
Socket         *acceptskt= NULL;


/* sanity check */
if(!skt) {
	return acceptskt;
	}

/* allocate a Socket */
acceptskt= makeSocket(skt->hostname,skt->sktname,PM_ACCEPT);
if(!acceptskt) {
	return acceptskt;
	}

/* accept a connection */
addrlen       = sizeof (addr);
acceptskt->skt= accept(skt->skt, &addr, &addrlen);
if(acceptskt->skt <= 0) {	/* failure to accept */

	freeSocket(acceptskt);


	return (Socket *) NULL;
	}

/* turn off TCP's buffering algorithm so small packets don't get delayed */
#ifndef SSLNOSETSOCKOPT


if(setsockopt(skt->skt,IPPROTO_TCP,TCP_NODELAY,(char *) &status,sizeof(status))
 < 0) {
	}
#endif	/* #ifndef SSLNOSETSOCKOPT ... */

return acceptskt;
}

/* --------------------------------------------------------------------- */
