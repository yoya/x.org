/*
 *    Copyright (c) 1994 Cray Research Inc.
 * 
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so.  This license includes without
 * limitation a license to do the foregoing actions under any patents of
 * the party supplying this software to the X Consortium.
 * 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS". EXCEPT AS EXPRESSLY PROVIDED IN
 * THE CONTRIBUTION LETTER, ALL WARRANTIES WITH RESPECT TO THE SOFTWARE
 * ARE, EXPRESS OR IMPLIED, ARE HEREBY DISCLAIMED AND EXCLUDED, INCLUDING
 * WITHOUT LIMITATION ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE OR USE, OR FREEDOM FROM INFRINGEMENT OR VIOLATION
 * OF THIRD PARTY INTELLECTUAL PROPERTY RIGHTS, AND ALL OBLIGATIONS
 * OR LIABILITIES FOR DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE
 * PERFORMANCE OF, OR THE USE OF THE SOFTWARE.
 */

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <stdio.h>
#include <malloc.h>
extern Widget detaillabel, drawform;
extern Widget xlabel, ylabel;
struct zoomd {
    struct zoomd *zp;
    double lx, ux;
    double ly, uy;
};
struct zoomd *zoomp = NULL, *zoompn;

extern int depth;
extern long iter;
extern int ncpus;
long mwidth;
long bias = 1;					/* bias into color table */
Dimension nwidth = 0;
Dimension wwidth = 0;
Dimension wheight = 0;

Arg arg;
XImage *xip = NULL;
char *dp = NULL;
Window draw_win;
GC draw_gc;
Screen *draw_Screen;
Display *draw_d;
Visual *draw_v;

void mandel();

#define MIN(a,b)((a) < (b) ? (a) : (b))
#define MAX(a,b)((a) < (b) ? (b) : (a))

void resetmandel(w, closure, call_data)
    Widget w;
    XtPointer closure;
    XtPointer call_data;
{
    if (zoomp) {	/* reset to beginning by popping zps off stack */
	while (zoompn = zoomp->zp) {
	    free(zoomp);
	    zoomp = zoompn;
	}
    } else {			/* This is first time */
	zoomp = (struct zoomd *) malloc(sizeof (struct zoomd));
	zoomp->zp = NULL;	/* NULL means last in stack - don't pop */
	zoomp->lx = -2.25; zoomp->ux = .75;
	zoomp->ly = -1.5; zoomp->uy = 1.5;
    }
    setxylabels();
    setiter();
    mandel(w, closure, call_data);
}

void mandel(w, closure, call_data)
    Widget w;
    XtPointer closure;
    XtPointer call_data;
{
    register double wx, wy, wx2, wy2;
    double x, y, x0, y0;
    double incrx, incry;
    long ix, iy;
    long i;
    long zero = 0;

    draw_d = XtDisplay(drawform);
    draw_win = XtWindow(drawform);
    draw_Screen = XtScreen(drawform);
    draw_gc = draw_Screen->default_gc;
    draw_v = draw_Screen->root_visual;

    /*
     * get height & width of drawing window in case they changed
     * this saves a round trip XGetWindowAttributes call 
     */
    XtSetArg(arg, XtNwidth, &nwidth);
    XtGetValues(drawform, &arg, 1);
    XtSetArg(arg, XtNheight, &wheight);
    XtGetValues(drawform, &arg, 1);

    if (wwidth != nwidth) {
        wwidth = nwidth;
        mwidth = (depth > 1)? wwidth: (1 + wwidth/8);
        if (xip)  XDestroyImage(xip);	/* will free(dp) automatically */

	dp = malloc(mwidth);
	if (dp == NULL) {printf("malloc failed\n"); return; }
	xip = XCreateImage(draw_d, draw_v, depth, ZPixmap, 0, dp,
                wwidth, 1, 8, mwidth);
        xip->byte_order = MSBFirst;
        xip->bitmap_bit_order = MSBFirst;
    }
 
    incrx = (zoomp->ux - zoomp->lx)/(long)wwidth;
    incry = (zoomp->uy - zoomp->ly)/(long)wheight;

    y = zoomp->uy;
    if (closure == 0) XClearWindow(draw_d, draw_win);


if (depth == 1) {
    for (iy = 0; iy < (long)wheight; iy++, y-= incry) {
	long i;
	for (i = 0; i < mwidth; i++) *(dp + i) = 0;
	x = zoomp->lx;
	for (ix = 0; ix < (long)wwidth; ix++, x+= incrx) {
	    wx = x; wy = y;
	    for (i = 0; i < iter; i++) {
		    if ((wx2 = wx * wx) + (wy2 = wy * wy) > 4) break;
		    wy = 2 * wx * wy + y;
		    wx = wx2 - wy2 + x;
	    }
	    if (i == iter) dp[ix/8] |= 1 << (7 - ix%8);
	    /* could also test if (i & 2) */
	}
	XPutImage(draw_d, draw_win, draw_gc, xip, 0,0, 0, iy, wwidth, 1);
    }
} else {			/* assumes depth == 8 */
    for (iy = 0; iy < (long)wheight; iy++, y-= incry) {
	long i;
	x = zoomp->lx;
	for (ix = 0; ix < (long)wwidth; ix++, x+= incrx) {
	    wx = x; wy = y;
	    for (i = 0; i < iter; i++) {
		    if ((wx2 = wx * wx) + (wy2 = wy * wy) > 4) break;
		    wy = 2 * wx * wy + y;
		    wx = wx2 - wy2 + x;
	    }
	    dp[ix] = i + bias;	/* use iteration count as color index */
	    
	}
	XPutImage(draw_d, draw_win, draw_gc, xip, 0,0, 0, iy, wwidth, 1);
    }
}
}

void zoom(w, closure, call_data)
    Widget w;
    XtPointer closure;
    XtPointer call_data;
{
    Window draw_win;
    GC draw_gc;
    Screen *draw_Screen;
    Display *draw_d;
    Window Root;
    int S;

    draw_d = XtDisplay(drawform);
    if (zoomp == NULL) { XBell(draw_d, 0); return; }
    draw_win = XtWindow(drawform);
    draw_Screen = XtScreen(drawform);
    draw_gc = draw_Screen->default_gc;
    S = DefaultScreen(draw_d);
    Root = RootWindow(draw_d, S);

    XSetForeground(draw_d, draw_gc, 1);
    XSetSubwindowMode(draw_d, draw_gc, IncludeInferiors);
    XSetFunction(draw_d, draw_gc, GXxor);

    XSelectInput(draw_d, draw_win, 
	ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    while (1) {
	static int rubberband = 1;
	XEvent report;
	XNextEvent(draw_d, &report);
	switch(report.type) {
	    int winx0,winy0,winx1,winy1,width,height;
	    int x0, y0, x1, y1;
	    double tw, th;

	    case ButtonPress:
		XGrabServer(draw_d);
		x0 = winx0 = report.xbutton.x;
		y0 = winy0 = report.xbutton.y;
		rubberband = 0;
		width = height = 0;
		break;

	    case ButtonRelease:
		winx1 = report.xbutton.x;
		winy1 = report.xbutton.y;
	XDrawRectangle(draw_d,draw_win, draw_gc, x0, y0, width, width);
		if ((width = winx1 - winx0) < 0)
			width = - width;
		if ((height = winy1 - winy0) < 0)
			height = - height;
		if (width < height)
			width = height;
		x0 = (winx1 > winx0) ? winx0 : winx0 - width;
		y0 = (winy1 > winy0) ? winy0 : winy0 - width;
		x1 = x0 + width;
		y1 = y0 + width;
	XDrawRectangle(draw_d,draw_win, draw_gc, x0, y0, width, width);
		rubberband = 1;
		XSetFunction(draw_d, draw_gc, GXcopy);
		XUngrabServer(draw_d);
		XFlush(draw_d);
		tw = zoomp->ux - zoomp->lx;
		th = zoomp->uy - zoomp->ly;
		zoompn = (struct zoomd *) malloc(sizeof (struct zoomd));
		zoompn->zp = zoomp;		/* push onto stack */
		zoompn->ux = zoomp->lx + ((double)x1/(double)wwidth) * tw;
		zoompn->lx = zoomp->lx + ((double)x0/(double)wwidth) * tw;
		zoompn->ly = zoomp->uy - ((double)y1/(double)wheight) * th;
		zoompn->uy = zoomp->uy - ((double)y0/(double)wheight) * th;
		zoomp = zoompn;			/* zoomp is current pointer */
		setxylabels();
		mandel(w, closure, call_data);
		return;			/* call mandel with new width, height */

	    case MotionNotify:
		if (rubberband) break;
		while (XCheckTypedEvent(draw_d, MotionNotify, &report));
	XDrawRectangle(draw_d,draw_win, draw_gc, x0, y0, width, width);
		winx1 = report.xbutton.x;
		winy1 = report.xbutton.y;
		if ((width = winx1 - winx0) < 0)
			width = - width;
		if ((height = winy1 - winy0) < 0)
			height = - height;
		if (width < height)
			width = height;
		x0 = (winx1 > winx0) ? winx0 : winx0 - width;
		y0 = (winy1 > winy0) ? winy0 : winy0 - width;
	XDrawRectangle(draw_d,draw_win, draw_gc, x0, y0, width, width);
		break;
	    default:
		break;
	}
    }
}
void mooz(w, closure, call_data)
    Widget w;
    XtPointer closure;
    XtPointer call_data;
{
    draw_d = XtDisplay(drawform);
    if (zoomp == NULL) { XBell(draw_d, 0); return; }
    if (zoomp->zp == NULL) { XBell(draw_d, 0); return; }

    if (zoompn = zoomp->zp) {	/* last on stack */
	free(zoomp);
	zoomp = zoompn;
    }
    setxylabels();
    mandel(w, closure, call_data);
}
void redo(w, closure, call_data)
    Widget w;
    XtPointer closure;
    XtPointer call_data;
{
    draw_d = XtDisplay(drawform);
    if (zoomp == NULL) { XBell(draw_d, 0); return; }
    mandel(w, 1, call_data);
}
void detail(w, closure, call_data)
    Widget w;
    XtPointer closure;
    XtPointer call_data;
{
    if (depth > 1) iter += 256; else iter += 64;
    setiter();
    /*mandel(w, 1, call_data);*/
}
void reset(w, closure, call_data)
    Widget w;
    XtPointer closure;
    XtPointer call_data;
{
    if (depth > 1) iter = 256; else iter = 64;
    setiter();
}

setiter()
{
    char siter[20];
    sprintf(siter, "%d iterations", iter);
    arg.name = XtNlabel; arg.value = (XtArgVal) siter;
    XtSetValues(detaillabel, &arg, 1);
}

setxylabels()
{
    char xtext[80], ytext[80];
    sprintf(xtext,"%.14g < x < %.14g", zoomp->lx, zoomp->ux);
    arg.name = XtNlabel; arg.value = (XtArgVal) xtext;
    XtSetValues(xlabel, &arg, 1);
    sprintf(ytext,"%.14g < y < %.14g", zoomp->ly, zoomp->uy);
    arg.name = XtNlabel; arg.value = (XtArgVal) ytext;
    XtSetValues(ylabel, &arg, 1);
}
