# You may have to redefine $(INCDIR) - it is the directory
# where the X11 include files are located. The usual place is /usr/include.
# Similarly, $(XLIBDIR) may need to be redefined - that's where
# the X libraries are supposed to be.
#
INCDIR = /usr/include
XLIBDIR = /usr/lib
CDEBUGFLAGS = -g
LDFLAGS = -s
CFLAGS = -I$(INCDIR) -DWideLabel -DKeepScale -DResize_Yes $(CDEBUGFLAGS)

xplot2.o: xplot2.c
	cc -c $(CFLAGS) xplot2.c -I$(INCDIR)
xplot2: xplot2.o
	cc $(CFLAGS) $(LDFLAGS) -L$(XLIBDIR) -o xplot2 xplot2.o\
			$(LDFLAGS) -lX11 -lm
rnxplot.o: rnxplot.c
	cc -c $(CFLAGS) rnxplot.c -I$(INCDIR)
rnxplot: rnxplot.o
	cc $(CFLAGS) $(LDFLAGS) -L$(XLIBDIR) -o rnxplot rnxplot.o\
			$(LDFLAGS) -lX11 -lm

XPOBJ = main.o openpl.o plotting.o readin.o plot.o alabel.o
xplot: $(XPOBJ)
	cc $(CFLAGS) $(LDFLAGS) -L$(XLIBDIR) -o xplot $(XPOBJ) -lX11 -lm

tarfile:
	 tar cvf tarfile ChangeLog Imakefile Makefile Makefile.simple README \
TODO alabel.c config.h main.c openpl.c patchlevel.h plot.c plotting.c \
readin.c test1 test2 test3 test4 test5 test6 testplot.c xplot.man

sharfile:
	 shar -a ChangeLog Imakefile Makefile Makefile.simple README \
TODO alabel.c config.h main.c openpl.c patchlevel.h plot.c plotting.c \
readin.c xplot.man > sharfile

# Following is the Imake stuff.
# -------------------------------------------------------------------------
# common rules for all Makefiles - do not edit

emptyrule::

clean::
	$(RM_CMD) "#"*

Makefile::
	-@if [ -f Makefile ]; then set -x; \
	rm Makefile.bak; mv Makefile Makefile.bak; \
	else exit 0; fi
	imake

tags::
	$(TAGS) -w *.[ch]
	$(TAGS) -xw *.[ch] > TAGS

saber:
	# load $(ALLDEFINES) $(SRCS)

osaber:
	# load $(ALLDEFINES) $(OBJS)

# -------------------------------------------------------------------------
# empty rules for directories that do not have SUBDIRS - do not edit

install::
	@echo "install in $(CURRENT_DIR) done"

install.man::
	@echo "install.man in $(CURRENT_DIR) done"

Makefiles::

includes::

# -------------------------------------------------------------------------
# dependencies generated by makedepend

