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
 * $Header: resource.h,v 4.1 88/02/14 14:22:51 toddb Exp $ 
 * $Locker:  $ 
 */

#include "debug.h"
#include "X10struct.h"

/*
 * This holds all the information about windows that we need to keep
 * around.  
 */
typedef struct _WinInfo *WinInfoptr;
typedef struct _WinInfo {
	Window		id;		/* window name */
	Pixmap		tile;
	short		mask;
	short		grabmask;
	short		width;		/* inside width */
	short		height;		/* inside height */
	short		borderWidth;
	short		x, y;		/* upper left outside coordinates */
	u_char		transparent;	/* true if transparent window */
	u_char		hasTransparency;/* true if transparent child */
					/* exists */
	u_char		drawThrough;	/* true if ClipModeDrawThrough */
	u_char		mapped;		/* true if window is mapped */
	u_char		foriegn;	/* true if window is real x11 client */
	u_char		tileModeRelative;/* true if tileMode is Relative */
	Pixmap		savedPixmap;	/* non-zero if this is PixmapSave'd */
	unsigned short	configured;	/* >0 if we need an expose event */
					/* when a ConfigureNotify arrives */
	WinInfoptr	sibling;	/* first in list of siblings */
					/* listed in stacking order */
	WinInfoptr	child;		/* first in list of children */
	WinInfoptr	parent;		/* parent window */
	Window		gcWin;		/* GC window (never free this!) */
	GC		gc;
} WinInfo;

/*
 * Information about pixmaps and bitmaps for X10 are kept in one of these.
 * X11 does not distinguish between bitmaps and pixmaps, because bitmaps are
 * pixmaps of depth one.  The bitmapid field may be superfluous because
 * all we use it for is to keep track of a bitmap used to create a
 * pixmap.  Hence, for referece, if bitmapid != 0 ==> this is a pixmap
 * created from a bitmap.  There is no way to infer whether an id
 * refers to a bitmap or a pixmap, except perhaps by the depth of 1.
 */
typedef struct _PixmapInfo *PixmapInfoptr;
typedef struct _PixmapInfo {
	Pixmap		id;		/* pixmap name */
	XID		bitmapid;	/* bitmap name */
	u_long		width;
	u_long		height;
	u_long		depth;
	GC		gc;
	PixmapInfoptr	next;
	u_char		savedPixmap;	/* true if this is PixmapSave pixmap */
} PixmapInfo;

typedef struct _FontInfo *FontInfoptr;
typedef struct _FontInfo {
	Font		id;		/* font name */
	XFontStruct	*info;
	FontInfoptr	next;
} FontInfo;

/*
 * Standard macro for looking up a drawable.  Note that we invalidate
 * the savedPixmap, if there was one.
 */
#define GetWinInfoOrReturnBadDrawable(client, wid, wp)			\
	WinInfoptr wp = WinLookup(client, wid, True);			\
	if (wp == NULL) {						\
		Xstatus = X10BadWindow;					\
		return(False);						\
	}								\
	if (wp->savedPixmap) {						\
		PixmapInfoptr pp = PixmapLookup(client, wp->savedPixmap); \
		if (pp != NULL)						\
			pp->savedPixmap = False;			\
		wp->savedPixmap = 0;					\
	}

/*
 * Standard macro for looking up a window.
 */
#define GetWinInfoOrReturnBadWindow(client, wid, wp)			\
	WinInfoptr wp = WinLookup(client, wid, False);			\
	if (wp == NULL) {						\
		Xstatus = X10BadWindow;					\
		return(False);						\
	}

/*
 * Macro to determine if we need to set the clipping rectangle or not.
 */
#define	CheckTransparentDrawableAndXY(dpy, wp, dptr, xptr, yptr)	\
if (wp->transparent)							\
	ChangeXYAndClipAndDrawable(dpy, wp, dptr, xptr, yptr);

/*
 * This handles the sequence recorder.
 */
typedef struct _sequence *Sequenceptr;
typedef struct _sequence {
	X10Req	req;
	int	x11seq;
} Sequence;

typedef struct _sequencelist {
	Sequenceptr	circList;
	int		last;
	int		head;
	int		tail;
	int		lastx11seq;
} SequenceList;

#define SEQ_INCREMENT	2	/* start small to test the algorythm */
/*
 * return the next pointer in the circular list.
 */
#define NextSeqPtr(cp, ptr) \
	(((ptr) >= (cp)->seq.last) ? 0 : (ptr)+1)


/*
 * Given the X10 graphics function, return the appropriate functgion for X11.
 */
#define	X10toX11function(func)				\
	((unsigned)(func) > 15 || NoTransform		\
		? func : GXFunctionInvert[ func ])

PixmapInfoptr	CreatePixmap();
PixmapInfoptr	PixmapLookup();
PixmapInfoptr	BitmapLookup();
PixmapInfoptr	StorePixmapOrBitmap();
FontInfoptr	FontLookup();
WinInfoptr	WinLookup();
WinInfoptr	AddWin();
WinInfoptr	FindWinTree();

typedef struct _Client {
	Display		*dpy;
	PixmapInfoptr	pixmaps;
	FontInfoptr	fontinfo;
	WinInfoptr	win;
	Window		grabwin;
	SequenceList	seq;
	XEvent		buffer;	/* buffer of 1 */
} Client, *Clientptr;

typedef struct _FontMap {
	char	*fontname;
	int	fontindex;
} FontMap, *FontMapptr;

/*
 * indices for known x11 fonts.
 */
#define	FONT_FIXED		0
#define	FONT_VARIABLE		1
#define	FONT_MAX		1

#ifndef NULL
#define	NULL	0
#endif

#ifndef	BUFSIZ
#define	BUFSIZ	1024
#endif

/*
 * event status types
 */
#define	EventOK			1
#define	EventNotSelected	2
#define	EventWindowMissing	3
#define	EventSubWindowMissing	4
#define	EventSelectedByChild	5
#define	EventLockToggleMode	6
#define	EventUnImplemented	7
#define	EventHasNoX10Equiv	8
#define	EventModeField		9
#define	EventNoChange		10
#define	EventSuppressed		11
#define	EventParentNotMapped	12
#define	EventAlreadyMapped	13
#define	EventUnknown		14

char	**X11FontNames();
char	*CopyStr();
char	*ToLower();
char	*Xalloc();
char	*Xrealloc();
char	*StaticAlloc();
char	*X10ProcPixmapGet();
unsigned int X10toX11EventMask();
unsigned int X11toX10EventMask();
unsigned int X11toX10KeyButState();
unsigned int X10ButtonMaskToX11KeyMask();
unsigned int X10ButtonMaskToX11Button();
void	SetClipMode();
void	SetTileMode();
void	UpdateWinTree();
XPoint	*X10toX11Points();

extern Clientptr clist[];
extern u_char GXFunctionInvert[];
extern int	FileDescriptors;
extern Bool	NoTransform;
