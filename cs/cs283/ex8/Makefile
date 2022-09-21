X_DIR     =	/usr/X11R6
LINK_X    =	-L${X_DIR}/lib -lX11
INCLUDE_X =	-I${X_DIR}/include

all: xstuff.o main

xstuff.o:
	gcc ${INCLUDE_X} -c xstuff.c
main:
	gcc ${LINK_X} ${INCLUDE_X} xstuff.o maze.c -o maze
file:
	gcc makeMazeFile.c -o makeMazeFile
	makeMazeFile
