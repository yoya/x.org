# Copyright 1985, Massachusetts Institute of Technology
# Makefile for the Xlib document.

TROFF= psroff
#TROFF= ditroff -Tpsc
PRINTER= lpr
FILES= macros.t\
	abstract.t\
	credits.t\
	ch[01][0123456789]\
	appendixA\
	appendixB\
	appendixC\
	appendixD\
	glossary\
	postproc

XLIBDOC = 
XLIBMAN = Xman
SUBDIRS = \
	$(XLIBMAN)

all: paper index

paper: $(FILES)
	eqn ${FILES} | tbl |${TROFF} -t -ms 2> index.raw  > paper.psc

preview:$(FILES)
	neqn ${FILES} | tbl | nroff -ms 2> index.nraw | colcrt

index.raw:
	eqn ${FILES} | tbl | ${TROFF} -ms -t > /dev/null 2> index.raw|\
	${PRINTER}

index:	index.raw
	sort +1 +0n -1n index.raw | awk -f fixindex.awk |awk -f block.awk > index.troff
	cat indexmacros.t index.troff | ${TROFF} -t -me > index.psc
	rm -f index.troff

install:
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "installing $$i"; \
			$(MAKE) $(MFLAGS) \
				INSTALL='$(INSTALL)' \
				DESTDIR='$(DESTDIR)' install ); \
	done

clean:
	rm -f *~ \#* *.bak errs index.traw index.nraw

get:
	co -q RCS/*

buildtree:
	mkdir Xsrc Xman Xdescrip
	(cp -r ${WHEREAMI}/Xsrc ./Xsrc )
	(cp -r ${WHEREAMI}/Xman ./Xman )
	(cp -r ${WHEREAMI}/Xdescrip ./XXdescrip )
	@echo ${WHEREAMI} `pwd`



