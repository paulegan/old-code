#include <stdio.h>
#include "bool.h"

#define NoOfPrimes 150

int primes[NoOfPrimes];
int position=0;

bool isPrimeNumber(int n);
bool isTwinPrime(int n);

void main()
{
 int n=1;
 
 primes[position]=2;
 while(position<NoOfPrimes)
  if(isPrimeNumber(n+=2))
   primes[++position]=n;
 
 for(position=0;position<NoOfPrimes;position++)
  if(isTwinPrime(primes[position]))
   printf("%6d/%d \n",primes[position],primes[position+1]);

}/* main */

bool isTwinPrime(int n)
{
 if(isPrimeNumber(n) && isPrimeNumber(n+2))
  return(TRUE);
 else
  return(FALSE);

}/* isTwinPrime */

bool isPrimeNumber(int n)
{
 int x;
 
 if (n<2) return(FALSE);
 
 for(x=(position-1);x>=0;x--)
  if((n%primes[x])==0)
   return(FALSE);
 return(TRUE);

}/* isPrimeNumber */
