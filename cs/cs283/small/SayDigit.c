/*             CS283/5 Exercise 1  Submission Date 10.11.1995			*/
/*										*/
/*      SayDigit: A simple program for converting inputed digits to words.	*/
/*      Written By: Paul Egan (94312974} and Michael Cronin (94337454)		*/
/*      Program uses an array of pointers to strings containing the words	*/
/*	which describe the numbers... This array is called with the input 	*/
/*	after the check for positive or negative				*/
/*										*/
/*	NOTE: This program can be used from the command line... For Example	*/
/*	typing 'SayDigit 5' returns '5 = five'.					*/


#include <stdio.h>

void main(int argc, char *argv[])
{
 char *nameOfDigit[] = {"zero","one","two","three","four","five",
			"six","seven","eight","nine"};	/* Sets the number strings */	
 int num;

 if (argc == 2)						/* If two arguments passed to main */
  num = atoi(argv[1]);			
 else{
  printf("%s","Input a single digit number: ");
  scanf("%d",&num);
 }							/* terminates else clause */

 if (num >= 0 && num < 10)				/* If number is positive and less than 10 */
   printf("%d = %s\n",num,nameOfDigit[num]);
 else if (num > -10 && num < 0)				/* Checks if number in range -1 -> -10 */
   printf("%d = minus %s\n",num,nameOfDigit[-num]);	
 else							/* Else invalid number from input */
   printf("ERROR %d is not a single digit\n",num);

}
