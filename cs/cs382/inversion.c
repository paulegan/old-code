
#include <stdio.h>
#define  BlockLength	5
#define  RecordLength	7


void x2output(char x)			/*	CP: X_OUTPUT	seq	*/
{
 int finished=0;
 static int b=-1;
 static char record[RecordLength+1];

 do{					/*	  P: record	iter	*/

   while((++b)<RecordLength){		/*	    CP: byte	iter	*/

     if((record[b]=x)==EOF){
       finished=1;
       break;
     }
     else return;

   }					/*	    CP: byte	end	*/
   record[b] = '\0';
   b=-1;


   printf("%s\n",record);
 }while(!finished);			/*	  P: record	end	*/

 printf("%c",EOF);			/*	  P: eof		*/

}					/*	CP: X_OUTPUT	end	*/



void input2x()				/*	CP: INPUT_X	seq	*/
{
 int size,i,b;
 char block[BlockLength+1];

 scanf("%d",&size);			/*	  C: size		*/

 for(i=0;i<size;i++){			/*	  C: block	iter	*/
   scanf("%s",block);

   for(b=0;b<BlockLength;b++){		/*	    CP: byte	iter	*/
     x2output(block[b]);
   }					/*	    CP: byte	end	*/

 }					/*	  C: block	end	*/

 x2output(EOF);
}/* input2x */				/*	CP: INPUT_X	end	*/



void main()
{
  input2x();
}/* main */
