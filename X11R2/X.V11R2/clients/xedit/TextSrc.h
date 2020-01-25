/*
* $Header: TextSrc.h,v 1.1 88/02/23 21:05:41 rws Exp $
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

#include <X11/Text.h>

typedef enum {XtsdLeft, XtsdRight} XtTextScanDirection;
typedef enum {XtstPositions, XtstWhiteSpace, XtstEOL, XtstParagraph, XtstAll}
    XtTextScanType;

typedef struct {
    int  firstPos;
    int  length;
    char *ptr;
    Atom format;
    } XtTextBlock, *XtTextBlockPtr;


/* the data field is really a pointer to source info, see disk and 
   stream sources in TextKinds.c */

typedef struct _XtTextSource {
    int			(*Read)();
    int			(*Replace)();
    XtTextPosition	(*GetLastPos)();
    int			(*SetLastPos)();
    XtTextPosition	(*Scan)();
    void		(*AddWidget)( /* source, widget */ );
    void		(*RemoveWidget)( /* source, widget */ );
    void		(*SetSelection)( /* source, left, right */);
    Boolean		(*GetSelection)( /* source, left, right */);
    XtTextEditType	edit_mode;
    caddr_t		data;	    
    };

typedef struct _XtTextSink {
    XFontStruct	*font;
    int foreground;
    int (*Display)();
    int (*InsertCursor)();
    int (*ClearToBackground)();
    int (*FindPosition)();
    int (*FindDistance)();
    int (*Resolve)();
    int (*MaxLines)();
    int (*MaxHeight)();
    caddr_t data;
    };
typedef enum {XtisOn, XtisOff} XtTextInsertState;

typedef enum {XtsmTextSelect, XtsmTextExtend} XtTextSelectionMode;

typedef enum {XtactionStart, XtactionAdjust, XtactionEnd}
    XtTextSelectionAction;
/*
typedef struct {
    XtTextPosition left, right;
    XtTextSelectType type;
} XtTextSelection;
*/
#define EditDone 0
#define EditError 1
typedef enum  {Normal, Selected }highlightType;
