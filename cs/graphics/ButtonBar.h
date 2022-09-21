
// ButtonBar.h - ButtonBar class definition.

#ifndef ButtonBarHeader
#define ButtonBarHeader

#include <Bool.h>
#include <Button.h>
#include <OptionBox.h>
#include <SLList.h>

int const ButtonBarMargin = 5;
int const ButtonSize = 22;
int const NumColumns = 2;
int const Spacing = ButtonBarMargin+ButtonSize;

class ButtonBar{

public:

			ButtonBar(window*,int,int);	
			~ButtonBar();
  void			addItem(Button&);
  void			display();
  void			dealWithMouseDown();
  Rectangle		extent();
  void			setOptionBoxTo(OptionBox*);
  
private:

  SLList<Button>	item;
  window*		w;
  Rectangle		ext;
  int 			height;
  OptionBox*		optBox;

};


#endif
