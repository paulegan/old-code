
// DrawArea.cc

#include <DrawArea.h>
#include <SLList.h>

DrawArea::DrawArea(window* win, Rectangle r)
{
 w=win;
 ext=r;
 fg=Black;
 bg=White;
 mode=DefaultMode;
 fillStyle=DefaultFillStyle;
 lineWidth=DefaultLineWidth;
}// DrawArea
	
DrawArea::~DrawArea()
{
}// ~DrawArea


void  DrawArea::dealWithMouseDown()
{
 w->setClip(ext);

 if ( mode==LineMode )
   doLineMode();
 else if ( mode==RectangleMode && w->lastButton()==RightButton )
   doSquareMode();
 else if ( mode==RectangleMode )
   doRectangleMode();
 else if ( mode==EllipseMode && w->lastButton()==RightButton )
   doCircleMode();
 else if ( mode==EllipseMode )
   doEllipseMode();
 else if ( mode==PolygonMode )
   doPolygonMode();

 w->resetClip();
}// dealWithMouseDown


void DrawArea::doLineMode()
{
 Point rootLoc,curLoc;

 rootLoc = curLoc = w->mouseLocation();
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) ){
   if ( !(curLoc==(w->mouseLocation())) ){
     w->drawLine(rootLoc,curLoc,fg,lineWidth,WRITE_XOR);
     curLoc = w->mouseLocation();
     w->drawLine(rootLoc,curLoc,fg,lineWidth,WRITE_XOR);
   }
 }
 w->drawLine(rootLoc,curLoc,fg,lineWidth);
}// doLineMode


void DrawArea::doRectangleMode()
{
 Point rootLoc,curLoc;

 rootLoc = curLoc = w->mouseLocation();
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) ){
   if ( !(curLoc==(w->mouseLocation())) ){
     w->drawRectangle(Rectangle(rootLoc,curLoc),fg,lineWidth,WRITE_XOR);
     curLoc = w->mouseLocation();
     w->drawRectangle(Rectangle(rootLoc,curLoc),fg,lineWidth,WRITE_XOR);
   }
 }

 if ( fillStyle!=NoFill ){
   w->drawRectangle(Rectangle(rootLoc,curLoc),fg,lineWidth,WRITE_XOR);
   w->drawFilledRectangle(Rectangle(rootLoc,curLoc),bg);
 }
 if ( fillStyle!=FillWithoutBorder )
   w->drawRectangle(Rectangle(rootLoc,curLoc),fg,lineWidth);

}// doRectangleMode

void DrawArea::doSquareMode()
{
 Point rootLoc,curLoc,opposite;

 rootLoc = w->mouseLocation();
 curLoc = Point(rootLoc.xValue(),w->mouseLocation().yValue());
 opposite = curLoc+Point(2*(rootLoc.yValue()-curLoc.yValue()),2*(rootLoc.yValue()-curLoc.yValue()));
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) ){
   if ( !(curLoc.yValue()==(w->mouseLocation().yValue())) ){
     w->drawRectangle(Rectangle(curLoc,opposite),fg,lineWidth,WRITE_XOR);
     curLoc = Point(rootLoc.xValue(),w->mouseLocation().yValue());
     opposite = curLoc+Point(2*(rootLoc.yValue()-curLoc.yValue()),2*(rootLoc.yValue()-curLoc.yValue()));
     w->drawRectangle(Rectangle(curLoc,opposite),fg,lineWidth,WRITE_XOR);
   }
 }

 if ( fillStyle!=NoFill ){
   w->drawRectangle(Rectangle(curLoc,opposite),fg,lineWidth,WRITE_XOR);
   w->drawFilledRectangle(Rectangle(curLoc,opposite),bg);
 }
 if ( fillStyle!=FillWithoutBorder )
   w->drawRectangle(Rectangle(curLoc,opposite),fg,lineWidth);
}// doSquareMode

void DrawArea::doEllipseMode()
{
 Point rootLoc,curLoc;

 rootLoc = curLoc = w->mouseLocation();
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) ){
   if ( !(curLoc==(w->mouseLocation())) ){
     w->drawEllipse(Rectangle(rootLoc,curLoc),fg,lineWidth,WRITE_XOR);
     curLoc = w->mouseLocation();
     w->drawEllipse(Rectangle(rootLoc,curLoc),fg,lineWidth,WRITE_XOR);
   }
 }

 if ( fillStyle!=NoFill ){
   w->drawEllipse(Rectangle(rootLoc,curLoc),fg,lineWidth,WRITE_XOR);
   w->drawFilledEllipse(Rectangle(rootLoc,curLoc),bg);
 }
 if ( fillStyle!=FillWithoutBorder )
   w->drawEllipse(Rectangle(rootLoc,curLoc),fg,lineWidth);

}// doEllipseMode

void DrawArea::doCircleMode()
{
 Point rootLoc,curLoc,opposite;

 rootLoc = w->mouseLocation();
 curLoc = Point(rootLoc.xValue(),w->mouseLocation().yValue());
 opposite = curLoc+Point(2*(rootLoc.yValue()-curLoc.yValue()),2*(rootLoc.yValue()-curLoc.yValue()));
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) ){
   if ( !(curLoc.yValue()==(w->mouseLocation().yValue())) ){
     w->drawEllipse(Rectangle(curLoc,opposite),fg,lineWidth,WRITE_XOR);
     curLoc = Point(rootLoc.xValue(),w->mouseLocation().yValue());
     opposite = curLoc+Point(2*(rootLoc.yValue()-curLoc.yValue()),2*(rootLoc.yValue()-curLoc.yValue()));
     w->drawEllipse(Rectangle(curLoc,opposite),fg,lineWidth,WRITE_XOR);
   }
 }

 if ( fillStyle!=NoFill ){
   w->drawEllipse(Rectangle(curLoc,opposite),fg,lineWidth,WRITE_XOR);
   w->drawFilledEllipse(Rectangle(curLoc,opposite),bg);
 }
 if ( fillStyle!=FillWithoutBorder )
   w->drawEllipse(Rectangle(curLoc,opposite),fg,lineWidth);
}// doCircleMode

void DrawArea::doPolygonMode()
{
 Point startLoc,rootLoc,curLoc;
 SLList<Point> points;

 startLoc = rootLoc = curLoc = w->mouseLocation();
 points.append(startLoc);
 while( ext.contains(w->mouseLocation()) && w->lastButton()!=RightButton ){
   while( !(w->mouseButtonDown()) )
     if ( !(curLoc==(w->mouseLocation())) ){
       w->drawLine(rootLoc,curLoc,fg,lineWidth,WRITE_XOR);
       curLoc = w->mouseLocation();
       w->drawLine(rootLoc,curLoc,fg,lineWidth,WRITE_XOR);
     }
   if ( w->lastButton()==LeftButton )
     points.append(rootLoc=curLoc);
   w->waitForMouseDown();
 }
 points.append(curLoc);

 if ( fillStyle!=NoFill ){
   w->drawPolygon(points,fg,lineWidth,WRITE_XOR);
   w->drawFilledPolygon(points,bg);
 }
 if ( fillStyle!=FillWithoutBorder )
   w->drawPolygon(points,fg,lineWidth);

 points.clear();
}// doPolygonMode


  

Rectangle DrawArea::extent()
{
 return(ext);
}// extent


void DrawArea::clear()
{
 w->drawFilledRectangle(ext,bg);
}// clear


void DrawArea::setMode(Mode m)
{
 mode=m;
}// setMode


void DrawArea::setForeground(Colour c)
{
 fg=c;
}// setForeground


void DrawArea::setBackground(Colour c)
{
 bg=c;
}// setBackground


void DrawArea::setLineWidth(int w)
{
 lineWidth=w;
}// setLineWidth


void DrawArea::setFillStyle(FillStyle s)
{
 fillStyle=s;
}// setFillStyle


