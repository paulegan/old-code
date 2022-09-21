X_DIR     =	/usr/X11R6
LINK_X    =	-L${X_DIR}/lib -lX11
INCLUDE_X =	-I${X_DIR}/include

all: xstuff.o corewar

xstuff.o:
	gcc ${INCLUDE_X} -c xstuff.c
corewar:
	gcc ${LINK_X} ${INCLUDE_X} xstuff.o corewar.c -o mars
test:
	gcc ${LINK_X} ${INCLUDE_X} test.c xstuff.c -o test
debug:
	gcc ${LINK_X} ${INCLUDE_X} -DSHOWEXECS xstuff.o corewar.c -o mars

