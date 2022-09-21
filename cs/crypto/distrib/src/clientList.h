
#ifndef ClientList_header
#define ClientList_header

#include <config.h>

typedef struct clientTypeS     clientType;
typedef struct clientListNodeS clientListNode;
typedef struct clientListTypeS clientListType;

struct clientTypeS{
  char    	  name[PrincipalNameSize];
  Socket 	 *socket;
  Socket	 *errorSkt;
  clientType     *interceptorTo;
  clientType     *interceptorFrom;
  clientListType *listenersTo;
  clientListType *listenersFrom;
};

struct clientListNodeS{
  clientType     *info;
  clientListNode *next;
};

struct clientListTypeS{
  clientListNode *head;
  clientListNode *tail;
};

clientType 	*newClient(const char *name, Socket *socket, Socket *errorSkt);
void	    	 freeClient(clientType *client);
clientListType 	*newClientList();
void	    	 deleteClientList(clientListType *list);
int	    	 addClientToList(clientListType *list, clientType *client);
void	    	 removeClientFromList(clientListType *list, const char *name);
void	    	 printClientList(clientListType *list);
clientType 	*lookupClient(clientListType *list, const char *name);
bool 		 emptyClientList(clientListType *list);

#endif
