
/* cryptTest2.c			lumpy@cs.ucc.ie			23.8.1996	*/

/* A simple program to test the cryptographic function of the library.		*/


#include <stdio.h>
#include <clientLib.h>

void main()
{
 char *key; 
 char *message;
  
 startSession("cryptTest2","cryptoServer@freedom");
 
 key = receiveMessage();
 printf("Key:%s. \n",key);
 message = receiveMessage();
 printf("Message:%s. \n",message);
 printf("Decrypted Message:%s. \n",DESdecrypt(message,key));

 free(key);
 free(message);
 
 key = receiveMessage();
 printf("Key:%s. \n",key);
 message = receiveMessage();
 printf("Message:%s. \n",message);
 printf("Decrypted Message:%s. \n",RSAdecrypt(message,key));
 
 free(key);
 free(message);
 
 endSession();
}/* main */
