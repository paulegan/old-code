#include <stdio.h>

#define NumOfRows	20
#define NumOfColumns	30

enum mazeBox {path,hedge,breadCrumb,trampledBreadCrumb};

void main()
{
 FILE *mazeFile;
 unsigned char maze1[NumOfRows][NumOfColumns]=
 	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	 1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1, 
  	 1,1,0,1,0,0,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1,
  	 1,0,0,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,1,0,1,
  	 1,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,
  	 1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,
  	 1,0,1,0,1,0,0,0,0,0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,
  	 1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,
  	 1,0,1,0,1,0,1,0,1,0,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,
  	 1,0,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,
  	 1,0,1,0,1,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,
  	 1,0,1,0,1,0,1,1,0,0,0,1,0,1,1,0,1,0,0,1,0,0,0,1,0,1,0,1,0,1,
  	 1,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,
  	 1,0,1,0,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,
  	 1,0,1,0,1,1,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0,1,
  	 1,0,1,0,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,1,1,1,
   	 1,0,1,0,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,1,1,1,1,1,1,1,0,0,0,1,
  	 1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,0,0,0,0,1,0,1,0,1,
  	 1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
 unsigned char maze2[NumOfRows][NumOfColumns]=
 	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	 1,0,0,0,1,0,0,0,0,0,0,1,0,1,0,1,1,1,1,1,0,0,1,0,0,0,1,0,0,1, 
  	 1,1,1,0,1,0,1,1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,1,0,1,0,1,0,1,1,
  	 1,0,1,0,1,0,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,0,1,0,0,1,
  	 1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,1,
  	 1,0,0,0,1,0,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,0,0,0,1,0,0,0,0,1,
  	 1,1,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,1,0,1,1,1,1,1,1,
  	 1,1,0,1,1,1,1,1,0,0,0,1,0,1,0,1,0,1,1,1,0,1,1,0,0,0,0,0,0,1,
  	 1,0,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,1,0,1,1,1,1,1,1,
  	 1,0,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,1,
  	 1,0,1,1,0,0,1,1,1,0,0,0,0,1,0,1,1,1,0,1,0,0,1,1,1,1,1,1,1,1,
  	 1,0,1,1,0,1,1,1,1,0,1,0,0,0,0,0,0,1,0,1,0,1,1,1,1,1,1,1,0,1,
  	 1,0,0,1,0,0,0,0,0,0,1,1,1,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,1,
  	 1,1,0,1,1,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,
  	 1,1,0,0,0,0,1,1,1,1,0,1,0,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
  	 1,1,0,1,1,0,1,1,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,
   	 1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,1,1,1,0,1,1,1,1,1,1,0,0,0,1,
  	 1,1,0,1,1,0,1,1,0,1,0,0,0,1,1,0,1,1,1,0,1,1,1,0,0,1,0,1,1,1,
  	 1,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
 unsigned char maze3[NumOfRows][NumOfColumns]=
 	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	 1,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, 
  	 1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,
  	 1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,
  	 1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,
  	 1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,1,1,0,1,1,1,0,0,1,1,1,1,1,0,1,
  	 1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  	 1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,1,1,0,1,1,0,1,1,1,0,1,1,1,0,1,
  	 1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  	 1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,
  	 1,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  	 1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1,
  	 1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,
  	 1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,
  	 1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,
  	 1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,
   	 1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,
  	 1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,
  	 1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
 unsigned char maze4[NumOfRows][NumOfColumns]=
 	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	 1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   	 1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	 1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	 1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	 1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	 1,0,1,1,1,1,0,1,1,0,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,1,
  	 1,0,1,1,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,
  	 1,0,1,1,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,
  	 1,0,1,1,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,
  	 1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
 unsigned char maze5[NumOfRows][NumOfColumns]=
 	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	 1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  	 1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	 1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  	 1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,
  	 1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  	 1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	 1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,
  	 1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,
  	 1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,
  	 1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,0,1,0,1,0,0,0,0,0,0,0,1,
  	 1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,
  	 1,0,1,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,1,
  	 1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,
  	 1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,
  	 1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,
  	 1,0,1,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
  	 1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,
  	 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  	 
 mazeFile = fopen("maze.data","wb");
  fwrite(maze1,sizeof(maze1),1,mazeFile);  
  fwrite(maze2,sizeof(maze2),1,mazeFile);
  fwrite(maze3,sizeof(maze3),1,mazeFile);  
  fwrite(maze4,sizeof(maze4),1,mazeFile);
  fwrite(maze5,sizeof(maze3),1,mazeFile);  
 fclose(mazeFile);

}/* main */
