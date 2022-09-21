
// MenuItem.cc

#include <MenuItem.h>
#include <misc.h>


MenuItem::MenuItem()
{
 enabled=FALSE;
 action=NULL;
 w=NULL;
 width=0;
 height=0;
}//  MenuItem


MenuItem::MenuItem(String n,FunctionPtr a,Bool e)
{
 Rectangle r;
 
 name=n;
 enabled=e;
 action=a;
 w=NULL;
 
 r=textExtent(name,MenuFont);
 textDisplacement=Point(-r.bottomLeft().xValue(),-r.bottomLeft().yValue());
 width=r.topRight().xValue();
 height=r.topRight().yValue()-r.bottomLeft().yValue();
}//  MenuItem


MenuItem::~MenuItem()
{
}// ~MenuItem


void MenuItem::setLocationTo(window* win, Point p)
{
 w=win;
 location=p;
}// setLocationTo


void MenuItem::dealWithMouseDown()
{
 Rectangle ext=extent();
 displayHighlighted();
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) );
 displayNormal();
 if (ext.contains(w->mouseLocation()) && enabled)
   action();
}// dealWithMouseDown


void MenuItem::displayNormal()
{
 if ( name!="" ){
   w->drawFilledRectangle(extent(),MenuBG);
   w->drawText(name,location+textDisplacement,enabled?MenuFG:MenuDA,MenuFont);
 }else
   w->drawButton(Rectangle(location+Point(0,int(height/2)),location+Point(width,2+int(height/2))),DefaultGrey,true,1);
}// displayNormal

void MenuItem::displayHighlighted()
{
 if ( name!="" ){
   w->drawFilledRectangle(extent(),enabled?MenuHL:MenuBG);
   w->drawText(name,location+textDisplacement,enabled?MenuFG:MenuDA,MenuFont);
 }else
   w->drawButton(Rectangle(location+Point(0,int(height/2)),location+Point(width,2+int(height/2))),DefaultGrey,true,1);
}// displayHighlighted


Rectangle MenuItem::extent()
{
 return(Rectangle(location,location+Point(width,height)));
}// extent
