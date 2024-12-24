#   
CCROOT=/
CC=$(CCROOT)/bin/cc
AS=$(CCROOT)/bin/as
AR=$(CCROOT)/bin/ar
LD=$(CCROOT)/bin/ld

INCRT = ../../include
LIBNAME = ../libXr.a
FRC =

FILES =\
	calc.o\
	editor.o\
	globals.o\
	group.o\
	init.o\
	input.o\
	inputTable.o\
	inputlib.o\
	resource.o\
	tree.o\
	utilities.o

all:	$(LIBNAME) 

.PRECIOUS:	$(LIBNAME)

$(LIBNAME):	$(FILES)

.s.a:
	$(AS) -o $*.o $<

.c.o:
	$(CC) -c -I$(INCRT) $(CFLAGS) -D$(OS) $<
	$(AR) rlv $(LIBNAME) $*.o

clean:
	-rm -f *.o

clobber:	clean
	-rm -f $(LIBNAME)

calc.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)


editor.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

globals.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

group.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

init.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

input.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

inputTable.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

inputlib.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

resource.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

tree.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

utilities.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

FRC:
