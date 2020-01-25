/* $SonyId: XimpP.h,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#ifndef	_XimpP_H
#define	_XimpP_H
#include	"Ximp.h"
#include	<X11/Xlib.h>

/* Ximp Protocol Version */
#define XIMP_PROTOCOL_VERSION3	"XIMP.3.5"
#define XIMP_PROTOCOL_VERSION4	"XIMP.4.0"

/* Input Context ID */
typedef unsigned long	ICID;

/* ClientMessage No. */

/* client <=> IM Server  */
#define  XIMP_KEYPRESS3		 	  1
#define  XIMP_KEYRELEASE4               100
#define  XIMP_KEYPRESS4			101

/* client => IM Server  */
/* Base Protocol       */
#define  XIMP_CREATE3			  2
#define  XIMP_DESTROY3			  3
#define  XIMP_BEGIN3			  4
#define  XIMP_END3			  5
#define  XIMP_SETFOCUS3			  6
#define  XIMP_UNSETFOCUS3		  7
#define  XIMP_CHANGE3			  8
#define  XIMP_MOVE3			  9
#define  XIMP_RESET3			 10
#define  XIMP_SETVALUE3			 11
#define  XIMP_GETVALUE3			 12
#define  XIMP_CREATE4			102
#define  XIMP_DESTROY4			103
#define  XIMP_REG_KEY_PRESSED4		104
#define  XIMP_SETFOCUS4			105
#define  XIMP_UNSETFOCUS4		106
#define  XIMP_CLIENT_WINDOW4		107
#define  XIMP_FOCUS_WINDOW4		108
#define  XIMP_MOVE4			109
#define  XIMP_RESET4			110
#define  XIMP_SETVALUE4			111
#define  XIMP_GETVALUE4			112

/* Callback  Protocol  */
#define  XIMP_PREEDITSTART_RETURN3	 20
#define  XIMP_PREEDITCARET_RETURN3	 21
#define  XIMP_PREEDITSTART_RETURN4	120
#define  XIMP_PREEDITCARET_RETURN4	121

/* IM Server => client  */
/* Base Protocol       */
#define  XIMP_CREATE_RETURN3		  2
#define  XIMP_PROCESS_BEGIN3		 30
#define  XIMP_PROCESS_END3		 31
#define  XIMP_READPROP3			 32
#define  XIMP_GETVALUE_RETURN3		 33
#define  XIMP_RESET_RETURN3		 34
#define  XIMP_SPROC_STARTED4		130
#define  XIMP_SPROC_STOPPED4		131
#define  XIMP_READPROP4			132
#define  XIMP_CLIENT_WINDOW_RETURN4	133
#define  XIMP_FOCUS_WINDOW_RETURN4	134
#define  XIMP_GETVALUE_RETURN4		135
#define  XIMP_RESET_RETURN4		136
#define  XIMP_CREATE_RETURN4		137
#define  XIMP_KEYPRESS_RETURN4		138
#define  XIMP_KEYRELEASE_RETURN4        139

/* client => IM Server for frontend method */
#define  XIMP_EVENTMASK_NOTIFY4		200
/* client <== frontend for frontend method */
#define  XIMP_EVENTMASK_NOTIFY_RETURN4	201

/* Extension Protocol */
#define  XIMP_EXTENSION3		 90
#define  XIMP_EXTENSION4		500

/* IM Server = ERROR => client */
#define  XIMP_ERROR3			 99
#define  XIMP_ERROR4			999

/* Error Notify from IM Server */
/*  Detail Error Number */
#define  XIMP_NoError			0	/* No Error */
#define  XIMP_BadAlloc			1	/* Memeory Alloc Fail */
#define  XIMP_BadStyle         		2	/* Unspported Input Style */
#define  XIMP_BadClientWindow         	3	/* Invalid Client Window */
#define  XIMP_BadFocusWindow		4	/* Invalid Focus Window */
#define  XIMP_BadArea			5	/* Invalid Area */
#define  XIMP_BadSpotLocation		6	/* SpotLocation Out Of Range */
#define  XIMP_BadColormap		7	/* Invalid Colormap ID */
#define  XIMP_BadAtom			8	/* Invalid Atom ID */
#define  XIMP_BadPixel			9	/* Invalid Pixel Value */
#define  XIMP_BadPixmap			10	/* Invalid Pixmap Value */
#define  XIMP_BadName			11	/* Invalid Font Name */
#define  XIMP_BadCursor			12	/* Invalid Cursor ID */
#define  XIMP_BadProtocol		13	/* Invalid Protocol ID */
#define  XIMP_BadProperty		14	/* Invalid Property Name */
#define  XIMP_BadPropertyType		15	/* Invalid Property Type */

/* Property Name */
#define  _XIMP_PROTOCOL		"_XIMP_PROTOCOL"
#define  _XIMP_BASE		"_XIMP_"

/* IMS Window Property Name */
#define  _XIMP_VERSION			"_XIMP_VERSION"
#define  _XIMP_STYLE			"_XIMP_STYLE"
#define  _XIMP_TYPE			"_XIMP_TYPE"
#define  _XIMP_KEYS			"_XIMP_KEYS"
#define  _XIMP_SPROC_STARTED_KEYS	"_XIMP_SPROC_STARTED_KEYS"
#define  _XIMP_SPROC_STOPPED_KEYS	"_XIMP_SPROC_STOPPED_KEYS"
#define  _XIMP_SERVERNAME		"_XIMP_SERVERNAME"
#define  _XIMP_SERVERVERSION		"_XIMP_SERVERVERSION"
#define  _XIMP_EXTENSIONS		"_XIMP_EXTENSIONS"
#define  _XIMP_PREEDITMAXSIZE		"_XIMP_PREEDITMAXSIZE"
#define  _XIMP_VENDORNAME		"_XIMP_VENDORNAME"

/* Client Window Property Name */
#define  _XIMP_FOCUS			"_XIMP_FOCUS"
#define  _XIMP_PREEDIT			"_XIMP_PREEDIT"
#define  _XIMP_STATUS			"_XIMP_STATUS"
#define  _XIMP_PREEDITFONT		"_XIMP_PREEDITFONT"
#define  _XIMP_STATUSFONT		"_XIMP_STATUSFONT"

#define  _XIMP_COMMIT			"_XIMP_COMMIT"
#define  _XIMP_CTEXT			"_XIMP_CTEXT"
#define  _XIMP_RESET			"_XIMP_RESET"

/* Ximp properties for extented XIC attribute */
#define	_XIMP_EXT_XIMP_CONVERSION		"_XIMP_EXT_XIMP_CONVERSION"
#define	_XIMP_EXT_XIMP_BACK_FRONT		"_XIMP_EXT_XIMP_BACK_FRONT"

/* mask (XIMP_CREATE, XIMP_SETVALUE, XIMP_GETVALUE) */
#define XIMP_FOCUS_WIN_MASK4		(1L <<  0)
#define XIMP_PRE_AREA_MASK4		(1L <<  1)
#define XIMP_PRE_AREANEED_MASK4		(1L <<  2)
#define XIMP_PRE_COLORMAP_MASK4		(1L <<  3)
#define XIMP_PRE_STD_COLORMAP_MASK4	(1L <<  4)
#define XIMP_PRE_FG_MASK4		(1L <<  5)
#define XIMP_PRE_BG_MASK4		(1L <<  6)
#define XIMP_PRE_BGPIXMAP_MASK4		(1L <<  7)
#define XIMP_PRE_LINESP_MASK4		(1L <<  8)
#define XIMP_PRE_CURSOR_MASK4		(1L <<  9)
#define XIMP_PRE_SPOTL_MASK4		(1L << 10)
#define XIMP_STS_AREA_MASK4		(1L << 11)
#define XIMP_STS_AREANEED_MASK4		(1L << 12)
#define XIMP_STS_COLORMAP_MASK4		(1L << 13)
#define XIMP_STS_STD_COLORMAP_MASK4	(1L << 14)
#define XIMP_STS_FG_MASK4		(1L << 15)
#define XIMP_STS_BG_MASK4		(1L << 16)
#define XIMP_STS_BGPIXMAP_MASK4		(1L << 17)
#define XIMP_STS_LINESP_MASK4		(1L << 18)
#define XIMP_STS_CURSOR_MASK4		(1L << 19)
#define XIMP_STS_WINDOW_MASK4		(1L << 20)
#define XIMP_PRE_FONT_MASK4		(1L << 21)
#define XIMP_STS_FONT_MASK4		(1L << 22)
#define XIMP_SERVERTYPE_MASK4		(1L << 23)

/* mask (For Backward Compatibility) */
#define XIMP_FOCUS_WIN_MASK3          (1L <<  0)
#define XIMP_PRE_AREA_MASK3           (1L <<  1)
#define XIMP_PRE_FG_MASK3             (1L <<  2)
#define XIMP_PRE_BG_MASK3             (1L <<  3)
#define XIMP_PRE_COLORMAP_MASK3       (1L <<  4)
#define XIMP_PRE_BGPIXMAP_MASK3       (1L <<  5)
#define XIMP_PRE_LINESP_MASK3         (1L <<  6)
#define XIMP_PRE_CURSOR_MASK3         (1L <<  7)
#define XIMP_PRE_AREANEED_MASK3       (1L <<  8)
#define XIMP_PRE_SPOTL_MASK3          (1L <<  9)
#define XIMP_STS_AREA_MASK3           (1L << 10)
#define XIMP_STS_FG_MASK3             (1L << 11)
#define XIMP_STS_BG_MASK3             (1L << 12)
#define XIMP_STS_COLORMAP_MASK3       (1L << 13)
#define XIMP_STS_BGPIXMAP_MASK3       (1L << 14)
#define XIMP_STS_LINESP_MASK3         (1L << 15)
#define XIMP_STS_CURSOR_MASK3         (1L << 16)
#define XIMP_STS_AREANEED_MASK3       (1L << 17)
#define XIMP_STS_WINDOW_MASK3         (1L << 18)
#define XIMP_PRE_FONT_MASK3           (1L << 19)
#define XIMP_STS_FONT_MASK3           (1L << 20)

/* MODE(FRONTEND or BACKEND), TYPE(Type1,2,3) and SYNC/ASYNC */
#define XIMP_FRONTEND		(1L << 0)
#define XIMP_BACKEND		(1L << 1)
#define XIMP_TYPE1		(1L << 2)
#define XIMP_TYPE2		(1L << 3)
#define XIMP_TYPE3		(1L << 4)
#define XIMP_SYNC		(1L << 5)
#define XIMP_FE_TYPE1		(XIMP_FRONTEND | XIMP_TYPE1)
#define XIMP_FE_TYPE2		(XIMP_FRONTEND | XIMP_TYPE2)
#define XIMP_FE_TYPE3		(XIMP_FRONTEND | XIMP_TYPE3)
#define XIMP_BE_TYPE1		(XIMP_BACKEND | XIMP_TYPE1)
#define XIMP_BE_TYPE2		(XIMP_BACKEND | XIMP_TYPE2)
#define XIMP_SYNC_BE_TYPE1	(XIMP_SYNC | XIMP_BE_TYPE1)
#define XIMP_SYNC_BE_TYPE2	(XIMP_SYNC | XIMP_BE_TYPE2)

/* Conversion MODE */
#define	XIMEnabel	1
#define	XIMDisable	0

/* FRONTEND or BACKEND MODE */
#define XIMP_FRONTEND_MODE	 0
#define XIMP_BACKEND_MODE	 1

#define NO_RESET_DATA		0
#define	RESET_DATA_VIA_CM	1
#define	RESET_DATA_VIA_PROP	2

typedef struct _XimpArea {
    long		x;
    long		y;
    long		width;
    long		height;
} XimpAreaRec;

typedef struct _XimpPoint {
    long		x;
    long		y;
} XimpPointRec;

typedef struct _XimpSize {
    long		width;
    long		height;
} XimpSizeRec;

typedef struct  _XimpPreedit {
    XimpAreaRec		area;
    unsigned long	foreground;
    unsigned long	background;
    Colormap		colormap;
    Atom		std_colormap;
    Pixmap		bg_pixmap;
    long		line_spacing;
    Cursor		cursor;
    XimpSizeRec		area_needed;
    XimpPointRec	spot_location;
} XimpPreeditPropRec;

typedef struct  _XimpStatus {
    XimpAreaRec		area;
    unsigned long  	foreground;
    unsigned long	background;
    Colormap		colormap;
    Atom		std_colormap;
    Pixmap		bg_pixmap;
    long		line_spacing;
    Cursor		cursor;
    XimpSizeRec		area_needed;
    Window		window;
} XimpStatusPropRec;

/* for Commit, PreEditDraw, StatusDraw */
typedef struct _XimpCommitPropRec {
    unsigned long	icid;
    char		size;
    char		ctext[11];
} XimpCommitPropRec;


#define	ISXimp3(q)	((q)->protocol == 35)
#define ISXimp4(q)	((q)->protocol == 40)

/* client <=> IM Server */
#define XIMP_KEYPRESS(q)	\
	((q->protocol == 35)?XIMP_KEYPRESS3:XIMP_KEYPRESS4)

/* client => IM Server */
#define XIMP_CREATE(p)		((p==35)?XIMP_CREATE3:XIMP_CREATE4)

/* IM Server => client */
#define XIMP_CREATE_RETURN(q)		\
	((q->protocol == 35)?XIMP_CREATE_RETURN3:XIMP_CREATE_RETURN4)
#define XIMP_PROCESS_BEGIN(q)		\
	((q->protocol == 35)?XIMP_PROCESS_BEGIN3:XIMP_SPROC_STARTED4)
#define XIMP_PROCESS_END(q)		\
	((q->protocol == 35)?XIMP_PROCESS_END3:XIMP_SPROC_STOPPED4)
#define XIMP_GETVALUE_RETURN(q)		\
	((q->protocol == 35)?XIMP_GETVALUE_RETURN3:XIMP_GETVALUE_RETURN4)
#define XIMP_RESET_RETURN(q)		\
	((q->protocol == 35)?XIMP_RESET_RETURN3:XIMP_RESET_RETURN4)
#define XIMP_READPROP(q)		\
	((q->protocol == 35)?XIMP_READPROP3:XIMP_READPROP4)

#define XIMP_EXTENSION(q)		\
	((q->protocol == 35)?XIMP_EXTENSION3:XIMP_EXTENSION4)
#define XIMP_ERROR(p)		((p == 35)?XIMP_ERROR3:XIMP_ERROR4)


#define XIMP_FOCUS_WIN_MASK(q)		\
	((q->protocol == 35)?XIMP_FOCUS_WIN_MASK3:XIMP_FOCUS_WIN_MASK4)
#define XIMP_PRE_AREA_MASK(q)		\
	((q->protocol == 35)?XIMP_PRE_AREA_MASK3:XIMP_PRE_AREA_MASK4)
#define XIMP_PRE_FG_MASK(q)		\
	((q->protocol == 35)?XIMP_PRE_FG_MASK3:XIMP_PRE_FG_MASK4)
#define XIMP_PRE_BG_MASK(q)		\
	((q->protocol == 35)?XIMP_PRE_BG_MASK3:XIMP_PRE_BG_MASK4)
#define XIMP_PRE_COLORMAP_MASK(q)	\
	((q->protocol == 35)?XIMP_PRE_COLORMAP_MASK3:XIMP_PRE_COLORMAP_MASK4)
#define XIMP_PRE_STD_COLORMAP_MASK(q)	\
	((q->protocol == 35)?0:XIMP_PRE_STD_COLORMAP_MASK4)
#define XIMP_PRE_BGPIXMAP_MASK(q)	\
	((q->protocol == 35)?XIMP_PRE_BGPIXMAP_MASK3:XIMP_PRE_BGPIXMAP_MASK4)
#define XIMP_PRE_LINESP_MASK(q)		\
	((q->protocol == 35)?XIMP_PRE_LINESP_MASK3:XIMP_PRE_LINESP_MASK4)
#define XIMP_PRE_CURSOR_MASK(q)		\
	((q->protocol == 35)?XIMP_PRE_CURSOR_MASK3:XIMP_PRE_CURSOR_MASK4)
#define XIMP_PRE_AREANEED_MASK(q)	\
	((q->protocol == 35)?XIMP_PRE_AREANEED_MASK3:XIMP_PRE_AREANEED_MASK4)
#define XIMP_PRE_SPOTL_MASK(q)		\
	((q->protocol == 35)?XIMP_PRE_SPOTL_MASK3:XIMP_PRE_SPOTL_MASK4)
#define XIMP_STS_AREA_MASK(q)		\
	((q->protocol == 35)?XIMP_STS_AREA_MASK3:XIMP_STS_AREA_MASK4)
#define XIMP_STS_FG_MASK(q)		\
	((q->protocol == 35)?XIMP_STS_FG_MASK3:XIMP_STS_FG_MASK4)
#define XIMP_STS_BG_MASK(q)		\
	((q->protocol == 35)?XIMP_STS_BG_MASK3:XIMP_STS_BG_MASK4)
#define XIMP_STS_COLORMAP_MASK(q)	\
	((q->protocol == 35)?XIMP_STS_COLORMAP_MASK3:XIMP_STS_COLORMAP_MASK4)
#define XIMP_STS_STD_COLORMAP_MASK(q)	\
	((q->protocol == 35)?0:XIMP_STS_STD_COLORMAP_MASK4)
#define XIMP_STS_BGPIXMAP_MASK(q)	\
	((q->protocol == 35)?XIMP_STS_BGPIXMAP_MASK3:XIMP_STS_BGPIXMAP_MASK4)
#define XIMP_STS_LINESP_MASK(q)		\
	((q->protocol == 35)?XIMP_STS_LINESP_MASK3:XIMP_STS_LINESP_MASK4)
#define XIMP_STS_CURSOR_MASK(q)		\
	((q->protocol == 35)?XIMP_STS_CURSOR_MASK3:XIMP_STS_CURSOR_MASK4)
#define XIMP_STS_AREANEED_MASK(q)	\
	((q->protocol == 35)?XIMP_STS_AREANEED_MASK3:XIMP_STS_AREANEED_MASK4)
#define XIMP_STS_WINDOW_MASK(q)		\
	((q->protocol == 35)?XIMP_STS_WINDOW_MASK3:XIMP_STS_WINDOW_MASK4)
#define XIMP_PRE_FONT_MASK(q)		\
	((q->protocol == 35)?XIMP_PRE_FONT_MASK3:XIMP_PRE_FONT_MASK4)
#define XIMP_STS_FONT_MASK(q)		\
	((q->protocol == 35)?XIMP_STS_FONT_MASK3:XIMP_STS_FONT_MASK4)
#define XIMP_SERVERTYPE_MASK(q)		\
	((q->protocol == 35)?0:XIMP_SERVERTYPE_MASK4)

#define _XIMP_PRE_SPOTL_MASK(p)		\
	((p == 35)?XIMP_PRE_SPOTL_MASK3:XIMP_PRE_SPOTL_MASK4)
#define _XIMP_PRE_FONT_MASK(p)		\
	((p == 35)?XIMP_PRE_FONT_MASK3:XIMP_PRE_FONT_MASK4)
#define _XIMP_STS_FONT_MASK(p)		\
	((p == 35)?XIMP_STS_FONT_MASK3:XIMP_STS_FONT_MASK4)

#define	XIMP_PRE_MASK(q)		\
    ((q->protocol == 35) ? (XIMP_PRE_AREA_MASK3    | XIMP_PRE_FG_MASK3      | \
			    XIMP_PRE_BG_MASK3      | XIMP_PRE_COLORMAP_MASK3| \
			    XIMP_PRE_BGPIXMAP_MASK3| XIMP_PRE_LINESP_MASK3  | \
			    XIMP_PRE_CURSOR_MASK3  | XIMP_PRE_AREANEED_MASK3| \
			    XIMP_PRE_SPOTL_MASK3)	\
     : 			   (XIMP_PRE_AREA_MASK4    | XIMP_PRE_FG_MASK4      | \
			    XIMP_PRE_BG_MASK4      | XIMP_PRE_COLORMAP_MASK4| \
			    XIMP_PRE_BGPIXMAP_MASK4| XIMP_PRE_LINESP_MASK4  | \
			    XIMP_PRE_CURSOR_MASK4  | XIMP_PRE_AREANEED_MASK4| \
			    XIMP_PRE_SPOTL_MASK4))

#define	XIMP_STS_MASK(q)		\
    ((q->protocol == 35) ? (XIMP_STS_AREA_MASK3    | XIMP_STS_FG_MASK3      | \
			    XIMP_STS_BG_MASK3      | XIMP_STS_COLORMAP_MASK3| \
			    XIMP_STS_BGPIXMAP_MASK3| XIMP_STS_LINESP_MASK3  | \
			    XIMP_STS_CURSOR_MASK3  | XIMP_STS_AREANEED_MASK3| \
			    XIMP_STS_WINDOW_MASK3)	\
    :			   (XIMP_STS_AREA_MASK4    | XIMP_STS_FG_MASK4      | \
			    XIMP_STS_BG_MASK4      | XIMP_STS_COLORMAP_MASK4| \
			    XIMP_STS_BGPIXMAP_MASK4| XIMP_STS_LINESP_MASK4  | \
			    XIMP_STS_CURSOR_MASK4  | XIMP_STS_AREANEED_MASK4| \
			    XIMP_STS_WINDOW_MASK4))

#define	ISFE1(q)	((q)->ximp_type == XIMP_FE_TYPE1)
#define	ISFE2(q)	((q)->ximp_type == XIMP_FE_TYPE2)
#define	ISFE3(q)	((q)->ximp_type == XIMP_FE_TYPE3)
#define	ISBE1(q)	((q)->ximp_type == XIMP_BE_TYPE1)
#define	ISFB2(q)	((q)->ximp_type == XIMP_BE_TYPE2)
#define ISFRONTEND(q)	((q)->ximp_type & XIMP_FRONTEND)
#define ISBACKEND(q)	((q)->ximp_type & XIMP_BACKEND)
#define	ISTYPE1(q)	((q)->ximp_type & XIMP_TYPE1)
#define	ISTYPE2(q)	((q)->ximp_type & XIMP_TYPE2)
#define	ISTYPE3(q)	((q)->ximp_type & XIMP_TYPE3)
#define	ISSYNC(q)	((q)->ximp_type & XIMP_SYNC)

#endif	/* _XimpP_H */
