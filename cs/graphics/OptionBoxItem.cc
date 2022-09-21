
// OptionBoxItem.cc

#include <OptionBoxItem.h>

OptionBoxItem::OptionBoxItem()
{
 enabled=FALSE;
 action=NULL;
 w=NULL;
 width=0;
 height=0;
}//  OptionBoxItem


OptionBoxItem::OptionBoxItem(PixMap p,FunctionPtr a,Bool e)
{
 enabled=e;
 action=a;
 pixmap=p;
 w=NULL;
 width=pixmap.width();
 height=pixmap.height();
}//  OptionBoxItem


OptionBoxItem::~OptionBoxItem()
{
}// ~OptionBoxItem


void OptionBoxItem::setLocationTo(window* win, Point p)
{
 w=win;
 location=p;
}// setLocationTo


void OptionBoxItem::dealWithMouseDown()
{
 Rectangle ext=extent();
 displayHighlighted();
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) );
 //displayNormal();
 if (ext.contains(w->mouseLocation()) && enabled)
   action();
}// dealWithMouseDown


void OptionBoxItem::displayNormal()
{
 w->drawPixMap(pixmap,location);
 w->drawRectangle(extent(),DefaultGrey);
}// displayNormal

void OptionBoxItem::displayHighlighted()
{
 w->drawPixMap(pixmap,location);
 w->drawRectangle(extent(),Black);
}// displayHighlighted


Rectangle OptionBoxItem::extent()
{
 return(Rectangle(location,location+Point(width,height)));
}// extent
