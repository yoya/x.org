#
# Warning: the cpp used on this machine replaces
# all newlines and multiple tabs/spaces in a macro
# expansion with a single space.  Imake tries to
# compensate for this, but is not always
# successful.
#

#
# This makefile is automatically generated by imake... do not modify
# or you may lose your changes when imake generates makefiles again.
# Ignore this message if you are not using imake.
#

            TOP = ./../..
             AS = as
             CC = cc
            CPP = /lib/cpp
             LD = ld
           LINT = lint
        INSTALL = install
           TAGS = ctags
             RM = rm -f
             MV = mv
             LN = ln -s
         RANLIB = ranlib
             AR = ar clq
             LS = ls
       LINTOPTS = -axz
    LINTLIBFLAG = -C
           MAKE = make
    STD_DEFINES =
    CDEBUGFLAGS = -O
        DESTDIR =

        PATHSEP = /
         DEPEND = $(DEPENDSRC)/makedepend
          IMAKE = $(IMAKESRC)/imake
            RGB = $(RGBSRC)/rgb
         CFLAGS = $(CDEBUGFLAGS) $(INCLUDES) $(STD_DEFINES) $(DEFINES)
      LINTFLAGS = $(LINTOPTS) $(INCLUDES) $(STD_DEFINES) $(DEFINES) -DLINT
        LDFLAGS = $(CDEBUGFLAGS) $(SYS_LIBRARIES) $(SYSAUX_LIBRARIES)
   INSTUIDFLAGS = -m 4755
   INSTLIBFLAGS = -m 0664
   INSTINCFLAGS = -m 0444
   INSTMANFLAGS = -m 0444
   INSTAPPFLAGS = -m 0444

      USRLIBDIR = $(DESTDIR)/usr/lib
         BINDIR = $(DESTDIR)/usr/bin/X11
         LIBDIR = $(USRLIBDIR)/X11
     LINTLIBDIR = $(USRLIBDIR)/lint
         INCDIR = $(DESTDIR)/usr/include/X11
        FONTDIR = $(LIBDIR)/fonts
         UWMDIR = $(LIBDIR)/uwm
         MANDIR = $(DESTDIR)/usr/man/mann
    XAPPLOADDIR = $(LIBDIR)/app-defaults
         ADMDIR = $(DESTDIR)/usr/adm

      CLIENTSRC = $(TOP)/clients
        DEMOSRC = $(TOP)/demos
         LIBSRC = $(TOP)/lib
        FONTSRC = $(TOP)/fonts
     INCLUDESRC = $(TOP)/X11
      SERVERSRC = $(TOP)/server
        UTILSRC = $(TOP)/util
     EXAMPLESRC = $(TOP)/examples
     CONTRIBSRC = $(TOP)/contrib
         DOCSRC = $(TOP)/doc
      DEPENDSRC = $(UTILSRC)/makedepend
       IMAKESRC = $(UTILSRC)/imake
       IRULESRC = $(UTILSRC)/imake.includes
         RGBSRC = $(UTILSRC)/rgb
        XLIBSRC = $(LIBSRC)/X
     TOOLKITSRC = $(LIBSRC)/Xt
     AWIDGETSRC = $(LIBSRC)/Xaw
     OLDXLIBSRC = $(LIBSRC)/oldX
   EXTENSIONSRC = $(TOP)/extensions
        XMANSRC = $(DOCSRC)/Xlib/Xman
   EXTENSIONLIB = $(EXTENSIONSRC)/lib/libXext.a
           XLIB = $(XLIBSRC)/libX11.a
        OLDXLIB = $(OLDXLIBSRC)/liboldX.a
       XTOOLLIB = $(TOOLKITSRC)/libXt.a
         XAWLIB = $(AWIDGETSRC)/libXaw.a
       LINTXLIB = $(XLIBSRC)/llib-lX11.ln
      LINTXTOOL = $(TOOLKITSRC)/llib-lXt.ln
        LINTXAW = $(AWIDGETSRC)/llib-lXaw.ln
       INCLUDES = -I$(TOP)
      MACROFILE = Sun.macros
      IMAKE_CMD = $(NEWTOP)$(IMAKE) -TImake.tmpl \
			-I$(NEWTOP)$(IRULESRC) \
			-s Makefile
         RM_CMD = $(RM) *.CKP *.ln *.BAK *.bak *.o core errs ,* *~ *.a \
			tags TAGS make.log

# $Header: Imakefile,v 1.7 88/02/23 16:08:02 rws Exp $
       INCLUDES = -I$(TOP) -I$(INCLUDESRC) -I$(XLIBSRC) -I../include
           SRCS = XFillZoids.c XBezier.c
           OBJS = XFillZoids.o XBezier.o
       LINTLIBS = $(LINTXLIB)

.c.o:
	$(RM) $@
	$(CC) -c $(CFLAGS) $*.c

all:: libXext.a

libXext.a: $(OBJS)
	$(RM) $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

lintlib:: llib-lXext.ln

llib-lXext.ln: $(SRCS)
	$(RM) $@
	$(LINT) $(LINTLIBFLAG)Xext $(LINTFLAGS) $(SRCS)

install:: libXext.a
	$(INSTALL) -c $(INSTLIBFLAGS) libXext.a $(USRLIBDIR)
	$(RANLIB) $(USRLIBDIR)/libXext.a

install:: llib-lXext.ln
	$(INSTALL) -c $(INSTLIBFLAGS) llib-lXext.ln $(LINTLIBDIR)

depend:: $(DEPEND)

depend::
	$(DEPEND) -s "# DO NOT DELETE" $(CFLAGS) $(SRCS)

$(DEPEND):
	@echo "making $(DEPENDSRC)"; \
	cd $(DEPENDSRC); $(MAKE)

lint:
	$(LINT) $(LINTFLAGS) $(SRCS) $(LINTLIBS)
lint1:
	$(LINT) $(LINTFLAGS) $(FILE) $(LINTLIBS)

clean::
	$(RM_CMD) \#*

Makefile:: $(IMAKE)

Makefile:: Imakefile \
	$(IRULESRC)/Imake.tmpl \
	$(IRULESRC)/Imake.rules \
	$(IRULESRC)/$(MACROFILE)
	-$(RM) Makefile.bak; $(MV) Makefile Makefile.bak
	$(IMAKE_CMD) -DTOPDIR=$(TOP)

$(IMAKE):
	@echo "making $(IMAKESRC)"; \
	cd $(IMAKESRC); $(MAKE)

tags::
	$(TAGS) -w *.[ch]
	$(TAGS) -xw *.[ch] > TAGS

install::
	@echo "install done"

Makefiles::

