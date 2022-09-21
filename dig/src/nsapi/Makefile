CC      = gcc
CFLAGS  = -DSOLARIS -Wall -fPIC

LD      = ld
LDFLAGS = -G

INSTALL     = install -f
INSTALL_GRP = siteadm

SERVER_ROOT = /opt/iplanet601
INSTALL_DIR = $(SERVER_ROOT)/shlib
INCLUDE_DIR = $(SERVER_ROOT)/plugins/include

INCS = -I$(INCLUDE_DIR) # -I$(INCLUDE_DIR)/base -I$(INCLUDE_DIR)/frame
DEFS = -DMCC_HTTPD -DXP_UNIX -DNET_SSL -DSOLARIS -D_REENTRANT

######################################################################

all : DOLSwidCookie.so printLocation.so

install : DOLSwidCookie.so
	$(INSTALL) $(INSTALL_DIR) -g $(INSTALL_GRP) DOLSwidCookie.so

clean :
	/bin/rm -f *.o *.so

DOLSwidCookie.so : DOLSwidCookie.o

printLocation.so : printLocation.o

libStripPort.so : stripPort.o

%.o : %.c
	$(CC) $(CFLAGS) $(DEFS) $(INCS) -c $< 

%.so : %.o
	$(LD) $(LDFLAGS) $< -o $@ -lposix4

