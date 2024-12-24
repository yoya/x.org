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
	CheckBox.o\
	MsgCommon.o\
	MsgItem.o\
	PageEdit.o\
	PushButton.o\
	RButton.o\
	RSelect.o\
	RasterEdit.o\
	SRaster.o\
	ScrollBar.o\
	StaticText.o\
	TextEdit.o\
	TitleBar.o\
	editorUtil.o\
	ellipUtil.o\
	gcUtil.o\
	ovalUtil.o\
	pixelExt.o\
	polyUtil.o\
	rectUtil.o\
	textUtil.o

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

CheckBox.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)


MsgCommon.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

MsgItem.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

PageEdit.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

PushButton.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

RButton.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

RSelect.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

RasterEdit.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

SRaster.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

ScrollBar.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

StaticText.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

TextEdit.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

TitleBar.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

editorUtil.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

ellipUtil.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

gcUtil.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

ovalUtil.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

pixelExt.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

polyUtil.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

rectUtil.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

textUtil.o:\
	$(INCRT)/Xr/types.h\
	$(INCRT)/Xr/defs.h\
	$(INCRT)/Xr/in_types.h\
	$(FRC)

FRC:
