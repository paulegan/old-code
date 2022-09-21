
/* Program to rotate a string cyclically by a user defined amount.	*/

/* Written by Paul Egan, 30-11-1995				    	*/
/* Contact: lumpy@freedom.ucc.ie				    	*/

#include <stdio.h>

char rotateChar(char ch,int x);		/* Function to rotate ch by x	*/


void main()
{
 char  space[BUFSIZ];			/* Space to hold the string	*/
 char *str=space;
 int   rot;

 printf("Enter an integer (to rotate by): ");
 scanf("%d",&rot);
 printf("Enter the string to rotate: \n");
 getchar();				/* Clear the \n from the buffer	*/
 fgets(str,BUFSIZ,stdin);		/* Get the string - scanf	*/
 					/*   doesn't like spaces!	*/
 					
 while(*str)				/* Rotate each char in string	*/
  printf("%c",rotateChar(*(str++),rot));
 
}/* main */


char rotateChar(char ch,int x)
{
 if (ch>='a' && ch<='z'){
  ch-='a';
  ch=(ch+x)%26;
  ch+='a';
 }
 if (ch>='A' && ch<='Z'){
  ch-='A';
  ch=(ch+x)%26;
  ch+='A';
 }

 return(ch);
}/* rotateChar */
