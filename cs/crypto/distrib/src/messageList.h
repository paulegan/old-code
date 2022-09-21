
/* messageList.h		lumpy@cs.ucc.ie			9.8.1996	*/

/*  messageList.h - header file for messageList.c 				*/
/*	( implements a list of message lists )					*/


#ifndef MessageList_header
#define MessageList_header

#include <clientList.h>

typedef struct messageTypeS     messageType;
typedef struct messageListTypeS messageListType;

struct messageTypeS{
  clientType  *clientFrom;
  char        *message;
  messageType *next;
};

struct messageListTypeS{
  char 	           name[PrincipalNameSize];
  messageType     *list;
  messageListType *next;
};



void	         addMessageToList(const char *listName, const char *message);
messageListType *getMessageList(const char *listName);
void	         deleteMessageList(const char *listName);
void		 printMessageList(const char *listName);
void		 printAllLists();

#endif
