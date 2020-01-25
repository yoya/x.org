$!  A VMS command procedure to build XDaliClock (v2.01)
$!  Created by MMS from the Makefile.MMS description file.
$!
$!  Rick Dyson (dyson@IowaSP.Physics.UIowa.EDU)  23-MAY-1994
$!
$ Define /NoLog Sys Sys$Library
$ Define /NoLog X11 DECW$Include
$ CC /NOLIST/OBJECT=XDALICLOCK.OBJ /Define = ("BUILTIN_FONT", "BUILTIN_FONT_2") /Include = ([.numbers], [.numbers2]) XDALICLOCK.C
$ CC /NOLIST/OBJECT=DIGITAL.OBJ /Define = ("BUILTIN_FONT", "BUILTIN_FONT_2") /Include = ([.numbers], [.numbers2]) DIGITAL.C
$ CC /NOLIST/OBJECT=COLORS.OBJ /Define = ("BUILTIN_FONT", "BUILTIN_FONT_2") /Include = ([.numbers], [.numbers2]) COLORS.C
$ CC /NOLIST/OBJECT=RESOURCES.OBJ /Define = ("BUILTIN_FONT", "BUILTIN_FONT_2") /Include = ([.numbers], [.numbers2]) RESOURCES.C
$ CC /NOLIST/OBJECT=VISUAL.OBJ /Define = ("BUILTIN_FONT", "BUILTIN_FONT_2") /Include = ([.numbers], [.numbers2]) VISUAL.C
$ CC /NOLIST/OBJECT=HSV.OBJ /Define = ("BUILTIN_FONT", "BUILTIN_FONT_2") /Include = ([.numbers], [.numbers2]) HSV.C
$ CC /NOLIST/OBJECT=USLEEP.OBJ /Define = ("BUILTIN_FONT", "BUILTIN_FONT_2") /Include = ([.numbers], [.numbers2]) USLEEP.C
$ LINK /TRACE/NOMAP/EXEC=XDALICLOCK.EXE /NoTraceBack xdaliclock,digital.obj,colors.obj,resources.obj,visual.obj,hsv.obj,usleep.obj,xdaliclock.opt/Option
$ Exit
