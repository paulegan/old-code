
#include <string.h>

main(int argc, char *argv[])
{
 char *a,*b,*c;
 if (a=strstr(argv[1],"://")){
   if ((b=strchr(a+3,':')) && (c=strchr(a+3,'/')) && (b<c)){
     while (*c) *b++ = *c++;
     *b = 0;
   }
 }
 printf("%s\n",argv[1]);
}
