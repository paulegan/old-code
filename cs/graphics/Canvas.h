
// Canvas.h - Canvas class definition.

#ifndef CanvasHeader
#define CanvasHeader

#include <Bool.h>
#include <String.h>
#include <Point.h>
#include <Rectangle.h>
#include <PixMap.h>
#include <misc.h>
#include <srgp.h>
#include <SLList.h>

int const	DefaultButtonDepth = 2;

class Canvas
{

public:

	Canvas(int,int,Bool isWindow=FALSE);
	Canvas(Canvas&,Rectangle);
	~Canvas();

  int	canvasWidth();
  int	canvasHeight();
  
  void  pasteCanvas(Canvas&,Point);
  void	drawText(String,Point,Colour,FontID f=DefaultFont);
  void	drawPixMap(PixMap,Point);
  void	drawButton(Rectangle,Colour, Bool =FALSE,int =DefaultButtonDepth);
  void	drawLine(Point,Point,Colour,int =1,writeMode =WRITE_REPLACE);
  void	drawRectangle(Rectangle,Colour,int =1,writeMode =WRITE_REPLACE);
  void	drawFilledRectangle(Rectangle,Colour,writeMode =WRITE_REPLACE);
  void	drawEllipse(Rectangle,Colour,int =1,writeMode =WRITE_REPLACE);
  void	drawFilledEllipse(Rectangle,Colour,writeMode =WRITE_REPLACE);
  void	drawPolygon(SLList<Point>&,Colour,int =1,writeMode =WRITE_REPLACE);
  void	drawFilledPolygon(SLList<Point>&,Colour,writeMode =WRITE_REPLACE);

protected:

  int	myID;
  int	width;
  int	height;

};


#endif
