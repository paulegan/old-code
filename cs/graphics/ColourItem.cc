
// ColourItem.cc

#include <ColourItem.h>

ColourItem::ColourItem()
{
 enabled=FALSE;
 action=NULL;
 w=NULL;
}//  ColourItem

ColourItem::ColourItem(Colour c,FunctionPtr a,Bool e)
{
 enabled=e;
 action=a;
 w=NULL;
 colour=c;
}//  ColourItem


ColourItem::~ColourItem()
{
}// ~ColourItem


void ColourItem::setLocationTo(window* win, Point p)
{
 w=win;
 location=p;
}// setLocationTo


void ColourItem::dealWithMouseDown()
{
 Rectangle ext=extent();
 displayHighlighted();
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) );
 displayNormal();
 if (ext.contains(w->mouseLocation()) && enabled)
   action();
}// dealWithMouseDown


void ColourItem::displayNormal()
{
 w->drawButton(extent(),colour,true,1);
}// displayNormal

void ColourItem::displayHighlighted()
{
 w->drawButton(extent(),colour,false,1);
}// displayHighlighted


Rectangle ColourItem::extent()
{
 return(Rectangle(location,location+Point(ColourItemSize,ColourItemSize)));
}// extent
