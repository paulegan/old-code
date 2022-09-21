
/* client.c			lumpy@cs.ucc.ie			13.8.1996	*/

/* A simple interactive client							*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <clientLib.h>
#include <bool.h>

char *getString(const char *prompt);

void main()
{
 char *command;
 bool notFinished = TRUE;
 int  error = Success;
 
 startSession(getString("Enter Name:"),getString("Server:"));
 
 sleep(1);
 while(newMessage()) printf("Message: \"%s\"\n",receiveMessage());
   
 do{
   command = getString("?");
   
   if (!strcmp(command,"e")){
     endSession();
     notFinished = FALSE;
   }
   else if (!strcmp(command,"s"))
     error = sendMessageTo(getString("To:"),getString("Message:"));
   else if (!strcmp(command,"r"))
     printf("Message: \"%s\"\n",receiveMessage());
   else if (!strcmp(command,"n"))
     if (newMessage()) printf("TRUE \n"); else printf("FALSE \n");
   else if (!strcmp(command,"lt"))
     error = listenTo(getString("Name:"));
   else if (!strcmp(command,"lf"))
     error = listenFrom(getString("Name:"));
   else if (!strcmp(command,"it"))
     error = interceptTo(getString("Name:"));
   else if (!strcmp(command,"if"))
     error = interceptFrom(getString("Name:"));
   else if (!strcmp(command,"nts"))
     printf("Time Stamp:%s. \n",newTimeStamp());
   else if (!strcmp(command,"vts"))
     if (validTimeStamp(getString("Time Stamp:"),10)) printf("TRUE \n"); else printf("FALSE \n");
   else if (!strcmp(command,"er"))
     if (error) printf("ERROR: %d \n",error); else printf("Success \n");
   else
     printf("Unknown cammond: %s\n",command);

 }while(notFinished);
 
}/* main */


char *getString(const char *prompt)
{
 const int bufSize = 200;
 char buf[bufSize];
 char *retStr;
  
 printf("%s ",prompt);
 fgets(buf,bufSize,stdin);
  
 retStr = (char*)malloc(strlen(buf)+1);
 strcpy(retStr,buf);
 retStr[strlen(retStr)-1] = '\0';
  
 return(retStr);
}/* getString */

