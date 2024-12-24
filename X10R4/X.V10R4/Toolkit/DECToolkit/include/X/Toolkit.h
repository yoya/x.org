/*
 *			  COPYRIGHT 1986
 *		   DIGITAL EQUIPMENT CORPORATION
 *		       MAYNARD, MASSACHUSETTS
 *			ALL RIGHTS RESERVED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
 * SHOULD NOT BE CONSTRUED AS A COMMITMENT BY DIGITAL EQUIPMENT CORPORATION.
 * DIGITAL MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE FOR
 * ANY PURPOSE.  IT IS SUPPLIED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
 *
 * IF THE SOFTWARE IS MODIFIED IN A MANNER CREATING DERIVATIVE COPYRIGHT RIGHTS,
 * APPROPRIATE LEGENDS MAY BE PLACED ON THE DERIVATIVE WORK IN ADDITION TO THAT
 * SET FORTH ABOVE.
 *
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting documentation,
 * and that the name of Digital Equipment Corporation not be used in advertising
 * or publicity pertaining to distribution of the software without specific, 
 * written prior permission.
 */

/* File: XToolkit.h - last edit by */
/* weissman     8-Jul-86 13:36 */
/* Smokey:	7-Jul-86 15:37 */
/* Larson:     17-Nov-86 10:00 */

#include "TArgs.h"

/* Misc. definitions */

#define FALSE 0
#define TRUE 1
#define NULL 0
#define xMargin 2
#define yMargin 2
#define buttonMask 0x143d /* LeftDownMotion | RightDownMotion | EnterWindow | 
			     LeaveWindow | ButtonPressed | ButtonReleased */
#define knobMask  0x1d0d  /* ButtonPressed | ButtonReleased | RightDownMotion
			     | LeftDownMotion | MiddleDownMotion */
#define allEvents 0x0fbf  /* MiddleDownMotion | RightDownMotion | ExposeCopy |
			     ExposeRegion | ExposeWindow | EnterWindow |
			     LeaveWindow | ButtonPressed | ButtonReleased |
			     KeyPressed | KeyReleased */

#define allButtons 0x0700 /* LeftMask | MiddleMask | RightMask */
/* Event Dispatch return codes */

#define PROCESSED	0		/* Event dispatched and handled */
#define NOTHANDLED	1		/* Event not handled by event proc */
#define NOENTRY		2		/* Entry not found. */

/* Associative lookup table return codes */

#define ERRNONE		0		/* No error. */
#define ERRMEMORY	1		/* Out of memory. */
#define ERRNOTFOUND	2		/* Entry not found. */

/****** begin form and geometry management definitions --- harry hersh ****/

/* Generic layout/geometry manager */

#define ResizeWindow 6
#define REQUESTYES      0       /* Request accepted. */
#define REQUESTNO       1       /* Request denied. */
#define REQUESTALMOST   2       /* Request denied. */
#define REQUESTNOENTRY	3	/* Request denied: no geometry manager */

enum TGeometryRequest {move, resize, top, bottom};

/* Form type layout/geometry manager */

#define NULLARRAY	1               /* NULL array passed. */
#define NOTAFORM	2               /* NULL array passed. */

enum XLayoutHints{ AbsoluteX,       /* Offset from parent's origin       */
                   UnchangedX,      /* Same as previous tool             */
                   RelativeX,       /* Offset from last tool's x-pos     */
                   CenterX};        /* Center tool in parent window      */

enum YLayoutHints{ AbsoluteY,       /* Offset from parent's origin       */
                   UnchangedY,      /* Same as previous tool             */
                   RelativeY,       /* Offset from last tool's x-pos     */
                   CenterY};        /* Immediately under last tool       */

enum WLayoutHints{ AbsoluteW,       /* Use specified width               */
                   UnchangedW,      /* Same as previous tool             */
                   CurrentW,        /* Use current width of tool         */
                   ParentW,         /* Extend full width of parent       */
                   ParentRight};    /* Extend to right border of parent  */

enum HLayoutHints{ AbsoluteH,       /* Use specified height              */
                   UnchangedH,      /* Same as previous tool             */
                   CurrentH,        /* Use current height of tool        */
                   ParentH,         /* Extend full height of parent      */
                   ParentBottom};   /* Extend to bottom border of parent */

typedef struct _WindowBox {
    int x, y, w, h;
    } WindowBox, *WindowBoxPtr;

typedef int (*FormFormatter)();

typedef struct _FormLayoutHints {
  int x, y,                    /* Upper-left corner of form tool   */
      w, h;                    /* Suggested size of object         */
  enum XLayoutHints xOption;
  enum YLayoutHints yOption;   /* Constraints for location         */
  enum WLayoutHints wOption;
  enum HLayoutHints hOption;   /* Constraints for size             */
} FormLayoutHints;


typedef struct _FormItem {
    Window sw;                 /* Bounding window for tool         */
    FormLayoutHints *hints;    /* Suggested size, location of tool */
    WindowBox box;             /* Current size, location           */
} FormItem;

typedef struct _FormData {
  Window parent;               /* Parent window of form            */
  int itemCount;               /* Number of tools in form          */
  FormItem *items;             /* Array of item definitions        */
  FormFormatter formatter;     /* Form organizing procedure        */
} FormData;

/******** end form definitions ***/



/********* -----> Additions by K. Langone ******/

/* Enumeration for scroll bars */
enum WhichWay {Left, Right, Up, Down};

/* Enumeration and defines for for valuator */
enum Direction {Vertical, Horizontal};
#define Invisible 0
#define Visible 1

typedef int TEntryType;

/********** begin text subwindow definitions ***********/

/* Text subwindow options */
#define wordBreak		0x01
#define scrollVertical		0x02
#define scrollHorizontal	0x04
#define scrollOnOverflow	0x08
#define resizeWidth		0x10
#define resizeHeight		0x20
#define editable		0x40

/*
 * structure to describe a block of text 
 */
typedef long TTextPosition;
typedef struct _TTextBlock {
    TTextPosition length;	/* length of text in *ptr */
    char *ptr;			/* text */
    } TTextBlock, *TextBlockPtr;

/*
 * structure used for replacement of text within a source.
 */
typedef struct _TTextReplace {
    TTextPosition startPos,  /* starting position of where to put text */
    		  endPos;    /* ending position of where to put text
			      * NOTE: if endPos=startPos then text is inserted.
			      *       it does not "replace" any text. */
    TTextBlock text;	     /* Text to be added to source */
} TTextReplace;


/*
 * types of text selection.
 */
enum SelectionType {charSelection, wordSelection, lineSelection,
    paraSelection, allSelection};

/******* end text subwindow definitions **********/




/********** begin paned windows definitions -- harry hersh ***********/

#define HORIZONTAL		1
#define VERTICAL		2

/******* end paned bwindow definitions **********/
