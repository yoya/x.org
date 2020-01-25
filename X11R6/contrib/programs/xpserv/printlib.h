/*
Copyright (c) 1994  Matthew Reynolds and Qualcomm Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
QUALCOMM INC., OR MATTHEW REYNOLDS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Qualcomm Inc., or Matthew 
Reynolds shall not be used in advertising or otherwise to promote the sale, 
use or other dealings in this Software without their prior written 
authorization.

*/

#ifndef _PRINTLIB_H_
#define _PRINTLIB_H_
#include <X11/Xlib.h>
#include <X11/Xmd.h>

#define PRINTNAME	"xprint"
#define PrintWindow	-1

Bool XOpenPrintWindow( 
    Display	*dpy,
    Drawable	drawable,
    char	*printfile
);

Bool XClosePrintWindow(
    Display	*dpy,
    Drawable	drawable
);

Bool XSetPrintParams(
    Display	*dpy,
    Bool      preserve_aspect,
    int       font_scale,
    int       pixmap_scale,
    int       page_width,			/* in points */
    int       page_height
);

#endif /* _PRINTLIB_H_ */
