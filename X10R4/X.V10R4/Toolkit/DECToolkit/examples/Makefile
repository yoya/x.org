INCLUDE=-I../include/X
#XLIB=-lX
XLIB=../../../Xlib/libX.a
CFLAGS= -O -s
LIBTOOL=../src/libTool.a

all:	booltest comtest formtest hpanetest label scrolltest valtest vpanetest\
	TextAppendSource TextDiskSource TextStringSource



booltest: booltest.c
	cc -o booltest $(CFLAGS) $(INCLUDE) booltest.c $(LIBTOOL) $(XLIB)

comtest: comtest.c
	cc -o comtest $(CFLAGS) $(INCLUDE) comtest.c $(LIBTOOL) $(XLIB)

formtest: formtest.c
	cc -o formtest $(CFLAGS) $(INCLUDE) formtest.c $(LIBTOOL) $(XLIB)

hpanetest: hpanetest.c
	cc -o hpanetest $(CFLAGS) $(INCLUDE) hpanetest.c $(LIBTOOL) $(XLIB)

label: label.c
	cc -o label $(CFLAGS) $(INCLUDE) label.c $(LIBTOOL) $(XLIB)

scrolltest: scrolltest.c
	cc -o scrolltest $(CFLAGS) $(INCLUDE) scrolltest.c $(LIBTOOL) $(XLIB)

valtest: valtest.c
	cc -o valtest $(CFLAGS) $(INCLUDE) valtest.c $(LIBTOOL) $(XLIB)

vpanetest: vpanetest.c
	cc -o vpanetest $(CFLAGS) $(INCLUDE) vpanetest.c $(LIBTOOL) $(XLIB)

TextAppendSource: TextAppendSource.c
	cc -o TextAppendSource $(CFLAGS) $(INCLUDE) TextAppendSource.c $(LIBTOOL) $(XLIB)

TextDiskSource: TextDiskSource.c
	@rm -f disksource.out
	@cp /etc/passwd disksource.out
	cc -o TextDiskSource $(CFLAGS) $(INCLUDE) TextDiskSource.c $(LIBTOOL) $(XLIB)

TextStringSource: TextStringSource.c
	cc -o TextStringSource $(CFLAGS) $(INCLUDE) TextStringSource.c $(LIBTOOL) $(XLIB)
