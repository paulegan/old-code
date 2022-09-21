
// DrawArea.h - DrawArea class definition.

#ifndef DrawAreaHeader
#define DrawAreaHeader

#include <misc.h>
#include <Window.h>

typedef enum { NullMode,LineMode,RectangleMode,EllipseMode,PolygonMode } Mode;
typedef enum { NoFill,FillWithoutBorder,FillWithBorder } FillStyle;

Mode const DefaultMode = NullMode;
FillStyle const DefaultFillStyle = NoFill;
int const DefaultLineWidth = 1;

class DrawArea{

public:

		DrawArea(window*,Rectangle);	
		~DrawArea();
  void		dealWithMouseDown();
  Rectangle	extent();
  void		clear();
  void		setMode(Mode);
  void		setForeground(Colour);
  void		setBackground(Colour);
  void		setLineWidth(int);
  void		setFillStyle(FillStyle);
  
private:

  void		doLineMode();
  void		doRectangleMode();
  void		doSquareMode();
  void		doEllipseMode();
  void		doCircleMode();
  void		doPolygonMode();
  
  window*	w;
  Rectangle	ext;
  Colour	fg;
  Colour	bg;
  Mode		mode;
  FillStyle	fillStyle;
  int		lineWidth;
  
};


#endif
