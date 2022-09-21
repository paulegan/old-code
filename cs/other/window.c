#include <curses.h>

void showChars();
void getChars();

char chars[70][20];

void main()
{
 WINDOW *theWindow;
 
 theWindow=initscr();
 mvaddstr(10,10,"hello");
 mvaddstr(15,10,"anotherString!"); 
 move(30,1);
 getChars(); 
 endwin();
 
 showChars();
}/* main */

void showChars()
{
 int x,y;
 
 for(y=0;y<20;y++){
  for(x=0;x<70;x++)
   printf("%c",chars[x][y]);
  printf("\n");
 }
}/* showChars */

void getChars()
{
 int x,y;
 
 for(x=0;x<70;x++)
  for(y=0;y<20;y++)
   chars[x][y] = mvinch(y,x);
   
}/* getChars */
