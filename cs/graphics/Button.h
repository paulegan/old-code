
// Button.h - Button class definition.

#ifndef ButtonHeader
#define ButtonHeader

#include <Window.h>
#include <Rectangle.h>
#include <Bool.h>
#include <PixMap.h>

int const ButtonMargin = 3;

class Button{

friend class ButtonBar;

public:

		Button();
		Button(PixMap,FunctionPtr,Bool);
		~Button();

protected:
	
  void		setLocationTo(window*,Point);
  void		dealWithMouseDown();
  void		displayNormal();
  void		displayHighlighted();
  Rectangle	extent();
  
private:

  Point		location;
  int		width;
  int		height;
  window*	w;
  FunctionPtr	action;
  Bool		enabled;
  PixMap	pixmap;
  
};


#endif
