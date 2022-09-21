
/*  			   CS 283/5 Assignment 2				*/
/* 		         Submission Date: 2/2/1996				*/

/* xstuff.h - Function prototypes for xstuff.c and related constants		*/

/* 	Written by Paul Egan (94312974) and Michael Cronin (94337454).		*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define WINWIDTH  	1000		/* The main window width		*/
#define WINHEIGHT 	40		/* The main window height		*/

#define NR_COLORS 	6		/* The number of colours defined	*/

					/* The defined colours:			*/
enum colours {white,black,red,darkRed,blue,darkBlue};

typedef struct{				/* Information on each window		*/
 Window       id;
 GC           gc;
 XFontStruct *font;
 char	      fontName[10];
}windowType;

					/* Opens connection to display:		*/
void initX();
					/* Closes connection to display:	*/
void closeX();
					/* Creates a window and returns info:	*/
void makeWindow(unsigned int width, unsigned int height, windowType *win);
					/* Draw a line at pos in win:		*/
void drawPos(int pos, enum colours colour, windowType win);
					/* Prints out a string at x,y in win:	*/
void XPrint(char *str,int x,int y, windowType win);
					/* Close the window and frees gc & font:*/
void getString(char *str, int x, int y, windowType win);
					/* Returns which button was clicked on:	*/
void closeWindow(windowType win);
					/* Inputs a string at x,y in win:	*/
int  getButton();
