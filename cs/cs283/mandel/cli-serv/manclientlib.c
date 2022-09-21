/*
 * client.c
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sltcp.h>
#define SERVER "security"
#define PORT 13666

	int conn;
	char ch[1024];
	char server[20];

void window_connect()
{
	
	printf("Please enter Server:");
	scanf("%s", &server);
	
	if((conn = conn_open(server, PORT)) < 0)
	{
		slperror("conn_opener");
		exit(1);
	}
}

void closedown(){
	conn_close(conn);
}

void drawpixel(unsigned int x, unsigned int y, int color){


	static int count=0;
		
	if (count++ == 600) {
		count=0;
		closedown();
 		sleep(0);
	if((conn = conn_open(server, PORT)) < 0)
	{
		slperror("conn_opener (lumpy)");
		exit(1);
	}
	} 
	sprintf(ch, "%u %u %i", x,y,color);

	if(conn_write(conn, ch, strlen(ch)) < 0)
	{
		slperror("conn_write");
		exit(1);
	}
conn_write(conn, NULL, 0);
}

