#include <stdio.h>

#define COLORBLACK 0                         /*the standard value for black*/
#define NRCOLORS 21            /*nr colors available, from 0 to NRCOLORS-1 */


/*
 * Create a window with pixel dimensions (size * size). Its top lefthand 
 * corner is (0,0) and bottom right is (size,size).
 */
void makewindow(unsigned int size);              


/*
 * draw a pixel, in the window created by makewindow,  at point 
 * (x,y) with colour given by color.
 * note that color must be in range BLACK to NRCOLORS-1 
 */
void drawpixel(unsigned int x, unsigned int y, int color);



/* 
 * Keep the window displayed until a mouse button is `clicked'
 * in the window.
 */
void keepwindow ();  



/*
 * Get the next event from the mouse. Function returns values 
 * in the range 1..3 ( left, middle and right mouse button pressed).
 * (x,y) gives the coordinates of the pointer when the button 
 * was pressed. 
 */
int getmouse(unsigned int *x, unsigned int *y);

void manflush();
