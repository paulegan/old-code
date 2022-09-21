/* Spm: The PortMaster
 *	This program is expected to run as a daemon on every machine
 *  which uses Terry McRoberts' Socket Library.
 *
 *  Its purpose in life is to:
 *   a) receive messages from processes starting up servers to maintain
 *	    a list of process names and associated ports,
 *   b) to inform processes starting up client sockets about the port
 *	    numbers of designed servers.
 *
 *  This program's services are generally hidden from the users, and is
 *  accessed by the Sopen() functions.  Since processes from anywhere
 *	need to access this process, it has a fixed port number (PORTMASTER)
 *	unless overridden from the command line.  This information is hard-coded
 *	into the <sockets.h> header file.
 *
 *  Routines Include: <Spm.c>
 *    int main( int argc, char **argv)
 *    static Socket *Sopen_prtmstr()
 *    static PortTable *makePortTable(SKTEVENT port,char *sharehost)
 *    static void delPortTable(PortTable *p)
 *    void pm_server(Socket *client)
 *    void pm_client(Socket *client)
 *    void pm_close(Socket *client)
 *    void pm_quit( Socket *server, Socket *client)
 *    void pm_table(Socket *client)
 *    void pm_rmserver(Socket *client)
 *    int firewall(Socket *skt)
 *    void firewall_init(char *fwfile)
 *    void pm_fwinit(Socket *skt)
 */
#include <stdio.h>
#include "sockets.h"
#include <setjmp.h>

/* ----------------------------------------------------------------------
 * Definitions:
 */
#define SPMFIREWALL		/* select PortMaster firewall code				*/

#define BUFSIZE		128	/* size of character buffers					*/
#define SPMFWDATA	6	/* SSL PortMaster Firewall data quantity		*/
#define TIMEOUT		20L	/* wait no more than 20 seconds at a Sread...	*/

/* ----------------------------------------------------------------------
 * Typedefs:
 */
#ifdef SPMFIREWALL
typedef struct spmfw_str    SpmFW;
#endif
typedef struct PortTablestr PortTable;

/* ----------------------------------------------------------------------
 * Data Structures:
 */

/* PortTable: this data structure supports matching host/process name
 *  to ports
 */
struct PortTablestr {
	char      *sktname;		/* name of skt							*/
	char      *sharehost;	/* optional Portmaster-sharing hostname	*/
	SKTEVENT   port;		/* port of socket						*/
	PortTable *nxt,*prv;	/* linked list							*/
	};

#ifdef SPMFIREWALL
struct spmfw_str {
	int    data[SPMFWDATA];
	SpmFW *nxt;
	};
#endif

/* ----------------------------------------------------------------------
 * Global Data:
 */
static struct sockaddr_in sin;
static char       hostname[BUFSIZE];
static char       hostaddr[4];
static char       sktname[BUFSIZE];
static SKTEVENT   ok     = 0;
static SKTEVENT   sorry  = 0;
static SKTEVENT   resend = 0;
static SKTEVENT   okshare= 0;
static PortTable *phd    = NULL;
static PortTable *ptl    = NULL;
static jmp_buf    timedout;

#ifdef SPMFIREWALL
static SpmFW     *spmfwhd=NULL;
#endif

/* ----------------------------------------------------------------------
 * Usage:
 */
static char *usage[]={
	XYELLOW,"Spm ",XGREEN,"[",XCYAN,"-f firewallfilename",XGREEN,"]\n\n",
	
	XUWHITE,"Author",XGREEN,": Dr. Charles E. Campbell, Jr.\n\n",

	XUWHITE,"Options:\n\n",

	XCYAN,"  -f firewallfilename",XGREEN,": optional firewall file\n",
	
	XUWHITE,"Explanation:\n\n",XGREEN,
	
	"The ",XYELLOW,"Spm",XGREEN," is the PortMaster for the Sockets library\n",
	"written by Dr. Charles E. Campbell, Jr. and Terry McRoberts of NASA.\n",
	"The Spm program provides a \"phonebook\" of server names and their\n",
	"associated port identifiers used by the Sopen function.  The data\n",
	"is maintained in an internal \"PortTable\" linked list in memory.\n",
	"The PortMaster (Spm) must be run on any machine using the Sockets\n",
	"library.\n\n",
	
	"The PortMaster is not intended to be used directly by the user.\n",
	"Instead, Sopen, Sclose, and Srmsrvr are the primary interfaces to the\n",
	"PortMaster for the user.  These functions follow a simple protocol and\n",
	"pass PortMaster \"event\"s to and from the PortMaster.\n\n",
	
	"There are some additional functions that the PortMaster provides: one\n",
	"may use PortMaster events to close down the PortMaster (PM_QUIT) and\n",
	"to query the PortMaster as to what servers are available (PM_TABLE).\n",
	"These functions do not have nice interfaces provided; one must be\n",
	"conversant with the PortMaster protocol to get them to work.\n\n",
	
	"",""};

/* ----------------------------------------------------------------------
 * Prototypes:
 */
#ifdef __PROTOTYPE__

int main( int, char **);                               /* Spm.c           */
static Socket *Sopen_prtmstr(void);                    /* Spm.c           */
static PortTable *makePortTable(SKTEVENT,char *);      /* Spm.c           */
static void delPortTable(PortTable *);                 /* Spm.c           */
void pm_server(Socket *,SKTEVENT);                     /* Spm.c           */
void pm_client(Socket *);                              /* Spm.c           */
void pm_close(Socket *);                               /* Spm.c           */
void pm_quit( Socket *, Socket *);                     /* Spm.c           */
void pm_table(Socket *);                               /* Spm.c           */
void pm_rmserver(Socket *);                            /* Spm.c           */

#ifdef SPMFIREWALL
int firewall(Socket *);                                /* Spm.c           */
void firewall_init(char *);                            /* Spm.c           */
void pm_fwinit(Socket *);                              /* Spm.c           */
#endif

#else

extern int main();                                     /* Spm.c           */
static Socket *Sopen_prtmstr();                        /* Spm.c           */
static PortTable *makePortTable();                     /* Spm.c           */
static void delPortTable();                            /* Spm.c           */
extern void pm_server();                               /* Spm.c           */
extern void pm_client();                               /* Spm.c           */
extern void pm_close();                                /* Spm.c           */
extern void pm_quit();                                 /* Spm.c           */
extern void pm_table();                                /* Spm.c           */
extern void pm_rmserver();                             /* Spm.c           */

#ifdef SPMFIREWALL
extern int firewall();                                 /* Spm.c           */
extern void firewall_init();                           /* Spm.c           */
extern void pm_fwinit();                               /* Spm.c           */
#endif

#endif	/* __PROTOTYPE__ */

/* ---------------------------------------------------------------------- */

/* main: this is where it starts, ladies and gentlemen! */
#ifdef __PROTOTYPE__
int main(
  int    argc,
  char **argv)
#else
int main(
  argc,
  argv)
int    argc;
char **argv;
#endif
{
int             resend_cnt=0;
struct hostent *he=NULL;
SKTEVENT        event;
Socket         *client=NULL;
Socket         *server=NULL;

#ifdef unix
int daemon=0;
int tty_fd;

daemon= fork();
if(daemon < 0) error(ERROR,"Spm: PortMaster cannot fork");
else if(daemon) {	/* we're the parent, so bye-bye! */
#ifdef vms
	exit(1);
#else
	exit(0);
#endif
	}
#endif	/* #ifdef unix ...		*/

/* read color strings */
rdcolor();


/* give out credits */
fprintf(stderr,"\n    *** Simple Sockets PortMaster ***\n");
fprintf(stderr,"Authors: Charles E. Campbell, Jr., Ph.D. and\n");
fprintf(stderr,"         Terry McRoberts\n");
fprintf(stderr,"of Goddard Space Flight Center, Maryland, USA\n");

/* handle command line */
if(argc > 1 && !strcmp(argv[1],"?")) {	/* give usage message */
	char **u;

	for(u= usage; u[0][0] || u[1][0]; ++u) rdcputs(*u,stdout);
	exit(0);
	}

/* initialize */
ok     = htons(PM_OK);
okshare= htons(PM_OKSHARE);
sorry  = htons(PM_SORRY);
resend = htons(PM_RESEND);

/* get the current hostname */
gethostname(hostname,BUFSIZE);

#ifdef SPMFIREWALL
/* get host address */
he= gethostbyname(hostname);

if(he) {
	hostaddr[0]= he->h_addr_list[0][0];
	hostaddr[1]= he->h_addr_list[0][1];
	hostaddr[2]= he->h_addr_list[0][2];
	hostaddr[3]= he->h_addr_list[0][3];

	if(hostaddr[0] < 0) hostaddr[0]+= 256;
	if(hostaddr[1] < 0) hostaddr[1]+= 256;
	if(hostaddr[2] < 0) hostaddr[2]+= 256;
	if(hostaddr[3] < 0) hostaddr[3]+= 256;


	/* initialize firewall: -f <firewallfile>
	 *  If no such option, then the "SPMFIREWALL" environment variable
	 *  is examined
	 */
	if(argc > 1 && argv[1][0] == '-' && argv[1][1] == 'f')
	  firewall_init(argv[1]+2);
	else firewall_init("");
	}
else hostaddr[0]= hostaddr[1]= hostaddr[2]= hostaddr[3]= '\0';
#endif

/* open a server for the PortMaster */
server= Sopen_prtmstr();
if(!server) {
	error(ERROR,"%s PortMaster's port (#%d) already in use!!!\n",
	  hostname, PORTMASTER);
#ifdef vms
	exit(1);
#else
	exit(0);
#endif
	}

#ifdef unix_close_stdio
/* get rid of things we no longer need */
fclose(stdin);
fclose(stdout);
fclose(stderr);
#endif

/* set up a timeout return point */
(void) setjmp(timedout);

/* accept clients */
while(1) {

	/* close out old connection		*/
	if(client) {
		shutdown(client->skt,2);
		close(client->skt);
		client->skt= NULL;
		freeSocket(client);	/* free client Socket			*/
		client= NULL;		/* paranoia is good medicine	*/
		}


	/* accept a client connection */
	client= Saccept(server);
	if(!client) {
		continue;
		}

#ifdef SPMFIREWALL
	if(firewall(client)) {	/* reject clients not inside firewall */
		Swrite(client,(char *) &sorry,sizeof(sorry));
		shutdown(client->skt,2);
		close(client->skt); client->skt= NULL;
		freeSocket(client); client     = NULL;
		continue;
		}
#endif	/* SPMFIREWALL */

	/* get a PrtMstrEvent */
	resend_cnt= 0;
	do {

		/* get the event that PortMaster is to handle */
		if(Stimeoutwait(client,TIMEOUT,0L) < 0) longjmp(timedout,1);
		Sreadbytes(client,(char *) &event,sizeof(event));
		event= ntohs( event);
	
/* The PortMaster handles several events:
 /===============================================\
 |            Client        PortMaster           |
 | Event      Sends         Sends                |
 |===============================================|
 | PM_SERVER                PM_OK / PM_RESEND    |
 |            sktname                            |
 |            port                               |
 |                          PM_OK / PM_SORRY     |
 |-----------------------------------------------|
 | PM_CLIENT                                     |
 |                          PM_OK / PM_RESEND    |
 |            sktname                            |
 |                          PM_OK / PM_SORRY     |
 |                          port                 |
 |-----------------------------------------------|
 | PM_CLOSE                                      |
 |                          PM_OK / PM_RESEND    |
 |           port                                |
 |                          PM_OK / PM_SORRY     |
 |-----------------------------------------------|
 | PM_TABLE                                      |
 |                          PM_OK / PM_RESEND    |
 |                          count of servers     |
 |                          "server : port#"     |
 |-----------------------------------------------|
 | PM_QUIT                                       |
 |                          PM_OK / PM_RESEND    |
 |           "PortMaster"                        |
 |-----------------------------------------------|
 | PM_FWINIT                                     |
 |                          PM_OK / PM_RESEND    |
 |-----------------------------------------------|
 | PM_SHARE                 PM_OK / PM_RESEND    |  Supports new portmaster
 |           hostname                            |  sharing mode.  Now, a
 |           sktname                             |  single Portmaster can support
 |           port                                |  a number of different machines.
 |                          PM_OK / PM_SORRY     |  The main reason for this is to
 |-----------------------------------------------|  support PC-only shops, but it
 | PM_CLIENT                                     |  will work with any PortMaster.
 |                          PM_OK / PM_RESEND    |
 |           sktname                             |
 |                          PM_OKSHARE / PM_SORRY|
 |                          hostname             |
 |                          port                 |
 \===============================================/
 */
		switch(event) {
	
		case PM_SERVER:	/* enter port into database */
		case PM_SHARE:	/* enter hostname & port into database */
			pm_server(client,event);
			resend_cnt= 0;
			break;
	
		case PM_CLIENT:	/* return port from database */
			pm_client(client);
			resend_cnt= 0;
			break;
	
		case PM_CLOSE:	/* delete port from database */
			pm_close(client);
			resend_cnt= 0;
			break;

		case PM_QUIT:	/* this function terminates the PortMaster */
			pm_quit(server,client);
			resend_cnt= 0;
			break;
	
		case PM_TABLE:	/* print out the PortTable */
			pm_table(client);
			resend_cnt= 0;
			break;

		case PM_RMSERVER:	/* remove a server entry from the PortMaster */
			pm_rmserver(client);
			resend_cnt= 0;
			break;

#ifdef SPMFIREWALL
		case PM_FWINIT:		/* re-read firewall file */
			pm_fwinit(client);
			resend_cnt= 0;
			break;
#endif

		default:		/* huh?  whaddaIdonow?	*/
			Swrite(client,(char *) &resend,sizeof(resend));

			/* terminate resending after PM_MAXTRY attempts */
			if(++resend_cnt > PM_MAXTRY) {
				resend_cnt= 0;
				shutdown(client->skt,2);
				close(client->skt); client->skt= NULL;
				freeSocket(client); client     = NULL;
				}
			break;
			}
		} while(resend_cnt);
	}
}

/* ---------------------------------------------------------------------- */

/* Sopen_prtmstr: this function opens a socket handle for the Portmaster */
static Socket *Sopen_prtmstr()
{
int     length;
int     status=1;
Socket *skt;


/* allocate a Socket */
skt= makeSocket(hostname,"PortMaster",PM_SERVER);

/*	create a socket      */
if((skt->skt= socket(AF_INET, SOCK_STREAM, 0)) <0 ) {
	freeSocket(skt); skt= NULL;
	return skt;
	}

#ifndef SSLNOSETSOCKOPT
/* allow PortMaster to be brought back up quickly (this should seldom be necessary!) */
status= 1;
if(setsockopt(skt->skt,SOL_SOCKET,SO_REUSEADDR,(char *) &status,sizeof(status)) < 0) {
	}
#endif

/*	initialize socket data structure     */
sin.sin_family      = AF_INET;
sin.sin_addr.s_addr = INADDR_ANY;
sin.sin_port        = htons((u_short) PORTMASTER);

/*	bind socket data structure to this socket     */
if(bind(skt->skt,(struct sockaddr *) &sin,sizeof(sin))) {
	freeSocket(skt); skt= NULL;
	return skt;
	}

/* getsockname fills in the socket structure with information,
 * such as the port number assigned to this socket
 */
length = sizeof(sin);
if(getsockname(skt->skt,(struct sockaddr *) &sin,&length)) {
	freeSocket(skt); skt= NULL;
	return skt;
	}
skt->port= ntohs(sin.sin_port);

/* 	prepare socket queue for (up to) PM_MAXREQUESTS (nominally 10) connection requests.
 *  Often, even though PM_MAXREQUESTS > 5, only 5 pending connection requests will be
 *  permitted by your o/s settings.
 */
listen(skt->skt,PM_MAXREQUESTS);

#ifndef MSDOS
#ifndef _AIX
/* turn off TCP's buffering algorithm so small packets don't get
 * needlessly delayed
 */
if(setsockopt(skt->skt,IPPROTO_TCP,TCP_NODELAY,(char *) &status,sizeof(status))
  < 0) {
	}
#endif	/* #ifndef _AIX ... */
#endif	/* #ifndef MSDOS ... */

/* return server socket */
return skt;
}

/* ---------------------------------------------------------------------- */

/* makePortTable: this function allocates, links, sets up a PortTable */
#ifdef __PROTOTYPE__
static PortTable *makePortTable(
  SKTEVENT port,
  char    *sharehost)
#else
static PortTable *makePortTable(port,sharehost)
SKTEVENT port;
char    *sharehost;
#endif
{
PortTable *p;


/* allocate a PortTable */
p= (PortTable *) malloc(sizeof(PortTable));
outofmem((void *) p,"attempt to makePortTable(%s,%d)",sktname,port);

/* double link a PortTable */
if(ptl) ptl->nxt= p;
else    phd     = p;
p->nxt= NULL;
p->prv= ptl;
ptl   = p;

/* set up PortTable */
p->sktname= (char *) calloc((size_t) strlen(sktname)+1,sizeof(char));
outofmem((void *) p->sktname,"attempt to allocate sktname<%s>\n",sktname);
strcpy(p->sktname,sktname);

p->sharehost= NULL;
p->port     = port;

/* set up optional sharehost */
if(sharehost) {
	p->sharehost= (char *) calloc((size_t) strlen(sharehost)+1,sizeof(char));
	outofmem((void *) p->sharehost,"attempt to allocate sharehost<%s>\n",sharehost);
	strcpy(p->sharehost,sharehost);
	}

return p;
}

/* ---------------------------------------------------------------------- */

/* delPortTable: this function deletes a PortTable */
#ifdef __PROTOTYPE__
static void delPortTable(PortTable *p)
#else
static void delPortTable(p)
PortTable *p;
#endif
{

/* free memory used by PortTable */
if(p->sktname)   free(p->sktname);
if(p->sharehost) free(p->sharehost);

/* de-link the PortTable */
if(p->prv) p->prv->nxt= p->nxt;
else       phd        = p->nxt;
if(p->nxt) p->nxt->prv= p->prv;
else       ptl        = p->prv;

}

/* ---------------------------------------------------------------------- */

/* pm_server: this function handles the PortMaster Server protocol */
#ifdef __PROTOTYPE__
void pm_server(
  Socket  *client,				/* temporary client->PortMaster socket	*/
  SKTEVENT event)				/* PM_SERVER or PM_SHARE				*/
#else
void pm_server(client,event)
Socket  *client;
SKTEVENT event;
#endif
{
PortTable  *p;
SKTEVENT    port;
static char sharehost[BUFSIZE];


Swrite(client,(char *) &ok,sizeof(ok));

if(event == PM_SHARE) {
	if(Stimeoutwait(client,TIMEOUT,0L) < 0) longjmp(timedout,1);
	Sgets(sharehost,BUFSIZE,client);
	}
else sharehost[0]= '\0';

if(Stimeoutwait(client,TIMEOUT,0L) < 0) longjmp(timedout,1);
Sgets(sktname,BUFSIZE,client);

/* check if sktname already in table */
for(p= phd; p; p= p->nxt) if(!strcmp(sktname,p->sktname)) {
	Swrite(client,(char *) &sorry,sizeof(sorry));
	return;
	}

if(Stimeoutwait(client,TIMEOUT,0L) < 0) longjmp(timedout,1);
Sreadbytes(client,(char *) &port,sizeof(port));
port= ntohs( port);

/* make a PortTable entry for the new server */
p   = makePortTable(port,(event == PM_SHARE)? sharehost : NULL);

/* unable to make PortTable entry */
if(!p) {
	Swrite(client,(char *) &sorry,sizeof(sorry));
	}
else {
	Swrite(client,(char *) &ok,sizeof(ok));
	}

}

/* ---------------------------------------------------------------------- */

/* pm_client: handle PortMaster client protocol (looks up server port) */
#ifdef __PROTOTYPE__
void pm_client(Socket *client)
#else
void pm_client(client)
Socket *client;
#endif
{
PortTable *p;
SKTEVENT   port;


Swrite(client,(char *) &ok,sizeof(ok));

if(Stimeoutwait(client,TIMEOUT,0L) < 0) longjmp(timedout,1);
Sgets(sktname,BUFSIZE,client);

/* attempt to look up sktname in PortTable linked list */
for(p= phd; p; p= p->nxt) {
	if(!strcmp(p->sktname,sktname)) {

		/* respond with PM_OK (normally) or PM_OKSHARE for PortMaster-sharing */
		if(p->sharehost) {
			Swrite(client,(char *) &okshare,sizeof(ok));
			Sputs(p->sharehost,client);
			}
		else {
			Swrite(client,(char *) &ok,sizeof(ok));
			}

		/* return port number */
		port= htons(p->port);
		Swrite(client,(char *) &port,sizeof(port));
		break;
		}
	}

/* if unable to find server */
if(!p) {
	Swrite(client,(char *) &sorry,sizeof(sorry));
	}

}

/* ---------------------------------------------------------------------- */

/* pm_close: this function handles a server close (removes server from
 *  PortTable)
 */
#ifdef __PROTOTYPE__
void pm_close(Socket *client)
#else
void pm_close(client)
Socket *client;
#endif
{
PortTable *p;
SKTEVENT   port;

Swrite(client,(char *) &ok,sizeof(ok));

/* get port of skt to be closed */
if(Stimeoutwait(client,TIMEOUT,0L) < 0) longjmp(timedout,1);
Sreadbytes(client,(char *) &port,sizeof(port));
port= ntohs( port);

for(p= phd; p; p= p->nxt) {
	if(p->port == port) {
		Swrite(client,(char *) &ok,sizeof(ok));
		delPortTable(p);
		break;
		}
	}

/* report if unable to find requested port */
if(!p) {
	Swrite(client,(char *) &sorry,sizeof(sorry));
	}

}

/* ---------------------------------------------------------------------- */

/* pm_quit: this function causes the PortMaster to quit (shutdown) */
#ifdef __PROTOTYPE__
void pm_quit(
  Socket *server,
  Socket *client)
#else
void pm_quit(
  server,
  client)
Socket *server;
Socket *client;
#endif
{
Swrite(client,(char *) &ok,sizeof(ok));

/* get sktname (should be PortMaster) */
if(Stimeoutwait(client,TIMEOUT,0L) < 0) longjmp(timedout,1);
Sgets(sktname,BUFSIZE,client);

/* if sktname was "PortMaster", then quit */
if(!strcmp(sktname,"PortMaster")) {
	if(client) {
		shutdown(client->skt,2);
		close(client->skt);
		client->skt= NULL;
		freeSocket(client);
		}
	if(server) {
		shutdown(client->skt,2);
		close(server->skt);
		server->skt= NULL;
		freeSocket(server);
		}
#ifdef vms
	exit(1);
#else
	exit(0);
#endif
	}

/* if not "PortMaster", then don't quit */
else {
	Swrite(client,(char *) &sorry,sizeof(sorry));
	}

}

/* ---------------------------------------------------------------------- */

/* pm_table: this function returns the PortMaster's PortTable through the
 * socket to it
 */
#ifdef __PROTOTYPE__
void pm_table(Socket *client)
#else
void pm_table(client)
Socket *client;
#endif
{
PortTable *p;
SKTEVENT   cnt;

Swrite(client,(char *) &ok,sizeof(ok));

/* count table entries */
for(p= phd, cnt= 0; p; p= p->nxt) ++cnt;

cnt= htons(cnt);
Swrite(client,(char *) &cnt,sizeof(cnt));

/* send PortMaster table out the socket */
for(p= phd; p; p= p->nxt) {
	Sprintf(client,"%s : %d",p->sktname,p->port);
	}

}

/* ---------------------------------------------------------------------- */

/* pm_rmserver: this function removes a server entry from the PortTable */
#ifdef __PROTOTYPE__
void pm_rmserver(Socket *client)
#else
void pm_rmserver(client)
Socket *client;
#endif
{
char       locbuf[BUFSIZE];
PortTable *p;

Swrite(client,(char *) &ok,sizeof(ok));

/* get name of server to be removed from PortTable */
if(Stimeoutwait(client,TIMEOUT,0L) < 0) longjmp(timedout,1);
Sgets(locbuf,BUFSIZE,client);


for(p= phd; p; p= p->nxt) {
	if(!strcmp(p->sktname,locbuf)) {
		Swrite(client,(char *) &ok,sizeof(ok));
		delPortTable(p);
		break;
		}
	}

/* report if unable to find requested port */
if(!p) {
	Swrite(client,(char *) &sorry,sizeof(sorry));
	}

}

/* --------------------------------------------------------------------- */
#ifdef SPMFIREWALL

/* firewall: how to keep out unwanted types
 *  Environment Variable: SPMFIREWALL=spfirewall.dat file
 *
 *  <spmfirewall.dat> format:
 *    [#*] [#*] [#*] [#*] [#*] [#*] 
 *
 *  Use a number to restrict entry; an * for accept all
 *
 * Returns:  0=accept
 *           1=reject
 */
#ifdef __PROTOTYPE__
int firewall(Socket *skt)
#else
int firewall(skt)
Socket *skt;
#endif
{
int                    ret    = 1;
static struct sockaddr name;
static int             namelen= sizeof(name.sa_data);
SpmFW                 *spmfw  = NULL;


if(!spmfwhd) {
	return 0;
	}

/* get connecting socket's internet address (peername) */
if(getpeername(skt->skt,&name,&namelen) == -1) {
	}
else {	/* check that connecting socket's internet address is approved */
	int i;
	int datum;
	for(spmfw= spmfwhd; spmfw; spmfw= spmfw->nxt) {
		for(i= 0; i < SPMFWDATA; ++i) {
			datum= name.sa_data[i];
			if(datum < 0) datum+= 256;
			if(spmfw->data[i] != -1 && spmfw->data[i] != datum) break;
			}
		if(i >= SPMFWDATA) {	/* name in firewall */
			ret= 0;
			break;
			}
		}
	}

return ret;
}

/* --------------------------------------------------------------------- */

/* firewall_init: initialize PortMaster firewall */
#ifdef __PROTOTYPE__
void firewall_init(char *fwfile)
#else
void firewall_init(fwfile)
char *fwfile;
#endif
{
char   buf[BUFSIZE];
char  *b;
int    i;
int    self = 0;
FILE  *fp   = NULL;
SpmFW *spmfw= NULL;


/* free up old firewall data (if any) */
if(spmfwhd) {
	SpmFW *spmfwnxt=NULL;

	for(spmfw= spmfwhd; spmfw; spmfw= spmfwnxt) {
		spmfwnxt= spmfw->nxt;
		free((char *) spmfw);
		}
	spmfwhd= NULL;
	}

if(!fwfile || !fwfile[0]) { /* get fwfile from environment SPMFIREWALL */
	fwfile= getenv("SPMFIREWALL");
	}

if(fwfile) {
	fp= fopen(fwfile,"r");
	if(fp) {

		/* allocate and forward-link a SpmFW */
		spmfw= (SpmFW *) malloc(sizeof(SpmFW));
		if(!spmfw) {
			error(WARNING,
			  "unable to allocate a SpmFW in firewall_init()\n");
			return;
			}
		spmfw->nxt    = spmfwhd;
		spmfwhd       = spmfw;

		/* insure that the PortMaster host is on the firewall list */
		spmfw->data[0]= -1;
		spmfw->data[1]= -1;
		spmfw->data[2]= hostaddr[0];
		spmfw->data[3]= hostaddr[1];
		spmfw->data[4]= hostaddr[2];
		spmfw->data[5]= hostaddr[3];

		/* read in <spmfirewall.dat> file */
		while(fgets(buf,BUFSIZE,fp)) {

			/* ignore comments, ignore blank lines */
			b= strchr(buf,'#');
			if(b) *b= '\0';
			b= stpblk(buf);
			if(!b[0]) continue;

			/* allocate and forward-link a SpmFW */
			spmfw= (SpmFW *) malloc(sizeof(SpmFW));
			if(!spmfw) {
				error(WARNING,
				  "unable to allocate a SpmFW in firewall_init()\n");
				return;
				}
			spmfw->nxt= spmfwhd;
			spmfwhd   = spmfw;

			for(i= 0; i < SPMFWDATA; ++i) {
				if(*b == '*') {
					spmfw->data[i]= -1;
					b             = stpblk(b+1);
					}
				else {
					sscanf(b,"%d",&spmfw->data[i]);
					b= stpnxt(b,"%d%b");
					}
				}
			}
		}
	}

}

/* --------------------------------------------------------------------- */

/* pm_fwinit: handles PortMaster firewall re-initialize */
#ifdef __PROTOTYPE__
void pm_fwinit(Socket *client)
#else
void pm_fwinit(client)
Socket *client;
#endif
{

firewall_init("");

Swrite(client,(char *) &ok,sizeof(ok));

}
#endif	/* SPMFIREWALL */

/* --------------------------------------------------------------------- */
