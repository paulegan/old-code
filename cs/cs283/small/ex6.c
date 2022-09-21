
/* 			      CS283/5  Exercise 6			*/
/*				  9-2-1996				*/

/* 			    Paul Egan ( 94312974 )			*/

/*  ex6.c - c implementation of the 6 functions specified in ex6.lcl	*/


#include "../bool.h"

int what1(int a,int b,int c)
{
 assert (a<=b);
  
 if (a<c) return(a);
     else return(c);
      
}/* what1 */


int what2(int a)
{
 assert (a>0);
   
 return(a);
}/* what2 */


bool what3(int a)
{  
 assert (a>0);
 
 return FALSE;
}/* what3 */


int what4(int a,int b)
{   
 int r=a;

 while (r>=b)
  r=r-b;
  
 return(r);
}/* what4 */


int what5(int a,int b)
{
 int i,z,r=0;

 z=a/b;
 for (i=0;i<b;i++)
  r=r+z;
  
 return(r);
}/* what5 */


int what6(int a, int b)
{
 int m;
 
 if (a<b) m=b;
     else m=a;
 while (!((m%a==0)&&(m%b==0)))
  m++;
  
 return(m);
}/* what6 */

