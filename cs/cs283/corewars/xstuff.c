
/*  			    		CS 283/5 Assignment 2						*/
/* 				      Submission Date: 2/2/1996						*/

/* xstuff.c - A library to provide X interface function to the MARS simulator.				*/

/* 		Written by Paul Egan (94312974) and Michael Cronin (94337454).				*/

/*	NOTE:	This file will not lclint - this is due more to the nature of X than our		*/
/*						programming ineptness!!	;-)				*/



#include <stdio.h>
#include <X11/Xlib.h>					/* The standard X11 header files		*/
#include <X11/Xutil.h>

#include "bool.h"					/* Standard boolean header file			*/
#include "xstuff.h"					/* Function prototypes and other		*/


Display        *display;				/* The main display to use			*/
int		screen;					/* Screen on display to use			*/
XEvent		report;					/* The structure for reporting of events	*/
unsigned long	colorpix[NR_COLORS];			/* Array holding color values			*/
 							


void initX()						/* Function to open connection to display	*/
{
    if ((display = XOpenDisplay("")) == NULL){
    	printf("ERROR Can't open display!");
        exit(-1);
    }
    
    screen = DefaultScreen(display);

}/* initX */

void closeX()						/* Function to close connection to display	*/
{
    XCloseDisplay(display);
}/* closeX */

void makeWindow(unsigned int width, unsigned int height, windowType *win)
{
    int 		c;					/* Counter variable			*/
    int 		WPosX = 0, WPosY = 0;			/* Window position			*/
    unsigned int    	border_width = 4;
    char               *window_name = "Core Wars";
    char               *icon_name = "Core Wars";
    Pixmap          	icon_pixmap = None;
    XSizeHints      	size_hints;				/* Window info passed to window manager	*/
    unsigned long   	valuemask = 0;				/* No vales set for the graphics 	*/
    XGCValues	       *values = NULL;				/*		context (GC)		*/
    Colormap		colormap;				/* The windows colour map		*/
    XColor 		colour;
    unsigned int 	rgbColours[NR_COLORS][3] = 		/* Colors used by windows		*/
    				{65535,65535,65535,   0    ,0    ,0,
    	 			 00000,0    ,0,       65535,0    ,0, 
    	 			 0    ,0    ,00000,   0    ,0    ,65535}; 
    					 
 
    win->id = XCreateSimpleWindow(display, RootWindow(display, screen),		/* Creates window	*/
                  WPosX, WPosY, width, height, border_width,
                  BlackPixel(display, screen),
                  WhitePixel(display, screen));
    
    
							/* Set the window info for the window manager	*/
    size_hints.flags = PPosition | PSize | PMinSize | PMaxSize;
    size_hints.x = WPosX;
    size_hints.y = WPosY;
    size_hints.width = width;
    size_hints.height = height;
    size_hints.min_width = width;
    size_hints.min_height = height;
    size_hints.max_width = width;
    size_hints.max_height = height;

    XSetStandardProperties(display, win->id, window_name, icon_name, 
                   	   icon_pixmap, NULL, 0, &size_hints);
                   	   
    XSelectInput(display, win->id, ButtonPressMask | 		/* Select events to be trapped by window*/
    				   ExposureMask |
    				   KeyPressMask);

    win->gc = XCreateGC(display, win->id, valuemask, values);	/* Create graphics context		*/
    
    if (XMapWindow(display, win->id)==BadWindow){		/* Display the window			*/
    	printf("ERROR Can't open window!");
	exit(-1);
    }
    
    colormap=DefaultColormap(display,screen);			/* Set the colors to be used by window	*/
    for(c=0;c<NR_COLORS;c++){
     colour.red = rgbColours[c][0];
     colour.green = rgbColours[c][1];
     colour.blue = rgbColours[c][2];
     XAllocColor(display,colormap,&colour);
     colorpix[c]=colour.pixel;
    }
    
    if (win->fontName==NULL) strcpy(win->fontName,"fixed");	/* Set the font to be used by window	*/
    if ((win->font = XLoadQueryFont(display,win->fontName))==NULL){
        printf("ERROR Can't load font! \n");
        exit(-1);
    }
    if (XSetFont(display,win->gc,win->font->fid)==BadAlloc){
        printf("ERROR Can't set font! \n");
        exit(-1);
    }

    XWindowEvent(display,win->id,ExposureMask,&report);		/* Wait for window to be placed		*/
    
}/* makeWindow */

void getString(char *str, int x, int y, windowType win)		/* Inputs a string at x,y in win	*/
{
    char   buf[5];
    KeySym keySymReturn;
    int    c,len;
    								/* Clear space for 40 characters	*/
    XClearArea(display,win.id,x,y-10,XTextWidth(win.font," ",1)*40,12,FALSE);

    for(c=0;c<40 && buf[0]!=13;c++){    			/* Loop until 40 chars or CR		*/
     XMaskEvent(display, KeyPressMask, &report);		/* Get next key press			*/
     len = XLookupString(&report.xkey,buf,sizeof(buf),&keySymReturn,NULL);	/* Return char to buf	*/
     buf[len] = '\0';						/* Terminate buf			*/
     if (buf[0]!=13){						/* If not CR add char to string and	*/
      str[c] = buf[0];						/*    print char			*/
      XPrint(buf,x+XTextWidth(win.font,buf,strlen(buf))*c,y,win);
     }
    }
    str[c-1] = '\0';						/* Terminate char			*/
    
}/* getString */

void XPrint(char *str,int x,int y, windowType win)	/* Clears area under text and outputs str at x,y*/
{    
    XClearArea(display,win.id,x,y-10,XTextWidth(win.font,str,strlen(str)),12,FALSE);

    XSetForeground(display, win.gc, colorpix[black]);
    XDrawString(display,win.id,win.gc,x,y,str,strlen(str));
    XFlush(display);
  
}/* XPrint */

void drawPos(int pos, enum colours colour, windowType win)
{							/* Draw a line of 'colour' at 'pos' in 'win'	*/

    XSetForeground(display,win.gc,colorpix[colour]);
    XDrawLine(display,win.id,win.gc,pos,0,pos,40);
    /* XFlush(display); */
 
}/* drawPos */

void drawRectangle(int x, int y, unsigned int width, unsigned int height,
			 enum colours colour, windowType win)			/* Draw a rectangle 	*/
{
    XSetForeground(display,win.gc,colorpix[colour]);
    XDrawRectangle(display,win.id,win.gc,x,y,width,height);
    
}/* drawRectangle */

void closeWindow (windowType win)				/* Close a window, freeing gc and font	*/
{
    XFreeFont(display, win.font);
    XFreeGC(display, win.gc);
    XDestroyWindow(display, win.id);
    
}/* closeWindow */

int getButton()							/* Used for final dialog box, wait for	*/
{								/*  for one of the buttons to be	*/
    while(TRUE){						/*  on and then returns which one	*/
     XMaskEvent(display,ButtonPressMask, &report);
     if (report.xbutton.y>53 && report.xbutton.y<74)
      if (report.xbutton.x>76 && report.xbutton.x<127)
       return(1);
      else if (report.xbutton.x>234 && report.xbutton.x<318)
       return(2);
    }/* while */

}/* getButton */
