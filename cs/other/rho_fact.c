#include <stdio.h>

int gcd(int a,int b);

void main()
{
 int num,i,x,y,k,d;
 
 printf("input: ");
 scanf("%d",&num);
 
 i=1;
 x=rand()%num;
 y=x;
 k=2;
 while(1){
  i++;
  x=((x*x)-1)%num;
  d=gcd(y-x,num);
  if (d!=1 && d!=num) printf("output: %d \n",d);
  if (i==k){
   y=x;
   k*=2;
  }
  printf("%d,",d);
 }
}

int gcd(int a,int b)
{
 if (b==0)
  return(a);
 else
  return(gcd(b,a%b));
}
