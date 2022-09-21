/* Sopen.c: these functions are responsible for opening Sockets
 *  Routines Include: <Sopen.c>
 *    Socket *Sopen( char *skthost, char *mode)
 *    Socket *Sopen_clientport( char *hostname, char *sktname, u_short port)
 *    static Socket *Sopen_server(char *sktname)
 *    static Socket *Sopen_client( char *hostname, char *sktname)
 */
#include <stdio.h>
#include <ctype.h>
#include "sockets.h"

/* ----------------------------------------------------------------------
 * Local Definitions:
 */
#define BUFSIZE 128
#define TIMEOUT  20L

/* ---------------------------------------------------------------------
 * Local Data:
 */
static unsigned long sslport= INADDR_ANY;

/* ----------------------------------------------------------------------
 * Local Prototypes:
 */
#ifdef __PROTOTYPE__
static Socket *Sopen_server(char *sktname);       /* Sopen.c              */
static Socket *Sopen_client( char *hostname,      /* Sopen.c              */
   char *sktname);
#else
static Socket *Sopen_server();                    /* Sopen.c              */
static Socket *Sopen_client();                    /* Sopen.c              */
#endif

/* ---------------------------------------------------------------------- */

/* Sopen:
 * prochost: SocketName@Hostname
 * mode    : selects type of socket desired
 *	         s=server - one must use Saccept(skt) to receive connects
 *	         S=server - like s, but will Srmsrvr() if first attempt fails
 *	         c=client - one has a valid r/w connection to a server
 */
#ifdef __PROTOTYPE__
Socket *Sopen(
  char *skthost,
  char *mode)
#else
Socket *Sopen(
  skthost,
  mode)
char *skthost;
char *mode;
#endif
{
char       *at                 = NULL;
char       *hostname           = NULL;
Socket     *ret                = NULL;
static char localhost[BUFSIZE] = "";
static char sktname[BUFSIZE];

#ifdef MSDOS
static int first=1;
#endif


#ifdef MSDOS
/* initialize WIN/API DOS Socket library */
if(first) {
	first= 0;
	if(socket_init()) error(ERROR,"socket_init() returned -1, cannot start\n");
	}
#endif

/* by default, sslport is always INADDR_ANY *unless* overridden in the mode */
sslport= INADDR_ANY;
if(isdigit(mode[1])) {
	sscanf(mode+1,"%lu",&sslport);
	}

/* call mode-selected server/client socket opening routine */
switch(*mode) {

case 's':	/* server */
case 'S':	/* use Srmsrvr when Sopen_server itself fails */
#ifdef SSLNEEDTOSHAREPM
	if(!getenv("PMSHARE")) {
		error(WARNING,"this machine must share a PortMaster (set PMSHARE, please)\n");
		ret= NULL;
		return ret;
		}
#endif	/* SSLNEEDTOSHAREPM: this machine needs to another's PortMaster */


	ret= Sopen_server(skthost);
	if(!ret && *mode == 'S') {
		Srmsrvr(skthost);
		ret= Sopen_server(skthost);
		}

	sslport= INADDR_ANY;	/* paranoia time -- insure that sslport will default to INADDR_ANY */
	return ret;

case 'c':	/* client */

	/* parse sktname into sktname and hostname strings */
	at= strchr(skthost,'@');
	if(!at) {	/* fill in missing hostname with current host */
		if(!localhost[0]) { /* get local host name */
			gethostname(localhost,BUFSIZE);
			}
		hostname= localhost;
		strcpy(sktname,skthost);
		}
	else {	/* use hostname specified in skthost */
		strcpy(sktname,skthost);
		sktname[(int) (at - skthost)]= '\0';
		hostname                     = sktname + ((int) (at - skthost)) + 1;
		}

	/* note: Sopen_client will free up hostname/sktname memory if unsuccessful
	 * open occurs
	 */
	ret    = Sopen_client(hostname,sktname);
	sslport= INADDR_ANY;	/* paranoia time -- insure that sslport will default to INADDR_ANY */
	return ret;

default:
	sslport= INADDR_ANY;	/* paranoia time -- insure that sslport will default to INADDR_ANY */
	return (Socket *) NULL;
	}
}

/* ---------------------------------------------------------------------- */

/* Sopen_server: this function opens a socket handle for the SSL socket
 *   function library
 */
#ifdef __PROTOTYPE__
static Socket *Sopen_server(char *sktname)
#else
static Socket *Sopen_server(sktname)
char *sktname;
#endif
{
char              *at     = NULL;
char              *pmshare= NULL;
int                length;
int                resend = 0;
int                status = 1;
Socket            *skt    = NULL;
Socket            *pmskt  = NULL;
SKTEVENT           mesg;
SKTEVENT           port;
struct sockaddr_in sin;	/* InterNet socket address structure */
static char        hostname[BUFSIZE];


/* check for no "@" in hostname */
at= strchr(sktname,'@');
if(at) *at= '\0';

/* determine if this Sopen is to share a PortMaster, and if so, what
 * host it resides at
 */
pmshare= getenv("PMSHARE");

/* make a Socket */
gethostname(hostname,BUFSIZE);
skt=  makeSocket(pmshare? pmshare : hostname,sktname,PM_SERVER);
if(!skt)  {
	return  (Socket *) NULL;
	}

/* create a socket */
if((skt->skt=  socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	freeSocket(skt);
	return (Socket *) NULL;
	}

#ifndef SSLNOSETSOCKOPT
if(sslport) {	/* allow fixed-port servers to be brought back up quickly */
	status= 1;
	if(setsockopt(skt->skt,SOL_SOCKET,SO_REUSEADDR,(char *) &status,sizeof(status)) < 0) {
	    }
	}
#endif	/* SSLNOSETSOCKOPT */

/*	initialize socket data structure
 *  get port specified by sslport, defaults to any
 */
sin.sin_family      = AF_INET;
sin.sin_addr.s_addr = htonl(INADDR_ANY);
sin.sin_port        = htonl(sslport);

/*	bind socket data structure to this socket     */
if(bind(skt->skt,(struct sockaddr *) &sin,sizeof(sin))) {
	freeSocket(skt);
	return (Socket *) NULL;
	}

/* getsockname fills in the socket structure with information,
 * such as the port number assigned to this socket
 */
length = sizeof(sin);
if(getsockname(skt->skt,(struct sockaddr *) &sin,&length)) {
	freeSocket(skt);
	return (Socket *) NULL;
	}
skt->port= ntohs(sin.sin_port);

/* --- PortMaster Interface --- */


/* Inform PortMaster of socket */
pmskt= Sopen_clientport(pmshare? pmshare : hostname,"PMClient",PORTMASTER);
if(!pmskt) {
	shutdown(skt->skt,2);
	close(skt->skt);
	freeSocket(skt);
	return (Socket *) NULL;
	}

/* keep trying until PortMaster sends PM_OK */
do {

	/* write a PM_SERVER request to the host's PortMaster */
	if(pmshare) {
		mesg= htons(PM_SHARE);
		}
	else {
		mesg= htons(PM_SERVER);
		}
	Swrite(pmskt,(char *) &mesg,sizeof(mesg));

	/* insure that the PortMaster is talking for up to TIMEOUT seconds */
	if(Stimeoutwait(pmskt,TIMEOUT,0L) < 0) {
		shutdown(pmskt->skt,2);
		close(pmskt->skt);
		freeSocket(pmskt);
		shutdown(skt->skt,2);
		close(skt->skt);
		freeSocket(skt);
		return (Socket *) NULL;
		}

	/* ok, read PortMaster's response */
	if(Sreadbytes(pmskt,(char *) &mesg,sizeof(mesg)) <= 0) {
		perror("");
		shutdown(skt->skt,2);
		close(skt->skt);
		freeSocket(skt);
		return (Socket *) NULL;
		}
	mesg= ntohs(mesg); /* convert to host form */

	/* only try to open a PM_SERVER protocol PM_MAXTRY times -or-
	 * if PortMaster says "PM_SORRY" (probably due to firewall violation)
	 */
	if(++resend >= PM_MAXTRY || mesg == PM_SORRY) {
		shutdown(pmskt->skt,2);
		close(pmskt->skt);
		freeSocket(pmskt);
		shutdown(skt->skt,2);
#ifdef SSLSKTZERO
		close(skt->skt); skt->skt= 0;
#else
		close(skt->skt); skt->skt= NULL;
#endif
		freeSocket(skt);
		return (Socket *) NULL;
		}
	} while(mesg != PM_OK);

if(pmshare) {
	/* send PortMaster the hostname for shared PortMaster mode */
	Sputs(hostname,pmskt);
	}

/* send PortMaster the sktname and port id */
Sputs(sktname,pmskt);

port= sin.sin_port;	/* network form of port already */
Swrite(pmskt,(char *) &port,sizeof(port));

/* find out if PortMaster received ok */
if(Stimeoutwait(pmskt,TIMEOUT,0L) < 0) {
	shutdown(pmskt->skt,2);
	close(pmskt->skt);
	freeSocket(pmskt);
	shutdown(skt->skt,2);
	close(skt->skt);
	freeSocket(skt);
	return (Socket *) NULL;
	}
Sreadbytes(pmskt,(char *) &mesg,sizeof(mesg));

mesg= ntohs(mesg);
if(mesg != PM_OK) {
	shutdown(pmskt->skt,2);
	close(pmskt->skt);
	freeSocket(pmskt);
	shutdown(skt->skt,2);
	close(skt->skt);
	freeSocket(skt);
	return (Socket *) NULL;
	}
shutdown(pmskt->skt,2);
close(pmskt->skt);
#ifdef SSLSKTZERO
pmskt->skt= 0;
#else
pmskt->skt= NULL;
#endif

/* --- Return to Server Initialization --- */

/* prepare socket queue for (up to) PM_MAXREQUESTS connection
 * requests
 */
listen(skt->skt,PM_MAXREQUESTS);

#ifndef SSLNOSETSOCKOPT
/* turn off TCP's buffering algorithm so small packets don't get
 * needlessly delayed
 */
if(setsockopt(skt->skt,IPPROTO_TCP,TCP_NODELAY,(char *) &status,sizeof(status)) < 0) {
	}
#endif	/* SSLNOSETSOCKOPT */

/* return server socket */
return skt;
}

/* ---------------------------------------------------------------------- */

/* Sopen_client: */
#ifdef __PROTOTYPE__
static Socket *Sopen_client(
  char *hostname,
  char *sktname)
#else
static Socket *Sopen_client(
  hostname,
  sktname)
char *hostname;
char *sktname;
#endif
{
char      *pmshare= NULL;		/* PortMaster hostname					*/
char       skthost[BUFSIZE];	/* client's hostname					*/
int        resend= 0;
Socket    *skt   = NULL;
SKTEVENT   port;
SKTEVENT   mesg;


skthost[0]= '\0';

/* --- PortMaster Interface --- */
if(sslport == (unsigned long) INADDR_ANY) {

	/* determine if this Sopen is to share a PortMaster, and if so, what
	 * host it resides at
	 */
	pmshare= getenv("PMSHARE");
	
	/* open a socket to the target host's PortMaster */
	skt= Sopen_clientport(pmshare? pmshare : hostname,sktname,PORTMASTER);
	if(!skt) {
		return (Socket *) NULL;
		}
	
	/* go through PortMaster client protocol to get port */
	do {
		mesg= htons(PM_CLIENT);
		Swrite(skt,(char *) &mesg,sizeof(mesg));
	
	
		/* get PM_OK or PM_RESEND from PortMaster */
		if(Stimeoutwait(skt,TIMEOUT,0L) < 0) {
			shutdown(skt->skt,2);
			close(skt->skt);
			freeSocket(skt);
			return (Socket *) NULL;
			}
		Sreadbytes(skt,(char *) &mesg,sizeof(mesg));
		mesg= ntohs(mesg);
	
	
		/* too many retries or PM_SORRY
		 * (latter usually due to firewall violation)
		 */
		if(++resend >= PM_MAXTRY || mesg == PM_SORRY) {
			shutdown(skt->skt,2);
			close(skt->skt);
			freeSocket(skt);
			return (Socket *) NULL;
			}
		} while(mesg != PM_OK);
	
	/* send sktname of desired port */
	Sputs(sktname,skt);
	
	/* get response (PM_OK, PM_OKSHARE, or PM_SORRY) */
	if(Stimeoutwait(skt,TIMEOUT,0L) < 0) {
		shutdown(skt->skt,2);
		close(skt->skt);
		freeSocket(skt);
		return (Socket *) NULL;
		}
	Sreadbytes(skt,(char *) &mesg,sizeof(mesg));
	mesg= ntohs(mesg);
	
	if(mesg == PM_OKSHARE) {
		if(Stimeoutwait(skt,TIMEOUT,0L) < 0) {
			shutdown(skt->skt,2);
			close(skt->skt);
			freeSocket(skt);
			return (Socket *) NULL;
			}
		Sgets(skthost,BUFSIZE,skt);
		}
	else if(mesg != PM_OK) {
		shutdown(skt->skt,2);
		close(skt->skt);
		freeSocket(skt);
		return (Socket *) NULL;
		}
	else skthost[0]= '\0';
	
	/* get response (server's port #) */
	if(Stimeoutwait(skt,TIMEOUT,0L) < 0) {
		shutdown(skt->skt,2);
		close(skt->skt);
		freeSocket(skt);
		return (Socket *) NULL;
		}
	Sreadbytes(skt,(char *) &mesg,sizeof(mesg));
	port= ntohs(mesg);
	
	/* close the PortMaster client socket */
	shutdown(skt->skt,2);
	close(skt->skt);
	freeSocket(skt);
	}
else {
	port= sslport;
	}

/* --- Return to Client Initialization --- */

/* open a socket to the port returned by the target host's PortMaster */
skt= Sopen_clientport(skthost[0]? skthost : hostname,sktname,port);
if(!skt) {
	return (Socket *) NULL;
	}

return skt;
}

/* ---------------------------------------------------------------------- */

/* Sopen_clientport: this function opens a client socket given a hostname
 * and port
 */
#ifdef __PROTOTYPE__
Socket *Sopen_clientport(
  char   *hostname,
  char   *sktname,
  u_short port)
#else
Socket *Sopen_clientport(
	hostname,
	sktname,
	port)
char *hostname;
char *sktname;
u_short port;
#endif
{
Socket            *skt        = NULL;
int                status     = 1;
static char        localhost[BUFSIZE];
struct hostent    *hostentptr = NULL;
struct sockaddr_in sin;				/* InterNet socket address structure */


/* if hostname is null, then use current hostname */
if(!hostname || !hostname[0]) {
	gethostname(localhost,BUFSIZE);
	hostname= localhost;
	}

/* allocate a Socket */
skt= makeSocket(hostname,sktname,PM_CLIENT);
if(!skt) {
	return (Socket *) NULL;
	}
skt->port= port;

/*	open socket */

if((skt->skt= socket(AF_INET,SOCK_STREAM,0)) < 0) {
	freeSocket(skt);
	return (Socket *) NULL;
	}

/*	initialize socket data structure */
sin.sin_family = AF_INET;
hostentptr     = gethostbyname(hostname);


if(!hostentptr) {
	freeSocket(skt);
	return (Socket *) NULL;
	}

/* get  host address */
sin.sin_addr = * ((struct in_addr *) hostentptr->h_addr);
sin.sin_port = htons((u_short) port);

/* send port number to server */

/* connect to remote host */
if(connect(skt->skt,(struct sockaddr *) &sin,sizeof(sin)) < 0){
	shutdown(skt->skt,2);
	close(skt->skt);
	freeSocket(skt);
	return (Socket *) NULL;
	}

#ifndef SSLNOSETSOCKOPT
/* turn off TCP's buffering algorithm so small packets don't get
 * needlessly delayed
 */

if(setsockopt(skt->skt,IPPROTO_TCP,TCP_NODELAY,(char *) &status,sizeof(status))
  < 0) {
	}
#endif	/* SSLNOSETSOCKOPT */

return skt;
}

/* --------------------------------------------------------------------- */
