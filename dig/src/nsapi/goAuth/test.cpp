
#include <stdio.h>
#include "green/CGreen.h"

#define BUF_LEN 1024

int main(int argc, char* argv[])
{
 char buf[BUF_LEN+1];
 CGreen g(argv[1]);

 if ( g.isValid() ){

   if ( g.getUserTokens(buf,BUF_LEN) )
     printf("List of users tokens [%s]\n",buf);
   else
     printf("...could not get list of users tokens [%s]\n",buf);

   if ( g.userHasToken(1) ) 
     printf("User has token 1\n");
   else
     printf("...User does not have token 1\n");

   if ( g.getUserId(buf,BUF_LEN) )
     printf("User Id: %s\n",buf);
   else
     printf("...could not getUserId\n");

 }else{

   printf ("GREEN is not valid\n");

 }

 if ( g.hasWarnings() ){
   g.getWarningInfo(buf,BUF_LEN);
   printf("GREEN warning information:\n%s",buf);
 }
 	 
 if ( g.hasErrors() ){
   g.getErrorInfo(buf,BUF_LEN);
   printf("GREEN error information:\n%s",buf);
 }

 exit(0);
}
