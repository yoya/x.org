# Copyright 1985, 1986 Massachusetts Institute of Technology
# Makefile for the Installation document.

TROFF= psroff -t
FILES= 	in.1 \
	in.2 \
	in.3 \
	in.4 \
	in.6

all: in.psc

in.psc: $(FILES)
	eqn ${FILES} | tbl |${TROFF} -ms >in.psc 

preview:$(FILES)
	neqn ${FILES} | tbl | nroff -ms | colcrt


clean:
	rm -f *~ \#* *.bak errs 
