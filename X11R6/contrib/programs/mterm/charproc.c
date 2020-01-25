/*
 *	$Header: /var/home/sm/ws/michael/work/xterm/RCS/charproc.c,v 1.2 1994/06/07 00:05:30 michael Exp $ Sony Corporation
 */
/******************************************************************

Copyright (c) 1992, 1993, 1994  Sony Corporation

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Sony Corporation shall
not be used in advertising or otherwise to promote the sale, use or ot
her dealings in this Software without prior written authorization from
Sony Corporation.

******************************************************************/
/*
 * $XConsortium: charproc.c,v 1.180 94/04/17 20:23:25 hersh Exp $
 */

/*
 
Copyright (c) 1988  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

*/
/*
 * Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Digital Equipment
 * Corporation not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior permission.
 *
 *
 * DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/* charproc.c */

#include "ptyx.h"
#include "VTparse.h"
#include "data.h"
#include "error.h"
#include "menu.h"
#include "main.h"
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/StringDefs.h>
#include <X11/Xmu/Atoms.h>
#include <X11/Xmu/CharSet.h>
#include <X11/Xmu/Converters.h>
#include <stdio.h>
#include <errno.h>
#include <setjmp.h>
#include <ctype.h>

/*
 * Check for both EAGAIN and EWOULDBLOCK, because some supposedly POSIX
 * systems are broken and return EWOULDBLOCK when they should return EAGAIN.
 * Note that this macro may evaluate its argument more than once.
 */
#if defined(EAGAIN) && defined(EWOULDBLOCK)
#define E_TEST(err) ((err) == EAGAIN || (err) == EWOULDBLOCK)
#else
#ifdef EAGAIN
#define E_TEST(err) ((err) == EAGAIN)
#else
#define E_TEST(err) ((err) == EWOULDBLOCK)
#endif
#endif

extern jmp_buf VTend;

extern Widget toplevel;
extern void exit();
extern char *malloc();
extern char *realloc();

static void VTallocbuf();
static int finput();
static void dotext();
static void WriteText();

static void bitset(), bitclr();
    
#define	DEFAULT		-1
#define	TEXT_BUF_SIZE	256
#define TRACKTIMESEC	4L
#define TRACKTIMEUSEC	0L
#define BELLSUPPRESSMSEC 200

#define XtNalwaysHighlight "alwaysHighlight"
#define XtNappcursorDefault "appcursorDefault"
#define XtNappkeypadDefault "appkeypadDefault"
#define XtNbellSuppressTime "bellSuppressTime"
#define XtNboldFont "boldFont"
#define XtNc132 "c132"
#define XtNcharClass "charClass"
#define XtNcurses "curses"
#define XtNhpLowerleftBugCompat "hpLowerleftBugCompat"
#define XtNcursorColor "cursorColor"
#define XtNcutNewline "cutNewline"
#define XtNcutToBeginningOfLine "cutToBeginningOfLine"
#define XtNeightBitInput "eightBitInput"
#define XtNeightBitOutput "eightBitOutput"
#define XtNgeometry "geometry"
#define XtNtekGeometry "tekGeometry"
#define XtNinternalBorder "internalBorder"
#define XtNjumpScroll "jumpScroll"
#ifdef ALLOWLOGGING
#define XtNlogFile "logFile"
#define XtNlogging "logging"
#define XtNlogInhibit "logInhibit"
#endif
#define XtNloginShell "loginShell"
#define XtNmarginBell "marginBell"
#define XtNpointerColor "pointerColor"
#define XtNpointerColorBackground "pointerColorBackground"
#define XtNpointerShape "pointerShape"
#define XtNmultiClickTime "multiClickTime"
#define XtNmultiScroll "multiScroll"
#define XtNnMarginBell "nMarginBell"
#define XtNresizeGravity "resizeGravity"
#define XtNreverseWrap "reverseWrap"
#define XtNautoWrap "autoWrap"
#define XtNsaveLines "saveLines"
#define XtNscrollBar "scrollBar"
#define XtNscrollTtyOutput "scrollTtyOutput"
#define XtNscrollKey "scrollKey"
#define XtNscrollLines "scrollLines"
#define XtNscrollPos "scrollPos"
#define XtNsignalInhibit "signalInhibit"
#define XtNtekInhibit "tekInhibit"
#define XtNtekSmall "tekSmall"
#define XtNtekStartup "tekStartup"
#define XtNtiteInhibit "titeInhibit"
#define XtNvisualBell "visualBell"
#define XtNallowSendEvents "allowSendEvents"

#define XtCAlwaysHighlight "AlwaysHighlight"
#define XtCAppcursorDefault "AppcursorDefault"
#define XtCAppkeypadDefault "AppkeypadDefault"
#define XtCBellSuppressTime "BellSuppressTime"
#define XtCBoldFont "BoldFont"
#define XtCC132 "C132"
#define XtCCharClass "CharClass"
#define XtCCurses "Curses"
#define XtCHpLowerleftBugCompat "HpLowerleftBugCompat"
#define XtCCutNewline "CutNewline"
#define XtCCutToBeginningOfLine "CutToBeginningOfLine"
#define XtCEightBitInput "EightBitInput"
#define XtCEightBitOutput "EightBitOutput"
#define XtCGeometry "Geometry"
#define XtCJumpScroll "JumpScroll"
#ifdef ALLOWLOGGING
#define XtCLogfile "Logfile"
#define XtCLogging "Logging"
#define XtCLogInhibit "LogInhibit"
#endif
#define XtCLoginShell "LoginShell"
#define XtCMarginBell "MarginBell"
#define XtCMultiClickTime "MultiClickTime"
#define XtCMultiScroll "MultiScroll"
#define XtCColumn "Column"
#define XtCResizeGravity "ResizeGravity"
#define XtCReverseWrap "ReverseWrap"
#define XtCAutoWrap "AutoWrap"
#define XtCSaveLines "SaveLines"
#define XtCScrollBar "ScrollBar"
#define XtCScrollLines "ScrollLines"
#define XtCScrollPos "ScrollPos"
#define XtCScrollCond "ScrollCond"
#define XtCSignalInhibit "SignalInhibit"
#define XtCTekInhibit "TekInhibit"
#define XtCTekSmall "TekSmall"
#define XtCTekStartup "TekStartup"
#define XtCTiteInhibit "TiteInhibit"
#define XtCVisualBell "VisualBell"
#define XtCAllowSendEvents "AllowSendEvents"

#define	doinput()		(bcnt-- > 0 ? *bptr++ : in_put())

static int nparam;
static ANSI reply;
static int param[NPARAM];

static unsigned long ctotal;
static unsigned long ntotal;
static jmp_buf vtjmpbuf;

#ifdef MULTIFONT
#define groundtable (screen->gtable)
extern int mbscstable[];
#else /* MULTIFONT */
extern int groundtable[];
#endif /* MULTIFONT */
extern int csitable[];
extern int dectable[];
extern int eigtable[];
extern int esctable[];
extern int iestable[];
extern int igntable[];
extern int scrtable[];
extern int scstable[];


/* event handlers */
extern void HandleKeyPressed(), HandleEightBitKeyPressed();
extern void HandleStringEvent();
extern void HandleEnterWindow();
extern void HandleLeaveWindow();
extern void HandleBellPropertyChange();
extern void HandleFocusChange();
static void HandleKeymapChange();
extern void HandleInsertSelection();
extern void HandleSelectStart(), HandleKeyboardSelectStart();
extern void HandleSelectExtend(), HandleSelectSet();
extern void HandleSelectEnd(), HandleKeyboardSelectEnd();
extern void HandleStartExtend(), HandleKeyboardStartExtend();
static void HandleBell();
static void HandleVisualBell();
static void HandleIgnore();
extern void HandleSecure();
extern void HandleScrollForward();
extern void HandleScrollBack();
extern void HandleCreateMenu(), HandlePopupMenu();
extern void HandleSetFont();
extern void SetVTFont();
#ifdef I18N
extern void HandleSetInputCode();
#endif /* I18N */

extern Boolean SendMousePosition();
extern void ScrnSetAttributes();

/*
 * NOTE: VTInitialize zeros out the entire ".screen" component of the 
 * XtermWidget, so make sure to add an assignment statement in VTInitialize() 
 * for each new ".screen" field added to this resource list.
 */

/* Defaults */
static  Boolean	defaultFALSE	   = FALSE;
static  Boolean	defaultTRUE	   = TRUE;
static  int	defaultBorderWidth = DEFBORDERWIDTH;
static  int	defaultIntBorder   = DEFBORDER;
static  int	defaultSaveLines   = SAVELINES;
static	int	defaultScrollLines = SCROLLLINES;
static  int	defaultNMarginBell = N_MARGINBELL;
static  int	defaultMultiClickTime = MULTICLICKTIME;
static  int	defaultBellSuppressTime = BELLSUPPRESSMSEC;
static	char *	_Font_Selected_ = "yes";  /* string is arbitrary */
#ifdef MULTIFONT
static  int	defaultGLeft = 0;
static  int	defaultGRight = 1;
static  int	defaultLineSpace = 0;
#endif /* MULTIFONT */
#ifdef I18N
static  int	defaultInputCode = T_LATIN1;
#endif /* I18N */

/*
 * Warning, the following must be kept under 1024 bytes or else some 
 * compilers (particularly AT&T 6386 SVR3.2) will barf).  Workaround is to
 * declare a static buffer and copy in at run time (the the Athena text widget
 * does).  Yuck.
 */
static char defaultTranslations[] =
"\
 Shift <KeyPress> Prior:scroll-back(1,halfpage) \n\
  Shift <KeyPress> Next:scroll-forw(1,halfpage) \n\
Shift <KeyPress> Select:select-cursor-start() select-cursor-end(PRIMARY, CUT_BUFFER0) \n\
Shift <KeyPress> Insert:insert-selection(PRIMARY, CUT_BUFFER0) \n\
       ~Meta <KeyPress>:insert-seven-bit() \n\
        Meta <KeyPress>:insert-eight-bit() \n\
       !Ctrl <Btn1Down>:popup-menu(mainMenu) \n\
  !Lock Ctrl <Btn1Down>:popup-menu(mainMenu) \n\
       ~Meta <Btn1Down>:select-start() \n\
     ~Meta <Btn1Motion>:select-extend() \n\
       !Ctrl <Btn2Down>:popup-menu(vtMenu) \n\
  !Lock Ctrl <Btn2Down>:popup-menu(vtMenu) \n\
 ~Ctrl ~Meta <Btn2Down>:ignore() \n\
   ~Ctrl ~Meta <Btn2Up>:insert-selection(PRIMARY, CUT_BUFFER0) \n\
       !Ctrl <Btn3Down>:popup-menu(fontMenu) \n\
  !Lock Ctrl <Btn3Down>:popup-menu(fontMenu) \n\
 ~Ctrl ~Meta <Btn3Down>:start-extend() \n\
     ~Meta <Btn3Motion>:select-extend()	\n\
                <BtnUp>:select-end(PRIMARY, CUT_BUFFER0) \n\
	      <BtnDown>:bell(0) \
";

static XtActionsRec actionsList[] = { 
    { "bell",		  HandleBell },
    { "create-menu",	  HandleCreateMenu },
    { "ignore",		  HandleIgnore },
    { "insert",		  HandleKeyPressed },  /* alias for insert-seven-bit */
    { "insert-seven-bit", HandleKeyPressed },
    { "insert-eight-bit", HandleEightBitKeyPressed },
    { "insert-selection", HandleInsertSelection },
    { "keymap", 	  HandleKeymapChange },
    { "popup-menu",	  HandlePopupMenu },
    { "secure",		  HandleSecure },
    { "select-start",	  HandleSelectStart },
    { "select-extend",	  HandleSelectExtend },
    { "select-end",	  HandleSelectEnd },
    { "select-set",	  HandleSelectSet },
    { "select-cursor-start",	  HandleKeyboardSelectStart },
    { "select-cursor-end",	  HandleKeyboardSelectEnd },
    { "set-vt-font",	  HandleSetFont },
    { "start-extend",	  HandleStartExtend },
    { "start-cursor-extend",	  HandleKeyboardStartExtend },
    { "string",		  HandleStringEvent },
    { "scroll-forw",	  HandleScrollForward },
    { "scroll-back",	  HandleScrollBack },
    /* menu actions */
    { "allow-send-events",	HandleAllowSends },
    { "set-visual-bell",	HandleSetVisualBell },
#ifdef ALLOWLOGGING
    { "set-logging",		HandleLogging },
#endif
    { "redraw",			HandleRedraw },
    { "send-signal",		HandleSendSignal },
    { "quit",			HandleQuit },
    { "set-scrollbar",		HandleScrollbar },
    { "set-jumpscroll",		HandleJumpscroll },
    { "set-reverse-video",	HandleReverseVideo },
    { "set-autowrap",		HandleAutoWrap },
    { "set-reversewrap",	HandleReverseWrap },
    { "set-autolinefeed",	HandleAutoLineFeed },
    { "set-appcursor",		HandleAppCursor },
    { "set-appkeypad",		HandleAppKeypad },
    { "set-scroll-on-key",	HandleScrollKey },
    { "set-scroll-on-tty-output",	HandleScrollTtyOutput },
    { "set-allow132",		HandleAllow132 },
    { "set-cursesemul",		HandleCursesEmul },
    { "set-marginbell",		HandleMarginBell },
    { "set-altscreen",		HandleAltScreen },
    { "soft-reset",		HandleSoftReset },
    { "hard-reset",		HandleHardReset },
    { "clear-saved-lines",	HandleClearSavedLines },
    { "set-terminal-type",	HandleSetTerminalType },
    { "set-visibility",		HandleVisibility },
    { "set-tek-text",		HandleSetTekText },
    { "tek-page",		HandleTekPage },
    { "tek-reset",		HandleTekReset },
    { "tek-copy",		HandleTekCopy },
    { "visual-bell",		HandleVisualBell },
#ifdef STATUSLINE
    { "set-statusline",		HandleStatusLine },
    { "set-reversestatus",	HandleReverseStatus },
#endif /* STATUSLINE */
#ifdef I18N
    { "set-input-code",		HandleSetInputCode },
#endif /* I18N */
};

static XtResource resources[] = {
{XtNfont, XtCFont, XtRString, sizeof(char *),
	XtOffsetOf(XtermWidgetRec, misc.f_n), XtRString,
	DEFFONT},
{XtNboldFont, XtCBoldFont, XtRString, sizeof(char *),
	XtOffsetOf(XtermWidgetRec, misc.f_b), XtRString,
	DEFBOLDFONT},
{XtNc132, XtCC132, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.c132),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNcharClass, XtCCharClass, XtRString, sizeof(char *),
	XtOffsetOf(XtermWidgetRec, screen.charClass),
	XtRString, (XtPointer) NULL},
{XtNcurses, XtCCurses, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.curses),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNhpLowerleftBugCompat, XtCHpLowerleftBugCompat, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.hp_ll_bc),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNcutNewline, XtCCutNewline, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.cutNewline),
	XtRBoolean, (XtPointer) &defaultTRUE},
{XtNcutToBeginningOfLine, XtCCutToBeginningOfLine, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.cutToBeginningOfLine),
	XtRBoolean, (XtPointer) &defaultTRUE},
{XtNbackground, XtCBackground, XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, core.background_pixel),
	XtRString, "XtDefaultBackground"},
{XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, screen.foreground),
	XtRString, "XtDefaultForeground"},
{XtNcursorColor, XtCForeground, XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, screen.cursorcolor),
	XtRString, "XtDefaultForeground"},
{XtNeightBitInput, XtCEightBitInput, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.input_eight_bits), 
	XtRBoolean, (XtPointer) &defaultTRUE},
{XtNeightBitOutput, XtCEightBitOutput, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.output_eight_bits), 
	XtRBoolean, (XtPointer) &defaultTRUE},
{XtNgeometry,XtCGeometry, XtRString, sizeof(char *),
	XtOffsetOf(XtermWidgetRec, misc.geo_metry),
	XtRString, (XtPointer) NULL},
{XtNalwaysHighlight,XtCAlwaysHighlight,XtRBoolean,
        sizeof(Boolean),XtOffsetOf(XtermWidgetRec, screen.always_highlight),
        XtRBoolean, (XtPointer) &defaultFALSE},
{XtNappcursorDefault,XtCAppcursorDefault,XtRBoolean,
        sizeof(Boolean),XtOffsetOf(XtermWidgetRec, misc.appcursorDefault),
        XtRBoolean, (XtPointer) &defaultFALSE},
{XtNappkeypadDefault,XtCAppkeypadDefault,XtRBoolean,
        sizeof(Boolean),XtOffsetOf(XtermWidgetRec, misc.appkeypadDefault),
        XtRBoolean, (XtPointer) &defaultFALSE},
{XtNbellSuppressTime, XtCBellSuppressTime, XtRInt, sizeof(int),
        XtOffsetOf(XtermWidgetRec, screen.bellSuppressTime),
        XtRInt, (XtPointer) &defaultBellSuppressTime},
{XtNtekGeometry,XtCGeometry, XtRString, sizeof(char *),
	XtOffsetOf(XtermWidgetRec, misc.T_geometry),
	XtRString, (XtPointer) NULL},
{XtNinternalBorder,XtCBorderWidth,XtRInt, sizeof(int),
	XtOffsetOf(XtermWidgetRec, screen.border),
	XtRInt, (XtPointer) &defaultIntBorder},
{XtNjumpScroll, XtCJumpScroll, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.jumpscroll),
	XtRBoolean, (XtPointer) &defaultTRUE},
#ifdef ALLOWLOGGING
{XtNlogFile, XtCLogfile, XtRString, sizeof(char *),
	XtOffsetOf(XtermWidgetRec, screen.logfile),
	XtRString, (XtPointer) NULL},
{XtNlogging, XtCLogging, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.log_on),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNlogInhibit, XtCLogInhibit, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.logInhibit),
	XtRBoolean, (XtPointer) &defaultFALSE},
#endif
{XtNloginShell, XtCLoginShell, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.login_shell),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNmarginBell, XtCMarginBell, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.marginbell),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNpointerColor, XtCForeground, XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, screen.mousecolor),
	XtRString, "XtDefaultForeground"},
{XtNpointerColorBackground, XtCBackground, XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, screen.mousecolorback),
	XtRString, "XtDefaultBackground"},
{XtNpointerShape,XtCCursor, XtRCursor, sizeof(Cursor),
	XtOffsetOf(XtermWidgetRec, screen.pointer_cursor),
	XtRString, (XtPointer) "xterm"},
{XtNmultiClickTime,XtCMultiClickTime, XtRInt, sizeof(int),
	XtOffsetOf(XtermWidgetRec, screen.multiClickTime),
	XtRInt, (XtPointer) &defaultMultiClickTime},
{XtNmultiScroll,XtCMultiScroll, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.multiscroll),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNnMarginBell,XtCColumn, XtRInt, sizeof(int),
	XtOffsetOf(XtermWidgetRec, screen.nmarginbell),
	XtRInt, (XtPointer) &defaultNMarginBell},
{XtNreverseVideo,XtCReverseVideo,XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.re_verse),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNresizeGravity, XtCResizeGravity, XtRGravity, sizeof(XtGravity),
	XtOffsetOf(XtermWidgetRec, misc.resizeGravity),
	XtRImmediate, (XtPointer) SouthWestGravity},
{XtNreverseWrap,XtCReverseWrap, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.reverseWrap),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNautoWrap,XtCAutoWrap, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.autoWrap),
	XtRBoolean, (XtPointer) &defaultTRUE},
{XtNsaveLines, XtCSaveLines, XtRInt, sizeof(int),
	XtOffsetOf(XtermWidgetRec, screen.savelines),
	XtRInt, (XtPointer) &defaultSaveLines},
{XtNscrollBar, XtCScrollBar, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.scrollbar),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNscrollTtyOutput,XtCScrollCond, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.scrollttyoutput),
	XtRBoolean, (XtPointer) &defaultTRUE},
{XtNscrollKey, XtCScrollCond, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.scrollkey),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNscrollLines, XtCScrollLines, XtRInt, sizeof(int),
	XtOffsetOf(XtermWidgetRec, screen.scrolllines),
	XtRInt, (XtPointer) &defaultScrollLines},
{XtNsignalInhibit,XtCSignalInhibit,XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.signalInhibit),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNtekInhibit, XtCTekInhibit, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.tekInhibit),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNtekSmall, XtCTekSmall, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.tekSmall),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNtekStartup, XtCTekStartup, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.TekEmu),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNtiteInhibit, XtCTiteInhibit, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.titeInhibit),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNvisualBell, XtCVisualBell, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.visualbell),
	XtRBoolean, (XtPointer) &defaultFALSE},
{XtNallowSendEvents, XtCAllowSendEvents, XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, screen.allowSendEvents),
	XtRBoolean, (XtPointer) &defaultFALSE},
{"font1", "Font1", XtRString, sizeof(String),
	XtOffsetOf(XtermWidgetRec, screen.menu_font_names[fontMenu_font1]),
	XtRString, (XtPointer) NULL},
{"font2", "Font2", XtRString, sizeof(String),
	XtOffsetOf(XtermWidgetRec, screen.menu_font_names[fontMenu_font2]),
	XtRString, (XtPointer) NULL},
{"font3", "Font3", XtRString, sizeof(String),
	XtOffsetOf(XtermWidgetRec, screen.menu_font_names[fontMenu_font3]),
	XtRString, (XtPointer) NULL},
{"font4", "Font4", XtRString, sizeof(String),
	XtOffsetOf(XtermWidgetRec, screen.menu_font_names[fontMenu_font4]),
	XtRString, (XtPointer) NULL},
{"font5", "Font5", XtRString, sizeof(String),
	XtOffsetOf(XtermWidgetRec, screen.menu_font_names[fontMenu_font5]),
	XtRString, (XtPointer) NULL},
{"font6", "Font6", XtRString, sizeof(String),
	XtOffsetOf(XtermWidgetRec, screen.menu_font_names[fontMenu_font6]),
	XtRString, (XtPointer) NULL},
#ifdef COLOR
{"textColor0", "TextColor0", XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, screen.color[1]),
	XtRString, "Black"},
{"textColor1", "TextColor1", XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, screen.color[2]),
	XtRString, "Red"},
{"textColor2", "TextColor2", XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, screen.color[3]),
	XtRString, "Green"},
{"textColor3", "TextColor3", XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, screen.color[4]),
	XtRString, "Yellow"},
{"textColor4", "TextColor4", XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, screen.color[5]),
	XtRString, "Blue"},
{"textColor5", "TextColor5", XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, screen.color[6]),
	XtRString, "Magenta"},
{"textColor6", "TextColor6", XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, screen.color[7]),
	XtRString, "Cyan"},
{"textColor7", "TextColor7", XtRPixel, sizeof(Pixel),
	XtOffsetOf(XtermWidgetRec, screen.color[8]),
	XtRString, "White"},
#endif /* COLOR */
#ifdef STATUSLINE
{"statusLine", "StatusLine", XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.statusline),
	XtRBoolean, (XtPointer) &defaultFALSE},
{"statusNormal", "StatusNormal", XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.statusnormal),
	XtRBoolean, (XtPointer) &defaultFALSE},
#endif /* STATUSLINE */
#ifdef MULTIFONT
{"gLeft", "GLeft", XtRInt, sizeof(int),
	XtOffsetOf(XtermWidgetRec, misc.gl),
	XtRInt, (XtPointer) &defaultGLeft},
{"gRight", "GRight", XtRInt, sizeof(int),
	XtOffsetOf(XtermWidgetRec, misc.gr),
	XtRInt, (XtPointer) &defaultGRight},
{"topSpace", "TopSpace", XtRInt, sizeof(int),
	XtOffsetOf(XtermWidgetRec, misc.top_space),
	XtRInt, (XtPointer) &defaultLineSpace},
{"bottomSpace", "BottomSpace", XtRInt, sizeof(int),
	XtOffsetOf(XtermWidgetRec, misc.bottom_space),
	XtRInt, (XtPointer) &defaultLineSpace},
#endif /* MULTIFONT */
#ifdef USE_XIM
{"international", "International", XtRBoolean, sizeof(Boolean),
	XtOffsetOf(XtermWidgetRec, misc.international),
	XtRBoolean, (XtPointer) &defaultFALSE},
#endif /* USE_XIM */
};

static void VTClassInit();
static void VTInitialize();
static void VTRealize();
static void VTExpose();
static void VTResize();
static void VTDestroy();
static Boolean VTSetValues();

static WidgetClassRec xtermClassRec = {
  {
/* core_class fields */	
    /* superclass	  */	(WidgetClass) &widgetClassRec,
    /* class_name	  */	"VT100",
    /* widget_size	  */	sizeof(XtermWidgetRec),
    /* class_initialize   */    VTClassInit,
    /* class_part_initialize */ NULL,
    /* class_inited       */	FALSE,
    /* initialize	  */	VTInitialize,
    /* initialize_hook    */    NULL,				
    /* realize		  */	VTRealize,
    /* actions		  */	actionsList,
    /* num_actions	  */	XtNumber(actionsList),
    /* resources	  */	resources,
    /* num_resources	  */	XtNumber(resources),
    /* xrm_class	  */	NULLQUARK,
    /* compress_motion	  */	TRUE,
    /* compress_exposure  */	FALSE,
    /* compress_enterleave */   TRUE,
    /* visible_interest	  */	FALSE,
    /* destroy		  */	VTDestroy,
    /* resize		  */	VTResize,
    /* expose		  */	VTExpose,
    /* set_values	  */	VTSetValues,
    /* set_values_hook    */    NULL,
    /* set_values_almost  */    NULL,
    /* get_values_hook    */    NULL,
    /* accept_focus	  */	NULL,
    /* version            */    XtVersion,
    /* callback_offsets   */    NULL,
    /* tm_table           */    defaultTranslations,
    /* query_geometry     */    XtInheritQueryGeometry,
    /* display_accelerator*/    XtInheritDisplayAccelerator,
    /* extension          */    NULL
  }
};

WidgetClass xtermWidgetClass = (WidgetClass)&xtermClassRec;

static void VTparse()
{
	register TScreen *screen = &term->screen;
	register int *parsestate = groundtable;
	register unsigned int c;
	register unsigned char *cp;
	register int row, col, top, bot, scstype;
	extern int TrackMouse();
#ifdef MULTIFONT
	int mbcs = 0;
	int scs96 = 0;
	static int height = 0;
#endif /* MULTIFONT */
#ifdef COLOR
	register unsigned fgs;
#endif /* COLOR */

	if(setjmp(vtjmpbuf))
		parsestate = groundtable;
	for( ; ; ) {
	        switch (parsestate[c = doinput()]) {
#ifdef MULTIFONT
		 case CASE_DEL:
			if (!(screen->gflags[screen->curgl] & SCS96)) {
				break;
			}
		 case CASE_SP:
			if (screen->gflags[screen->curgl] & MBSCS) {
				cp = bptr;
				*--bptr = c;
				dotext(screen, term->flags, 'B', bptr, cp);
				bptr = cp;
				break;
			}
#endif /* MULTIFONT */
		 case CASE_PRINT:
			/* printable characters */
			top = bcnt > TEXT_BUF_SIZE ? TEXT_BUF_SIZE : bcnt;
			cp = bptr;
			*--bptr = c;
#ifdef MULTIFONT
			if (screen->gflags[screen->curgl] & MBSCS) {
				while(top > 0 && !(*cp & 0x80) && isgraph(*cp)) {
					top--;
					bcnt--;
					cp++;
				}
			} else if (!(screen->gflags[screen->curgl] & SCS96)) {
				while(top > 0 && !(*cp & 0x80) && isprint(*cp)) {
					top--;
					bcnt--;
					cp++;
				}
			} else {
				while(top > 0 && !(*cp & 0x80) && (*cp > 0x1f)) {
					top--;
					bcnt--;
					cp++;
				}
			}
#else /* MULTIFONT */
			while(top > 0 && isprint(*cp & 0x7f)) {
				top--;
				bcnt--;
				cp++;
			}
#endif /* MULTIFONT */
			if(screen->curss) {
#ifdef MULTIFONT
				if (screen->gflags[screen->curss] & MBSCS) {
				    dotext(screen, term->flags | MBSCS,
				     screen->gsets[screen->curss],
				     bptr, bptr + 2);
				    bptr++;
				} else {
				    dotext(screen, term->flags,
				     screen->gsets[screen->curss],
				     bptr, bptr + 1);
				}
#else /* MULTIFONT */
				dotext(screen, term->flags,
				 screen->gsets[screen->curss], bptr, bptr + 1);
#endif /* MULTIFONT */
				screen->curss = 0;
				bptr++;
			}
#ifdef MULTIFONT
			if (bptr < cp) {
				if ((screen->gflags[screen->curgl] & MBSCS) &&
				    (cp - bptr) & 1) {
					if (bcnt > 0) {
						if (top == 0) {
							bcnt++;
							cp--;
						} else {
							bcnt--;
							cp++;
						}
					} else {
						pending = *--cp;
					}
				}
				if (bptr < cp) {
					dotext(screen, term->flags | screen->gflags[screen->curgl],
					 screen->gsets[screen->curgl], bptr, cp);
				}
			}
#else /* MULTIFONT */
			if(bptr < cp)
				dotext(screen, term->flags,
				 screen->gsets[screen->curgl], bptr, cp);
#endif /* MULTIFONT */
			bptr = cp;
			break;
#ifdef MULTIFONT
		 case CASE_DELR:
			if (!(screen->gflags[screen->curgr] & SCS96)) {
				break;
			}

		 case CASE_SPR:
			if (screen->gflags[screen->curgr] & MBSCS) {
				cp = bptr;
				*--bptr = c;
				dotext(screen, term->flags,
				 'A' | SCS96, bptr, cp);
				bptr = cp;
				break;
			}

		 case CASE_PRINTR:
			/* printable characters */
			top = bcnt > TEXT_BUF_SIZE ? TEXT_BUF_SIZE : bcnt;
			cp = bptr;
			*--bptr = c;
			if (screen->gflags[screen->curgr] & MBSCS) {
				while(top > 0 && (*cp & 0x80) && isgraph(*cp & 0x7f)) {
					top--;
					bcnt--;
					cp++;
				}
			} else if (!(screen->gflags[screen->curgr] & SCS96)) {
				while(top > 0 && (*cp & 0x80) && isprint(*cp & 0x7f)) {
					top--;
					bcnt--;
					cp++;
				}
			} else {
				while(top > 0 && (*cp > 0xa0)) {
					top--;
					bcnt--;
					cp++;
				}
			}
			if(screen->curss) {
				if (screen->gflags[screen->curss] & MBSCS) {
					dotext(screen, term->flags | MBSCS,
					 screen->gsets[screen->curss],
					 bptr, bptr + 2);
					bptr++;
				} else {
					dotext(screen, term->flags,
					 screen->gsets[screen->curss],
					 bptr, bptr + 1);
				}
				screen->curss = 0;
				bptr++;
			}
			if (bptr < cp) {
				if ((screen->gflags[screen->curgr] & MBSCS) &&
				    (cp - bptr) & 1) {
					if (bcnt > 0) {
						if (top == 0) {
							bcnt++;
							cp--;
						} else {
							bcnt--;
							cp++;
						}
					} else {
						pending = *--cp;
					}
				}
				if(bptr < cp) {
					dotext(screen, term->flags | screen->gflags[screen->curgr],
					 screen->gsets[screen->curgr], bptr,
					 cp);
				}
			}
			bptr = cp;
			break;

#ifdef I18N
		 case CASE_PRINTK:
			/* printable characters */
			top = bcnt > TEXT_BUF_SIZE ? TEXT_BUF_SIZE : bcnt;
			cp = bptr;
			*--bptr = c;
			while(top > 0 &&
			      (*cp >= 0xa1 && *cp <= 0xdf)) {
				top--;
				bcnt--;
				cp++;
			}
			if(screen->curss) {
				dotext(screen, term->flags,
				 'I', bptr, bptr + 1);
				screen->curss = 0;
				bptr++;
			}
			if(bptr < cp)
				dotext(screen, term->flags,
				 'I', bptr, cp);
			bptr = cp;
			break;

		 case CASE_PRINTS:
			/* printable characters */
			top = bcnt > TEXT_BUF_SIZE ? TEXT_BUF_SIZE : bcnt;
			cp = bptr;
			*--bptr = c;
			if (top > 0) {
				*bptr -= (*bptr >= 0xa0) ? 0xc1: 0x81;
				if (*cp >= 0x9f) {
					*bptr = (*bptr << 1) + 0x22;
					*cp -= 0x7e;
				} else {
					*bptr = (*bptr << 1) + 0x21;
					*cp -= ((*cp <= 0x7e) ? 0x1f: 0x20);
				}
				top--;
				bcnt--;
				cp++;
			}
			while(top > 1 &&
			      ((*cp >= 0x81 && *cp <= 0x9f) ||
			       (*cp >= 0xe0 && *cp <= 0xfc))) {
				*cp -= (*cp >= 0xa0) ? 0xc1: 0x81;
				if (cp[1] >= 0x9f) {
					*cp = (*cp << 1) + 0x22;
					cp[1] -= 0x7e;
				} else {
					*cp = (*cp << 1) + 0x21;
					cp[1] -= ((cp[1] <= 0x7e) ? 0x1f: 0x20);
				}
				top -= 2;
				bcnt -= 2;
				cp += 2;
			}
			if (bptr < cp) {
				if ((cp - bptr) & 1) {
					if (bcnt > 0) {
						if (top == 0) {
							bcnt++;
							cp--;
						} else {
							bcnt--;
							cp++;
						}
					} else {
						pending = *--cp;
					}
				}
				if(bptr < cp) {
					dotext(screen, term->flags|MBSCS,
					 'B', bptr,
					 cp);
				}
			}
			bptr = cp;
			break;
#endif /* I18N */
#endif /* MULTIFONT */

		 case CASE_GROUND_STATE:
			/* exit ignore mode */
#ifdef MULTIFONT
			scs96 = 0;
			mbcs = 0;
#endif /* MULTIFONT */
			parsestate = groundtable;
			break;

		 case CASE_IGNORE_STATE:
			/* Ies: ignore anything else */
			parsestate = igntable;
			break;

		 case CASE_IGNORE_ESC:
			/* Ign: escape */
			parsestate = iestable;
			break;

		 case CASE_IGNORE:
			/* Ignore character */
			break;

		 case CASE_BELL:
			/* bell */
			Bell();
			break;

		 case CASE_BS:
			/* backspace */
			CursorBack(screen, 1);
			break;

		 case CASE_CR:
			/* carriage return */
			CarriageReturn(screen);
			parsestate = groundtable;
			break;

		 case CASE_ESC:
			/* escape */
			parsestate = esctable;
			break;

		 case CASE_VMOT:
			/*
			 * form feed, line feed, vertical tab
			 */
#ifdef STATUSLINE
			if (!screen->instatus) {
				Index(screen, 1);
			}
#else /* STATUSLINE */
			Index(screen, 1);
#endif /* STATUSLINE */
			if (term->flags & LINEFEED)
				CarriageReturn(screen);
			if (QLength(screen->display) > 0 ||
			    GetBytesAvailable (ConnectionNumber(screen->display)) > 0)
			  xevents();
			parsestate = groundtable;
			break;

		 case CASE_TAB:
			/* tab */
			screen->cur_col = TabNext(term->tabs, screen->cur_col);
			if (screen->cur_col > screen->max_col)
				screen->cur_col = screen->max_col;
			break;

		 case CASE_SI:
			screen->curgl = 0;
			break;

		 case CASE_SO:
			screen->curgl = 1;
			break;

		 case CASE_SCR_STATE:
			/* enter scr state */
			parsestate = scrtable;
			break;

		 case CASE_SCS0_STATE:
			/* enter scs state 0 */
			scstype = 0;
#ifdef MULTIFONT
			if (!mbcs) parsestate = scstable;
#else /* MULTIFONT */
			parsestate = scstable;
#endif /* MULTIFONT */
			break;

		 case CASE_SCS1_STATE:
			/* enter scs state 1 */
			scstype = 1;
#ifdef MULTIFONT
			if (!mbcs) parsestate = scstable;
#else /* MULTIFONT */
			parsestate = scstable;
#endif /* MULTIFONT */
			break;

		 case CASE_SCS2_STATE:
			/* enter scs state 2 */
			scstype = 2;
#ifdef MULTIFONT
			if (!mbcs) parsestate = scstable;
#else /* MULTIFONT */
			parsestate = scstable;
#endif /* MULTIFONT */
			break;

		 case CASE_SCS3_STATE:
			/* enter scs state 3 */
			scstype = 3;
#ifdef MULTIFONT
			if (!mbcs) parsestate = scstable;
#else /* MULTIFONT */
			parsestate = scstable;
#endif /* MULTIFONT */
			break;

#ifdef MULTIFONT
		 case CASE_MBSCS:
			mbcs = 1;
			scstype = 0;
			parsestate = mbscstable;
			break;

		 case CASE_96SCS1_STATE:
			scs96 = 1;
			scstype = 1;
			parsestate = scstable;
			break;

		 case CASE_96SCS2_STATE:
			/* enter scs state 2 */
			scs96 = 1;
			scstype = 2;
			parsestate = scstable;
			break;

		 case CASE_96SCS3_STATE:
			/* enter scs state 3 */
			scs96 = 1;
			scstype = 3;
			parsestate = scstable;
			break;

#endif /* MULTIFONT */
		 case CASE_ESC_IGNORE:
			/* unknown escape sequence */
			parsestate = eigtable;
			break;

		 case CASE_ESC_DIGIT:
			/* digit in csi or dec mode */
			if((row = param[nparam - 1]) == DEFAULT)
				row = 0;
			param[nparam - 1] = 10 * row + (c - '0');
			break;

		 case CASE_ESC_SEMI:
			/* semicolon in csi or dec mode */
			param[nparam++] = DEFAULT;
			break;

		 case CASE_DEC_STATE:
			/* enter dec mode */
			parsestate = dectable;
			break;

		 case CASE_ICH:
			/* ICH */
			if((row = param[0]) < 1)
				row = 1;
			InsertChar(screen, row);
			parsestate = groundtable;
			break;

		 case CASE_CUU:
			/* CUU */
#ifdef STATUSLINE
			if (!screen->instatus) {
				if((row = param[0]) < 1)
					row = 1;
				CursorUp(screen, row);
			}
#else /* STATUSLINE */
			if((row = param[0]) < 1)
				row = 1;
			CursorUp(screen, row);
#endif /* STATUSLINE */
			parsestate = groundtable;
			break;

		 case CASE_CUD:
			/* CUD */
#ifdef STATUSLINE
			if (!screen->instatus) {
				if((row = param[0]) < 1)
					row = 1;
				CursorDown(screen, row);
			}
#else /* STATUSLINE */
			if((row = param[0]) < 1)
				row = 1;
			CursorDown(screen, row);
#endif /* STATUSLINE */
			parsestate = groundtable;
			break;

		 case CASE_CUF:
			/* CUF */
			if((row = param[0]) < 1)
				row = 1;
			CursorForward(screen, row);
			parsestate = groundtable;
			break;

		 case CASE_CUB:
			/* CUB */
			if((row = param[0]) < 1)
				row = 1;
			CursorBack(screen, row);
			parsestate = groundtable;
			break;

		 case CASE_CUP:
			/* CUP | HVP */
#ifdef STATUSLINE
			if (!screen->instatus) {
				if((row = param[0]) < 1)
					row = 1;
				if(nparam < 2 || (col = param[1]) < 1)
					col = 1;
				CursorSet(screen, row-1, col-1, term->flags);
			}
#else /* STATUSLINE */
			if((row = param[0]) < 1)
				row = 1;
			if(nparam < 2 || (col = param[1]) < 1)
				col = 1;
			CursorSet(screen, row-1, col-1, term->flags);
#endif /* STATUSLINE */
			parsestate = groundtable;
			break;

		 case CASE_HP_BUGGY_LL:
			/* Some HP-UX applications have the bug that they
			   assume ESC F goes to the lower left corner of
			   the screen, regardless of what terminfo says. */
			if (screen->hp_ll_bc)
			    CursorSet(screen, screen->max_row, 0, term->flags);
			parsestate = groundtable;
			break;

		 case CASE_ED:
			/* ED */
			switch (param[0]) {
			 case DEFAULT:
			 case 0:
#ifdef STATUSLINE
				if (screen->instatus)
					ClearRight(screen);
				else
					ClearBelow(screen);
#else /* STATUSLINE */
				ClearBelow(screen);
#endif /* STATUSLINE */
				break;

			 case 1:
#ifdef STATUSLINE
				if (screen->instatus)
					ClearLeft(screen);
				else
					ClearAbove(screen);
#else /* STATUSLINE */
				ClearAbove(screen);
#endif /* STATUSLINE */
				break;

			 case 2:
#ifdef STATUSLINE
				if (screen->instatus)
					ClearLine(screen);
				else
					ClearScreen(screen);
#else /* STATUSLINE */
				ClearScreen(screen);
#endif /* STATUSLINE */
				break;
			}
			parsestate = groundtable;
			break;

		 case CASE_EL:
			/* EL */
			switch (param[0]) {
			 case DEFAULT:
			 case 0:
				ClearRight(screen);
				break;
			 case 1:
				ClearLeft(screen);
				break;
			 case 2:
				ClearLine(screen);
				break;
			}
			parsestate = groundtable;
			break;

		 case CASE_IL:
			/* IL */
#ifdef STATUSLINE
			if (!screen->instatus) {
				if((row = param[0]) < 1)
					row = 1;
				InsertLine(screen, row);
			}
#else /* STATUSLINE */
			if((row = param[0]) < 1)
				row = 1;
			InsertLine(screen, row);
#endif /* STATUSLINE */
			parsestate = groundtable;
			break;

		 case CASE_DL:
			/* DL */
#ifdef STATUSLINE
			if (!screen->instatus) {
				if((row = param[0]) < 1)
					row = 1;
				DeleteLine(screen, row);
			}
#else /* STATUSLINE */
			if((row = param[0]) < 1)
				row = 1;
			DeleteLine(screen, row);
#endif /* STATUSLINE */
			parsestate = groundtable;
			break;

		 case CASE_DCH:
			/* DCH */
			if((row = param[0]) < 1)
				row = 1;
			DeleteChar(screen, row);
			parsestate = groundtable;
			break;

		 case CASE_TRACK_MOUSE:
		 	/* Track mouse as long as in window and between
			   specified rows */
			TrackMouse(param[0], param[2]-1, param[1]-1,
			 param[3]-1, param[4]-2);
			break;

		 case CASE_DECID:
			param[0] = -1;		/* Default ID parameter */
			/* Fall through into ... */
		 case CASE_DA1:
			/* DA1 */
			if (param[0] <= 0) {	/* less than means DEFAULT */
				reply.a_type   = CSI;
				reply.a_pintro = '?';
				reply.a_nparam = 2;
				reply.a_param[0] = 1;		/* VT102 */
				reply.a_param[1] = 2;		/* VT102 */
				reply.a_inters = 0;
				reply.a_final  = 'c';
				unparseseq(&reply, screen->respond);
			}
			parsestate = groundtable;
			break;

		 case CASE_TBC:
			/* TBC */
			if ((row = param[0]) <= 0) /* less than means default */
				TabClear(term->tabs, screen->cur_col);
			else if (row == 3)
				TabZonk(term->tabs);
			parsestate = groundtable;
			break;

		 case CASE_SET:
			/* SET */
			ansi_modes(term, bitset);
			parsestate = groundtable;
			break;

		 case CASE_RST:
			/* RST */
			ansi_modes(term, bitclr);
			parsestate = groundtable;
			break;

		 case CASE_SGR:
			/* SGR */
			for (row=0; row<nparam; ++row) {
				switch (param[row]) {
				 case DEFAULT:
				 case 0:
#ifdef COLOR
					term->flags &= ~(INVERSE|BOLD|UNDERLINE|COLOR_MASK);
#else /* COLOR */
					term->flags &= ~(INVERSE|BOLD|UNDERLINE);
#endif /* COLOR */
					break;
				 case 1:
				 case 5:	/* Blink, really.	*/
					term->flags |= BOLD;
					break;
				 case 4:	/* Underscore		*/
					term->flags |= UNDERLINE;
					break;
				 case 7:
					term->flags |= INVERSE;
#ifdef EXTEND
					break;
				 case 21:
				 case 25:	/* Blink, really.	*/
					term->flags &= ~(BOLD);
					break;
				 case 24:	/* Underscore		*/
					term->flags &= ~(UNDERLINE);
					break;
				 case 27:
					term->flags &= ~(INVERSE);
					break;
#endif /* EXTEND */
#ifdef COLOR
				 case 30:
					term->flags &= ~FORE_MASK;
					term->flags |= FORE_COLOR0;
					break;
				 case 31:
					term->flags &= ~FORE_MASK;
					term->flags |= FORE_COLOR1;
					break;
				 case 32:
					term->flags &= ~FORE_MASK;
					term->flags |= FORE_COLOR2;
					break;
				 case 33:
					term->flags &= ~FORE_MASK;
					term->flags |= FORE_COLOR3;
					break;
				 case 34:
					term->flags &= ~FORE_MASK;
					term->flags |= FORE_COLOR4;
					break;
				 case 35:
					term->flags &= ~FORE_MASK;
					term->flags |= FORE_COLOR5;
					break;
				 case 36:
					term->flags &= ~FORE_MASK;
					term->flags |= FORE_COLOR6;
					break;
				 case 37:
					term->flags &= ~FORE_MASK;
					term->flags |= FORE_COLOR7;
					break;
				 case 39:
					term->flags &= ~FORE_MASK;
					break;
				 case 40:
					term->flags &= ~BACK_MASK;
					term->flags |= BACK_COLOR0;
					break;
				 case 41:
					term->flags &= ~BACK_MASK;
					term->flags |= BACK_COLOR1;
					break;
				 case 42:
					term->flags &= ~BACK_MASK;
					term->flags |= BACK_COLOR2;
					break;
				 case 43:
					term->flags &= ~BACK_MASK;
					term->flags |= BACK_COLOR3;
					break;
				 case 44:
					term->flags &= ~BACK_MASK;
					term->flags |= BACK_COLOR4;
					break;
				 case 45:
					term->flags &= ~BACK_MASK;
					term->flags |= BACK_COLOR5;
					break;
				 case 46:
					term->flags &= ~BACK_MASK;
					term->flags |= BACK_COLOR6;
					break;
				 case 47:
					term->flags &= ~BACK_MASK;
					term->flags |= BACK_COLOR7;
					break;
				 case 49:
					term->flags &= ~BACK_MASK;
					break;
#endif /* COLOR */
				}
			}
#ifdef COLOR
			fgs = term->flags;
			fgs ^= INVERSE;
			if (!term->misc.re_verse && (fgs & COLOR_MASK)) {
				term->flags &= REVERSE_VIDEO;
				term->misc.re_verse = True;
			}
#endif /* COLOR */
			parsestate = groundtable;
			break;

		 case CASE_CPR:
			/* CPR */
			if ((row = param[0]) == 5) {
				reply.a_type = CSI;
				reply.a_pintro = 0;
				reply.a_nparam = 1;
				reply.a_param[0] = 0;
				reply.a_inters = 0;
				reply.a_final  = 'n';
				unparseseq(&reply, screen->respond);
			} else if (row == 6) {
				reply.a_type = CSI;
				reply.a_pintro = 0;
				reply.a_nparam = 2;
				reply.a_param[0] = screen->cur_row+1;
				reply.a_param[1] = screen->cur_col+1;
				reply.a_inters = 0;
				reply.a_final  = 'R';
				unparseseq(&reply, screen->respond);
			}
			parsestate = groundtable;
			break;

		 case CASE_HP_MEM_LOCK:
		 case CASE_HP_MEM_UNLOCK:
			if(screen->scroll_amt)
			    FlushScroll(screen);
			if (parsestate[c] == CASE_HP_MEM_LOCK)
			    screen->top_marg = screen->cur_row;
			else
			    screen->top_marg = 0;
			parsestate = groundtable;
			break;

		 case CASE_DECSTBM:
			/* DECSTBM - set scrolling region */
#ifdef STATUSLINE
			if (!screen->instatus) {
				if((top = param[0]) < 1)
					top = 1;
				if(nparam < 2 || (bot = param[1]) == DEFAULT
				   || bot > screen->max_row + 1
				   || bot == 0)
					bot = screen->max_row+1;
				if (bot > top) {
					if(screen->scroll_amt)
						FlushScroll(screen);
					screen->top_marg = top-1;
					screen->bot_marg = bot-1;
					CursorSet(screen, 0, 0, term->flags);
				}
			}
#else /* STATUSLINE */
			if((top = param[0]) < 1)
				top = 1;
			if(nparam < 2 || (bot = param[1]) == DEFAULT
			   || bot > screen->max_row + 1
			   || bot == 0)
				bot = screen->max_row+1;
			if (bot > top) {
				if(screen->scroll_amt)
					FlushScroll(screen);
				screen->top_marg = top-1;
				screen->bot_marg = bot-1;
				CursorSet(screen, 0, 0, term->flags);
			}
#endif
			parsestate = groundtable;
			break;

		 case CASE_DECREQTPARM:
			/* DECREQTPARM */
			if ((row = param[0]) == DEFAULT)
				row = 0;
			if (row == 0 || row == 1) {
				reply.a_type = CSI;
				reply.a_pintro = 0;
				reply.a_nparam = 7;
				reply.a_param[0] = row + 2;
				reply.a_param[1] = 1;	/* no parity */
				reply.a_param[2] = 1;	/* eight bits */
				reply.a_param[3] = 112;	/* transmit 9600 baud */
				reply.a_param[4] = 112;	/* receive 9600 baud */
				reply.a_param[5] = 1;	/* clock multiplier ? */
				reply.a_param[6] = 0;	/* STP flags ? */
				reply.a_inters = 0;
				reply.a_final  = 'x';
				unparseseq(&reply, screen->respond);
			}
			parsestate = groundtable;
			break;

		 case CASE_DECSET:
			/* DECSET */
			dpmodes(term, bitset);
			parsestate = groundtable;
			if(screen->TekEmu)
				return;
			break;

		 case CASE_DECRST:
			/* DECRST */
			dpmodes(term, bitclr);
			parsestate = groundtable;
			break;

		 case CASE_DECALN:
			/* DECALN */
			if(screen->cursor_state)
				HideCursor();
			for(row = screen->max_row ; row >= 0 ; row--) {
#ifdef NBUF_TYPE
				int i;

				bzero(screen->buf[i = (NBUF_TYPE * row) + ATTR_BUF],
				 col = screen->max_col + 1);
				for(cp = (unsigned char *)screen->buf[i + CODE_BUF] ; col > 0 ; col--)
					*cp++ = (unsigned char) 'E';
#ifdef MULTIFONT
				for(cp = (unsigned char *)screen->buf[i + FNUM_BUF] ; col > 0 ; col--)
					*cp++ = (unsigned char) 'B';
#endif /* MULTIFONT */
#ifdef COLOR
				bzero(screen->buf[i + COLOR_BUF], col);
#endif /* COLOR */
#else /* NBUF_TYPE */
				bzero(screen->buf[2 * row + 1],
				 col = screen->max_col + 1);
				for(cp = (unsigned char *)screen->buf[2 * row] ; col > 0 ; col--)
					*cp++ = (unsigned char) 'E';
#endif /* NBUF_TYPE */
			}
			ScrnRefresh(screen, 0, 0, screen->max_row + 1,
			 screen->max_col + 1, False);
			parsestate = groundtable;
			break;

		 case CASE_GSETS:
			screen->gsets[scstype] = c;
#ifdef MULTIFONT
			if (mbcs) {
				screen->gflags[scstype] |= MBSCS;
				mbcs = 0;
			} else {
				screen->gflags[scstype] &= ~MBSCS;
			}
			if (scs96) {
				screen->gflags[scstype] |= SCS96;
				scs96 = 0;
			} else {
				screen->gflags[scstype] &= ~SCS96;
			}
#endif /* MULTIFONT */
			parsestate = groundtable;
			break;

		 case CASE_DECSC:
			/* DECSC */
			CursorSave(term, &screen->sc);
			parsestate = groundtable;
			break;

		 case CASE_DECRC:
			/* DECRC */
			CursorRestore(term, &screen->sc);
			parsestate = groundtable;
			break;

		 case CASE_DECKPAM:
			/* DECKPAM */
			term->keyboard.flags |= KYPD_APL;
			update_appkeypad();
			parsestate = groundtable;
			break;

		 case CASE_DECKPNM:
			/* DECKPNM */
			term->keyboard.flags &= ~KYPD_APL;
			update_appkeypad();
			parsestate = groundtable;
			break;

		 case CASE_IND:
			/* IND */
#ifdef STATUSLINE
			if (!screen->instatus) {
				Index(screen, 1);
			}
#else /* STATUSLINE */
			Index(screen, 1);
#endif /* STATUSLINE */
			if (QLength(screen->display) > 0 ||
			    GetBytesAvailable (ConnectionNumber(screen->display)) > 0)
			  xevents();
			parsestate = groundtable;
			break;

		 case CASE_NEL:
			/* NEL */
#ifdef STATUSLINE
			if (!screen->instatus) {
				Index(screen, 1);
			}
#else /* STATUSLINE */
			Index(screen, 1);
#endif /* STATUSLINE */
			CarriageReturn(screen);
			
			if (QLength(screen->display) > 0 ||
			    GetBytesAvailable (ConnectionNumber(screen->display)) > 0)
			  xevents();
			parsestate = groundtable;
			break;

		 case CASE_HTS:
			/* HTS */
			TabSet(term->tabs, screen->cur_col);
			parsestate = groundtable;
			break;

		 case CASE_RI:
			/* RI */
#ifdef STATUSLINE
			if (!screen->instatus) {
				RevIndex(screen, 1);
			}
#else /* STATUSLINE */
			RevIndex(screen, 1);
#endif /* STATUSLINE */
			parsestate = groundtable;
			break;

		 case CASE_SS2:
			/* SS2 */
			screen->curss = 2;
			parsestate = groundtable;
			break;

		 case CASE_SS3:
			/* SS3 */
			screen->curss = 3;
			parsestate = groundtable;
			break;

		 case CASE_CSI_STATE:
			/* enter csi state */
			nparam = 1;
			param[0] = DEFAULT;
			parsestate = csitable;
			break;

		 case CASE_OSC:
			/* Operating System Command: ESC ] */
			do_osc(finput);
			parsestate = groundtable;
			break;

		 case CASE_RIS:
			/* RIS */
			VTReset(TRUE);
			parsestate = groundtable;
			break;

		 case CASE_LS2:
			/* LS2 */
			screen->curgl = 2;
			parsestate = groundtable;
			break;

		 case CASE_LS3:
			/* LS3 */
			screen->curgl = 3;
			parsestate = groundtable;
			break;

		 case CASE_LS3R:
			/* LS3R */
			screen->curgr = 3;
			parsestate = groundtable;
			break;

		 case CASE_LS2R:
			/* LS2R */
			screen->curgr = 2;
			parsestate = groundtable;
			break;

		 case CASE_LS1R:
			/* LS1R */
			screen->curgr = 1;
			parsestate = groundtable;
			break;

		 case CASE_XTERM_SAVE:
			savemodes(term);
			parsestate = groundtable;
			break;

		 case CASE_XTERM_RESTORE:
			restoremodes(term);
			parsestate = groundtable;
			break;
#ifdef STATUSLINE
		 case CASE_TO_STATUS:
			if ((c = param[0]) < 1) {
				c = 1;
			}
			ToStatus(screen, c - 1);
			update_statusline();
			parsestate = groundtable;
			break;

		case CASE_SHOW_STATUS:
			ShowStatus(screen);
			update_statusline();
			parsestate = groundtable;
			break;

		case CASE_HIDE_STATUS:
			HideStatus(screen);
			update_statusline();
			parsestate = groundtable;
			break;

		case CASE_FROM_STATUS:
			FromStatus(screen);
			parsestate = groundtable;
			break;

		case CASE_ERASE_STATUS:
			EraseStatus(screen);
			parsestate = groundtable;
			break;
#endif /* STATUSLINE */
		}
	}
}

static finput()
{
	return(doinput());
}


static char *v_buffer;		/* pointer to physical buffer */
static char *v_bufstr = NULL;	/* beginning of area to write */
static char *v_bufptr;		/* end of area to write */
static char *v_bufend;		/* end of physical buffer */
#define	ptymask()	(v_bufptr > v_bufstr ? pty_mask : 0)

/* Write data to the pty as typed by the user, pasted with the mouse,
   or generated by us in response to a query ESC sequence. */

v_write(f, d, len)
    int f;
    char *d;
    int len;
{
	int riten;
	int c = len;

	if (v_bufstr == NULL  &&  len > 0) {
	        v_buffer = XtMalloc(len);
		v_bufstr = v_buffer;
		v_bufptr = v_buffer;
		v_bufend = v_buffer + len;
	}
#ifdef DEBUG
	if (debug) {
	    fprintf(stderr, "v_write called with %d bytes (%d left over)",
		    len, v_bufptr - v_bufstr);
	    if (len > 1  &&  len < 10) fprintf(stderr, " \"%.*s\"", len, d);
	    fprintf(stderr, "\n");
	}
#endif

	if ((1 << f) != pty_mask)
		return(write(f, d, len));

	/*
	 * Append to the block we already have.
	 * Always doing this simplifies the code, and
	 * isn't too bad, either.  If this is a short
	 * block, it isn't too expensive, and if this is
	 * a long block, we won't be able to write it all
	 * anyway.
	 */

	if (len > 0) {
	    if (v_bufend < v_bufptr + len) { /* we've run out of room */
		if (v_bufstr != v_buffer) {
		    /* there is unused space, move everything down */
		    /* possibly overlapping memmove here */
#ifdef DEBUG
		    if (debug)
			fprintf(stderr, "moving data down %d\n",
				v_bufstr - v_buffer);
#endif
		    memmove( v_buffer, v_bufstr, v_bufptr - v_bufstr);
		    v_bufptr -= v_bufstr - v_buffer;
		    v_bufstr = v_buffer;
		}
		if (v_bufend < v_bufptr + len) {
		    /* still won't fit: get more space */
		    /* Don't use XtRealloc because an error is not fatal. */
		    int size = v_bufptr - v_buffer; /* save across realloc */
		    v_buffer = realloc(v_buffer, size + len);
		    if (v_buffer) {
#ifdef DEBUG
			if (debug)
			    fprintf(stderr, "expanded buffer to %d\n",
				    size + len);
#endif
			v_bufstr = v_buffer;
			v_bufptr = v_buffer + size;
			v_bufend = v_bufptr + len;
		    } else {
			/* no memory: ignore entire write request */
			fprintf(stderr, "%s: cannot allocate buffer space\n",
				xterm_name);
			v_buffer = v_bufstr; /* restore clobbered pointer */
			c = 0;
		    }
		}
	    }
	    if (v_bufend >= v_bufptr + len) {
		/* new stuff will fit */
		memmove( v_bufptr, d, len);
		v_bufptr += len;
	    }
	}

	/*
	 * Write out as much of the buffer as we can.
	 * Be careful not to overflow the pty's input silo.
	 * We are conservative here and only write
	 * a small amount at a time.
	 *
	 * If we can't push all the data into the pty yet, we expect write
	 * to return a non-negative number less than the length requested
	 * (if some data written) or -1 and set errno to EAGAIN,
	 * EWOULDBLOCK, or EINTR (if no data written).
	 *
	 * (Not all systems do this, sigh, so the code is actually
	 * a little more forgiving.)
	 */

#define MAX_PTY_WRITE 128	/* 1/2 POSIX minimum MAX_INPUT */

	if (v_bufptr > v_bufstr) {
	    riten = write(f, v_bufstr, v_bufptr - v_bufstr <= MAX_PTY_WRITE ?
			  	       v_bufptr - v_bufstr : MAX_PTY_WRITE);
	    if (riten < 0) {
#ifdef DEBUG
		if (debug) perror("write");
#endif
		riten = 0;
	    }
#ifdef DEBUG
	    if (debug)
		fprintf(stderr, "write called with %d, wrote %d\n",
			v_bufptr - v_bufstr <= MAX_PTY_WRITE ?
			v_bufptr - v_bufstr : MAX_PTY_WRITE,
			riten);
#endif
	    v_bufstr += riten;
	    if (v_bufstr >= v_bufptr) /* we wrote it all */
		v_bufstr = v_bufptr = v_buffer;
	}

	/*
	 * If we have lots of unused memory allocated, return it
	 */
	if (v_bufend - v_bufptr > 1024) { /* arbitrary hysteresis */
	    /* save pointers across realloc */
	    int start = v_bufstr - v_buffer;
	    int size = v_bufptr - v_buffer;
	    int allocsize = size ? size : 1;
	    
	    v_buffer = realloc(v_buffer, allocsize);
	    if (v_buffer) {
		v_bufstr = v_buffer + start;
		v_bufptr = v_buffer + size;
		v_bufend = v_buffer + allocsize;
#ifdef DEBUG
		if (debug)
		    fprintf(stderr, "shrunk buffer to %d\n", allocsize);
#endif
	    } else {
		/* should we print a warning if couldn't return memory? */
		v_buffer = v_bufstr - start; /* restore clobbered pointer */
	    }
	}
	return(c);
}

static int select_mask;
static int write_mask;
static int pty_read_bytes;

in_put()
{
    register TScreen *screen = &term->screen;
    register int i;
    static struct timeval select_timeout;

    for( ; ; ) {
	if (select_mask & pty_mask && eventMode == NORMAL) {
#ifdef ALLOWLOGGING
	    if (screen->logging)
		FlushLog(screen);
#endif
#ifdef MULTIFONT
	    if (pending) {
		*buffer = pending;
		bptr = buffer + 1;
		bcnt = BUF_SIZE - 1;
	    } else {
		bptr = buffer;
		bcnt = BUF_SIZE;
	    }
	    bcnt = read(screen->respond, (char *)bptr, bcnt);
#else /* MULTIFONT */
	    bcnt = read(screen->respond, (char *)(bptr = buffer), BUF_SIZE);
#endif /* MULTIFONT */
	    if (bcnt < 0) {
		if (errno == EIO)
		    Cleanup (0);
		else if (!E_TEST(errno))
		    Panic(
			  "input: read returned unexpected error (%d)\n",
			  errno);
	    } else if (bcnt == 0)
		Panic("input: read returned zero\n", 0);
	    else {
#ifdef MULTIFONT
		if (pending) {
		    pending = 0;
		    bptr = buffer;
		    bcnt++;
		}
#endif /* MULTIFONT */
		/* read from pty was successful */
		if (!screen->output_eight_bits) {
		    register int bc = bcnt;
		    register Char *b = bptr;

		    for (; bc > 0; bc--, b++) {
			*b &= (Char) 0x7f;
		    }
		}
		if ( screen->scrollWidget && screen->scrollttyoutput &&
		     screen->topline < 0)
		    WindowScroll(screen, 0);  /* Scroll to bottom */
		pty_read_bytes += bcnt;
		/* stop speed reading at some point to look for X stuff */
		/* (4096 is just a random large number.) */
		if (pty_read_bytes > 4096) {
		    select_mask &= ~pty_mask;
		}
		break;
	    }
	}
	pty_read_bytes = 0;
	/* update the screen */
	if (screen->scroll_amt)
	    FlushScroll(screen);
	if (screen->cursor_set && (screen->cursor_col != screen->cur_col
				   || screen->cursor_row != screen->cur_row)) {
	    if (screen->cursor_state)
		HideCursor();
#ifdef USE_XIM
	    if (screen->ic)
		SetPreedit(screen);
#endif /* USE_XIM */
	    ShowCursor();
	} else if (screen->cursor_set != screen->cursor_state) {
#ifdef USE_XIM
	    if (screen->cursor_set) {
		if (screen->ic)
		    SetPreedit(screen);
		ShowCursor();
	    }
#else /* USE_XIM */
	    if (screen->cursor_set)
		ShowCursor();
#endif /* USE_XIM */
	    else
		HideCursor();
	}

	XFlush(screen->display); /* always flush writes before waiting */

	/* Update the masks and, unless X events are already in the queue,
	   wait for I/O to be possible. */
	select_mask = Select_mask;
	write_mask = ptymask();
	select_timeout.tv_sec = 0;
	select_timeout.tv_usec = 0;
	i = select(max_plus1, &select_mask, &write_mask, (int *)NULL,
		   QLength(screen->display) ? &select_timeout
		   : (struct timeval *) NULL);
	if (i < 0) {
	    if (errno != EINTR)
		SysError(ERROR_SELECT);
	    continue;
	} 

	/* if there is room to write more data to the pty, go write more */
	if (write_mask & ptymask()) {
	    v_write(screen->respond, 0, 0); /* flush buffer */
	}

	/* if there are X events already in our queue, it
	   counts as being readable */
	if (QLength(screen->display) || (select_mask & X_mask)) {
	    xevents();
	}

    }
    bcnt--;
    return(*bptr++);
}

#ifdef MULTIFONT
int replaceTable[256] = {
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1,  0,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  3,  4,  5,  6, -1,
      7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  8,  9, 10, 11, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

char *nrcTable[] = {
/*    x23 x24 x40 x5b x5c x5d x5e x60 x7b x7c x7d x7e */
    "\043\244\100\133\134\135\136\140\173\174\175\020", /* @ IRV	*/
    "\243\044\100\133\134\135\136\140\173\174\175\020", /* A United Kingdom */
    "\043\044\100\133\134\135\136\140\173\174\175\176", /* B US ASCII	*/
    "\043\044\311\304\326\305\334\351\344\366\345\374", /* C Swedish	*/
    "\043\044\100\306\330\305\334\140\346\370\345\374", /* E Norwegian	*/
    "\043\044\247\304\326\334\136\140\344\366\374\337", /* K German	*/
    "\243\044\340\260\347\247\136\140\351\371\350\250", /* R French	*/
    "\243\044\247\260\347\351\136\371\340\362\350\354", /* Y Italian	*/
    "\243\044\247\241\321\277\136\140\260\361\347\176", /* Z Spanish	*/
};
#endif /* MULTIFONT */

/*
 * process a string of characters according to the character set indicated
 * by charset.  worry about end of line conditions (wraparound if selected).
 */
static void
dotext(screen, flags, charset, buf, ptr)
    register TScreen	*screen;
    unsigned	flags;
    char	charset;
    char	*buf;		/* start of characters to process */
    char	*ptr;		/* end */
{
	register char	*s;
	register int	len;
	register int	n;
	register int	next_col;
#ifdef MULTIFONT
	register char *nrc;
	int dowrapflag = 0;
	int fset;
#endif /* MULTIFONT */

#ifdef MULTIFONT
	if (flags & MBSCS) {
		switch (charset) {
		case '@':	/* JIS Old Kanji			*/
		case 'B':	/* JIS Old Kanji			*/
			fset = F_JISX0208_0;
			for (s = buf; s < ptr; ++s)
				*s &= 0x7f;
			break;

		case 'A':	/* GB Hanzi				*/
		case 'E':
			fset = F_GB2312_0;
			for (s = buf; s < ptr; ++s)
				*s &= 0x7f;
			break;

		case 'C':	/* KS Hangul/Hanja			*/
			fset = F_KSC5601_0;
			for (s = buf; s < ptr; ++s)
				*s &= 0x7f;
			break;

		default:	/* any character sets we don't recognize*/
			return;
		}
	} else if (flags & SCS96) {
		switch (charset) {
		case 'A':	/* ISO Latin 1 set			*/
			fset = F_ISO8859_1;
			for (s = buf; s < ptr; ++s)
				*s |= 0x80;
			break;
		case 'B':	/* ISO Latin 2 set			*/
			fset = F_ISO8859_2;
			for (s = buf; s < ptr; ++s)
				*s |= 0x80;
			break;
		case 'C':	/* ISO Latin 3 set			*/
			fset = F_ISO8859_3;
			for (s = buf; s < ptr; ++s)
				*s |= 0x80;
			break;
		case 'D':	/* ISO Latin 4 set			*/
			fset = F_ISO8859_4;
			for (s = buf; s < ptr; ++s)
				*s |= 0x80;
			break;
		case 'L':	/* ISO Latin 5 set			*/
			fset = F_ISO8859_5;
			for (s = buf; s < ptr; ++s)
				*s |= 0x80;
			break;
		case 'G':	/* ISO Latin 6 set			*/
			fset = F_ISO8859_6;
			for (s = buf; s < ptr; ++s)
				*s |= 0x80;
			break;
		case 'F':	/* ISO Latin 7 set			*/
			fset = F_ISO8859_7;
			for (s = buf; s < ptr; ++s)
				*s |= 0x80;
			break;
		case 'H':	/* ISO Latin 8 set			*/
			fset = F_ISO8859_8;
			for (s = buf; s < ptr; ++s)
				*s |= 0x80;
			break;
		case 'M':	/* ISO Latin 9 set			*/
			fset = F_ISO8859_9;
			for (s = buf; s < ptr; ++s)
				*s |= 0x80;
			break;
		default:	/* any character sets we don't recognize*/
			return;
		}
	} else {
		switch (charset) {
		case '@':	/* ISO IRV				*/
			nrc = nrcTable[0];
			goto do_replace;

		case 'A':	/* Uinted Kingdom set			*/
			nrc = nrcTable[1];
			goto do_replace;

#ifdef notdef
		case 'B':	/* ASCII set				*/
			nrc = nrcTable[2];
			goto do_replace;
#endif

		case 'C':	/* Swedish/Finish set			*/
			nrc = nrcTable[3];
			goto do_replace;

		case 'E':	/* Norwegian/Denish set			*/
			nrc = nrcTable[4];
			goto do_replace;

		case 'K':	/* German set				*/
			nrc = nrcTable[5];
			goto do_replace;

		case 'R':	/* French set				*/
			nrc = nrcTable[6];
			goto do_replace;

		case 'Y':	/* Italian set				*/
			nrc = nrcTable[7];
			goto do_replace;

		case 'Z':	/* Spanish set				*/
			nrc = nrcTable[8];
do_replace:
			fset = F_ISO8859_1;
			for (s = buf; s < ptr; ++s) {
				if (replaceTable[*s & 0x7f] >= 0) {
					*s = nrc[replaceTable[*s & 0x7f]];
				} else {
					*s &= 0x7f;
				}
			}
			break;

		case 'H':	/* JIS ROMAN set (wrong)		*/
		case 'J':	/* JIS ROMAN set			*/
			fset = F_JISX0201_0;
			for (s = buf; s < ptr; ++s)
				*s &= 0x7f;
			break;

		case 'I':	/* JIS KANA set				*/
			fset = F_JISX0201_0;
			for (s = buf; s < ptr; ++s)
				*s |= 0x80;
			break;

		case 'B':	/* ASCII set				*/
			fset = F_ISO8859_1;
			break;

		case '0':	/* special graphics (line drawing)	*/
			fset = F_ISO8859_1;
			for (s=buf; s<ptr; ++s)
				if (*s>=0x5f && *s<=0x7e)
					*s = *s == 0x5f ? 0x7f : *s - 0x5f;
			break;

		default:	/* any character sets we don't recognize*/
			return;
		}
	}
	flags |= fset << 28;
#else /* MULTIFONT */
	switch (charset) {
	case 'A':	/* United Kingdom set			*/
		for (s=buf; s<ptr; ++s)
			if (*s == '#')
				*s = '\036';	/* UK pound sign*/
		break;

	case 'B':	/* ASCII set				*/
		break;

	case '0':	/* special graphics (line drawing)	*/
		for (s=buf; s<ptr; ++s)
			if (*s>=0x5f && *s<=0x7e)
				*s = *s == 0x5f ? 0x7f : *s - 0x5f;
		break;

	default:	/* any character sets we don't recognize*/
		return;
	}
#endif /* MULTIFONT */

	len = ptr - buf; 
	ptr = buf;
	while (len > 0) {
		n = screen->max_col - screen->cur_col +1;
		if (n <= 1) {
#ifdef STATUSLINE
			if (screen->do_wrap && (flags&WRAPAROUND) &&
			    !screen->instatus) {
#else /* STATUSLINE */
			if (screen->do_wrap && (flags&WRAPAROUND)) {
#endif /* STATUSLINE */
			    /* mark that we had to wrap this line */
			    ScrnSetAttributes(screen, screen->cur_row, 0,
					      LINEWRAPPED, LINEWRAPPED, 1);
			    Index(screen, 1);
			    screen->cur_col = 0;
			    screen->do_wrap = 0;
			    n = screen->max_col+1;
			} else
			    n = 1;
		}
		if (len < n)
			n = len;
#ifdef MULTIFONT
		if ((flags & MBSCS) && (n & 1)
#ifdef STATUSLINE
		    && !screen->instatus
#endif /* STATUSLINE */
		   ) {
			n--;
			dowrapflag = TRUE;
		} else {
			dowrapflag = FALSE;
		}
		if (n == 0) {
			if (!(flags & WRAPAROUND))
				len -= 1;
			screen->do_wrap = TRUE;
			continue;
		}
#endif /* MULTIFONT */
		next_col = screen->cur_col + n;
		WriteText(screen, ptr, n, flags);
		/*
		 * the call to WriteText updates screen->cur_col.
		 * If screen->cur_col != next_col, we must have
		 * hit the right margin, so set the do_wrap flag.
		 */
#ifdef MULTIFONT
		screen->do_wrap = (screen->cur_col < next_col || dowrapflag);
#else /* MULTIFONT */
		screen->do_wrap = (screen->cur_col < next_col);
#endif /* MULTIFONT */
		len -= n;
		ptr += n;
	}
}
 
/*
 * write a string str of length len onto the screen at
 * the current cursor position.  update cursor position.
 */
static void
WriteText(screen, str, len, flags)
    register TScreen	*screen;
    register char	*str;
    register int	len;
    unsigned		flags;
{
	register int cx, cy;
	register unsigned fgs = flags;
	GC	currentGC;
#ifdef COLOR
	XGCValues xgc;
#ifdef MULTIFONT
	XGCValues rxgc;
#endif /* MULTIFONT */
#endif /* COLOR */
#ifdef MULTIFONT
	GC	revcurrentGC;
	register int fset = fgs >> 28;
#endif /* MULTIFONT */
 
#ifdef MULTIFONT
if (!screen->fnt_norm) load_codeset_font(screen, fset);
if (screen->fnt_norm) {
#endif /* MULTIFONT */
#ifdef STATUSLINE
   if(screen->instatus && screen->reversestatus) {
	fgs ^= INVERSE;
   }
   if(screen->cur_row - screen->topline <= screen->max_row ||
      screen->instatus) {
#else /* STATUSLINE */
   if(screen->cur_row - screen->topline <= screen->max_row) {
#endif /* STATUSLINE */
	/*
	if(screen->cur_row == screen->cursor_row && screen->cur_col <=
	 screen->cursor_col && screen->cursor_col <= screen->cur_col + len - 1)
		screen->cursor_state = OFF;
	 */
	if(screen->cursor_state)
		HideCursor();

	/*
	 *	make sure that the correct GC is current
	 */

#ifdef COLOR
	if (fgs & INVERSE) {
		if (fgs & BOLD) {
			currentGC = screen->reverseboldGC;
#ifdef MULTIFONT
			revcurrentGC = screen->normalboldGC;
#endif /* MULTIFONT */
		} else {
			currentGC = screen->reverseGC;
#ifdef MULTIFONT
			revcurrentGC = screen->normalGC;
#endif /* MULTIFONT */
		}
		if (fgs & FORE_MASK) {
		    xgc.background = screen->color[(fgs & FORE_MASK) >> 20];
#ifdef MULTIFONT
		    rxgc.foreground = screen->color[(fgs & FORE_MASK) >> 20];
#endif /* MULTIFONT */
		} else {
		    xgc.background = screen->foreground;
#ifdef MULTIFONT
		    rxgc.foreground = screen->foreground;
#endif /* MULTIFONT */
		}
		if (fgs & BACK_MASK) {
		    xgc.foreground = screen->color[(fgs & BACK_MASK) >> 24];
#ifdef MULTIFONT
		    rxgc.background = screen->color[(fgs & BACK_MASK) >> 24];
#endif /* MULTIFONT */
		} else {
		    xgc.foreground = term->core.background_pixel;
#ifdef MULTIFONT
		    rxgc.background = term->core.background_pixel;
#endif /* MULTIFONT */
		}
	} else {
		if (fgs & BOLD) {
			currentGC = screen->normalboldGC;
#ifdef MULTIFONT
			revcurrentGC = screen->reverseboldGC;
#endif /* MULTIFONT */
		} else {
			currentGC = screen->normalGC;
#ifdef MULTIFONT
			revcurrentGC = screen->reverseGC;
#endif /* MULTIFONT */
		}
		if (fgs & FORE_MASK) {
		    xgc.foreground = screen->color[(fgs & FORE_MASK) >> 20];
#ifdef MULTIFONT
		    rxgc.background = screen->color[(fgs & FORE_MASK) >> 20];
#endif /* MULTIFONT */
		} else {
		    xgc.foreground = screen->foreground;
#ifdef MULTIFONT
		    rxgc.background = screen->foreground;
#endif /* MULTIFONT */
		}
		if (fgs & BACK_MASK) {
		    xgc.background = screen->color[(fgs & BACK_MASK) >> 24];
#ifdef MULTIFONT
		    rxgc.foreground = screen->color[(fgs & BACK_MASK) >> 24];
#endif /* MULTIFONT */
		} else {
		    xgc.background = term->core.background_pixel;
#ifdef MULTIFONT
		    rxgc.foreground = term->core.background_pixel;
#endif /* MULTIFONT */
		}
	}
	XChangeGC(screen->display, currentGC, GCForeground|GCBackground, &xgc);
#ifdef MULTIFONT
	XChangeGC(screen->display, revcurrentGC, GCForeground|GCBackground, &rxgc);
#endif /* MULTIFONT */
#else /* COLOR */
#ifdef MULTIFONT
	if (fgs & BOLD) {
		if (fgs & INVERSE) {
			currentGC = screen->reverseboldGC;
			revcurrentGC = screen->normalboldGC;
		} else {
			currentGC = screen->normalboldGC;
			revcurrentGC = screen->reverseboldGC;
		}
	} else {  /* not bold */
		if (fgs & INVERSE) {
			currentGC = screen->reverseGC;
			revcurrentGC = screen->normalGC;
		} else {
			currentGC = screen->normalGC;
			revcurrentGC = screen->reverseGC;
		}
	}
#else /* MULTIFONT */
	if (fgs & BOLD)
		if (fgs & INVERSE)
			currentGC = screen->reverseboldGC;
		else	currentGC = screen->normalboldGC;
	else  /* not bold */
		if (fgs & INVERSE)
			currentGC = screen->reverseGC;
		else	currentGC = screen->normalGC;
#endif /* MULTIFONT */
#endif /* COLOR */

	if (fgs & INSERT)
		InsertChar(screen, len);
      if (!(AddToRefresh(screen))) {
		if(screen->scroll_amt)
			FlushScroll(screen);
	cx = CursorX(screen, screen->cur_col);
#ifdef MULTIFONT
	cy = CursorY(screen, screen->cur_row)+screen->max_ascent + screen->top_space;
#else /* MULTIFONT */
	cy = CursorY(screen, screen->cur_row)+screen->fnt_norm->ascent;
#endif /* MULTIFONT */
#ifdef MULTIFONT
	if (screen->top_space) {
		XFillRectangle(screen->display, TextWindow(screen),
			revcurrentGC, cx, CursorY(screen, screen->cur_row),
			len * FontWidth(screen),
			screen->top_space);
	}
	if (fgs & MBSCS) {
		XDrawImageString16(screen->display, TextWindow(screen), currentGC,
				cx, cy, (XChar2b *)str, len/2);

		if((fgs & BOLD) && screen->enbolden) 
			if (currentGC == screen->normalGC || screen->reverseGC)
				XDrawString16(screen->display, TextWindow(screen),
					currentGC,cx + 1, cy, (XChar2b *)str, len/2);

	} else {
		XDrawImageString(screen->display, TextWindow(screen), currentGC,
				cx, cy, str, len);

		if((fgs & BOLD) && screen->enbolden) 
			if (currentGC == screen->normalGC || screen->reverseGC)
				XDrawString(screen->display, TextWindow(screen),
					currentGC,cx + 1, cy, str, len);

	}
	if (screen->bottom_space) {
		XFillRectangle(screen->display, TextWindow(screen),
			revcurrentGC, cx, cy + screen->max_descent,
			len * FontWidth(screen),
			screen->bottom_space);
	}
	if(fgs & UNDERLINE) 
		XDrawLine(screen->display, TextWindow(screen), currentGC,
			cx, cy+1,
			cx + len * FontWidth(screen), cy+1);
#else /* MULTIFONT */
 	XDrawImageString(screen->display, TextWindow(screen), currentGC,
			cx, cy, str, len);

	if((fgs & BOLD) && screen->enbolden) 
		if (currentGC == screen->normalGC || screen->reverseGC)
			XDrawString(screen->display, TextWindow(screen),
			      	currentGC,cx + 1, cy, str, len);

	if(fgs & UNDERLINE) 
		XDrawLine(screen->display, TextWindow(screen), currentGC,
			cx, cy+screen->ul_position,
			cx + len * FontWidth(screen), cy+1);
#endif /* MULTIFONT */
	/*
	 * the following statements compile data to compute the average 
	 * number of characters written on each call to XText.  The data
	 * may be examined via the use of a "hidden" escape sequence.
	 */
	ctotal += len;
	++ntotal;
      }
    }
#ifdef MULTIFONT
}
#endif /* MULTIFONT */
	ScreenWrite(screen, str, flags, len);
	CursorForward(screen, len);
}
 
/*
 * process ANSI modes set, reset
 */
ansi_modes(termw, func)
    XtermWidget	termw;
    int		(*func)();
{
	register int	i;

	for (i=0; i<nparam; ++i) {
		switch (param[i]) {
		case 4:			/* IRM				*/
			(*func)(&termw->flags, INSERT);
			break;

		case 20:		/* LNM				*/
			(*func)(&termw->flags, LINEFEED);
			update_autolinefeed();
			break;
		}
	}
}

/*
 * process DEC private modes set, reset
 */
dpmodes(termw, func)
    XtermWidget	termw;
    void (*func)();
{
	register TScreen	*screen	= &termw->screen;
	register int	i, j;

	for (i=0; i<nparam; ++i) {
		switch (param[i]) {
		case 1:			/* DECCKM			*/
			(*func)(&termw->keyboard.flags, CURSOR_APL);
			update_appcursor();
			break;
		case 2:			/* ANSI/VT52 mode		*/
			if (func == bitset) {
				screen->gsets[0] =
					screen->gsets[1] =
					screen->gsets[2] =
					screen->gsets[3] = 'B';
				screen->curgl = 0;
				screen->curgr = 2;
#ifdef MULTIFONT
				screen->gflags[0] =
					screen->gflags[1] =
					screen->gflags[2] =
					screen->gflags[3] = 0;
#endif /* MULTIFONT */
			}
			break;
		case 3:			/* DECCOLM			*/
			if(screen->c132) {
				ClearScreen(screen);
				CursorSet(screen, 0, 0, termw->flags);
				if((j = func == bitset ? 132 : 80) !=
				 ((termw->flags & IN132COLUMNS) ? 132 : 80) ||
				 j != screen->max_col + 1) {
				        Dimension replyWidth, replyHeight;
					XtGeometryResult status;

					status = XtMakeResizeRequest (
					    (Widget) termw, 
					    (Dimension) FontWidth(screen) * j
					        + 2*screen->border
						+ screen->scrollbar,
					    (Dimension) FontHeight(screen)
						* (screen->max_row + 1)
#ifdef StatusHeight
						+ StatusHeight(screen)
#endif /* StatusHeight */
						+ 2 * screen->border,
					    &replyWidth, &replyHeight);

					if (status == XtGeometryYes ||
					    status == XtGeometryDone) {
					    ScreenResize (&termw->screen,
							  replyWidth,
							  replyHeight,
							  &termw->flags);
					}
				}
				(*func)(&termw->flags, IN132COLUMNS);
			}
			break;
		case 4:			/* DECSCLM (slow scroll)	*/
			if (func == bitset) {
				screen->jumpscroll = 0;
				if (screen->scroll_amt)
					FlushScroll(screen);
			} else
				screen->jumpscroll = 1;
			(*func)(&termw->flags, SMOOTHSCROLL);
			update_jumpscroll();
			break;
		case 5:			/* DECSCNM			*/
			j = termw->flags;
			(*func)(&termw->flags, REVERSE_VIDEO);
			if ((termw->flags ^ j) & REVERSE_VIDEO)
				ReverseVideo(termw);
			/* update_reversevideo done in RevVid */
			break;

		case 6:			/* DECOM			*/
			(*func)(&termw->flags, ORIGIN);
			CursorSet(screen, 0, 0, termw->flags);
			break;

		case 7:			/* DECAWM			*/
			(*func)(&termw->flags, WRAPAROUND);
			update_autowrap();
			break;
		case 8:			/* DECARM			*/
			/* ignore autorepeat */
			break;
		case 9:			/* MIT bogus sequence		*/
			if(func == bitset)
				screen->send_mouse_pos = 1;
			else
				screen->send_mouse_pos = 0;
			break;
#ifdef EXTEND
		case 25:
			if (func == bitset) {
				(*func)(&termw->flags, ENABLECURSOR);
				ShowCursor();
			} else {
				HideCursor();
				(*func)(&termw->flags, ENABLECURSOR);
			}
			break;
#endif /* EXTEND */
		case 38:		/* DECTEK			*/
			if(func == bitset && !(screen->inhibit & I_TEK)) {
#ifdef ALLOWLOGGING
				if(screen->logging) {
					FlushLog(screen);
					screen->logstart = Tbuffer;
				}
#endif
				screen->TekEmu = TRUE;
			}
			break;
		case 40:		/* 132 column mode		*/
			screen->c132 = (func == bitset);
			update_allow132();
			break;
		case 41:		/* curses hack			*/
			screen->curses = (func == bitset);
			update_cursesemul();
			break;
		case 44:		/* margin bell			*/
			screen->marginbell = (func == bitset);
			if(!screen->marginbell)
				screen->bellarmed = -1;
			update_marginbell();
			break;
		case 45:		/* reverse wraparound	*/
			(*func)(&termw->flags, REVERSEWRAP);
			update_reversewrap();
			break;
#ifdef ALLOWLOGGING
		case 46:		/* logging		*/
#ifdef ALLOWLOGFILEONOFF
			/*
			 * if this feature is enabled, logging may be 
			 * enabled and disabled via escape sequences.
			 */
			if(func == bitset)
				StartLog(screen);
			else
				CloseLog(screen);
#else
			Bell();
			Bell();
#endif /* ALLOWLOGFILEONOFF */
			break;
#endif
		case 47:		/* alternate buffer */
			if (!termw->misc.titeInhibit) {
			    if(func == bitset)
				ToAlternate(screen);
			    else
				FromAlternate(screen);
			}
			break;
#ifdef STATUSLINE
		case 48:
			j = screen->reversestatus;
			screen->reversestatus = (func == bitset);

			if (j != screen->reversestatus) {
				ScrnRefresh(screen, screen->max_row + 1, 0, 1,
					    screen->max_col + 1, False);
			}
			update_reversestatus();
#endif /* STATUSLINE */
		case 1000:		/* xterm bogus sequence		*/
			if(func == bitset)
				screen->send_mouse_pos = 2;
			else
				screen->send_mouse_pos = 0;
			break;
		case 1001:		/* xterm sequence w/hilite tracking */
			if(func == bitset)
				screen->send_mouse_pos = 3;
			else
				screen->send_mouse_pos = 0;
			break;
		}
	}
}

/*
 * process xterm private modes save
 */
savemodes(termw)
    XtermWidget termw;
{
	register TScreen	*screen	= &termw->screen;
	register int i;

	for (i = 0; i < nparam; i++) {
		switch (param[i]) {
		case 1:			/* DECCKM			*/
			screen->save_modes[0] = termw->keyboard.flags &
			 CURSOR_APL;
			break;
		case 3:			/* DECCOLM			*/
			if(screen->c132)
			    screen->save_modes[1] = termw->flags & IN132COLUMNS;
			break;
		case 4:			/* DECSCLM (slow scroll)	*/
			screen->save_modes[2] = termw->flags & SMOOTHSCROLL;
			break;
		case 5:			/* DECSCNM			*/
			screen->save_modes[3] = termw->flags & REVERSE_VIDEO;
			break;
		case 6:			/* DECOM			*/
			screen->save_modes[4] = termw->flags & ORIGIN;
			break;

		case 7:			/* DECAWM			*/
			screen->save_modes[5] = termw->flags & WRAPAROUND;
			break;
		case 8:			/* DECARM			*/
			/* ignore autorepeat */
			break;
		case 9:			/* mouse bogus sequence */
			screen->save_modes[7] = screen->send_mouse_pos;
			break;
		case 40:		/* 132 column mode		*/
			screen->save_modes[8] = screen->c132;
			break;
		case 41:		/* curses hack			*/
			screen->save_modes[9] = screen->curses;
			break;
		case 44:		/* margin bell			*/
			screen->save_modes[12] = screen->marginbell;
			break;
		case 45:		/* reverse wraparound	*/
			screen->save_modes[13] = termw->flags & REVERSEWRAP;
			break;
#ifdef ALLOWLOGGING
		case 46:		/* logging		*/
			screen->save_modes[14] = screen->logging;
			break;
#endif
		case 47:		/* alternate buffer		*/
			screen->save_modes[15] = screen->alternate;
			break;
#ifdef STATUSLINE
		case 48:		/* alternate buffer		*/
			screen->save_modes[16] = screen->reversestatus;
			break;
#endif /* STATUSLINE */
		case 1000:		/* mouse bogus sequence		*/
		case 1001:
			screen->save_modes[7] = screen->send_mouse_pos;
			break;
		}
	}
}

/*
 * process xterm private modes restore
 */
restoremodes(termw)
    XtermWidget termw;
{
	register TScreen	*screen	= &termw->screen;
	register int i, j;

	for (i = 0; i < nparam; i++) {
		switch (param[i]) {
		case 1:			/* DECCKM			*/
			termw->keyboard.flags &= ~CURSOR_APL;
			termw->keyboard.flags |= screen->save_modes[0] &
			 CURSOR_APL;
			update_appcursor();
			break;
		case 3:			/* DECCOLM			*/
			if(screen->c132) {
				ClearScreen(screen);
				CursorSet(screen, 0, 0, termw->flags);
				if((j = (screen->save_modes[1] & IN132COLUMNS)
				 ? 132 : 80) != ((termw->flags & IN132COLUMNS)
				 ? 132 : 80) || j != screen->max_col + 1) {
				        Dimension replyWidth, replyHeight;
					XtGeometryResult status;
					status = XtMakeResizeRequest (
					    (Widget) termw,
					    (Dimension) FontWidth(screen) * j 
						+ 2*screen->border
						+ screen->scrollbar,
					    (Dimension) FontHeight(screen)
						* (screen->max_row + 1)
#ifdef StatusHeight
						+ StatusHeight(screen)
#endif /* StatusHeight */
						+ 2*screen->border,
					    &replyWidth, &replyHeight);

					if (status == XtGeometryYes ||
					    status == XtGeometryDone) {
					    ScreenResize (&termw->screen,
							  replyWidth,
							  replyHeight,
							  &termw->flags);
					}
				}
				termw->flags &= ~IN132COLUMNS;
				termw->flags |= screen->save_modes[1] &
				 IN132COLUMNS;
			}
			break;
		case 4:			/* DECSCLM (slow scroll)	*/
			if (screen->save_modes[2] & SMOOTHSCROLL) {
				screen->jumpscroll = 0;
				if (screen->scroll_amt)
					FlushScroll(screen);
			} else
				screen->jumpscroll = 1;
			termw->flags &= ~SMOOTHSCROLL;
			termw->flags |= screen->save_modes[2] & SMOOTHSCROLL;
			update_jumpscroll();
			break;
		case 5:			/* DECSCNM			*/
			if((screen->save_modes[3] ^ termw->flags) & REVERSE_VIDEO) {
				termw->flags &= ~REVERSE_VIDEO;
				termw->flags |= screen->save_modes[3] & REVERSE_VIDEO;
				ReverseVideo(termw);
				/* update_reversevideo done in RevVid */
			}
			break;
		case 6:			/* DECOM			*/
			termw->flags &= ~ORIGIN;
			termw->flags |= screen->save_modes[4] & ORIGIN;
			CursorSet(screen, 0, 0, termw->flags);
			break;

		case 7:			/* DECAWM			*/
			termw->flags &= ~WRAPAROUND;
			termw->flags |= screen->save_modes[5] & WRAPAROUND;
			update_autowrap();
			break;
		case 8:			/* DECARM			*/
			/* ignore autorepeat */
			break;
		case 9:			/* MIT bogus sequence		*/
			screen->send_mouse_pos = screen->save_modes[7];
			break;
		case 40:		/* 132 column mode		*/
			screen->c132 = screen->save_modes[8];
			update_allow132();
			break;
		case 41:		/* curses hack			*/
			screen->curses = screen->save_modes[9];
			update_cursesemul();
			break;
		case 44:		/* margin bell			*/
			if(!(screen->marginbell = screen->save_modes[12]))
				screen->bellarmed = -1;
			update_marginbell();
			break;
		case 45:		/* reverse wraparound	*/
			termw->flags &= ~REVERSEWRAP;
			termw->flags |= screen->save_modes[13] & REVERSEWRAP;
			update_reversewrap();
			break;
#ifdef ALLOWLOGGING
		case 46:		/* logging		*/
#ifdef ALLOWLOGFILEONOFF
			if(screen->save_modes[14])
				StartLog(screen);
			else
				CloseLog(screen);
#endif /* ALLOWLOGFILEONOFF */
			/* update_logging done by StartLog and CloseLog */
			break;
#endif
		case 47:		/* alternate buffer */
			if (!termw->misc.titeInhibit) {
			    if(screen->save_modes[15])
				ToAlternate(screen);
			    else
				FromAlternate(screen);
			    /* update_altscreen done by ToAlt and FromAlt */
			}
			break;
#ifdef STATUSLINE
		case 48:
			if (screen->save_modes[16] != screen->reversestatus) {
				screen->reversestatus = screen->save_modes[16];
				ScrnRefresh(screen, screen->max_row + 1, 0, 1,
					    screen->max_col + 1, False);
			}
			update_reversestatus();
			break;
#endif /* STATUSLINE */
		case 1000:		/* mouse bogus sequence		*/
		case 1001:
			screen->send_mouse_pos = screen->save_modes[7];
			break;
		}
	}
}

/*
 * set a bit in a word given a pointer to the word and a mask.
 */
static void bitset(p, mask)
    unsigned *p;
    int mask;
{
	*p |= mask;
}

/*
 * clear a bit in a word given a pointer to the word and a mask.
 */
static void bitclr(p, mask)
    unsigned *p;
    int mask;
{
	*p &= ~mask;
}

unparseseq(ap, fd)
    register ANSI *ap;
    int fd;
{
	register int	c;
	register int	i;
	register int	inters;

	c = ap->a_type;
	if (c>=0x80 && c<=0x9F) {
		unparseputc(ESC, fd);
		c -= 0x40;
	}
	unparseputc(c, fd);
	c = ap->a_type;
	if (c==ESC || c==DCS || c==CSI || c==OSC || c==PM || c==APC) {
		if (ap->a_pintro != 0)
			unparseputc((char) ap->a_pintro, fd);
		for (i=0; i<ap->a_nparam; ++i) {
			if (i != 0)
				unparseputc(';', fd);
			unparseputn((unsigned int) ap->a_param[i], fd);
		}
		inters = ap->a_inters;
		for (i=3; i>=0; --i) {
			c = (inters >> (8*i)) & 0xff;
			if (c != 0)
				unparseputc(c, fd);
		}
		unparseputc((char) ap->a_final, fd);
	}
}

unparseputn(n, fd)
unsigned int	n;
int fd;
{
	unsigned int	q;

	q = n/10;
	if (q != 0)
		unparseputn(q, fd);
	unparseputc((char) ('0' + (n%10)), fd);
}

unparseputc(c, fd)
char c;
int fd;
{
	char	buf[2];
	register i = 1;
	extern XtermWidget term;

	if((buf[0] = c) == '\r' && (term->flags & LINEFEED)) {
		buf[1] = '\n';
		i++;
	}
	v_write(fd, buf, i);
}

unparsefputs (s, fd)
    register char *s;
    int fd;
{
    if (s) {
	while (*s) unparseputc (*s++, fd);
    }
}

static void SwitchBufs();

ToAlternate(screen)
register TScreen *screen;
{
	extern ScrnBuf Allocate();

	if(screen->alternate)
		return;
	if(!screen->altbuf)
		screen->altbuf = Allocate(screen->max_row + 1, screen->max_col
		 + 1, &screen->abuf_address);
	SwitchBufs(screen);
	screen->alternate = TRUE;
	update_altscreen();
}

FromAlternate(screen)
register TScreen *screen;
{
	if(!screen->alternate)
		return;
	screen->alternate = FALSE;
	SwitchBufs(screen);
	update_altscreen();
}

static void
SwitchBufs(screen)
    register TScreen *screen;
{
	register int rows, top;

	if(screen->cursor_state)
		HideCursor();
	rows = screen->max_row + 1;
	SwitchBufPtrs(screen);
	TrackText(0, 0, 0, 0);	/* remove any highlighting */
	if((top = -screen->topline) <= screen->max_row) {
		if(screen->scroll_amt)
			FlushScroll(screen);
#ifdef StatusHeight
		if(top == 0 && !StatusHeight(screen))
#else /* StatusHeight */
		if(top == 0)
#endif /* StatusHeight */
			XClearWindow(screen->display, TextWindow(screen));
		else
			XClearArea(
			    screen->display,
			    TextWindow(screen),
			    (int) screen->border + screen->scrollbar,
			    (int) top * FontHeight(screen) + screen->border,
			    (unsigned) Width(screen),
			    (unsigned) (screen->max_row - top + 1)
				* FontHeight(screen),
			    FALSE);
	}
	ScrnRefresh(screen, 0, 0, rows, screen->max_col + 1, False);
}

/* swap buffer line pointers between alt and regular screens */

SwitchBufPtrs(screen)
    register TScreen *screen;
{
    register int rows = screen->max_row + 1;
#ifdef NBUF_TYPE
    char *save [NBUF_TYPE * MAX_ROWS];

    memmove( (char *)save, (char *)screen->buf, NBUF_TYPE * sizeof(char *) * rows);
    memmove( (char *)screen->buf, (char *)screen->altbuf, 
	  NBUF_TYPE * sizeof(char *) * rows);
    memmove( (char *)screen->altbuf, (char *)save, NBUF_TYPE * sizeof(char *) * rows);
#else /* NBUF_TYPE */
    char *save [2 * MAX_ROWS];

    memmove( (char *)save, (char *)screen->buf, 2 * sizeof(char *) * rows);
    memmove( (char *)screen->buf, (char *)screen->altbuf, 
	  2 * sizeof(char *) * rows);
    memmove( (char *)screen->altbuf, (char *)save, 2 * sizeof(char *) * rows);
#endif /* NBUF_TYPE */
}

VTRun()
{
	register TScreen *screen = &term->screen;
	register int i;
	
	if (!screen->Vshow) {
	    set_vt_visibility (TRUE);
	} 
	update_vttekmode();
	update_vtshow();
	update_tekshow();
	set_vthide_sensitivity();

	if (screen->allbuf == NULL) VTallocbuf ();

	screen->cursor_state = OFF;
	screen->cursor_set = ON;

	bcnt = 0;
	bptr = buffer;
	while(Tpushb > Tpushback) {
		*bptr++ = *--Tpushb;
		bcnt++;
	}
	bcnt += (i = Tbcnt);
	for( ; i > 0 ; i--)
		*bptr++ = *Tbptr++;
	bptr = buffer;
	if(!setjmp(VTend))
		VTparse();
	HideCursor();
	screen->cursor_set = OFF;
}

/*ARGSUSED*/
static void VTExpose(w, event, region)
    Widget w;
    XEvent *event;
    Region region;
{
	register TScreen *screen = &term->screen;

#ifdef DEBUG
	if(debug)
		fputs("Expose\n", stderr);
#endif	/* DEBUG */
	if (event->type == Expose)
		HandleExposure (screen, event);
}

static void VTGraphicsOrNoExpose (event)
    XEvent *event;
{
	register TScreen *screen = &term->screen;
	if (screen->incopy <= 0) {
		screen->incopy = 1;
		if (screen->scrolls > 0)
			screen->scrolls--;
	}
	if (event->type == GraphicsExpose)
	  if (HandleExposure (screen, event))
		screen->cursor_state = OFF;
	if ((event->type == NoExpose) || ((XGraphicsExposeEvent *)event)->count == 0) {
		if (screen->incopy <= 0 && screen->scrolls > 0)
			screen->scrolls--;
		if (screen->scrolls)
			screen->incopy = -1;
		else
			screen->incopy = 0;
	}
}

/*ARGSUSED*/
static void VTNonMaskableEvent (w, closure, event, cont)
Widget w;			/* unused */
XtPointer closure;		/* unused */
XEvent *event;
Boolean *cont;			/* unused */
{
    switch (event->type) {
       case GraphicsExpose:
       case NoExpose:
	  VTGraphicsOrNoExpose (event);
	  break;
      }
}




static void VTResize(w)
    Widget w;
{
    if (XtIsRealized(w))
      ScreenResize (&term->screen, term->core.width, term->core.height,
		    &term->flags);
}

				
extern Atom wm_delete_window;	/* for ICCCM delete window */

static String xterm_trans =
    "<ClientMessage>WM_PROTOCOLS: DeleteWindow()\n\
     <MappingNotify>: KeyboardMapping()\n";

int VTInit ()
{
    register TScreen *screen = &term->screen;
    Widget vtparent = term->core.parent;

    XtRealizeWidget (vtparent);
    XtOverrideTranslations(vtparent, XtParseTranslationTable(xterm_trans));
    (void) XSetWMProtocols (XtDisplay(vtparent), XtWindow(vtparent),
			    &wm_delete_window, 1);

    if (screen->allbuf == NULL) VTallocbuf ();
    return (1);
}

static void VTallocbuf ()
{
    register TScreen *screen = &term->screen;
#ifdef STATUSLINE
    int nrows = screen->max_row + 2;
#else /* STATUSLINE */
    int nrows = screen->max_row + 1;
#endif /* STATUSLINE */
    extern ScrnBuf Allocate();

    /* allocate screen buffer now, if necessary. */
    if (screen->scrollWidget)
      nrows += screen->savelines;
    screen->allbuf = Allocate (nrows, screen->max_col + 1,
     &screen->sbuf_address);
    if (screen->scrollWidget)
#ifdef NBUF_TYPE
      screen->buf = &screen->allbuf[NBUF_TYPE * screen->savelines];
#else /* NBUF_TYPE */
      screen->buf = &screen->allbuf[2 * screen->savelines];
#endif /* NBUF_TYPE */
    else
      screen->buf = screen->allbuf;
    return;
}

static void VTClassInit ()
{
    XtAddConverter(XtRString, XtRGravity, XmuCvtStringToGravity,
		   (XtConvertArgList) NULL, (Cardinal) 0);
}


/* ARGSUSED */
static void VTInitialize (wrequest, wnew, args, num_args)
   Widget wrequest, wnew;
   ArgList args;
   Cardinal *num_args;
{
   XtermWidget request = (XtermWidget) wrequest;
   XtermWidget new     = (XtermWidget) wnew;
   int i;

   /* Zero out the entire "screen" component of "new" widget,
      then do field-by-field assigment of "screen" fields
      that are named in the resource list. */

   bzero ((char *) &new->screen, sizeof(new->screen));
   new->screen.c132 = request->screen.c132;
   new->screen.curses = request->screen.curses;
   new->screen.hp_ll_bc = request->screen.hp_ll_bc;
   new->screen.foreground = request->screen.foreground;
   new->screen.cursorcolor = request->screen.cursorcolor;
   new->screen.border = request->screen.border;
   new->screen.jumpscroll = request->screen.jumpscroll;
#ifdef ALLOWLOGGING
   new->screen.logfile = request->screen.logfile;
#endif
   new->screen.marginbell = request->screen.marginbell;
   new->screen.mousecolor = request->screen.mousecolor;
   new->screen.mousecolorback = request->screen.mousecolorback;
   new->screen.multiscroll = request->screen.multiscroll;
   new->screen.nmarginbell = request->screen.nmarginbell;
   new->screen.savelines = request->screen.savelines;
   new->screen.scrolllines = request->screen.scrolllines;
   new->screen.scrollttyoutput = request->screen.scrollttyoutput;
   new->screen.scrollkey = request->screen.scrollkey;
   new->screen.visualbell = request->screen.visualbell;
   new->screen.TekEmu = request->screen.TekEmu;
   new->misc.re_verse = request->misc.re_verse;
   new->screen.multiClickTime = request->screen.multiClickTime;
   new->screen.bellSuppressTime = request->screen.bellSuppressTime;
   new->screen.charClass = request->screen.charClass;
   new->screen.cutNewline = request->screen.cutNewline;
   new->screen.cutToBeginningOfLine = request->screen.cutToBeginningOfLine;
   new->screen.always_highlight = request->screen.always_highlight;
   new->screen.pointer_cursor = request->screen.pointer_cursor;
   new->screen.input_eight_bits = request->screen.input_eight_bits;
   new->screen.output_eight_bits = request->screen.output_eight_bits;
   new->screen.allowSendEvents = request->screen.allowSendEvents;
   new->misc.titeInhibit = request->misc.titeInhibit;
#ifdef COLOR
   for (i = 0; i < 9; i++) {
       new->screen.color[i] = request->screen.color[i];
   }
#endif
#ifdef STATUSLINE
   new->misc.statusline = request->misc.statusline;
   new->misc.statusnormal = request->misc.statusnormal;
#endif /* STATUSLINE */
   for (i = fontMenu_font1; i <= fontMenu_lastBuiltin; i++) {
       new->screen.menu_font_names[i] = request->screen.menu_font_names[i];
   }
   /* set default in realize proc */
   new->screen.menu_font_names[fontMenu_fontdefault] = NULL;
   new->screen.menu_font_names[fontMenu_fontescape] = NULL;
   new->screen.menu_font_names[fontMenu_fontsel] = NULL;
   new->screen.menu_font_number = fontMenu_fontdefault;

    /*
     * The definition of -rv now is that it changes the definition of 
     * XtDefaultForeground and XtDefaultBackground.  So, we no longer
     * need to do anything special.
     */
   new->keyboard.flags = 0;
   new->screen.display = new->core.screen->display;
   new->core.height = new->core.width = 1;
      /* dummy values so that we don't try to Realize the parent shell 
	 with height or width of 0, which is illegal in X.  The real
	 size is computed in the xtermWidget's Realize proc,
	 but the shell's Realize proc is called first, and must see
	 a valid size. */

   /* look for focus related events on the shell, because we need
    * to care about the shell's border being part of our focus.
    */
   XtAddEventHandler(XtParent(new), EnterWindowMask, FALSE,
		HandleEnterWindow, (Opaque)NULL);
   XtAddEventHandler(XtParent(new), LeaveWindowMask, FALSE,
		HandleLeaveWindow, (Opaque)NULL);
   XtAddEventHandler(XtParent(new), FocusChangeMask, FALSE,
		HandleFocusChange, (Opaque)NULL);
   XtAddEventHandler((Widget)new, 0L, TRUE,
		VTNonMaskableEvent, (Opaque)NULL);
   XtAddEventHandler((Widget)new, PropertyChangeMask, FALSE,
		     HandleBellPropertyChange, (Opaque)NULL);
   new->screen.bellInProgress = FALSE;

   set_character_class (new->screen.charClass);

   /* create it, but don't realize it */
   ScrollBarOn (new, TRUE, FALSE);

   /* make sure that the resize gravity acceptable */
   if ( new->misc.resizeGravity != NorthWestGravity &&
        new->misc.resizeGravity != SouthWestGravity) {
       extern XtAppContext app_con;
       Cardinal nparams = 1;

       XtAppWarningMsg(app_con, "rangeError", "resizeGravity", "XTermError",
		       "unsupported resizeGravity resource value (%d)",
		       (String *) &(new->misc.resizeGravity), &nparams);
       new->misc.resizeGravity = SouthWestGravity;
   }

   return;
}


static void VTDestroy (w)
Widget w;
{
    XtFree(((XtermWidget)w)->screen.selection);
}

/*ARGSUSED*/
static void VTRealize (w, valuemask, values)
    Widget w;
    XtValueMask *valuemask;
    XSetWindowAttributes *values;
{
	unsigned int width, height;
	register TScreen *screen = &term->screen;
	int xpos, ypos, pr;
	XSizeHints		sizehints;
	int scrollbar_width;
#ifdef MULTIFONT
	int fset;
#endif /* MULTIFONT */

	TabReset (term->tabs);

	screen->menu_font_names[fontMenu_fontdefault] = term->misc.f_n;
#ifdef MULTIFONT
	for (fset = 0; fset < F_MAX; fset++) {
#endif /* MULTIFONT */
	screen->fnt_norm = screen->fnt_bold = NULL;
#ifdef MULTIFONT
	}
#endif /* MULTIFONT */
	if (!LoadNewFont(screen, term->misc.f_n, term->misc.f_b, False, 0)) {
	    if (XmuCompareISOLatin1(term->misc.f_n, "fixed") != 0) {
		fprintf (stderr, 
		     "%s:  unable to open font \"%s\", trying \"fixed\"....\n",
		     xterm_name, term->misc.f_n);
		(void) LoadNewFont (screen, "fixed", NULL, False, 0);
		screen->menu_font_names[fontMenu_fontdefault] = "fixed";
	    }
	}

	/* really screwed if we couldn't open default font */
	if (!screen->fnt_norm) {
	    fprintf (stderr, "%s:  unable to locate a suitable font\n",
		     xterm_name);
	    Exit (1);
	}

	/* making cursor */
	if (!screen->pointer_cursor) 
	  screen->pointer_cursor = make_colored_cursor(XC_xterm, 
						       screen->mousecolor,
						       screen->mousecolorback);
	else 
	  recolor_cursor (screen->pointer_cursor, 
			  screen->mousecolor, screen->mousecolorback);

	scrollbar_width = (term->misc.scrollbar ?
			   screen->scrollWidget->core.width /* +
			   screen->scrollWidget->core.border_width */ : 0);
#ifdef STATUSLINE
	if (term->misc.statusline) screen->statusheight = -1;
#endif /* STATUSLINE */
#ifdef USE_XIM
	if (term->misc.international) screen->im_statusheight = -1;
#endif /* USE_XIM */

	/* set defaults */
	xpos = 1; ypos = 1; width = 80; height = 24;
	pr = XParseGeometry (term->misc.geo_metry, &xpos, &ypos,
			     &width, &height);
	screen->max_col = (width - 1);	/* units in character cells */
	screen->max_row = (height - 1);	/* units in character cells */
	update_font_info (&term->screen, False);

	width = screen->fullVwin.fullwidth;
	height = screen->fullVwin.fullheight;

	if ((pr & XValue) && (XNegative&pr)) 
	  xpos += DisplayWidth(screen->display, DefaultScreen(screen->display))
			- width - (term->core.parent->core.border_width * 2);
	if ((pr & YValue) && (YNegative&pr))
	  ypos += DisplayHeight(screen->display,DefaultScreen(screen->display))
			- height - (term->core.parent->core.border_width * 2);

	/* set up size hints for window manager; min 1 char by 1 char */
	sizehints.base_width = 2 * screen->border + scrollbar_width;
#ifdef StatusHeight
	sizehints.base_height = 2 * screen->border + StatusHeight(screen);
#else /* StatusHeight */
	sizehints.base_height = 2 * screen->border;
#endif /* StatusHeight */
	sizehints.width_inc = FontWidth(screen);
	sizehints.height_inc = FontHeight(screen);
	sizehints.min_width = sizehints.base_width + sizehints.width_inc;
	sizehints.min_height = sizehints.base_height + sizehints.height_inc;
	sizehints.flags = (PBaseSize|PMinSize|PResizeInc);
	sizehints.x = xpos;
	sizehints.y = ypos;
	if ((XValue&pr) || (YValue&pr)) {
	    sizehints.flags |= USSize|USPosition;
	    sizehints.flags |= PWinGravity;
	    switch (pr & (XNegative | YNegative)) {
	      case 0:
		sizehints.win_gravity = NorthWestGravity;
		break;
	      case XNegative:
		sizehints.win_gravity = NorthEastGravity;
		break;
	      case YNegative:
		sizehints.win_gravity = SouthWestGravity;
		break;
	      default:
		sizehints.win_gravity = SouthEastGravity;
		break;
	    }
	} else {
	    /* set a default size, but do *not* set position */
	    sizehints.flags |= PSize;
	}
	sizehints.width = width;
	sizehints.height = height;
	if ((WidthValue&pr) || (HeightValue&pr)) 
	  sizehints.flags |= USSize;
	else sizehints.flags |= PSize;

	(void) XtMakeResizeRequest((Widget) term,
				   (Dimension)width, (Dimension)height,
				   &term->core.width, &term->core.height);

	/* XXX This is bogus.  We are parsing geometries too late.  This
	 * is information that the shell widget ought to have before we get
	 * realized, so that it can do the right thing.
	 */
        if (sizehints.flags & USPosition)
	    XMoveWindow (XtDisplay(term), term->core.parent->core.window,
			 sizehints.x, sizehints.y);

	XSetWMNormalHints (XtDisplay(term), term->core.parent->core.window,
			   &sizehints);
	XFlush (XtDisplay(term));	/* get it out to window manager */

	/* use ForgetGravity instead of SouthWestGravity because translating
	   the Expose events for ConfigureNotifys is too hard */
	values->bit_gravity = term->misc.resizeGravity == NorthWestGravity ?
	    NorthWestGravity : ForgetGravity;
	term->screen.fullVwin.window = term->core.window =
	  XCreateWindow(XtDisplay(term), XtWindow(term->core.parent),
		term->core.x, term->core.y,
		term->core.width, term->core.height, term->core.border_width,
		(int) term->core.depth,
		InputOutput, CopyFromParent,	
		*valuemask|CWBitGravity, values);

	set_cursor_gcs (screen);

	/* Reset variables used by ANSI emulation. */

#ifdef MULTIFONT
	screen->gsets[0] = term->misc.gsets[0];
	screen->gsets[1] = term->misc.gsets[1];
	screen->gsets[2] = term->misc.gsets[2];
	screen->gsets[3] = term->misc.gsets[3];
	screen->gflags[0] = term->misc.gflags[0];
	screen->gflags[1] = term->misc.gflags[1];
	screen->gflags[2] = term->misc.gflags[2];
	screen->gflags[3] = term->misc.gflags[3];
	screen->gtable = term->misc.gtable;
	screen->curgl = term->misc.gl;	
	screen->curgr = term->misc.gr;
#else /* MULTIFONT */
	screen->gsets[0] = 'B';			/* ASCII_G		*/
	screen->gsets[1] = 'B';
	screen->gsets[2] = 'B';			/* DEC supplemental.	*/
	screen->gsets[3] = 'B';
	screen->curgl = 0;			/* G0 => GL.		*/
	screen->curgr = 2;			/* G2 => GR.		*/
#endif /* MULTIFONT */
	screen->curss = 0;			/* No single shift.	*/

	XDefineCursor(screen->display, VShellWindow, screen->pointer_cursor);

        screen->cur_col = screen->cur_row = 0;
	screen->max_col = Width(screen)/screen->fullVwin.f_width - 1;
	screen->top_marg = 0;
	screen->bot_marg = screen->max_row = Height(screen) /
				screen->fullVwin.f_height - 1;

	screen->sc.row = screen->sc.col = screen->sc.flags = 0;

	/* Mark screen buffer as unallocated.  We wait until the run loop so
	   that the child process does not fork and exec with all the dynamic
	   memory it will never use.  If we were to do it here, the
	   swap space for new process would be huge for huge savelines. */
	if (!tekWidget)			/* if not called after fork */
	  screen->buf = screen->allbuf = NULL;

	screen->do_wrap = 0;
	screen->scrolls = screen->incopy = 0;
	set_vt_box (screen);

	screen->savedlines = 0;
#ifdef STATUSLINE
	screen->reversestatus = term->misc.statusnormal;
	update_reversestatus();
#endif /* STATUSLINE */
#ifdef I18N
	screen->icode = term->misc.icode;
#endif /* I18N */
#ifdef USE_XIM
	if (term->misc.international) {
		screen->im_style = term->misc.im_style;
		ConnectXIM(screen);
	}
#endif /* USE_XIM */

	if (term->misc.scrollbar) {
		screen->scrollbar = 0;
		ScrollBarOn (term, FALSE, TRUE);
	}
	CursorSave (term, &screen->sc);
	return;
}

static Boolean VTSetValues (cur, request, new, args, num_args)
    Widget cur, request, new;
    ArgList args;
    Cardinal *num_args;
{
    XtermWidget curvt = (XtermWidget) cur;
    XtermWidget newvt = (XtermWidget) new; 
    Boolean refresh_needed = FALSE;
    Boolean fonts_redone = FALSE;

    if(curvt->core.background_pixel != newvt->core.background_pixel
       || curvt->screen.foreground != newvt->screen.foreground
       || curvt->screen.menu_font_names[curvt->screen.menu_font_number]
          != newvt->screen.menu_font_names[newvt->screen.menu_font_number]
       || curvt->misc.f_n != newvt->misc.f_n) {
	if(curvt->misc.f_n != newvt->misc.f_n)
	    newvt->screen.menu_font_names[fontMenu_fontdefault] = newvt->misc.f_n;
	if (LoadNewFont(&newvt->screen,
			newvt->screen.menu_font_names[curvt->screen.menu_font_number],
			newvt->screen.menu_font_names[curvt->screen.menu_font_number],
			TRUE, newvt->screen.menu_font_number)) {
	    /* resizing does the redisplay, so don't ask for it here */
	    refresh_needed = TRUE;
	    fonts_redone = TRUE;
	} else
	    if(curvt->misc.f_n != newvt->misc.f_n)
		newvt->screen.menu_font_names[fontMenu_fontdefault] = curvt->misc.f_n;
    }
    if(!fonts_redone
       && curvt->screen.cursorcolor != newvt->screen.cursorcolor) {
	set_cursor_gcs(&newvt->screen);
	refresh_needed = TRUE;
    }
    if(curvt->misc.re_verse != newvt->misc.re_verse) {
	newvt->flags ^= REVERSE_VIDEO;
	ReverseVideo(newvt);
	newvt->misc.re_verse = !newvt->misc.re_verse; /* ReverseVideo toggles */
	refresh_needed = TRUE;
    }
    if(curvt->screen.mousecolor != newvt->screen.mousecolor
       || curvt->screen.mousecolorback != newvt->screen.mousecolorback) {
	recolor_cursor (newvt->screen.pointer_cursor, 
			newvt->screen.mousecolor,
			newvt->screen.mousecolorback);
	refresh_needed = TRUE;
    }
    if (curvt->misc.scrollbar != newvt->misc.scrollbar) {
	if (newvt->misc.scrollbar) {
	    ScrollBarOn (newvt, FALSE, FALSE);
	} else {
	    ScrollBarOff (&newvt->screen);
	}
	update_scrollbar();
    }
#ifdef STATUSLINE
    if (curvt->misc.statusnormal != newvt->misc.statusnormal) {
	newvt->screen.reversestatus = term->misc.statusnormal;
	if (newvt->screen.statusheight)
	    ScrnRefresh(newvt->screen, newvt->screen.max_row + 1, 0, 1, newvt->screen.max_col + 1, False);
	update_reversestatus();
    }
    if (curvt->misc.statusline != newvt->misc.statusline) {
	if (newvt->misc.statusline) {
	    ShowStatus(&newvt->screen);
	} else {
	    HideStatus(&newvt->screen);
	}
	update_statusline();
    }
#endif /* STATUSLINE */

    return refresh_needed;
}

/*
 * Shows cursor at new cursor position in screen.
 */
ShowCursor()
{
	register TScreen *screen = &term->screen;
	register int x, y, flags;
#ifdef MULTIFONT
	Char c[2];
#else /* MULTIFONT */
	Char c;
#endif /* MULTIFONT */
	GC	currentGC;
	Boolean	in_selection;
#ifdef MULTIFONT
	GC	revcurrentGC;
	Char fset;
#endif /* MULTIFONT */
#ifdef COLOR
	XGCValues xgc;
#ifdef MULTIFONT
	XGCValues rxgc;
#endif /* MULTIFONT */
	Char color;
#endif /* COLOR */

	if (eventMode != NORMAL) return;
#ifdef EXTEND
	if (!(term->flags & ENABLECURSOR)) return;
#endif /* EXTEND */
#ifdef STATUSLINE
	if (screen->cur_row - screen->topline > screen->max_row &&
	    !screen->instatus)
#else /* STATUSLINE */
	if (screen->cur_row - screen->topline > screen->max_row)
#endif /* STATUSLINE */
		return;

#ifdef NBUF_TYPE
	x = screen->cursor_col = screen->cur_col;
	y = NBUF_TYPE * (screen->cursor_row = screen->cur_row);
#ifdef MULTIFONT
	c[0] = screen->buf[y + CODE_BUF][x];
#else /* MULTIFONT */
	c = screen->buf[y + CODE_BUF][x];
#endif /* MULTIFONT */
	flags = screen->buf[y + ATTR_BUF][x];
#ifdef MULTIFONT
	fset = screen->buf[y + FNUM_BUF][x];
#endif /* MULTIFONT */
#ifdef COLOR
	color = screen->buf[y + COLOR_BUF][x];
#endif /* COLOR */
#else /* NBUF_TYPE */
	c = screen->buf[y = 2 * (screen->cursor_row = screen->cur_row)]
	 [x = screen->cursor_col = screen->cur_col];
	flags = screen->buf[y + 1][x];
#endif /* NBUF_TYPE */
#ifdef STATUSLINE
	if (screen->instatus && screen->reversestatus) {
		flags ^= INVERSE;
	}
#endif /* STATUSLINE */
#ifdef MULTIFONT
	if (!screen->fnt_norm) load_codeset_font(screen, fset);
	if (c[0] == 0 || !screen->fnt_norm) {
		c[0] = ' ';
		fset = F_ISO8859_1;
	} else if (flags & SECOND) {
		c[1] = c[0];
		c[0] = screen->buf[y + CODE_BUF][x-1];
	} else if (flags & FIRST) {
		c[1] = screen->buf[y + CODE_BUF][x+1];
	}
#else /* MULTIFONT */
	if (c == 0)
		c = ' ';
#endif /* MULTIFONT */

	if (screen->cur_row > screen->endHRow ||
	    (screen->cur_row == screen->endHRow &&
	     screen->cur_col >= screen->endHCol) ||
	    screen->cur_row < screen->startHRow ||
	    (screen->cur_row == screen->startHRow &&
	     screen->cur_col < screen->startHCol))
	    in_selection = False;
	else
	    in_selection = True;

	if(screen->select || screen->always_highlight) {
		if (( (flags & INVERSE) && !in_selection) ||
		    (!(flags & INVERSE) &&  in_selection)){
		    /* text is reverse video */
		    if (screen->cursorGC) {
			currentGC = screen->cursorGC;
#ifdef COLOR
			xgc.background = screen->cursorcolor;
#endif /* COLOR */
		    } else {
			if (flags & BOLD) {
				currentGC = screen->normalboldGC;
			} else {
				currentGC = screen->normalGC;
			}
#ifdef COLOR
			if (color & 0x0f) {
				xgc.background = screen->color[color & 0xf0 >> 4];
			} else {
				xgc.background = term->core.background_pixel;
			}
#endif /* COLOR */
		    }
#ifdef COLOR
		    if (color & 0xf0) {
			    xgc.foreground = screen->color[color & 0x0f];
		    } else {
			    xgc.foreground = screen->foreground;
		    }
#endif /* COLOR */
		} else { /* normal video */
		    if (screen->reversecursorGC) {
			currentGC = screen->reversecursorGC;
#ifdef COLOR
			xgc.background = screen->cursorcolor;
#endif /* COLOR */
		    } else {
			if (flags & BOLD) {
				currentGC = screen->reverseboldGC;
			} else {
				currentGC = screen->reverseGC;
			}
#ifdef COLOR
			if (color & 0xf0) {
				xgc.background = screen->color[color & 0x0f];
			} else {
				xgc.background = screen->foreground;
			}
#endif /* COLOR */
		    }
#ifdef COLOR
		    if (color & 0x0f) {
			    xgc.foreground = screen->color[color & 0xf0 >> 4];
		    } else {
			    xgc.foreground = term->core.background_pixel;
		    }
#endif /* COLOR */
		}
#ifdef COLOR
		XChangeGC(screen->display, currentGC, GCForeground|GCBackground, &xgc);
#endif /* COLOR */
#ifdef MULTIFONT
		revcurrentGC = screen->cursoroutlineGC;
#endif /* MULTIFONT */
	} else { /* not selected */
		if (( (flags & INVERSE) && !in_selection) ||
		    (!(flags & INVERSE) &&  in_selection)) {
		    /* text is reverse video */
			currentGC = screen->reverseGC;
#ifdef MULTIFONT
			revcurrentGC = screen->normalGC;
#endif /* MULTIFONT */
#ifdef COLOR
			if (color & 0x0f) {
			    xgc.background = screen->color[color & 0x0f];
#ifdef MULTIFONT
			    rxgc.foreground = screen->color[color & 0x0f];
#endif /* MULTIFONT */
			} else {
			    xgc.background = screen->foreground;
#ifdef MULTIFONT
			    rxgc.foreground = screen->foreground;
#endif /* MULTIFONT */
			}
			if (color & 0xf0) {
			    xgc.foreground = screen->color[color & 0xf0 >> 4];
#ifdef MULTIFONT
			    rxgc.background = screen->color[color & 0xf0 >> 4];
#endif /* MULTIFONT */
			} else {
			    xgc.foreground = term->core.background_pixel;
#ifdef MULTIFONT
			    rxgc.background = term->core.background_pixel;
#endif /* MULTIFONT */
			}
#endif /* COLOR */
		} else { /* normal video */
			currentGC = screen->normalGC;
#ifdef MULTIFONT
			revcurrentGC = screen->reverseGC;
#endif /* MULTIFONT */
#ifdef COLOR
			if (color & 0x0f) {
			    xgc.foreground = screen->color[color & 0x0f];
#ifdef MULTIFONT
			    rxgc.background = screen->color[color & 0x0f];
#endif /* MULTIFONT */
			} else {
			    xgc.foreground = screen->foreground;
#ifdef MULTIFONT
			    rxgc.background = screen->foreground;
#endif /* MULTIFONT */
			}
			if (color & 0xf0) {
			    xgc.background = screen->color[color & 0xf0 >> 4];
#ifdef MULTIFONT
			    rxgc.foreground = screen->color[color & 0xf0 >> 4];
#endif /* MULTIFONT */
			} else {
			    xgc.background = term->core.background_pixel;
#ifdef MULTIFONT
			    rxgc.foreground = term->core.background_pixel;
#endif /* MULTIFONT */
			}
#endif /* COLOR */
		}
#ifdef COLOR
		XChangeGC(screen->display, currentGC, GCForeground|GCBackground, &xgc);
#ifdef MULTIFONT
		XChangeGC(screen->display, revcurrentGC, GCForeground|GCBackground, &rxgc);
#endif /* MULTIFONT */
#endif /* COLOR */
	    
	}

	x = CursorX (screen, screen->cur_col);
	y = CursorY(screen, screen->cur_row) + 
#ifdef MULTIFONT
	  screen->max_ascent + screen->top_space;
#else /* MULTIFONT */
	  screen->fnt_norm->ascent;
#endif /* MULTIFONT */
#ifdef MULTIFONT
	if (flags & MBSCS) {
		if (screen->top_space)
			XFillRectangle(screen->display, TextWindow(screen),
					revcurrentGC,
					x, y-screen->top_space-screen->max_ascent,
					FontWidth(screen)*2, screen->top_space);
		XDrawImageString16(screen->display, TextWindow(screen), currentGC,
			x, y, (XChar2b *) &c, 1);

		if((flags & BOLD) && screen->enbolden) /* no bold font */
			XDrawString16(screen->display, TextWindow(screen), currentGC,
				x + 1, y, (XChar2b *) &c, 1);
		if (screen->bottom_space)
			XFillRectangle(screen->display, TextWindow(screen),
					revcurrentGC,
					x, y+screen->max_descent,
					FontWidth(screen)*2, screen->bottom_space);
		if(flags & UNDERLINE) 
			XDrawLine(screen->display, TextWindow(screen), currentGC,
				x, y+screen->ul_position, x + FontWidth(screen)*2, y+screen->ul_position);
		if (!screen->select && !screen->always_highlight) {
			screen->box->x = x;
			screen->box->y = y - screen->max_ascent - screen->top_space;
			XDrawLines (screen->display, TextWindow(screen), 
				    screen->cursoroutlineGC ? screen->cursoroutlineGC 
							    : currentGC,
				    screen->box2, NBOX, CoordModePrevious);
		}
	} else {
		if (screen->top_space)
			XFillRectangle(screen->display, TextWindow(screen),
					revcurrentGC,
					x, y-screen->top_space-screen->max_ascent,
					FontWidth(screen), screen->top_space);
		XDrawImageString(screen->display, TextWindow(screen), currentGC,
			x, y, (char *) &c, 1);
		if((flags & BOLD) && screen->enbolden) /* no bold font */
			XDrawString(screen->display, TextWindow(screen), currentGC,
				x + 1, y, (char *) &c, 1);
		if (screen->bottom_space)
			XFillRectangle(screen->display, TextWindow(screen),
					revcurrentGC,
					x, y+screen->max_descent,
					FontWidth(screen), screen->bottom_space);
		if(flags & UNDERLINE) 
			XDrawLine(screen->display, TextWindow(screen), currentGC,
				x, y+screen->ul_position, x + FontWidth(screen), y+1);
		if (!screen->select && !screen->always_highlight) {
			screen->box->x = x;
			screen->box->y = y - screen->max_ascent - screen->top_space;
			XDrawLines (screen->display, TextWindow(screen), 
				    screen->cursoroutlineGC ? screen->cursoroutlineGC 
							    : currentGC,
				    screen->box, NBOX, CoordModePrevious);
		}
	}
#else /* MULTIFONT */
	XDrawImageString(screen->display, TextWindow(screen), currentGC,
		x, y, (char *) &c, 1);

	if((flags & BOLD) && screen->enbolden) /* no bold font */
		XDrawString(screen->display, TextWindow(screen), currentGC,
			x + 1, y, (char *) &c, 1);
	if(flags & UNDERLINE) 
		XDrawLine(screen->display, TextWindow(screen), currentGC,
			x, y+1, x + FontWidth(screen), y+1);
	if (!screen->select && !screen->always_highlight) {
		screen->box->x = x;
		screen->box->y = y - screen->fnt_norm->ascent;
		XDrawLines (screen->display, TextWindow(screen), 
			    screen->cursoroutlineGC ? screen->cursoroutlineGC 
			    			    : currentGC,
			    screen->box, NBOX, CoordModePrevious);
	}
#endif /* MULTIFONT */
	screen->cursor_state = ON;
}

/*
 * hide cursor at previous cursor position in screen.
 */
HideCursor()
{
	register TScreen *screen = &term->screen;
	GC	currentGC;
	register int x, y, flags;
#ifdef MULTIFONT
	char c[2];
#else /* MULTIFONT */
	char c;
#endif /* MULTIFONT */
	Boolean	in_selection;
#ifdef MULTIFONT
	Char fset;
	GC revcurrentGC;
#endif /* MULTIFONT */
#ifdef COLOR
	XGCValues xgc;
#ifdef MULTIFONT
	XGCValues rxgc;
#endif /* MULTIFONT */
	Char color;
#endif /* COLOR */
#ifdef STATUSLINE
	Boolean in_status;
#endif /* STATUSLINE */

#ifdef EXTEND
	if (!(term->flags & ENABLECURSOR)) return;
#endif /* EXTEND */
#ifdef STATUSLINE
	if(!(in_status = (screen->cursor_row > screen->max_row)) &&
	   screen->cursor_row - screen->topline > screen->max_row)
#else /* STATUSLINE */
	if(screen->cursor_row - screen->topline > screen->max_row)
#endif /* STATUSLINE */
		return;
#ifdef NBUF_TYPE
	x = screen->cursor_col;
	y = NBUF_TYPE * screen->cursor_row;
#ifdef MULTIFONT
	c[0] = screen->buf[y + CODE_BUF][x];
#else /* MULTIFONT */
	c = screen->buf[y + CODE_BUF][x];
#endif /* MULTIFONT */
	flags = screen->buf[y + ATTR_BUF][x];
#ifdef MULTIFONT
	fset = screen->buf[y + FNUM_BUF][x];
#endif /* MULTIFONT */
#ifdef COLOR
	color = screen->buf[y + COLOR_BUF][x];
#endif /* COLOR */
#else /* NBUF_TYPE */
	c = screen->buf[y = 2 * screen->cursor_row][x = screen->cursor_col];
	flags = screen->buf[y + 1][x];
#endif /* NBUF_TYPE */
#ifdef STATUSLINE
	if (in_status && screen->reversestatus) {
		flags ^= INVERSE;
	}
#endif /* STATUSLINE */

	if (screen->cursor_row > screen->endHRow ||
	    (screen->cursor_row == screen->endHRow &&
	     screen->cursor_col >= screen->endHCol) ||
	    screen->cursor_row < screen->startHRow ||
	    (screen->cursor_row == screen->startHRow &&
	     screen->cursor_col < screen->startHCol))
	    in_selection = False;
	else
	    in_selection = True;

#ifdef MULTIFONT
	if (!screen->fnt_norm) load_codeset_font(screen, fset);
	if (c[0] == 0 || !screen->fnt_norm) {
		c[0] = ' ';
		fset = F_ISO8859_1;
	} else if (flags & SECOND) {
		c[1] = c[0];
		c[0] = screen->buf[y + CODE_BUF][x-1];
	} else if (flags & FIRST) {
		c[1] = screen->buf[y + CODE_BUF][x+1];
	}
#endif /* MULTIFONT */
	if (( (flags & INVERSE) && !in_selection) ||
	    (!(flags & INVERSE) &&  in_selection)) {
		if(flags & BOLD) {
			currentGC = screen->reverseboldGC;
#ifdef MULTIFONT
			revcurrentGC = screen->normalboldGC;
#endif /* MULTIFONT */
		} else {
			currentGC = screen->reverseGC;
#ifdef MULTIFONT
			revcurrentGC = screen->normalGC;
#endif /* MULTIFONT */
		}
#ifdef COLOR
		if (color & 0x0f) {
			xgc.background = screen->color[color & 0x0f];
#ifdef MULTIFONT
			rxgc.foreground = screen->color[color & 0x0f];
#endif /* MULTIFONT */
		} else {
			xgc.background = screen->foreground;
#ifdef MULTIFONT
			rxgc.foreground = screen->foreground;
#endif /* MULTIFONT */
		}
		if (color & 0xf0) {
			xgc.foreground = screen->color[color & 0xf0 >> 4];
#ifdef MULTIFONT
			rxgc.background = screen->color[color & 0xf0 >> 4];
#endif /* MULTIFONT */
		} else {
			xgc.foreground = term->core.background_pixel;
#ifdef MULTIFONT
			rxgc.background = term->core.background_pixel;
#endif /* MULTIFONT */
		}
#endif /* COLOR */
	} else {
		if(flags & BOLD) {
			currentGC = screen->normalboldGC;
			revcurrentGC = screen->reverseboldGC;
		} else {
			currentGC = screen->normalGC;
			revcurrentGC = screen->reverseGC;
		}
#ifdef COLOR
		if (color & 0x0f) {
			xgc.foreground = screen->color[color & 0x0f];
#ifdef MULTIFONT
			rxgc.background = screen->color[color & 0x0f];
#endif /* MULTIFONT */
		} else {
			xgc.foreground = screen->foreground;
#ifdef MULTIFONT
			rxgc.background = screen->foreground;
#endif /* MULTIFONT */
		}
		if (color & 0xf0) {
			xgc.background = screen->color[(color & 0xf0) >> 4];
#ifdef MULTIFONT
			rxgc.foreground = screen->color[color & 0xf0 >> 4];
#endif /* MULTIFONT */
		} else {
			xgc.background = term->core.background_pixel;
#ifdef MULTIFONT
			rxgc.foreground = term->core.background_pixel;
#endif /* MULTIFONT */
		}
#endif /* COLOR */
	}
#ifdef COLOR
	XChangeGC(screen->display, currentGC, GCForeground|GCBackground, &xgc);
#ifdef MULTIFONT
	XChangeGC(screen->display, revcurrentGC, GCForeground|GCBackground, &rxgc);
#endif /* MULTIFONT */
#endif /* COLOR */

#ifndef MULTIFONT
	if (c == 0)
		c = ' ';
#endif /* MULTIFONT */
	x = CursorX (screen, screen->cursor_col);
#ifdef STATUSLINE
	y = (in_status ?
	    screen->cursor_row * FontHeight(screen):
	    (screen->cursor_row - screen->topline) * FontHeight(screen)) +
#else /* STATUSLINE */
	y = (((screen->cursor_row - screen->topline) * FontHeight(screen))) +
#endif /* STATUSLINE */
	 screen->border;
#ifdef MULTIFONT
	y = y+screen->max_ascent + screen->top_space;
#else /* MULTIFONT */
	y = y+screen->fnt_norm->ascent;
#endif /* MULTIFONT */
#ifdef MULTIFONT
	if (flags & MBSCS) {
		if (screen->top_space)
			XFillRectangle(screen->display, TextWindow(screen),
					revcurrentGC,
					x, y-screen->top_space-screen->max_ascent,
					FontWidth(screen)*2, screen->top_space);
		XDrawImageString16(screen->display, TextWindow(screen), currentGC,
			x, y, (XChar2b *)&c, 1);
		if((flags & BOLD) && screen->enbolden)
			XDrawString16(screen->display, TextWindow(screen), currentGC,
				x + 1, y, (XChar2b *)&c, 1);
		if (screen->bottom_space)
			XFillRectangle(screen->display, TextWindow(screen),
					revcurrentGC,
					x, y+screen->max_descent,
					FontWidth(screen)*2, screen->bottom_space);
		if(flags & UNDERLINE) 
			XDrawLine(screen->display, TextWindow(screen), currentGC,
				x, y+screen->ul_position, x + FontWidth(screen)*2, y+screen->ul_position);
	} else {
		if (screen->top_space)
			XFillRectangle(screen->display, TextWindow(screen),
					revcurrentGC,
					x, y-screen->top_space-screen->max_ascent,
					FontWidth(screen), screen->top_space);
		XDrawImageString(screen->display, TextWindow(screen), currentGC,
			x, y, (char *)&c, 1);
		if((flags & BOLD) && screen->enbolden)
			XDrawString(screen->display, TextWindow(screen), currentGC,
				x + 1, y, (char *)&c, 1);
		if (screen->bottom_space)
			XFillRectangle(screen->display, TextWindow(screen),
					revcurrentGC,
					x, y+screen->max_descent,
					FontWidth(screen), screen->bottom_space);
		if(flags & UNDERLINE) 
			XDrawLine(screen->display, TextWindow(screen), currentGC,
				x, y+screen->ul_position, x + FontWidth(screen), y+screen->ul_position);
	}
#else /* MULTIFONT */
	XDrawImageString(screen->display, TextWindow(screen), currentGC,
		x, y, &c, 1);
	if((flags & BOLD) && screen->enbolden)
		XDrawString(screen->display, TextWindow(screen), currentGC,
			x + 1, y, &c, 1);
	if(flags & UNDERLINE) 
		XDrawLine(screen->display, TextWindow(screen), currentGC,
			x, y+1, x + FontWidth(screen), y+1);
#endif /* MULTIFONT */
	screen->cursor_state = OFF;
}

VTReset(full)
    Boolean full;
{
	register TScreen *screen = &term->screen;

	/* reset scrolling region */
	screen->top_marg = 0;
	screen->bot_marg = screen->max_row;
	term->flags &= ~ORIGIN;
	if(full) {
		TabReset (term->tabs);
		term->keyboard.flags = 0;
		update_appcursor();
		update_appkeypad();
#ifdef MULTIFONT
		screen->gsets[0] = term->misc.gsets[0];
		screen->gsets[1] = term->misc.gsets[1];
		screen->gsets[2] = term->misc.gsets[2];
		screen->gsets[3] = term->misc.gsets[3];
		screen->gflags[0] = term->misc.gflags[0];
		screen->gflags[1] = term->misc.gflags[1];
		screen->gflags[2] = term->misc.gflags[2];
		screen->gflags[3] = term->misc.gflags[3];
		screen->gtable = term->misc.gtable;
		screen->curgl = term->misc.gl;	
		screen->curgl = term->misc.gr;	
#else /* MULTIFONT */
		screen->gsets[0] = 'B';
		screen->gsets[1] = 'B';
		screen->gsets[2] = 'B';
		screen->gsets[3] = 'B';
		screen->curgl = 0;
		screen->curgr = 2;
#endif /* MULTIFONT */
		screen->curss = 0;
#ifdef I18N
		screen->icode = term->misc.icode;
#endif /* I18N */
		FromAlternate(screen);
		ClearScreen(screen);
		screen->cursor_state = OFF;
		if (term->flags & REVERSE_VIDEO)
			ReverseVideo(term);

		term->flags = term->initflags;
		update_reversevideo();
		update_autowrap();
		update_reversewrap();
		update_autolinefeed();
		screen->jumpscroll = !(term->flags & SMOOTHSCROLL);
		update_jumpscroll();
		if(screen->c132 && (term->flags & IN132COLUMNS)) {
		        Dimension junk;
			XtMakeResizeRequest(
			    (Widget) term,
			    (Dimension) 80*FontWidth(screen)
				+ 2 * screen->border + screen->scrollbar,
			    (Dimension) FontHeight(screen)
#ifdef StatusHeight
			        * (screen->max_row + 1) +
				StatusHeight(screen) +
				2 * screen->border,
#else /* StatusHeight */
			        * (screen->max_row + 1) + 2 * screen->border,
#endif /* StatusHeight */
			    &junk, &junk);
			XSync(screen->display, FALSE);	/* synchronize */
			if(QLength(screen->display) > 0)
				xevents();
		}
		CursorSet(screen, 0, 0, term->flags);
	}
	longjmp(vtjmpbuf, 1);	/* force ground state in parser */
}

#ifdef STATUSLINE
ToStatus(screen, col)
register TScreen *screen;
int col;
{
	if(col > screen->max_col)
		col = screen->max_col;
	if(!screen->instatus) {
		if(!screen->statusheight)
			ShowStatus(screen);
		CursorSave(term, &screen->statussc);
		screen->instatus = TRUE;
		screen->cur_row = screen->max_row + 1;
	}
	screen->cur_col = col;
}

FromStatus(screen)
register TScreen *screen;
{
	if(!screen->instatus)
		return;
	screen->instatus = FALSE;
	CursorRestore(term, &screen->statussc);
}

ShowStatus(screen)
register TScreen *screen;
{
	register int border = 2 * screen->border;

	if(screen->statusheight)
		return;
	screen->statusheight = FontHeight(screen) + 2;
	DoResizeScreen(term);
	if (screen->scrollWidget)
	    ResizeScrollBar(screen->scrollWidget, -1, -1, FullHeight(screen) + StatusHeight(screen));
}

HideStatus(screen)
register TScreen *screen;
{
	register int border = 2 * screen->border;
	register int i, j;

	if(!screen->statusheight)
	    return;
	if(screen->instatus)
	    FromStatus(screen);
	screen->statusheight = 0;
#ifdef NBUF_TYPE
	bzero(screen->buf[(i = NBUF_TYPE * (screen->max_row + 1)) + CODE_BUF], j = screen->max_col + 1);
	bzero(screen->buf[i + ATTR_BUF], j);
#ifdef MULTIFONT
	bzero(screen->buf[i + FNUM_BUF], j);
#endif /* MULTIFONT */
#ifdef COLOR
	bzero(screen->buf[i + COLOR_BUF], j);
#endif /* COLOR */
#else
	bzero(screen->buf[i = 2 * (screen->max_row + 1)], j = screen->max_col + 1);
	bzero(screen->buf[i + 1], j);
#endif
	DoResizeScreen(term);
	if (screen->scrollWidget)
	    ResizeScrollBar(screen->scrollWidget, -1, -1, FullHeight(screen));
}

EraseStatus(screen)
register TScreen *screen;
{
	register int i, j;
#ifdef MULTIFONT
	int fset = F_ISO8859_1;
#endif /* MULTIFONT */

	if(!screen->statusheight)
		return;
#ifdef NBUF_TYPE
	bzero(screen->buf[(i = NBUF_TYPE * (screen->max_row + 1)) + CODE_BUF], j = screen->max_col + 1);
	bzero(screen->buf[i + ATTR_BUF], j);
#ifdef MULTIFONT
	bzero(screen->buf[i + FNUM_BUF], j);
#endif /* MULTIFONT */
#ifdef COLOR
	bzero(screen->buf[i + COLOR_BUF], j);
#endif /* COLOR */
#else /* NBUF_TYPE */
	bzero(screen->buf[i = 2 * (screen->max_row + 1)], j = screen->max_col +
	 1);
	bzero(screen->buf[i + 1], j);
#endif /* NBUF_TYPE */
	XFillRectangle(screen->display, TextWindow(screen),
		       screen->reversestatus ? screen->normalGC:
					       screen->reverseGC,
		       screen->border - 1 + screen->scrollbar,
		       Height(screen) + screen->border * 2,
		       Width(screen) + screen->border * 2,
		       screen->statusheight);
	if(!screen->reversestatus)
		StatusBox(screen);
}

StatusBox(screen)
register TScreen *screen;
{
#ifdef MULTIFONT
	int fset = F_ISO8859_1;
#endif /* MULTIFONT */
	XDrawRectangle(screen->display, TextWindow(screen),
 		       screen->normalGC,
		       screen->scrollbar,
		       Height(screen) + screen->border * 2,
		       Width(screen) + screen->border * 2 - 1,
		       screen->statusheight - 1);
}
#endif /* STATUSLINE */

/*
 * set_character_class - takes a string of the form
 * 
 *                 low[-high]:val[,low[-high]:val[...]]
 * 
 * and sets the indicated ranges to the indicated values.
 */

int set_character_class (s)
    register char *s;
{
    register int i;			/* iterator, index into s */
    int len;				/* length of s */
    int acc;				/* accumulator */
    int low, high;			/* bounds of range [0..127] */
    int base;				/* 8, 10, 16 (octal, decimal, hex) */
    int numbers;			/* count of numbers per range */
    int digits;				/* count of digits in a number */
    static char *errfmt = "%s:  %s in range string \"%s\" (position %d)\n";
    extern char *ProgramName;

    if (!s || !s[0]) return -1;

    base = 10;				/* in case we ever add octal, hex */
    low = high = -1;			/* out of range */

    for (i = 0, len = strlen (s), acc = 0, numbers = digits = 0;
	 i < len; i++) {
	char c = s[i];

	if (isspace(c)) {
	    continue;
	} else if (isdigit(c)) {
	    acc = acc * base + (c - '0');
	    digits++;
	    continue;
	} else if (c == '-') {
	    low = acc;
	    acc = 0;
	    if (digits == 0) {
		fprintf (stderr, errfmt, ProgramName, "missing number", s, i);
		return (-1);
	    }
	    digits = 0;
	    numbers++;
	    continue;
	} else if (c == ':') {
	    if (numbers == 0)
	      low = acc;
	    else if (numbers == 1)
	      high = acc;
	    else {
		fprintf (stderr, errfmt, ProgramName, "too many numbers",
			 s, i);
		return (-1);
	    }
	    digits = 0;
	    numbers++;
	    acc = 0;
	    continue;
	} else if (c == ',') {
	    /*
	     * now, process it
	     */

	    if (high < 0) {
		high = low;
		numbers++;
	    }
	    if (numbers != 2) {
		fprintf (stderr, errfmt, ProgramName, "bad value number", 
			 s, i);
	    } else if (SetCharacterClassRange (low, high, acc) != 0) {
		fprintf (stderr, errfmt, ProgramName, "bad range", s, i);
	    }

	    low = high = -1;
	    acc = 0;
	    digits = 0;
	    numbers = 0;
	    continue;
	} else {
	    fprintf (stderr, errfmt, ProgramName, "bad character", s, i);
	    return (-1);
	}				/* end if else if ... else */

    }

    if (low < 0 && high < 0) return (0);

    /*
     * now, process it
     */

    if (high < 0) high = low;
    if (numbers < 1 || numbers > 2) {
	fprintf (stderr, errfmt, ProgramName, "bad value number", s, i);
    } else if (SetCharacterClassRange (low, high, acc) != 0) {
	fprintf (stderr, errfmt, ProgramName, "bad range", s, i);
    }

    return (0);
}

/* ARGSUSED */
static void HandleKeymapChange(w, event, params, param_count)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *param_count;
{
    static XtTranslations keymap, original;
    static XtResource key_resources[] = {
	{ XtNtranslations, XtCTranslations, XtRTranslationTable,
	      sizeof(XtTranslations), 0, XtRTranslationTable, (XtPointer)NULL}
    };
    char mapName[1000];
    char mapClass[1000];

    if (*param_count != 1) return;

    if (original == NULL) original = w->core.tm.translations;

    if (strcmp(params[0], "None") == 0) {
	XtOverrideTranslations(w, original);
	return;
    }
    (void) sprintf( mapName, "%sKeymap", params[0] );
    (void) strcpy( mapClass, mapName );
    if (islower(mapClass[0])) mapClass[0] = toupper(mapClass[0]);
    XtGetSubresources( w, (XtPointer)&keymap, mapName, mapClass,
		       key_resources, (Cardinal)1, NULL, (Cardinal)0 );
    if (keymap != NULL)
	XtOverrideTranslations(w, keymap);
}


/* ARGSUSED */
static void HandleBell(w, event, params, param_count)
    Widget w;
    XEvent *event;		/* unused */
    String *params;		/* [0] = volume */
    Cardinal *param_count;	/* 0 or 1 */
{
    int percent = (*param_count) ? atoi(params[0]) : 0;

    XBell( XtDisplay(w), percent );
}


/* ARGSUSED */
static void HandleVisualBell(w, event, params, param_count)
    Widget w;
    XEvent *event;		/* unused */
    String *params;		/* unused */
    Cardinal *param_count;	/* unused */
{
    VisualBell();
}


/* ARGSUSED */
static void HandleIgnore(w, event, params, param_count)
    Widget w;
    XEvent *event;		/* unused */
    String *params;		/* unused */
    Cardinal *param_count;	/* unused */
{
    /* do nothing, but check for funny escape sequences */
    (void) SendMousePosition(w, event);
}

static void
DoSetSelectedFont(w, client_data, selection, type, value, length, format)
    Widget w;
    XtPointer client_data;
    Atom *selection, *type;
    XtPointer value;
    unsigned long *length;
    int *format;
{
    char *val = (char *)value;
    int len;
    if (*type != XA_STRING  ||  *format != 8) {
	Bell();
	return;
    }
    len = strlen(val);
    if (len > 0) {
	if (val[len-1] == '\n') val[len-1] = '\0';
	/* Do some sanity checking to avoid sending a long selection
	   back to the server in an OpenFont that is unlikely to succeed.
	   XLFD allows up to 255 characters and no control characters;
	   we are a little more liberal here. */
	if (len > 1000  ||  strchr(val, '\n'))
	    return;
	if (!LoadNewFont (&term->screen, val, NULL, True, fontMenu_fontsel))
	    Bell();
    }
}

void FindFontSelection (atom_name, justprobe)
    char *atom_name;
    Bool justprobe;
{
    static AtomPtr *atoms;
    static int atomCount = 0;
    AtomPtr *pAtom;
    int a;
    Atom target;

    if (!atom_name) atom_name = "PRIMARY";

    for (pAtom = atoms, a = atomCount; a; a--, pAtom++) {
	if (strcmp(atom_name, XmuNameOfAtom(*pAtom)) == 0) break;
    }
    if (!a) {
	atoms = (AtomPtr*) XtRealloc ((char *)atoms,
				      sizeof(AtomPtr)*(atomCount+1));
	*(pAtom = &atoms[atomCount++]) = XmuMakeAtom(atom_name);
    }

    target = XmuInternAtom(XtDisplay(term), *pAtom);
    if (justprobe) {
	term->screen.menu_font_names[fontMenu_fontsel] = 
	  XGetSelectionOwner(XtDisplay(term), target) ? _Font_Selected_ : NULL;
    } else {
	XtGetSelectionValue((Widget)term, target, XA_STRING,
			    DoSetSelectedFont, NULL,
			    XtLastTimestampProcessed(XtDisplay(term)));
    }
    return;
}


/* ARGSUSED */
void HandleSetFont(w, event, params, param_count)
    Widget w;
    XEvent *event;		/* unused */
    String *params;		/* unused */
    Cardinal *param_count;	/* unused */
{
    int fontnum;
    char *name1 = NULL, *name2 = NULL;

    if (*param_count == 0) {
	fontnum = fontMenu_fontdefault;
    } else {
	int maxparams = 1;		/* total number of params allowed */

	switch (params[0][0]) {
	  case 'd': case 'D': case '0':
	    fontnum = fontMenu_fontdefault; break;
	  case '1':
	    fontnum = fontMenu_font1; break;
	  case '2':
	    fontnum = fontMenu_font2; break;
	  case '3':
	    fontnum = fontMenu_font3; break;
	  case '4':
	    fontnum = fontMenu_font4; break;
	  case '5':
	    fontnum = fontMenu_font5; break;
	  case '6':
	    fontnum = fontMenu_font6; break;
	  case 'e': case 'E':
	    fontnum = fontMenu_fontescape; maxparams = 3; break;
	  case 's': case 'S':
	    fontnum = fontMenu_fontsel; maxparams = 2; break;
	  default:
	    Bell();
	    return;
	}
	if (*param_count > maxparams) {	 /* see if extra args given */
	    Bell();
	    return;
	}
	switch (*param_count) {		/* assign 'em */
	  case 3:
	    name2 = params[2];
	    /* fall through */
	  case 2:
	    name1 = params[1];
	    break;
	}
    }

    SetVTFont (fontnum, True, name1, name2);
}


void SetVTFont (i, doresize, name1, name2)
    int i;
    Bool doresize;
    char *name1, *name2;
{
    TScreen *screen = &term->screen;

    if (i < 0 || i >= NMENUFONTS) {
	Bell();
	return;
    }
    if (i == fontMenu_fontsel) {	/* go get the selection */
	FindFontSelection (name1, False);  /* name1 = atom, name2 is ignored */
	return;
    }
    if (!name1) name1 = screen->menu_font_names[i];
    if (!LoadNewFont(screen, name1, name2, doresize, i)) {
	Bell();
    }
    return;
}


int LoadNewFont (screen, nfontname, bfontname, doresize, fontnum)
    TScreen *screen;
    char *nfontname, *bfontname;
    Bool doresize;
    int fontnum;
{
    XFontStruct *nfs = NULL, *bfs = NULL;
    XGCValues xgcv;
    unsigned long mask;
    GC new_normalGC = NULL, new_normalboldGC = NULL;
    GC new_reverseGC = NULL, new_reverseboldGC = NULL;
    char *tmpname = NULL;
#ifdef MULTIFONT
    int fset = F_ISO8859_1;
#endif /* MULTIFONT */
#ifdef USE_XIM
    XVaNestedList list;
#endif /* USE_XIM */

    if (!nfontname) return 0;

    if (fontnum == fontMenu_fontescape &&
	nfontname != screen->menu_font_names[fontnum]) {
	tmpname = (char *) malloc (strlen(nfontname) + 1);
	if (!tmpname) return 0;
	strcpy (tmpname, nfontname);
    }

    if (!(nfs = XLoadQueryFont (screen->display, nfontname))) goto bad;

    if (!(bfontname && 
	  (bfs = XLoadQueryFont (screen->display, bfontname))))
      bfs = nfs;

    mask = (GCFont | GCForeground | GCBackground | GCGraphicsExposures |
	    GCFunction);

    xgcv.font = nfs->fid;
    xgcv.foreground = screen->foreground;
    xgcv.background = term->core.background_pixel;
    xgcv.graphics_exposures = TRUE;	/* default */
    xgcv.function = GXcopy;

    new_normalGC = XtGetGC((Widget)term, mask, &xgcv);
    if (!new_normalGC) goto bad;

    if (nfs == bfs) {			/* there is no bold font */
	new_normalboldGC = new_normalGC;
    } else {
	xgcv.font = bfs->fid;
	new_normalboldGC = XtGetGC((Widget)term, mask, &xgcv);
	if (!new_normalboldGC) goto bad;
    }

    xgcv.font = nfs->fid;
    xgcv.foreground = term->core.background_pixel;
    xgcv.background = screen->foreground;
    new_reverseGC = XtGetGC((Widget)term, mask, &xgcv);
    if (!new_reverseGC) goto bad;

    if (nfs == bfs) {			/* there is no bold font */
	new_reverseboldGC = new_reverseGC;
    } else {
	xgcv.font = bfs->fid;
	new_reverseboldGC = XtGetGC((Widget)term, mask, &xgcv);
	if (!new_reverseboldGC) goto bad;
    }

    if (screen->normalGC != screen->normalboldGC)
	XtReleaseGC ((Widget) term, screen->normalboldGC);
    XtReleaseGC ((Widget) term, screen->normalGC);
    if (screen->reverseGC != screen->reverseboldGC)
	XtReleaseGC ((Widget) term, screen->reverseboldGC);
    XtReleaseGC ((Widget) term, screen->reverseGC);
    screen->normalGC = new_normalGC;
    screen->normalboldGC = new_normalboldGC;
    screen->reverseGC = new_reverseGC;
    screen->reverseboldGC = new_reverseboldGC;
    screen->fnt_norm = nfs;
    screen->fnt_bold = bfs;
    screen->enbolden = (nfs == bfs);
#ifdef MULTIFONT
    for (fset = F_ISO8859_1 + 1; fset < F_MAX; fset++) {
	if (screen->normalGC != screen->normalboldGC)
	    XtReleaseGC ((Widget) term, screen->normalboldGC);
	XtReleaseGC ((Widget) term, screen->normalGC);
	if (screen->reverseGC != screen->reverseboldGC)
	    XtReleaseGC ((Widget) term, screen->reverseboldGC);
	XtReleaseGC ((Widget) term, screen->reverseGC);
	screen->normalGC = NULL;
	screen->normalboldGC = NULL;
	screen->reverseGC = NULL;
	screen->reverseboldGC = NULL;
	screen->fnt_norm = NULL;
	screen->fnt_bold = NULL;
    }
#endif /* MULTIFONT */
    set_menu_font (False);
    screen->menu_font_number = fontnum;
    set_menu_font (True);
    if (tmpname) {			/* if setting escape or sel */
	if (screen->menu_font_names[fontnum])
	  free (screen->menu_font_names[fontnum]);
	screen->menu_font_names[fontnum] = tmpname;
	if (fontnum == fontMenu_fontescape) {
	    set_sensitivity (term->screen.fontMenu,
			     fontMenuEntries[fontMenu_fontescape].widget,
			     TRUE);
	}
    }
    set_cursor_gcs (screen);
    update_font_info (screen, doresize);
#ifdef USE_XIM
    if (screen->im_style & XIMStatusArea) {
	if (screen->im_fontset) {
	    list = XVaCreateNestedList(NULL,
					XNFontSet, screen->im_fontset,
					NULL);
	    XSetICValues(screen->ic, XNStatusAttributes, list, NULL);
	    XFree(list);
	}
    }
    if (screen->im_style & XIMPreeditPosition ||
	screen->im_style & XIMPreeditArea) {
	if (screen->im_fontset) {
	    list = XVaCreateNestedList(NULL,
					XNFontSet, screen->im_fontset,
					NULL);
	    XSetICValues(screen->ic, XNPreeditAttributes, list, NULL);
	    XFree(list);
	}
    }
#endif /* USE_XIM */
    return 1;

  bad:
    if (tmpname) free (tmpname);
    if (new_normalGC)
      XtReleaseGC ((Widget) term, screen->normalGC);
    if (new_normalGC && new_normalGC != new_normalboldGC)
      XtReleaseGC ((Widget) term, new_normalboldGC);
    if (new_reverseGC)
      XtReleaseGC ((Widget) term, new_reverseGC);
    if (new_reverseGC && new_reverseGC != new_reverseboldGC)
      XtReleaseGC ((Widget) term, new_reverseboldGC);
    if (nfs) XFreeFont (screen->display, nfs);
    if (nfs && nfs != bfs) XFreeFont (screen->display, bfs);
    return 0;
}


update_font_info (screen, doresize)
    TScreen *screen;
    Bool doresize;
{
    int i, j, width, height, scrollbar_width;
#ifdef MULTIFONT
    int fset = F_ISO8859_1;
    int max_width = 0;
    int max_ascent = 0;
    int max_descent = 0;
    int f_height;
#endif /* MULTIFONT */

#ifdef MULTIFONT
    for (fset = F_ISO8859_1; fset < F_MAX; fset++) {
	if (screen->fnt_norm) {
	    if (screen->fnt_norm->max_byte1 > 0) {
		if (max_width < screen->fnt_norm->max_bounds.width / 2) {
		    max_width = screen->fnt_norm->max_bounds.width / 2;
		}
	    } else {
		if (max_width < screen->fnt_norm->max_bounds.width) {
		    max_width = screen->fnt_norm->max_bounds.width;
		}
	    }
	    if (max_ascent < screen->fnt_norm->ascent) {
		max_ascent = screen->fnt_norm->ascent;
	    }
	    if (max_descent < screen->fnt_norm->descent) {
		max_descent = screen->fnt_norm->descent;
	    }
	}
    }
    screen->fullVwin.f_width = max_width;
    f_height = max_ascent + max_descent;
    screen->top_space = f_height * term->misc.top_space / 100;
    if (term->misc.bottom_space) {
	screen->bottom_space = f_height *
				term->misc.bottom_space / 100;
    }
    screen->fullVwin.f_height = f_height + screen->top_space + screen->bottom_space;
    screen->max_ascent = max_ascent;
    screen->max_descent = max_descent;
    screen->ul_position = screen->bottom_space ? screen->max_descent: 1;
#else /* MULTIFONT */
    screen->fullVwin.f_width = screen->fnt_norm->max_bounds.width;
    screen->fullVwin.f_height = (screen->fnt_norm->ascent +
				 screen->fnt_norm->descent);
#endif /* MULTIFONT */
    scrollbar_width = (term->misc.scrollbar ? 
		       screen->scrollWidget->core.width +
		       screen->scrollWidget->core.border_width : 0);
    i = 2 * screen->border + scrollbar_width;
    j = 2 * screen->border;
#ifdef STATUSLINE
    if (screen->statusheight) {
	j += (screen->statusheight = FontHeight(screen) + 2);
    }
#endif /* STATUSLINE */
#ifdef USE_XIM
    if (screen->im_statusheight) {
	j += (screen->im_statusheight = FontHeight(screen) + 2);
    }
    create_imfontset(screen);
#endif /* USE_XIM */
    width = (screen->max_col + 1) * screen->fullVwin.f_width + i;
    height = (screen->max_row + 1) * screen->fullVwin.f_height + j;
    screen->fullVwin.fullwidth = width;
    screen->fullVwin.fullheight = height;
    screen->fullVwin.width = width - i;
    screen->fullVwin.height = height - j;

    if (doresize) {
	if (VWindow(screen)) {
	    XClearWindow (screen->display, VWindow(screen));
	}
	DoResizeScreen (term);		/* set to the new natural size */
	if (screen->scrollWidget)
	  ResizeScrollBar (screen->scrollWidget, -1, -1,
			   Height(screen) + screen->border * 2);
	Redraw ();
    }
    set_vt_box (screen);
}

set_vt_box (screen)
	TScreen *screen;
{
	XPoint	*vp;

	vp = &VTbox[1];
	(vp++)->x = FontWidth(screen) - 1;
	(vp++)->y = FontHeight(screen) - 1;
	(vp++)->x = -(FontWidth(screen) - 1);
	vp->y = -(FontHeight(screen) - 1);
	screen->box = VTbox;
#ifdef MULTIFONT
	vp = &VTbox2[1];
	(vp++)->x = FontWidth(screen) * 2 - 1;
	(vp++)->y = FontHeight(screen) - 1;
	(vp++)->x = -(FontWidth(screen) * 2 - 1);
	vp->y = -(FontHeight(screen) - 1);
	screen->box = VTbox;
#endif /* MULTIFONT */
}


set_cursor_gcs (screen)
    TScreen *screen;
{
    XGCValues xgcv;
    unsigned long mask;
    unsigned long cc = screen->cursorcolor;
    unsigned long fg = screen->foreground;
    unsigned long bg = term->core.background_pixel;
    GC new_cursorGC = NULL, new_reversecursorGC = NULL;
    GC new_cursoroutlineGC = NULL;
#ifdef MULTIFONT
    int fset = F_ISO8859_1;
#endif /* MULTIFONT */

    /*
     * Let's see, there are three things that have "color":
     *
     *     background
     *     text
     *     cursorblock
     *
     * And, there are four situation when drawing a cursor, if we decide
     * that we like have a solid block of cursor color with the letter
     * that it is highlighting shown in the background color to make it
     * stand out:
     *
     *     selected window, normal video - background on cursor
     *     selected window, reverse video - foreground on cursor
     *     unselected window, normal video - foreground on background
     *     unselected window, reverse video - background on foreground
     *
     * Since the last two are really just normalGC and reverseGC, we only
     * need two new GC's.  Under monochrome, we get the same effect as
     * above by setting cursor color to foreground.
     */

    xgcv.font = screen->fnt_norm->fid;
    mask = (GCForeground | GCBackground | GCFont);
    if (cc != fg && cc != bg) {
	/* we have a colored cursor */
	xgcv.foreground = fg;
	xgcv.background = cc;
	new_cursorGC = XtGetGC ((Widget) term, mask, &xgcv);

	if (screen->always_highlight) {
	    new_reversecursorGC = (GC) 0;
	    new_cursoroutlineGC = (GC) 0;
	} else {
	    xgcv.foreground = bg;
	    xgcv.background = cc;
	    new_reversecursorGC = XtGetGC ((Widget) term, mask, &xgcv);
	    xgcv.foreground = cc;
	    xgcv.background = bg;
	    new_cursoroutlineGC = XtGetGC ((Widget) term, mask, &xgcv);
		}
    } else {
	new_cursorGC = (GC) 0;
	new_reversecursorGC = (GC) 0;
	new_cursoroutlineGC = (GC) 0;
    }
    if (screen->cursorGC) XtReleaseGC ((Widget)term, screen->cursorGC);
    if (screen->reversecursorGC)
	XtReleaseGC ((Widget)term, screen->reversecursorGC);
    if (screen->cursoroutlineGC)
	XtReleaseGC ((Widget)term, screen->cursoroutlineGC);
    screen->cursorGC = new_cursorGC;
    screen->reversecursorGC = new_reversecursorGC;
    screen->cursoroutlineGC = new_cursoroutlineGC;
}

#ifdef MULTIFONT
static char *
xlfd_codeset[] = {
    "iso8859-1",
    "iso8859-2",
    "iso8859-3",
    "iso8859-4",
    "iso8859-5",
    "iso8859-6",
    "iso8859-7",
    "iso8859-8",
    "iso8859-9",
    "jisx0201.1976-0",
    "jisx0208.1983-0",
    "gb2312.1980-0",
    "ksc5601.1987-0",
};

static char *
get_font_name (dpy, fs)
    Display *dpy;
    XFontStruct *fs;
{
    register XFontProp *fp;
    register int i;
    Atom fontatom = XInternAtom (dpy, "FONT", False);

    for (i = 0, fp = fs->properties; i < fs->n_properties; i++, fp++) {
	if (fp->name == fontatom) {
	    return (XGetAtomName (dpy, fp->card32));
	}
    }
    return NULL;
}

static char *
get_base_name(screen, fs)
    TScreen *screen;
    XFontStruct *fs;
{
    static char basename[256];
    register char *name;
    register char *tmptail;
    register int i;

    if (name = get_font_name(screen->display, fs)) {
	strcpy(basename, "-*");
	name++;
	if (name = index(name, '-')) {
	    XmuCopyISOLatin1Lowered(basename + 2, name);
	}
	tmptail = basename;
	for (i = 0; i < 12; i++) {
	    if (!(tmptail = index(tmptail, '-'))) {
		break;
	    }
	    tmptail++;
	}
	if (tmptail) {
	    *tmptail = '\0';
	    return strcat(basename, "*-");
	}
    }
    return NULL;
}

static char *
get_codeset_name (basename, code, bold)
    char *basename;
    int code;
    Bool bold;
{
    static char name[256];
    register char *p;

    if (bold) {
	if (basename) {
	    p = index(basename + 1, '-');
	    p = index(p + 1, '-');
	    strncpy(name, basename, p - basename + 1);
	    strcpy(name + (p - basename + 1), "bold");
	    p = index(p + 1, '-');
	    strcat(name, p);
	    return strcat(name, xlfd_codeset[code]);
	} else {
	    return NULL;
	}
    } else {
	if (basename) {
	    strcpy(name, basename);
	    return strcat(name, xlfd_codeset[code]);
	} else {
	    return NULL;
	}
    }
}

#ifdef USE_XIM
static char *
get_wildcard_name(basename)
    char *basename;
{
    static char name[256];

    if (basename) {
	strcpy(name, basename);
	return (strcat(name, "*-*"));
    } else {
	return NULL;
    }
}

create_imfontset(screen)
    register TScreen *screen;
{
    char *nfn;
    char *def_string;
    char **mis_list;
    int mis_count;
    int fset = F_ISO8859_1;

    if (screen->im_fontset) {
	XFreeFontSet(screen->display, screen->im_fontset);
	screen->im_fontset = NULL;
    }
    nfn = get_wildcard_name(get_base_name(screen, screen->fnt_norm));
    if (nfn) {
	screen->im_fontset = XCreateFontSet(screen->display, nfn, &mis_list,
					    &mis_count, &def_string);
    }
}
#endif /* USE_XIM */

load_codeset_font (screen, fnum)
    TScreen *screen;
    int fnum;
{
    register XFontStruct *nfs = NULL, *bfs = NULL;
    XGCValues xgcv;
    unsigned long mask;
    GC new_normalGC = NULL, new_normalboldGC = NULL;
    GC new_reverseGC = NULL, new_reverseboldGC = NULL;
    char *nfn = NULL, *bfn = NULL;
    int fset = F_ISO8859_1;

    if (nfn = get_codeset_name(get_base_name(screen, screen->fnt_norm),
			       fnum, False)) {
	nfs = XLoadQueryFont (screen->display, nfn);
    }
    if (bfn = get_codeset_name(get_base_name(screen, screen->fnt_bold),
			       fnum, True)) {
	bfs = XLoadQueryFont (screen->display, bfn);
    }

    mask = (GCFont | GCForeground | GCBackground | GCGraphicsExposures |
	    GCFunction);

    if (!nfs) {
	if (fnum == F_JISX0201_0) {
	    nfs = screen->fnt_norm;
	    bfs = screen->fnt_bold;
	} else {
	    goto bad;
	}
    }
    xgcv.font = nfs->fid;
    xgcv.foreground = screen->foreground;
    xgcv.background = term->core.background_pixel;
    xgcv.graphics_exposures = TRUE;	/* default */
    xgcv.function = GXcopy;

    new_normalGC = XtGetGC((Widget)term, mask, &xgcv);
    if (!new_normalGC) goto bad;

    if (bfs) {					/* there is no bold font */
	xgcv.font = bfs->fid;
	new_normalboldGC = XtGetGC((Widget)term, mask, &xgcv);
	if (!new_normalboldGC) goto bad;
    } else {
	new_normalboldGC = new_normalGC;
    }

    xgcv.font = nfs->fid;
    xgcv.foreground = term->core.background_pixel;
    xgcv.background = screen->foreground;
    new_reverseGC = XtGetGC((Widget)term, mask, &xgcv);
    if (!new_reverseGC) goto bad;

    if (bfs) {					/* there is no bold font */
	xgcv.font = bfs->fid;
	new_reverseboldGC = XtGetGC((Widget)term, mask, &xgcv);
	if (!new_reverseboldGC) goto bad;
    } else {
	bfs = nfs;
	new_reverseboldGC = new_reverseGC;
    }

    fset = fnum;
    if (screen->normalGC != screen->normalboldGC)
      XtReleaseGC ((Widget) term, screen->normalboldGC);
    XtReleaseGC ((Widget) term, screen->normalGC);
    if (screen->reverseGC != screen->reverseboldGC)
      XtReleaseGC ((Widget) term, screen->reverseboldGC);
    XtReleaseGC ((Widget) term, screen->reverseGC);
    screen->normalGC = new_normalGC;
    screen->normalboldGC = new_normalboldGC;
    screen->reverseGC = new_reverseGC;
    screen->reverseboldGC = new_reverseboldGC;
    screen->fnt_norm = nfs;
    screen->fnt_bold = bfs;
    screen->enbolden = (nfs == bfs);

    mask = (GCForeground | GCBackground | GCFont);
    if (screen->cursorcolor != screen->foreground &&
	screen->cursorcolor != term->core.background_pixel) {
	/* we have a colored cursor */
	xgcv.foreground = screen->foreground;
	xgcv.background = screen->cursorcolor;
	new_normalGC = XtGetGC ((Widget) term, mask, &xgcv);
	xgcv.foreground = term->core.background_pixel;
	xgcv.background = screen->cursorcolor;
	new_reverseGC = XtGetGC ((Widget) term, mask, &xgcv);
    } else {
	new_normalGC = (GC) 0;
	new_reverseGC = (GC) 0;
    }
    if (screen->cursorGC)
	XtReleaseGC ((Widget)term, screen->cursorGC);
    if (screen->reversecursorGC)
	XtReleaseGC ((Widget)term, screen->reversecursorGC);
    screen->cursorGC = new_normalGC;
    screen->reversecursorGC = new_reverseGC;

    update_font_info (screen, False);
    return;

bad:
    if (new_normalGC)
      XtReleaseGC ((Widget) term, screen->normalGC);
    if (new_normalGC && new_normalGC != new_normalboldGC)
      XtReleaseGC ((Widget) term, new_normalboldGC);
    if (new_reverseGC)
      XtReleaseGC ((Widget) term, new_reverseGC);
    if (new_reverseGC && new_reverseGC != new_reverseboldGC)
      XtReleaseGC ((Widget) term, new_reverseboldGC);
    if (nfs) XFreeFont (screen->display, nfs);
    if (nfs && nfs != bfs) XFreeFont (screen->display, bfs);
    return;
}
#endif /* MULTIFONT */
