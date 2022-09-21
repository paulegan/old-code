
/* keyList.h			lumpy@cs.ucc.ie			29.8.1996	*/

/*  keyList.h - header file for keyList.c 					*/
/*	( implements a list of key lists )					*/


#ifndef KeyList_header
#define KeyList_header

#include <stdio.h>

typedef struct keyListTypeS keyListType;

struct keyListTypeS{
  char 	      *name;
  char        *key;
  keyListType *next;
};



void  storeKey(const char *name, const char *key);
char *getKey(const char *name);
void  removeKey(const char *name);
void  saveKeyList(const char *fileName);
void  loadKeyList(const char *fileName);
void  freeKeyList();
void  printKeyList();


#endif
