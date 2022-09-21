/* 				server.c 					*/

/* 	server program for client/server Mandelbrot generator			*/

/*	written by Paul Egan 19-12-95	contact: lumpy@freedom.ucc.ie		*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

#include "manclientlib.h"
#include "bool.h"

void makeWindow(unsigned int width, unsigned int height);

Display        *display;
Window 		win;

int main(int argc, char *argv[])
{
 char 	WIDString[20];
 bool 	notDone = TRUE;
 XEvent report;
 
 makeWindow(WINSIZE,WINSIZE);
 
 sprintf(WIDString,"MBWID: %u",win);		/* Put the window ID into the	*/
 XStoreBytes(display,WIDString,strlen(WIDString));		/* Cut Buffer	*/

 while (notDone){				/* Main event loop		*/
  XNextEvent(display, &report);
  switch (report.type){
     case ButtonPress:
     case KeyPress:
       notDone = FALSE;				/* Exit on button or key press	*/
     break;
  }/* switch */
 }/* while */

 XStoreBytes(display,"",0);				/* Clear Cut Buffer	*/
 XDestroyWindow(display,win);
 XCloseDisplay(display);

}/* main */


void makeWindow(unsigned int width, unsigned int height)
{
    int             	screen;
    int 		WPosX = 0, WPosY = 0;
    unsigned int    	border_width = 4;
    char               *window_name = "Mandelbrot";
    char               *icon_name = "Mandel";
    Pixmap          	icon_pixmap = None;
    XSizeHints      	size_hints;

    if ((display = XOpenDisplay("")) == NULL){
    	printf("ERROR Can't open display!");
        exit(-1);
    }
    
    screen = DefaultScreen(display);
 
    win = XCreateSimpleWindow(display, RootWindow(display, screen),
                  WPosX, WPosY, width, height, border_width,
                  BlackPixel(display, screen),
                  WhitePixel(display, screen));
    
    size_hints.flags = PPosition | PSize | PMinSize;
    size_hints.x = WPosX;
    size_hints.y = WPosY;
    size_hints.width = width;
    size_hints.height = height;
    size_hints.min_width = width;
    size_hints.min_height = height;

    XSetStandardProperties(display, win, window_name, icon_name, 
                   	   icon_pixmap, NULL, 0, &size_hints);
                   	   
    XSelectInput(display, win, 	ButtonPressMask | ExposureMask | KeyPressMask);

    if (XMapWindow(display, win)==BadWindow){
    	printf("ERROR Can't open window!");
	exit(-1);
    }
    
}/* makeWindow */
