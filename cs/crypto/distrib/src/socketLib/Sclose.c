/* Sclose.c: this file contains Socket library support */
#include <stdio.h>
#include "sockets.h"

#ifdef unix
#include <unistd.h>
#endif

/* -------------------------------------------------------------------------
 * Local Definitions
 */
#define TIMEOUT 20L

/* ------------------------------------------------------------------------- */

/* Sclose: this function closes a socket.  Closing a socket is easy, but
 * the PortMaster also needs to be informed
 */
#ifdef __PROTOTYPE__
void Sclose(Socket *skt)
#else
void Sclose(skt)
Socket *skt;
#endif
{
int      trycnt;
Socket  *pmskt = NULL;
SKTEVENT mesg;
SKTEVENT port;


if(!skt) { /* return immediately */
	return;
	}

/* --- PortMaster Interface --- */
if(skt->type == PM_SERVER) {	/* Servers Only */
	pmskt = Sopen_clientport(skt->hostname,"SktClose",PORTMASTER);
	if(!pmskt) {
		}
	
	trycnt= 0;
	do {
		mesg= htons(PM_CLOSE);
		Swrite(pmskt,(char *) &mesg,sizeof(mesg));
		if(Stimeoutwait(pmskt,TIMEOUT,0L) < 0) goto ShutDown;
		if(Sreadbytes(pmskt,(char *) &mesg,sizeof(mesg)) <= 0) {
			shutdown(skt->skt,2);
			close(skt->skt);
			return;
			}
		mesg= ntohs(mesg);
	
		/* only allow PM_MAXTRY attempts to communicate with the PortMaster */
		if(++trycnt >= PM_MAXTRY) {
			shutdown(pmskt->skt,2);
			close(pmskt->skt);
			shutdown(skt->skt,2);
			close(skt->skt);
			return;
			}
		} while(mesg != PM_OK);
	
	port= htons(skt->port);
	Swrite(pmskt,(char *) &port,sizeof(port));
	
	/* read PortMaster's response (should be PM_OK) */
	if(Stimeoutwait(pmskt,TIMEOUT,0L) < 0) goto ShutDown;
	Sreadbytes(pmskt,(char *) &mesg,sizeof(mesg));
	mesg= ntohs(mesg);
	shutdown(pmskt->skt,2);
	close(pmskt->skt);
	}	/* end of PortMaster Interface */

else {	/* Handle non-server sockets (clients, accepts) */
	mesg= PM_OK;
	}

/* --- Close the Socket --- */
ShutDown:
shutdown(skt->skt,2);
close(skt->skt);
freeSocket(skt);

}

/* --------------------------------------------------------------------- */
