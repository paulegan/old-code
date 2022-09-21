#include <stdio.h>
#include <math.h>
#include <malloc.h>

unsigned convert_10(char *str, unsigned base);
char *convert_b(unsigned num, unsigned base);

void main()
{
 char *str;
 
 printf("23 in base 4  = %d in base 10\n",convert_10("23",4));
 str = convert_b(12,2);
 printf("12 in base 10 = %s in base 2\n",str);
 free(str);
  
}/* main */


unsigned convert_10(char *str, unsigned base)
{
 int num,p=strlen(str);
 
 while(*str)
  num+=((*(str++))-'0')*(int)pow(base,--p);
 return(num);
}/* convert_10 */


char *convert_b(unsigned num, unsigned base)
{
 int  size=(int)(pow(num,1.0/base)+2);
 char *temp=malloc(size)+size-1;
 
 *temp='\0'; 
 while(num>0){
  *(--temp) = (num%base)+'0';
  num/=base;
 }
 return(temp);
}/* convert_b */
