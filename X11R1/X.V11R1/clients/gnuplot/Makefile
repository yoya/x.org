
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
           MAKE = make
    CDEBUGFLAGS = -O
        DESTDIR =

        INSTALL = install
           TAGS = ctags
         DEPEND = $(DEPENDSRC)/makedepend
          IMAKE = $(IMAKESRC)/imake
            RGB = $(RGBSRC)/rgb
         CFLAGS = $(CDEBUGFLAGS) $(INCLUDES) $(DEFINES)
        LDFLAGS = $(CDEBUGFLAGS) $(SYS_LIBRARIES)
   INSTLIBFLAGS = -c -m 0664
      USRLIBDIR = $(DESTDIR)/usr/lib
         BINDIR = $(DESTDIR)/usr/bin/X11
         LIBDIR = $(DESTDIR)/usr/lib/X11
         INCDIR = $(DESTDIR)/usr/include/X11
   FUTUREINCDIR = $(DESTDIR)/usr/include/X
        FONTDIR = $(DESTDIR)$(LIBDIR)/fonts
         MANDIR = $(DESTDIR)/usr/man/mann
      CLIENTSRC = $(TOP)/clients
        DEMOSRC = $(TOP)/demos
         LIBSRC = $(TOP)/lib
        FONTSRC = $(TOP)/fonts
     INCLUDESRC = $(TOP)/include
      SERVERSRC = $(TOP)/server
        UTILSRC = $(TOP)/util
        HACKSRC = $(TOP)/hacks
         DOCSRC = $(TOP)/doc
      DEPENDSRC = $(UTILSRC)/makedepend
       IMAKESRC = $(UTILSRC)/imake
         RGBSRC = $(UTILSRC)/rgb
        XLIBSRC = $(LIBSRC)/X
         XRMSRC = $(LIBSRC)/Xrm
     TOOLKITSRC = $(LIBSRC)/Xtk
     OLDXLIBSRC = $(LIBSRC)/oldX
    OLDXMENUSRC = $(LIBSRC)/oldXMenu
      OLDXRMSRC = $(LIBSRC)/oldXrm
  OLDTOOLKITSRC = $(LIBSRC)/oldXtk
   EXTENSIONSRC = $(TOP)/extensions
        XMANSRC = $(DOCSRC)/Xlib/Xman
   EXTENSIONLIB = $(EXTENSIONSRC)/lib/libXext.a
           XLIB = $(XLIBSRC)/libX.a
        OLDXLIB = $(OLDXLIBSRC)/liboldX.a
       XTOOLLIB = $(TOOLKITSRC)/libXtk.a
    OLDXTOOLLIB = $(OLDTOOLKITSRC)/liboldXtk.a
    OLDXMENULIB = $(OLDXMENUSRC)/libXMenu.a
         XRMLIB = $(XRMSRC)/libXrm.a
      OLDXRMLIB = $(OLDXRMSRC)/liboldXrm.a
       INCLUDES = -I$(TOP)
      MACROFILE = Vax.macros
      IMAKE_CMD = $(NEWTOP)$(IMAKE) -TImake.tmpl \
			-I$(NEWTOP)$(UTILSRC)/imake.includes \
			-s Makefile
         RM_CMD = rm -f *.CKP *.ln *.BAK *.bak *.o core errs ,* *~ *.a \
			tags TAGS make.log

LOCAL_LIBRARIES = $(XLIB)

# -lplot if you have -DUNIXPLOT

  SYS_LIBRARIES = -lnm -lplot

SRCS =	command.c eval.c graphics.c internal.c misc.c parse.c help.c\
	plot.c scanner.c standard.c term.c util.c version.c x11term.c\
	unix-fns.c obstack.c

OBJS = 	command.o eval.o graphics.o internal.o misc.o parse.o plot.o\
	scanner.o standard.o term.o util.o x11term.o unix-fns.o help.o\
	obstack.o version.o

# -DFORK if you have vfork()
# -DBCOPY if your memcpy() is called bcopy() (a Berkeleyism, right?)
# -DNOCOPY if you don't have a memcpy() by any name
# -DGAMMA if you've got gamma(3)
# -DOLD_HELP if you want to use the old help system,
#       which as far as I can tell doesn't work.
# -O if you trust your compiler's optimizer
# -ma (???) for RT PC, to make alloca() work correctly.
# -D<terminal> in TERMFLAGS iff you wish to support <terminal>
# -DAED         AED 512 and AED 767
# -DBITGRAPH    BBN BitGraph
# -DHP26        HP2623A and maybe others
# -DHP75        HP7580, and probably other HPs
# -DPOSTSCRIPT  Postscript
# -DQMS         QMS/QUIC laserprinter (Talaris 1200 and others)
# -DREGIS       ReGis graphics (vt125, vt220, vt240, Gigis...)
# -DSELANAR     Selanar
# -DTEK         Tektronix 4010, and probably others
# -DUNIXPLOT    unixplot
# -DV384        Vectrix 384 and tandy color printer

DEFINES = -DVFORK -DBCOPY -DGAMMA -DTEK -DPOSTSCRIPT -DXWINDOW

        PROGRAM = gnuplot

all:: gnuplot

gnuplot: $(OBJS) $(LOCAL_LIBRARIES)
	rm -f $@
	$(CC) -o $@ $(OBJS) $(LOCAL_LIBRARIES) $(LDFLAGS)

relink:
	rm -f $(PROGRAM)
	$(MAKE) $(MFLAGS) $(PROGRAM)

install:: gnuplot
	$(INSTALL) -c $(INSTALLFLAGS) gnuplot $(BINDIR)

install:: gnuplot.man
	$(INSTALL) -c -m 0664 gnuplot.man $(MANDIR)/gnuplot.n

depend::
	$(DEPEND) -s "# DO NOT DELETE" $(CFLAGS) $(SRCS)
clean::
	rm -f $(PROGRAM)

clean::
	$(RM_CMD) \#*

Makefile:: Imakefile \
	$(UTILSRC)/imake.includes/Imake.tmpl \
	$(UTILSRC)/imake.includes/Imake.rules \
	$(UTILSRC)/imake.includes/$(MACROFILE)
		-rm -f Makefile.bak; mv Makefile Makefile.bak
		$(IMAKE_CMD) -DTOPDIR=$(TOP)

tags::
	$(TAGS) -w *.[ch]
	$(TAGS) -xw *.[ch] > TAGS

install::
	@echo "install done"

Makefiles::

# DO NOT DELETE

command.o: /usr/include/stdio.h /usr/include/math.h plot.h
eval.o: /usr/include/stdio.h plot.h
graphics.o: /usr/include/stdio.h /usr/include/math.h plot.h
internal.o: /usr/include/math.h /usr/include/stdio.h plot.h
misc.o: /usr/include/stdio.h plot.h
parse.o: /usr/include/stdio.h /usr/include/setjmp.h /usr/include/signal.h
parse.o: /usr/include/errno.h plot.h
help.o: /usr/include/stdio.h /usr/include/ctype.h plot.h obstack.h
plot.o: /usr/include/stdio.h /usr/include/setjmp.h /usr/include/signal.h
plot.o: plot.h
scanner.o: /usr/include/stdio.h /usr/include/ctype.h plot.h
standard.o: /usr/include/math.h /usr/include/stdio.h plot.h
term.o: /usr/include/stdio.h plot.h
util.o: /usr/include/ctype.h /usr/include/setjmp.h /usr/include/stdio.h
util.o: /usr/include/errno.h plot.h
x11term.o: /usr/include/stdio.h ./../../X11/Xlib.h /usr/include/sys/types.h
x11term.o: ./../../X11/X.h ./../../X11/Xutil.h /usr/include/fcntl.h
x11term.o: /usr/include/signal.h obstack.h plot.h
unix-fns.o: /usr/include/math.h /usr/include/errno.h
obstack.o: obstack.h
