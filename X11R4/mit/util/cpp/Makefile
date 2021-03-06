#
# This is a sample makefile.  If your system can handle it, you should make
# imake and then regenerate this makefile.  Otherwise, you can do it by hand.
# The defaults are currently set for systems which we know are likely to need
# this version of cpp.
#

# Unix makefile for cpp
#
# The redefinition of strchr() and strrchr() are needed for
# Ultrix-32, Unix 4.2 bsd (and maybe some other Unices).
#
#BSDDEFINE = -Dstrchr=index -Dstrrchr=rindex
BSDDEFINE = 
#
# On certain systems, such as Unix System III, you may need to define
# $(LINTFLAGS) in the make command line to set system-specific lint flags.
#
# This Makefile assumes cpp will replace the "standard" preprocessor.
# Delete the reference to -DLINE_PREFIX=\"\" if cpp is used stand-alone.
# LINEFIX is a sed script filter that reinserts #line -- used for testing
# if LINE_PREFIX is set to "".   Note that we must stand on our heads to
# match the # and a line had better not begin with $.  By the way, what
# we really want is
#	LINEFIX = | sed "s/^#/#line/"
#
CPPDEFINE = -DLINE_PREFIX=\"\"
LINEFIX = | sed "s/^[^ !\"%-~]/&line/"
#
# Define OLD_PREPROCESSOR non-zero to make a preprocessor which is
# "as compatible as possible" with the standard Unix V7 or Ultrix
# preprocessors.  This is needed to rebuild 4.2bsd, for example, as
# the preprocessor is used to modify assembler code, rather than C.
# This is not recommended for current development.  OLD_PREPROCESSOR
# forces the following definitions:
#   OK_DOLLAR		FALSE	$ is not allowed in variables
#   OK_CONCAT		FALSE	# cannot concatenate tokens
#   COMMENT_INVISIBLE	TRUE	old-style comment concatenation
#   STRING_FORMAL	TRUE	old-style string expansion
#
OLDDEFINE = -DOLD_PREPROCESSOR=1
#
# DEFINES collects all -D arguments for cc and lint:
# Change DEFINES = $(BSDDEFINE) $(CPPDEFINE) $(OLDDEFINE)
# for an old-style preprocessor.
#
DEFINES = $(BSDDEFINE) $(CPPDEFINE) $(OLDDEFINE)

CFLAGS = -O $(DEFINES)

#
# ** compile cpp
#
SRCS = cpp1.c cpp2.c cpp3.c cpp4.c cpp5.c cpp6.c
OBJS = cpp1.o cpp2.o cpp3.o cpp4.o cpp5.o cpp6.o
cpp: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o cpp

#
# ** Test cpp by preprocessing itself, compiling the result,
# ** repeating the process and diff'ing the result.  Note: this
# ** is not a good test of cpp, but a simple verification.
# ** The diff's should not report any changes.
# ** Note that a sed script may be executed for each compile
#
test:
	cpp cpp1.c $(LINEFIX) >old.tmp1.c
	cpp cpp2.c $(LINEFIX) >old.tmp2.c
	cpp cpp3.c $(LINEFIX) >old.tmp3.c
	cpp cpp4.c $(LINEFIX) >old.tmp4.c
	cpp cpp5.c $(LINEFIX) >old.tmp5.c
	cpp cpp6.c $(LINEFIX) >old.tmp6.c
	$(CC) $(CFLAGS) old.tmp[123456].c
	a.out cpp1.c >new.tmp1.c
	a.out cpp2.c >new.tmp2.c
	a.out cpp3.c >new.tmp3.c
	a.out cpp4.c >new.tmp4.c
	a.out cpp5.c >new.tmp5.c
	a.out cpp6.c >new.tmp6.c
	diff old.tmp1.c new.tmp1.c
	diff old.tmp2.c new.tmp2.c
	diff old.tmp3.c new.tmp3.c
	diff old.tmp4.c new.tmp4.c
	diff old.tmp5.c new.tmp5.c
	diff old.tmp6.c new.tmp6.c
	rm a.out old.tmp[123456].* new.tmp[123456].*

#
# A somewhat more extensive test is provided by the "clock"
# program (which is not distributed).  Substitute your favorite
# macro-rich program here.
#
clock:	clock.c cpp
	cpp clock.c $(LINEFIX) >temp.cpp.c
	cc temp.cpp.c -lcurses -ltermcap -o clock
	rm temp.cpp.c

#
# ** Lint the code
#

lint:	$(SRCS)
	lint $(LINTFLAGS) $(DEFINES) $(SRCS)

#
# ** Remove unneeded files
#
clean:
	rm -f $(OBJS) cpp

#
# ** Rebuild the archive files needed to distribute cpp
# ** Uses the Decus C archive utility.
#

archc:	archc.c
	$(CC) $(CFLAGS) archc.c -o archc

archx:	archx.c
	$(CC) $(CFLAGS) archx.c -o archx

archive: archc
	archc readme.txt cpp.mem archx.c archc.c cpp.rno makefile.txt \
		cpp*.h >cpp1.arc
	archc cpp1.c cpp2.c cpp3.c >cpp2.arc
	archc cpp4.c cpp5.c cpp6.c >cpp3.arc

#
# Object module dependencies
#

cpp1.o	:	cpp1.c cpp.h cppdef.h

cpp2.o	:	cpp2.c cpp.h cppdef.h

cpp3.o	:	cpp3.c cpp.h cppdef.h

cpp4.o	:	cpp4.c cpp.h cppdef.h

cpp5.o	:	cpp5.c cpp.h cppdef.h

cpp6.o	:	cpp6.c cpp.h cppdef.h

#
# The following is included from a version of the makefile generated by Imake.
#

#
# Warning: the cpp used on this machine replaces
# all newlines and multiple tabs/spaces in a macro
# expansion with a single space.  Imake tries to
# compensate for this, but is not always
# successful.
#

#
# $XConsortium: Makefile.sav,v 1.2 88/10/06 10:43:30 jim Exp $
#
#                                 WARNING
#
# This makefile is generated automatically by the imake program.  Do not
# modify it or you will lose your changes the next time imake is run.  Read
# the INSTALLATION GUIDE and look at the Imake.tmpl and *.macros files in
# the directory ./util/imake.includes/ if you need to change any of the
# configuration parameters.
#
# If you are not using imake, you may need to fix up dependancies from
# the system on which this was generated.
#

#
# $XConsortium: Makefile.sav,v 1.2 88/10/06 10:43:30 jim Exp $
#
# BEGIN Sun.macros - OKAY TO MODIFY (but not recommended)
#

            TOP = ../../.
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
             AR = ar clq
             LS = ls
       LINTOPTS = -axz
    LINTLIBFLAG = -C
           MAKE = make
STD_SYS_DEFINES =
    STD_DEFINES =
    CDEBUGFLAGS = -O
        DESTDIR =

#
# END Sun.macros
#

        PATHSEP = /
         DEPEND = $(DEPENDSRC)/makedepend
          IMAKE = $(IMAKESRC)/imake
            RGB = $(RGBSRC)/rgb
             FC = $(BDFTOSNFSRC)/bdftosnf
      MKFONTDIR = $(MKFONTDIRSRC)/mkfontdir
         CFLAGS = $(CDEBUGFLAGS) $(INCLUDES) $(STD_DEFINES) $(DEFINES)
      LINTFLAGS = $(LINTOPTS) $(INCLUDES) $(STD_DEFINES) $(DEFINES) -DLINT
        LDFLAGS = $(CDEBUGFLAGS) $(SYS_LIBRARIES) $(SYSAUX_LIBRARIES)
   INSTBINFLAGS = -m 0755
   INSTUIDFLAGS = -m 4755
   INSTLIBFLAGS = -m 0664
   INSTINCFLAGS = -m 0444
   INSTMANFLAGS = -m 0444
   INSTAPPFLAGS = -m 0444
        FCFLAGS = -t

      USRLIBDIR = $(DESTDIR)/usr/lib
         BINDIR = $(DESTDIR)/usr/bin/X11
         LIBDIR = $(USRLIBDIR)/X11
     LINTLIBDIR = $(USRLIBDIR)/lint
         INCDIR = $(DESTDIR)/usr/include/X11
        FONTDIR = $(LIBDIR)/fonts
       XINITDIR = $(LIBDIR)/xinit
         XDMDIR = $(LIBDIR)/xdm
         UWMDIR = $(LIBDIR)/uwm
         AWMDIR = $(LIBDIR)/awm
         TWMDIR = $(LIBDIR)/twm
        MANPATH = $(DESTDIR)/usr/man
         MANDIR = $(MANPATH)/mann
      LIBMANDIR = $(MANPATH)/man3
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
         XMUSRC = $(LIBSRC)/Xmu
     TOOLKITSRC = $(LIBSRC)/Xt
     AWIDGETSRC = $(LIBSRC)/Xaw
     OLDXLIBSRC = $(LIBSRC)/oldX
    BDFTOSNFSRC = $(FONTSRC)/bdftosnf
   MKFONTDIRSRC = $(FONTSRC)/mkfontdir
   EXTENSIONSRC = $(TOP)/extensions
        XMANSRC = $(DOCSRC)/Xlib/Xman
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
      IMAKE_CMD = $(NEWTOP)$(IMAKE) -TImake.tmpl \
			-I$(NEWTOP)$(IRULESRC) \
			-s Makefile
         RM_CMD = $(RM) *.CKP *.ln *.BAK *.bak *.o core errs ,* *~ *.a \
			.emacs_* tags TAGS make.log MakeOut

#
# $XConsortium: Makefile.sav,v 1.2 88/10/06 10:43:30 jim Exp $
#
# BEGIN imakeRules - DO NOT MODIFY
#

#
# END imakeRules
#

Makefile:: $(IMAKE)

Makefile:: Imakefile \
	$(IRULESRC)/Imake.tmpl \
	$(IRULESRC)/Imake.rules \
	$(IRULESRC)/$(MACROFILE)
	-$(RM) Makefile.bak; $(MV) Makefile Makefile.bak
	$(IMAKE_CMD) -DTOPDIR=$(TOP)

$(IMAKE):
	@echo "making $(IMAKESRC)"; \
	cd $(IMAKESRC); $(MAKE) BOOTSTRAPCFLAGS=$(BOOTSTRAPCFLAGS)

