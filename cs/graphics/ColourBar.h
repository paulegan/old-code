
// ColourBar.h - ColourBar class definition.

#ifndef ColourBarHeader
#define ColourBarHeader

#include <Bool.h>
#include <ColourItem.h>
#include <SLList.h>

int const ColourBarMargin = 3;
int const NumRows = 2;

class ColourBar{

public:

			ColourBar(window*);	
			~ColourBar();
  void			addItem(ColourItem&);
  void			display();
  void			dealWithMouseDown();
  Rectangle		extent();

  void			setForeground(Colour);
  void			setBackground(Colour);
  
private:

  void			displaySelection();
  
  SLList<ColourItem>	item;
  window*		w;
  Rectangle		ext;
  Colour		foreground;
  Colour		background;
  
};


#endif
