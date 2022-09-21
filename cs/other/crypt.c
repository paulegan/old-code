#include <stdio.h>

void convertToBitArray(char *str, char *bits);
void convertFromBitArray(char *str, char *bits);
void encryptString(char *str);

void main(int argc, char *argv[])
{
 const int MaxStringLength = 128;
 int  i=0,x;
 char str[MaxStringLength];
 char key[64];
 
 if ( argc != 2 || strlen(argv[1])!=2 ){
  printf("Usage: crypt <key> \n");
  printf(" <key> is a two character string \n");
  exit(1);
 }

 convertToBitArray(arv[1],key);
 setkey(key);
 
 while( !feof(stdin) ){
  fgets(str,MaxStringLength,stdin);
  encryptString(str);
  printf("%s \n",str);
 }

}/* main */

void encryptString(char *str)
{
 int c;
 char subString[8];
 char bits[64];
 
 for(c=0;c<strlen(str);c+=8){
  strcpy(subString,str+c);
  convertToBitArray(subString,bits);
  encrypt(bits,TRUE);
  convertFromBitArray(subString,bits);
  strcpy(str+c,subString);
 }
}/* encryptString */

void convertToBitArray(char *str, char *bits)
{
 for(b=0;b<8;b++)
  
}/* convertToBitArray */

void convertFromBitArray(char *str, char *bits)
{

}/* convertFromBitArray */


