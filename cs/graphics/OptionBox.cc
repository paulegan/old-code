
// OptionBox.cc

#include <OptionBox.h>


OptionBox::OptionBox()
{
 ext=Rectangle(0,0,OptionBoxWidth,OptionBoxHeight);
 selection=NULL;
}// OptionBox


OptionBox::~OptionBox()
{
}// ~OptionBox

void OptionBox::setLocationTo(window* win, Point p)
{
 int height = 1;

 w=win;
 ext=Rectangle(p,p+Point(OptionBoxWidth,OptionBoxHeight)); 
 for(Pix i=item.first(); i!=0; item.next(i)){
   item(i).setLocationTo(w,p+Point(1,height));
   height+=item(i).height;
 }
}// setLocationTo

void OptionBox::display()
{
 w->drawButton(ext,DefaultGrey,true,1);
 for(Pix i=item.first(); i!=0; item.next(i))
   item(i).displayNormal();
 if (selection) item(selection).displayHighlighted();
}// displayNormal


void OptionBox::addItem(OptionBoxItem& opt)
{
 static int height = 1;
 
 opt.setLocationTo(w,ext.bottomLeft()+Point(1,height));
 item.append(opt);
 height+=opt.height;
 selection=item.first();
}// addItem


void OptionBox::dealWithMouseDown()
{
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) )
   for(Pix i=item.first(); i!=0; item.next(i))
     if ( item(i).extent().contains(w->mouseLocation()) ){
       item(selection).displayNormal();
       selection=i;
       item(i).dealWithMouseDown();
     }
}// dealWithMouseDown

Rectangle OptionBox::extent()
{
 return(ext);
}// extent
