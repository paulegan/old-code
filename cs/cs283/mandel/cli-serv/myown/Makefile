all: mandel server

manclientlib:
	gcc -I/usr/X11R6/include -c manclientlib.c 

mandel: manclientlib
	gcc -L/usr/X11R6/lib -lX11 manclientlib.o mandel.c -o mandel

server:
	gcc -L/usr/X11R6/lib -I/usr/X11R6/include server.c -lX11 -o manServer
