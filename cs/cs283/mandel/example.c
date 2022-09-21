#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>

Display *display;
int screen;


void main(int argc, char *argv[])
{
Window win;
unsigned int width, height, display_width, display_height;
int     x = 0, y = 0;
unsigned int border_width = 4;
char *window_name = "Basic Window Program";
char *icon_name = "basicwin";
Pixmap icon_pixmap;
XSizeHints size_hints;
XEvent report;
char *display_name = NULL;
int window_size = 0;

Atom *atomList;
int numOfAtoms;

if ((display=XOpenDisplay(display_name)) == NULL)
  {
  fprintf(stderr,"basicwin: cannot connect to X server %s\n",
          XDisplayName(display_name));
  exit(-1);
  }

screen = DefaultScreen(display);

display_width = DisplayWidth(display,screen);
display_height = DisplayHeight(display,screen);

width = display_width/3;
height = display_height/4;

win = XCreateSimpleWindow(display, RootWindow(display,screen),
                          x, y, width, height, border_width,
                          BlackPixel(display,screen),
                          WhitePixel(display,screen));

size_hints.flags = PPosition | PSize | PMinSize;
size_hints.x = x;
size_hints.y = y;
size_hints.width = width;
size_hints.height = height;
size_hints.min_width = 350;
size_hints.min_height = 250;

XSetStandardProperties(display, win, window_name, icon_name, icon_pixmap,
                       argv, argc, &size_hints);

XSelectInput(display, win, ExposureMask |
             KeyPressMask |
             ButtonPressMask |
             StructureNotifyMask);

XMapWindow(display, win);

atomList = XListProperties(display,RootWindow(display,screen),&numOfAtoms);

printf("Num of atoms: %d \n",numOfAtoms);
for(x=0; x<numOfAtoms; x++)
 printf("atom%d: %s \n",x,XGetAtomName(display,atomList[x]));

while (1){
  XNextEvent(display, &report);
  switch (report.type){
     case ButtonPress:
     case KeyPress:
    	XCloseDisplay(display);
    	exit(1);
     break;
  }/* switch */
}/* while */

}/* main */

