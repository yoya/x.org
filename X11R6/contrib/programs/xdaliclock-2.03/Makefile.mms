#	MMS Description file for building xdaliclock on VMS
#       Created for use with XDaliClock v2.00, DECWindows XUI (X11R3)
#
#       Please customize the xdaliclock.h entry to your local Time Zone
#       if you wish to use the UT/Local switch.
#
#	Written by: Rick Dyson 19-May-1994
#
#       Use with the following command:
#
#           MMS /Description = Makefile.mms

OPT = xdaliclock.opt
DEFS = /Define = ("BUILTIN_FONT", "BUILTIN_FONT_2")
INCS = /Include = ([.numbers], [.numbers2])
CFLAGS = $(CFLAGS) $(DEFS) $(INCS)
LINKFLAGS = $(LINKFLAGS)

OBJS = digital.obj colors.obj resources.obj visual.obj hsv.obj usleep.obj
OBJLIST = digital.obj,colors.obj,resources.obj,visual.obj,hsv.obj,usleep.obj

.first
	@ Define /NoLog Sys Sys$Library
	@ Define /NoLog X11 DECW$Include

xdaliclock.exe :	xdaliclock.obj $(OBJS)
	$(LINK) $(LINKFLAGS) /NoTraceBack xdaliclock,$(OBJLIST),$(OPT)/Option

xdaliclock.obj :	xdaliclock.c xdaliclock.h version.h defaults.h vroot.h
colors.obj :		xdaliclock.h
digital.obj :		xdaliclock.h

clean :
	- Purge
	- Delete *.obj;
