CC = gcc

CFLAGS=-DSOLARIS -O2 -D_REENTRANT -DMCC_HTTPD -DXP_UNIX -D__OS_SOLARIS__ -DSTRICT -D_DEBUG

# Compile with 03 optimization

# This is the location of your source code.
#INCLUDEDIR1=/home/toml/espn.36
INCLUDEDIR1=.

# This is the location of the NSAPI source files.
#INCLUDEDIR2=/files1/ns3.6/suitespot/include
INCLUDEDIR2=/opt/iplanet601/plugins/include


INCLUDE_FLAGS= -I$(INCLUDEDIR1) -I$(INCLUDEDIR2) -I$(INCLUDEDIR2)/base -I$(INCLUDEDIR2)/frame


AUTH_OBJS   = auth/CGoAuth.o auth/CGoAuthResponse.o \
		auth/CGoAuthIPCollection.o auth/CIP.o auth/CPremiumArea.o

GREEN_OBJS  = green/CGreen.o green/GreenUtils.o

UTILS_OBJS  = utils/CInterpreter.o utils/CLog.o utils/CTime.o utils/StrUtils.o \
		utils/QuickRegEx.o utils/UrlEncode.o utils/swport_mutex.o

XML_OBJS    = xml/CSimpleXMLParser.o xml/CXMLNode.o


# libC.so      can be found in /opt/SUNWspro4.2/lib/
# libstdc++.a  can be found in /usr/local/lib

LIBDIR = -L /usr/lib -L /usr/local/lib

LIBS = -lm -lw -lC -lnsl 

LD = ld -G


.SUFFIXES : .cpp .o

.cpp.o:
	@echo "\n============================"
	@echo "Invoking .cpp/.o rule..."
	@echo "============================\n"
	${CC} ${CFLAGS} ${INCLUDE_FLAGS} -fPIC -c $<

#default:
#	@echo "\n============================"
#	@echo "make release     : Build release version of GoAuth.so"
#	@echo "make debug       : Build debug version of GoAuth.so"
#	@echo "make build_auth  : Compile /auth source files"
#	@echo "make build_green : Compile /green source files"
#	@echo "make build_utils : Compile /utils source files"
#	@echo "make build_xml   : Compile /xml source files"
#	@echo "make clean       : Remove all .so and .o files...\n"

#main.o:
#	@echo "\n============================"
#	@echo "Compile main.cpp..."
#	@echo "============================\n"
#	${CC} -o main.o ${CFLAGS} ${INCLUDE_FLAGS} -pic -c main.cpp


#debug: main.o build_auth build_green build_utils build_xml
#	@echo "\n============================"
#	@echo "Building .so file..."
#	@echo "============================\n"
#	${LD} -o libGoAuth.so main.o ${AUTH_OBJS} ${GREEN_OBJS} ${UTILS_OBJS} ${XML_OBJS} ${LIBS}


#release: main.o build_auth build_green build_utils build_xml
#	@echo "\n============================"
#	@echo "Building .so file..."
#	@echo "============================\n"
#	${LD} -o libGoAuth.so main.o ${AUTH_OBJS} ${GREEN_OBJS} ${UTILS_OBJS} ${XML_OBJS} ${LIBS}

libGoAuth.so: main.o build_auth build_green build_utils build_xml
	@echo "\n============================"
	@echo "Building .so file..."
	@echo "============================\n"
	${LD} -o libGoAuth.so main.o ${AUTH_OBJS} ${GREEN_OBJS} ${UTILS_OBJS} ${XML_OBJS} ${LIBDIR} ${LIBS}


build_auth: 
	@echo "\n============================"
	@echo "Building auth..."
	@echo "============================\n"
	cd auth;  ${MAKE} auth

build_green: 
	@echo "\n============================"
	@echo "Building green..."
	@echo "============================\n"
	cd green;  ${MAKE} green

build_utils: 
	@echo "\n============================"
	@echo "Building utils..."
	@echo "============================\n"
	cd utils;  ${MAKE} utils

build_xml: 
	@echo "\n============================"
	@echo "Building xml..."
	@echo "============================\n"
	cd xml;  ${MAKE} xml
clean:
	@echo "\n============================"
	@echo "Removing all .o, .so files"
	@echo "(also in /auth, /green, /utils, /xml)..."
	@echo "============================\n"
	rm -f *.o *.so ${AUTH_OBJS} ${GREEN_OBJS} ${UTILS_OBJS} ${XML_OBJS}


