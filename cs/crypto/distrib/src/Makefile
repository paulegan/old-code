#
# Makefile by lumpy@cs.ucc.ie
#


SocketLibDir = ./socketLib
SocketLib = ${SocketLibDir}/socket.a

DESLibDir = ./desLib
DESLib = ${DESLibDir}/des.a

RSALibDir = ./rsaLib
RSALib = ${RSALibDir}/rsa.a

CC = gcc
INCLUDES = -I. -I${SocketLibDir} -I${DESLibDir} -I${RSALibDir} 
CFLAGS = -O ${INCLUDES}

ServerFiles = server.o clientList.o messageList.o memalloc.o ${SocketLib}
ClientLibFiles = clientLib.o ASCIIencode.o des.o rsa.o memalloc.o keyList.o



all: server clientLib.a client

server: ${ServerFiles}
	${CC} ${CFLAGS} ${ServerFiles} -o server

client: client.o clientLib.a
	${CC} ${CFLAGS} client.o clientLib.a -o client

cryptTest1: cryptTest.o clientLib.a
	${CC} ${CFLAGS} cryptTest.o clientLib.a -o cryptTest

cryptTest2: cryptTest2.o clientLib.a
	${CC} ${CFLAGS} cryptTest2.o clientLib.a -o cryptTest2

clientLib.a: ${ClientLibFiles} ${SocketLib} ${DESLib} ${RSALib}
	ar r clientLib.a ${ClientLibFiles} ${SocketLibDir}/*.o ${DESLibDir}/*.o ${RSALibDir}/*.o	
	ranlib clientLib.a

${SocketLib}:
	cd ${SocketLibDir}; make

${DESLib}:
	cd ${DESLibDir}; make	

${RSALib}:
	cd ${RSALibDir}; make

clean:
	@rm -fr *.o server clientLib.a client cryptTest
	@cd ${SocketLibDir}; make clean
	@cd ${DESLibDir}; make clean
	@cd ${RSALibDir}; make clean

new: clean all
