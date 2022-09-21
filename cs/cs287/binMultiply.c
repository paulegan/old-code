/*				CS287 - Assignment 1					*/

/*			Paul Egan (94312974) & Michael Cronin (94337454)		*/

/* binMultiply.c - A program to simulate the muliplication process of a 32-bit ALU.	*/
/*	This program reads two decimal numbers from standard input, converts them to	*/
/*	twos-compliment binary, and then multiplies them displaying the three registers	*/
/*	it uses in each step.  The result is then displayed in both binary and decimal.	*/
/* Note - This program does not conform to the exact specification given the 		*/
/* 	assignment sheet but covers the basic principles!  This program does not take	*/
/*	account of overflow.  The output requires a fairly large screen width!		*/

#include <stdio.h>
#include <math.h>
#include "bool.h"

#define WordSize 32

typedef unsigned char bitType;
typedef bitType aRegister[WordSize];			/* The register data type	*/

void convertBin2Dec(aRegister reg, int *number);
void convertDec2Bin(int number, aRegister reg);
void readReg(aRegister reg);
void printReg(aRegister reg);
void printRegisters(aRegister reg1, aRegister reg2, aRegister reg3);
void add(aRegister reg1, aRegister reg2);
void multiply(aRegister reg1, aRegister reg2);
void initialize(aRegister reg);
void negate(aRegister reg);
void reverse(aRegister reg);
void shiftRight(aRegister reg);
void shiftLeft(aRegister reg);


void main()
{
 aRegister reg1,reg2;
 int result;
 
 readReg(reg1);
 readReg(reg2);
 
 printf("\n%15s%35s%32s\n","Register 1","Register 2","Product");
 multiply(reg1,reg2);

 convertBin2Dec(reg1,&result);
 printf("\nThe result was %d : ",result);
 printReg(reg1);
 printf("\n\n");

}/* main */


void multiply(aRegister reg1, aRegister reg2)
{					/* Muliplies reg1 by reg2 and stores the result	*/
 aRegister product;			/*  in reg1.  Displays registers at each step.	*/
 int x;					/* reg2 and product are taken as one 64-bit	*/
 					/*  register.					*/
 initialize(product);
 add(product,reg2);
 initialize(reg2);
 
 for(x=0;x<WordSize;x++){
  printRegisters(reg1,reg2,product);
  if (*product==1)			/* If least significant bit of product = 1 ...	*/
   add(reg2,reg1);
  shiftRight(product);			/* Shift reg2 and product right together	*/
  *(product+WordSize-1)=*reg2;
  shiftRight(reg2);
 }
  
 initialize(reg1);
 add(reg1,product);
}/* multiply */

void add(aRegister reg1, aRegister reg2)
{					/* Adds reg1 to reg2 and stores result in reg1	*/
 int x;
 bitType carry=0;
 
 for(x=0;x<WordSize;x++,reg1++,reg2++){
  (*reg1)+=(*reg2)+carry;
  if (*reg1>1){
   carry=1;
   *reg1-=2;
  }
  else
   carry=0;
 }
   
}/* add */


void initialize(aRegister reg)
{					/* Sets reg to zero.				*/
 int x;
 
 for(x=0;x<WordSize;x++,reg++)
  *reg=0;
}/* initialize */

void readReg(aRegister reg)
{					/* Reads in a decimal number and stores it in	*/
 int aNumber;				/*  twos-compliment notation in reg.		*/
 
 initialize(reg);

 printf("Please enter a decimal number: ");
 scanf("%d",&aNumber);
 fflush(stdin);
 convertDec2Bin(aNumber,reg);
 printReg(reg);
 printf("\n");
  
}/* readReg */

void printReg(aRegister reg)
{					/* Display a register.				*/
 int x;
 
 reg+=WordSize-1;
 for(x=0;x<WordSize;x++,reg--)
  printf("%d",*reg);
 
}/* printReg */

void printRegisters(aRegister reg1, aRegister reg2, aRegister reg3)
{					/* Prints the 3 registers used in multiplication*/
 static int counter=0;
  
 printf("%3d: ",counter++);
 printReg(reg1);
 printf("   ");
 printReg(reg2);
 printf("   ");
 printReg(reg3);
 printf("\n");
 
}/* printRegisters */

void reverse(aRegister reg)
{					/* Changes all 1s to 0s and visa-versa for reg	*/
 int x;
 
 for(x=0;x<WordSize;x++,reg++)
  if ((*reg)==1) (*reg)=0; 
  else           (*reg)=1;
  
}/* reverse */

void negate(aRegister reg)		/* Sets reg = -reg.				*/
{
 aRegister one;

 reverse(reg);
 convertDec2Bin(1,one);
 add(reg,one);

}/* negate */


void convertDec2Bin(int number, aRegister reg)
{					/* Converts the decimal number to 		*/
 bool negative=FALSE;			/*  twos-compliment and stores it in reg.	*/
 bitType *regPtr=reg;
 
 initialize(reg);
 
 if (number<0){
  negative=TRUE;
  number=abs(number);
 }
 
 while(number>0){
  *reg++=number%2;
  number/=2;
 }
 *reg=number%2;
 
 if (negative)
  negate(regPtr);
  
}/* convert */

void convertBin2Dec(aRegister reg, int *number)
{					/* Converts a twos-compliment reg to decimal	*/
 int x;					/*   number.					*/
 bool negative=FALSE;
 
 if (*(reg+WordSize-1)){
  negative=TRUE;
  negate(reg);
 }
  
 *number=0;
 for(x=0;x<WordSize;x++,reg++)
  if (*reg==1)
   *number+=pow(2,x);
 
 if (negative){
  *number=-(*number);
  negate(reg-WordSize);
 }
}/* convertBin2Dec */

void shiftLeft(aRegister reg)
{					/* Shifts reg left				*/
 int x;
 
 reg+=WordSize-1;			/* Start at end and work back			*/
 for(x=1;x<WordSize;x++,reg--)
  *reg=*(reg-1);
 *reg=0;
}/* shiftLeft */

void shiftRight(aRegister reg)
{					/* Shift reg right				*/
 int x;
 
 for(x=1;x<WordSize;x++,reg++)
  *reg=*(reg+1);
 *reg=0;
  
}/* shiftRight */
