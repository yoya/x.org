/* $SonyId: Xmw.h,v 1.1 1994/05/30 13:05:43 makoto Exp $ */
/******************************************************************
Copyright (c) 1994  Sony Corporation

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
#ifndef _Xmw_h
#define _Xmw_h
#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	"XmwStrDefs.h"

#define	XmwICON_DEFAULT_COLOR	0x1000000
#define XmwICON_KEEP_COLOR	0x2000000

typedef enum {
    XmwHIGHLIGHT_NORMAL,
    XmwHIGHLIGHT_SELECTED,
    XmwHIGHLIGHT_SECONDARY_SELECTED,
    XmwHIGHLIGHT_TERTIARY_SELECTED
} XmwHighlightMode;

typedef enum {
    XmwCURSOR_LEFT,
    XmwCURSOR_RIGHT
} XmwCursorAttachment;


enum{
    XmwCR_BROWSE_SELECT,
    XmwCR_NOTIFY,
    XmwCR_SET,
    XmwCR_UNSET
};


typedef struct {
    char		ptr[32];
    wchar_t		wcs_ptr[32];
    int			length;
    XmwHighlightMode	mode;
} XmwTextBlockRec, *XmwTextBlock;

typedef struct {
    int			reason;
    XEvent		*event;
    Boolean		doit;
    long		currInsert, newInsert;
    long		startPos, endPos;
    XmwTextBlock	text;
} XmwTextVerifyStruct, *XmwTextVerifyPtr;

#endif  /* _Xmw_h */
/* DON'T ADD ANY STUFF AFTER THIS #endif */
