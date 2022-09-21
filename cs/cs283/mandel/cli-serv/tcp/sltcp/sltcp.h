/*
 * sltcp.h
 * TCP/IP Session Layer header file
 *
 * Copyright (C) 1993 Student Computing Research Group,
 * Department of Computer Science,
 * Trinity College,
 * Dublin 2,
 * Ireland.
 */

#if !defined(_sltcp_h)
#define _sltcp_h

/*
 * prototype and void handling for non-ANSI compilers
 */

#if defined(__STDC__)
#ifndef __P
#define __P(x) x
#endif
#ifndef __VOID
#define __VOID void
#endif
#else
#ifndef __P
#define __P(x) ()
#endif
#ifndef __VOID
#define __VOID char
#endif
#endif

/*
 * Create a service on a machine.
 */

int conn_attach __P((unsigned short));

/*
 * Open a connection to a service on a remote machine.
 */

int conn_open __P((char *, unsigned short));

/*
 * Write data to connection.
 */

int conn_write __P((int, char *, int));

/*
 * Graceful close of a connection.
 */

int conn_close __P((int));

/*
 * Register a callback routine for a connection.
 */

int conn_register __P((void (*)(), int));

/*
 * Various types of callback routines (second parameter to conn_register()).
 */

#define IO_READ 0
#define IO_ACCEPT 1
#define IO_ABORT 2
#define IO_CLOSE 3

/*
 * This callback is for dealing with plain file descriptors.
 */

#define IO_USER 1001

/*
 * Register a callback routine for a file descriptor.
 */

int conn_addfd __P((void (*)(), int, int));

/*
 * The types of file descriptors
 */

#define CONN_FD_READ 0
#define CONN_FD_WRITE 1
#define CONN_FD_EXCEPT 2

/*
 * The event loop.
 */

__VOID sl_event_loop __P((void));

/*
 * Error handler
 */

int slperror __P((char *s));

/*
 * sltcp's very own errno.
 */

extern int slerrno;

/*
 * Various Session Layer error messages.
 */

#define CE_OK                   ((__VOID *)0)
#define CE_SOCKET_FAIL		((__VOID *)1)
#define CE_BIND_FAIL		((__VOID *)2)
#define CE_MALLOC_FAIL		((__VOID *)3)
#define CE_GETHOSTBYNAME_FAIL	((__VOID *)4)
#define CE_CONNECT_FAIL		((__VOID *)5)
#define CE_REGISTER_FAIL	((__VOID *)6)
#define CE_UNKNOWN_SESSION	((__VOID *)7)
#define CE_READ_FAIL		((__VOID *)8)
#define CE_WRITE_FAIL		((__VOID *)9)
#define CE_MESSAGE_FAIL		((__VOID *)10)
#define CE_STATE_UNKNOWN	((__VOID *)11)

#endif /* !defined(_sltcp_h) */


