
/*
* $Header: CascadeP.h,v 1.3 88/02/26 10:32:38 swick Exp $
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

#ifndef _XtPopupPrivate_h
#define _XtPopupPrivate_h

/***********************************************************************
 *
 * Popup Widget Private Data
 *
 ***********************************************************************/

/* New fields for the Popup widget class record */

typedef struct {int mumble;} PopupClassPart;


typedef struct _PopupClassRec {
  	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ShellClassPart		shell_class;
	WMShellClassPart	wm_shell_class;
	VendorShellClassPart	vendor_shell_class;
	TransientShellClassPart transient_shell_class;
	PopupClassPart		popup_part;
} PopupClassRec;

extern PopupClassRec popupClassRec;

/* New fields for the popup widget */

typedef struct _PopupPart {int mumble;} PopupPart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef  struct {
	CorePart 	core;
	CompositePart 	composite;
	ShellPart 	shell;
	WMShellPart	wm;
	VendorShellPart	vendor;
	TransientShellPart transient;	
	PopupPart	popup;
} PopupRec, *PopupWidget;

#endif  _XtPopupPrivate_h
