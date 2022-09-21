
# Makefile for paint

# lumpy@cs.ucc.ie	28.12.1997

X11_DIR		= /usr/X11R6
SRGP_DIR	= /usr/local/SRGP_DISTRIB
INCLUDES	= -I. -I${SRGP_DIR}/include -I${X11_DIR}/include
LIBRARYS	= ${SRGP_DIR}/lib/libsrgp.a ${X11_DIR}/lib/libX11.a

CXX		= g++
CXXFLAGS	= -fpcc-struct-return -g ${INCLUDES}
LOADLIBES	= ${LIBRARYS}

OBJECTS		= paint.o misc.o \
		  Window.o Canvas.o DrawArea.o \
		  ButtonBar.o Button.o \
		  OptionBox.o OptionBoxItem.o \
		  ColourBar.o ColourItem.o \
		  MenuBar.o Menu.o MenuItem.o 



paint: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} $(LOADLIBES) -o $@
	
clean:
	@rm -f *.o
