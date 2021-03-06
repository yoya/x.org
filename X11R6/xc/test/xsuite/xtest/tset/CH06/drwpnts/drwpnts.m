/*
 
Copyright (c) 1990, 1991  X Consortium

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

 *
 * Copyright 1990, 1991 by UniSoft Group Limited.
 * 
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of UniSoft not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.  UniSoft
 * makes no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * $XConsortium: drwpnts.m,v 1.6 94/04/17 21:04:58 rws Exp $
 */
>>TITLE XDrawPoints CH06
void

Display	*display = Dsp;
Drawable d;
GC		gc;
XPoint	*points = defpoints;
int 	npoints = sizeof(defpoints)/sizeof(XPoint);
int 	mode = CoordModeOrigin;
>>EXTERN

static XPoint	defpoints[] = {
	{2, 2},
	{10, 5},
	{50, 21},
	{20, 12},
	{78, 21},
	{0, 0},
};

>>ASSERTION Good A
A call to xname
draws 
.A npoints
points 
specified by
.A points
in the drawable
.A d .
>>STRATEGY
Draw points.
Direct validation or pixmap compare.
>>CODE
XVisualInfo *vp;

	for (resetvinf(VI_WIN_PIX); nextvinf(&vp); ) {

		d = makewin(display, vp);
		gc = makegc(display, d);

		XCALL;
		PIXCHECK(display, d);
	}
	CHECKPASS(nvinf());

>>ASSERTION def
When the
.A mode
is
.S CoordModeOrigin ,
then all coordinates are treated as relative to the origin.
>>ASSERTION Good A
When the
.A mode
is
.S CoordModePrevious ,
then all coordinates after the first are taken relative to
the previous point.
>>STRATEGY
Draw points
Pixmap verify
>>CODE
XVisualInfo *vp;

	for (resetvinf(VI_WIN_PIX); nextvinf(&vp); ) {

		d = makewin(display, vp);
		gc = makegc(display, d);

		mode = CoordModePrevious;

		XCALL;
		PIXCHECK(display, d);
	}
	CHECKPASS(nvinf());

>>ASSERTION Good B 1
A call to xname draws the points in the order listed in the array.
>>ASSERTION gc
On a call to xname the GC components
.M function ,
.M plane-mask ,
.M foreground ,
.M subwindow-mode ,
.M clip-x-origin ,
.M clip-y-origin ,
and 
.M clip-mask
are used.
>>ASSERTION Bad A
.ER BadDrawable
>>ASSERTION Bad A
.ER BadGC
>>ASSERTION Bad A
.ER BadMatch inputonly
>>ASSERTION Bad A
.ER BadMatch gc-drawable-depth
>>ASSERTION Bad A
.ER BadMatch gc-drawable-screen
>>ASSERTION Bad A
.ER BadValue mode CoordModeOrigin CoordModePrevious
>># HISTORY steve Completed	Written in new format and style
>># HISTORY kieron Completed	Global and pixel checking to do - 19/11/90
>># HISTORY dave Completed	Final checking to do - 21/11/90
