###########################################################
# makefile for auth
#

CC = gcc

CFLAGS=-DSOLARIS -O2 -D_REENTRANT -DMCC_HTTPD -DXP_UNIX -D__OS_SOLARIS__ -DSTRICT

#INCLUDEDIR2=/files1/ns3.6/suitespot/include
INCLUDEDIR2=/opt/iplanet601/plugins/include

INCLUDE_FLAGS= -I$(INCLUDEDIR2) -I$(INCLUDEDIR2)/base -I$(INCLUDEDIR2)/frame

AUTH_OBJS = CGoAuth.o CGoAuthIPCollection.o CGoAuthResponse.o CIP.o CPremiumArea.o


# Suffix rules
.SUFFIXES : .cpp .o

.cpp.o:
	${CC} ${CFLAGS} ${INCLUDE_FLAGS} -fPIC -c $<

auth: ${AUTH_OBJS}


clean:
	rm -f *.o 





