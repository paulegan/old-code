
// Button.cc

#include <Button.h>

Button::Button()
{
 enabled=FALSE;
 action=NULL;
 w=NULL;
 width=0;
 height=0;
}//  Button


Button::Button(PixMap p,FunctionPtr a,Bool e)
{
 enabled=e;
 action=a;
 pixmap=p;
 w=NULL;
 width=pixmap.width()+2*ButtonMargin;
 height=pixmap.height()+2*ButtonMargin;
}//  Button


Button::~Button()
{
}// ~Button


void Button::setLocationTo(window* win, Point p)
{
 w=win;
 location=p;
}// setLocationTo


void Button::dealWithMouseDown()
{
 Rectangle ext=extent();
 displayHighlighted();
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) );
 //displayNormal();
 if (ext.contains(w->mouseLocation()) && enabled)
   action();
}// dealWithMouseDown


void Button::displayNormal()
{
 w->drawButton(extent(),DefaultGrey);
 w->drawPixMap(pixmap,location+Point(ButtonMargin,ButtonMargin));
}// displayNormal

void Button::displayHighlighted()
{
 w->drawButton(extent(),DefaultGrey,true);
 w->drawPixMap(pixmap,location+Point(ButtonMargin,ButtonMargin));
}// displayHighlighted


Rectangle Button::extent()
{
 return(Rectangle(location,location+Point(width,height)));
}// extent
