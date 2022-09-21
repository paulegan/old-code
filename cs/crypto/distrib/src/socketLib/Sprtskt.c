/* Sprtskt.c: */
#include <stdio.h>
#include "sockets.h"

/* ------------------------------------------------------------------------
 * Local Definitions:
 */
#define BUFSIZE 128

/* ------------------------------------------------------------------------ */

/* Sprtskt: this function prints out a socket description */
#ifdef __PROTOTYPE__
char *Sprtskt(Socket *skt)
#else
char *Sprtskt(skt)
Socket *skt;
#endif
{
static char buf1[BUFSIZE];
static char buf2[BUFSIZE];
static char *b= buf1;

if(!skt) return "null socket";

/* toggles between the two static buffers */
if(b == buf1) b= buf2;
else          b= buf1;

sprintf(b,"#%d:p%d:%s:%s@%s",
  skt->skt,
  skt->port,
  (skt->type == PM_SERVER)? "server" :
  (skt->type == PM_CLIENT)? "client" :
  (skt->type == PM_ACCEPT)? "accept" : "???",
  skt->sktname?  skt->sktname  : "null-sktname",
  skt->hostname? skt->hostname : "null-host");

return b;
}

/* ------------------------------------------------------------------------ */
