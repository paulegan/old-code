
#include <stdio.h>
#define  BlockLength	5


void input2x()				/*	CP: INPUT_X	seq	*/
{
 int size,i,b;
 char block[BlockLength+1];
 FILE *x = fopen("X","w");

 scanf("%d",&size);			/*	  C: size		*/

 for(i=0;i<size;i++){			/*	  C: block	iter	*/
   scanf("%s",block);

   for(b=0;b<BlockLength;b++){		/*	    CP: byte	iter	*/
     fprintf(x,"%c",block[b]);
   }					/*	    CP: byte	end	*/

 }					/*	  C: block	end	*/

}/* input2x */				/*	CP: INPUT_X	end	*/



void main()
{
  input2x();
}/* main */
