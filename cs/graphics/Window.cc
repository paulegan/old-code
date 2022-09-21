
// Window.cc

#include <srgp.h>
#include <Window.h>

// Window constructor.
window::window(String n, int w, int h, int d) : Canvas(w,h,TRUE)
{
 name = n;
 myID = 0;
 bitDepth = d;
 lastButtonPressed=NoButton;
 SRGP_begin(name,width,height,bitDepth,FALSE);
 SRGP_setLocatorButtonMask(LEFT_BUTTON_MASK | RIGHT_BUTTON_MASK);
 loadColours();
 loadFonts();
}// window


// window destructor.
window::~window()
{
 SRGP_end();
}// ~window


void window::waitForKeypress()
{
 SRGP_setInputMode(KEYBOARD,EVENT);
 SRGP_setKeyboardProcessingMode(RAW);
 SRGP_waitEvent(INDEFINITE);
}// waitForKeypress

void window::waitForMouseDown()
{
 locator_measure mouseInfo;
 
 SRGP_setInputMode(LOCATOR,EVENT);
 SRGP_waitEvent(INDEFINITE);
 SRGP_getLocator(&mouseInfo);
 if ( mouseInfo.buttonChord[LEFT_BUTTON]==DOWN )
   lastButtonPressed=LeftButton;
 else if ( mouseInfo.buttonChord[RIGHT_BUTTON]==DOWN )
   lastButtonPressed=RightButton;
}// waitForMouseDown

Bool window::mouseButtonDown()
{
 locator_measure mouseInfo;
 
 SRGP_setInputMode(LOCATOR,SAMPLE);
 SRGP_sampleLocator(&mouseInfo);
 if ( mouseInfo.buttonChord[LEFT_BUTTON]==DOWN ){
   lastButtonPressed=LeftButton;
   return(true); 
 }
 else if ( mouseInfo.buttonChord[RIGHT_BUTTON]==DOWN ){
   lastButtonPressed=RightButton;
   return(true); 
 }
 else return(false);
}// mouseButtonDown

Point window::mouseLocation()
{
 locator_measure mouseInfo;
 
 SRGP_setInputMode(LOCATOR,SAMPLE);
 SRGP_sampleLocator(&mouseInfo);
 return(Point(mouseInfo.position.x,mouseInfo.position.y));
}// mouseLocation

MouseButton window::lastButton()
{
 return(lastButtonPressed);
}// lastButton

void window::flush()
{
 SRGP_refresh();
}// flush

void window::setClip(Rectangle r)
{
 SRGP_setClipRectangle(SRGP_defRectangle(r.bottomLeft().xValue(),r.bottomLeft().yValue(),r.topRight().xValue(),r.topRight().yValue()));
}// setClip


void window::resetClip()
{
 SRGP_setClipRectangle(SRGP_defRectangle(0,0,width-1,height-1));
}// resetClip

