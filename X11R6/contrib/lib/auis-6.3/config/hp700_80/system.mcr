/* Copyright IBM Corporation 1988,1991 - All Rights Reserved */

/*
	$Disclaimer: 
# Permission to use, copy, modify, and distribute this software and its 
# documentation for any purpose is hereby granted without fee, 
# provided that the above copyright notice appear in all copies and that 
# both that copyright notice, this permission notice, and the following 
# disclaimer appear in supporting documentation, and that the names of 
# IBM, Carnegie Mellon University, and other copyright holders, not be 
# used in advertising or publicity pertaining to distribution of the software 
# without specific, written prior permission.
# 
# IBM, CARNEGIE MELLON UNIVERSITY, AND THE OTHER COPYRIGHT HOLDERS 
# DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
# ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO EVENT 
# SHALL IBM, CARNEGIE MELLON UNIVERSITY, OR ANY OTHER COPYRIGHT HOLDER 
# BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
# DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 
# WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS 
# ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
# OF THIS SOFTWARE.
#  $
*/

/* For full copyright information see:'andrew/config/COPYRITE' */

#define In_Imake 1
/* The next two lines need to be kept in sync */
#include <hp700_80/system.h>
        SYSTEM_H_FILE = hp700_80/system.h
#undef In_Imake

/* These next three lines help configure the embedded machine-dependent
    directories overhead/class/machdep, atk/console/stats, and
    atk/console/stats/common. */
        SYS_IDENT = hp700
        SYS_OS_ARCH = hp_pa_risc_80

/* Get parent inclusions */
#include <allsys.mcr>


/* Redefine for HP-UX 8.0 */
        XLIBDIR = /usr/lib/X11R5
        XINCDIR = /usr/include/X11R5
        XFC = /usr/bin/X11/bdftopcf
        XBINDIR = /usr/bin/X11
        XUTILDIR = /usr/local/bin

/* Now for the system-dependent information. */
  
        MALLOCALIGNMENT = 8
        CDEBUGFLAGS = +O1
        SHLIBLDFLAGS= -b -E

SUPLIBS = $(BASEDIR)/lib/libossup.a $(BASEDIR)/lib/libatkos.a

/* In order to handle all the defines needed for im.c */
        COMPILERFLAGS = +z -W p,-H1000000

        DYN_LINK_LIB = -ldld
        CLASS_ROUTINESTRUCT = class_RoutineStruct
        CURSESLIBS = -lcurses
        CLASSLIB = -L$(BASEDIR)/lib -lclass
        XLIB = -L$(XLIBDIR) -lX11

/* Get site-specific inclusions */
#include <site.mcr>
SYS_CONFDIR = hp700_80
