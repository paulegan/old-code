#
# Makefile for rsaeuro		lumpy@cs.ucc.ie
#


CC = gcc
INCLUDES =  -I.
OPTIONS =  -DPROTOTYPES=0 -DUSA_ANSI=1
CFLAGS = -O ${INCLUDES} ${OPTIONS}

OBJS = rsa.o nn.o r_stdlib.o r_keygen.o r_random.o md5c.o prime.o

all :  ${OBJS}

clean:
	@rm -fr ${OBJS}
