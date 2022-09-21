
/*  			   	CS 283/5 Exercise 8				*/
/* 		             Submission Date: 1/3/1996				*/

/* xstuff.h - Function prototypes for xstuff.c and related constants		*/

/* 			  Written by Paul Egan (94312974)			*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define NoOfColours 	5		/* The number of colours defined	*/
#define BoxWidth	20
#define BoxHeigth	20
					/* The defined colours:			*/
enum colours {white,black,red,green,blue};

typedef struct{				/* Information on each window		*/
 Window       id;
 GC           gc;
 XFontStruct *font;
 char	      fontName[10];
}windowType;

typedef struct{
 char	  title[20];
 unsigned x1,y1;
 unsigned x2,y2;
}buttonInfo;
	
					/* Opens connection to display:		*/
void initX();
					/* Closes connection to display:	*/
void closeX();
					/* Creates a window and returns info:	*/
void makeWindow(unsigned int width, unsigned int height, windowType *win);
					/* Draws a box 				*/
void drawBox(int x, int y, enum colours colour, windowType win);
					/* Prints out a string at x,y in win:	*/
void XPrint(char *str,int x,int y, windowType win);
					/* Inputs a string at x,y in win:	*/
void getString(char *str, int x, int y, windowType win);
					/* Close the window and frees gc & font:*/
void closeWindow(windowType win);
					/* Draws the buttons in win		*/
void drawButtons(buttonInfo button[], unsigned noOfButtons, windowType win);
					/* Returns which button was clicked on:	*/
int  getButton(buttonInfo buttons[], unsigned noOfButtons);
