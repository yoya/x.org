#
#	$Header: Makefile,v 10.2 86/11/30 16:15:29 jg Rel $
#

R_OBJS = colors.o \
		coup.o \
		data.o \
		death.o \
		detonate.o \
		dmessage.o \
		enter.o \
		getship.o \
		inform.o \
		input.o \
		interface.o \
		main.o \
		newwin.o \
		orbit.o \
		phaser.o \
		planetlist.o \
		playerlist.o \
		pstats.o \
		redraw.o \
		smessage.o \
		startdaemon.o \
		stats.o \
		torp.o \
		util.o \
		warning.o

C_OBJS = colors.o \
		coup.o \
		data.o \
		death.o \
		detonate.o \
		dmessage.o \
		enter.o \
		getship.o \
		inform.o \
		input.o \
		interface.o \
		copilot.o \
		newwin.o \
		orbit.o \
		phaser.o \
		planetlist.o \
		playerlist.o \
		pstats.o \
		redraw.o \
		smessage.o \
		startdaemon.o \
		stats.o \
		torp.o \
		util.o \
		warning.o

D_OBJS =	daemon.o \
		data.o

r_OBJS =	data.o \
		detonate.o \
		enter.o \
		getship.o \
		interface.o \
		phaser.o \
		robot.o \
		rmove.o \
		startdaemon.o \
		torp.o \
		util.o

LIBS = ../Xlib/libX.a -lm

INCLUDES = -I../include
CFLAGS = -O $(INCLUDES)

all: xtrek daemon robot copilot

xtrek: $(R_OBJS)
	cc $(CFLAGS) -o xtrek $(R_OBJS) $(LIBS)
	chmod 4711 xtrek

daemon: $(D_OBJS)
	cc $(CFLAGS) -o daemon $(D_OBJS) -lm
	chmod 4711 daemon

robot: $(r_OBJS)
	cc $(CFLAGS) -o robot $(r_OBJS) $(LIBS)
	chmod 4711 robot

copilot: $(C_OBJS)
	cc $(CFLAGS) -o copilot $(C_OBJS) $(LIBS)
	chmod 4711 copilot

install: xtrek
	rm -rf ${DESTDIR}/usr/games/lib/xtrek
	-mkdir ${DESTDIR}/usr/games/lib/xtrek
	install -c -m 4711 xtrek ${DESTDIR}/usr/games
	install -c -m 4711 copilot ${DESTDIR}/usr/games
	install -c -m 4711 daemon ${DESTDIR}/usr/games/lib/xtrek
	install -c -m 4711 robot ${DESTDIR}/usr/games/lib/xtrek
	touch ${DESTDIR}/usr/games/lib/xtrek/.motd
	touch ${DESTDIR}/usr/games/lib/xtrek/.scores
	touch ${DESTDIR}/usr/games/lib/xtrek/.planets

clean:
	rm -f xtrek daemon robot copilot *.o *~ *.bak *.BAK

###
