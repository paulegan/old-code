
/*	xstuff.c - Usefull X funcs			*/

/*	lumpy@cs.ucc.ie		4.3.1998		*/

#include <xstuff.h>

void openWindow(char *name, unsigned int width, unsigned int height)
{
 int		screen, c, WPosX = 0, WPosY = 0; 
 unsigned int	borderWidth = 4;
 char*		displayName = NULL;
 char*		windowName = name;
 char*		iconName = name;
 Pixmap		iconPixmap = None;
 XSizeHints	sizeHints;
 unsigned long	GCValueMask = NULL;
 XGCValues*	GCValues;
 XColor		colourA,colourB;
 unsigned long	colourPixels[NumColours];
 char		colourNames[NumColours][30] = { "black","red","blue","green","grey","white" };

 if((display = XOpenDisplay(displayName)) == NULL){
   printf("ERROR Can't open display!");
   exit(-1);
 }

 screen = DefaultScreen(display);

 win = XCreateSimpleWindow(display, RootWindow(display, screen),
               WPosX, WPosY, width, height, borderWidth,
               BlackPixel(display, screen),
               WhitePixel(display, screen));

 sizeHints.flags = PPosition | PSize | PMinSize | PMaxSize;
 sizeHints.x = WPosX;
 sizeHints.y = WPosY;
 sizeHints.width = width;
 sizeHints.height = height;
 sizeHints.min_width = width;
 sizeHints.min_height = height;
 sizeHints.max_width = MAXWWidth;
 sizeHints.max_height = MAXWHeight;

 XSetStandardProperties(display, win, windowName, iconName, 
                   	iconPixmap, NULL, 0, &sizeHints);

 XSelectInput(display, win, ButtonPressMask|ExposureMask|StructureNotifyMask);

 gc = XCreateGC(display, win, GCValueMask, GCValues);

 if (XMapWindow(display, win)==BadWindow){
   printf("ERROR Can't open window!");
   exit(-1);
 }

 colourMap = DefaultColormap(display,screen);
 /* XAllocColorCells(display,colourMap,0,NULL,0,colourPixels,NumColours); */
 for(c=0; c<NumColours; c++){
   /* XLookupColor(display,colourMap,colourNames[c],&colourA,&colourB); */
   XAllocNamedColor(display,colourMap,colourNames[c],&colourA,&colourB);
   colours[c]=colourA;
   /* colours[c].pixel=colourPixels[c]; */
   /* colours[c].flags = DoRed|DoGreen|DoBlue; */
 }
 /* XStoreColors(display,colourMap,colours,NumColours); */
 XSetForeground(display,gc,colours[0].pixel);

 if ((font = XLoadQueryFont(display,"fixed"))==NULL){
   printf("ERROR Can't load font! \n");
   exit(-1);
 }

 XWindowEvent(display,win,ExposureMask,&report);

}/* openWindow */


void closeWindow()
{
 XFreeFont(display, font);
 XFreeColormap(display, colourMap);
 XFreeGC(display, gc);
 XCloseDisplay(display);
}/* closeWindow */


void XPrint(char *str,int x,int y)
{    
 XClearArea(display,win,x,y-10,XTextWidth(font,str,strlen(str)),12,0);
 XSetForeground(display,gc,colours[0].pixel);
 XDrawString(display,win,gc,x,y,str,strlen(str));
 XFlush(display);
}/* XPrint */


void waitForMouseClick()
{
 XWindowEvent(display,win,ButtonPressMask,&report);
}/* waitForMouseClick */

