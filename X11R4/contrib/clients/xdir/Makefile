# Makefile generated by imake - do not edit!
# $XConsortium: imake.c,v 1.46 89/08/28 10:54:41 jim Exp $
#
# The cpp used on this machine replaces all newlines and multiple tabs and
# spaces in a macro expansion with a single space.  Imake tries to compensate
# for this, but is not always successful.
#

###########################################################################
# Makefile generated from "Imake.tmpl" and <Imakefile>
# $XConsortium: Imake.tmpl,v 1.49 89/08/24 10:40:13 jim Exp $
#
# Platform-specific parameters may be set in the appropriate .cf
# configuration files.  Site-wide parameters may be set in the file
# site.def.  Full rebuilds are recommended if any parameters are changed.
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
# platform-specific configuration parameters - edit sun.cf to change

# platform:  $XConsortium: sun.cf,v 1.25 89/08/22 18:02:07 jim Exp $
# operating system:  SunOS 4.0.1

###########################################################################
# site-specific configuration parameters - edit site.def to change

# site:  $XConsortium: site.def,v 1.9 89/08/24 14:06:40 jim Exp $

            SHELL = /bin/sh

              TOP = /x11
      CURRENT_DIR = ../clients/xdir

               AR = ar cq
  BOOTSTRAPCFLAGS =
               CC = cc
         COMPRESS = compress
              CPP = /lib/cpp $(STD_CPP_DEFINES)
    PREPROCESSCMD = cc -E $(STD_CPP_DEFINES)
          INSTALL = install
               LD = ld
             LINT = lint
      LINTLIBFLAG = -C
         LINTOPTS = -axz
               LN = ln -s
             MAKE = make
               MV = mv
               CP = cp
           RANLIB = ranlib
  RANLIBINSTFLAGS =
               RM = rm -f
     STD_INCLUDES =
  STD_CPP_DEFINES =
      STD_DEFINES =
 EXTRA_LOAD_FLAGS =
  EXTRA_LIBRARIES =
             TAGS = ctags

         SHLIBDEF = -DSUNSHLIB

     INSTBINFLAGS = -m 0755
     INSTUIDFLAGS = -m 4755
     INSTLIBFLAGS = -m 0664
     INSTINCFLAGS = -m 0444
     INSTMANFLAGS = -m 0444
     INSTDATFLAGS = -m 0444
    INSTKMEMFLAGS = -m 4755

          DESTDIR =

         INCLUDES = -I$(TOP)

      CDEBUGFLAGS = -O
        CCOPTIONS = -f68881 -pipe
      COMPATFLAGS =
       ALLDEFINES = $(STD_INCLUDES) $(INCLUDES) $(STD_DEFINES) $(DEFINES) $(COMPATFLAGS)
           CFLAGS = $(CDEBUGFLAGS) $(CCOPTIONS) $(ALLDEFINES)
        LINTFLAGS = $(LINTOPTS) -DLINT $(ALLDEFINES)
          LDFLAGS = $(CDEBUGFLAGS) $(CCOPTIONS) $(SYS_LIBRARIES) $(EXTRA_LIBRARIES)

        MACROFILE = sun.cf
           RM_CMD = $(RM) *.CKP *.ln *.BAK *.bak *.o core errs ,* *~ *.a \
			.emacs_* tags TAGS make.log MakeOut

    IMAKE_DEFINES =

         IRULESRC = $(CONFIGSRC)
        IMAKE_CMD = $(NEWTOP)$(IMAKE) -I$(NEWTOP)$(IRULESRC) $(IMAKE_DEFINES)

     ICONFIGFILES = $(IRULESRC)/Imake.tmpl $(IRULESRC)/Imake.rules \
			$(IRULESRC)/Project.tmpl $(IRULESRC)/site.def \
			$(IRULESRC)/$(MACROFILE)

###########################################################################
# X Window System Build Parameters
# $XConsortium: Project.tmpl,v 1.37 89/08/24 10:40:17 jim Exp $

###########################################################################
# X Window System make variables; this need to be coordinated with rules
# $XConsortium: Project.tmpl,v 1.37 89/08/24 10:40:17 jim Exp $

          PATHSEP = /
        USRLIBDIR = $(DESTDIR)/usr/lib
           BINDIR = $(DESTDIR)/usr/bin/X11
          INCROOT = $(DESTDIR)/usr/include
     BUILDINCROOT = $(TOP)
      BUILDINCDIR = $(BUILDINCROOT)/X11
      BUILDINCTOP = ..
           INCDIR = $(INCROOT)/X11
           ADMDIR = $(DESTDIR)/usr/adm
           LIBDIR = $(USRLIBDIR)/X11
        CONFIGDIR = $(LIBDIR)/config
       LINTLIBDIR = $(USRLIBDIR)/lint

          FONTDIR = $(LIBDIR)/fonts
         XINITDIR = $(LIBDIR)/xinit
           XDMDIR = $(LIBDIR)/xdm
           AWMDIR = $(LIBDIR)/awm
           TWMDIR = $(LIBDIR)/twm
          MANPATH = $(DESTDIR)/usr/man
    MANSOURCEPATH = $(MANPATH)/man
           MANDIR = $(MANSOURCEPATH)n
        LIBMANDIR = $(MANSOURCEPATH)3
      XAPPLOADDIR = $(LIBDIR)/app-defaults

        SOXLIBREV = 4.2
          SOXTREV = 4.0
         SOXAWREV = 4.0
        SOOLDXREV = 4.0
         SOXMUREV = 4.0
        SOXEXTREV = 4.0

          FCFLAGS = -t

     INSTAPPFLAGS = $(INSTDATFLAGS)

            IMAKE = $(IMAKESRC)/imake
           DEPEND = $(DEPENDSRC)/makedepend

              RGB = $(RGBSRC)/rgb
               FC = $(BDFTOSNFSRC)/bdftosnf
        MKFONTDIR = $(MKFONTDIRSRC)/mkfontdir
        MKDIRHIER = /bin/sh $(SCRIPTSRC)/mkdirhier.sh

        CONFIGSRC = $(TOP)/config
        CLIENTSRC = $(TOP)/clients
          DEMOSRC = $(TOP)/demos
           LIBSRC = $(TOP)/lib
          FONTSRC = $(TOP)/fonts
       INCLUDESRC = $(TOP)/X11
        SERVERSRC = $(TOP)/server
          UTILSRC = $(TOP)/util
        SCRIPTSRC = $(UTILSRC)/scripts
       EXAMPLESRC = $(TOP)/examples
       CONTRIBSRC = $(TOP)/contrib
           DOCSRC = $(TOP)/doc
           RGBSRC = $(TOP)/rgb
        DEPENDSRC = $(UTILSRC)/makedepend
         IMAKESRC = $(UTILSRC)/imake
         XAUTHSRC = $(LIBSRC)/Xau
          XLIBSRC = $(LIBSRC)/X
           XMUSRC = $(LIBSRC)/Xmu
       TOOLKITSRC = $(LIBSRC)/Xt
       AWIDGETSRC = $(LIBSRC)/Xaw
       OLDXLIBSRC = $(LIBSRC)/oldX
      BDFTOSNFSRC = $(FONTSRC)/bdftosnf
     MKFONTDIRSRC = $(FONTSRC)/mkfontdir
     EXTENSIONSRC = $(TOP)/extensions

          DEPXLIB = $(XLIBSRC)/libX11.so.$(SOXLIBREV)
             XLIB = -L$(XLIBSRC) -lX11

  DEPEXTENSIONLIB = $(EXTENSIONSRC)/lib/libXext.a
     EXTENSIONLIB = $(DEPEXTENSIONLIB)

      DEPXAUTHLIB = $(XAUTHSRC)/libXau.a
         XAUTHLIB = $(DEPXAUTHLIB)

        DEPXMULIB = $(XMUSRC)/libXmu.a
           XMULIB = $(DEPXMULIB)

       DEPOLDXLIB = $(OLDXLIBSRC)/liboldX.so.$(SOOLDXREV)
          OLDXLIB = -L$(OLDXLIBSRC) -loldX

      DEPXTOOLLIB = $(TOOLKITSRC)/libXt.a
         XTOOLLIB = $(DEPXTOOLLIB)

        DEPXAWLIB = $(AWIDGETSRC)/libXaw.a
           XAWLIB = $(DEPXAWLIB)

         LINTXLIB = $(XLIBSRC)/llib-lX11.ln
          LINTXMU = $(XMUSRC)/llib-lXmu.ln
        LINTXTOOL = $(TOOLKITSRC)/llib-lXt.ln
          LINTXAW = $(AWIDGETSRC)/llib-lXaw.ln

          DEPLIBS = $(DEPXAWLIB) $(DEPXMULIB) $(DEPXTOOLLIB) $(DEPEXTENSIONLIB) $(DEPXLIB)
         DEPLIBS1 = $(DEPLIBS)
         DEPLIBS2 = $(DEPLIBS)
         DEPLIBS3 = $(DEPLIBS)

###########################################################################
# Imake rules for building libraries, programs, scripts, and data files
# rules:  $XConsortium: Imake.rules,v 1.51 89/08/24 10:40:01 jim Exp $

###########################################################################
# start of Imakefile

       INCLUDES = -I$(TOP) -I$(AWIDGETSRC) -I$(TOOLKITSRC)
LOCAL_LIBRARIES = $(XAWLIB) $(XMULIB) $(XTOOLLIB) $(XLIB)
CDEBUGFLAGS = -g

           SRCS = xdir.c dir.c util.c display.c error.c \
		  resources.c help.c menus.c actions.c configure.c commands.c
           OBJS = xdir.o dir.o util.o display.o error.o \
		  resources.o help.o menus.o actions.o configure.o commands.o

 PROGRAM = xdir

all:: xdir

xdir: $(OBJS) $(DEPLIBS)
	$(RM) $@
	$(CC) -o $@ $(OBJS) $(LOCAL_LIBRARIES) $(LDFLAGS) $(EXTRA_LOAD_FLAGS)

relink::
	$(RM) $(PROGRAM)
	$(MAKE) $(MFLAGS) $(PROGRAM)

saber_xdir:
	#load $(ALLDEFINES) $(SRCS) $(LOCAL_LIBRARIES) $(SYS_LIBRARIES) $(EXTRA_LIBRARIES)

osaber_xdir:
	#load $(ALLDEFINES) $(OBJS) $(LOCAL_LIBRARIES) $(SYS_LIBRARIES) $(EXTRA_LIBRARIES)

install:: xdir
	$(INSTALL) -c $(INSTALLFLAGS) xdir $(BINDIR)

install.man:: xdir.man
	$(INSTALL) -c $(INSTMANFLAGS) xdir.man $(MANDIR)/xdir.n

depend:: $(DEPEND)

$(DEPEND):
	@echo "checking $@ over in $(DEPENDSRC) first..."; \
	cd $(DEPENDSRC); $(MAKE); \
	echo "okay, continuing in $(CURRENT_DIR)"

depend::
	$(DEPEND) -s "# DO NOT DELETE" -- $(ALLDEFINES) -- $(SRCS)

lint:
	$(LINT) $(LINTFLAGS) $(SRCS) $(LINTLIBS)
lint1:
	$(LINT) $(LINTFLAGS) $(FILE) $(LINTLIBS)

clean::
	$(RM) $(PROGRAM)

install:: xdir.help
	$(INSTALL) -c $(INSTALLFLAGS) xdir.help $(LIBDIR)

install:: xdir.about
	$(INSTALL) -c $(INSTALLFLAGS) xdir.about $(LIBDIR)

install:: xdir-view.help
	$(INSTALL) -c $(INSTALLFLAGS) xdir-view.help $(LIBDIR)

install:: Xdir.ad
	$(INSTALL) -c $(INSTAPPFLAGS) Xdir.ad $(XAPPLOADDIR)/Xdir

###########################################################################
# common rules for all Makefiles - do not edit

emptyrule::

clean::
	$(RM_CMD) \#*

Makefile:: $(IMAKE)

$(IMAKE):
	@(cd $(IMAKESRC); if [ -f Makefile ]; then \
	echo "checking $@ in $(IMAKESRC) first..."; $(MAKE) all; else \
	echo "bootstrapping $@ from Makefile.ini in $(IMAKESRC) first..."; \
	$(MAKE) -f Makefile.ini BOOTSTRAPCFLAGS=$(BOOTSTRAPCFLAGS); fi; \
	echo "okay, continuing in $(CURRENT_DIR)")

Makefile:: Imakefile $(ICONFIGFILES)
	-@if [ -f Makefile ]; then \
	echo "	$(RM) Makefile.bak; $(MV) Makefile Makefile.bak"; \
	$(RM) Makefile.bak; $(MV) Makefile Makefile.bak; \
	else exit 0; fi
	$(IMAKE_CMD) -DTOPDIR=$(TOP) -DCURDIR=$(CURRENT_DIR)

forceMakefile::
	-@if [ -f Makefile ]; then \
	echo "	$(RM) Makefile.bak; $(MV) Makefile Makefile.bak"; \
	$(RM) Makefile.bak; $(MV) Makefile Makefile.bak; \
	else exit 0; fi
	$(IMAKE_CMD) -DTOPDIR=$(TOP) -DCURDIR=$(CURRENT_DIR)

tags::
	$(TAGS) -w *.[ch]
	$(TAGS) -xw *.[ch] > TAGS

saber:
	#load $(ALLDEFINES) $(SRCS)

osaber:
	#load $(ALLDEFINES) $(OBJS)

###########################################################################
# empty rules for directories that do not have SUBDIRS - do not edit

install::
	@echo "install in $(CURRENT_DIR) done"

install.man::
	@echo "install.man in $(CURRENT_DIR) done"

Makefiles::

includes::

###########################################################################
# dependencies generated by makedepend

# DO NOT DELETE

xdir.o: /usr/include/stdio.h /usr/include/sys/param.h
xdir.o: /usr/include/machine/param.h /usr/include/sys/signal.h
xdir.o: /usr/include/vm/faultcode.h /x11/X11/Xos.h /usr/include/sys/types.h
xdir.o: /usr/include/sys/sysmacros.h /usr/include/strings.h
xdir.o: /usr/include/sys/file.h /usr/include/sys/fcntlcom.h
xdir.o: /usr/include/sys/time.h /usr/include/sys/time.h /x11/X11/X.h
xdir.o: /x11/X11/Xlib.h /x11/X11/Xatom.h /x11/X11/Intrinsic.h
xdir.o: /x11/X11/Xutil.h /x11/X11/Xresource.h /x11/X11/Core.h
xdir.o: /x11/X11/Composite.h /x11/X11/Constraint.h /x11/X11/StringDefs.h
xdir.o: /x11/X11/Xaw/Viewport.h /x11/X11/Xaw/Form.h /x11/X11/Constraint.h
xdir.o: /x11/X11/Xaw/Box.h /x11/X11/Xaw/Command.h /x11/X11/Xaw/Label.h
xdir.o: /x11/X11/Xaw/Simple.h /x11/X11/Xmu/Converters.h /x11/X11/Xaw/VPaned.h
xdir.o: /x11/X11/Xaw/Paned.h /x11/X11/Xaw/List.h xdir.h
dir.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
dir.o: /usr/include/sys/dir.h /x11/X11/Xos.h /usr/include/strings.h
dir.o: /usr/include/sys/file.h /usr/include/sys/fcntlcom.h
dir.o: /usr/include/sys/time.h /usr/include/sys/time.h /x11/X11/X.h
dir.o: /x11/X11/Xlib.h /x11/X11/Xatom.h /x11/X11/Intrinsic.h /x11/X11/Xutil.h
dir.o: /x11/X11/Xresource.h /x11/X11/Core.h /x11/X11/Composite.h
dir.o: /x11/X11/Constraint.h xdir.h
util.o: /usr/include/stdio.h /usr/include/string.h /usr/include/sys/types.h
util.o: /usr/include/sys/sysmacros.h /usr/include/sys/stat.h
util.o: /usr/include/sys/param.h /usr/include/machine/param.h
util.o: /usr/include/sys/signal.h /usr/include/vm/faultcode.h /x11/X11/Xos.h
util.o: /usr/include/strings.h /usr/include/sys/file.h
util.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
util.o: /usr/include/sys/time.h /x11/X11/X.h /x11/X11/Xlib.h /x11/X11/Xatom.h
util.o: /x11/X11/Intrinsic.h /x11/X11/Xutil.h /x11/X11/Xresource.h
util.o: /x11/X11/Core.h /x11/X11/Composite.h /x11/X11/Constraint.h
util.o: /x11/X11/StringDefs.h /x11/X11/Shell.h xdir.h
display.o: /usr/include/stdio.h /usr/include/sys/param.h
display.o: /usr/include/machine/param.h /usr/include/sys/signal.h
display.o: /usr/include/vm/faultcode.h /x11/X11/Xos.h
display.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
display.o: /usr/include/strings.h /usr/include/sys/file.h
display.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
display.o: /usr/include/sys/time.h /x11/X11/X.h /x11/X11/Xlib.h
display.o: /x11/X11/Xatom.h /x11/X11/Intrinsic.h /x11/X11/Xutil.h
display.o: /x11/X11/Xresource.h /x11/X11/Core.h /x11/X11/Composite.h
display.o: /x11/X11/Constraint.h /x11/X11/StringDefs.h /x11/X11/Shell.h
display.o: /x11/X11/Xaw/Box.h /x11/X11/Xaw/Text.h /x11/X11/Xaw/AsciiSrc.h
display.o: /x11/X11/Xaw/VPaned.h /x11/X11/Xaw/Paned.h /x11/X11/Constraint.h
display.o: /x11/X11/Xaw/Command.h /x11/X11/Xaw/Label.h /x11/X11/Xaw/Simple.h
display.o: /x11/X11/Xmu/Converters.h /x11/X11/Xaw/AsciiText.h xdir.h
error.o: /usr/include/stdio.h /usr/include/string.h /usr/include/sys/types.h
error.o: /usr/include/sys/sysmacros.h /usr/include/sys/stat.h
error.o: /usr/include/sys/param.h /usr/include/machine/param.h
error.o: /usr/include/sys/signal.h /usr/include/vm/faultcode.h /x11/X11/Xos.h
error.o: /usr/include/strings.h /usr/include/sys/file.h
error.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
error.o: /usr/include/sys/time.h /x11/X11/X.h /x11/X11/Xlib.h
error.o: /x11/X11/Xatom.h /x11/X11/Intrinsic.h /x11/X11/Xutil.h
error.o: /x11/X11/Xresource.h /x11/X11/Core.h /x11/X11/Composite.h
error.o: /x11/X11/Constraint.h /x11/X11/StringDefs.h /x11/X11/Shell.h
error.o: /x11/X11/Xaw/Dialog.h /x11/X11/Xaw/Form.h /x11/X11/Constraint.h
error.o: /x11/X11/Xaw/Command.h /x11/X11/Xaw/Label.h /x11/X11/Xaw/Simple.h
error.o: /x11/X11/Xmu/Converters.h xdir.h
resources.o: /x11/X11/Xos.h /usr/include/sys/types.h
resources.o: /usr/include/sys/sysmacros.h /usr/include/strings.h
resources.o: /usr/include/sys/file.h /usr/include/sys/fcntlcom.h
resources.o: /usr/include/sys/time.h /usr/include/sys/time.h /x11/X11/X.h
resources.o: /x11/X11/Xlib.h /x11/X11/Xatom.h /x11/X11/Intrinsic.h
resources.o: /x11/X11/Xutil.h /x11/X11/Xresource.h /x11/X11/Core.h
resources.o: /x11/X11/Composite.h /x11/X11/Constraint.h /x11/X11/StringDefs.h
resources.o: xdir.h
help.o: /usr/include/stdio.h /usr/include/sys/param.h
help.o: /usr/include/machine/param.h /usr/include/sys/signal.h
help.o: /usr/include/vm/faultcode.h /x11/X11/Xos.h /usr/include/sys/types.h
help.o: /usr/include/sys/sysmacros.h /usr/include/strings.h
help.o: /usr/include/sys/file.h /usr/include/sys/fcntlcom.h
help.o: /usr/include/sys/time.h /usr/include/sys/time.h /x11/X11/X.h
help.o: /x11/X11/Xlib.h /x11/X11/Xatom.h /x11/X11/Intrinsic.h
help.o: /x11/X11/Xutil.h /x11/X11/Xresource.h /x11/X11/Core.h
help.o: /x11/X11/Composite.h /x11/X11/Constraint.h /x11/X11/StringDefs.h
help.o: /x11/X11/Shell.h /x11/X11/Xaw/Box.h /x11/X11/Xaw/Text.h
help.o: /x11/X11/Xaw/AsciiSrc.h /x11/X11/Xaw/VPaned.h /x11/X11/Xaw/Paned.h
help.o: /x11/X11/Constraint.h /x11/X11/Xaw/Command.h /x11/X11/Xaw/Label.h
help.o: /x11/X11/Xaw/Simple.h /x11/X11/Xmu/Converters.h
help.o: /x11/X11/Xaw/AsciiText.h xdir.h
menus.o: /usr/include/stdio.h /x11/X11/Xos.h /usr/include/sys/types.h
menus.o: /usr/include/sys/sysmacros.h /usr/include/strings.h
menus.o: /usr/include/sys/file.h /usr/include/sys/fcntlcom.h
menus.o: /usr/include/sys/time.h /usr/include/sys/time.h /x11/X11/X.h
menus.o: /x11/X11/Xlib.h /x11/X11/Xatom.h /x11/X11/Intrinsic.h
menus.o: /x11/X11/Xutil.h /x11/X11/Xresource.h /x11/X11/Core.h
menus.o: /x11/X11/Composite.h /x11/X11/Constraint.h /x11/X11/StringDefs.h
menus.o: /x11/X11/Xaw/Box.h xdir.h
actions.o: /usr/include/stdio.h /usr/include/sys/param.h
actions.o: /usr/include/machine/param.h /usr/include/sys/signal.h
actions.o: /usr/include/vm/faultcode.h /x11/X11/Xos.h
actions.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
actions.o: /usr/include/strings.h /usr/include/sys/file.h
actions.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
actions.o: /usr/include/sys/time.h /x11/X11/X.h /x11/X11/Xlib.h
actions.o: /x11/X11/Xatom.h /x11/X11/Intrinsic.h /x11/X11/Xutil.h
actions.o: /x11/X11/Xresource.h /x11/X11/Core.h /x11/X11/Composite.h
actions.o: /x11/X11/Constraint.h /x11/X11/StringDefs.h
actions.o: /x11/X11/Xaw/SimpleMenu.h /x11/X11/Shell.h
actions.o: /x11/X11/Xmu/Converters.h /x11/X11/Xaw/MenuButton.h
actions.o: /x11/X11/Xaw/Command.h /x11/X11/Xaw/Label.h /x11/X11/Xaw/Simple.h
actions.o: xdir.h
configure.o: /usr/include/stdio.h /x11/X11/Xos.h /usr/include/sys/types.h
configure.o: /usr/include/sys/sysmacros.h /usr/include/strings.h
configure.o: /usr/include/sys/file.h /usr/include/sys/fcntlcom.h
configure.o: /usr/include/sys/time.h /usr/include/sys/time.h /x11/X11/X.h
configure.o: /x11/X11/Xlib.h /x11/X11/Xatom.h /x11/X11/Intrinsic.h
configure.o: /x11/X11/Xutil.h /x11/X11/Xresource.h /x11/X11/Core.h
configure.o: /x11/X11/Composite.h /x11/X11/Constraint.h /x11/X11/StringDefs.h
configure.o: /x11/X11/Xaw/MenuButton.h /x11/X11/Xaw/Command.h
configure.o: /x11/X11/Xaw/Label.h /x11/X11/Xaw/Simple.h
configure.o: /x11/X11/Xmu/Converters.h /x11/X11/Xaw/SimpleMenu.h
configure.o: /x11/X11/Shell.h xdir.h
commands.o: /usr/include/stdio.h /usr/include/sys/param.h
commands.o: /usr/include/machine/param.h /usr/include/sys/signal.h
commands.o: /usr/include/vm/faultcode.h /x11/X11/Xos.h
commands.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
commands.o: /usr/include/strings.h /usr/include/sys/file.h
commands.o: /usr/include/sys/fcntlcom.h /usr/include/sys/time.h
commands.o: /usr/include/sys/time.h /x11/X11/X.h /x11/X11/Xlib.h
commands.o: /x11/X11/Xatom.h /x11/X11/Intrinsic.h /x11/X11/Xutil.h
commands.o: /x11/X11/Xresource.h /x11/X11/Core.h /x11/X11/Composite.h
commands.o: /x11/X11/Constraint.h /x11/X11/StringDefs.h
commands.o: /x11/X11/Xaw/SimpleMenu.h /x11/X11/Shell.h
commands.o: /x11/X11/Xmu/Converters.h /x11/X11/Xaw/MenuButton.h
commands.o: /x11/X11/Xaw/Command.h /x11/X11/Xaw/Label.h /x11/X11/Xaw/Simple.h
commands.o: xdir.h
