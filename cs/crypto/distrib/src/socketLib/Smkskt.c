/* Smkskt.c: this function makes a Socket */
#include <stdio.h>
#include "sockets.h"

/* --------------------------------------------------------------------- */

/* makeSocket: this function allocates a Socket */
#ifdef __PROTOTYPE__
Socket *makeSocket(
  char *hostname,
  char *sktname,
  int   sktType)
#else
Socket *makeSocket(hostname, sktname, sktType)
char *hostname;
char *sktname;
int   sktType;
#endif
{
Socket *skt;


/* allocate a Socket */
skt= (Socket *) malloc(sizeof(Socket));
outofmem((void *) skt,
  "makeSocket: while allocating a Socket for <%s:%s>\n",
  hostname,sktname);

/* initialize Socket */
skt->hostname= (char *) calloc((size_t) strlen(hostname)+1,sizeof(char));
outofmem((void *) skt->hostname,
  "makeSocket: while allocating string for <%s>\n",hostname);
strcpy(skt->hostname,hostname);

skt->sktname= (char *) calloc((size_t) strlen(sktname)+1,sizeof(char));
outofmem((void *) skt->sktname,
  "makeSocket: while allocating string for <%s>\n",sktname);
strcpy(skt->sktname,sktname);

/* set port and skt to zero */
skt->port= skt->skt= 0;
skt->type= sktType;

/* return Socket */
return skt;
}

/* ------------------------------------------------------------------------ */

/* freeSocket: this function frees up a Socket */
#ifdef __PROTOTYPE__
void freeSocket(Socket *skt)
#else
void freeSocket(skt)
Socket *skt;
#endif
{

if(skt) {
	if(skt->hostname) free(skt->hostname);
	if(skt->sktname)  free(skt->sktname);
	free((char *) skt);
	}

}

/* ------------------------------------------------------------------------ */
