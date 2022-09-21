
/* des.h			lumpy@cs.ucc.ie			22.8.1996	*/

/* header file for des.c								*/

#ifndef des_header
#define des_header

char *newDESKey();
char *DESencrypt(char *message, char *keyStr);
char *DESdecrypt(char *message, char *keyStr);

#endif
