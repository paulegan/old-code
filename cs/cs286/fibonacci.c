#include <stdio.h>

#define NoInSequence

int fibNumByIteration(int n);
int fibNumByRecursion(int n);

void main()
{
 int x;
 
 for(x=0;x<300;x++)
  printf("%d,",fibNumByRecursion(x));
 printf("\n"); 

 for(x=0;x<300;x++)
  printf("%d,",fibNumByIteration(x));
 printf("\n"); 
}/* main */

int fibNumByIteration(int n)
{
 int x,num=1;
 int numMinusOne=1;
 int numMinusTwo=0;
 
 for(x=1;x<n;x++){
  numMinusTwo=numMinusOne;
  numMinusOne=num;
  num=numMinusOne+numMinusTwo;
 }
 return(num);
 
}/* fibNumByIteration */

int fibNumByRecursion(int n)
{
 if (n<2)
  return(1);
 else
  return(fibNumByRecursion(n-1)+fibNumByRecursion(n-2));

}/* fibRecurse */
