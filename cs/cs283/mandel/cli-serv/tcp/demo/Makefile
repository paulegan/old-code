CC=gcc
CFLAGS=-O4 -I../sltcp


all:    client
new:    clean all

client:
	$(CC) $(CFLAGS) ../sltcp/sltcp.c client.c -o client

sunClient:
	$(CC) $(CFLAGS) ../sltcp/sltcp.c client.c -o sunClient

server:
	$(CC) $(CFLAGS) ../sltcp/sltcp.c server.c -o server

sunServer:
	$(CC) $(CFLAGS) ../sltcp/sltcp.c server.c -o sunServer

clean:
	rm -f *.o core server

