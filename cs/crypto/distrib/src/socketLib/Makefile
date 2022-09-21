# Makefile for the Simple Sockets Library (C version)
#
# Makefile by lumpy@cs.ucc.ie
#

CC = gcc
CFLAGS = -O -I.

LIBNAME = socket.a

OBJS = Saccept.o      Sprintf.o      Stest.o        outofmem.o     \
       Sclose.o       Sprtskt.o      Stimeoutwait.o rdcolor.o      \
       Sgets.o        Sputs.o        sprt.o         \
       Smaskwait.o    Sread.o        Swait.o        srmtrblk.o     \
       Smkskt.o       Sreadbytes.o   Swrite.o       stpblk.o       \
       Sopen.o        Srmsrvr.o      error.o        stpnxt.o       \
       Sopenv.o       Sscanf.o       fopenv.o       strnxtfmt.o    \
       Speek.o        cprt.o         Speername.o    Speeraddr.o

# Excluded Svprintf !!

all: lib Spm

lib: $(OBJS)
	ar r $(LIBNAME) *.o
	ranlib $(LIBNAME)

Spm: lib
	$(CC) $(CFLAGS) Spm.c $(LIBNAME) -o Spm

clean:
	@/bin/rm -f *.a *.bak *.err Spm $(OBJS)

new:	clean all
