# Copyright 1985, 1986 Massachusetts Institute of Technology
# Makefile for the Installation document.

TROFF= psroff -t
FILES= 	in.1 in.2 in.3 in.4 in.5 in.6 in.7

all: notes.PS

notes.doc:  $(FILES)
	neqn ${FILES} | tbl | nroff -ms >$@

notes.PS: $(FILES)
	eqn $(FILES) | tbl | $(TROFF) -ms >$@	

preview:$(FILES)
	neqn ${FILES} | tbl | nroff -ms | colcrt


clean:
	rm -f *~ \#* *.bak errs 
