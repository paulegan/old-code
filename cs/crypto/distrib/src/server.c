
/* server.c			lumpy@cs.ucc.ie			14.8.1996		*/

#include <clientList.h>
#include <messageList.h>

int diag = FALSE;
clientListType *clientList = NULL;

void    dealWithClient(clientType *client);
void    sendMessage(clientType *clientFrom, const char *principalTo, char *message);
void 	connectClient(Socket *skt, Socket *errorSkt);
void    disconnectClient(clientType *client);
void    directMessage(int command, clientType *client, const char *principal);
void    validateTimeStamp(clientType *client);
char   *newDESKey();
Socket *startServer(int argc, char *argv[]);



int main(int argc, char *argv[])
{
 Socket	  	*skt = NULL;
 Socket	  	*errorSkt = NULL;
 Socket	  	*srvr = NULL;
 clientListNode *curListNode = NULL;
 int		 sktError;

 srandom((int)time(NULL));
 srvr = startServer(argc, argv);
 clientList = newClientList();
 
 Smaskset(srvr);
 do{

   Smaskwait();					/* Wait for incoming			*/
   
   if (Stest(srvr)){				/* Client looking for connection	*/  
     if (!(skt = Saccept(srvr))){
       error(WARNING,"Unable to accept connection!\n");
       continue;
     }
     if (!(errorSkt = Saccept(srvr))){
       error(WARNING,"Unable to connect error socket\n");
       continue;
     }
     connectClient(skt,errorSkt);
   }else{					/* Incoming data from a client		*/
     curListNode = clientList->head;
     while(curListNode){
       if (sktError = Stest(curListNode->info->socket)){	/* Data on this socket	*/
 	 if ((sktError == EOF) || (sktError<0))			/* Socket error		*/
	   disconnectClient(curListNode->info);
	 else
           dealWithClient(curListNode->info);
 	 break;					/* curListNode might be deleted so go	*/
       }					/*	go back to start!		*/
       curListNode = curListNode->next;
     }
   }/* else */
  
 }while(1);

 Sclose(srvr);
 deleteClientList(clientList);
  
}/* main */


void dealWithClient(clientType *client)
{
 int   command;
 char  principal[PrincipalNameSize]; 
 int   messageSize;
 char *message;
 
 Sscanf(client->socket,"%d",&command);
 
 switch(command){
 case EndSession:
   disconnectClient(client);
 break;
 case SendMessage:
   Sgets(principal,PrincipalNameSize,client->socket);
   Sscanf(client->socket,"%d",&messageSize);
   message = (char*)memalloc(messageSize);
   Sgets(message,messageSize,client->socket);
   sendMessage(client,principal,message);
 break;
 case ListenTo:
 case ListenFrom:
 case InterceptTo:
 case InterceptFrom:
   Sgets(principal,PrincipalNameSize,client->socket);
   directMessage(command,client,principal);
 break;
 case NewTimeStamp:
   Sprintf(client->socket,"%d",time(NULL));
   Sprintf(client->errorSkt,"%d",Success);
 break;
 case ValidTimeStamp:
   validateTimeStamp(client);
 break;
 case NewDESKey:
   Sprintf(client->socket,"%s",newDESKey());
   Sprintf(client->errorSkt,"%d",Success);
 break;
 default:
   error(WARNING,"Unknown command from %s! \n",client->name);
   Sprintf(client->errorSkt,"%d",Failure);
 }
 
 free(message);
}/* dealWithClient */


void sendMessage(clientType *clientFrom, const char *principalTo, char *message)
{
 clientListNode *listener;
 clientType *clientTo = lookupClient(clientList,principalTo);
 
 if (clientFrom){
  Sprintf(clientFrom->errorSkt,"%d",Success);
  if (clientFrom->interceptorFrom){
    sendMessage(NULL,clientFrom->interceptorFrom->name,message);
    return;
  }
  listener = clientFrom->listenersFrom->head;
  while(listener){
    sendMessage(NULL,listener->info->name,message);
    listener = listener->next;
  }
 }/* if */
  
 if (!clientTo){				/* 'clientTo' isn't registered!		*/
   addMessageToList(principalTo,message);
   if (diag){
     printf("Saving message for %s. Current messages: \t",principalTo);
     printMessageList(principalTo);
   }
   return;
 }
 
 if (clientTo->interceptorTo){
   sendMessage(NULL,clientTo->interceptorTo->name,message);
   return;
 }
 listener = clientTo->listenersTo->head;
 while(listener){
   sendMessage(NULL,listener->info->name,message);
   listener = listener->next;
 }
 
 Sprintf(clientTo->socket,"%d",strlen(message)+3);
 Sputs(message,clientTo->socket);

 if (diag) printf("Sending message \"%s\" to %s. \n",message,principalTo);
}/* sendMessage */


void connectClient(Socket *skt, Socket *errorSkt)
{
 char	          name[PrincipalNameSize];
 messageListType *savedMessages;
 messageType     *curMessage;
 
 Sgets(name,PrincipalNameSize,skt);
 
 if (lookupClient(clientList,name)){
   if (diag) printf("%s not connected: Already registered! \n",name);
   Sprintf(errorSkt,"%d",AlreadyRegistered);
   return;
 }
 Smaskset(skt);
 addClientToList(clientList,newClient(name,skt,errorSkt));
 Sprintf(errorSkt,"%d",Success);
 
 if (diag){
   printf("%s has connected.\t\tclientList: ",name);
   printClientList(clientList);
 }
 
 savedMessages = getMessageList(name);
 curMessage = savedMessages->list;
 while(curMessage){					/* Send all saved messages	*/
   sendMessage(NULL,name,curMessage->message);
   curMessage = curMessage->next;
 }
 deleteMessageList(name);
 
}/* connectClient */


void disconnectClient(clientType *client)
{
 clientListNode *curListNode = clientList->head;
 
 while(curListNode){					/* Remove client from all lists	*/
   if (curListNode->info->interceptorTo == client)
     curListNode->info->interceptorTo = NULL;
   if (curListNode->info->interceptorFrom == client)
     curListNode->info->interceptorFrom = NULL;
   removeClientFromList(curListNode->info->listenersTo,client->name);
   removeClientFromList(curListNode->info->listenersFrom,client->name);
   curListNode = curListNode->next;
 }

 if (diag) printf("%s has disconnected.\t\tclientList: ",client->name);

 removeClientFromList(clientList,client->name);
 Sprintf(client->errorSkt,"%d",Success);
 Smaskunset(client->socket);
 Sclose(client->socket);
 Sclose(client->errorSkt);
 freeClient(client);
 
 if (diag) printClientList(clientList);

}/* disconnectClient */


void directMessage(int command, clientType *client, const char *principal)
{
 clientType  *client2;
 
 if (!(client2 = lookupClient(clientList,principal))){
   Sprintf(client->errorSkt,"%d",NotRegistered);
   if (diag) printf("Request from \"%s\", to intercept messages from/to unregistered principal - \"%s\". \n",client->name,principal);
   return;
 }
 switch(command){
  case ListenTo:
    addClientToList(client2->listenersTo,client);
  break;
  case ListenFrom:
    addClientToList(client2->listenersFrom,client);
  break;
  case InterceptTo:
    client2->interceptorTo = client;
  break;
  case InterceptFrom:
    client2->interceptorFrom = client;
  break;
 }
  
 Sprintf(client->errorSkt,"%d",Success);
 if (diag) printf("Request from \"%s\", to intercept messages from/to principal - \"%s\". \n",client->name,principal);
  
}/* directMessage */


void validateTimeStamp(clientType *client)
{
 char timeStamp[TimeStampSize];
 long range;
 
 Sscanf(client->socket,"%s %d",timeStamp,&range);
 if (abs(time(NULL)-atol(timeStamp))<range)
   Sprintf(client->socket,"%d",TRUE);
 else
   Sprintf(client->socket,"%d",FALSE);
}/* validateTimeStamp */


char *newDESKey()
{
 int   keyLen = NewDESKeySize-1;
 char *keyStr = (char*)memalloc(NewDESKeySize);
 char *keyPtr = keyStr;
 char  ch;
 
 while(--keyLen){
   while(!isalnum(ch = (char)(random()%127)));
   *keyPtr++ = ch;
 }
 keyStr[NewDESKeySize-1] = '\0';
 
 return(keyStr);
}/* newDESKey */


Socket *startServer(int argc, char *argv[])
{
 Socket	*srvr = NULL;
 char   *serverName;
 
 switch(argc){
  case 1:
   serverName = DefaultServerName;
  break;
  case 2:
   if (!strcmp(argv[1],"-debug")){
     serverName = DefaultServerName;
     diag = TRUE;
   }
   else
     serverName = argv[1];
  break;
  case 3:
   if (strcmp(argv[1],"-debug"))
     error(ERROR,"Usage: %s [-debug] <serverName> \n",argv[0]);
   diag = TRUE;
   serverName = argv[2];
  break;
  default:
   error(ERROR,"Usage: %s [-debug] <serverName> \n",argv[0]);
 }
 
 if (diag) printf("Using server name: %s \n",serverName);
 if (!(srvr = Sopen(serverName,"S")))
   error(ERROR,"Unable to open <%s> as server\n",serverName);

 return(srvr);
}/* startServer */
