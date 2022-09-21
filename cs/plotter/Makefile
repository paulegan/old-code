
# Makefile for snow

# lumpy@cs.ucc.ie	2.3.1998

X11_INC		= /usr/X11/include
X11_LIB		= /usr/X11/lib
SRGP_DIR	= /usr/local/SRGP_DISTRIB
INCLUDES	= -I. -I${X11_INC}
LIBRARYS	= -lX11

CC		= gcc
CFLAGS		= -O3 ${INCLUDES}
LOADLIBES	= ${LIBRARYS}

OBJECTS		= plotter.o xstuff.o

plotter: ${OBJECTS}
	${CC} ${OBJECTS} $(LOADLIBES) -o $@

clean:
	@rm -f *.o
