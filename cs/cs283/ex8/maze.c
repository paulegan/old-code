#include <stdio.h>
#include <sys/time.h>
#include "bool.h"
#include "xstuff.h"

#define NumOfRows	20
#define NumOfColumns	30
#define Failure		0
#define FoundExit	!(Failure)

enum mazeElement {path,hedge,breadCrumb,trampledBreadCrumb};
unsigned char maze[NumOfRows][NumOfColumns];

bool findExitFrom(int y, int x, windowType win);
void readMaze(int m);
void displayMaze(windowType win);
void delay(long time);
void initButtons(buttonInfo button[]);

void main()
{
 bool 	    success=FALSE;
 bool	    notFinished=TRUE;
 windowType win;
 unsigned   mazeNo;
 buttonInfo buttons[2];
 
 srandom((int)time(NULL));
 mazeNo=random()%5;
 
 initButtons(buttons);
 initX();
 makeWindow((NumOfColumns*BoxWidth)+10,(NumOfRows*BoxHeigth)+40,&win);
 drawButtons(buttons,2,win);

 while(notFinished){
  mazeNo=((mazeNo+1)%5)+1;
  readMaze(mazeNo);
 
  displayMaze(win);
  delay(999999);
  XPrint("Searching...        ",10,(NumOfRows*BoxHeigth)+25,win);

  success=findExitFrom(1,1,win);
 
  if (success)
   XPrint("Found exit!        ",10,(NumOfRows*BoxHeigth)+25,win);
  else
   XPrint("Couldn't find exit!",10,(NumOfRows*BoxHeigth)+25,win);
  
  if (getButton(buttons,2)==0)
   notFinished=FALSE;
 }/* while */

 closeWindow(win);
 closeX();
}/* main */

bool findExitFrom(int y, int x, windowType win)
{
 if (x==NumOfColumns-2 && y==NumOfRows-2)
  return(FoundExit);
  
 maze[y][x]=breadCrumb;
 drawBox((x*BoxWidth)+5,(y*BoxHeigth)+5,blue,win);
 flushDisplay();
 delay(99999);
 
 if (maze[y][x+1]==path)
  if (findExitFrom(y,x+1,win)==FoundExit) return(FoundExit);  
 if (maze[y+1][x]==path)
  if (findExitFrom(y+1,x,win)==FoundExit) return(FoundExit);  
 if (maze[y-1][x]==path)
  if (findExitFrom(y-1,x,win)==FoundExit) return(FoundExit);  
 if (maze[y][x-1]==path)
  if (findExitFrom(y,x-1,win)==FoundExit) return(FoundExit);  
 
 maze[y][x]=trampledBreadCrumb;
 drawBox((x*BoxWidth)+5,(y*BoxHeigth)+5,red,win);
 flushDisplay();
 
 return(Failure);
}/* findExitFrom */


void readMaze(int m)
{
 FILE *mazeFile;
 
 mazeFile = fopen("maze.data","rb");
 fseek(mazeFile,(m-1)*sizeof(maze),SEEK_SET);
 fread(maze,sizeof(maze),m,mazeFile);
 fclose(mazeFile);
 
}/* readMaze */


void displayMaze(windowType win)
{
 enum colours colour;
 int x,y;
 
 for(y=0;y<NumOfRows;y++)
  for(x=0;x<NumOfColumns;x++){
   switch(maze[y][x]){
    case path:
     colour = white;
    break;
    case hedge:
     colour = green;
    break;
    case breadCrumb:
     colour = blue;
    break;
    case trampledBreadCrumb:
     colour = red;
    break;
    default:
     colour = black;
    break;
   }
   /*printf("Drawing maze row=%d col=%d val=%d\n",y,x,maze[y][x]);*/
   drawBox((x*BoxWidth)+5,(y*BoxHeigth)+5,colour,win);
  }
 flushDisplay();
}/* displayMaze */

void initButtons(buttonInfo button[])
{
 strcpy(button[0].title,"Exit");
 button[0].x1=(NumOfColumns*BoxWidth)-190; 
 button[0].y1=(NumOfRows*BoxHeigth)+10;
 
 strcpy(button[1].title,"Go again");
 button[1].x1=(NumOfColumns*BoxWidth)-90; 
 button[1].y1=(NumOfRows*BoxHeigth)+10;
}/* initButtons */

void delay(long time)
{
 while(time--);
}/* delay */
