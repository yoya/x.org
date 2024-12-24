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
	panelmgr.o\
	MsgBox.o\
	MenuEdit.o\
	MenuMgr.o

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

panelmgr.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)


MsgBox.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

MenuEdit.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

MenuMgr.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

FRC:
