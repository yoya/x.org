/* $SonyId: PreeditP.h,v 1.1 1994/05/30 13:05:43 makoto Exp $ */
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
#ifndef	_XmwPreeditP_H
#define	_XmwPreeditP_H

#include	<Xmw/Preedit.h>
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

typedef struct {
    Cardinal		number;
    Cardinal		maximum;
    Position		*position;
} LineData;


typedef struct	_XmwPreeditClassPart {
    XtPointer	extension;
} XmwPreeditClassPart;

typedef struct	_XmwPreeditClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    XmwPreeditClassPart	preedit_class;
} XmwPreeditClassRec;

typedef enum {
    XsmCURSOR_LEFT,
    XsmCURSOR_RIGHT
} XsmCursorAttachment;


extern XmwPreeditClassRec	xmwPreeditClassRec;


typedef struct	_XmwPreeditPart
{
    Boolean		add_ascent;		/* XmwNaddAscent	*/
    Window		focus_window;		/* XmwNfocusWindow	*/
    Dimension		line_spacing;		/* XmwNlineSpacing	*/
    Dimension		area_needed_width;	/* XmwNareaNeededWidth	*/
    Dimension		area_needed_height;	/* XmwNareaNeededHeight	*/
    Position		x;			/* XmwNareaX		*/
    Position		y;			/* XmwNareaY		*/
    Dimension		width;			/* XmwNareaWidth	*/
    Dimension		height;			/* XmwNareaHeight	*/
    int			top_line;		/* XmwNtopLine		*/
    int			max_lines;

    Position		x2, y2, x3, y3;
    Window		window2, window3;
    Dimension		width2, height2, width3, height3;
    long		event_mask;
    char		*value;			/* XmwNvalue		*/
    wchar_t		*wcs_value;		/* XmwNvalueWcs		*/
    Position		cursor_position;	/* XmwNcursorPosition	*/
    Position		left_cursor_position;
    Position		right_cursor_position;
    XmwCursorAttachment	attachment;
    short		columns;
    int			max_length;
    int			blink_rate;		/* XmwNblinkRate	*/
    XFontSet		font_set;		/* XmwNfontSet		*/
    int			ascent;
    int			descent;
    Pixel		foreground;		/* XmwNforeground	*/

    Boolean		editable;
    Boolean		verify_bell;
    Boolean		display_caret;		/* XmwNdisplayCaret	*/
    Boolean		has_focus;
    Boolean		blink_on;
    Boolean		cursor_on;
    Boolean		dest_on;

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
    LineData		line;
    XtIntervalId	timer_id;
} XmwPreeditPart;

typedef struct	_XmwPreeditRec {
    CorePart		core;
    XmwPreeditPart	preedit;
} XmwPreeditRec;


/* Macros for the data */
#define	TextWindow1(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.window1)
#define	TextWindow2(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.window2)
#define	TextWindow3(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.window3)
#define	TextLineSpacing(w)	(((XmwPreeditWidget)(w))-> \
				 preedit.line_spacing)
#define	TextWidth(w)		(((XmwPreeditWidget)(w))-> \
				 core.width)
#define	TextWidth1(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.width1)
#define	TextWidth2(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.width2)
#define	TextWidth3(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.width3)
#define	TextHeight2(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.height2)

#define	TextCursorPosition(w)	(((XmwPreeditWidget)(w))-> \
				 preedit.cursor_position)
#define	TextValue(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.value)
#define	TextValueWcs(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.wcs_value)
#define	TextFontSet(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.font_set)
#define	TextColumns(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.columns)
#define	TextEditable(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.editable)
#define	TextColumns(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.columns)
#define	TextMaxLength(w)	(((XmwPreeditWidget)(w))-> \
				 preedit.max_length)
#define	TextBlinkRate(w)	(((XmwPreeditWidget)(w))-> \
				 preedit.blink_rate)
#define	TextGC(w)		(((XmwPreeditWidget)(w))-> \
				 preedit.gc)

#endif	/* _XmwPreeditP_H */
