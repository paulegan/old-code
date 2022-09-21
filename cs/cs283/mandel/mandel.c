/* 
/*  			    		CS 283/5 Assignment 1						*/
/* 				      Submission Date: 8/12/1995					*/

/* mandel.c: A program to display and navigate the mandelbrot set.					*/
/* Operations of note:  Left click enlarges the set at mouse cursor by MAG_STEP				*/
/*			Right click moves back by MAG_STEP, keeping centre of plot as new centre	*/
/*			Middle button quits program							*/
/*			Resizing and exposing of window is dealt with					*/
/*			Increases infinity with size to prevent blockiness at low sizes			*/

/* 		Written by Paul Egan (94312974) and Michael Cronin (94337454).				*/

/*	NOTE:	This program will not lclint - this is due more to the nature of X than our		*/
/*						programming ineptness!!	;-)				*/



#include <stdio.h>
#include <X11/Xlib.h>					/* The standard X11 header files		*/
#include <X11/Xutil.h>
#include "bool.h"					/* Standard boolean header file			*/

#define STARTWWidth 	250				/* The starting dimensions of the window	*/
#define STARTWHeight 	270
#define MAXWWidth	500				/* The maximum dimensions of the window		*/
#define MAXWHeight	540

#define MAG_STEP	10				/* The magnification step			*/
#define START_INF 	50				/* The starting value for infinity		*/
#define INF_FUNC(n)	START_INF+(2*n*MAG_STEP)	/* Function defining how infinity changes with	*/
#define NR_COLORS 	22				/* The number of colors */	/* plot size	*/
#define START_X	 	-2				/* The starting values for plot corner and size	*/
#define START_Y	 	-1.16
#define START_S	 	2.5


Display        *display;
Window          win;
unsigned int	WWidth, WHeight;			/* The current dimensions of the window		*/
unsigned int	PWidth, PHeight;			/* The current dimensions of the plot		*/
GC              gc;					/* The graphics context				*/
XFontStruct    *font;
XEvent		report;					/* The structure for reporting of events	*/
unsigned long	colorpix[NR_COLORS];			/* Array holding color values			*/
char		thePlot[MAXWWidth][MAXWHeight];		/* Array to hold the plot			*/
int		magFactor=0;				/* The current magnification factor		*/
 							/*  ( used for INF_FUNC )			*/
 							
							/* The function prototypes:			*/
void displayOpeningMessage();				
void plotMandel(double xCorn, double yCorn, double size);
void drawPlot(double xCorn, double yCorn, double size);
void makeWindow(unsigned int width, unsigned int height);
void XPrint(char *str,int x,int y);
void XPrintPos(double x,double y,double s);
void closeWindow();


int main()
{
 double		xCorn=START_X, yCorn=START_Y, size=START_S;	/* The current corner and size of plot	*/
 bool 		doEnd=FALSE;					/* If true then end program		*/
 unsigned long 	lastExposeEvent=0;				/* Holds the serial no. of last 	*/
 								/*		expose event		*/
 								
 makeWindow(STARTWWidth,STARTWHeight);				/* Create the window, drawables, fonts	*/
 displayOpeningMessage();				
 plotMandel(xCorn,yCorn,size);					/* Plot the mandelbrot set to thePlot	*/
								/*				array	*/
								
 XWindowEvent(display,win,ButtonPressMask,&report);		/* Wait for button press		*/
 if (report.xbutton.button==2)					/* If middle button then exit		*/
  doEnd = TRUE;
 else
  drawPlot(xCorn,yCorn,size);					/* else output thePlot to the window	*/
 
 while(doEnd==FALSE){						/* While user doesn't want to exit	*/
  XNextEvent(display, &report);					/* Get the next event			*/
  switch(report.type){
    case ConfigureNotify:					/* If window has been resized		*/
      WWidth = report.xconfigure.width;				/* Set the current window dimensions	*/
      WHeight = report.xconfigure.height;			/*  to the new dimensions		*/
    break;
    case Expose:			/* If the window has been exposed (created,uncovered or resized)*/
      if ((report.xexpose.count  == 0) && 			/* Make sure that this is the last	*/
      	  (report.xexpose.serial != lastExposeEvent)){		/* expose event on the event queue	*/
	        drawPlot(xCorn,yCorn,size);			/* Output thePlot to the window		*/
	        lastExposeEvent = report.xexpose.serial;
      }
    break;
    case ButtonPress:						/* If mouse button is pressed		*/
      if (report.xbutton.button==1){				/* If it was left button		*/
        xCorn+=size*(((double)report.xbutton.x/WWidth)-(0.5/MAG_STEP));	/* Set corner to new values	*/
        yCorn+=size*(((double)report.xbutton.y/WHeight)-(0.5/MAG_STEP));
        size/=MAG_STEP;
        magFactor++;
        plotMandel(xCorn,yCorn,size);				/* Plot with new corner and size	*/
        drawPlot(xCorn,yCorn,size);				/* Output thePlot to the window		*/
      }
      if (report.xbutton.button==3){				/* If it was right button		*/
        xCorn+=size*(1-MAG_STEP)/2;				/* Set corner to new values		*/
        yCorn+=size*(1-MAG_STEP)/2;				/* Note: ignores mouse position		*/
        size*=MAG_STEP;
        magFactor--;
        plotMandel(xCorn,yCorn,size);				/* Plot with new corner and size	*/
        drawPlot(xCorn,yCorn,size);				/* Output thePlot to window		*/
      }
      if (report.xbutton.button==2)				/* If middle button			*/
        doEnd=TRUE;						/*  then exit				*/
    break;
  }/* switch */
 }/* while */
 
 closeWindow();							/* Close window and drawable. Free font	*/
 
}/* main */



void plotMandel(double xCorn, double yCorn, double size)
{
 struct complex{double a,b;} z,c;				/* Declare z and c as complex numbers	*/
 double zaSq,zbSq;						/* Variables to hold (z.a)^2 & (z.b)^2	*/
 int j,k,count;
 int infinity=INF_FUNC(magFactor);				/* Infinity increases as size decreases	*/
 
 XPrint("Working...",5,13);					/* Tells the user whats going on!	*/
 
 for(j=1;j<(WWidth-2);j++)					/* Standard mandelbrot algorithm:	*/
   for(k=1;k<WHeight;k++){
     count=0;
     c.a=xCorn+j*size/(WWidth-2);
     c.b=yCorn+k*size/WHeight;
     z.a=z.b=0;
     zaSq=zbSq=0;
     while((count<infinity) && ((zaSq+zbSq)<4)){
       count++;
       zaSq=(z.a*z.a);
       zbSq=(z.b*z.b);
       z.b=(2 * z.a * z.b)+c.b;
       z.a=zaSq-zbSq+c.a;
     }
     								/* Stores the colour in thePlot array	*/
     if (count<infinity) thePlot[j][k]=(char)(count*(NR_COLORS-1)/infinity)+1;
     	       	    else thePlot[j][k]=0;			/* rather than ouputting to screen	*/
     
   } /* for */
   
 PWidth = WWidth;						/* Set new plot dimensions		*/
 PHeight = WHeight;
 
}/* plotMandel */


void drawPlot(double xCorn, double yCorn, double size)
{
 int j,k;
 
 for(j=1;j<(WWidth-2);j++)					/* Outputs thePlot array to window	*/
  for(k=1;k<(WHeight-25);k++){
    XSetForeground(display, gc, colorpix[(unsigned long)thePlot[j*PWidth/WWidth]
    							       [k*PHeight/WHeight]]);
    XDrawPoint(display,win,gc,j,k); 
  }
 XPrintPos(xCorn,yCorn,size);					/* Writes out current values for the	*/
 								/*    corner, size and infinity		*/
 XFlush(display);						/* Flushes output from buffer		*/
}/* drawPlot */
    
    
void makeWindow(unsigned int width, unsigned int height)
{
    int 		c;					/* Counter variable			*/
    int 		WPosX = 0, WPosY = 0;			/* Window position			*/
    int             	screen;					/* Screen descriptor			*/
    unsigned int    	border_width = 4;
    char               *display_name = NULL;
    char               *window_name = "Mandelbrot";
    char               *icon_name = "Mandelbrot";
    Pixmap          	icon_pixmap = None;
    XSizeHints      	size_hints;				/* Window info passed to window manager	*/
    unsigned long   	valuemask = 0;				/* No vales set for the graphics 	*/
    XGCValues	       *values = NULL;				/*		context (GC)		*/
    Colormap		colormap;				/* The windows colour map		*/
    XColor 		colorA,colorB;
    char 		colors[NR_COLORS][30] =			/* Names of the colours to be used:	*/
    {
     "white", "yellow", "gold", "orange", "coral", "red",
     "indian red", "aquamarine", "spring green", "lime green", "dark turquoise",
     "forest green", "violet", "magenta", "light blue", "medium aquamarine", 
     "medium blue", "blue", "medium turquoise", "navy", "midnight blue", "black"
    };

    if ((display = XOpenDisplay(display_name)) == NULL){	/* Get the display server		*/
    	printf("ERROR Can't open display!");
        exit(-1);
    }
    
    screen = DefaultScreen(display);
 
    win = XCreateSimpleWindow(display, RootWindow(display, screen),	/* Creates window		*/
                  WPosX, WPosY, width, height, border_width,
                  BlackPixel(display, screen),
                  WhitePixel(display, screen));
    
    WWidth = width; WHeight = height;
    
							/* Set the window info for the window manager	*/
    size_hints.flags = PPosition | PSize | PMinSize | PMaxSize;
    size_hints.x = WPosX;
    size_hints.y = WPosY;
    size_hints.width = width;
    size_hints.height = height;
    size_hints.min_width = width;
    size_hints.min_height = height;
    size_hints.max_width = MAXWWidth;
    size_hints.max_height = MAXWHeight;

    XSetStandardProperties(display, win, window_name, icon_name, 
                   	   icon_pixmap, NULL, 0, &size_hints);
                   	   
    XSelectInput(display, win, 	ButtonPressMask | 		/* Select events to be trapped by window*/
    				ExposureMask |
    				StructureNotifyMask);

    gc = XCreateGC(display, win, valuemask, values);		/* Create graphics context		*/
    
    if (XMapWindow(display, win)==BadWindow){			/* Display the window			*/
    	printf("ERROR Can't open window!");
	exit(-1);
    }
    
    colormap=DefaultColormap(display,screen);			/* Set the colors to be used by window	*/
    for (c=0; c<NR_COLORS; c++){
        XAllocNamedColor(display,colormap,colors[c],&colorA,&colorB);
        colorpix[(NR_COLORS-(c+1))]=colorA.pixel;
    }
    
    if ((font = XLoadQueryFont(display,"fixed"))==NULL){	/* Set the font to be used by window	*/
        printf("ERROR Can't load font! \n");
        exit(-1);
    }
    
    XWindowEvent(display,win,ExposureMask,&report);		/* Wait for window to be placed		*/
    
}/* makeWindow */


void XPrint(char *str,int x,int y)		/* Clears area under text and outputs str at x,y	*/
{    
    XClearArea(display,win,x,y-10,XTextWidth(font,str,strlen(str)),12,FALSE);

    XSetForeground(display, gc, colorpix[0]);
    XDrawString(display,win,gc,x,y,str,strlen(str));
    XFlush(display);
  
}/* XPrint */


void XPrintPos(double x,double y, double s)	/* Outputs the current corner, size and infinity values	*/
{
    char str[60];
    int  SLength;
    
    sprintf(str,"Corner: (%g + %gi)   ",x,y);
    XPrint(str,5,WHeight-13);
    sprintf(str,"Size: %g ",s);
    XPrint(str,17,WHeight-3);
    SLength=XTextWidth(font,str,strlen(str));
    sprintf(str,"Infinity: %d ",INF_FUNC(magFactor));
    XPrint(str,17+SLength,WHeight-3);
    
}/* XPrintPos */


void displayOpeningMessage()					/* I think you can figure this one out!!*/
{
    XFontStruct		*bigFont;
    
    if ((bigFont = XLoadQueryFont(display,"9x15"))==NULL){
        printf("ERROR Can't load font! \n");
        exit(-1);
    }
    if (XSetFont(display,gc,bigFont->fid)==BadAlloc){
        printf("ERROR Can't set font! \n");
        exit(-1);
    }
   
    XPrint("Mandelbrot Plotter",40,30);
    XPrint("written by",75,50);
    XPrint("Lumpy & Fred",67,70);
    XPrint("Right Mouse Button:",30,170);
    XPrint("Zoom in",50,183);
    XPrint("Left Mouse Button:",30,200);
    XPrint("Zoom out",50,213);
    XPrint("Middle Mouse Button:",30,230);
    XPrint("Exit Program",50,243);

    XFreeFont(display, bigFont);
    if (XSetFont(display,gc,font->fid)==BadAlloc){
        printf("ERROR Can't set font! \n");
        exit(-1);
    }

}/* displayOpeningMessage */


void closeWindow ()						/* Clear up window, gc and font		*/
{
    XFreeFont(display, font);
    XFreeGC(display, gc);
    XCloseDisplay(display);
    
}/* closeWindow */


