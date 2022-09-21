
#include <stdio.h>

#define PlayTime 200

int main()
{
 time_t	now,startTime;

 time(&startTime);
 time(&now);

 while((now-startTime)<PlayTime)
   time(&now);

 return(0);
}/* main */


