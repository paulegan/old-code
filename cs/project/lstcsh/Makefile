# Makefile generated by imake - do not edit!
# $XConsortium: imake.c /main/88 1995/12/08 16:33:13 gildea $

# ----------------------------------------------------------------------
# Makefile generated from "Imake.tmpl" and <Imakefile>
# $XConsortium: Imake.tmpl /main/r61_main/3 1996/02/02 18:57:32 gildea $
# $XFree86: xc/config/cf/Imake.tmpl,v 3.27 1996/09/14 13:05:02 dawes Exp $
#

.SUFFIXES: .i

# $XConsortium: Imake.cf /main/22 1995/12/05 16:39:53 mor $
# $XFree86: xc/config/cf/Imake.cf,v 3.28 1996/10/16 14:28:24 dawes Exp $

# -----------------------------------------------------------------------
# site-specific configuration parameters that need to come before
# the platform-specific parameters - edit site.def to change

# site:  $XConsortium: site.sample /main/19 1995/12/14 13:03:29 matt $
# site:  $XFree86: xc/config/cf/site.def,v 3.10 1996/01/07 09:57:49 dawes Exp $

# $XFree86: xc/config/cf/xf86site.def,v 3.91 1996/10/19 15:12:30 dawes Exp $

# ----------------------------------------------------------------------
# platform-specific configuration parameters - edit FreeBSD.cf to change

# platform:  $XConsortium: FreeBSD.cf /main/10 1996/01/22 17:35:30 gildea $
# platform:  $XFree86: xc/config/cf/FreeBSD.cf,v 3.54 1996/10/16 14:28:22 dawes Exp $

# operating system:  FreeBSD 2.1.5

   COMPRESSMANCMD = gzip -n

# $XConsortium: bsdLib.rules /main/2 1996/01/24 06:38:15 kaleb $

# $XFree86: xc/config/cf/bsdLib.rules,v 3.2 1996/10/03 08:28:08 dawes Exp $

# $XFree86: xc/config/cf/xfree86.cf,v 3.117 1996/10/19 15:12:31 dawes Exp $

# $XConsortium: xfree86.cf /main/17 1996/01/03 14:50:55 kaleb $

LINKKITDIR = $(USRLIBDIR)/Server
XF98LINKKITDIR = $(USRLIBDIR)/Server

       XF86SRC = $(SERVERSRC)/hw/xfree86
  XF86ACCELSRC = $(XF86SRC)/accel
    XF86COMSRC = $(XF86SRC)/common
 XF86CONFIGSRC = $(XF86COMSRC)
     XF86HWSRC = $(XF86SRC)/common_hw
     XF86OSSRC = $(XF86SRC)/os-support
  VGADRIVERSRC = $(XF86SRC)/vga256/drivers
VGA16DRIVERSRC = $(XF86SRC)/vga16/drivers
 VGA2DRIVERSRC = $(XF86SRC)/vga2/drivers
 MONODRIVERSRC = $(XF86SRC)/mono/drivers
   S3DRIVERSRC = $(XF86SRC)/accel/s3/drivers
  S3VDRIVERSRC = $(XF86SRC)/accel/s3_virge/drivers

       XF68SRC = $(SERVERSRC)/hw/xfree68
    XF68COMSRC = $(XF68SRC)/common
 XF68CONFIGSRC = $(XF68COMSRC)
     XF68OSSRC = $(XF68SRC)/os-support

           XF98SRC = $(SERVERSRC)/hw/xfree98
      XF98ACCELSRC = $(XF98SRC)/accel
        XF98COMSRC = $(XF98SRC)/common
     XF98CONFIGSRC = $(XF98COMSRC)
         XF98HWSRC = $(XF98SRC)/common_hw/generic
      XF98HWNECSRC = $(XF98SRC)/common_hw/nec
    XF98HWPWSKBSRC = $(XF98SRC)/common_hw/pwskb
     XF98HWPWLBSRC = $(XF98SRC)/common_hw/pwlb
    XF98HWGA968SRC = $(XF98SRC)/common_hw/ga968
         XF98OSSRC = $(XF98SRC)/os-support
  XF98VGADRIVERSRC = $(XF98SRC)/vga256/drivers
XF98VGA16DRIVERSRC = $(XF98SRC)/vga16/drivers
 XF98VGA2DRIVERSRC = $(XF98SRC)/vga2/drivers
 XF98MONODRIVERSRC = $(XF98SRC)/mono/drivers
XF98NECS3DRIVERSRC = $(XF98SRC)/accel/s3nec/drivers
XF98PWSKBDRIVERSRC = $(XF98SRC)/accel/s3pwskb/drivers
 XF98PWLBDRIVERSRC = $(XF98SRC)/accel/s3pwlb/drivers
XF98GA968DRIVERSRC = $(XF98SRC)/accel/s3ga968/drivers

        XFREE86DOCDIR = $(LIBDIR)/doc
      XFREE86PSDOCDIR = $(XFREE86DOCDIR)/PostScript
    XFREE86HTMLDOCDIR = $(XFREE86DOCDIR)/html
XFREE86JAPANESEDOCDIR = $(XFREE86DOCDIR)/Japanese

# $XConsortium: xf86.rules,v 1.7 95/01/25 16:34:39 kaleb Exp $
# $XFree86: xc/config/cf/xf86.rules,v 3.15 1996/09/24 13:48:46 dawes Exp $

# ----------------------------------------------------------------------
# site-specific configuration parameters that go after
# the platform-specific parameters - edit site.def to change

# site:  $XConsortium: site.sample /main/19 1995/12/14 13:03:29 matt $
# site:  $XFree86: xc/config/cf/site.def,v 3.10 1996/01/07 09:57:49 dawes Exp $

# ---------------------------------------------------------------------
# Imake rules for building libraries, programs, scripts, and data files
# rules:  $XConsortium: Imake.rules /main/208 1996/01/17 11:08:27 kaleb $
# rules:  $XFree86: xc/config/cf/Imake.rules,v 3.27 1996/09/01 04:12:46 dawes Exp $

 _NULLCMD_ = @ echo -n

          PATHSEP = /
            SHELL = /bin/sh

              TOP = .
      CURRENT_DIR = .

            IMAKE = imake
           DEPEND = gccmakedep
        MKDIRHIER = mkdir -p
        CONFIGSRC = $(TOP)/config
         IMAKESRC = $(CONFIGSRC)/imake
        DEPENDSRC = $(CONFIGSRC)/util

          INCROOT = /usr/X11R6/include
        USRLIBDIR = /usr/X11R6/lib
        VARLIBDIR = /var/lib
         SHLIBDIR = /usr/X11R6/lib
       LINTLIBDIR = $(USRLIBDIR)/lint
          MANPATH = /usr/X11R6/man
    MANSOURCEPATH = $(MANPATH)/man
           MANDIR = $(MANSOURCEPATH)$(MANSUFFIX)
        LIBMANDIR = $(MANSOURCEPATH)$(LIBMANSUFFIX)
       FILEMANDIR = $(MANSOURCEPATH)$(FILEMANSUFFIX)

               AR = ar clq
  BOOTSTRAPCFLAGS =
               CC = cc
               AS = as

.SUFFIXES: .cxx

              CXX = g++
    CXXDEBUGFLAGS = -m486 -O2
 CXXEXTRA_DEFINES =
CXXEXTRA_INCLUDES =
   CXXSTD_DEFINES = -DCSRG_BASED
       CXXOPTIONS =
      CXXINCLUDES = $(INCLUDES) $(TOP_INCLUDES) $(CXXEXTRA_INCLUDES)
       CXXDEFINES = $(CXXINCLUDES) $(CXXSTD_DEFINES) $(THREADS_CXXDEFINES) $(CXXEXTRA_DEFINES)
         CXXFLAGS = $(CXXDEBUGFLAGS) $(CXXOPTIONS) $(THREADS_CXXFLAGS) $(CXXDEFINES)

         COMPRESS = compress
              CPP = /usr/libexec/cpp $(STD_CPP_DEFINES)
    PREPROCESSCMD = /usr/libexec/cpp $(STD_CPP_DEFINES)
          INSTALL = /usr/bin/install
     INSTALLFLAGS = -c
               LD = ld
              LEX = lex
           LEXLIB = -ll
             YACC = yacc
           CCYACC = yacc
             LINT = lint
      LINTLIBFLAG = -C
         LINTOPTS = -axz
               LN = ln -s
             MAKE = make
               MV = mv -f
               CP = cp

           RANLIB = ranlib
  RANLIBINSTFLAGS =

               RM = rm -f
        MANSUFFIX = 1
     LIBMANSUFFIX = 3
    FILEMANSUFFIX = 5
            TROFF = groff -Tps
            NROFF = nroff
         MSMACROS = -ms
        MANMACROS = -man
              TBL = tbl
              EQN = eqn
             NEQN = neqn
              COL = col

            DVIPS = dvips
            LATEX = latex

     STD_INCLUDES =
  STD_CPP_DEFINES = -traditional
      STD_DEFINES = -DCSRG_BASED
 EXTRA_LOAD_FLAGS =
  EXTRA_LDOPTIONS =
  EXTRA_LIBRARIES = -lgnumalloc
             TAGS = ctags

    SHAREDCODEDEF =
         SHLIBDEF = -DBSDSHLIB

     SHLIBLDFLAGS = -Bshareable

         PICFLAGS = -fpic

      CXXPICFLAGS = -fpic

    PROTO_DEFINES = -DFUNCPROTO=15 -DNARROWPROTO

     INSTPGMFLAGS = -s

     INSTBINFLAGS = -m 0755
     INSTUIDFLAGS = -m 4711
     INSTLIBFLAGS = -m 0644
     INSTINCFLAGS = -m 0444
     INSTMANFLAGS = -m 0444
     INSTDATFLAGS = -m 0444
    INSTKMEMFLAGS = -g kmem -m 2755

      PROJECTROOT = /usr/X11R6

     TOP_INCLUDES = -I$(INCROOT)

      CDEBUGFLAGS = -m486 -O2
        CCOPTIONS =

      ALLINCLUDES = $(INCLUDES) $(EXTRA_INCLUDES) $(TOP_INCLUDES) $(STD_INCLUDES)
       ALLDEFINES = $(ALLINCLUDES) $(STD_DEFINES) $(EXTRA_DEFINES) $(PROTO_DEFINES) $(THREADS_DEFINES) $(DEFINES)
           CFLAGS = $(CDEBUGFLAGS) $(CCOPTIONS) $(THREADS_CFLAGS) $(ALLDEFINES)
        LINTFLAGS = $(LINTOPTS) -DLINT $(ALLDEFINES) $(DEPEND_DEFINES)
         LDPRELIB = -L$(USRLIBDIR)
        LDPOSTLIB =
        LDOPTIONS = $(CDEBUGFLAGS) $(CCOPTIONS)  $(EXTRA_LDOPTIONS) $(THREADS_LDFLAGS) $(LOCAL_LDFLAGS) $(LDPRELIB)
     CXXLDOPTIONS = $(CXXDEBUGFLAGS) $(CXXOPTIONS) $(EXTRA_LDOPTIONS) $(THREADS_CXXLDFLAGS) $(LOCAL_LDFLAGS) $(LDPRELIB)

           LDLIBS = $(LDPOSTLIB) $(THREADS_LIBS) $(SYS_LIBRARIES) $(EXTRA_LIBRARIES)

           CCLINK = $(CC)

          CXXLINK = $(CXX)

     LDSTRIPFLAGS = -x
   LDCOMBINEFLAGS = -X -r
      DEPENDFLAGS =

# Not sure this belongs here
         TKLIBDIR =
         TKINCDIR =
        TKLIBNAME = tk40
        TKLIBRARY = -LTkLibDir -lTkLibName
        TCLLIBDIR =
        TCLINCDIR =
       TCLLIBNAME = tcl74
       TCLLIBRARY = -LTclLibDir -lTclLibName

        MACROFILE = FreeBSD.cf
           RM_CMD = $(RM)

    IMAKE_DEFINES =

         IRULESRC = $(CONFIGDIR)
        IMAKE_CMD = $(IMAKE) -DUseInstalled -I$(IRULESRC) $(IMAKE_DEFINES)

     ICONFIGFILES = $(IRULESRC)/Imake.tmpl $(IRULESRC)/Project.tmpl 			$(IRULESRC)/site.def $(IRULESRC)/$(MACROFILE) 			$(IRULESRC)/xfree86.cf $(IRULESRC)/xf86.rules $(IRULESRC)/xf86site.def $(IRULESRC)/host.def $(EXTRA_ICONFIGFILES)

# ----------------------------------------------------------------------
# X Window System Build Parameters and Rules
# $XConsortium: Project.tmpl /main/r61_main/6 1996/01/15 12:26:59 gildea $
# $XFree86: xc/config/cf/Project.tmpl,v 3.34 1996/10/03 08:28:05 dawes Exp $

# -----------------------------------------------------------------------
# X Window System make variables; these need to be coordinated with rules

           BINDIR = /usr/X11R6/bin
     BUILDINCROOT = $(TOP)
      BUILDINCDIR = $(BUILDINCROOT)/X11
      BUILDINCTOP = ..
      BUILDLIBDIR = $(TOP)/usrlib
      BUILDLIBTOP = ..
           INCDIR = $(INCROOT)/X11
           ADMDIR = /usr/adm
           LIBDIR = $(USRLIBDIR)/X11

          FONTDIR = $(LIBDIR)/fonts
         XINITDIR = $(LIBDIR)/xinit
           XDMDIR = $(LIBDIR)/xdm
        XDMVARDIR = $(VARLIBDIR)/xdm
           TWMDIR = $(LIBDIR)/twm
           XSMDIR = $(LIBDIR)/xsm
           NLSDIR = $(LIBDIR)/nls
       XLOCALEDIR = $(LIBDIR)/locale
        PEXAPIDIR = $(LIBDIR)/PEX
      XAPPLOADDIR = $(LIBDIR)/app-defaults
       FONTCFLAGS = -t

     INSTAPPFLAGS = $(INSTDATFLAGS)

              RGB = rgb
            FONTC = bdftopcf
        MKFONTDIR = mkfontdir

       DOCUTILSRC = $(TOP)/doc/util
        CLIENTSRC = $(TOP)/clients
          DEMOSRC = $(TOP)/demos
       XDOCMACROS = $(DOCUTILSRC)/macros.t
       XIDXMACROS = $(DOCUTILSRC)/indexmacros.t
       PROGRAMSRC = $(TOP)/programs
           LIBSRC = $(TOP)/lib
          FONTSRC = $(TOP)/fonts
       INCLUDESRC = $(TOP)/X11
        SERVERSRC = $(TOP)/programs/Xserver
       CONTRIBSRC = $(TOP)/../contrib
   UNSUPPORTEDSRC = $(TOP)/unsupported
           DOCSRC = $(TOP)/doc
           RGBSRC = $(TOP)/programs/rgb
      BDFTOPCFSRC = $(PROGRAMSRC)/bdftopcf
     MKFONTDIRSRC = $(PROGRAMSRC)/mkfontdir
    FONTSERVERSRC = $(PROGRAMSRC)/xfs
       FONTINCSRC = $(TOP)/include/fonts
        EXTINCSRC = $(TOP)/include/extensions
     TRANSCOMMSRC = $(LIBSRC)/xtrans
   TRANS_INCLUDES = -I$(TRANSCOMMSRC)

# $XConsortium: bsdLib.tmpl,v 1.2 95/01/23 19:07:12 kaleb Exp $
# $XFree86: xc/config/cf/bsdLib.tmpl,v 3.1 1995/01/28 15:40:19 dawes Exp $

          XLIBSRC = $(LIBSRC)/X11

SOXLIBREV = 6.1
DEPXONLYLIB =
XONLYLIB =  -lX11

LINTXONLY = $(LINTLIBDIR)/llib-lX11.ln

         XLIBONLY = $(XONLYLIB)

      XEXTLIBSRC = $(LIBSRC)/Xext

SOXEXTREV = 6.1
DEPXEXTLIB =
XEXTLIB =  -lXext

LINTXEXT = $(LINTLIBDIR)/llib-lXext.ln

    XSSLIBSRC = $(LIBSRC)/Xss

DEPXSSLIB = $(USRLIBDIR)/libXss.a
XSSLIB =  -lXss

LINTXSS = $(LINTLIBDIR)/llib-lXss.ln

    XXF86MISCLIBSRC = $(LIBSRC)/Xxf86misc

DEPXXF86MISCLIB = $(USRLIBDIR)/libXxf86misc.a
XXF86MISCLIB =  -lXxf86misc

LINTXXF86MISC = $(LINTLIBDIR)/llib-lXxf86misc.ln

    XXF86VMLIBSRC = $(LIBSRC)/Xxf86vm

DEPXXF86VMLIB = $(USRLIBDIR)/libXxf86vm.a
XXF86VMLIB =  -lXxf86vm

LINTXXF86VM = $(LINTLIBDIR)/llib-lXxf86vm.ln

    XXF86DGALIBSRC = $(LIBSRC)/Xxf86dga

DEPXXF86DGALIB = $(USRLIBDIR)/libXxf86dga.a
XXF86DGALIB =  -lXxf86dga

LINTXXF86DGA = $(LINTLIBDIR)/llib-lXxf86dga.ln

     EXTENSIONLIB = $(XEXTLIB)
  DEPEXTENSIONLIB = $(DEPXEXTLIB)
    LINTEXTENSION = $(LINTXEXT)

 LINTEXTENSIONLIB = $(LINTEXTENSION)
          DEPXLIB = $(DEPEXTENSIONLIB) $(DEPXONLYLIB)
             XLIB = $(EXTENSIONLIB) $(XONLYLIB)
         LINTXLIB = $(LINTXONLYLIB)

         XAUTHSRC = $(LIBSRC)/Xau

DEPXAUTHLIB = $(USRLIBDIR)/libXau.a
XAUTHLIB =  -lXau

LINTXAUTH = $(LINTLIBDIR)/llib-lXau.ln

      XDMCPLIBSRC = $(LIBSRC)/Xdmcp

DEPXDMCPLIB = $(USRLIBDIR)/libXdmcp.a
XDMCPLIB =  -lXdmcp

LINTXDMCP = $(LINTLIBDIR)/llib-lXdmcp.ln

           XMUSRC = $(LIBSRC)/Xmu

SOXMUREV = 6.0
DEPXMULIB =
XMULIB =  -lXmu $(LDPRELIB) $(XTOOLONLYLIB) $(XONLYLIB)

LINTXMU = $(LINTLIBDIR)/llib-lXmu.ln

       OLDXLIBSRC = $(LIBSRC)/oldX

SOOLDXREV = 6.0
DEPOLDXLIB =
OLDXLIB =  -loldX

LINTOLDX = $(LINTLIBDIR)/llib-loldX.ln

       TOOLKITSRC = $(LIBSRC)/Xt

SOXTREV = 6.0
DEPXTOOLONLYLIB =
XTOOLONLYLIB =  -lXt

LINTXTOOLONLY = $(LINTLIBDIR)/llib-lXt.ln

      DEPXTOOLLIB = $(DEPXTOOLONLYLIB) $(DEPSMLIB) $(DEPICELIB)
         XTOOLLIB = $(XTOOLONLYLIB) $(SMLIB) $(ICELIB)
     LINTXTOOLLIB = $(LINTXTOOLONLYLIB)

       AWIDGETSRC = $(LIBSRC)/Xaw

SOXAWREV = 6.1
DEPXAWLIB =
XAWLIB =  -lXaw

LINTXAW = $(LINTLIBDIR)/llib-lXaw.ln

         XILIBSRC = $(LIBSRC)/Xi

SOXINPUTREV = 6.0
DEPXILIB =
XILIB =  -lXi

LINTXI = $(LINTLIBDIR)/llib-lXi.ln

      XTESTLIBSRC = $(LIBSRC)/Xtst

SOXTESTREV = 6.1
DEPXTESTLIB =
XTESTLIB =  -lXtst

LINTXTEST = $(LINTLIBDIR)/llib-lXtst.ln

        PEXLIBSRC = $(LIBSRC)/PEX5

SOPEXREV = 6.0
DEPPEXLIB =
PEXLIB =  -lPEX5

LINTPEX = $(LINTLIBDIR)/llib-lPEX5.ln

        XIELIBSRC = $(LIBSRC)/XIE

SOXIEREV = 6.0
DEPXIELIB =
XIELIB =  -lXIE

LINTXIE = $(LINTLIBDIR)/llib-lXIE.ln

      PHIGSLIBSRC = $(LIBSRC)/PHIGS

DEPPHIGSLIB = $(USRLIBDIR)/libphigs.a
PHIGSLIB =  -lphigs

LINTPHIGS = $(LINTLIBDIR)/llib-lphigs.ln

DEPXBSDLIB = $(USRLIBDIR)/libXbsd.a
XBSDLIB =  -lXbsd

LINTXBSD = $(LINTLIBDIR)/llib-lXbsd.ln

           ICESRC = $(LIBSRC)/ICE

SOICEREV = 6.0
DEPICELIB =
ICELIB =  -lICE

LINTICE = $(LINTLIBDIR)/llib-lICE.ln

            SMSRC = $(LIBSRC)/SM

SOSMREV = 6.0
DEPSMLIB =
SMLIB =  -lSM

LINTSM = $(LINTLIBDIR)/llib-lSM.ln

         FSLIBSRC = $(LIBSRC)/FS

DEPFSLIB = $(USRLIBDIR)/libFS.a
FSLIB =  -lFS

LINTFS = $(LINTLIBDIR)/llib-lFS.ln

         FONTLIBSRC = $(LIBSRC)/font

DEPFONTLIB = $(USRLIBDIR)/libfont.a
FONTLIB =  -lfont

LINTFONT = $(LINTLIBDIR)/llib-lfont.ln

          XPMLIBSRC = $(LIBSRC)/Xpm

DEPXPMLIB = $(USRLIBDIR)/libXpm.a
XPMLIB =  -lXpm

LINTXPM = $(LINTLIBDIR)/llib-lXpm.ln

    XKBFILELIBSRC = $(LIBSRC)/xkbfile

DEPXKBFILELIB = $(USRLIBDIR)/libxkbfile.a
XKBFILELIB =  -lxkbfile

LINTXKBFILE = $(LINTLIBDIR)/llib-lxkbfile.ln

     XKBCOMPCMD = xkbcomp

    XKBUILIBSRC = $(LIBSRC)/xkbui

DEPXKBUILIB = $(USRLIBDIR)/libxkbui.a
XKBUILIB =  -lxkbui

LINTXKBUI = $(LINTLIBDIR)/llib-lxkbui.ln

          DEPLIBS = $(DEPXAWLIB) $(DEPXMULIB) $(DEPXTOOLLIB) $(DEPXLIB)

         DEPLIBS1 = $(DEPLIBS)
         DEPLIBS2 = $(DEPLIBS)
         DEPLIBS3 = $(DEPLIBS)

        CONFIGDIR = $(LIBDIR)/config

# ----------------------------------------------------------------------
# start of Imakefile

#
# $Id$
#
# Imakefile for tcsh 6.07
# Marc Horowitz, MIT SIPB
#

SYSSRCS=ma.setp.c
SYSOBJS=ma.setp.${SUF}
EXTF=mi.termios.c mi.wait.h mi.varargs.h vms.termcap.c

LIBTERMCAP = -ltermcap

LIBCURSES =

LIBNET =

LIBSOCKET =

LIBBSD =

LIBC_S =

LIBSUN =

LIBCPOSIX =

LIBINET =

LIBDIRECT =

LIBX =

LIBINTL =

LIBCRYPT = -lcrypt

LIBPOSIX =

LIBDIRECTENT =

SYSLIBS = $(LIBPOSIX) $(LIBDIRECTENT) $(LIBTERMCAP) $(LIBCURSES) 	$(LIBNET) $(LIBINTL) $(LIBSOCKET) $(LIBSUN) $(LIBBSD) $(LIBCPOSIX) 	$(LIBINET) $(LIBDIRECT) $(LIBX) $(LIBC_S) $(LIBSQNT) $(LIBCRYPT)

CC = gcc

HESLIB =
HESDEF =

AFSLIB =
AFSDEF =

PARALLEL=12				# Make the multi-max run fast.

TCSHTOP = /usr/local

MANSUFFIX = 1

TCSHPATH =

TCSH_BINDIR = $(TCSHTOP)/bin

TCSH_MANDIR = $(TCSHTOP)/man/man$(MANSUFFIX)

LOCALLIBS =

CDEBUGFLAGS = -O2

HTDEF =

DEFINES = $(TCSHPATH) $(HESDEF) $(AFSDEF) $(HTDEF)
INCLUDES = -I.

SUF = o
VERSION = 6.07

SHSRCS=	sh.c sh.dir.c sh.dol.c sh.err.c sh.exec.c sh.char.c 	sh.exp.c sh.file.c sh.func.c sh.glob.c sh.hist.c sh.init.c 	sh.lex.c sh.ls.c sh.misc.c sh.parse.c sh.print.c sh.proc.c sh.sem.c 	sh.set.c sh.time.c sh.char.h sh.dir.h sh.proc.h sh.h 	sh.decls.h glob.c glob.h ${SYSSRCS}

SHOBJS=	sh.${SUF} sh.dir.${SUF} sh.dol.${SUF} sh.err.${SUF} sh.exec.${SUF} 	sh.char.${SUF} sh.exp.${SUF} sh.func.${SUF} sh.glob.${SUF} 	sh.hist.${SUF} sh.init.${SUF} sh.lex.${SUF} sh.ls.${SUF} sh.misc.${SUF} 	sh.parse.${SUF} sh.print.${SUF} sh.proc.${SUF} sh.sem.${SUF} 	sh.set.${SUF} sh.time.${SUF} glob.${SUF} ${SYSOBJS}

TWSRCS= tw.decls.h tw.h tw.help.c tw.init.c tw.parse.c tw.spell.c 	tw.comp.c

TWOBJS=	tw.help.${SUF} tw.init.${SUF} tw.parse.${SUF} tw.spell.${SUF} 	tw.comp.${SUF}

EDSRCS= ed.chared.c ed.decls.h ed.defns.c ed.h ed.init.c ed.inputl.c 	ed.refresh.c ed.screen.c ed.xmap.c ed.term.c ed.term.h

EDOBJS=	ed.chared.${SUF} ed.refresh.${SUF} ed.screen.${SUF} ed.init.${SUF} 	ed.inputl.${SUF} ed.defns.${SUF} ed.xmap.${SUF} ed.term.${SUF}

TCSRCS= tc.alloc.c tc.bind.c tc.const.c tc.decls.h tc.disc.c 	tc.func.c tc.os.c tc.os.h tc.printf.c tc.prompt.c 	tc.sched.c tc.sig.c tc.sig.h tc.str.c sh.types.h tc.vers.c tc.wait.h 	tc.who.c tc.h

TCOBJS=	tc.alloc.${SUF} tc.bind.${SUF} tc.const.${SUF} tc.defs.${SUF} 	tc.disc.${SUF} tc.func.${SUF} tc.os.${SUF} tc.printf.${SUF} 	tc.prompt.${SUF} tc.sched.${SUF} tc.sig.${SUF} tc.str.${SUF} 	tc.vers.${SUF} tc.who.${SUF}

MISCF = Makefile.std Fixes MAKEDIFFS MAKESHAR NewThings README FAQ 	WishList config_f.h eight-bit.me glob.3 patchlevel.h 	pathnames.h tcsh.man Ported src.desc Imakefile imake.config 	README.imake complete.tcsh vmsreadme.txt Makefile.vms termcap.vms 	snames.h host.defs gethost.c tcsh.man2html Makefile.in configure.in

CONFSRCS=config/[a-z]*

SRCS = $(SHSRCS) $(TWSRCS) $(EDSRCS) $(TCSRCS)
OBJS = $(SHOBJS) $(TWOBJS) $(EDOBJS) $(TCOBJS)

ALLSRCS= $(MISCF) $(SRCS) $(EXTF)

all:: tcsh

ed.defns.h: config.h ed.defns.c
	@rm -f $@
	@echo '/* Do not edit this file, make creates it. */' > $@
	@echo '#ifndef _h_ed_defns' >> $@
	@echo '#define _h_ed_defns' >> $@
	egrep '[FV]_' ed.defns.c | egrep '^#define' >> $@
	@echo '#endif /* _h_ed_defns */' >> $@

sh.err.h: config.h sh.err.c
	@rm -f $@
	@echo '/* Do not edit this file, make creates it. */' > $@
	@echo '#ifndef _h_sh_err' >> $@
	@echo '#define _h_sh_err' >> $@
	egrep 'ERR_' sh.err.c | egrep '^#define' >> $@
	@echo '#endif /* _h_sh_err */' >> $@

tc.const.h: config.h tc.const.c
	@rm -f $@
	@echo '/* Do not edit this file, make creates it. */' > $@
	@echo '#ifndef _h_tc_const' >> $@
	@echo '#define _h_tc_const' >> $@
	${CC} -E $(INCLUDES) ${DEFINES} -D_h_tc_const tc.const.c | 	    grep 'Char STR' | 	    sed -e 's/Char \([a-zA-Z0-9_]*\)\[\].*/extern Char \1[];/' | 	    sort >> $@

	@echo '#endif /* _h_tc_const */' >> $@

config.h: config_f.h
	cp config/bsd4.4 config.h

$(OBJS): sh.err.h tc.const.h ed.defns.h

tar.Z:
	rm -f tcsh-${VERSION}.tar.Z
	rm -rf tcsh-${VERSION}
	mkdir tcsh-${VERSION} tcsh-${VERSION}/config
	cp ${ALLSRCS} tcsh-${VERSION}
	cp ${CONFSRCS} tcsh-${VERSION}/config
	tar cf - nls/Makefile nls/?*/set?* | (cd tcsh-${VERSION}; tar xpf -)
	tar cvf - tcsh-${VERSION} | compress > tcsh-${VERSION}.tar.Z
	rm -rf tcsh-${VERSION}

tar.gz:
	rm -f tcsh-${VERSION}.tar.gz
	rm -rf tcsh-${VERSION}
	mkdir tcsh-${VERSION} tcsh-${VERSION}/config
	cp ${ALLSRCS} tcsh-${VERSION}
	cp ${CONFSRCS} tcsh-${VERSION}/config
	tar cf - nls/Makefile nls/?*/set?* | (cd tcsh-${VERSION}; tar xpf -)
	tar cvf - tcsh-${VERSION} | gzip > tcsh-${VERSION}.tar.gz
	rm -rf tcsh-${VERSION}

shar:
	rm -f tcsh-*.shar
	rm -rf tcsh-${VERSION}
	mkdir tcsh-${VERSION} tcsh-${VERSION}/config
	cp ${ALLSRCS} tcsh-${VERSION}
	cp ${CONFSRCS} tcsh-${VERSION}/config
	tar cf - nls/Makefile nls/?*/set?* | (cd tcsh-${VERSION}; tar xpf -)
	MAKESHAR -v -n tcsh-${VERSION} tcsh-${VERSION} 		 tcsh-${VERSION}/?* tcsh-${VERSION}/config/?* 		 tcsh-${VERSION}/?*/set?*

	rm -rf tcsh-${VERSION}

catalogs:
	@(cd nls; make catalogs)

world:
	$(MAKE) clean ; $(MAKE) depend ; $(MAKE) tcsh ; $(MAKE) install

clean::
	rm -f ed.defns.h sh.err.h tc.const.h config.h tc.defs.*
	rm -f tcsh.*.m tcsh.*.cat

depend:: config.h ed.defns.h sh.err.h tc.const.h $(SRCS) tc.defs.c

tc.defs.${SUF}:	tc.defs.c sh.h

tc.defs.c:	gethost host.defs
	@rm -f $@
	@echo "/* Do not edit this file, make creates it */" > $@
	./gethost host.defs >> $@

ALIB=$(HESLIB) $(AFSLIB) $(SYSLIBS)
AINC=ed.defns.h sh.err.h tc.const.h sh.h

tcsh:  $(OBJS)  $(AINC)
	$(RM) $@
	$(CCLINK) -o $@ $(LDOPTIONS)  $(OBJS)  $(LOCALLIBS) $(LDLIBS)  $(ALIB) $(EXTRA_LOAD_FLAGS)

clean::
	$(RM) tcsh

gethost:  gethost.${SUF}  $(AINC)
	$(RM) $@
	$(CCLINK) -o $@ $(LDOPTIONS)  gethost.${SUF}  $(LOCALLIBS) $(LDLIBS)  $(ALIB) $(EXTRA_LOAD_FLAGS)

clean::
	$(RM) gethost

install:: tcsh
	@if [ -d $(DESTDIR)$(TCSH_BINDIR) ]; then set +x; \
	else (set -x; $(MKDIRHIER) $(DESTDIR)$(TCSH_BINDIR)); fi
	$(INSTALL) $(INSTALLFLAGS) $(INSTPGMFLAGS)  tcsh $(DESTDIR)$(TCSH_BINDIR)/tcsh

install.man:: tcsh.man
	@if [ -d $(DESTDIR)$(TCSH_MANDIR) ]; then set +x; \
	else (set -x; $(MKDIRHIER) $(DESTDIR)$(TCSH_MANDIR)); fi
	$(RM) $(DESTDIR)$(TCSH_MANDIR)/tcsh.$(MANSUFFIX)*
	$(INSTALL) -c $(INSTMANFLAGS) tcsh.man \
 $(DESTDIR)$(TCSH_MANDIR)/tcsh.$(MANSUFFIX)
	-$(COMPRESSMANCMD) $(DESTDIR)$(TCSH_MANDIR)/tcsh.$(MANSUFFIX)

depend:: .depend

.depend::
	$(RM) $@
	$(DEPEND) -f- $(DEPENDFLAGS) -- $(ALLDEFINES) $(DEPEND_DEFINES) -- $(SRCS) > $@

# ----------------------------------------------------------------------
# common rules for all Makefiles - do not edit

.c.i:
	$(RM) $@
		$(CC) -E $(CFLAGS) $(_NOOP_) $*.c > $@

emptyrule::

clean::
	$(RM) *.CKP *.ln *.BAK *.bak *.o core errs ,* *~ *.a .emacs_* tags TAGS make.log MakeOut *.core "#"*

Makefile::
	-@if [ -f Makefile ]; then set -x; \
	$(RM) Makefile.bak; $(MV) Makefile Makefile.bak; \
	else exit 0; fi
	$(IMAKE_CMD) -DTOPDIR=$(TOP) -DCURDIR=$(CURRENT_DIR)

tags::
	$(TAGS) -w *.[ch]
	$(TAGS) -xw *.[ch] > TAGS

man_keywords::

# ----------------------------------------------------------------------
# empty rules for directories that do not have SUBDIRS - do not edit

install::
	@echo "install in $(CURRENT_DIR) done"

install.man::
	@echo "install.man in $(CURRENT_DIR) done"

install.linkkit::
	@echo "install.linkkit in $(CURRENT_DIR) done"

Makefiles::

includes::

depend::

# ----------------------------------------------------------------------
# dependencies generated by makedepend

# dependencies are in .depend

