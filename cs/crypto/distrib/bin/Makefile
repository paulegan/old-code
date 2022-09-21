#
# Makefile by lumpy@cs.ucc.ie				29.8.1996
#

Server = 	server
Client = 	client
ClientLib = 	clientLib.a
SocketLib = 	socket.a

SourceDir = 	../src
SocketDir = 	${SourceDir}/socketLib
DESDir = 	${SourceDir}/desLib
RSADir = 	${SourceDir}/rsaLib


CC = 		gcc
INCLUDES = 	-I${SourceDir} -I${SocketDir} -I${DESDir} -I${RSADir} 
CFLAGS = 	-O ${INCLUDES}

Socket_CFLAGS =	-O -I${SocketDir}
DES_CFLAGS = 	-O3 -I${DESDir} -fomit-frame-pointer -DRAND -DTERMIO
RSA_CFLAGS = 	-O -I${RSADir} -DPROTOTYPES=0 -DUSA_ANSI=1


Socket_OBJS = 	Saccept.o      Sprintf.o      Stest.o        outofmem.o     \
       		Sclose.o       Sprtskt.o      Stimeoutwait.o rdcolor.o      \
       		Sgets.o        Sputs.o        sprt.o         Speeraddr.o    \
       		Smaskwait.o    Sread.o        Swait.o        srmtrblk.o     \
       		Smkskt.o       Sreadbytes.o   Swrite.o       stpblk.o       \
       		Sopen.o        Srmsrvr.o      error.o        stpnxt.o       \
       		Sopenv.o       Sscanf.o       fopenv.o       strnxtfmt.o    \
       		Speek.o        cprt.o         Speername.o

RSA_OBJS = 	rsa_crypt.o nn.o r_stdlib.o r_keygen.o r_random.o md5c.o prime.o

DES_OBJS = 	cfb64enc.o set_key.o ecb_enc.o str2key.o cbc_cksm.o

ClientLib_OBJS = clientLib.o ASCIIencode.o des.o rsa.o memalloc.o keyList.o \
		${Socket_OBJS} ${DES_OBJS} ${RSA_OBJS}

Server_OBJS = 	server.o clientList.o messageList.o memalloc.o ${SocketLib}



all: ${Server} ${ClientLib} ${Client} Spm

${Server}: ${Server_OBJS}
	${CC} ${CFLAGS} ${Server_OBJS} -o ${Server}

${ClientLib}: ${ClientLib_OBJS}
	ar r ${ClientLib} ${ClientLib_OBJS}
	ranlib ${ClientLib}

${Client}: client.o ${ClientLib}
	${CC} ${CFLAGS} client.o ${ClientLib} -o ${Client}

cryptTest1: cryptTest1.o ${ClientLib}
	${CC} ${CFLAGS} cryptTest1.o ${ClientLib} -o cryptTest1

cryptTest2: cryptTest2.o ${ClientLib}
	${CC} ${CFLAGS} cryptTest2.o ${ClientLib} -o cryptTest2

${SocketLib}: ${Socket_OBJS}
	ar r ${SocketLib} ${Socket_OBJS}
	ranlib ${SocketLib}

Spm: ${SocketLib}
	${CC} -O -I${SocketDir} ${SocketDir}/Spm.c ${SocketLib} -o Spm

clean: tidy
	@rm -fr ${Server} ${ClientLib} ${Client} Spm ${SocketLib} cryptTest1 cryptTest2

tidy:
	@rm -fr *.o

new: clean all

include targets.mak
