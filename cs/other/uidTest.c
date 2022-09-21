#include <stdio.h>
#include <unistd.h>

extern int setreuid(int, int);

void main()
{
 int UID;
 
 setreuid(2068,2068);
 UID = getuid();
 printf("User ID is %d.",UID);
}
