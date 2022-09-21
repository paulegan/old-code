#include <stdio.h>

void main()
{
 void *mem;

 while(fork()>=0);
 wait();
 printf("Process %d terminating!\n",getpid());
}/* main */
