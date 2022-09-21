
// Point.h - Point class definition.

#ifndef PointHeader
#define PointHeader

#include <Bool.h> 
#include <iostream.h>

class Point{

public:

	Point()				{ x=y=0; };
	Point(Point& p)			{ x=p.x; y=p.y; };
	Point(int xVal,int yVal)	{ x=xVal; y=yVal; };	
	~Point()			{};
  int	xValue()			{ return(x); };
  int	yValue()			{ return(y); };
  Point	operator+(Point& p)		{ return(Point(x+p.x,y+p.y)); };
  Point operator+=(Point& p)		{ x+=p.x; y+=p.y; return(*this); };
  Point operator=(Point& p)		{ x=p.x; y=p.y; return(*this); };
  Bool  operator==(Point& p)		{ return(x==p.x && y==p.y); };
	print()				{ cout << '(' << x << ',' << y << ')'; };
	
private:

  int		x,y;

};


#endif
