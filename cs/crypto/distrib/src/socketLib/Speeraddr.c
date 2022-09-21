/* Speeraddr.c: this function gets the name of the peer */
#include "sockets.h"

/* ---------------------------------------------------------------------
 * Definitions:
 */
#define PEERBUF	65

/* --------------------------------------------------------------------- */

/* Speeraddr: get peer IP address */
#ifdef __PROTOTYPE__
unsigned long Speeraddr(Socket *skt)
#else
unsigned long Speeraddr(skt)
Socket *skt;
#endif
{
unsigned long          ret;
static struct sockaddr sktname;
static int             namelen  = sizeof(sktname.sa_data);


if(getpeername(skt->skt,&sktname,&namelen) == -1) {
	ret= 0;
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
	ret = u.i;
	}

return ret;
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

printf("client<%s> accepted, peer is %lu\n",Sprtskt(skt),Speeraddr(skt));

Sclose(skt);
Sclose(srvr);

return 0;
}

#endif	/* DEBUG_TEST */
/* ===================================================================== */
