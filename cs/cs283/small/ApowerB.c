/*			   CS 283/5 Assignment 2			*/
/* 			Submission Date: 17/11/1995			*/

/* Program: ApowerB.c - A program to input two integers and output	*/
/*  the first to the power of the second.				*/

/* Written by Paul Egan (94312974) and Michael Cronin (94337454).	*/


#include <stdio.h>

void main()
{
 int num,pow;					/* The two input values */
 long result;		      	  

 printf("Input two positive integers seperated by a space: ");
 scanf("%d %d",&num,&pow);
 printf("%d to the power of %d = ",num,pow);
 
 for(result=1; pow>0; pow--)			/* Compute power. 	*/
	result*=num;

 printf("%ld \n",result);
}
