

/* cryptTest.c			lumpy@cs.ucc.ie			23.8.1996	*/

/* A simple program to test the cryptographic function of the library.		*/


#include <stdio.h>
#include <clientLib.h>

#define TestString	"This is a test!"

void main()
{
 char *DESKey;
 char  publicKey[PublicKeyStrSize];
 char  privateKey[PrivateKeyStrSize];
 long  startTime;
 
 startSession("cryptTest","cryptoServer@freedom");
 
 printf("Test string:%s. \n",TestString);
 
 DESKey = newDESKey();
 
 printf("Key:%s. \n",DESKey);
 
 sendMessageTo("cryptTest2",DESKey);
 sendMessageTo("cryptTest2",DESencrypt(TestString,DESKey));

 startTime = time(NULL);
 newRSAKeys(publicKey,privateKey);
 printf("Time to generate RSA keys = %d. \n",time(NULL)-startTime);
 
 sendMessageTo("cryptTest2",publicKey);
 sendMessageTo("cryptTest2",RSAencrypt(TestString,privateKey));
 
 endSession();
 
 free(DESKey);
}/* main */
