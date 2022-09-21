SRCS=sltcp.c
OBJS=sltcp.o

LIB=libsltcp.a
INC=sltcp.h

DESTDIR=.
INCDIR=include
LIBDIR=lib

#CC=cc
CC=gcc

#CFLAGS=-g -I. -DDEBUG
#CFLAGS=-O2 -I.
CFLAGS=-O4 -I.

all: $(LIB)

install: all
	install -c -m 644 $(INC) $(DESTDIR)/$(INCDIR)
	install -c -m 644 $(LIB) $(DESTDIR)/$(LIBDIR)
	[ -r /usr/bin/ranlib ] && ranlib -t $(DESTDIR)/$(LIBDIR)/$(LIB)

$(LIB): $(OBJS)
	ar cru $(LIB) $(OBJS)
	[ -r /usr/bin/ranlib ] && ranlib $(LIB)

sltcp.o: sltcp.c sltcp.h sltcp_int.h

clean:
	rm -f *.o core $(LIB)


