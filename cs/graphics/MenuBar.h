
// MenuBar.h - MenuBar class definition.

#ifndef MenuBarHeader
#define MenuBarHeader

#include <Bool.h>
#include <Menu.h>
#include <SLList.h>

int const MenuBarMargin = 2;

class MenuBar{

public:

		MenuBar(window*);	
		~MenuBar();
  void		addItem(Menu&);
  void		display();
  void		dealWithMouseDown();
  Rectangle	extent();

private:

  SLList<Menu>	item;
  window*	w;
  Rectangle	ext;
  
};


#endif
