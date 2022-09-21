/*  			    CS 283/5 Exercise 3				*/
/* 			Submission Date: 24/11/1995			*/

/* Program: palin.c - A program that reads integers from input and	*/
/*   determines whether they they are palindromic. 			*/

/* Written by Paul Egan (94312974) and Michael Cronin (94337454).	*/

#include <stdio.h>
#include <limits.h>

#define SENTINEL -1


int reverseOf(int number);	      /* Function to reverse an integer */

void main()
{
 int theNum;
 
 printf("               Palindromic Number Tester \n");
 printf("         Input numbers between 0 and  %d \n",INT_MAX);
 printf("              Terminate session with a %d \n\n",SENTINEL);
 
 
 printf("Enter number: ");
 scanf("%d",&theNum);
 
 while(theNum != SENTINEL){		/* The reverse of a palindrome	*/
   if (theNum == reverseOf(theNum))			/* is itself. 	*/
	printf("%d is a palindrome \n",theNum);
   else
	printf("%d is not a palindrome \n",theNum);

   printf("Enter number: ");
   scanf("%d",&theNum);
 } /* while */

 printf("bye \n");
}


int reverseOf(int number)	      /* Function to reverse an integer */
{
 int temp;
 int x=1;
 
 for(temp=number/10; temp>0; temp/=10)		
	x*=10;				/* Sets x = 10^(num of digits)	*/
	
 for(temp=0; number>0; number/=10){
 	temp+=(number%10)*x;		/* Sets temp = reverse of number*/
 	x/=10;
 }
 
 return(temp);
}
