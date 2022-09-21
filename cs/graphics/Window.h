
// Window.h - Window class definition.

#ifndef WindowHeader
#define WindowHeader

#include <Canvas.h>
#include <Bool.h>
#include <String.h>
#include <Point.h>
#include <misc.h>


typedef enum { LeftButton,RightButton,NoButton } MouseButton;

// Would call it Window ( with a capital W ), only X has taken that!

class window : public Canvas
{

public:

		window(String,int,int,int d=DefaultBitDepth);	
		~window();

  void		waitForKeypress();
  void		waitForMouseDown();
  Bool		mouseButtonDown();
  Point		mouseLocation();
  MouseButton	lastButton();

  void		flush();
  void		setClip(Rectangle);
  void		resetClip();

private:

  String	name;
  int		bitDepth;
  MouseButton	lastButtonPressed;

};


#endif

