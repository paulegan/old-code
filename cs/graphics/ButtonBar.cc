
// ButtonBar.cc

#include <ButtonBar.h>


ButtonBar::ButtonBar(window* win, int bottom, int top)
{
 w=win;
 height=Spacing;
 ext=Rectangle(0,bottom,(4*ButtonBarMargin)+(2*ButtonSize),top);
 optBox = new OptionBox();
}// ButtonBar


ButtonBar::~ButtonBar()
{
}// ~ButtonBar


void ButtonBar::display()
{
 w->drawButton(ext,DefaultGrey,FALSE,1);
 for(Pix i=item.first(); i!=0; item.next(i))
   item(i).displayNormal();
 item(item.first()).displayHighlighted();
 optBox->display();
}// displayNormal


void ButtonBar::addItem(Button& button)
{
 Point topLeft(ButtonBarMargin,ext.topRight().yValue());
 
 button.setLocationTo(w,topLeft+Point((item.length()%NumColumns)*Spacing,-height));
 item.append(button);
 if ((item.length()%NumColumns)==0) height += Spacing;
 optBox->setLocationTo(w,topLeft+Point(2*ButtonBarMargin,-height-OptionBoxHeight));
}// addItem


void ButtonBar::dealWithMouseDown()
{
 static Pix sel=item.first();
 
 while( w->mouseButtonDown() && ext.contains(w->mouseLocation()) ){
   for(Pix i=item.first(); i!=0; item.next(i))
     if ( item(i).extent().contains(w->mouseLocation()) ){
       item(sel).displayNormal();
       sel=i;
       item(i).dealWithMouseDown();
     }
   if ( optBox->extent().contains(w->mouseLocation()) )
     optBox->dealWithMouseDown();
 }
}// dealWithMouseDown

Rectangle ButtonBar::extent()
{
 return(ext);
}// extent

void ButtonBar::setOptionBoxTo(OptionBox* b)
{
 optBox = b;
 optBox->setLocationTo(w,Point(3*ButtonBarMargin,ext.topRight().yValue()-height-OptionBoxHeight));
 optBox->display();
}// setOptionBoxTo
