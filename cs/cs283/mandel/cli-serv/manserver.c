/*
 * manserver.c
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sltcp.h>
#include "manlib.h"

#define SOCKET 13666
#define WINSIZE 600                    /*width and height of window, in pixels*/
#define INFINITY 100           /*what an `infinite' nr of iterations really is*/
#define ENLARGESCALE 10                            /*Enlarge by this on ZoomIn*/

#define FLUSH WINSIZE

void user_draw(int, char *, unsigned);

int count=0;


/*
 * Initialize windows etc. 
 */
void initialize()
{
    makewindow(WINSIZE);                           /*Pop up the drawing window*/
}/*initialize*/




main()
{

	if(conn_attach(SOCKET) < 0)
	{
		slperror("conn_attach");
		exit(1);
	}

	if(conn_register(user_draw, IO_READ) < 0)
	{
		slperror("conn_register");
		exit(1);
	}

	/*
	 * Enter the event loop, and wait.
	 */

	initialize();

	sl_event_loop();
}

/*
 * Reads what was sent up a connection,.
 */

void user_draw(int c, char *buf, unsigned len)
{
	unsigned int i,j; 
	int v; 

	sscanf(buf, "%u %u %i", &i, &j, &v); 
	drawpixel(i,j,v);

	if ((count++)==FLUSH){
   	    manflush();
	    count=0;
        }

}

