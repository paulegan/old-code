#
# Makefile by lumpy@cs.ucc.ie			22.8.1996
#

CC = gcc
OPTIONS = -DRAND -DTERMIO
CFLAGS = -O3 -fomit-frame-pointer ${OPTIONS}

OBJS = cfb64enc.o set_key.o ecb_enc.o str2key.o cbc_cksm.o

all: ${OBJS}

clean:
	@rm -fr ${OBJS}
