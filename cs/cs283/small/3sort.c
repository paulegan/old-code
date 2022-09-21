/*             CS283/5 Exercise 1  Submission Date 10.11.1995			  */
/*										  */
/*      3Sort: A simple program to output given numbers in assending order.	  */
/*      Written By: Paul Egan (94312974} and Michael Cronin (94337454)		  */
/*      Program uses an array and swaps the numbers within same to create	  */
/*	the correct order for the numbers... These are then sent to output	  */
/*									       	  */
/*	NOTE: This program can be used from the command line... For Example	  */
/*	typing '3Sort 5 7 3' returns 'The numbers in ascending order are: 3 5 7'  */


#include <stdio.h>

void swap(int *ptr); 					/* Declares Function */

void main(int argc, char *argv[])
{
 int num[3];
 int *ptr = &num[0];
 int counter;

 if (argc == 4)						/* If from Command Line */				
  for(counter = 1;counter < 4;counter++)
   num[counter-1] = atoi(argv[counter]);
 else{							/* Asks for Numbers to sort */
  printf("Enter three integers, seperated by spaces: ");
  scanf("%d %d %d",ptr,ptr+1,ptr+2);
 }

 if (num[0] > num[1])					/* Checks for order in Numbers */
  swap(ptr);						/* And calls the swap function */
 if (num[1] > num[2]){
  swap(ptr+1);
  if (num[0] > num[1])
   swap(ptr);
 }

 printf("The numbers in ascending order are: %d %d %d \n",num[0],num[1],num[2]);

}


void swap(int *ptr)					/* Functions to swap element at  */
{							/* pointer with next element     */
 int temp = *ptr;

 *ptr = *(ptr+1);
 *(ptr+1) = temp;
}
