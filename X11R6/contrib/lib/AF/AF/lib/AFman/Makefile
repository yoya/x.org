# Makefile generated by imake - do not edit!
# $XConsortium: imake.c,v 1.65 91/07/25 17:50:17 rws Exp $

###########################################################################
# Makefile generated from "Imake.tmpl" and </tmp/IIf.009728>
# $RCS: Imake.tmpl,v 1.77 89/12/18 17:01:37 jim Exp $
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

# -------------------------------------------------------------------------
# site-specific configuration parameters that need to come before
# the platform-specific parameters - edit site.def to change

# -------------------------------------------------------------------------
# platform-specific configuration parameters - edit osf.cf to change

# -------------------------------------------------------------------------
# site-specific configuration parameters that go after
# the platform-specific parameters - edit site.def to change

            SHELL = /bin/sh

              TOP = ../../.
      CURRENT_DIR = ./lib/AFman

               AR = ar cq
  BOOTSTRAPCFLAGS =
               CC = cc

         COMPRESS = CompressCmd
              CPP = /lib/cpp $(STD_CPP_DEFINES)
    PREPROCESSCMD = cc -E $(STD_CPP_DEFINES)
          INSTALL = installbsd
               LD = ld
             LINT = lint
      LINTLIBFLAG = -o
         LINTOPTS = -ax -Nn3000
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
  EXTRA_LIBRARIES = -ldnet_stub
             TAGS = ctags

    PROTO_DEFINES =

     INSTPGMFLAGS =

     INSTBINFLAGS = -m 0755
     INSTUIDFLAGS = -m 4755
     INSTLIBFLAGS = -m 0664
     INSTINCFLAGS = -m 0444
     INSTMANFLAGS = -m 0444
     INSTDATFLAGS = -m 0444
    INSTKMEMFLAGS = -m 4755

          DESTDIR =

     TOP_INCLUDES = -I$(TOP)

      CDEBUGFLAGS = -g
        CCOPTIONS = -std
      COMPATFLAGS =

      ALLINCLUDES = $(STD_INCLUDES) $(TOP_INCLUDES) $(INCLUDES) $(EXTRA_INCLUDES)
       ALLDEFINES = $(ALLINCLUDES) $(STD_DEFINES) $(PROTO_DEFINES) $(DEFINES) $(COMPATFLAGS)
           CFLAGS = $(CDEBUGFLAGS) $(CCOPTIONS) $(ALLDEFINES)
        LINTFLAGS = $(LINTOPTS) -DLINT $(ALLDEFINES)
           LDLIBS = $(SYS_LIBRARIES) $(EXTRA_LIBRARIES)
        LDOPTIONS = $(CDEBUGFLAGS) $(CCOPTIONS)
   LDCOMBINEFLAGS = -X -r

        MACROFILE = osf.cf
           RM_CMD = $(RM) *.CKP *.ln *.BAK *.bak *.o core errs ,* *~ *.a .emacs_* tags TAGS make.log MakeOut

    IMAKE_DEFINES =

         IRULESRC = $(CONFIGSRC)
        IMAKE_CMD = $(NEWTOP)$(IMAKE) -I$(NEWTOP)$(IRULESRC) $(IMAKE_DEFINES)

     ICONFIGFILES = $(IRULESRC)/Imake.tmpl $(IRULESRC)/Imake.rules \
			$(IRULESRC)/Project.tmpl $(IRULESRC)/site.def \
			$(IRULESRC)/$(MACROFILE) $(EXTRA_ICONFIGFILES)

###########################################################################
# AudioFile Build Parameters

###########################################################################
# AudioFile make variables; this need to be coordinated with rules

          PATHSEP = /
        USRLIBDIR = $(DESTDIR)/crl/audio/lib.alpha/
           BINDIR = $(DESTDIR)/crl/audio/bin.alpha/
          INCROOT = $(DESTDIR)/crl/audio/include
     BUILDINCROOT = $(TOP)
      BUILDINCDIR = $(BUILDINCROOT)/AF
      BUILDINCTOP = ..
           INCDIR = $(INCROOT)/AF
           ADMDIR = $(DESTDIR)/usr/adm
           LIBDIR = $(USRLIBDIR)/AF
        CONFIGDIR = $(LIBDIR)/config
       LINTLIBDIR = $(USRLIBDIR)/lint

          MANPATH = $(DESTDIR)/crl/audio/man/
    MANSOURCEPATH = $(MANPATH)/man
        MANSUFFIX = 1
     LIBMANSUFFIX = 3
           MANDIR = $(MANSOURCEPATH)1
        LIBMANDIR = $(MANSOURCEPATH)3
    SOAUDIOLIBREV = 0.0

           LODDIR = /usr/local/lib/AF/lodfiles
      DSPTOOLROOT = /usr/local

            IMAKE = $(IMAKESRC)/imake
           DEPEND = $(DEPENDSRC)/makedepend
        MKDIRHIER = /bin/sh $(SCRIPTSRC)/mkdirhier.sh

        CONFIGSRC = $(TOP)/config
        CLIENTSRC = $(TOP)/clients
          DEMOSRC = $(TOP)/demos
           LIBSRC = $(TOP)/lib
       INCLUDESRC = $(TOP)/AF
        SERVERSRC = $(TOP)/server
          UTILSRC = $(TOP)/util
        SCRIPTSRC = $(UTILSRC)/scripts
       EXAMPLESRC = $(TOP)/examples
       CONTRIBSRC = $(TOP)/contrib
           DOCSRC = $(TOP)/doc
        DEPENDSRC = $(UTILSRC)/makedepend
         IMAKESRC = $(CONFIGSRC)
         AFLIBSRC = $(LIBSRC)/AF
     AFUTILLIBSRC = $(LIBSRC)/AFUtil
     EXTENSIONSRC = $(TOP)/extensions

        UIDLIBDIR = $(USRLIBDIR)/uid
        MOTIFBASE = /usr/lib/DXM
           UILSRC = $(MOTIFBASE)/clients/uil
              UIL = $(UILSRC)/uil

     LINTAUDIOLIB = $(AFLIBSRC)/llib-lAF.ln
        AFLIBONLY = -L$(AFLIBSRC) -lAF
         DEPAFLIB = $(AFLIBSRC)/libAF.a
        AFUTILLIB = -L$(AFUTILLIBSRC) -lAFUtil
     DEPAFUTILLIB = $(AFUTILLIBSRC)/libAFUtil.a
  DEPEXTENSIONLIB = $(EXTENSIONSRC)/lib/libAFext.a
     EXTENSIONLIB = -L$(EXTENSIONSRC)/lib -lAFext
            AFLIB = $(EXTENSIONLIB) $(AFLIBONLY)

         DEPLIBS = $(LOCAL_LIBRARIES)
         DEPLIBS1 = $(DEPLIBS)
         DEPLIBS2 = $(DEPLIBS)
         DEPLIBS3 = $(DEPLIBS)

###########################################################################
# Imake rules for building libraries, programs, scripts, and data files
# rules:  $XConsortium: Imake.rules,v 1.67 89/12/18 17:14:15 jim Exp $

###########################################################################
# start of Imakefile

# Makefile for the AF manpages.
MANDIR = $(LIBMANDIR)
MANSUFFIX = $(LIBMANSUFFIX)

# Manual pages are currently listed twice. "Real" pages are listed
# MANPAGES and with an 'InstallManPage()' command.
# Aliases (i.e., multiple entries on the same page) are listed in
# an 'InstallManPageAliases()' command.

MANPAGES = AFAddHost.man \
	AFAudioConnName.man \
	AFCreateAC.man \
	AFCreatePhoneAC.man \
	AFDialPhone.man \
	AFEnableInput.man \
	AFEnablePassThrough.man \
	AFFlush.man \
	AFGetErrorText.man \
	AFGetProperty.man \
	AFGetTime.man \
	AFHookSwitch.man \
	AFIfEvent.man \
	AFInternAtom.man \
	AFNextEvent.man \
	AFNoOp.man \
	AFOpenAudioConn.man \
	AFPlaySamples.man \
	AFQueryInputGain.man \
	AFQueryPhone.man \
	AFRecordSamples.man \
	AFSelectEvents.man \
	AFSynchronize.man \
	AFree.man \
	AAudioDeviceDescriptor.man

# Note: no commas in the aliases list!

install.man:: AFAddHost.man
	$(INSTALL) -c $(INSTMANFLAGS) AFAddHost.man  $(MANDIR)/AFAddHost.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFAddHost.$(MANSUFFIX) > $${TMP}; \
	for i in  AFAddHosts AFListHosts AFRemoveHost AFRemoveHosts AFSetAccessControl AFEnableAccessControl AFDisableAccessControl  AFHostAddress; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFCreateAC.man
	$(INSTALL) -c $(INSTMANFLAGS) AFCreateAC.man  $(MANDIR)/AFCreateAC.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFCreateAC.$(MANSUFFIX) > $${TMP}; \
	for i in  AFChangeACAttributes AFFreeAC; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFOpenAudioConn.man
	$(INSTALL) -c $(INSTMANFLAGS) AFOpenAudioConn.man  $(MANDIR)/AFOpenAudioConn.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFOpenAudioConn.$(MANSUFFIX) > $${TMP}; \
	for i in  AFCloseAudioConn; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFAudioConnName.man
	$(INSTALL) -c $(INSTMANFLAGS) AFAudioConnName.man  $(MANDIR)/AFAudioConnName.$(MANSUFFIX)

install.man:: AFCreatePhoneAC.man
	$(INSTALL) -c $(INSTMANFLAGS) AFCreatePhoneAC.man  $(MANDIR)/AFCreatePhoneAC.$(MANSUFFIX)

install.man:: AFDialPhone.man
	$(INSTALL) -c $(INSTMANFLAGS) AFDialPhone.man  $(MANDIR)/AFDialPhone.$(MANSUFFIX)
#

install.man:: AFGetProperty.man
	$(INSTALL) -c $(INSTMANFLAGS) AFGetProperty.man  $(MANDIR)/AFGetProperty.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFGetProperty.$(MANSUFFIX) > $${TMP}; \
	for i in  AFListProperties AFChangeProperty AFDeleteProperty; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFGetTime.man
	$(INSTALL) -c $(INSTMANFLAGS) AFGetTime.man  $(MANDIR)/AFGetTime.$(MANSUFFIX)
#

install.man:: AFEnableInput.man
	$(INSTALL) -c $(INSTMANFLAGS) AFEnableInput.man  $(MANDIR)/AFEnableInput.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFEnableInput.$(MANSUFFIX) > $${TMP}; \
	for i in  AFDisableInput AFEnableOutput AFDisableOutput; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFFlush.man
	$(INSTALL) -c $(INSTMANFLAGS) AFFlush.man  $(MANDIR)/AFFlush.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFFlush.$(MANSUFFIX) > $${TMP}; \
	for i in  AFSync AFEventsQueued AFPending; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFGetErrorText.man
	$(INSTALL) -c $(INSTMANFLAGS) AFGetErrorText.man  $(MANDIR)/AFGetErrorText.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFGetErrorText.$(MANSUFFIX) > $${TMP}; \
	for i in  AFSetErrorHandler AFSetIOErrorHandler AFGetErrorDatabaseText; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFHookSwitch.man
	$(INSTALL) -c $(INSTMANFLAGS) AFHookSwitch.man  $(MANDIR)/AFHookSwitch.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFHookSwitch.$(MANSUFFIX) > $${TMP}; \
	for i in  AFFlashHook; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFNextEvent.man
	$(INSTALL) -c $(INSTMANFLAGS) AFNextEvent.man  $(MANDIR)/AFNextEvent.$(MANSUFFIX)
#

install.man:: AFIfEvent.man
	$(INSTALL) -c $(INSTMANFLAGS) AFIfEvent.man  $(MANDIR)/AFIfEvent.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFIfEvent.$(MANSUFFIX) > $${TMP}; \
	for i in  AFCheckIfEvent AFPeekIfEvent; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFInternAtom.man
	$(INSTALL) -c $(INSTMANFLAGS) AFInternAtom.man  $(MANDIR)/AFInternAtom.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFInternAtom.$(MANSUFFIX) > $${TMP}; \
	for i in  AFGetAtomName; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFNoOp.man
	$(INSTALL) -c $(INSTMANFLAGS) AFNoOp.man  $(MANDIR)/AFNoOp.$(MANSUFFIX)

install.man:: AFPlaySamples.man
	$(INSTALL) -c $(INSTMANFLAGS) AFPlaySamples.man  $(MANDIR)/AFPlaySamples.$(MANSUFFIX)
#

install.man:: AFQueryInputGain.man
	$(INSTALL) -c $(INSTMANFLAGS) AFQueryInputGain.man  $(MANDIR)/AFQueryInputGain.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFQueryInputGain.$(MANSUFFIX) > $${TMP}; \
	for i in  AFQueryOutputGain AFSetInputGain AFSetOutputGain; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFQueryPhone.man
	$(INSTALL) -c $(INSTMANFLAGS) AFQueryPhone.man  $(MANDIR)/AFQueryPhone.$(MANSUFFIX)

install.man:: AFRecordSamples.man
	$(INSTALL) -c $(INSTMANFLAGS) AFRecordSamples.man  $(MANDIR)/AFRecordSamples.$(MANSUFFIX)

install.man:: AFSelectEvents.man
	$(INSTALL) -c $(INSTMANFLAGS) AFSelectEvents.man  $(MANDIR)/AFSelectEvents.$(MANSUFFIX)
#

install.man:: AFEnablePassThrough.man
	$(INSTALL) -c $(INSTMANFLAGS) AFEnablePassThrough.man  $(MANDIR)/AFEnablePassThrough.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFEnablePassThrough.$(MANSUFFIX) > $${TMP}; \
	for i in  AFDisablePassThrough; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFSynchronize.man
	$(INSTALL) -c $(INSTMANFLAGS) AFSynchronize.man  $(MANDIR)/AFSynchronize.$(MANSUFFIX)

install.man::
	@(TMP=/tmp/tmp.$$$$; \
	$(RM) $${TMP}; \
	echo .so `basename  $(MANDIR)`/AFSynchronize.$(MANSUFFIX) > $${TMP}; \
	for i in  AFSetAfterFunction; do \
	   (set -x; \
	   $(INSTALL) -c $(INSTMANFLAGS) $${TMP}  $(MANDIR)/$$i.$(MANSUFFIX)); \
	done; \
	$(RM) $${TMP})
#

install.man:: AFree.man
	$(INSTALL) -c $(INSTMANFLAGS) AFree.man  $(MANDIR)/AFree.$(MANSUFFIX)
#

install.man:: AAudioDeviceDescriptor.man
	$(INSTALL) -c $(INSTMANFLAGS) AAudioDeviceDescriptor.man  $(MANDIR)/AAudioDeviceDescriptor.$(MANSUFFIX)

TROFF= groff
PRINTER= lpr
INSTALLFLAGS = $(INSTMANFLAGS)

all: # nothing to do

PS:
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(MANPAGES) ; \
	do \
	echo formatting $$i ; \
	tbl $$i  | $(TROFF) -man -t - > `basename $$i .3AF`.ps ; \
	done

onePS:
	tbl $(MANPAGES)  | $(TROFF) -man - > $(FILE).ps

techreport:
	tbl $(MANPAGES)  | $(TROFF) -man crltr intro.r - > AF-manual.ps

clean::
	rm -f *.PS *.ps

#

install.man:: $(MANPAGES)
	@case '${MFLAGS}' in *[i]*) set +e;; esac; \
	for i in $(MANPAGES); do \
		(set -x; $(INSTALL) -c $(INSTALLFLAGS) $$i $(LIBMANDIR)); \
	done

depend:

###########################################################################
# common rules for all Makefiles - do not edit

emptyrule::

clean::
	$(RM_CMD) "#"*

Makefile:: $(IMAKE)

$(IMAKE):
	@(cd $(IMAKESRC); if [ -f Makefile ]; then \
	echo "checking $@ in $(IMAKESRC) first..."; $(MAKE) all; else \
	echo "bootstrapping $@ from Makefile.ini in $(IMAKESRC) first..."; \
	$(MAKE) -f Makefile.ini BOOTSTRAPCFLAGS=$(BOOTSTRAPCFLAGS); fi; \
	echo "okay, continuing in $(CURRENT_DIR)")

Makefile::
	-@if [ -f Makefile ]; then \
		echo "	$(RM) Makefile.bak; $(MV) Makefile Makefile.bak"; \
		$(RM) Makefile.bak; $(MV) Makefile Makefile.bak; \
	else exit 0; fi
	$(IMAKE_CMD) -DTOPDIR=$(TOP) -DCURDIR=$(CURRENT_DIR)

tags::
	$(TAGS) -w *.[ch]
	$(TAGS) -xw *.[ch] > TAGS

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

