
/* messageList.c		lumpy@cs.ucc.ie			9.8.1996	*/


#include <messageList.h>

messageListType *listOfLists = NULL;

messageListType *addNewList(const char *listName)
{
  messageListType *newList = (messageListType*)memalloc(sizeof(messageListType));
  messageListType *curList = listOfLists;
  messageListType *prevList = NULL;
  
  strcpy(newList->name,listName);
  newList->list = NULL;
  newList->next = NULL;
  
  while(curList){
    prevList = curList;
    curList = curList->next;
  }
  
  if (curList == listOfLists)
    listOfLists = newList;
  else
    prevList->next = newList;
  
  return(newList);
}/*  addNewList */


void addMessageToList(const char *listName, const char *message)
{
  messageListType *curList = getMessageList(listName);
  messageType *newMessage = (messageType*)memalloc(sizeof(messageType));
  messageType *curMessage = curList->list;
  
  newMessage->message = (char*)memalloc(strlen(message)+1);
  strcpy(newMessage->message,message);
  newMessage->next = NULL;
  
    
  if (!curList->list)
    curList->list = newMessage;
  else{
    while(curMessage->next)
      curMessage = curMessage->next;
    curMessage->next = newMessage;
  }
  
}/* addMessageToList */


messageListType *getMessageList(const char *listName)
{
  messageListType *curList = listOfLists;

  while(curList && strcmp(curList->name,listName))
    curList = curList->next;

  if (curList)
    return(curList);
  else
    return(addNewList(listName));
}/* getMessageList */


void deleteMessageList(const char *listName)
{
 messageListType *curList = getMessageList(listName);
 messageType *curMessage = curList->list;
 messageType *prevMessage = NULL;

 if (curMessage){
   while(curMessage){
     prevMessage = curMessage;
     curMessage = curMessage->next;
     free(prevMessage->message);
     free(prevMessage);  
   }
   curList->list = NULL;
 }

}/* deleteMessageList */


void printMessageList(const char *listName)
{
 messageListType *curList = getMessageList(listName);
 messageType *curMessage = curList->list;

 while(curMessage){
   printf("\"%s\", ",curMessage->message);
   curMessage = curMessage->next;
 }
 printf("\n");

}/* printMessageList */


void printAllLists()
{
 messageListType *curList = listOfLists;
 
 while(curList){
   printMessageList(curList->name);
   curList = curList->next;
 }
}/* printAllLists */


