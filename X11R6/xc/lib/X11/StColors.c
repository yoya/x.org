/* $XConsortium: StColors.c,v 11.15 94/04/17 20:21:10 rws Exp $ */
/*

Copyright (c) 1986  X Consortium

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
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

*/

#include "Xlibint.h"

XStoreColors(dpy, cmap, defs, ncolors)
register Display *dpy;
Colormap cmap;
XColor *defs;
int ncolors;
{
    register int i;
    xColorItem citem;
    register xStoreColorsReq *req;

    LockDisplay(dpy);    
    GetReq(StoreColors, req);

    req->cmap = cmap;

    req->length += (ncolors * SIZEOF(xColorItem)) >> 2; /* assume size is 4*n */

    for (i = 0; i < ncolors; i++) {
	citem.pixel = defs[i].pixel;
	citem.red = defs[i].red;
	citem.green = defs[i].green;
	citem.blue = defs[i].blue;
	citem.flags = defs[i].flags;

	/* note that xColorItem doesn't contain all 16-bit quantities, so
	   we can't use Data16 */
	Data(dpy, (char *)&citem, (long) SIZEOF(xColorItem)); 
			/* assume size is 4*n */
    }
    UnlockDisplay(dpy);
    SyncHandle();
}
