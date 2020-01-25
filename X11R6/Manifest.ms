.\" Contrib manifest  -*- Nroff -*-
.\" $XConsortium: Manifest.ms,v 1.6 94/06/14 13:54:54 gildea Exp $
.\" #! troff -ms
.nr PS 11
.if n \{
.nr LL 72n
.na
.\}
.			\" .DH - directory heading
.\" .DH level name
.de DH
.nr DL \\$1
.ds D\\n(DL \\$2
.\" create the section name
.ds SN \\*(D1
.if \\n(DL-1 .as SN /\\*(D2
.if \\n(DL-2 .as SN /\\*(D3
.if \\n(DL-3 .as SN /\\*(D4
.if \\n(DL-4 .as SN /\\*(D5
.SH
.if 2-\\n(DL .LG	\" make level 1 headings larger
\\*(SN
.NL
.LP
..
.			\" AT - apropos table
.de AT
.ta 20n
.nf
.\" end table with .fi
..

.\" move page number to bottom
.ds CF - \\n(PN -
.ds CH \"

.ce 100
\s+2\fBX Window System, Version 11, Release 6\fP\s0

\s+2\fBContrib Manifest\fP\s0
.ce 0
.sp .5i
Copyright \(co\ 1994 X Consortium
.nr PS -2
.nr VS -2
.LP
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
\*QSoftware\*U), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:
.LP
The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.
.LP
THE SOFTWARE IS PROVIDED \*QAS IS\*U, WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
.LP
Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.
.nr PS +2
.nr VS +2
.NL
.LP
\fIX Window System\fP is a trademark of X Consortium, Inc.
.sp 3
.LP
This document is a walk-through of the directories of the X11R6
contrib part.
.LP
Where programs have provided a top-level \fI.man\fP file, the
line that \fIapropos\fP(1) would extract from that file is shown.
For other packages, if there is a succinct description of
the software near the top of a \fIREADME\fP file, that is given here.
.LP
The descriptions of the software given here are simply how
the software describes itself; the X Consortium has not tested that
the software performs as described.
.LP
We do not claim to have built or tested any of this software.  We do
not know what hardware (if any) it will work on.  All questions
regarding contributed software should be directed to the authors, not
to the X Consortium.
.LP
You will need approximately 175 megabytes of storage to unpack the
entire R6 contrib part.
See the Release Notes for general guidelines on unpacking the
distribution and building contrib software.
.LP
Patches to some of this software can usually be found in the directory
pub/R6/contrib-fixes on ftp.x.org or your local X mirror site, using
anonymous FTP.
Entirely new versions of some packages may be found in the appropriate
subdirectory of contrib on ftp.x.org or your local X mirror site.
.LP
The X Consortium thanks everyone who contributed software to Release 6.
.sp 2
.LP
And now the software, directory by directory:
.DH 1 doc
documentation on various items:
.AT
CLX	Common Lisp interface to X
papers/tutorials	various papers, including the comp.windows.x FAQ
PHIGS	PHIGS interface to PEX
Xserver	X server internals
.fi

.DH 1 examples
Sample programs for Fresco.

.DH 1 extensions
.DH 2 XTrap
An extension to X to facilitate user emulation.
Trap device input and graphics output to record and playback a user session.
.DH 2 screensaver
a hack screen saver extension

.DH 1 fonts
.AT
8x13-2	8x13 font with ISO Latin 2 encoding
ter7x14	terminal emulator 7x14 font
ter7x14B	terminal emulator 7x14 bold font
.fi

.DH 1 lib
.DH 2 AF
A device-independent network-transparent system including 
client applications and audio servers.
.DH 2 auis-6.3
The Andrew Messaging System, the Andrew Toolkit.
.DH 2 CLX
Common Lisp X interface library.
.DH 2 DPS
Client libraries and utilities for writing applications that use the Display
PostScript extension to X.  Includes the pswrap preprocessor.
.DH 2 IMdkit
Input Method server developers kit.
.DH 2 nas-1.2
Network Audio System, a
network-transparent, client/server audio system.
.DH 2 pcthreads
POSIX pthreads to MACH cthreads ininterface routines
.DH 2 PHIGS
3D graphics library interface to PEX using the PHIGS bindings.
.DH 2 tcl
An embeddable tool command language.
.DH 2 Ximp
Xlib internationalization support.
.DH 2 xpm-3.4c
Support for X Pixmap files.

.DH 1 programs
.AT
a2x	converts ASCII into X device events
AnswerGarden	organizational memory program
auto_box	display a rotating cube using PHIGS
beach_ball	display a bouncing sphere using PHIGS
Canna32	yet another Japanese input system
cmodext	control modifier extension
crock	one or two player interactive fighting game
ctwm-3.2	Tab Window Manager for the X Window System
cxterm	Chinese terminal emulator for X
Dclock	digital clock for X
dish	Tcl application allowing invocation of Fresco objects
dox	call Xlib function
emu	X based extensible terminal emulator
fdraw	graphics editor supporting instancing and embedding widgets
gpc/plbpex	picture level benchmark program
hterm	terminal emulator for X
i2mif	IDL to FrameMaker Interchange Format, generate docs from IDL
ico	animate an icosahedron or other polyhedron
ImageMagick	display and interactive manipulation of images
iman	development toolkit, window manager, and help server
ixx	IDL compiler, generates C++
kgames	a collection of games, and some useful widgets
kinput2	an input server for easy input of Japanese text
kterm-6.1.0	kanji terminal emulator for X
listres	list resources in widgets
MacFS	font server for Macintosh
maze	demo repeatedly creating and solving a random maze
mazewar	distributed rats in a maze
mterm	terminal emulator for X
ora-examples	O'Reilly and Associates examples
pixmap	Xpm pixmap editor for X
pubdic+	dictionary for kana-to-kanji conversion
puzzle	15-puzzle game for X
rr	collection of X11 clients
showfont	font dumper for X font server
sj3	Japanese input system
sjxa	Japanese Input Method Server for X Window System
timex	a manager for time lists
tkpostage-1.3a	fancy mailbox message counter (like xbiff)
tvtwm	Tom's Virtual Tab Window Manager
viewres	graphical class browser for Xt
xTeXcad.13	a graphic editor for portable LaTeX diagrams
xancur-1.6	root cursor animation program for X
xbiff	mailbox flag for X
xboing2.2	An X Window System based blockout clone. V2.2
xbuffy3	yet another biff for the X Window System
xcalc	scientific calculator for X
xcalendar	calendar with a notebook for X11
xcmdmenu-1.6	command menu utility for X
xcms/col2rgb	convert colors to RGB:0/0/0 or RGB.TXT format
xco	display X11 color names and colors 
xcolorize	start X client with pleasant background color
xcolors	display all X11 color names and colors
xdaliclock-2.03	melting digital clock
xdiary	interactive daily diary and calendar for X
xditview	display ditroff output
xdu	display the output of "du" in an X window
xdvi	program for previewing .dvi files
xdvorak	Dvorak keyboard layout for X11.
xearth-0.92	shaded image of the Earth in the root window
xed	a text editor for X
xedit	simple text editor for X
xev	print contents of X events
xexit-1.0	terminate all clients of a X session and exit
xeyes	a follow the mouse X demo
xfed	font editor for X Window System BDF font files
xfedor	bdf/xbm/xpm/mouse editor
xfeoak	Japanese Language Input Server
xfontsel	point and click selection of X11 font names
xgas	animated simulation of an ideal gas
xgc	X graphics demo
xkeycaps-2.22	graphically display and edit the X keyboard mapping
xlax	X window system program to send keyboard input to multiple windows
xlbiff	mailbox message previewer for X
xless-1.6.1	File browsing program for the X Window System.
xload	system load average display for X
xmailbox	mailbox checker for X using XPM icons
xman	Manual page display program for the X Window System
xmandel 	window based interface to Mandelbrot sets and Julia sets
xmessage	display a message or query in a window (X-based /bin/echo)
xmove	pseudoserver to support mobile X11 clients
xpd-1.03	process manager for X
xpr/xdpr	dump an X window directly to a printer
xpr	print an X window dump
xpserv	X to PostScript Printing Server
xrsh	start an X program on a remote machine
xrsh/xrlogin	start an xterm that uses rlogin or telnet to connect to a remote host
xrubik	Rubik's cube X widgets
xrunclient	launch an X client
xscreensaver-1.22	run graphics hacks after the user has been idle for a while
xsession-1.1	session manager for the X Window System
xterm_color	diffs to make xterm handle color changes in text
youbin-2.2	mail arrival notification service
.fi

.DH 1 test
.AT
InsPEX	PEX test suite using PHIGS
suspex	PEX test suite using PEXlib
record	tests for RECORD extension
Xcms	Xcms test program
Xext	tests for some extensions: SHAPE, MIT-SHM, SYNC, Multi-Buffering, XTEST
Xi	tests for X Input Extension
.fi

.DH 1 util
.AT
att-nameserver	old name server for SVR3 network software
PHIGS	convert previous version of PHIGS to ISO standard version
.fi

.DH 1 Motif
Software that requires Motif.  Motif is not free software and is not
available from the X Consortium.

.AT
mgdiff	Motif-based graphical file difference browser
pixedit	pixel color editor for X/Motif
pixedit/cpicker	colormap editor for X/Motif
winterp	Widget INTERPreter, an application development environment
xmphone	while you were out phone message pad
.fi

.\" the end
