#
# 	$Source: /u1/Sx.new/code/RCS/Makefile,v $
#	$Header: Makefile,v 1.1 86/12/03 16:12:36 swick Exp $
#

#
# $Header: Makefile,v 1.1 86/12/03 16:12:36 swick Exp $ SPRITE (Berkeley)
#

MODULE = sx
ROOT = ../..
SPRITEHOME = /sprite/users/sprite
LIB =	  ${ROOT}/lib
H =	  ${LIB}/include
LINT =	  ${LIB}/lint
IFLAGS = -I${ROOT}/include
DBX = -O
PROFILE = -p -DPROFILE
PROFDIR=  ./Profiled
DFLAGS =
CFLAGS =  ${DBX} ${IFLAGS} ${DFLAGS}
PCFLAGS = ${DBX} ${IFLAGS} ${DFLAGS} ${PROFILE}
AFLAGS =  
CTFLAGS = -t
PPFLAGS = -E ${IFLAGS} ${DFLAGS}
#CC = cc -m68010
#AS = as -m68010
LD = ld

UPDATE = /usr/local/update
DEPEND = ${SPRITEHOME}/bin/depend.csh
DEPENDSCRIPT = ${SPRITEHOME}/lib/ex.depend

.SUFFIXES: .po

OTHEROBJS =
POTHEROBJS =

OBJS = list.o sxButton.o sxDispatch.o sxEntry.o sxMenu.o sxNotify.o sxPack.o sxScrollbar.o sxSelect.o sxTitle.o sxUtils.o xProcs.o

POBJS = list.po sxButton.po sxDispatch.po sxEntry.po sxMenu.po sxNotify.po sxPack.po sxScrollbar.po sxSelect.po sxTitle.po sxUtils.po xProcs.po

CSRCS = list.c sxButton.c sxDispatch.c sxEntry.c sxMenu.c sxNotify.c sxPack.c sxScrollbar.c sxSelect.c sxTitle.c sxUtils.c xProcs.c

ASMSRCS = 

STUBS = 

SRCS = ${CSRCS} ${ASMSRCS}

HDRS = XlibInternal.h sx.h sxInt.h
EXTERNHDRS = XlibInternal.h sx.h

LINTFLAGS = ${IFLAGS} -u
# Make sure to remove the current module from this list.
LINTLIBS = ${ROOT}/lib/lint/libc.ln
LINTSRCS = 
ALLSRCS = ${SRCS} ${HDRS} Makefile

${MODULE}.a: ${OBJS} ${OTHEROBJS}
	rm -f ${MODULE}.a
	ar cr ${MODULE}.a ${OBJS} ${OTHEROBJS}
	ranlib ${MODULE}.a


profile: ${PROFDIR}/${MODULE}.a

${PROFDIR}/${MODULE}.a : ${POBJS} ${POTHEROBJS}
	rm -f ${PROFDIR}/${MODULE}.a
	ar cr ${PROFDIR}/${MODULE}.a ${POBJS} ${POTHEROBJS}
	ranlib ${PROFDIR}/${MODULE}.a

tags: ${HDRS} ${CSRCS}
	rm -f tags
	ctags ${CTFLAGS} ${HDRS} ${CSRCS}

.c.po:
	rm -f $*.po
	-mv $*.o $*.so
	${CC} -c ${PCFLAGS} $*.c
	mv $*.o $*.po
	-mv $*.so $*.o
.c.o:
	rm -f $*.o
	${CC} ${CFLAGS} -c $*.c

.s.o:
	rm -f $*.o
	/lib/cpp ${PPFLAGS} $*.s > $*.pp
	${AS} ${AFLAGS} -o $*.o $*.pp
	rm -f $*.pp

# install: ${LIB}/${MODULE}.a installhdrs installsrc ${LINT}/${MODULE}.ln
install: ${LIB}/${MODULE}.a instlint

installhdrs:
	@${UPDATE} ${EXTERNHDRS} ${H}
		  
${LIB}/${MODULE}.a: ${MODULE}.a
	rm -f ${LIB}/${MODULE}.a
	cp ${MODULE}.a ${LIB}
	ar d ${LIB}/${MODULE}.a __.SYMDEF
	ranlib ${LIB}/${MODULE}.a

instlint: ${LINT}/${MODULE}.ln

${LINT}/${MODULE}.ln: ${CSRCS} ${HDRS}
	rm -f ${LINT}/${MODULE}.ln
	lint ${LINTFLAGS} -C${MODULE} ${CSRCS}
	mv llib-l${MODULE}.ln ${LINT}/${MODULE}.ln

lint:	${CSRCS} ${HDRS} ${LINTLIBS}
	rm -f lint
	lint ${LINTFLAGS} ${CSRCS} ${LINTLIBS} > lint

calls: ${CSRCS}
	/usr/local/calls ${IFLAGS} ${CSRCS} > calls

clean:
	rm -f lint *.o

depend :
	ex - Makefile < ${DEPENDSCRIPT}
	${DEPEND} ${IFLAGS} ${SRCS} >> Makefile

# The stuff below, to make sxDemo, isn't important in the Sx
# home directory at UCB, but becomes useful when a distribution
# directory (to which the sxDemo.c source has been added) is
# assembled.  "fixup" is used to recreate the cursors directory.

sxDemo: sxDemo.o sx.a
	${CC} ${CFLAGS} sxDemo.o sx.a -lX -o sxDemo

fixup:
	mkdir cursors
	cd cursors; ar x ../cursors.ar

# below is automatically generated
###     don't delete this line, make depend looks for it
list.o: ./list.h
list.o: ./sprite.h
list.o: list.c
sxButton.o: ./cursors/dot
sxButton.o: ./cursors/dotMask
sxButton.o: ./cursors/star
sxButton.o: ./cursors/starMask
sxButton.o: ./mem.h
sxButton.o: ./sprite.h
sxButton.o: ./string.h
sxButton.o: ./sx.h
sxButton.o: ./sxInt.h
sxButton.o: sxButton.c
sxDispatch.o: ./mem.h
sxDispatch.o: ./sprite.h
sxDispatch.o: ./sx.h
sxDispatch.o: sxDispatch.c
sxEntry.o: ./caret.bits
sxEntry.o: ./caretMask.bits
sxEntry.o: ./char.h
sxEntry.o: ./cursors/ptr
sxEntry.o: ./mem.h
sxEntry.o: ./sprite.h
sxEntry.o: ./string.h
sxEntry.o: ./sx.h
sxEntry.o: ./sxInt.h
sxEntry.o: sxEntry.c
sxMenu.o: ./cursors/dot
sxMenu.o: ./cursors/dotMask
sxMenu.o: ./cursors/star
sxMenu.o: ./cursors/starMask
sxMenu.o: ./mem.h
sxMenu.o: ./sprite.h
sxMenu.o: ./string.h
sxMenu.o: ./sx.h
sxMenu.o: ./sxInt.h
sxMenu.o: sxMenu.c
sxNotify.o: ./char.h
sxNotify.o: ./cursors/dot
sxNotify.o: ./cursors/dotMask
sxNotify.o: ./io.h
sxNotify.o: ./notify.bits
sxNotify.o: ./sprite.h
sxNotify.o: ./string.h
sxNotify.o: ./sx.h
sxNotify.o: ./sxInt.h
sxNotify.o: ./varg.h
sxNotify.o: sxNotify.c
sxPack.o: ./list.h
sxPack.o: ./mem.h
sxPack.o: ./sprite.h
sxPack.o: ./sx.h
sxPack.o: ./sxInt.h
sxPack.o: sxPack.c
sxScrollbar.o: ./cursors/down
sxScrollbar.o: ./cursors/downMask
sxScrollbar.o: ./cursors/horiz
sxScrollbar.o: ./cursors/horizMask
sxScrollbar.o: ./cursors/left
sxScrollbar.o: ./cursors/leftMask
sxScrollbar.o: ./cursors/right
sxScrollbar.o: ./cursors/rightMask
sxScrollbar.o: ./cursors/thumbH
sxScrollbar.o: ./cursors/thumbHMask
sxScrollbar.o: ./cursors/thumbV
sxScrollbar.o: ./cursors/thumbVMask
sxScrollbar.o: ./cursors/up
sxScrollbar.o: ./cursors/upMask
sxScrollbar.o: ./cursors/vert
sxScrollbar.o: ./cursors/vertMask
sxScrollbar.o: ./mem.h
sxScrollbar.o: ./sprite.h
sxScrollbar.o: ./sx.h
sxScrollbar.o: sxScrollbar.c
sxSelect.o: ./XlibInternal.h
sxSelect.o: ./byte.h
sxSelect.o: ./mem.h
sxSelect.o: ./sprite.h
sxSelect.o: ./string.h
sxSelect.o: ./sx.h
sxSelect.o: sxSelect.c
sxTitle.o: ./mem.h
sxTitle.o: ./sprite.h
sxTitle.o: ./string.h
sxTitle.o: ./sx.h
sxTitle.o: ./sxInt.h
sxTitle.o: sxTitle.c
sxUtils.o: ./shadow.bits
sxUtils.o: ./sprite.h
sxUtils.o: ./sx.h
sxUtils.o: ./sxInt.h
sxUtils.o: sxUtils.c
xProcs.o: ./XlibInternal.h
xProcs.o: ./sxInt.h
xProcs.o: xProcs.c
