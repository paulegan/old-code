
#include <stdio.h>
#define  RecordLength	7


void x2output()				/*	CP: X_OUTPUT	seq	*/
{
 int finished=0;
 static int b=-1;
 static char record[RecordLength+1];
 FILE *x = fopen("X","r");

 do{					/*	  P: record	iter	*/

   while((++b)<RecordLength){		/*	    CP: byte	iter	*/

     if(fscanf(x,"%c",record[b])==EOF){
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

 fclose(x);
}					/*	CP: X_OUTPUT	end	*/


void main()
{
  x2output();
}/* main */
