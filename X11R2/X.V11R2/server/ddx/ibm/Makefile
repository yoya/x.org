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

            TOP = ./../../..
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

 SUBDIRS0=
 SUBOBJS0=
 DSUBOBJS0=
 PSUBOBJS0=

 SUBDIRS1= $(SUBDIRS0)
 SUBOBJS1= $(SUBOBJS0)
 DSUBOBJS1= $(DSUBOBJS0)
 PSUBOBJS1= $(PSUBOBJS0)

 SUBDIRS2= $(SUBDIRS1)
 SUBOBJS2= $(SUBOBJS1)
 DSUBOBJS2= $(DSUBOBJS1)
 PSUBOBJS2= $(PSUBOBJS1)

 SUBDIRS3= $(SUBDIRS2)
 SUBOBJS3= $(SUBOBJS2)
 DSUBOBJS3= $(DSUBOBJS2)
 PSUBOBJS3= $(PSUBOBJS2)

 SUBDIRS4= $(SUBDIRS3)
 SUBOBJS4= $(SUBOBJS3)
 DSUBOBJS4= $(DSUBOBJS3)
 PSUBOBJS4= $(PSUBOBJS3)

 SUBDIRS5= $(SUBDIRS4)
 SUBOBJS5= $(SUBOBJS4)
 DSUBOBJS5= $(DSUBOBJS4)
 PSUBOBJS5= $(PSUBOBJS4)

SUBDIRS= rt $(SUBDIRS5) ppc
SUBOBJS= rt/rt.o $(SUBOBJS5) ppc/ppc.o
DSUBOBJS= rt/rt_d.o $(DSUBOBJS5) ppc/ppc_d.o
PSUBOBJS= rt/rt_p.o $(PSUBOBJS5) ppc/ppc_p.o

all::	subdirs
libibm.a:	Makefile

all:: libibm.a

libibm.a: $(SUBOBJS)
	$(RM) $@
	$(AR) $@ $(SUBOBJS)
	$(RANLIB) $@

subdirs::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
	(cd $$i ; echo "making $$i"; \
	$(MAKE) 'CDEBUGFLAGS=$(CDEBUGFLAGS)' 'CC=$(CC)' $(MFLAGS)); \
	done

lint::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
	(cd $$i ; echo "linting $$i"; \
	$(MAKE) $(MFLAGS) \
	LINTOPTS='$(LINTOPTS)' lint ); \
	done

lintlib::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
	(cd $$i ; echo "making lintlib in $$i"; \
	$(MAKE) $(MFLAGS) \
	LINTOPTS='$(LINTOPTS)' lintlib ); \
	done

#
# before doing the depend, be sure that makedepend exists.
#
depend:: $(DEPEND)

depend::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
	(cd $$i ; echo "depending $$i"; \
	$(MAKE) $(MFLAGS) depend ); \
	done

#
# before making the makefiles, be sure that imake exists.
#
Makefiles:: $(IMAKE)

$(IMAKE):
	@echo "making $(IMAKESRC)"; \
	cd $(IMAKESRC); $(MAKE)

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
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
	(cd $$i ; echo "installing $$i"; \
	$(MAKE) $(MFLAGS) \
	DESTDIR='$(DESTDIR)' install ); \
	done

clean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
	(cd $$i ; echo "cleaning $$i"; \
	$(MAKE) $(MFLAGS) RM_CMD='$(RM_CMD)' clean ); \
	done

tags::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
	(cd $$i ; echo "tagging $$i"; \
	$(MAKE) $(MFLAGS) TAGS='$(TAGS)' tags ); \
	done

Makefiles:: $(IMAKE)

Makefiles::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
	echo "Making Makefiles in $$i..."; \
	case "$$i" in \
	./?*/?*/?*/?*) newtop=../../../../ sub=subsubsubsub;; \
	./?*/?*/?*) newtop=../../../ sub=subsubsub;; \
	./?*/?*) newtop=../../ sub=subsub;; \
	./?*) newtop=../ sub=sub;; \
	*/?*/?*/?*) newtop=../../../../ sub=subsubsubsub;; \
	*/?*/?*) newtop=../../../ sub=subsubsub;; \
	*/?*) newtop=../../ sub=subsub;; \
	*) newtop=../ sub=sub;; \
	esac; \
	$(MAKE) $${sub}dirMakefiles NEWTOP=$$newtop \
	MAKEFILE_SUBDIR=$$i;\
	done

subdirMakefiles:
	$(RM) $(MAKEFILE_SUBDIR)/Makefile.bak
	-$(MV) $(MAKEFILE_SUBDIR)/Makefile $(MAKEFILE_SUBDIR)/Makefile.bak
	cd $(MAKEFILE_SUBDIR); $(IMAKE_CMD) -DTOPDIR=$(TOP)/..; \
	$(MAKE) $(MFLAGS) Makefiles

subsubdirMakefiles:
	$(RM) $(MAKEFILE_SUBDIR)/Makefile.bak
	-$(MV) $(MAKEFILE_SUBDIR)/Makefile $(MAKEFILE_SUBDIR)/Makefile.bak
	cd $(MAKEFILE_SUBDIR); $(IMAKE_CMD) -DTOPDIR=$(TOP)/../..; \
	$(MAKE) $(MFLAGS) Makefiles

subsubsubdirMakefiles:
	$(RM) $(MAKEFILE_SUBDIR)/Makefile.bak
	-$(MV) $(MAKEFILE_SUBDIR)/Makefile $(MAKEFILE_SUBDIR)/Makefile.bak
	cd $(MAKEFILE_SUBDIR); $(IMAKE_CMD) -DTOPDIR=$(TOP)/../../..; \
	$(MAKE) $(MFLAGS) Makefiles

subsubsubsubdirMakefiles:
	$(RM) $(MAKEFILE_SUBDIR)/Makefile.bak
	-$(MV) $(MAKEFILE_SUBDIR)/Makefile $(MAKEFILE_SUBDIR)/Makefile.bak
	cd $(MAKEFILE_SUBDIR); $(IMAKE_CMD) -DTOPDIR=$(TOP)/../../../..; \
	$(MAKE) $(MFLAGS) Makefiles

