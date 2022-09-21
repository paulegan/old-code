
/*	snow.c		- Simulates analog snow		*/

/*	lumpy@cs.ucc.ie		2.3.1998		*/


#include <xstuff.h>

#define WWidth			500
#define WHeight			500
#define numPointsInHexagon	6
#define hexagonSideLen		3
#define aVal			2	/* s x 0.866 */
#define bVal			1	/* s x 0.500 */

void drawHexagon(XPoint p, int s, XColor c)
{
 static XPoint points[numPointsInHexagon];
 int a=s*0.866, b=s*0.500;

 points[0].x = p.x;
 points[1].x = p.x + a;
 points[2].x = p.x + a;
 points[3].x = p.x;
 points[4].x = p.x - a;
 points[5].x = p.x - a;

 points[0].y = p.y + s;
 points[1].y = p.y + b;
 points[2].y = p.y - b;
 points[3].y = p.y - s;
 points[4].y = p.y - b;
 points[5].y = p.y + b;

 XSetForeground(display,gc,c.pixel);
 XFillPolygon(display,win,gc,points,numPointsInHexagon,Convex,CoordModeOrigin);

}/* drawHexagon */


void rotateColours()
{
 int		c;
 unsigned long	tempPixel;

 tempPixel = colours[0].pixel;
 for(c=0; c<(NumColours-1); c++)
   colours[c].pixel=colours[c+1].pixel;
 colours[NumColours-1].pixel = tempPixel;

 XStoreColors(display,colourMap,colours,NumColours);
}/* rotateColours */


void doSnow()
{
 XPoint	p;
 int	x,y;
 int	s=hexagonSideLen;
 int	o=1.5*s,a=0.866*s;
 int	xMax=(WWidth/o)+1,yMax=(WHeight/o)+1;

 for(y=0; y<yMax; y++){
  for(x=0; x<xMax; x++){
    p.y = (y*o);
    p.x = (x*o)+((y%2)?a:0);
    drawHexagon(p,s,colours[((x+y)*((y%2)?2:3))%NumColours]);
  }
 }

/*
 *  for(x=0; x<20000; x++)
 *    rotateColours();
 */

 for(x=0; x<200000; x++){
   if (!(x%20)) rotateColours();
   y = (rand()%yMax);
   p.y = (y*o);
   p.x = ((rand()%xMax)*o)+((y%2)?a:0); 
   drawHexagon(p,s,colours[y%NumColours]);
 }
   
}/* doSnow */

int main()
{
 openWindow("Snow",WWidth,WHeight);
 doSnow();
 waitForMouseClick();
 closeWindow();

 return(0);
}/* main */


