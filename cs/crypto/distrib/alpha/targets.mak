
#
# Main object files:
#

clientLib.o: ${SourceDir}/clientLib.c
	${CC} ${CFLAGS} -c ${SourceDir}/clientLib.c
ASCIIencode.o: ${SourceDir}/ASCIIencode.c
	${CC} ${CFLAGS} -c ${SourceDir}/ASCIIencode.c
des.o: ${SourceDir}/des.c
	${CC} ${CFLAGS} -c ${SourceDir}/des.c
rsa.o: ${SourceDir}/rsa.c
	${CC} ${CFLAGS} -c ${SourceDir}/rsa.c
memalloc.o: ${SourceDir}/memalloc.c
	${CC} ${CFLAGS} -c ${SourceDir}/memalloc.c
server.o: ${SourceDir}/server.c
	${CC} ${CFLAGS} -c ${SourceDir}/server.c
clientList.o: ${SourceDir}/clientList.c
	${CC} ${CFLAGS} -c ${SourceDir}/clientList.c
messageList.o: ${SourceDir}/messageList.c
	${CC} ${CFLAGS} -c ${SourceDir}/messageList.c
keyList.o: ${SourceDir}/keyList.c
	${CC} ${CFLAGS} -c ${SourceDir}/keyList.c
client.o: ${SourceDir}/client.c
	${CC} ${CFLAGS} -c ${SourceDir}/client.c
cryptTest1.o: ${SourceDir}/cryptTest1.c
	${CC} ${CFLAGS} -c ${SourceDir}/cryptTest1.c
cryptTest2.o: ${SourceDir}/cryptTest2.c
	${CC} ${CFLAGS} -c ${SourceDir}/cryptTest2.c

#
# Socket object files:
#

Saccept.o: ${SocketDir}/Saccept.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Saccept.c
Sprintf.o: ${SocketDir}/Sprintf.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Sprintf.c
Stest.o: ${SocketDir}/Stest.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Stest.c
outofmem.o: ${SocketDir}/outofmem.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/outofmem.c
Sclose.o: ${SocketDir}/Sclose.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Sclose.c
Sprtskt.o: ${SocketDir}/Sprtskt.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Sprtskt.c
Stimeoutwait.o: ${SocketDir}/Stimeoutwait.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Stimeoutwait.c
rdcolor.o: ${SocketDir}/rdcolor.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/rdcolor.c
Sgets.o: ${SocketDir}/Sgets.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Sgets.c
Sputs.o: ${SocketDir}/Sputs.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Sputs.c
sprt.o: ${SocketDir}/sprt.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/sprt.c
Speeraddr.o: ${SocketDir}/Speeraddr.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Speeraddr.c
Smaskwait.o: ${SocketDir}/Smaskwait.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Smaskwait.c
Sread.o: ${SocketDir}/Sread.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Sread.c
Swait.o: ${SocketDir}/Swait.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Swait.c
srmtrblk.o: ${SocketDir}/srmtrblk.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/srmtrblk.c
Smkskt.o: ${SocketDir}/Smkskt.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Smkskt.c
Sreadbytes.o: ${SocketDir}/Sreadbytes.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Sreadbytes.c
Swrite.o: ${SocketDir}/Swrite.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Swrite.c
stpblk.o: ${SocketDir}/stpblk.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/stpblk.c
Sopen.o: ${SocketDir}/Sopen.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Sopen.c
Srmsrvr.o: ${SocketDir}/Srmsrvr.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Srmsrvr.c
error.o: ${SocketDir}/error.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/error.c
stpnxt.o: ${SocketDir}/stpnxt.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/stpnxt.c
Sopenv.o: ${SocketDir}/Sopenv.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Sopenv.c
Sscanf.o: ${SocketDir}/Sscanf.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Sscanf.c
fopenv.o: ${SocketDir}/fopenv.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/fopenv.c
strnxtfmt.o: ${SocketDir}/strnxtfmt.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/strnxtfmt.c
Speek.o: ${SocketDir}/Speek.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Speek.c
cprt.o: ${SocketDir}/cprt.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/cprt.c
Speername.o: ${SocketDir}/Speername.c
	${CC} ${Socket_CFLAGS} -c ${SocketDir}/Speername.c

#
# RSA object files:
#

rsa_crypt.o: ${RSADir}/rsa_crypt.c
	${CC} ${RSA_CFLAGS} -c ${RSADir}/rsa_crypt.c
nn.o: ${RSADir}/nn.c
	${CC} ${RSA_CFLAGS} -c ${RSADir}/nn.c
r_stdlib.o: ${RSADir}/r_stdlib.c
	${CC} ${RSA_CFLAGS} -c ${RSADir}/r_stdlib.c
r_keygen.o: ${RSADir}/r_keygen.c
	${CC} ${RSA_CFLAGS} -c ${RSADir}/r_keygen.c
r_random.o: ${RSADir}/r_random.c
	${CC} ${RSA_CFLAGS} -c ${RSADir}/r_random.c
md5c.o: ${RSADir}/md5c.c
	${CC} ${RSA_CFLAGS} -c ${RSADir}/md5c.c
prime.o: ${RSADir}/prime.c
	${CC} ${RSA_CFLAGS} -c ${RSADir}/prime.c

#
# DES object files:
#

cfb64enc.o: ${DESDir}/cfb64enc.c
	${CC} ${DES_CFLAGS} -c ${DESDir}/cfb64enc.c
set_key.o: ${DESDir}/set_key.c
	${CC} ${DES_CFLAGS} -c ${DESDir}/set_key.c
ecb_enc.o: ${DESDir}/ecb_enc.c
	${CC} ${DES_CFLAGS} -c ${DESDir}/ecb_enc.c
str2key.o: ${DESDir}/str2key.c
	${CC} ${DES_CFLAGS} -c ${DESDir}/str2key.c
cbc_cksm.o: ${DESDir}/cbc_cksm.c
	${CC} ${DES_CFLAGS} -c ${DESDir}/cbc_cksm.c

