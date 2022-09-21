
/* clientList.c			lumpy@cs.ucc.ie			8.8.1996	*/


#include <clientList.h>

clientType *newClient(const char *name, Socket *skt, Socket *errorSkt)
{
 clientType *theNewClient = (clientType*)memalloc(sizeof(clientType));
 
 strcpy(theNewClient->name,name);
 theNewClient->socket = skt;
 theNewClient->errorSkt = errorSkt;
 theNewClient->interceptorTo = NULL;
 theNewClient->interceptorFrom = NULL;
 theNewClient->listenersTo = newClientList();
 theNewClient->listenersFrom = newClientList();

 return(theNewClient);
}/* newClient */


void freeClient(clientType *client)
{
 deleteClientList(client->listenersTo);
 deleteClientList(client->listenersFrom);
 
 free(client);
}/* freeClient */


clientListType *newClientList()
{
 clientListType *newList = (clientListType*)memalloc(sizeof(clientListType));
 
 newList->head = NULL;
 newList->tail = NULL;

 return(newList);
}/* newClientList */


bool emptyClientList(clientListType *list)
{
 return(list->head == NULL);
}/* emptyClientList */


int addClientToList(clientListType *list, clientType *client)
{							/* Adds client to end of list	*/
 clientListNode *newListNode = NULL;
 
 if (lookupClient(list,client->name))			/* Client already registered	*/
   return(-1);
 
 newListNode = (clientListNode*)memalloc(sizeof(clientListNode));
 newListNode->info = client;
 newListNode->next = NULL;
 
 if (!list->head)
   list->head = newListNode;
 else
   list->tail->next = newListNode;
 list->tail = newListNode;

 return(0);
}/* addClientToList */


void removeClientFromList(clientListType *list, const char *name)
{
 clientListNode *prevListNode = NULL;
 clientListNode *curListNode = list->head;
 
 while(curListNode && strcmp(curListNode->info->name,name)){
   prevListNode = curListNode;
   curListNode = curListNode->next;
 }
 
 if (curListNode){
   if (curListNode == list->head){
     list->head = curListNode->next;
     if (curListNode == list->tail) list->tail = NULL;
   }
   else{
     prevListNode->next = curListNode->next;
     if (curListNode == list->tail) list->tail = prevListNode;
   }
   free(curListNode);
 }

}/* removeClientFromList */


void printClientList(clientListType *list)
{
 clientListNode *curListNode = list->head;
 
 while(curListNode){
   printf("%s,",curListNode->info->name);
   curListNode = curListNode->next;
 }
 printf("\n");
}/* printClientList */


void deleteClientList(clientListType *list)
{
 clientListNode *curListNode = list->head;

 while(curListNode)
   removeClientFromList(list,curListNode->info->name);
  
}/* deleteClientList */


clientType *lookupClient(clientListType *list, const char *name)
{
 clientListNode *curListNode = list->head;
 
 while(curListNode && strcmp(curListNode->info->name,name))
   curListNode = curListNode->next;

 if (curListNode) 
  return(curListNode->info);
 else
  return(NULL);
}/* lookupSkt */

