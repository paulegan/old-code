
// ColourBar.cc

#include <ColourBar.h>


ColourBar::ColourBar(window* win)
{
 w=win;
 ext=Rectangle(0,0,w->canvasWidth()-1,(3*ColourBarMargin)+(2*ColourItemSize));
 foreground=DefaultFG;
 background=DefaultBG;
}// ColourBar


ColourBar::~ColourBar()
{
}// ~ColourBar


void ColourBar::display()
{
 w->drawButton(ext,DefaultGrey,FALSE,1);
 for(Pix i=item.first(); i!=0; item.next(i))
   item(i).displayNormal();
 displaySelection();
}// displayNormal


void ColourBar::addItem(ColourItem& colourItem)
{
 int spacing = ColourBarMargin+ColourItemSize;
 static int width=spacing+(3*ColourItemSize);
 Point bottomLeft = ext.bottomLeft() + Point(0,ColourBarMargin);
 
 colourItem.setLocationTo(w,bottomLeft+Point(width,(item.length()%NumRows)*spacing));
 item.append(colourItem);
 if ((item.length()%NumRows)==0) width += spacing;
}// addItem


void ColourBar::dealWithMouseDown()
{
 static Pix sel=item.first();
 
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) )
   for(Pix i=item.first(); i!=0; item.next(i))
     if ( item(i).extent().contains(w->mouseLocation()) ){
       item(sel).displayNormal();
       sel=i;
       item(i).dealWithMouseDown();
     }
}// dealWithMouseDown

Rectangle ColourBar::extent()
{
 return(ext);
}// extent

void ColourBar::setForeground(Colour fg)
{
 foreground=fg;
 displaySelection();
}// setForeground

void ColourBar::setBackground(Colour bg)
{
 background=bg;
 displaySelection();
}// setBackground

void ColourBar::displaySelection()
{
 static Point SelectionLoc = Point(15,15);
 w->drawButton(Rectangle(SelectionLoc+Point(int(ColourItemSize*0.5),int(-(ColourItemSize*0.5))),SelectionLoc+Point(int(ColourItemSize*1.5),int(ColourItemSize*0.5))),background,false,1);
 w->drawButton(Rectangle(SelectionLoc,SelectionLoc+Point(ColourItemSize,ColourItemSize)),foreground,false,1);
}// displaySelection

