/*
 * Copyright (c) 1993 2001 S.A.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the name of
 * 2001 S.A. may not be used in any advertising or publicity relating to the
 * software without the specific, prior written permission of 2001 S.A.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL 2001 S.A. BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef Xtf_Viewer_h
#define Xtf_Viewer_h

/***********************************************************************
 *
 * Widget for Fresco Viewers
 *
 ***********************************************************************/

#include <X11/Fresco/_leave.h>
#include <X11/Fresco/Impls/Xdefs.h>
#include <X11/Xmu/Converters.h>
#include <X11/Fresco/Impls/Xundefs.h>

/* Parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		white
 backingStore	     BackingStore	BackingStore	default
 border		     BorderColor	Pixel		Black
 borderWidth	     BorderWidth	Dimension	1
 destroyCallback     Callback		Pointer		NULL
 font		     Font		XFontStruct*	fixed
 foreground	     Foreground		Pixel		black
 height		     Height		Dimension	164
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 width		     Width		Dimension	164
 x		     Position		Position	0
 y		     Position		Position	0
 fresco		     Fresco		Fresco		NULL
 glyph		     Glyph		Glyph		NULL
 viewer		     Viewer		Viewer		NULL

*/

#define XtNfresco "fresco"
#define XtCFresco "Fresco"
#define XtRFresco "Fresco"
#define XtNglyph "glyph"
#define XtCGlyph "Glyph"
#define XtRGlyph "Glyph"
#define XtNviewer "viewer"
#define XtCViewer "Viewer"
#define XtRViewer "Viewer"
#define XtNfrescoWindow "frescoWindow"
#define XtCFrescoWindow "FrescoWindow"
#define XtRFrescoWindow "FrescoWindow"

#include <X11/Fresco/_enter.h>

struct Glyph;
struct Fresco;

typedef struct _XtfViewerRec* XtfViewer;

typedef struct _XtfViewerClassRec* XtfViewerClass;

extern WidgetClass XtfviewerWidgetClass;

/*
 * Creation entry points:
 */

#ifdef _NO_PROTO

extern Widget XtCreateXtfViewer();

#else /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern Widget XtCreateXtfViewer(
    Widget p, ArgList args, Cardinal n
);
#endif /* _NO_PROTO */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
