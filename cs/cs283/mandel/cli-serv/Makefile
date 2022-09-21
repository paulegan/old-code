CC=gcc
CFLAGS=-O4 -Itcp/sltcp -Ltcp/sltcp
XLIB= -L/usr/X386/lib -I/usr/X386/include -lX11

all:	server
new:	clean all

manlib:
	$(CC) $(XLIB) -c manlib.c

manserver: manlib.o
	$(CC) $(CFLAGS) $(XLIB) -lsltcp manserver.c  manlib.o -o manserver

manclientlib: 
	$(CC) $(CFLAGS) -c manclientlib.c

mandel: manclientlib
	$(CC) $(CFLAGS) -lsltcp  manclientlib.o mandel.c -o mandel

server:
	$(CC) $(XLIB) -Itcp/sltcp tcp/sltcp/sltcp.c manlib.o manserver.c -o manserver
client:
	$(CC) -Itcp/sltcp tcp/sltcp/sltcp.c manclientlib.c mandel.c -o manclient


serverX:
	gcc -L/usr/X386/lib -I/usr/X386/include serverX.c -lX11 -o serverX
clientX:
	gcc -L/usr/X386/lib -I/usr/X386/include clientX.c -lX11 -o clientX
	
clean:
	rm -f *.o core server

