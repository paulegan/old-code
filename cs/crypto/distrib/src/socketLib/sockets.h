/* sockets.h: this file is the header file for the tlm socket function
 *   library
 *
 *  Version: 2.0
 *	Authors: Dr. Charles E. Campbell, GSFC/NASA
 *           Terry McRoberts, GSFC/NASA
 *
 *  Updated by lumpy@cs.ucc.ie to compile on SunOS, OSF and FreeBSD
 *
 */

#ifndef SOCKETS_H
#define SOCKETS_H

#include "xtdio.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#ifdef __FreeBSD__
# include "FreeBSD_tcp.h"
#else
# include <netinet/tcp.h>
#endif

#ifdef SSLNEEDTIME
# include <sys/time.h>
#endif

/* --------------------------------------------------------------------------
 * Definitions Section
 */
typedef u_short SKTEVENT;
#define PORTMASTER	((u_short) 1541)	/* standard PortMaster port		*/


/* PortMaster messages */
#define PM_SERVER	((u_short) 1)
#define PM_CLIENT	((u_short) 2)
#define PM_CLOSE	((u_short) 3)
#define PM_RESEND	((u_short) 4)
#define PM_QUIT		((u_short) 5)
#define PM_SORRY	((u_short) 6)
#define PM_OK		((u_short) 7)
#define PM_ACCEPT	((u_short) 8)
#define PM_TABLE	((u_short) 9)
#define PM_RMSERVER	((u_short) 10)
#define PM_FWINIT	((u_short) 11)
#define PM_SHARE    ((u_short) 12)
#define PM_OKSHARE	((u_short) 13)

#define PM_BIGBUF			1024
#define PM_MAXTRY			20	/* max number of resends to PortMaster	*/
#define PM_MAXREQUESTS		10	/* max pending connects					*/

/* --------------------------------------------------------------------------
 * Typedef Section
 */
typedef struct Skt_str     Socket;
typedef int                PrtMstrEvent;
typedef struct Smask_str   Smask;

/* --------------------------------------------------------------------------
 * Data Structures:
 */
struct Skt_str {
	int      skt;		/* skt handle						*/
	SKTEVENT port;		/* associated port					*/
	int      type;		/* PM_SERVER, PM_CLIENT, PM_ACCEPT	*/
	char    *sktname;	/* name of socket					*/
	char    *hostname;	/* name of host						*/
	};

struct Smask_str {
	fd_set   mask;
	unsigned waitall;
	};

/* --------------------------------------------------------------------------
 * Prototypes:
 */

#ifdef __PROTOTYPE__

Socket *Saccept(Socket *);                             /* Saccept.c       */
void Sclose(Socket *);                                 /* Sclose.c        */
char *Sgets( char *, int, Socket *);                   /* Sgets.c         */
Socket *makeSocket(char *,char *,int);                 /* Smkskt.c        */
void freeSocket(Socket *);                             /* Smkskt.c        */
int Smaskwait(void);                                   /* Smaskwait.c     */
void Smaskset(Socket *);                               /* Smaskwait.c     */
void Smaskfdset(int);                                  /* Smaskwait.c     */
void Smasktime(long,long);                             /* Smaskwait.c     */
int Smasktest(void);                                   /* Smaskwait.c     */
void Smaskunset(Socket *);                             /* Smaskwait.c     */
void Smaskunfdset(int);                                /* Smaskwait.c     */
Smask Smaskget(void);                                  /* Smaskwait.c     */
void Smaskuse(Smask);                                  /* Smaskwait.c     */
void Smaskpush(void);                                  /* Smaskwait.c     */
void Smaskpop(void);                                   /* Smaskwait.c     */
Socket *Sopen( char *, char *);                        /* Sopen.c         */
Socket *Sopen_clientport( char *, char *,              /* Sopen.c         */
   u_short);     
Socket *Sopenv( char *,  char *,  char *);             /* Sopenv.c        */
int Speek( Socket *,  void *,  int);                   /* Speek.c         */
unsigned long Speeraddr(Socket *);                     /* Speeraddr.c     */
char *Speername(Socket *);                             /* Speername.c     */
int Sprintf( Socket *, char *, ...);                   /* Sprintf.c       */
char *Sprtskt(Socket *);                               /* Sprtskt.c       */
void Sputs( char *, Socket *);                         /* Sputs.c         */
int Sread( Socket *,  void *,  int);                   /* Sread.c         */
int Sreadbytes( Socket *,  void *,  int);              /* Sreadbytes.c    */
SKTEVENT Srmsrvr(char *);                              /* Srmsrvr.c       */
int Sscanf(Socket *,char *,...);                       /* Sscanf.c        */
int Stest(Socket *);                                   /* Stest.c         */
int Stimeoutwait(Socket *,long,long);                  /* Stimeoutwait.c  */
int Stvwaitmany(long,Socket **,int);                   /* Stvwaitmany.c   */
int Stwaitmany(long,int,...);                          /* Stwaitmany.c    */
int Svprintf(Socket *, char *, void *);                /* Svprintf.c      */
int Svwaitmany(Socket **,int);                         /* Svwaitmany.c    */
int Swait(Socket *);                                   /* Swait.c         */
int Swaitmany(int,...);                                /* Swaitmany.c     */
int Swrite( Socket *,  void *,  int);                  /* Swrite.c        */

#else

extern Socket *Saccept();                              /* Saccept.c       */
extern void Sclose();                                  /* Sclose.c        */
extern char *Sgets();                                  /* Sgets.c         */
extern Socket *makeSocket();                           /* Smkskt.c        */
extern void freeSocket();                              /* Smkskt.c        */
extern int Smaskwait();                                /* Smaskwait.c     */
extern void Smaskset();                                /* Smaskwait.c     */
extern void Smaskfdset();                              /* Smaskwait.c     */
extern void Smasktime();                               /* Smaskwait.c     */
extern int Smasktest();                                /* Smaskwait.c     */
extern void Smaskunset();                              /* Smaskwait.c     */
extern void Smaskunfdset();                            /* Smaskwait.c     */
extern Smask Smaskget();                               /* Smaskwait.c     */
extern void Smaskuse();                                /* Smaskwait.c     */
extern void Smaskpush();                               /* Smaskwait.c     */
extern void Smaskpop();                                /* Smaskwait.c     */
extern Socket *Sopen();                                /* Sopen.c         */
extern Socket *Sopen_clientport();                     /* Sopen.c         */
extern Socket *Sopenv();                               /* Sopenv.c        */
extern int Speek();                                    /* Speek.c         */
extern unsigned long Speeraddr();                      /* Speeraddr.c     */
extern char *Speername();                              /* Speername.c     */
extern int Sprintf();                                  /* Sprintf.c       */
extern char *Sprtskt();                                /* Sprtskt.c       */
extern void Sputs();                                   /* Sputs.c         */
extern int Sread();                                    /* Sread.c         */
extern int Sreadbytes();                               /* Sreadbytes.c    */
extern SKTEVENT Srmsrvr();                             /* Srmsrvr.c       */
extern int Sscanf();                                   /* Sscanf.c        */
extern int Stest();                                    /* Stest.c         */
extern int Stimeoutwait();                             /* Stimeoutwait.c  */
extern int Stvwaitmany();                              /* Stvwaitmany.c   */
extern int Stwaitmany();                               /* Stwaitmany.c    */
extern int Stwaitmany();                               /* Stwaitmany.c    */
extern int Svprintf();                                 /* Svprintf.c      */
extern int Svwaitmany();                               /* Svwaitmany.c    */
extern int Swait();                                    /* Swait.c         */
extern int Swaitmany();                                /* Swaitmany.c     */
extern int Swrite();                                   /* Swrite.c        */
#endif

#ifdef __cplusplus
	}
#endif

#endif /* #ifndef SOCKETS_H */
