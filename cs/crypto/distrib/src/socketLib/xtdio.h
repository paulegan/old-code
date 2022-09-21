/* xtdio.h: */
/* 
 *  Updated by lumpy@cs.ucc.ie to compile on SunOS, OSF and FreeBSD
 * 
 */

#ifndef XTDIO_H
#define XTDIO_H

#include "rdcolor.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifndef __FreeBSD__
# include <varargs.h>
#endif
#include <string.h>

#define __PROTOTYPE__

/* ---------------------------------------------------------------------
 * Definitions:
 */


#define SEVERE      0
#define ERROR       1
#ifndef WARNING
# define WARNING    2
#endif
#define NOTE        3


/* ---------------------------------------------------------------------
 * Global Variables:
 */
#ifdef __PROTOTYPE__
extern void (*error_exit)(int);
#else
extern void (*error_exit)();
#endif

/* ---------------------------------------------------------------------
 * Prototypes:
 */
# ifdef __PROTOTYPE__

char *cprt(const char);                                /* cprt.c          */
void error(int, char *, ...);                          /* error.c         */
FILE *fopenv(char *, char *, char *);                  /* fopenv.c        */
void outofmem(void *, char *, ...);                    /* outofmem.c      */
void rdcolor(void);                                    /* rdcolor.c       */
void rdcputs(char *, FILE *);                          /* rdcolor.c       */
char *sprt(const char *);                              /* sprt.c          */
void srmtrblk(char *);                                 /* srmtrblk.c      */
char *stpblk(char *);                                  /* stpblk.c        */
char *stpnxt(char *, char *);                          /* stpnxt.c        */
char *strnxtfmt(char *);                               /* strnxtfmt.c     */

# else	/* __PROTOTYPE__ */

extern char *cprt();                                   /* cprt.c          */
extern char *stpnxt();                                 /* stpnxt.c        */
extern char *stpblk();                                 /* stpblk.c        */
extern FILE *fopenv();                                 /* fopenv.c        */
extern char *sprt();                                   /* sprt.c          */
extern char *strnxtfmt();                              /* strnxtfmt.c     */

# endif	/* __PROTOTYPE__ */

#endif
