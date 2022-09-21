
/*                        CS 286 - Assignment 1                         */
/*                        Paul Egan  (94312974)                         */

/* fibonaci.c - Provides three functions that return the nth number     */
/*      in the Fibonacci sequence. One using recursion, one iterative   */
/*      and one using assembly language.                                */
/* note - For simplicity the return value is an integer.  This does     */
/*      mean that the range of these functions is limited!              */

#include <stdio.h>

int fibNumByAssembly(int n);
int fibNumByIteration(int n);
int fibNumByRecursion(int n);

#define  NoToPrintOut   20
void main()
{
 int x;                         /* A a test print out the first few in  */
				/*  in the sequence for each function.  */
 for(x=0;x<NoToPrintOut;x++)
  printf("%d,",fibNumByRecursion(x));
 printf("\n");

 for(x=0;x<NoToPrintOut;x++)
  printf("%d,",fibNumByIteration(x));
 printf("\n");

 for(x=0;x<NoToPrintOut;x++)
  printf("%d,",fibNumByAssembly(x));
 printf("\n");

}/* main */



int fibNumByAssembly(int n)
{
 __asm__ {
  mov ax,1;                     /* AX is fib(n)     "num"               */
  mov bx,0;                     /* BX is fib(n-1)   "numMinusOne"       */
  mov cx,0;                     /* CX is fib(n-2)   "numMinusTwo"       */
  mov dx,n;                     /* DX is the counter                    */
 }
 LoopLabel:
 __asm__ {
  mov cx,bx;                    /* fib(n-2) = fib(n-1)                  */
  mov bx,ax;                    /* fib(n-1) = fib(n)                    */
  mov ax,bx;                    /* fib(n) = fib(n-1)                    */
  add ax,cx;                    /* fib(n) += fib(n-2)                   */
  dec dx;
  jg  LoopLabel                 /* Keep looping n times                 */
 }
 return(_AX);                   /* return the value in the AX reg       */
}/* fibNumByAssembly */

int fibNumByIteration(int n)
{                               /* Fairly strightforward!               */
 int num=1;                     /* Same as above.                       */
 int numMinusOne=1;
 int numMinusTwo=0;

 while(--n>0){
  numMinusTwo=numMinusOne;
  numMinusOne=num;
  num=numMinusOne+numMinusTwo;
 }
 return(num);

}/* fibNumByIteration */

int fibNumByRecursion(int n)
{                               /* Doesn't deserve comment! 8-)         */
 if (n<2)
  return(1);
 else
  return(fibNumByRecursion(n-1)+fibNumByRecursion(n-2));

}/* fibRecurse */
