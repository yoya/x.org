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

 XWSRC = $(CONTRIBSRC)/widgets/Xhp/Xw

 XWLIB = $(USRLIBDIR)/libXw.a

###########################################################################
# definitions common to all Makefiles - do not edit

          SHELL = /bin/sh

        DESTDIR =
      USRLIBDIR = /local/lib/X11
         BINDIR = /local/bin/X11
         INCDIR = $(LIBDIR)/include
        INCROOT = $(DESTDIR)/usr/include
         ADMDIR = /var/scr/Xerrors
         LIBDIR = /local/share/X11
     LINTLIBDIR = $(LIBDIR)/lint
        FONTDIR = $(LIBDIR)/fonts
       XINITDIR = $(LIBDIR)/xinit
         XDMDIR = $(LIBDIR)/xdm
         UWMDIR = $(LIBDIR)/uwm
         AWMDIR = $(LIBDIR)/awm
         TWMDIR = $(LIBDIR)/twm
        MANPATH = /local/man
  MANSOURCEPATH = $(MANPATH)/man
         MANDIR = $(MANSOURCEPATH)x
      LIBMANDIR = $(MANSOURCEPATH)3
    XAPPLOADDIR = $(LIBDIR)/app-defaults

   INSTBINFLAGS = -m 0755
   INSTUIDFLAGS = -m 4755
   INSTLIBFLAGS = -m 0664
   INSTINCFLAGS = -m 0444
   INSTMANFLAGS = -m 0444
   INSTAPPFLAGS = -m 0444
  INSTKMEMFLAGS = -g kmem -m 2755
        FCFLAGS = -t
    CDEBUGFLAGS = -O

        PATHSEP = /
         DEPEND = $(DEPENDSRC)/makedepend
          IMAKE = $(BINDIR)/imake
            RGB = $(RGBSRC)/rgb
             FC = $(BDFTOSNFSRC)/bdftosnf
      MKFONTDIR = $(MKFONTDIRSRC)/mkfontdir
      MKDIRHIER = $(SCRIPTSSRC)/mkdirhier.sh

         CFLAGS = $(CDEBUGFLAGS) $(INCLUDES) $(STD_DEFINES) $(DEFINES)
      LINTFLAGS = $(LINTOPTS) $(INCLUDES) $(STD_DEFINES) $(DEFINES) -DLINT
        LDFLAGS = $(CDEBUGFLAGS) $(SYS_LIBRARIES) $(SYSAUX_LIBRARIES)
            TOP = ../../../../../../src/X.V11R3
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

   EXTENSIONLIB = $(USRLIBDIR)/lib/libXext.a

           XLIB = -L$(USRLIBDIR) -lX11

         XMULIB = $(USRLIBDIR)/libXmu.a
        OLDXLIB = $(USRLIBDIR)/liboldX.a
       XTOOLLIB = $(USRLIBDIR)/libXt.a
         XAWLIB = $(USRLIBDIR)/libXaw.a
       INCLUDES = -I$(INCDIR) -I$(INCROOT)

       LINTXLIB = $(XLIBSRC)/llib-lX11.ln
        LINTXMU = $(XMUSRC)/llib-lXmu.ln
      LINTXTOOL = $(TOOLKITSRC)/llib-lXt.ln
        LINTXAW = $(AWIDGETSRC)/llib-lXaw.ln
      MACROFILE = Sun.macros
   ICONFIGFILES = $(IRULESRC)/Imake.tmpl \
			$(IRULESRC)/$(MACROFILE) $(IRULESRC)/site.def
  IMAKE_DEFINES =
      IMAKE_CMD = $(IMAKE) -TImake.tmpl -I$(NEWTOP)$(IRULESRC) \
			-s Makefile $(IMAKE_DEFINES)
         RM_CMD = $(RM) *.CKP *.ln *.BAK *.bak *.o core errs ,* *~ *.a \
			.emacs_* tags TAGS make.log MakeOut

###########################################################################
# rules:  $XConsortium: Imake.rules,v 1.71 88/10/23 22:46:34 jim Exp $

###########################################################################
# start of Imakefile

#	Not standalone - it gets a lot of variables from the invocation.
#	VFONTS, RSTFONTS, SUNTROFF_FONTS, PERCENT, PREVIEWER, FC

DEFINES=-DVFONTDIR=\"$(VFONTS)\" -DRSTFONTDIR=\"$(RSTFONTS)\"
SRCS = vft2ch.c ch2vft.c ch2rst.c rst2ch.c scalech.c vf2bdf.c

all: vft2ch ch2vft ch2rst rst2ch scalech vf2bdf

vft2ch: vft2ch.o
	$(RM) $@
	$(CC) -o $@ vft2ch.o  $(LDFLAGS)  $(SYSLAST_LIBRARIES)

relink::
	$(RM) vft2ch
	$(MAKE) $(MFLAGS) vft2ch

clean::
	$(RM) vft2ch

install:: vft2ch
	$(INSTALL) -c $(INSTALLFLAGS) vft2ch  $(BINDIR)

install.man:: vft2ch.man
	$(INSTALL) -c $(INSTMANFLAGS) vft2ch.man  $(MANDIR)/vft2ch.x

ch2vft: ch2vft.o
	$(RM) $@
	$(CC) -o $@ ch2vft.o  $(LDFLAGS)  $(SYSLAST_LIBRARIES)

relink::
	$(RM) ch2vft
	$(MAKE) $(MFLAGS) ch2vft

clean::
	$(RM) ch2vft

install:: ch2vft
	$(INSTALL) -c $(INSTALLFLAGS) ch2vft  $(BINDIR)

install.man:: ch2vft.man
	$(INSTALL) -c $(INSTMANFLAGS) ch2vft.man  $(MANDIR)/ch2vft.x

ch2rst: ch2rst.o
	$(RM) $@
	$(CC) -o $@ ch2rst.o  $(LDFLAGS)  $(SYSLAST_LIBRARIES)

relink::
	$(RM) ch2rst
	$(MAKE) $(MFLAGS) ch2rst

clean::
	$(RM) ch2rst

install:: ch2rst
	$(INSTALL) -c $(INSTALLFLAGS) ch2rst  $(BINDIR)

install.man:: ch2rst.man
	$(INSTALL) -c $(INSTMANFLAGS) ch2rst.man  $(MANDIR)/ch2rst.x

rst2ch: rst2ch.o
	$(RM) $@
	$(CC) -o $@ rst2ch.o  $(LDFLAGS)  $(SYSLAST_LIBRARIES)

relink::
	$(RM) rst2ch
	$(MAKE) $(MFLAGS) rst2ch

clean::
	$(RM) rst2ch

install:: rst2ch
	$(INSTALL) -c $(INSTALLFLAGS) rst2ch  $(BINDIR)

install.man:: rst2ch.man
	$(INSTALL) -c $(INSTMANFLAGS) rst2ch.man  $(MANDIR)/rst2ch.x

scalech: scalech.o
	$(RM) $@
	$(CC) -o $@ scalech.o  $(LDFLAGS)  $(SYSLAST_LIBRARIES)

relink::
	$(RM) scalech
	$(MAKE) $(MFLAGS) scalech

clean::
	$(RM) scalech

install:: scalech
	$(INSTALL) -c $(INSTALLFLAGS) scalech  $(BINDIR)

install.man:: scalech.man
	$(INSTALL) -c $(INSTMANFLAGS) scalech.man  $(MANDIR)/scalech.x

vf2bdf: vf2bdf.o
	$(RM) $@
	$(CC) -o $@ vf2bdf.o  $(LDFLAGS)  $(SYSLAST_LIBRARIES)

relink::
	$(RM) vf2bdf
	$(MAKE) $(MFLAGS) vf2bdf

clean::
	$(RM) vf2bdf

install:: vf2bdf
	$(INSTALL) -c $(INSTALLFLAGS) vf2bdf  $(BINDIR)

install.man:: vf2bdf.man
	$(INSTALL) -c $(INSTMANFLAGS) vf2bdf.man  $(MANDIR)/vf2bdf.x

depend:: $(DEPEND)

depend::
	$(DEPEND) -s "# DO NOT DELETE" -- $(CFLAGS) -- $(SRCS)

$(DEPEND)::

#		The two scripts check for suntroff or xtroff.
sunfonts:	all
		if test -d ${VFONTS}; then \
			./MakeSunFonts ${VFONTS} ${SUNTROFF_FONTS} \
			${SUNPERCENT} ${PREVIEWER} ${FC} ${SUNDEVICE}; fi

rstfonts:	all
		if test -d ${RSTFONTS}; then \
			./MakeImFonts ${RSTFONTS} ${SUNTROFF_FONTS} \
			${RSTPERCENT} ${PREVIEWER} ${FC} ${RSTDEVICE}; fi

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

tags::
	$(TAGS) -w *.[ch]
	$(TAGS) -xw *.[ch] > TAGS

$(XLIB)::;
-L$(USRLIBDIR)::;
-L$(LIBSRC)/Xpic::;

###########################################################################
# empty rules for directories that do not have SUBDIRS - do not edit

install::
	@echo "install done"

install.man::
	@echo "install.man done"

Makefiles::

###########################################################################
# dependencies generated by makedepend

