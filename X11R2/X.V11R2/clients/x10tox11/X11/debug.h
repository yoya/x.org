/*
 * Copyright 1987 by Tektronix Inc, Beaverton, Oregon.
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Tektronix not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.
 *
 *
 * TEKTRONIX DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * TEKTRONIX BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */
/* 
 * $Header: debug.h,v 4.0 88/01/27 16:51:54 toddb Exp $ 
 * $Locker:  $ 
 */
#ifdef DEBUG

extern int	debugbits[];

#define	debugged(req)	\
		(debugbits[ (req)>>5 ] & (1<<((req)&0x1f)))
#define	debug(req, printfargs)	\
		if (debugged(req)) { bugout printfargs; }
#define	SetDebugbit(req)	\
		(debugbits[ (req)>>5 ] |= (1<<((req)&0x1f)))
#define	ClearDebugbit(req)	\
		(debugbits[ (req)>>5 ] &= (~(1<<((req)&0x1f))))
#define	debugevent(printfargs)	\
		debug(X10EventDebug, printfargs)

#define	X11_EventBase		120
#define	X11_Reply		(Reply+X11_EventBase)
#define	X11_Error		(Error+X11_EventBase)
#define	X11_KeyPress		(KeyPress+X11_EventBase)
#define	X11_KeyRelease		(KeyRelease+X11_EventBase)
#define	X11_ButtonPress		(ButtonPress+X11_EventBase)
#define	X11_ButtonRelease	(ButtonRelease+X11_EventBase)
#define	X11_MotionNotify	(MotionNotify+X11_EventBase)
#define	X11_EnterNotify		(EnterNotify+X11_EventBase)
#define	X11_LeaveNotify		(LeaveNotify+X11_EventBase)
#define	X11_FocusIn		(FocusIn+X11_EventBase)
#define	X11_FocusOut		(FocusOut+X11_EventBase)
#define	X11_KeymapNotify	(KeymapNotify+X11_EventBase)
#define	X11_Expose		(Expose+X11_EventBase)
#define	X11_GraphicsExpose	(GraphicsExpose+X11_EventBase)
#define	X11_NoExpose		(NoExpose+X11_EventBase)
#define	X11_VisibilityNotify	(VisibilityNotify+X11_EventBase)
#define	X11_CreateNotify	(CreateNotify+X11_EventBase)
#define	X11_DestroyNotify	(DestroyNotify+X11_EventBase)
#define	X11_UnmapNotify		(UnmapNotify+X11_EventBase)
#define	X11_MapNotify		(MapNotify+X11_EventBase)
#define	X11_MapRequest		(MapRequest+X11_EventBase)
#define	X11_ReparentNotify	(ReparentNotify+X11_EventBase)
#define	X11_ConfigureNotify	(ConfigureNotify+X11_EventBase)
#define	X11_ConfigureRequest	(ConfigureRequest+X11_EventBase)
#define	X11_GravityNotify	(GravityNotify+X11_EventBase)
#define	X11_ResizeRequest	(ResizeRequest+X11_EventBase)
#define	X11_CirculateNotify	(CirculateNotify+X11_EventBase)
#define	X11_CirculateRequest	(CirculateRequest+X11_EventBase)
#define	X11_PropertyNotify	(PropertyNotify+X11_EventBase)
#define	X11_SelectionClear	(SelectionClear+X11_EventBase)
#define	X11_SelectionRequest	(SelectionRequest+X11_EventBase)
#define	X11_SelectionNotify	(SelectionNotify+X11_EventBase)
#define	X11_ColormapNotify	(ColormapNotify+X11_EventBase)
#define	X11_ClientMessage	(ClientMessage+X11_EventBase)

#define	XPseudoBase		(X11_ClientMessage+1)
#define	XEventMaskDebug		(XPseudoBase+0)
#define	XButtonMaskDebug	(XPseudoBase+1)
#define	XBitmapDebug		(XPseudoBase+2)
#define	XUnimplementedDebug	(XPseudoBase+3)
#define	XWinTreeDebug		(XPseudoBase+4)
#define	XWinTreeMappedOnlyDebug	(XPseudoBase+5)
#define	XPromptForEachRequest	(XPseudoBase+6)
#define	XPromptForEachEvent	(XPseudoBase+7)
#define	XLineCoordinatesDebug	(XPseudoBase+8)
#define	XSequenceDebug		(XPseudoBase+9)

#define	XLastDebug		(XSequenceDebug+1)

char *X10EventName();
#else /* DEBUG */

#define	debugged(req)
#define	debug(req, printfargs)
#define	SetDebugbit(req)
#define	debugevent(printfargs)
#define ShowReq(req, rep)
#define ShowReqName(client, req)

#endif /* DEBUG */
