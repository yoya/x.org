# Copyright 1985, Massachusetts Institute of Technology
# Makefile for the Toolkit documents.

TROFF= psroff
#TROFF= ditroff -Tps
PRINTER= lpr
FILES1= macros.t\
	tmac.mit\
	Xtk.widg.front\
	Xtk.widgets
FILES2= macros.t\
	tmac.mit\
	Xtk.intr.front\
	Xtk.intrinsics

all: widgets intrinsics

widgets: $(FILES1)
	rm -f widg.idx.raw widgets.PS widgets.idx.PS
	tbl $(FILES1) | ${TROFF} -t -ms 2> widg.idx.raw  > widgets.PS
	make index.widgets

intrinsics: $(FILES2)
	rm -f intr.idx.raw intrinsics.PS intrin.idx.PS
	tbl $(FILES2) | ${TROFF} -t -ms 2> intr.idx.raw  > \
		intrinsics.PS
	make index.intrinsics

preview:$(FILES)
	rm -f index.nraw
	neqn ${FILES} | tbl | nroff -ms 2> index.nraw | colcrt

index.raw:
	rm -f index.raw
	eqn ${FILES} | tbl | ${TROFF} -ms -t > /dev/null 2> index.raw|\
	${PRINTER}

index.widgets:
	rm -f widgets.idx.PS
	sort -f -t: +1 -3 +0n -1n widg.idx.raw | awk -f fixindex.awk \
		| awk -f block.awk > index.troff
	cat widg.idxmac.t index.troff | ${TROFF} -t -me > \
		widgets.idx.PS
	rm -f index.troff

index.intrinsics:	
	rm -f intrin.idx.PS
	sort -f -t: +1 -3 +0n -1n intr.idx.raw | awk -f fixindex.awk \
		| awk -f block.awk > index.troff
	cat intr.idxmac.t index.troff | ${TROFF} -t -me > \
		intrin.idx.PS
	rm -f index.troff

clean:
	rm -f *~ \#* *.bak errs index.traw index.nraw

get:
	co -q RCS/*

