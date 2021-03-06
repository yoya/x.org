/*
* $Header: LoadP.h,v 1.7 88/02/26 09:32:42 swick Exp $
*/


/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#ifndef _LoadP_h
#define _LoadP_h

#include <X11/Load.h>
#include <X11/CoreP.h>

#define SEG_BUFF_SIZE		128

/* New fields for the load widget instance record */
typedef struct {
	 Pixel	fgpixel;	/* color index for text */
	 XFontStruct	*font;	/* font for text */
	 GC	myGC;		/* pointer to GraphicsContext */
/* start of graph stuff */
	 int	update;		/* update frequence */
	 int	scale;		/* scale factor */
	 int	 min_scale;	/* smallest scale factor */
	 int	 interval;	/* data point interval */
	 Boolean reverse_video;		/* display in reverse video */
	 char	*text;		/* label */
	 double max_value;	/* Max Value in window */
	 double valuedata[2048];/* record of data points */
	 XtIntervalId interval_id;
	 XtCallbackList get_load; /* proc to call to fetch load pt */
   } LoadPart;

/* Full instance record declaration */
typedef struct _LoadRec {
   CorePart core;
   LoadPart load;
   } LoadRec;

/* New fields for the Load widget class record */
typedef struct {int dummy;} LoadClassPart;

/* Full class record declaration. */
typedef struct _LoadClassRec {
   CoreClassPart core_class;
   LoadClassPart load_class;
   } LoadClassRec;

/* Class pointer. */
extern LoadClassRec loadClassRec;

#endif _LoadP_h
