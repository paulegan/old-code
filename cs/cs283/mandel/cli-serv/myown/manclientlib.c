
/* 				manclientlib.c 					*/

/* function library for client program of client/server Mandelbrot generator	*/

/*	written by Paul Egan 21-12-95	contact: lumpy@freedom.ucc.ie		*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

#define NR_COLORS       22 


Display        *display;
Window 		win;
GC 		gc;
unsigned long	colorpix[NR_COLORS];	     /* array containing colours to use */


		/* window_connect makes connection to manserver window by	*/
		/*  reading window ID from the specified display's Cut Buffer	*/
		/* Also sets colorpix array and creates a graphics context	*/
void window_connect()
{
 char  		displayName[30];
 char 	       *WIDString;
 int   		lenOfWIDString;
 int   		c;
 unsigned long 	valuemask = 0;
 XGCValues     *values = NULL;
 Colormap	colormap; 
 XColor		colorA,colorB;
 char		colors[NR_COLORS][30] =
    {
     "white", "yellow", "gold", "orange", "coral", "red",
     "indian red", "aquamarine", "spring green", "lime green", "dark turquoise",
     "forest green", "violet", "magenta", "light blue", "medium aquamarine", 
     "medium blue", "blue", "medium turquoise", "navy", "midnight blue", "black"
    };

 printf("Enter the hostname for the server machine: \n");
 scanf("%s",displayName);				/* input hostname	*/
 strcat(displayName,":0");				/* use screen 0		*/
 
 if ((display=XOpenDisplay(displayName)) == NULL){
   fprintf(stderr,"This display name is invalid! \n");
   exit(-1);
 }
				/* get servers window ID from the Cut Buffer:	*/
 if ((WIDString = XFetchBytes(display,&lenOfWIDString)) == NULL){
   fprintf(stderr,"Couldn't find server.\n");
   exit(-1);
 }
 if (sscanf(WIDString,"MBWID: %u",&win) != 1){	  /* translate string to window */
   fprintf(stderr,"Couldn't find server.\n");
   exit(-1);
 }
 XFree(WIDString);

 if ((gc = XCreateGC(display, win, valuemask, values)) == BadMatch){
   fprintf(stderr,"Couldn't create graphics context!!\n");
   exit(-1);
 }
							/* set colorpix array:	*/
 colormap=DefaultColormap(display,DefaultScreen(display));
 for (c=0; c<NR_COLORS; c++){
   XAllocNamedColor(display,colormap,colors[c],&colorA,&colorB);
   colorpix[(NR_COLORS-(c+1))]=colorA.pixel;
 }
	
}/* window_connect */


void closedown()					/* free resources 	*/
{
 XFreeGC(display, gc);
 XCloseDisplay(display);
}/* closedown */


void drawpixel(unsigned int x, unsigned int y, int color)
{
 XSetForeground(display,gc,colorpix[color]);
 XDrawPoint(display,win,gc,x,y);
 XFlush(display);
}/* drawpixel */

