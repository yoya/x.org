#	GNUmakefile : der Fuehrer der Uebersetzung

#	@(#) GNUmakefile Copyright (c) 1993 Christopher Alexander North-Keys
#	$Grueppe: Talisman $
#	$Anfang: Mon Jul  5 12:04:26 GMT 1993 $
#	$Source: /home/erlkonig/lib/xcms/GNUmakefile $
#	$State: $
#	$Revision: $
#	$Date: Mon Jul  5 12:04:26 GMT 1993 $
#	$Author: erlkonig $

#	principle: preprocess *.xcms with c++ preprocessor yielding *.xcmsdb file.

#	Sun Jun  5 10:44:59 MET DST 1994
#	Everything works.  Now we need to get it to work with gmake.  Bummer.

#	.xcms	Xcms pallette creation program/script
#	.col	Xcms color listing in multiple forms, RGBi, CIE*, TekHVC, etc.
#	.rgb	X Server-Format RGB component listing.
#	.xcmsdb	Xcmd client-side database for use with XCMSDB

#	for col2rgb
CC		=	gcc -g
LD		=	$(CC)
LDFLAGS		=	-L/vol/X11R6/lib
LDLIBS		=	-lXt -lSM -lICE -lXext -lX11
COL2RGB		=	./col2rgb
COLS2XCMSDB	=	./cols2cdb.sh
RGB_UNIQ	=	./uniqrgb.sh

RGB_PROG	=	./rgb
RGB_BASE	=	rgb
RGB_XCMSDB	=	$(RGB_BASE).xcmsdb
RGB_DBM		=	$(RGB_BASE).built
RGB_RGB		=	$(RGB_BASE).txt

GAMMA_FILE	=	talisman.dcc

SRCS		=	$(wildcard *.xcms) $(wildcard ./Others/*.xcms)
COLS		=	$(subst .xcms,.col,$(SRCS))
RGBS		=	$(subst .xcms,.rgb,$(SRCS))

%.col : %.xcms
	@echo $^ '->' $@ ; \
	$^ | sed 's@#.*$$@@g;/^[ 	]*$$/d' > $@

#	/vol/X11R6/xc/lib/X11/cmsColNm.c has somewhat dodgy format requirements
#	This code uses "%s\t\t%s\n" format.

%.rgb : %.col
	@echo $^ '->' $@
	XCMSDB=$(RGB_XCMSDB) ; export XCMSDB ; \
	cat $^ | $(COL2RGB) -rgbtxt | cat > $@

all :: gamma $(RGB_XCMSDB) $(COL2RGB) $(RGB_DBM)

gamma ::
	@echo 'note: upcoming "Could not find..." messages, if any,  are harmless.'
	-xcmsdb -remove 2>/dev/null
	xcmsdb $(GAMMA_FILE)

$(RGB_RGB) : $(RGBS)
	@echo $^ '->' $@ ; \
	cat $^ | $(RGB_UNIQ) > $@

$(RGB_DBM) : $(RGB_XCMSDB) $(RGB_RGB) $(RGB_PROG)
	@echo 'making data...' ; \
	xmkmf && make data && touch $@

$(COL2RGB) :
	@echo 'making $@...' ; \
	xmkmf && make $@

#	The nawk fragment below is to drop comments, indented stuff,
#	redundant color defs (drops latter).  Be aware that spaces are
#	not valid field delimiters, since they may be part of color names,
#	zB: "forest green"

$(RGB_XCMSDB) : $(COLS)
	@echo 'making $@...' ; \
	$(COLS2XCMSDB) $@ $^ > $@

$(RGB_PROG) :
	@echo 'making $@...' ; \
	xmkmf && make $@

install ::
	xmkmf && make install install.man

kempt ::
	@$(RM) *.BAK *.bak ,* .*~ *~ make.log core errs mon.out gmon.out

tidy :: kempt
	$(RM) $(COL2RGB_OBJS)

clean :: tidy
	$(RM) $(RGBS)
	$(RM) $(COLS)
	make clean

pure :: clean
	$(RM) $(RGB_XCMSDB) $(RGB_DBM) $(RGB_RGB) $(COL2RGB) Makefile

#-----------eof
