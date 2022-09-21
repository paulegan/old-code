#include <stdio.h>
#include "bool.h"

#define NoOfPrimes 510

int primes[NoOfPrimes];
int position=0;

bool isPrimeNumber(int n);
 
void main()
{
 int n=1;
 
 primes[position]=2;
 while(position<NoOfPrimes)
  if(isPrimeNumber(n+=2))
   primes[++position]=n;
 
 for(n=0;n<NoOfPrimes;n++){
  printf("%6d",primes[n]);
  if (((n+1)%15)==0)
   printf("\n");
 }
 printf("\n");
}/* main */

bool isPrimeNumber(int n)
{
 int x;
 
 for(x=position;x>=0;x--)
  if((n%primes[x])==0)
   return(FALSE);
 return(TRUE);

}/* isPrimeNumber */
