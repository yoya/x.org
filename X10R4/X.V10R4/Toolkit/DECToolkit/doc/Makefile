PRINTER=ln01
FILTER=ln01
MACROS=-ms

nroff:	toolkit.ms macros.t
	tbl toolkit.ms | nroff $(MACROS) macros.t - >toolkit.nro

ditroff: toolkit.ms macros.t
	tbl toolkit.ms | ditroff -T$(FILTER) -t $(MACROS) macros.t - | lpr -n -P$(PRINTER)
