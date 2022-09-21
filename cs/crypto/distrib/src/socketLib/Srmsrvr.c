/* Srmsrvr.c: this file contains Socket library support */
#include <stdio.h>
#include "xtdio.h"
#include "sockets.h"

/* -------------------------------------------------------------------------
 * Local Definitions:
 */
#define BUFSIZE 128
#define TIMEOUT  20L

/* ------------------------------------------------------------------------- */

/* Srmsrvr: this function removes a server from the PortMaster's table
 * It only needs a server name.
 *
 *  Returns:  PM_OK    if it worked
 *            PM_SORRY otherwise
 */
#ifdef __PROTOTYPE__
SKTEVENT Srmsrvr(char *skthost)
#else
SKTEVENT Srmsrvr(skthost)
char *skthost;
#endif
{
char       *at=NULL;
char       *hostname=NULL;
int         trycnt;
Socket     *pmskt=NULL;
static char localhost[BUFSIZE]="";
SKTEVENT    mesg;


/* parse skthost into sktname and hostname strings */
at= strchr(skthost,'@');
if(!at) {	/* fill in missing hostname with current host */
	if(!localhost[0]) gethostname(localhost,BUFSIZE);
	hostname= localhost;
	pmskt   = Sopen_clientport(hostname,"SktRmSrvr",PORTMASTER);
	}

else {	/* use hostname specified in skthost */
	skthost[(int) (at - skthost)]= '\0';
	hostname                     = skthost + ((int) (at - skthost)) + 1;
	pmskt= Sopen_clientport(hostname,"SktRmSrvr",PORTMASTER);
	}

/* --- PortMaster Interface --- */
if(!pmskt) {
	if(at) skthost[(int) (at - skthost)]= '@';
	return PM_SORRY;
	}

trycnt= 0;
do {
	mesg= htons(PM_RMSERVER);
	Swrite(pmskt,(char *) &mesg,sizeof(mesg));

	/* don't wait forever... */
	if(Stimeoutwait(pmskt,TIMEOUT,0L) < 0) {
		shutdown(pmskt->skt,2);
		close(pmskt->skt);
		freeSocket(pmskt);
		return PM_SORRY;
		}

	/* read PortMaster's response */
	if(Sreadbytes(pmskt,(char *) &mesg,sizeof(mesg)) <= 0) {
		if(at) skthost[(int) (at - skthost)]= '@';
		shutdown(pmskt->skt,2);
		close(pmskt->skt);
		return PM_SORRY;
		}
	mesg= ntohs(mesg);

	/* only allow PM_MAXTRY attempts to communicate with the PortMaster */
	if(++trycnt >= PM_MAXTRY) {
		if(at) skthost[(int) (at - skthost)]= '@';
		shutdown(pmskt->skt,2);
		close(pmskt->skt);
		return PM_SORRY;
		}
	} while(mesg != PM_OK);

Sputs(skthost,pmskt);

/* read PortMaster's response (should be PM_OK) */
Sreadbytes(pmskt,(char *) &mesg,sizeof(mesg));
shutdown(pmskt->skt,2);
close(pmskt->skt);

/* restore skthost string */
if(at) skthost[(int) (at - skthost)]= '@';

return mesg;
}

/* --------------------------------------------------------------------- */
