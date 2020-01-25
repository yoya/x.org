/* $SonyId: XlcpP.h,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#ifndef _XlcpP_H
#define _XlcpP_H
#include	"Xlcp.h"

#define JAPANESE_CONVERSION	"JAPANESE_CONVERSION"
#define JAPANESE_CONVERSION2	"_JAPANESE_CONVERSION"

#define	CONVERSION_REQUEST	"CONVERSION_REQUEST"
#define	CONVERSION_NOTIFY	"CONVERSION_NOTIFY"
#define	CONVERSION_END		"CONVERSION_END"
#define	CONVERSION_END_REQUEST	"CONVERSION_END_REQUEST"
#define CONVERSION_CLOSE	"CONVERSION_CLOSE"
#define CONVERSION_PROPERTY	"CONVERSION_PROPERTY"

#define COMPOUND_TEXT		"COMPOUND_TEXT"

#define CONVERSION_INPLACE	"_XLC_ON_THE_SPOT"
#define CONVERSION_STATUS	"_XLC_STATUS"

typedef struct _InplaceWindowAttributes {
    unsigned long		back;
    unsigned long		border;
    unsigned int		bwidth;
} InplaceWindowAttributes;

typedef struct _InplaceDrawingSet {
    unsigned long		fore;
    unsigned long		back;
    Font			font8;
    Font			font16;
    Font			efont16;
} InplaceDrawingSet;

typedef struct _InplaceFrame {
    unsigned int		x;
    unsigned int		y;
    unsigned int		width;
    unsigned int		height;
    unsigned int		x_off;
    unsigned int		y_off;
    unsigned int		line_height;
} InplaceFrame;

typedef struct _InplaceInfo {
    unsigned int		in_flag;
    Window			in_win;
    InplaceWindowAttributes	in_attr;
    InplaceDrawingSet		in_draw;
    InplaceFrame		in_frame;
} InplaceInfo;

#define AutoReplace		1
#define AllInformation		2
#define FrameInformation	4
#define OffsetInformation	8

typedef struct _PropertyQueue {
    struct _PropertyQueue	*next;
    struct _PropertyQueue	*previous;
    Window			 input_window;
    Window			 client_window;
    Atom			 client_text;
    Atom			 client_property;
    Atom			 client_inplace;
    Atom			 selection;
    InplaceInfo			*info;
    Boolean			 connect_flag;
    Boolean			 conversion_close_flag;
    Widget			 preedit_widget;
    Buffer			*buffer;
} PropertyQueue;

#endif /* _XlcpP_H */
