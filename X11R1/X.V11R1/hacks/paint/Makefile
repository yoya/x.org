
#
# This makefile is automatically generated by imake... do not modify
# or you may lose your changes.
#

            TOP = ./../..
             AS = as
             CC = cc
            CPP = /lib/cpp
             LD = ld
           MAKE = make
    CDEBUGFLAGS = -O
        DESTDIR =

        INSTALL = install
           TAGS = ctags
         DEPEND = $(DEPENDSRC)/makedepend
          IMAKE = $(IMAKESRC)/imake
         CFLAGS = $(CDEBUGFLAGS) $(INCLUDES) $(DEFINES)
        LDFLAGS = $(CDEBUGFLAGS) $(SYS_LIBRARIES)
      USRLIBDIR = $(DESTDIR)/usr/lib
         BINDIR = $(DESTDIR)/usr/bin/X11
         LIBDIR = $(DESTDIR)/usr/lib/X11
         INCDIR = $(DESTDIR)/usr/include/X11
        FONTDIR = $(DESTDIR)$(LIBDIR)/fonts
      CLIENTSRC = $(TOP)/clients
      DEPENDSRC = $(UTILSRC)/makedepend
       IMAKESRC = $(UTILSRC)/imake
        FONTSRC = $(TOP)/fonts
     INCLUDESRC = $(TOP)/include
      SERVERSRC = $(TOP)/server
     TOOLKITSRC = $(TOP)/toolkit
        UTILSRC = $(TOP)/util
        XLIBSRC = $(TOP)/Xlib
       XMENUSRC = $(TOP)/XMenu
         X11LIB = $(XLIBSRC)/libX11.a
           XLIB = $(X11LIB)
         X10LIB = $(XLIBSRC)/libX10.a
       XTOOLLIB = $(TOOLKITSRC)/libXt11.a
       INCLUDES = -I$(TOP)
      MACROFILE = Vax.macros
      IMAKE_CMD = $(NEWTOP)$(IMAKE) \
			-I$(NEWTOP)$(UTILSRC)/imake.includes \
			-s Makefile
         RM_CMD = rm -f *.CKP *.BAK *.bak *.o core errs ,* *~ *.a make.log

LOCAL_LIBRARIES = $(XLIB)
  SYS_LIBRARIES = -lm
           SRCS = main.c util.c
           OBJS = main.o util.o

        PROGRAM = paint

paint: $(OBJS) $(LOCAL_LIBRARIES)
	rm -f $@
	$(CC) -o $@ $(OBJS) $(LOCAL_LIBRARIES) $(LDFLAGS)

relink:
	rm -f $(PROGRAM)
	$(MAKE) $(MFLAGS) $(PROGRAM)

install:: paint
	$(INSTALL) -c $(INSTALLFLAGS) paint  $(BINDIR)

depend::
	$(DEPEND) -s "# DO NOT DELETE" $(CFLAGS) $(SRCS)
clean::
	rm -f $(PROGRAM)

clean::
	$(RM_CMD) \#*

Makefile:: Imakefile \
	$(UTILSRC)/imake.includes/Imake.template \
	$(UTILSRC)/imake.includes/Imake.rules \
	$(UTILSRC)/imake.includes/$(MACROFILE)
		$(IMAKE_CMD) -DTOPDIR=$(TOP)

tags::
	$(TAGS) -w *.[ch]
	$(TAGS) -xw *.[ch] > TAGS

install::
	@echo "install done"

Makefiles::

# DO NOT DELETE

main.o: paint.h /usr/include/stdio.h ./../../X11/Xlib.h
main.o: /usr/include/sys/types.h ./../../X11/X.h ./../../X11/Xatom.h
util.o: paint.h /usr/include/stdio.h ./../../X11/Xlib.h
util.o: /usr/include/sys/types.h ./../../X11/X.h ./../../X11/Xatom.h
