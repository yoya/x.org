# Makefile generated by imake - do not edit!
# $XConsortium: imake.c,v 1.37 88/10/08 20:08:30 jim Exp $
#
# The cpp used on this machine replaces all newlines and multiple tabs and
# spaces in a macro expansion with a single space.  Imake tries to compensate
# for this, but is not always successful.
#

###########################################################################
# X Window System Makefile generated from template file Imake.tmpl
# $XConsortium: Imake.tmpl,v 1.91 88/10/23 22:37:10 jim Exp $
#
# Do not change the body of the imake template file.  Server-specific
# parameters may be set in the appropriate .macros file; site-specific
# parameters (but shared by all servers) may be set in site.def.  If you
# make any changes, you'll need to rebuild the makefiles using
# "make World" (at best) or "make Makefile; make Makefiles" (at least) in
# the top level directory.
#
# If your C preprocessor doesn't define any unique symbols, you'll need
# to set BOOTSTRAPCFLAGS when rebuilding imake (usually when doing
# "make Makefile", "make Makefiles", or "make World").
#
# If you absolutely can't get imake to work, you'll need to set the
# variables at the top of each Makefile as well as the dependencies at the
# bottom (makedepend will do this automatically).
#

###########################################################################
# platform-specific configuration parameters - edit Sun.macros to change

# platform:  $XConsortium: Sun.macros,v 1.52 88/10/23 11:00:55 jim Exp $
# operating system:  SunOS 4.0

BOOTSTRAPCFLAGS =
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
RANLIBINSTFLAGS = -t
             AR = ar clq
             LS = ls
       LINTOPTS = -axz
    LINTLIBFLAG = -C
           MAKE = make
STD_CPP_DEFINES =
    STD_DEFINES =

###########################################################################
# site-specific configuration parameters - edit site.def to change

# site:  $XConsortium: site.def,v 1.16 88/10/12 10:30:24 jim Exp $

###########################################################################
# definitions common to all Makefiles - do not edit

          SHELL = /bin/sh

        DESTDIR =
      USRLIBDIR = $(DESTDIR)/usr/lib
         BINDIR = $(DESTDIR)/usr/bin/X11
         INCDIR = $(DESTDIR)/usr/include/X11
         ADMDIR = $(DESTDIR)/usr/adm
         LIBDIR = $(USRLIBDIR)/X11
     LINTLIBDIR = $(USRLIBDIR)/lint
        FONTDIR = $(LIBDIR)/fonts
       XINITDIR = $(LIBDIR)/xinit
         XDMDIR = $(LIBDIR)/xdm
         UWMDIR = $(LIBDIR)/uwm
         AWMDIR = $(LIBDIR)/awm
         TWMDIR = $(LIBDIR)/twm
        MANPATH = $(DESTDIR)/usr/man
  MANSOURCEPATH = $(MANPATH)/man
         MANDIR = $(MANSOURCEPATH)n
      LIBMANDIR = $(MANSOURCEPATH)3
    XAPPLOADDIR = $(LIBDIR)/app-defaults

   INSTBINFLAGS = -m 0755
   INSTUIDFLAGS = -m 4755
   INSTLIBFLAGS = -m 0664
   INSTINCFLAGS = -m 0444
   INSTMANFLAGS = -m 0444
   INSTAPPFLAGS = -m 0444
  INSTKMEMFLAGS = -m 4755
        FCFLAGS = -t
    CDEBUGFLAGS = -O

        PATHSEP = /
         DEPEND = $(DEPENDSRC)/makedepend
          IMAKE = $(IMAKESRC)/imake
            RGB = $(RGBSRC)/rgb
             FC = $(BDFTOSNFSRC)/bdftosnf
      MKFONTDIR = $(MKFONTDIRSRC)/mkfontdir
      MKDIRHIER = $(SCRIPTSSRC)/mkdirhier.sh

         CFLAGS = $(CDEBUGFLAGS) $(INCLUDES) $(STD_DEFINES) $(DEFINES)
      LINTFLAGS = $(LINTOPTS) $(INCLUDES) $(STD_DEFINES) $(DEFINES) -DLINT
        LDFLAGS = $(CDEBUGFLAGS) $(SYS_LIBRARIES) $(SYSAUX_LIBRARIES)
            TOP = ../../../../.
      CLIENTSRC = $(TOP)/clients
        DEMOSRC = $(TOP)/demos
         LIBSRC = $(TOP)/lib
        FONTSRC = $(TOP)/fonts
     INCLUDESRC = $(TOP)/X11
      SERVERSRC = $(TOP)/server
        UTILSRC = $(TOP)/util
     SCRIPTSSRC = $(UTILSRC)/scripts
     EXAMPLESRC = $(TOP)/examples
     CONTRIBSRC = $(TOP)/contrib
         DOCSRC = $(TOP)/doc
         RGBSRC = $(TOP)/rgb
      DEPENDSRC = $(UTILSRC)/makedepend
       IMAKESRC = $(UTILSRC)/imake
       IRULESRC = $(UTILSRC)/imake.includes
        XLIBSRC = $(LIBSRC)/X
         XMUSRC = $(LIBSRC)/Xmu
     TOOLKITSRC = $(LIBSRC)/Xt
     AWIDGETSRC = $(LIBSRC)/Xaw
     OLDXLIBSRC = $(LIBSRC)/oldX
    BDFTOSNFSRC = $(FONTSRC)/bdftosnf
   MKFONTDIRSRC = $(FONTSRC)/mkfontdir
   EXTENSIONSRC = $(TOP)/extensions
   EXTENSIONLIB = $(EXTENSIONSRC)/lib/libXext.a
           XLIB = $(XLIBSRC)/libX11.a
         XMULIB = $(XMUSRC)/libXmu.a
        OLDXLIB = $(OLDXLIBSRC)/liboldX.a
       XTOOLLIB = $(TOOLKITSRC)/libXt.a
         XAWLIB = $(AWIDGETSRC)/libXaw.a
       LINTXLIB = $(XLIBSRC)/llib-lX11.ln
        LINTXMU = $(XMUSRC)/llib-lXmu.ln
      LINTXTOOL = $(TOOLKITSRC)/llib-lXt.ln
        LINTXAW = $(AWIDGETSRC)/llib-lXaw.ln
       INCLUDES = -I$(TOP)
      MACROFILE = Sun.macros
   ICONFIGFILES = $(IRULESRC)/Imake.tmpl \
			$(IRULESRC)/$(MACROFILE) $(IRULESRC)/site.def
  IMAKE_DEFINES =
      IMAKE_CMD = $(NEWTOP)$(IMAKE) -TImake.tmpl -I$(NEWTOP)$(IRULESRC) \
			-s Makefile $(IMAKE_DEFINES)
         RM_CMD = $(RM) *.CKP *.ln *.BAK *.bak *.o core errs ,* *~ *.a \
			.emacs_* tags TAGS make.log MakeOut

###########################################################################
# rules:  $XConsortium: Imake.rules,v 1.71 88/10/23 22:46:34 jim Exp $

###########################################################################
# start of Imakefile

###########################################################################
# definitions common to all Picture Widget Makefiles  - do not  edit

          PWTOP = $(CONTRIBSRC)/widgets/picture
   PWINCLUDESRC = $(PWTOP)
       PWLIBSRC = $(PWTOP)/lib

          PWLIB = $(PWLIBSRC)/libPicture.a
        MENULIB = $(CONTRIBSRC)/widgets/MenuBox/libMenu.a

###########################################################################
# start of PW's Imakefile

     PWDEFINES  = $(SYSTEM) -DOMITARC -DINCDIR="\"$(INCDIR)\""

       INCLUDES = -I$(PWINCLUDESRC) -I$(TOP)
       DEFINES = $(PWDEFINES)
       LINTLIBS =

SRCS=	StPicAtr.c StPicDraw.c StPicIo.c StPicMisc.c StPicMod.c \
	StPicObj.c StPicPf.c StPicPopup.c StPicSel.c StPicUi.c \
	StPicUndo.c StPicUtl.c StPicture.c

OBJS=	StPicAtr.o StPicDraw.o StPicIo.o StPicMisc.o StPicMod.o \
	StPicObj.o StPicPf.o StPicPopup.o StPicSel.o StPicUi.o \
	StPicUndo.o StPicUtl.o StPicture.o

.c.o:
	$(RM) $@
	$(CC) -c $(CFLAGS) $*.c

all:: libPicture.a

libPicture.a: $(OBJS)
	$(RM) $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

lintlib:: llib-lPicture.ln

llib-lPicture.ln: $(SRCS)
	$(RM) $@
	$(LINT) $(LINTLIBFLAG)Picture $(LINTFLAGS) $(SRCS)

install:: libPicture.a
	$(INSTALL) -c $(INSTLIBFLAGS) libPicture.a $(USRLIBDIR)
	$(RANLIB) $(RANLIBINSTFLAGS) $(USRLIBDIR)/libPicture.a

install.ln:: llib-lPicture.ln
	$(INSTALL) -c $(INSTLIBFLAGS) llib-lPicture.ln $(LINTLIBDIR)

depend:: $(DEPEND)

depend::
	$(DEPEND) -s "# DO NOT DELETE" -- $(CFLAGS) -- $(SRCS)

$(DEPEND):
	@echo "making $@"; \
	cd $(DEPENDSRC); $(MAKE)

lint:
	$(LINT) $(LINTFLAGS) $(SRCS) $(LINTLIBS)
lint1:
	$(LINT) $(LINTFLAGS) $(FILE) $(LINTLIBS)

###########################################################################
# Imake.tmpl common rules for all Makefiles - do not edit

emptyrule::

clean::
	$(RM_CMD) \#*

Makefile:: $(IMAKE)

Makefile:: Imakefile \
	$(IRULESRC)/Imake.tmpl \
	$(IRULESRC)/Imake.rules \
	$(IRULESRC)/site.def \
	$(IRULESRC)/$(MACROFILE)
	-@if [ -f Makefile ]; then \
	echo "$(RM) Makefile.bak; $(MV) Makefile Makefile.bak"; \
	$(RM) Makefile.bak; $(MV) Makefile Makefile.bak; \
	else exit 0; fi
	$(IMAKE_CMD) -DTOPDIR=$(TOP)

$(IMAKE):
	@echo "making $@"; \
	cd $(IMAKESRC); $(MAKE) BOOTSTRAPCFLAGS=$(BOOTSTRAPCFLAGS)

tags::
	$(TAGS) -w *.[ch]
	$(TAGS) -xw *.[ch] > TAGS

###########################################################################
# empty rules for directories that do not have SUBDIRS - do not edit

install::
	@echo "install done"

install.man::
	@echo "install.man done"

Makefiles::

###########################################################################
# dependencies generated by makedepend

# DO NOT DELETE

StPicAtr.o: /usr/include/stdio.h /usr/include/ctype.h
StPicAtr.o: ../../../.././X11/Xlib.h /usr/include/sys/types.h
StPicAtr.o: /usr/include/sys/sysmacros.h ../../../.././X11/X.h
StPicAtr.o: ../../../.././X11/Intrinsic.h ../../../.././X11/Xutil.h
StPicAtr.o: ../../../.././X11/Xresource.h ../../../.././X11/Xos.h
StPicAtr.o: /usr/include/strings.h /usr/include/sys/file.h
StPicAtr.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
StPicAtr.o: /usr/include/sys/time.h ../../../.././X11/Core.h
StPicAtr.o: ../../../.././X11/Composite.h ../../../.././X11/Constraint.h
StPicAtr.o: ../../../.././X11/StringDefs.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicAtr.o: ../../../.././X11/IntrinsicP.h ../../../.././X11/Intrinsic.h
StPicAtr.o: ../../../.././X11/CoreP.h ../../../.././X11/CompositeP.h
StPicAtr.o: ../../../.././X11/ConstrainP.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicAtr.o: ../../../.././X11/Simple.h ../../../.././X11/copyright.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicAtr.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicAtr.o: ../../../.././contrib/widgets/picture/St/StPicPopup.h
StPicDraw.o: /usr/include/stdio.h /usr/include/math.h
StPicDraw.o: /usr/include/floatingpoint.h /usr/include/sys/ieeefp.h
StPicDraw.o: ../../../.././X11/Intrinsic.h ../../../.././X11/Xlib.h
StPicDraw.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
StPicDraw.o: ../../../.././X11/X.h ../../../.././X11/Xutil.h
StPicDraw.o: ../../../.././X11/Xresource.h ../../../.././X11/Xos.h
StPicDraw.o: /usr/include/strings.h /usr/include/sys/file.h
StPicDraw.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
StPicDraw.o: /usr/include/sys/time.h ../../../.././X11/Core.h
StPicDraw.o: ../../../.././X11/Composite.h ../../../.././X11/Constraint.h
StPicDraw.o: ../../../.././X11/StringDefs.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicDraw.o: ../../../.././X11/IntrinsicP.h ../../../.././X11/Intrinsic.h
StPicDraw.o: ../../../.././X11/CoreP.h ../../../.././X11/CompositeP.h
StPicDraw.o: ../../../.././X11/ConstrainP.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicDraw.o: ../../../.././X11/Simple.h ../../../.././X11/copyright.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicDraw.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicDraw.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicIo.o: /usr/include/stdio.h ../../../.././X11/Xlib.h
StPicIo.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
StPicIo.o: ../../../.././X11/X.h ../../../.././X11/Intrinsic.h
StPicIo.o: ../../../.././X11/Xutil.h ../../../.././X11/Xresource.h
StPicIo.o: ../../../.././X11/Xos.h /usr/include/strings.h
StPicIo.o: /usr/include/sys/file.h /usr/include/sys/fcntlcom.h
StPicIo.o: /usr/include/sys/time.h /usr/include/sys/time.h
StPicIo.o: ../../../.././X11/Core.h ../../../.././X11/Composite.h
StPicIo.o: ../../../.././X11/Constraint.h ../../../.././X11/StringDefs.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicIo.o: ../../../.././X11/IntrinsicP.h ../../../.././X11/Intrinsic.h
StPicIo.o: ../../../.././X11/CoreP.h ../../../.././X11/CompositeP.h
StPicIo.o: ../../../.././X11/ConstrainP.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicIo.o: ../../../.././X11/Simple.h ../../../.././X11/copyright.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicIo.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicIo.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicMisc.o: /usr/include/stdio.h ../../../.././X11/Xlib.h
StPicMisc.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
StPicMisc.o: ../../../.././X11/X.h ../../../.././X11/Intrinsic.h
StPicMisc.o: ../../../.././X11/Xutil.h ../../../.././X11/Xresource.h
StPicMisc.o: ../../../.././X11/Xos.h /usr/include/strings.h
StPicMisc.o: /usr/include/sys/file.h /usr/include/sys/fcntlcom.h
StPicMisc.o: /usr/include/sys/time.h /usr/include/sys/time.h
StPicMisc.o: ../../../.././X11/Core.h ../../../.././X11/Composite.h
StPicMisc.o: ../../../.././X11/Constraint.h ../../../.././X11/StringDefs.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicMisc.o: ../../../.././X11/IntrinsicP.h ../../../.././X11/Intrinsic.h
StPicMisc.o: ../../../.././X11/CoreP.h ../../../.././X11/CompositeP.h
StPicMisc.o: ../../../.././X11/ConstrainP.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicMisc.o: ../../../.././X11/Simple.h ../../../.././X11/copyright.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicMisc.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicMisc.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicMod.o: ../../../.././X11/Intrinsic.h ../../../.././X11/Xlib.h
StPicMod.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
StPicMod.o: ../../../.././X11/X.h ../../../.././X11/Xutil.h
StPicMod.o: ../../../.././X11/Xresource.h ../../../.././X11/Xos.h
StPicMod.o: /usr/include/strings.h /usr/include/sys/file.h
StPicMod.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
StPicMod.o: /usr/include/sys/time.h ../../../.././X11/Core.h
StPicMod.o: ../../../.././X11/Composite.h ../../../.././X11/Constraint.h
StPicMod.o: ../../../.././X11/StringDefs.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicMod.o: ../../../.././X11/IntrinsicP.h ../../../.././X11/Intrinsic.h
StPicMod.o: ../../../.././X11/CoreP.h ../../../.././X11/CompositeP.h
StPicMod.o: ../../../.././X11/ConstrainP.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicMod.o: ../../../.././X11/Simple.h ../../../.././X11/copyright.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicMod.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicMod.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicObj.o: ../../../.././X11/Intrinsic.h ../../../.././X11/Xlib.h
StPicObj.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
StPicObj.o: ../../../.././X11/X.h ../../../.././X11/Xutil.h
StPicObj.o: ../../../.././X11/Xresource.h ../../../.././X11/Xos.h
StPicObj.o: /usr/include/strings.h /usr/include/sys/file.h
StPicObj.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
StPicObj.o: /usr/include/sys/time.h ../../../.././X11/Core.h
StPicObj.o: ../../../.././X11/Composite.h ../../../.././X11/Constraint.h
StPicObj.o: ../../../.././X11/StringDefs.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicObj.o: ../../../.././X11/IntrinsicP.h ../../../.././X11/Intrinsic.h
StPicObj.o: ../../../.././X11/CoreP.h ../../../.././X11/CompositeP.h
StPicObj.o: ../../../.././X11/ConstrainP.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicObj.o: ../../../.././X11/Simple.h ../../../.././X11/copyright.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicObj.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicObj.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicPf.o: /usr/include/stdio.h ../../../.././X11/Xlib.h
StPicPf.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
StPicPf.o: ../../../.././X11/X.h ../../../.././X11/Intrinsic.h
StPicPf.o: ../../../.././X11/Xutil.h ../../../.././X11/Xresource.h
StPicPf.o: ../../../.././X11/Xos.h /usr/include/strings.h
StPicPf.o: /usr/include/sys/file.h /usr/include/sys/fcntlcom.h
StPicPf.o: /usr/include/sys/time.h /usr/include/sys/time.h
StPicPf.o: ../../../.././X11/Core.h ../../../.././X11/Composite.h
StPicPf.o: ../../../.././X11/Constraint.h ../../../.././X11/StringDefs.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicPf.o: ../../../.././X11/IntrinsicP.h ../../../.././X11/Intrinsic.h
StPicPf.o: ../../../.././X11/CoreP.h ../../../.././X11/CompositeP.h
StPicPf.o: ../../../.././X11/ConstrainP.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicPf.o: ../../../.././X11/Simple.h ../../../.././X11/copyright.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicPf.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicPf.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicPopup.o: ../../../.././X11/IntrinsicP.h ../../../.././X11/Intrinsic.h
StPicPopup.o: ../../../.././X11/CoreP.h ../../../.././X11/CompositeP.h
StPicPopup.o: ../../../.././X11/ConstrainP.h ../../../.././X11/StringDefs.h
StPicPopup.o: ../../../.././X11/Cardinals.h ../../../.././X11/Shell.h
StPicPopup.o: ../../../.././X11/Scroll.h ../../../.././X11/Xmu.h
StPicPopup.o: ../../../.././X11/Intrinsic.h ../../../.././X11/Xlib.h
StPicPopup.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
StPicPopup.o: ../../../.././X11/X.h ../../../.././X11/Xutil.h
StPicPopup.o: ../../../.././X11/Xresource.h ../../../.././X11/Xos.h
StPicPopup.o: /usr/include/strings.h /usr/include/sys/file.h
StPicPopup.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
StPicPopup.o: /usr/include/sys/time.h ../../../.././X11/Core.h
StPicPopup.o: ../../../.././X11/Composite.h ../../../.././X11/Constraint.h
StPicPopup.o: ../../../.././X11/Label.h ../../../.././X11/Simple.h
StPicPopup.o: ../../../.././X11/copyright.h ../../../.././X11/Command.h
StPicPopup.o: ../../../.././X11/MenuBox.h ../../../.././X11/Dialog.h
StPicPopup.o: ../../../.././X11/Form.h ../../../.././X11/Constraint.h
StPicPopup.o: ../../../.././X11/List.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicPopup.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicPopup.o: ../../../.././contrib/widgets/picture/St/StPicPopup.h
StPicSel.o: ../../../.././X11/Xlib.h /usr/include/sys/types.h
StPicSel.o: /usr/include/sys/sysmacros.h ../../../.././X11/X.h
StPicSel.o: ../../../.././X11/Intrinsic.h ../../../.././X11/Xutil.h
StPicSel.o: ../../../.././X11/Xresource.h ../../../.././X11/Xos.h
StPicSel.o: /usr/include/strings.h /usr/include/sys/file.h
StPicSel.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
StPicSel.o: /usr/include/sys/time.h ../../../.././X11/Core.h
StPicSel.o: ../../../.././X11/Composite.h ../../../.././X11/Constraint.h
StPicSel.o: ../../../.././X11/StringDefs.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicSel.o: ../../../.././X11/IntrinsicP.h ../../../.././X11/Intrinsic.h
StPicSel.o: ../../../.././X11/CoreP.h ../../../.././X11/CompositeP.h
StPicSel.o: ../../../.././X11/ConstrainP.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicSel.o: ../../../.././X11/Simple.h ../../../.././X11/copyright.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicSel.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicSel.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicUi.o: ../../../.././X11/Xlib.h /usr/include/sys/types.h
StPicUi.o: /usr/include/sys/sysmacros.h ../../../.././X11/X.h
StPicUi.o: ../../../.././X11/Intrinsic.h ../../../.././X11/Xutil.h
StPicUi.o: ../../../.././X11/Xresource.h ../../../.././X11/Xos.h
StPicUi.o: /usr/include/strings.h /usr/include/sys/file.h
StPicUi.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
StPicUi.o: /usr/include/sys/time.h ../../../.././X11/Core.h
StPicUi.o: ../../../.././X11/Composite.h ../../../.././X11/Constraint.h
StPicUi.o: ../../../.././X11/StringDefs.h ../../../.././X11/cursorfont.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicUi.o: ../../../.././X11/IntrinsicP.h ../../../.././X11/Intrinsic.h
StPicUi.o: ../../../.././X11/CoreP.h ../../../.././X11/CompositeP.h
StPicUi.o: ../../../.././X11/ConstrainP.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicUi.o: ../../../.././X11/Simple.h ../../../.././X11/copyright.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicUi.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicUi.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicUi.o: /usr/include/math.h /usr/include/floatingpoint.h
StPicUi.o: /usr/include/sys/ieeefp.h
StPicUndo.o: ../../../.././X11/Intrinsic.h ../../../.././X11/Xlib.h
StPicUndo.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
StPicUndo.o: ../../../.././X11/X.h ../../../.././X11/Xutil.h
StPicUndo.o: ../../../.././X11/Xresource.h ../../../.././X11/Xos.h
StPicUndo.o: /usr/include/strings.h /usr/include/sys/file.h
StPicUndo.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
StPicUndo.o: /usr/include/sys/time.h ../../../.././X11/Core.h
StPicUndo.o: ../../../.././X11/Composite.h ../../../.././X11/Constraint.h
StPicUndo.o: ../../../.././X11/StringDefs.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicUndo.o: ../../../.././X11/IntrinsicP.h ../../../.././X11/Intrinsic.h
StPicUndo.o: ../../../.././X11/CoreP.h ../../../.././X11/CompositeP.h
StPicUndo.o: ../../../.././X11/ConstrainP.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicUndo.o: ../../../.././X11/Simple.h ../../../.././X11/copyright.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicUndo.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicUndo.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicUtl.o: ../../../.././X11/Intrinsic.h ../../../.././X11/Xlib.h
StPicUtl.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
StPicUtl.o: ../../../.././X11/X.h ../../../.././X11/Xutil.h
StPicUtl.o: ../../../.././X11/Xresource.h ../../../.././X11/Xos.h
StPicUtl.o: /usr/include/strings.h /usr/include/sys/file.h
StPicUtl.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
StPicUtl.o: /usr/include/sys/time.h ../../../.././X11/Core.h
StPicUtl.o: ../../../.././X11/Composite.h ../../../.././X11/Constraint.h
StPicUtl.o: ../../../.././X11/StringDefs.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicUtl.o: ../../../.././X11/IntrinsicP.h ../../../.././X11/Intrinsic.h
StPicUtl.o: ../../../.././X11/CoreP.h ../../../.././X11/CompositeP.h
StPicUtl.o: ../../../.././X11/ConstrainP.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicUtl.o: ../../../.././X11/Simple.h ../../../.././X11/copyright.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicUtl.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicUtl.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicture.o: /usr/include/ctype.h ../../../.././X11/IntrinsicP.h
StPicture.o: ../../../.././X11/Intrinsic.h ../../../.././X11/CoreP.h
StPicture.o: ../../../.././X11/CompositeP.h ../../../.././X11/ConstrainP.h
StPicture.o: ../../../.././X11/StringDefs.h ../../../.././X11/Cardinals.h
StPicture.o: ../../../.././X11/Scroll.h ../../../.././X11/Xmu.h
StPicture.o: ../../../.././X11/Intrinsic.h ../../../.././X11/Xlib.h
StPicture.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
StPicture.o: ../../../.././X11/X.h ../../../.././X11/Xutil.h
StPicture.o: ../../../.././X11/Xresource.h ../../../.././X11/Xos.h
StPicture.o: /usr/include/strings.h /usr/include/sys/file.h
StPicture.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
StPicture.o: /usr/include/sys/time.h ../../../.././X11/Core.h
StPicture.o: ../../../.././X11/Composite.h ../../../.././X11/Constraint.h
StPicture.o: ../../../.././X11/Dialog.h ../../../.././X11/Form.h
StPicture.o: ../../../.././X11/Constraint.h ../../../.././X11/Logo.h
StPicture.o: ../../../.././X11/bitmaps/black ../../../.././X11/bitmaps/boxes
StPicture.o: ../../../.././X11/bitmaps/cross_weave
StPicture.o: ../../../.././X11/bitmaps/dimple1
StPicture.o: ../../../.././X11/bitmaps/dimple3
StPicture.o: ../../../.././X11/bitmaps/flipped_gray
StPicture.o: ../../../.././X11/bitmaps/gray1 ../../../.././X11/bitmaps/gray3
StPicture.o: ../../../.././X11/bitmaps/light_gray
StPicture.o: ../../../.././X11/bitmaps/root_weave
StPicture.o: ../../../.././X11/bitmaps/scales
StPicture.o: ../../../.././X11/bitmaps/stipple
StPicture.o: ../../../.././X11/bitmaps/wide_weave
StPicture.o: ../../../.././contrib/widgets/picture/St/StPictureP.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPictureA.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPicture.h
StPicture.o: ../../../.././X11/Simple.h ../../../.././X11/copyright.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPictureD.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPicUi.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPicMod.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPicObj.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPicUtl.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPicPf.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPicAtr.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPicSel.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPicMisc.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPicIo.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPicUndo.h
StPicture.o: ../../../.././X11/SimpleP.h ../../../.././X11/CoreP.h
StPicture.o: ../../../.././contrib/widgets/picture/St/StPicPopup.h
