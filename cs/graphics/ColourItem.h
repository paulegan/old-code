
// ColourItem.h - ColourItem class definition.

#ifndef ColourItemHeader
#define ColourItemHeader

#include <Window.h>
#include <Rectangle.h>
#include <Bool.h>

int const ColourItemSize = 15;

class ColourItem{

public:

		ColourItem();
		ColourItem(Colour,FunctionPtr,Bool);
		~ColourItem();
	
  void		setLocationTo(window*,Point);
  void		dealWithMouseDown();
  void		displayNormal();
  void		displayHighlighted();
  Rectangle	extent();
  
private:

  Point		location;
  window*	w;
  FunctionPtr	action;
  Bool		enabled;
  Colour	colour;
  
};


#endif
