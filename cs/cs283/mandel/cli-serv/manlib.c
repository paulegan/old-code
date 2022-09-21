/*
 *
 * A real `quick and dirty hack' of some X based routines 
 * to provide some primitive pixel-level drawing routines.
 * Clobbered together to work only for the CS283 Mandelbrot 
 * exercise, so use at your own risk!
   If you want a real challenge, you could use the underlying X11 calls
   instead of my simplifying interface and develop a more robust
   Mandelbrot program. One possible extension is to handle `expose'
   events so that the window gets refreshed when hidden and then
   exposed.  Have a hunt in the library for material on X11.

   Oh, and there will be `magic cookies' for any really cool
   extensions/generalizations. 
 *                                 s.foley@cs.ucc.ie 23/11/95
 */

#include "manlib.h"


/*all the X11 libraries needed by the graphics routines*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>


Display        *display;
int             screen;
Window          win;
GC              gc;
unsigned int    display_width, display_height;
int             x = 0, y = 0;
unsigned int    border_width = 4;
char           *window_name = "Mandelbrot Set in Complex Plane";
char           *icon_name = "mandel";
Pixmap          icon_pixmap;
XSizeHints      size_hints;
XEvent          report;
XFontStruct    *font_info;
char           *display_name = NULL;
int             window_size = 0;
unsigned long   valuemask = 0;
XGCValues       values;
XStandardColormap    xcolormap;
Colormap colormap;
unsigned long colorpix[NRCOLORS];
XColor a,b;

void makewindow(unsigned int size){
    unsigned int width, height;
     int i;

    /*the colors that will be available (keeping it really simple!*/
    char colors[NRCOLORS][30] =
    {
    "yellow", "gold", "orange", "coral", "red",
    "indian red", "aqamarine", "spring green", "lime green", "dark turquoise",
    "forest green", "violet", "magenta", "light blue", "medium aquamarine", 
    "medium blue", "blue", "medium turquoise", "navy", "midnight blue", "black"
    };
    if ((display = XOpenDisplay(display_name)) == NULL) {
        printf("Error: cannot connect to X server %s\n",
            XDisplayName(display_name));
        printf("See your demonstrator/Simon Foley");
        abort(-1);
    }
    screen = DefaultScreen(display);

    display_width = DisplayWidth(display, screen);
    display_height = DisplayHeight(display, screen);


    width=size; height=size;
    if ((width >display_width)|| (height >display_height)){
        printf("Error: cannot draw a window of this size\n");
        printf("Screen dimensions is (%i,%i)", 
                display_width, display_height);
        abort(-1);
    }
 
    win = XCreateSimpleWindow(display, RootWindow(display, screen),
                  x, y, size, size, border_width,
                  BlackPixel(display, screen),
                  WhitePixel(display, screen));

    size_hints.flags = PPosition | PSize | PMinSize;
    size_hints.x = x;
    size_hints.y = y;
    size_hints.width = width;
    size_hints.height = height;
    size_hints.min_width = 350;
    size_hints.min_height = 250;

    XSetStandardProperties(display, win, window_name, icon_name, 
                   icon_pixmap,
                   NULL, 0, &size_hints);

    XSelectInput(display, win, ButtonPressMask | ExposureMask);

    gc = XCreateGC(display, win, valuemask, &values);
    XSetForeground(display, gc, BlackPixel(display, screen)); 
  
    XMapWindow(display, win);

        colormap= DefaultColormap(display,screen);

    /*setup pixel values for colorpix*/

    for (i=0; i<NRCOLORS; i++){
        XAllocNamedColor(display,colormap,colors[i],&a,&b);
        colorpix[(NRCOLORS-(i+1))]=a.pixel;
    }
}


void drawpixel(unsigned int x, unsigned int y, int color){
 
    if ((color <0)||(color>NRCOLORS-1)){
        printf("Error: drawpixel passed a value (%i) not 
            in color range",color); 
        abort(-1);
    }

    XSetForeground(display, gc, colorpix[color]);
      XDrawPoint(display,win,gc,x,y); 
}

void keepwindow (){

    while (report.type != ButtonPress)
        XNextEvent(display, &report);

    /* clean up */
    XFreeGC(display, gc);
    XCloseDisplay(display);
}

int getmouse(unsigned int *x, unsigned int *y){
    int button;

    XWindowEvent(display, win, ButtonPressMask, &report);
    
    *x=report.xbutton.x;
    *y=report.xbutton.y;
    button=report.xbutton.button;

    return(button);
}

void manflush(){
    XFlush(display);
}
