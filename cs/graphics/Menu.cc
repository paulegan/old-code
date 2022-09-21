
// Menu.cc

#include <Menu.h>


Menu::Menu()
{
 w=NULL;
 titleWidth=0;
 titleHeight=0;
 menuWidth=0;
 menuHeight=0;
 undergrowth=NULL;
}// Menu


Menu::Menu(String t)
{
 Rectangle r;

 title=t;
 w=NULL;
 undergrowth=NULL;
 
 r=textExtent(title,MenuFont);
 textDisplacement=Point(TitleLRMargin-r.bottomLeft().xValue(),TitleTBMargin-r.bottomLeft().yValue());
 titleWidth=r.topRight().xValue()+(2*TitleLRMargin);
 titleHeight=r.topRight().yValue()-r.bottomLeft().yValue()+(2*TitleTBMargin);
 menuWidth=titleWidth;
 menuHeight=MenuTopMargin+MenuBottomMargin;
}// Menu


Menu::~Menu()
{
}// ~Menu


void Menu::setLocationTo(window* win, Point p)
{
 w=win;
 location=p;
 menuHeight=MenuTopMargin+MenuBottomMargin;
 for(Pix i=item.first(); i!=0; item.next(i)){
   menuHeight+=item(i).height;
   item(i).setLocationTo(w,location+Point(MenuLeftMargin,MenuBottomMargin-menuHeight));
 }
}// setLocationTo


void Menu::addItem(MenuItem& menuItem)
{
 int newWidth=0,aBit=MenuLeftMargin;
 Pix i;
 
 menuHeight+=menuItem.height;
 menuItem.setLocationTo(w,location+Point(MenuLeftMargin,MenuBottomMargin-menuHeight));
 item.append(menuItem);
 for(i=item.first(); i!=0; item.next(i))
   if ( item(i).width>newWidth )
     newWidth=item(i).width;
 for(i=item.first(); i!=0; item.next(i))
   item(i).width=newWidth+MenuRightMargin-aBit;
 menuWidth=newWidth+MenuRightMargin+MenuLeftMargin;
}// addItem


void Menu::displayNormal()
{
 w->drawFilledRectangle(titleExtent(),MenuBG);
 w->drawText(title,location+textDisplacement,MenuFG,MenuFont);
 if (undergrowth){
   w->pasteCanvas(*undergrowth,location+Point(0,-menuHeight));
   delete undergrowth;
   undergrowth=NULL;
 }
}// displayNormal


void Menu::displaySelected()
{
 undergrowth = new Canvas(*w,menuExtent());
 w->drawFilledRectangle(titleExtent(),MenuHL);
 w->drawText(title,location+textDisplacement,MenuFG,MenuFont);
 w->drawButton(menuExtent(),MenuBG,FALSE,1);
 for(Pix i=item.first(); i!=0; item.next(i))
   item(i).displayNormal();
}// displaySelected


void Menu::dealWithMouseDown()
{
 Rectangle menuExt=menuExtent(), titleExt=titleExtent();
 
 displaySelected();
 while( w->mouseButtonDown() && titleExt.contains(w->mouseLocation()) );
 if ( titleExt.contains(w->mouseLocation()) )
   w->waitForMouseDown();
 while( w->mouseButtonDown() && menuExt.contains(w->mouseLocation()) )
   for(Pix i=item.first(); i!=0; item.next(i))
     if ( item(i).extent().contains(w->mouseLocation()) )
       item(i).dealWithMouseDown();
 displayNormal();
}// dealWithMouseDown


Rectangle Menu::titleExtent()
{
 return(Rectangle(location,location+Point(titleWidth,titleHeight)));
}// titleExtent


Rectangle Menu::menuExtent()
{
 return(Rectangle(location+Point(0,-menuHeight),location+Point(menuWidth,1)));
}// menuExtent

