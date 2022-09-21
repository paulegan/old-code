
// Misc.h - Misc stuff.

#ifndef MiscHeader
#define MiscHeader

#include <Rectangle.h>
#include <String.h>

typedef void	(*FunctionPtr)();

// Colour types & constants:
int const	DefaultBitDepth = 5;
typedef int	Colour;
Colour const	White = 0;
Colour const	Black = 1;
Colour const	Grey40 = 2;
Colour const	Grey85 = 3;
Colour const	Grey95 = 4;
Colour const	Red = 5;
Colour const	Green = 6;
Colour const	Blue = 7;
Colour const	Yellow = 8;
Colour const	Cyan = 9;
Colour const	Magenta = 10;
Colour const	DarkRed = 11;
Colour const	DarkYellow = 12;
Colour const	DarkGreen = 13;
Colour const	DarkCyan = 14;
Colour const	DarkBlue = 15;
Colour const	DarkMagenta = 16;
Colour const	DefaultGrey = Grey85;
Colour const	ButtonShadeColour = Grey40;
Colour const	ButtonHighlightColour = Grey95;
Colour const	MenuFG = Black;
Colour const	MenuBG = DefaultGrey;
Colour const	MenuHL = Blue;
Colour const	MenuDA = Grey40;
Colour const	DefaultFG = Black;
Colour const	DefaultBG = White;

// Font types & constants:
typedef int	FontID;
FontID const	DefaultFont = 0;
FontID const	HelvBold16 = 1;
FontID const	MenuFont = HelvBold16;

// Misc functions:
void		loadColours();
void		loadFonts();
Rectangle	textExtent(String,FontID);

#endif

