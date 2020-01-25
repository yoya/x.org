#
# Makefile for gsftopk.
#

# Please set the following paths.

# The following three should be as in dvips.
HEADERDIR=/usr/local/tex/lib/dvips
HEADERPATH=.:$(HEADERDIR)
CONFIGPATH=.:/usr/local/tex/lib/dvips

# The following should be as in TeX.
TEXFONTS_DEFAULT=.:/usr/local/tex/lib/fonts/tfm

# Where to put things.
BINDIR=/usr/local/tex/bin
MANDIR=/usr/man/man1

# (Optional) The fully-qualified path of Ghostscript.
# GSPATH=-DGS=\"/usr/local/gnu/bin/gs\"

# CC=gcc
# LINT=gcc -c -Wall -Wshadow -Wpointer-arith -Dlint
# also maybe -Wcast-qual -Wwrite-strings
RM=rm -f
INSTALL=install

CFLAGS=-O -DCONFIGPATH=\"$(CONFIGPATH)\" -DHEADERPATH=\"$(HEADERPATH)\" \
  -DTEXFONTS_DEFAULT=\"$(TEXFONTS_DEFAULT)\" $(GSPATH)

gsftopk: gsftopk.c config.h
	$(CC) -o gsftopk $(CFLAGS) gsftopk.c

config.h: Configure
	./Configure $(CC) $(CFLAGS)

install: gsftopk
	$(INSTALL) gsftopk $(BINDIR)
	$(INSTALL) -m 644 render.ps $(HEADERDIR)
	$(INSTALL) -m 644 gsftopk.1 $(MANDIR)

lint:
	$(LINT) $(CFLAGS) gsftopk.c

clean:
	$(RM) *.o gsftopk a.out core errs *~

veryclean:
	$(RM) *.o gsftopk config.h a.out core errs *~
