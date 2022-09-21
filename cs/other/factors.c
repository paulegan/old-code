/* Program to find the factors of any positive integer up to MAXLONG 	*/

/* Written by Paul Egan, 16-11-1995				    	*/
/* Contact: lumpy@freedom.ucc.ie				    	*/

#include <stdio.h>
#include <values.h>


void main(int argc,char *argv[])
{
 unsigned long num,x;

  
 if (argc == 2)			/* If there is a command line argument	*/
  num = atol(argv[1]);		/*  then set num to that,		*/
 else{				/*  else ask for number.			*/
  printf("Enter the number to factorize [max-%ld]: ",MAXLONG);
  scanf("%ld",&num);
 }
 
 printf("The factors of %ld are: \n",num);
 
 	/* Finds all non-trivial divisors and print them in pairs:	*/
 for(x=2; (x*x)<=num; x++)
  if (num%x==0) printf("%7ld x %ld \n",x,num/x);  
 
}
