
// OptionBox.h - OptionBox class definition.

#ifndef OptionBoxHeader
#define OptionBoxHeader

#include <Bool.h>
#include <OptionBoxItem.h>
#include <SLList.h>

int const OptionBoxWidth = 32;
int const OptionBoxHeight = 43;

class OptionBox{

public:

			OptionBox();	
			~OptionBox();
  void			addItem(OptionBoxItem&);
  void			setLocationTo(window*,Point);
  void			display();
  void			dealWithMouseDown();
  Rectangle		extent();
  
private:

  SLList<OptionBoxItem>	item;
  Pix			selection;
  window*		w;
  Rectangle		ext;

};


#endif
