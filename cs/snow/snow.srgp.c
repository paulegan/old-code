
/*	snow.c	- Simulates analog snow		*/

/*	lumpy@cs.ucc.ie	2.3.1998		*/


#include <srgp.h>

#define numPointsInHexagon	6
#define hexagonSideLen		10

typedef struct{
  int x,y;
}Point;

typedef int Colour;


void drawHexagon(Point p, Colour c, int s)
{
 int xPoints[numPointsInHexagon],yPoints[numPointsInHexagon];
 int a=s*0.866,b=2*0.5;

 xPoints[0] = p.x;
 xPoints[1] = p.x + a;
 xPoints[2] = p.x + a;
 xPoints[3] = p.x;
 xPoints[4] = p.x - a;
 xPoints[5] = p.x - a;

 yPoints[0] = p.y + s;
 yPoints[1] = p.y + b;
 yPoints[2] = p.y - b;
 yPoints[3] = p.y - s;
 yPoints[4] = p.y - b;
 yPoints[5] = p.y + b;

 SRGP_setColor(c);
 SRGP_fillPolygonCoord(numPointsInHexagon,xPoints,yPoints);
}/* drawHexagon */

void doSnow()
{
 Point	randPos;
 Colour	randCol;

 while(1){
  drawHexagon(randPos,randCol,10);
 }
}/* doSnow */

int main()
{
 Point p;

 SRGP_begin("snow",300,300,1,FALSE);

 /* doSnow(); */
 
 p.x=50; p.y=50; 
/*  drawHexagon(p,1,10);
 */
 SRGP_setInputMode(KEYBOARD,EVENT);
 SRGP_setKeyboardProcessingMode(RAW);
 SRGP_waitEvent(INDEFINITE);
 SRGP_end();
 return(0);
}/* main */


