
// PixMap.h - PixMap class definition.

#ifndef PixMapHeader
#define PixMapHeader

#include <misc.h>

class PixMap{

public:

		PixMap()					{ data=NULL; w=0; h=0; };
		PixMap(Colour* d, int width, int height)	{ data=d;  w=width; h=height; };
		~PixMap()					{};
  Colour	pix(int x, int y)				{ return(data[x+y*w]); };
  int		width()						{ return(w); };
  int		height()					{ return(h); };
  	
private:

  Colour*	data;
  int		w;
  int		h;

};


#endif
