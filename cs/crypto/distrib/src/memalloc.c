
#include <stdio.h>
#include <sys/types.h>

void *memalloc(size_t size)
{
 void *ptr = (void*)malloc(size);

 if (!ptr){
   fprintf(stderr,"Out of memory! \n");
   exit(-1);
 }
 
 return(ptr);
}/* memalloc */
