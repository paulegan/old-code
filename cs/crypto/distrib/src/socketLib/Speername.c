/* Speername.c: this function gets the name of the peer */
#include "sockets.h"

/* ---------------------------------------------------------------------
 * Definitions:
 */
#define PEERBUF	65

/* --------------------------------------------------------------------- */

/* Speername: get peername (a string) */
#ifdef __PROTOTYPE__
char *Speername(Socket *skt)
#else
char *Speername(skt)
Socket *skt;
#endif
{
static struct sockaddr sktname;
static char            buf1[PEERBUF];
static char            buf2[PEERBUF];
static char            buf3[PEERBUF];
static char           *buf    = buf1;
static int             namelen= sizeof(sktname.sa_data);


if     (buf == buf1) buf= buf2;
else if(buf == buf2) buf= buf3;
else                 buf= buf1;

if(getpeername(skt->skt,&sktname,&namelen) == -1) {
	strcpy(buf,"unknown");
	}
else {
	struct hostent *host;
	struct in_addr inaddr;
	union {
		unsigned char byte[4];
		unsigned long i;
		} u;
	u.byte[0]     = sktname.sa_data[2];
	u.byte[1]     = sktname.sa_data[3];
	u.byte[2]     = sktname.sa_data[4];
	u.byte[3]     = sktname.sa_data[5];
	inaddr.s_addr = u.i;
	host          = gethostbyaddr(&inaddr,namelen,AF_INET);
	if(host && host->h_name) strcpy(buf,host->h_name);
	else                     strcpy(buf,"unknown hostname");
	}

return buf;
}

/* ===================================================================== */
#ifdef DEBUG_TEST

/* main: test routine begins here... */
int main(int argc,char **argv)
{
Socket *srvr;
Socket *skt;

rdcolor();				/* initialize color names (GREEN, RED, etc.)    */

srvr= Sopen("GetPeerTest","S");
if(!srvr) error(ERROR,"unable to open <GetPeerTest> server\n");

skt = Saccept(srvr);

printf("client<%s> accepted, peer is <%s>\n",Sprtskt(skt),Speername(skt));

Sclose(skt);
Sclose(srvr);

return 0;
}

#endif	/* DEBUG_TEST */
/* ===================================================================== */
