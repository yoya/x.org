/* $SonyId: TextOutP.h,v 1.1 1994/05/30 13:05:43 makoto Exp $ */
/******************************************************************
Copyright (c) 1993, 1994  Sony Corporation

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
#ifndef	_XmwTextOutP_h
#define	_XmwTextOutP_h

#include	<Xmw/TextOut.h>
#include	<X11/Xaw/SimpleP.h>

#define IBEAM_WIDTH 3
#define CARET_WIDTH 9
#define CARET_HEIGHT 5

static unsigned char caretBits[] = {
   0x10, 0x00, 0x38, 0x00, 0x6c, 0x00, 0xc6, 0x00, 0x83, 0x01 };

typedef struct {
    Position		position;
    XmwHighlightMode	mode;
} HighlightRec;

typedef struct {
    Cardinal		number;
    Cardinal		maximum;
    HighlightRec	*list;
} HighlightData;

/* TextOut class structure */
typedef struct	_XmwTextOutClassPart {
    XtPointer	extension;
} XmwTextOutClassPart;


/* Full class record declaration for XmwTextOut class */
typedef struct	_XmwTextOutClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    XmwTextOutClassPart	text_out_class;
} XmwTextOutClassRec;


/* XmwTextOut instance record */
typedef struct	_XmwTextOutPart
{
    Dimension		old_width, old_height;
    long		event_mask;
    char		*value;			/* XmwNvalue		*/
    wchar_t		*wcs_value;		/* XmwNvalueWcs		*/
    Position		cursor_position;	/* XmwNcursorPosition	*/
    Position		left_cursor_position;
    Position		right_cursor_position;
    short		columns;		/* XmwNcolumns		*/
    XmwCursorAttachment	attachment;
    int			max_length;		/* XmwNmaxLength	*/
    XFontSet		font_set;		/* XmwNfontSet		*/
    int			ascent;
    int			descent;
    Dimension		max_extent;
    Pixel		foreground;		/* XmwNforeground	*/
    Position		top_margin;		/* XtNtopMargin		*/
    Position		bottom_margin;		/* XtNbottomMargin	*/
    Position		left_margin;		/* XtNleftMargin	*/
    Position		right_margin;		/* XtNrightMargin	*/

    Boolean		editable;
    Boolean		verify_bell;		/* XmwNverifyBell	*/
    Boolean		display_caret;		/* XtNdisplayCaret	*/
    Boolean		enter;
    Boolean		cursor_on;
    Boolean		caret_on;

    GC			gc;
    GC			inv_gc;
    GC			xor_gc;

    int			string_length;

    int			alloc_size;

    int			cursor_height;
    int			cursor_width;
    Pixmap		dest_cursor;
    Pixmap		cursor;

    HighlightData	highlight;
    XtIntervalId	timer_id;
} XmwTextOutPart;


/* Full instance record declaration */
typedef struct	_XmwTextOutRec {
    CorePart		core;
    SimplePart		simple;
    XmwTextOutPart	text_out;
} XmwTextOutRec;


/* Macros for the data */
#define	TextLineSpacing(w)	(((XmwTextOutWidget)(w))-> \
				 text_out.line_spacing)
#define	TextWidth(w)		(((XmwTextOutWidget)(w))-> \
				 core.width)
#define	TextCursorPosition(w)	(((XmwTextOutWidget)(w))-> \
				 text_out.cursor_position)
#define	TextValue(w)		(((XmwTextOutWidget)(w))-> \
				 text_out.value)
#define	TextValueWcs(w)		(((XmwTextOutWidget)(w))-> \
				 text_out.wcs_value)
#define	TextFontSet(w)		(((XmwTextOutWidget)(w))-> \
				 text_out.font_set)
#define	TextMaxLength(w)	(((XmwTextOutWidget)(w))-> \
				 text_out.max_length)
#define	TextGC(w)		(((XmwTextOutWidget)(w))-> \
				 text_out.gc)

#endif	/* _XmwTextOutP_h */
