
// MenuBar.cc

#include <MenuBar.h>


MenuBar::MenuBar(window* win)
{
 Rectangle r;
 int height;
 
 w=win;
 r=textExtent(" A test string!",MenuFont);
 height=r.topRight().yValue()-r.bottomLeft().yValue()+2*(MenuBarMargin+TitleTBMargin);
 ext=Rectangle(0,w->canvasHeight()-height,w->canvasWidth()-1,w->canvasHeight()-1);
}// MenuBar


MenuBar::~MenuBar()
{
}// ~MenuBar


void MenuBar::display()
{
 w->drawButton(ext,MenuBG,FALSE,1);
 for(Pix i=item.first(); i!=0; item.next(i))
   item(i).displayNormal();
}// displayNormal


void MenuBar::addItem(Menu& menu)
{
 static int width=0,height=0;
 
 menu.setLocationTo(w,ext.bottomLeft()+Point(width,1));
 item.append(menu);
 width+=menu.titleWidth;
}// addItem


void MenuBar::dealWithMouseDown()
{
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) )
   for(Pix i=item.first(); i!=0; item.next(i))
     if ( item(i).titleExtent().contains(w->mouseLocation()) )
       item(i).dealWithMouseDown();
}// dealWithMouseDown

Rectangle MenuBar::extent()
{
 return(ext);
}// extent
