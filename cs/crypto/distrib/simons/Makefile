#
# Makefile by lumpy@cs.ucc.ie				29.8.1996
#

CC = 		gcc
INCLUDES = 	-I.
CFLAGS = 	-O ${INCLUDES} -L../alpha


all: configure trent client

trent: frogmsg.o trent.c frogmsg.h
	$(CC) $(CFLAGS) -o trent trent.c frogmsg.o -l

configure: 
	$(CC) $(CFLAGS) -o configure configure.c -lclientLib.a

client: frogmsg.o client.c frogmsg.h 
	$(CC) $(CFLAGS) -o client client.c frogmsg.o -lcryproto

frogmsg.o: frogmsg.c frogmsg.h
	$(CC)  -c -DNTEST frogmsg.c

frogmsg: frogmsg.c frogmsg.h
	$(CC) -o frogmsg frogmsg.c 
