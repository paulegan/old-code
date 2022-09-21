
// MenuItem.h - MenuItem class definition.

#ifndef MenuItemHeader
#define MenuItemHeader

#include <Window.h>
#include <String.h>
#include <Rectangle.h>
#include <Bool.h>

class MenuItem{

friend class Menu;

public:

		MenuItem();
		MenuItem(String,FunctionPtr,Bool);
		~MenuItem();
  
protected:
	
  void		setLocationTo(window*,Point);
  void		dealWithMouseDown();
  void		displayNormal();
  void		displayHighlighted();
  Rectangle	extent();
  int		width;
  int		height;
  
private:

  String	name;
  Point		location;
  window*	w;
  Point		textDisplacement;
  FunctionPtr	action;
  Bool		enabled;
  
};


#endif
