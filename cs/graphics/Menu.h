
// Menu.h - Menu class definition.

#ifndef MenuHeader
#define MenuHeader

#include <Bool.h>
#include <String.h>
#include <MenuItem.h>
#include <SLList.h>

int const MenuTopMargin = 5;
int const MenuLeftMargin = 5;
int const MenuRightMargin = 20;
int const MenuBottomMargin = 5;
int const TitleTBMargin = 2;
int const TitleLRMargin = 7;

class Menu{

friend class MenuBar;

public:

			Menu();	
			Menu(String);	
			~Menu();
  void			addItem(MenuItem&);

protected:

  void			setLocationTo(window*,Point);
  void			displayNormal();
  void			displaySelected();
  void			dealWithMouseDown();
  Rectangle		titleExtent();
  int			titleWidth;
  int			titleHeight;
  String		title;

private:

  SLList<MenuItem>	item;
  Point			location;
  window*		w;
  Point			textDisplacement;
  Rectangle		menuExtent();
  int			menuWidth;
  int			menuHeight;
  Canvas*		undergrowth;
  
};


#endif
