
/* rsa.h			lumpy@cs.ucc.ie			22.8.1996	*/

/* header file for rsa.c							*/

#ifndef rsa_header
#define rsa_header

void newRSAKeys(char *publicKeyStr, char *privateKeyStr);
char *RSAencrypt(char *message, char *keyStr);
char *RSAdecrypt(char *message, char *keyStr);

#endif
