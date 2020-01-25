# Copyright 1985, Massachusetts Institute of Technology
# Makefile for the Xlib document.

TROFF= psroff
#TROFF= ditroff -Tpsc
PRINTER= lpr
FILES= macros.t\
	tmac.mit\
	intrinsics.rno

all: paper index

paper: $(FILES)
	tbl $(FILES) | ${TROFF} -t -ms 2> index.raw  > paper.psc

preview:$(FILES)
	neqn ${FILES} | tbl | nroff -ms 2> index.nraw | colcrt

index.raw:
	eqn ${FILES} | tbl | ${TROFF} -ms -t > /dev/null 2> index.raw|\
	${PRINTER}

index:	index.raw
	sort +1 +0n -1n index.raw | awk -f fixindex.awk |awk -f block.awk > index.troff
	cat indexmacros.t index.troff | ${TROFF} -t -me > index.psc
	rm -f index.troff

clean:
	rm -f *~ \#* *.bak errs index.traw index.nraw

get:
	co -q RCS/*


