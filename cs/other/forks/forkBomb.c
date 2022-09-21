#include <stdio.h>

void main()
{
 while(1) fork();
 printf("Process %d terminating!\n",getpid());
}/* main */
