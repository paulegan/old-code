
// OptionBoxItem.h - OptionBoxItem class definition.

#ifndef OptionBoxItemHeader
#define OptionBoxItemHeader

#include <Window.h>
#include <Rectangle.h>
#include <Bool.h>
#include <PixMap.h>

class OptionBoxItem{

friend class OptionBox;

public:

		OptionBoxItem();
		OptionBoxItem(PixMap,FunctionPtr,Bool);
		~OptionBoxItem();
	
protected:

  void		setLocationTo(window*,Point);
  void		dealWithMouseDown();
  void		displayNormal();
  void		displayHighlighted();
  Rectangle	extent();
  int		width;
  int		height;

private:

  Point		location;
  window*	w;
  FunctionPtr	action;
  Bool		enabled;
  PixMap	pixmap;
  
};


#endif
