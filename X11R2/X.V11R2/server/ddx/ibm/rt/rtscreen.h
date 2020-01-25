/***********************************************************
		Copyright IBM Corporation 1987

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of IBM not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
#ifndef RT_SCREEN
/* $Header:rtscreen.h 6.0$ */
/* $ACIS:rtscreen.h 6.0$ */
/* $Source: /vice/X11/src/server/ddx/ibm/rt/RCS/rtscreen.h,v $ */

	/*
	 * RT specific, per-screen information
	 */

typedef struct RTPERSCRINFO {
	BoxRec		  rt_ScreenBounds;

	int		   rt_NumFormats;
	PixmapFormatRec	  *rt_ScreenFormats;

	Bool		(*rt_InitFunc)();
	Bool		(*rt_ProbeFunc)();
	void		(*rt_HideCursor)();

	char		 *rt_ScreenFlag;
	char		 *rt_ScreenDevice;
	char		 *rt_ScreenPointer;

	ScreenPtr	  rt_Screen;

	int		  rt_ScreenFD;

	int		  rt_Wanted;

	int	 	  rt_SoftCursor;
	short		 *rt_CursorX;
	short		 *rt_CursorY;
	short		  rt_CursorHotX;
	short		  rt_CursorHotY;
	BoxRec		  rt_CursorBounds;
	int		(*rt_CursorShow)();
	CursorPtr	  rt_CurrentCursor;
} rtPerScreenInfo;
	
#define	rtScreenBounds(n)	(&rtScreens[(n)]->rt_ScreenBounds)
#define	rtScreenMinX(n)		(rtScreenBounds(n)->x1)
#define	rtScreenMinY(n)		(rtScreenBounds(n)->y1)
#define	rtScreenMaxX(n)		(rtScreenBounds(n)->x2)
#define	rtScreenMaxY(n)		(rtScreenBounds(n)->y2)
#define	rtNumFormats(n)		(rtScreens[(n)]->rt_NumFormats)
#define	rtScreenFormats(n)	(rtScreens[(n)]->rt_ScreenFormats)
#define	rtScreenInit(n)		(rtScreens[(n)]->rt_InitFunc)
#define	rtHideCursor(n)		(rtScreens[(n)]->rt_HideCursor)
#define	rtScreenFlag(n)		(rtScreens[(n)]->rt_ScreenFlag)
#define	rtScreenDevice(n)	(rtScreens[(n)]->rt_ScreenDevice)
#define	rtScreenPointer(n)	(rtScreens[(n)]->rt_ScreenPointer)
#define	rtScreenFD(n)		(rtScreens[(n)]->rt_ScreenFD)
#define	rtSoftCursor(n)		(rtScreens[(n)]->rt_SoftCursor)
#define	rtCursorX(n)		(rtScreens[(n)]->rt_CursorX)
#define	rtCursorY(n)		(rtScreens[(n)]->rt_CursorY)
#define	rtCursorHotX(n)		(rtScreens[(n)]->rt_CursorHotX)
#define	rtCursorHotY(n)		(rtScreens[(n)]->rt_CursorHotY)
#define	rtCursorBounds(n)	(&rtScreens[(n)]->rt_CursorBounds)
#define	rtCursorShow(n)		(rtScreens[(n)]->rt_CursorShow)
#define	rtCurrentCursor(n)	(rtScreens[(n)]->rt_CurrentCursor)
#define	rtScreen(n)		(rtScreens[(n)]->rt_Screen)

extern	int		 rtSaveScreen();
extern	void		 rtInitEmulator();
extern	rtPerScreenInfo	 rtPossibleScreens[];
extern	rtPerScreenInfo	*rtScreens[MAXSCREENS];
extern	int		 rtNumScreens;
extern	int		 rtXWrapScreen;
extern	int		 rtYWrapScreen;
extern	int		 rtCurrentScreen;
extern	int		 rtUseHardware;
extern	int		 rtEmulatorFD;

#endif /* RT_SCREEN */
