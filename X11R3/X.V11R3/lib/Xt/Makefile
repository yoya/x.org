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
# operating system:   SunOS 3.4

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

          SHELL =  /bin/sh

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
            TOP = ../../.
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

    STD_DEFINES =  $(ALIGN_DEFINES)
    APP_DEFINES = -DXAPPLOADDIR=\"$(XAPPLOADDIR)$(PATHSEP)\"
     DB_DEFINES = -DERRORDB=\"$(LIBDIR)/XtErrorDB\"
    CDEBUGFLAGS = -O
       INCLUDES = -I. -I$(XLIBSRC) -I$(TOP) -I$(TOP)/X11
   INSTALLFLAGS = $(INSTINCFLAGS)
       LINTLIBS = $(LINTXLIB)

HEADERS = \
	Composite.h \
	CompositeP.h \
	Constraint.h \
	ConstrainP.h \
	Core.h \
	CoreP.h \
	Intrinsic.h \
	IntrinsicP.h \
	Quarks.h \
	Selection.h \
	Shell.h \
	ShellP.h \
	StringDefs.h \
	Vendor.h \
	VendorP.h

SRCS = \
	Alloc.c \
	ArgList.c \
	Callback.c \
	CompObj.c \
	Composite.c \
	Constraint.c \
	Convert.c \
	Converters.c \
	Core.c \
	Create.c \
	Destroy.c \
	Display.c \
	Error.c \
	Event.c \
	GCManager.c \
	Geometry.c \
	Initialize.c \
	Intrinsic.c \
	Manage.c \
	NextEvent.c \
	Object.c \
	Popup.c \
	RectObj.c \
	Resources.c \
	Selection.c \
	Shell.c \
	TMparse.c \
	TMstate.c \
	Vendor.c \
	WindowObj.c

OBJS = \
	Alloc.o \
	ArgList.o \
	Callback.o \
	CompObj.o \
	Composite.o \
	Constraint.o \
	Convert.o \
	Converters.o \
	Core.o \
	Create.o \
	Destroy.o \
	Display.o \
	Error.o \
	Event.o \
	GCManager.o \
	Geometry.o \
	Initialize.o \
	Intrinsic.o \
	Manage.o \
	NextEvent.o \
	Object.o \
	Popup.o \
	RectObj.o \
	Resources.o \
	Selection.o \
	Shell.o \
	TMparse.o \
	TMstate.o \
	Vendor.o \
	WindowObj.o

all::

.c.o:
	$(RM) $@
	$(CC) -c $(CFLAGS) $*.c

Initialize.o: $(ICONFIGFILES)
	$(RM) $@
	$(CC) -c $(CFLAGS) $(APP_DEFINES) $*.c

Error.o: $(ICONFIGFILES)
	$(RM) $@
	$(CC) -c $(CFLAGS) $(DB_DEFINES) $*.c

all:: libXt.a

libXt.a: $(OBJS)
	$(RM) $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

lintlib:: llib-lXt.ln

llib-lXt.ln: $(SRCS)
	$(RM) $@
	$(LINT) $(LINTLIBFLAG)Xt $(LINTFLAGS) $(SRCS)

install:: libXt.a
	$(INSTALL) -c $(INSTLIBFLAGS) libXt.a $(USRLIBDIR)
	$(RANLIB) $(RANLIBINSTFLAGS) $(USRLIBDIR)/libXt.a

install.ln:: llib-lXt.ln
	$(INSTALL) -c $(INSTLIBFLAGS) llib-lXt.ln $(LINTLIBDIR)

install:: $(HEADERS)
	@case '${MFLAGS}' in *[i]*) set +e;; esac; \
	for i in $(HEADERS); do \
	(set -x; $(INSTALL) -c $(INSTALLFLAGS) $$i $(INCDIR)); \
	done

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

Alloc.o: ../.././X11/Xlib.h /usr/include/sys/types.h
Alloc.o: /usr/include/sys/sysmacros.h ../.././X11/X.h IntrinsicI.h
Alloc.o: IntrinsicP.h Intrinsic.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Alloc.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Alloc.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Alloc.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Alloc.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
Alloc.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
Alloc.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h
Alloc.o: /usr/include/sys/param.h /usr/include/machine/param.h
Alloc.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h
ArgList.o: ../.././X11/Xlib.h /usr/include/sys/types.h
ArgList.o: /usr/include/sys/sysmacros.h ../.././X11/X.h /usr/include/stdio.h
ArgList.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xutil.h
ArgList.o: ../.././X11/Xresource.h ../.././X11/Xos.h /usr/include/strings.h
ArgList.o: /usr/include/sys/file.h /usr/include/sys/fcntl.h
ArgList.o: /usr/include/sys/time.h /usr/include/sys/time.h Core.h Composite.h
ArgList.o: Constraint.h CoreP.h CompositeP.h ConstrainP.h Object.h RectObj.h
ArgList.o: WindowObj.h CompObj.h ObjectP.h RectObjP.h WindowObjP.h CompObjP.h
ArgList.o: TranslateI.h CallbackI.h CompositeI.h ConvertI.h InitialI.h
ArgList.o: /usr/include/sys/param.h /usr/include/machine/param.h
ArgList.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h
Callback.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Callback.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Callback.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Callback.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Callback.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Callback.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Callback.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h
Callback.o: CompObj.h ObjectP.h RectObjP.h WindowObjP.h CompObjP.h
Callback.o: TranslateI.h CallbackI.h CompositeI.h ConvertI.h InitialI.h
Callback.o: /usr/include/sys/param.h /usr/include/machine/param.h
Callback.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h
CompObj.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
CompObj.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
CompObj.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
CompObj.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
CompObj.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
CompObj.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
CompObj.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
CompObj.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
CompObj.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h
CompObj.o: /usr/include/sys/param.h /usr/include/machine/param.h
CompObj.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h StringDefs.h
Composite.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Composite.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Composite.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Composite.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Composite.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Composite.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Composite.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h
Composite.o: CompObj.h ObjectP.h RectObjP.h WindowObjP.h CompObjP.h
Composite.o: TranslateI.h CallbackI.h CompositeI.h ConvertI.h InitialI.h
Composite.o: /usr/include/sys/param.h /usr/include/machine/param.h
Composite.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h StringDefs.h
Constraint.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Constraint.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Constraint.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Constraint.o: ../.././X11/Xos.h /usr/include/strings.h
Constraint.o: /usr/include/sys/file.h /usr/include/sys/fcntl.h
Constraint.o: /usr/include/sys/time.h /usr/include/sys/time.h Core.h
Constraint.o: Composite.h Constraint.h CoreP.h CompositeP.h ConstrainP.h
Constraint.o: Object.h RectObj.h WindowObj.h CompObj.h ObjectP.h RectObjP.h
Constraint.o: WindowObjP.h CompObjP.h TranslateI.h CallbackI.h CompositeI.h
Constraint.o: ConvertI.h InitialI.h /usr/include/sys/param.h
Constraint.o: /usr/include/machine/param.h /usr/include/sys/signal.h fd.h
Constraint.o: ResourceI.h EventI.h StringDefs.h
Convert.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Convert.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Convert.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Convert.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Convert.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Convert.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Convert.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
Convert.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
Convert.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h
Convert.o: /usr/include/sys/param.h /usr/include/machine/param.h
Convert.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h Quarks.h
Convert.o: /usr/include/stdio.h
Converters.o: StringDefs.h /usr/include/stdio.h ../.././X11/cursorfont.h
Converters.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Converters.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Converters.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Converters.o: ../.././X11/Xos.h /usr/include/strings.h
Converters.o: /usr/include/sys/file.h /usr/include/sys/fcntl.h
Converters.o: /usr/include/sys/time.h /usr/include/sys/time.h Core.h
Converters.o: Composite.h Constraint.h CoreP.h CompositeP.h ConstrainP.h
Converters.o: Object.h RectObj.h WindowObj.h CompObj.h ObjectP.h RectObjP.h
Converters.o: WindowObjP.h CompObjP.h TranslateI.h CallbackI.h CompositeI.h
Converters.o: ConvertI.h InitialI.h /usr/include/sys/param.h
Converters.o: /usr/include/machine/param.h /usr/include/sys/signal.h fd.h
Converters.o: ResourceI.h EventI.h Quarks.h
Core.o: IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h /usr/include/sys/types.h
Core.o: /usr/include/sys/sysmacros.h ../.././X11/X.h ../.././X11/Xutil.h
Core.o: ../.././X11/Xresource.h ../.././X11/Xos.h /usr/include/strings.h
Core.o: /usr/include/sys/file.h /usr/include/sys/fcntl.h
Core.o: /usr/include/sys/time.h /usr/include/sys/time.h Core.h Composite.h
Core.o: Constraint.h CoreP.h CompositeP.h ConstrainP.h WindowObj.h
Core.o: WindowObjP.h RectObjP.h ObjectP.h
Create.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Create.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Create.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Create.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Create.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Create.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Create.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
Create.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
Create.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h
Create.o: /usr/include/sys/param.h /usr/include/machine/param.h
Create.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h StringDefs.h
Create.o: Shell.h ShellP.h VendorP.h /usr/include/stdio.h
Destroy.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Destroy.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Destroy.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Destroy.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Destroy.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Destroy.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Destroy.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
Destroy.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
Destroy.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h
Destroy.o: /usr/include/sys/param.h /usr/include/machine/param.h
Destroy.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h
Display.o: ../.././X11/Xlib.h /usr/include/sys/types.h
Display.o: /usr/include/sys/sysmacros.h ../.././X11/X.h
Display.o: /usr/include/sys/param.h /usr/include/machine/param.h
Display.o: /usr/include/sys/signal.h IntrinsicI.h IntrinsicP.h Intrinsic.h
Display.o: ../.././X11/Xutil.h ../.././X11/Xresource.h ../.././X11/Xos.h
Display.o: /usr/include/strings.h /usr/include/sys/file.h
Display.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Display.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Display.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
Display.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
Display.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h fd.h ResourceI.h
Display.o: EventI.h
Error.o: ../.././X11/Xlib.h /usr/include/sys/types.h
Error.o: /usr/include/sys/sysmacros.h ../.././X11/X.h /usr/include/stdio.h
Error.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xutil.h
Error.o: ../.././X11/Xresource.h ../.././X11/Xos.h /usr/include/strings.h
Error.o: /usr/include/sys/file.h /usr/include/sys/fcntl.h
Error.o: /usr/include/sys/time.h /usr/include/sys/time.h Core.h Composite.h
Error.o: Constraint.h CoreP.h CompositeP.h ConstrainP.h Object.h RectObj.h
Error.o: WindowObj.h CompObj.h ObjectP.h RectObjP.h WindowObjP.h CompObjP.h
Error.o: TranslateI.h CallbackI.h CompositeI.h ConvertI.h InitialI.h
Error.o: /usr/include/sys/param.h /usr/include/machine/param.h
Error.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h
Event.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Event.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Event.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Event.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Event.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Event.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Event.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
Event.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
Event.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h
Event.o: /usr/include/sys/param.h /usr/include/machine/param.h
Event.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h Shell.h
Event.o: StringDefs.h
GCManager.o: /usr/include/stdio.h IntrinsicI.h IntrinsicP.h Intrinsic.h
GCManager.o: ../.././X11/Xlib.h /usr/include/sys/types.h
GCManager.o: /usr/include/sys/sysmacros.h ../.././X11/X.h ../.././X11/Xutil.h
GCManager.o: ../.././X11/Xresource.h ../.././X11/Xos.h /usr/include/strings.h
GCManager.o: /usr/include/sys/file.h /usr/include/sys/fcntl.h
GCManager.o: /usr/include/sys/time.h /usr/include/sys/time.h Core.h
GCManager.o: Composite.h Constraint.h CoreP.h CompositeP.h ConstrainP.h
GCManager.o: Object.h RectObj.h WindowObj.h CompObj.h ObjectP.h RectObjP.h
GCManager.o: WindowObjP.h CompObjP.h TranslateI.h CallbackI.h CompositeI.h
GCManager.o: ConvertI.h InitialI.h /usr/include/sys/param.h
GCManager.o: /usr/include/machine/param.h /usr/include/sys/signal.h fd.h
GCManager.o: ResourceI.h EventI.h
Geometry.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Geometry.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Geometry.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Geometry.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Geometry.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Geometry.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Geometry.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h
Geometry.o: CompObj.h ObjectP.h RectObjP.h WindowObjP.h CompObjP.h
Geometry.o: TranslateI.h CallbackI.h CompositeI.h ConvertI.h InitialI.h
Geometry.o: /usr/include/sys/param.h /usr/include/machine/param.h
Geometry.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h Shell.h
Initialize.o: /usr/include/pwd.h /usr/include/sys/param.h
Initialize.o: /usr/include/machine/param.h /usr/include/sys/signal.h
Initialize.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Initialize.o: /usr/include/stdio.h IntrinsicI.h IntrinsicP.h Intrinsic.h
Initialize.o: ../.././X11/Xlib.h ../.././X11/X.h ../.././X11/Xutil.h
Initialize.o: ../.././X11/Xresource.h ../.././X11/Xos.h
Initialize.o: /usr/include/strings.h /usr/include/sys/file.h
Initialize.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Initialize.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Initialize.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h
Initialize.o: CompObj.h ObjectP.h RectObjP.h WindowObjP.h CompObjP.h
Initialize.o: TranslateI.h CallbackI.h CompositeI.h ConvertI.h InitialI.h
Initialize.o: fd.h ResourceI.h EventI.h StringDefs.h Shell.h ShellP.h
Initialize.o: VendorP.h Quarks.h
Intrinsic.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Intrinsic.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Intrinsic.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Intrinsic.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Intrinsic.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Intrinsic.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Intrinsic.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h
Intrinsic.o: CompObj.h ObjectP.h RectObjP.h WindowObjP.h CompObjP.h
Intrinsic.o: TranslateI.h CallbackI.h CompositeI.h ConvertI.h InitialI.h
Intrinsic.o: /usr/include/sys/param.h /usr/include/machine/param.h
Intrinsic.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h StringDefs.h
Manage.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Manage.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Manage.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Manage.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Manage.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Manage.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Manage.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
Manage.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
Manage.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h
Manage.o: /usr/include/sys/param.h /usr/include/machine/param.h
Manage.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h
NextEvent.o: /usr/include/stdio.h /usr/include/errno.h
NextEvent.o: /usr/include/sys/errno.h ../.././X11/Xlib.h
NextEvent.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
NextEvent.o: ../.././X11/X.h IntrinsicI.h IntrinsicP.h Intrinsic.h
NextEvent.o: ../.././X11/Xutil.h ../.././X11/Xresource.h ../.././X11/Xos.h
NextEvent.o: /usr/include/strings.h /usr/include/sys/file.h
NextEvent.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
NextEvent.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
NextEvent.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h
NextEvent.o: CompObj.h ObjectP.h RectObjP.h WindowObjP.h CompObjP.h
NextEvent.o: TranslateI.h CallbackI.h CompositeI.h ConvertI.h InitialI.h
NextEvent.o: /usr/include/sys/param.h /usr/include/machine/param.h
NextEvent.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h
Object.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Object.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Object.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Object.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Object.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Object.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Object.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
Object.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
Object.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h
Object.o: /usr/include/sys/param.h /usr/include/machine/param.h
Object.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h StringDefs.h
Popup.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Popup.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Popup.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Popup.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Popup.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Popup.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Popup.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
Popup.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
Popup.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h
Popup.o: /usr/include/sys/param.h /usr/include/machine/param.h
Popup.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h Shell.h ShellP.h
Popup.o: VendorP.h StringDefs.h
RectObj.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
RectObj.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
RectObj.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
RectObj.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
RectObj.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
RectObj.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
RectObj.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
RectObj.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
RectObj.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h
RectObj.o: /usr/include/sys/param.h /usr/include/machine/param.h
RectObj.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h StringDefs.h
Resources.o: /usr/include/stdio.h IntrinsicI.h IntrinsicP.h Intrinsic.h
Resources.o: ../.././X11/Xlib.h /usr/include/sys/types.h
Resources.o: /usr/include/sys/sysmacros.h ../.././X11/X.h ../.././X11/Xutil.h
Resources.o: ../.././X11/Xresource.h ../.././X11/Xos.h /usr/include/strings.h
Resources.o: /usr/include/sys/file.h /usr/include/sys/fcntl.h
Resources.o: /usr/include/sys/time.h /usr/include/sys/time.h Core.h
Resources.o: Composite.h Constraint.h CoreP.h CompositeP.h ConstrainP.h
Resources.o: Object.h RectObj.h WindowObj.h CompObj.h ObjectP.h RectObjP.h
Resources.o: WindowObjP.h CompObjP.h TranslateI.h CallbackI.h CompositeI.h
Resources.o: ConvertI.h InitialI.h /usr/include/sys/param.h
Resources.o: /usr/include/machine/param.h /usr/include/sys/signal.h fd.h
Resources.o: ResourceI.h EventI.h Shell.h ShellP.h VendorP.h StringDefs.h
Selection.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
Selection.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Selection.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
Selection.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
Selection.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Selection.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Selection.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h
Selection.o: CompObj.h ObjectP.h RectObjP.h WindowObjP.h CompObjP.h
Selection.o: TranslateI.h CallbackI.h CompositeI.h ConvertI.h InitialI.h
Selection.o: /usr/include/sys/param.h /usr/include/machine/param.h
Selection.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h Selection.h
Selection.o: StringDefs.h SelectionI.h
Shell.o: /usr/include/pwd.h /usr/include/stdio.h /usr/include/sys/param.h
Shell.o: /usr/include/machine/param.h /usr/include/sys/signal.h
Shell.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
Shell.o: ../.././X11/Xatom.h IntrinsicI.h IntrinsicP.h Intrinsic.h
Shell.o: ../.././X11/Xlib.h ../.././X11/X.h ../.././X11/Xutil.h
Shell.o: ../.././X11/Xresource.h ../.././X11/Xos.h /usr/include/strings.h
Shell.o: /usr/include/sys/file.h /usr/include/sys/fcntl.h
Shell.o: /usr/include/sys/time.h /usr/include/sys/time.h Core.h Composite.h
Shell.o: Constraint.h CoreP.h CompositeP.h ConstrainP.h Object.h RectObj.h
Shell.o: WindowObj.h CompObj.h ObjectP.h RectObjP.h WindowObjP.h CompObjP.h
Shell.o: TranslateI.h CallbackI.h CompositeI.h ConvertI.h InitialI.h fd.h
Shell.o: ResourceI.h EventI.h StringDefs.h Shell.h ShellP.h VendorP.h
Shell.o: Vendor.h
TMparse.o: ../.././X11/Xos.h /usr/include/sys/types.h
TMparse.o: /usr/include/sys/sysmacros.h /usr/include/strings.h
TMparse.o: /usr/include/sys/file.h /usr/include/sys/fcntl.h
TMparse.o: /usr/include/sys/time.h /usr/include/sys/time.h ../.././X11/Xlib.h
TMparse.o: ../.././X11/X.h StringDefs.h /usr/include/stdio.h IntrinsicI.h
TMparse.o: IntrinsicP.h Intrinsic.h ../.././X11/Xutil.h
TMparse.o: ../.././X11/Xresource.h Core.h Composite.h Constraint.h CoreP.h
TMparse.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
TMparse.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
TMparse.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h
TMparse.o: /usr/include/sys/param.h /usr/include/machine/param.h
TMparse.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h
TMparse.o: ../.././X11/keysymdef.h
TMstate.o: ../.././X11/Xlib.h /usr/include/sys/types.h
TMstate.o: /usr/include/sys/sysmacros.h ../.././X11/X.h
TMstate.o: ../.././X11/keysymdef.h StringDefs.h /usr/include/stdio.h
TMstate.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xutil.h
TMstate.o: ../.././X11/Xresource.h ../.././X11/Xos.h /usr/include/strings.h
TMstate.o: /usr/include/sys/file.h /usr/include/sys/fcntl.h
TMstate.o: /usr/include/sys/time.h /usr/include/sys/time.h Core.h Composite.h
TMstate.o: Constraint.h CoreP.h CompositeP.h ConstrainP.h Object.h RectObj.h
TMstate.o: WindowObj.h CompObj.h ObjectP.h RectObjP.h WindowObjP.h CompObjP.h
TMstate.o: TranslateI.h CallbackI.h CompositeI.h ConvertI.h InitialI.h
TMstate.o: /usr/include/sys/param.h /usr/include/machine/param.h
TMstate.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h
Vendor.o: /usr/include/pwd.h /usr/include/stdio.h /usr/include/sys/param.h
Vendor.o: /usr/include/machine/param.h /usr/include/sys/signal.h
Vendor.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h IntrinsicI.h
Vendor.o: IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h ../.././X11/X.h
Vendor.o: ../.././X11/Xutil.h ../.././X11/Xresource.h ../.././X11/Xos.h
Vendor.o: /usr/include/strings.h /usr/include/sys/file.h
Vendor.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
Vendor.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
Vendor.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h CompObj.h
Vendor.o: ObjectP.h RectObjP.h WindowObjP.h CompObjP.h TranslateI.h
Vendor.o: CallbackI.h CompositeI.h ConvertI.h InitialI.h fd.h ResourceI.h
Vendor.o: EventI.h StringDefs.h Shell.h ShellP.h VendorP.h Vendor.h
WindowObj.o: IntrinsicI.h IntrinsicP.h Intrinsic.h ../.././X11/Xlib.h
WindowObj.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
WindowObj.o: ../.././X11/X.h ../.././X11/Xutil.h ../.././X11/Xresource.h
WindowObj.o: ../.././X11/Xos.h /usr/include/strings.h /usr/include/sys/file.h
WindowObj.o: /usr/include/sys/fcntl.h /usr/include/sys/time.h
WindowObj.o: /usr/include/sys/time.h Core.h Composite.h Constraint.h CoreP.h
WindowObj.o: CompositeP.h ConstrainP.h Object.h RectObj.h WindowObj.h
WindowObj.o: CompObj.h ObjectP.h RectObjP.h WindowObjP.h CompObjP.h
WindowObj.o: TranslateI.h CallbackI.h CompositeI.h ConvertI.h InitialI.h
WindowObj.o: /usr/include/sys/param.h /usr/include/machine/param.h
WindowObj.o: /usr/include/sys/signal.h fd.h ResourceI.h EventI.h StringDefs.h
