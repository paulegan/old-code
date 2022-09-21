/*
 * server.c
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sltcp.h>

void user_read(int, char *, unsigned);

void main()
{
	if(conn_attach(13666) < 0)
	{
		slperror("conn_attach");
		exit(1);
	}

	/*
	 * Whenever a complete data structure is available on a connection,
	 * call the routine user_read().
	 */

	if(conn_register(user_read, IO_READ) < 0)
	{
		slperror("conn_register");
		exit(1);
	}

	/*
	 * Enter the event loop, and wait.
	 */

	sl_event_loop();
}

/*
 * Reads what was sent up a connection, prints it on the screen, and writes
 * and acknowledgement back down to the sender.
 */

void user_read(int c, char *buf, unsigned len)
{
	printf("%s [len=%d]\n", buf,strlen(buf));
	if(conn_write(c, buf, strlen(buf)) < 0)
	{
		slperror("conn_write");
		exit(1);
	}
}

