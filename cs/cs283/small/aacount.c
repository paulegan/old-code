/*  			    CS 283/5 Exercise 3				*/
/* 			Submission Date: 24/11/1995			*/

/* Program: aacount.c - A program that reads a passage of text 		*/
/*   termined by EOF, and counts the number of times the character a or */
/*   A appears next to itself.						*/

/* Written by Paul Egan (94312974) and Michael Cronin (94337454).	*/
 

#include <stdio.h>
#include "bool.h"		/* Boolean include file from cs283 dir.	*/


void main()
{
 int  ch = 0;					/* The input character	*/
 long count = 0;				
 bool lastWasA = FALSE;		      	  /* This flag is true when the */
 				      	  /*   last character was an a.	*/
 				      
 while(ch != EOF){
   ch = getchar();
  
   if (ch == 'a' || ch == 'A'){			/* If this char is an a	*/
     if (lastWasA == TRUE)			/* and the last one was */
       count++;					/* then increment count */
     lastWasA = TRUE;
   }
   else 					/* If char isn't an a...*/
     lastWasA = FALSE;
 } /* while */

 printf("The number of adjacent a's is %ld \n",count);
}
