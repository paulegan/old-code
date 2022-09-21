
// Rectangle.h - Rectangle class definition.

#ifndef RectangleHeader
#define RectangleHeader

#include <iostream.h>
#include <Point.h>
#include <Bool.h> 

class Rectangle{

public:

		Rectangle()			{};
		Rectangle(Rectangle& r)		{ bl=r.bl; tr=r.tr; };
		Rectangle(Point p1, Point p2)	{ bl=Point((p1.xValue()<p2.xValue())?p1.xValue():p2.xValue(),(p1.yValue()<p2.yValue())?p1.yValue():p2.yValue()); tr=Point((p1.xValue()>p2.xValue())?p1.xValue():p2.xValue(),(p1.yValue()>p2.yValue())?p1.yValue():p2.yValue()); };	
		Rectangle(int x1, int y1, int x2, int y2)	{ bl=Point((x1<x2)?x1:x2,(y1<y2)?y1:y2); tr=Point((x1>x2)?x1:x2,(y1>y2)?y1:y2); };	
		~Rectangle()			{};
  Point		bottomLeft()			{ return(bl); };
  Point		topRight()			{ return(tr); };
  Rectangle	operator+(Point& p)		{ return(Rectangle(bl+p,tr+p)); };
  Rectangle	operator+=(Point& p)		{ bl+=p; tr+=p; return(*this); };
  Rectangle	operator=(Rectangle& r)		{ bl=r.bl; tr=r.tr; return(*this); };
  Bool		contains(Point p)		
  { return(
  ( (bl.xValue()<p.xValue()) && (p.xValue()<tr.xValue())) && 
  ( (bl.yValue()<p.yValue()) && (p.yValue()<tr.yValue()))
  ); };
  		print()				{ bl.print(); tr.print(); };
	
private:

  Point bl,tr;

};


#endif
