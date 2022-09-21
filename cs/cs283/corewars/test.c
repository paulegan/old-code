#include <stdio.h>
#include <X11/Xlib.h>
#include "xstuff.h"
#include "bool.h"

void openFile(int w, FILE **wFile);

bool usersChoice(int w);

void main()
{
 bool thing;
 
 initX();
 thing = usersChoice(3);
 closeX();
 
}/* main */


bool usersChoice(int w)
{
 windowType  dialogBox={NULL,NULL,NULL,"9x15"};
 bool	     doEnd=FALSE;
 
 makeWindow(403,100,&dialogBox);
 if (w == 1)
  XPrint("Warrior 1 [blue] has lost!",85,30,dialogBox);
 else if (w == 2)
  XPrint("Warrior 2 [red] has lost!",85,30,dialogBox);
 else
  XPrint("A stalemate was reached!",85,30,dialogBox);
  
 drawRectangle(77,54,48,19,black,dialogBox);
 XPrint("EXIT",82,70,dialogBox);
 drawRectangle(235,54,81,19,black,dialogBox);
 XPrint("GO AGAIN",240,70,dialogBox);
 
 processEvents(dialogBox);
 
}/* usersChoice */
