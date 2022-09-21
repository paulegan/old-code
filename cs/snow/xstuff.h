
/*	xstuff.h - Header file for xstuff.c		*/

/*	lumpy@cs.ucc.ie		4.3.1998		*/

#ifndef xstuff_header
#define xstuff_header

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define MAXWWidth	1000
#define MAXWHeight	1000
#define NumColours 	6

Display*	display;
Window		win;
GC		gc;
Colormap	colourMap;
XFontStruct*	font;
XEvent		report;
XColor		colours[NumColours];


void openWindow(char *name, unsigned int width, unsigned int height);
void closeWindow();
void XPrint(char *str,int x,int y);
void waitForMouseClick();

#endif
