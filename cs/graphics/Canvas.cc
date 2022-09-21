
// Canvas.cc

#include <srgp.h>
#include <std.h>
#include <Canvas.h>

// Canvas constructor. Windows are created differently!
Canvas::Canvas(int w, int h, Bool isWindow)
{
 width=w; height=h;
 if ( !isWindow )
   myID = SRGP_createCanvas(width,height);
}// Canvas


Canvas::Canvas(Canvas& c, Rectangle r)
{
 Point bl=r.bottomLeft(), tr=r.topRight();

 width=tr.xValue()-bl.xValue()+1; height=tr.yValue()-bl.yValue()+1;
 myID = SRGP_createCanvas(width,height);
 SRGP_useCanvas(myID);
 SRGP_copyPixel(c.myID,SRGP_defRectangle(bl.xValue(),bl.yValue(),tr.xValue(),tr.yValue()),SRGP_defPoint(0,0));
}// Canvas


// Canvas destructor.
Canvas::~Canvas()
{
 if ( myID!=0 )
   SRGP_deleteCanvas(myID);
}// ~Canvas


int Canvas::canvasWidth()
{
 return(width);
}// canvasWidth


int Canvas::canvasHeight()
{
 return(height);
}// canvasHeight


void Canvas::pasteCanvas(Canvas& c, Point p)
{
 SRGP_useCanvas(myID);
 SRGP_copyPixel(c.myID,SRGP_defRectangle(0,0,c.width-1,c.height-1),SRGP_defPoint(p.xValue(),p.yValue()));
}// pasteCanvas


// Draws text in this canvas with the specified attribs.
void Canvas::drawText(String text, Point p, Colour fg, FontID f)
{
 SRGP_useCanvas(myID);
 SRGP_setColor(fg);
 SRGP_setFont(f);
 SRGP_text(SRGP_defPoint(p.xValue(),p.yValue()),text);
}// drawText

void Canvas::drawPixMap(PixMap pm, Point p)
{
 int x0=p.xValue(), y0=p.yValue()+pm.height();
 
 SRGP_useCanvas(myID);
 for(int x=0; x<pm.width(); x++)
  for(int y=0; y<pm.height(); y++){
   SRGP_setColor(pm.pix(x,y));
   SRGP_pointCoord(x0+x,y0-y);
  }
}// drawPixMap

void Canvas::drawButton(Rectangle r, Colour fg, Bool depressed, int depth)
{
 Point bl=r.bottomLeft(), tr=r.topRight();
 Colour c1=ButtonHighlightColour,c2=ButtonShadeColour;
 
 SRGP_useCanvas(myID);
 SRGP_setLineStyle(CONTINUOUS);
 SRGP_setLineWidth(1);
 SRGP_setFillStyle(SOLID);
 SRGP_setColor(fg);
 SRGP_fillRectangleCoord(bl.xValue(),bl.yValue(),tr.xValue(),tr.yValue());
 if (depressed) { c1=ButtonShadeColour,c2=ButtonHighlightColour; }
 for(int x=0; x<depth; x++){
   SRGP_setColor(c1);
   SRGP_lineCoord(bl.xValue()+x,bl.yValue()+x,bl.xValue()+x,tr.yValue()-x);
   SRGP_lineCoord(bl.xValue()+x,tr.yValue()-x,tr.xValue()-x,tr.yValue()-x);
   SRGP_setColor(c2);
   SRGP_lineCoord(bl.xValue()+1+x,bl.yValue()+x,tr.xValue()-x,bl.yValue()+x);
   SRGP_lineCoord(tr.xValue()-x,bl.yValue()+x,tr.xValue()-x,tr.yValue()-x);
 }
}// drawButton

void Canvas::drawLine(Point p1, Point p2, Colour c, int w, writeMode m)
{
 SRGP_useCanvas(myID);
 SRGP_setColor(c);
 SRGP_setLineWidth(w);
 SRGP_setWriteMode(m);
 SRGP_lineCoord(p1.xValue(),p1.yValue(),p2.xValue(),p2.yValue());
 SRGP_setWriteMode(WRITE_REPLACE);
 SRGP_setLineWidth(1);
}// drawLine

void Canvas::drawRectangle(Rectangle r, Colour c, int w, writeMode m)
{
 SRGP_useCanvas(myID);
 SRGP_setColor(c);
 SRGP_setLineWidth(w);
 SRGP_setWriteMode(m);
 SRGP_rectangleCoord(r.bottomLeft().xValue(),r.bottomLeft().yValue(),r.topRight().xValue(),r.topRight().yValue());
 SRGP_setWriteMode(WRITE_REPLACE);
 SRGP_setLineWidth(1);
}// drawRectangle

void Canvas::drawFilledRectangle(Rectangle r, Colour c, writeMode m)
{
 SRGP_useCanvas(myID);
 SRGP_setFillStyle(SOLID);
 SRGP_setColor(c);
 SRGP_setWriteMode(m);
 SRGP_fillRectangleCoord(r.bottomLeft().xValue(),r.bottomLeft().yValue(),r.topRight().xValue(),r.topRight().yValue());
 SRGP_setWriteMode(WRITE_REPLACE);
}// drawFilledRectangle

void Canvas::drawEllipse(Rectangle r, Colour c, int w, writeMode m)
{
 SRGP_useCanvas(myID);
 SRGP_setColor(c);
 SRGP_setLineWidth(w);
 SRGP_setWriteMode(m);
 SRGP_ellipse(SRGP_defRectangle(r.bottomLeft().xValue(),r.bottomLeft().yValue(),r.topRight().xValue(),r.topRight().yValue()));
 SRGP_setWriteMode(WRITE_REPLACE);
 SRGP_setLineWidth(1);
}// drawEllipse

void Canvas::drawFilledEllipse(Rectangle r, Colour c, writeMode m)
{
 SRGP_useCanvas(myID);
 SRGP_setFillStyle(SOLID);
 SRGP_setColor(c);
 SRGP_setWriteMode(m);
 SRGP_fillEllipse(SRGP_defRectangle(r.bottomLeft().xValue(),r.bottomLeft().yValue(),r.topRight().xValue(),r.topRight().yValue()));
 SRGP_setWriteMode(WRITE_REPLACE);
}// drawFilledEllipse


void Canvas::drawPolygon(SLList<Point>& points, Colour c, int w, writeMode m)
{
 int  j,pCount = points.length();
 int* xCoords = (int*)malloc(pCount*sizeof(int));
 int* yCoords = (int*)malloc(pCount*sizeof(int));
 Pix  i;

 for(i=points.first(),j=0; i!=0; points.next(i),j++){
   xCoords[j]=points(i).xValue();
   yCoords[j]=points(i).yValue();
 }
 SRGP_useCanvas(myID);
 SRGP_setColor(c);
 SRGP_setLineWidth(w);
 SRGP_setWriteMode(m);
 SRGP_polygonCoord(pCount,xCoords,yCoords);
 SRGP_setWriteMode(WRITE_REPLACE);
 SRGP_setLineWidth(1);

 free(xCoords);
 free(yCoords);
}// drawPolygon

void Canvas::drawFilledPolygon(SLList<Point>& points, Colour c, writeMode m)
{
 int  j,pCount = points.length();
 int* xCoords = (int*)malloc(pCount*sizeof(int));
 int* yCoords = (int*)malloc(pCount*sizeof(int));
 Pix  i;

 for(i=points.first(),j=0; i!=0; points.next(i),j++){
   xCoords[j]=points(i).xValue();
   yCoords[j]=points(i).yValue();
 }
 SRGP_useCanvas(myID);
 SRGP_setFillStyle(SOLID);
 SRGP_setColor(c);
 SRGP_setWriteMode(m);
 SRGP_fillPolygonCoord(pCount,xCoords,yCoords);
 SRGP_setWriteMode(WRITE_REPLACE);

 free(xCoords);
 free(yCoords);
}// drawFilledPolygon




