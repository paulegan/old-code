/*
 * client.c
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sltcp.h>
#define SERVER "inter45"

int conn;
int dontExit=1;
char ch[1024];

void main(void)
{

   if((conn = conn_open(SERVER, 13666)) < 0){
	slperror("conn_open");
	exit(1);
   }

   while(dontExit){

	fgets(ch, 1024, stdin);
	if(feof(stdin)){
		conn_close(conn);
		dontExit=0;
	}
	else{
		if(ch[strlen(ch) - 1] == '\n'){
			ch[strlen(ch) - 1] = '\0';
		}
		if(conn_write(conn, ch, strlen(ch)) < 0){
			slperror("conn_write");
			exit(1);
		}
	}/* else */

   }/* while */

}/* main */

