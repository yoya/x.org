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
extern Widget drawform;
struct zoomd {
    struct zoomd *zp;
    double lx, ux;
    double ly, uy;
};
extern int depth;
extern struct zoomd *zoomp;
extern long iter;
XWindowAttributes draw_wattr;

void julia(w, closure, call_data)
    Widget w;
    XtPointer closure;
    XtPointer call_data;
{
    double a,b;
    double wx, wy, wx2, wy2;
    double sx, x, y;
    double incrx, incry;
    long ix, iy;
    static long width = 0;
    static long mwidth = 0;
    static long bias = 1;

    Arg arg;
    Dimension wwidth, wheight;
    static XImage *xip1 = NULL, *xip2 = NULL;
    static char *dp1 = NULL, *dp2 = NULL;
    char *malloc();
    char ab[80];
    Window draw_win;
    static Window s_win = NULL;
    GC draw_gc;
    Screen *draw_Screen;
    Display *draw_d;
    Visual *draw_v;
    Window Root;
    int S;				/* Screen */


    draw_d = XtDisplay(drawform);
    if (zoomp == NULL) 		{ XBell(draw_d, 0); return;}
    draw_win = XtWindow(drawform);
    draw_Screen = XtScreen(drawform);
    draw_gc = draw_Screen->default_gc;
    draw_v = draw_Screen->root_visual;

    XtSetArg(arg, XtNwidth, &wwidth);
    XtGetValues(drawform, &arg, 1);
    XtSetArg(arg, XtNheight, &wheight);
    XtGetValues(drawform, &arg, 1);

    XSelectInput(draw_d, draw_win, ButtonPressMask | ButtonReleaseMask);
    while (1) {
	int winx, winy;
	XEvent report;
	XNextEvent(draw_d, &report);
	if (report.type == ButtonRelease) {
/*	    if (report.xbutton.x <= 0 || report.xbutton.y <= 0) {	*/
	    /* make sure button release is in drawform window */
	    if (report.xbutton.window != draw_win ||
	        report.xbutton.x & 0x8000 || 
		report.xbutton.y & 0x8000 ||
	        (report.xbutton.x == 0 && report.xbutton.y == 0) ||
		report.xbutton.x > (long)wwidth || 
		report.xbutton.y > (long)wheight) {
		    XBell(draw_d, 0);
		    continue;
	    }
	    a = ( (double)report.xbutton.x/ (double)wwidth);
	    b = ( (double)report.xbutton.y/ (double)wheight);
	    a = (a * (zoomp->ux - zoomp->lx)) + zoomp->lx;
	    b = zoomp->uy - (b * (zoomp->uy - zoomp->ly));
	    /*printf("julia: a = %f, b = %f\n", a, b);*/
	    break;
	}
	continue;
    }
    S = DefaultScreen(draw_d);
    Root = RootWindow(draw_d, S);
    if (s_win == NULL) {
	s_win = XCreateSimpleWindow(draw_d, Root, 0, 0, 256, 256, 1, 1, 0);
	XSelectInput(draw_d, s_win, ExposureMask);
	XMapWindow(draw_d, s_win);
	XSync(draw_d, False);
	while (1) {
	    XEvent report;
	    XNextEvent(draw_d, &report);
	    if (report.type == Expose) break;
	}
    }
    sprintf(ab, "%f + %fi", a,b);
    XStoreName(draw_d, s_win, ab);
    XClearWindow(draw_d, s_win);

    XGetWindowAttributes(draw_d, s_win, &draw_wattr);
    if (width != draw_wattr.width) {
	width = draw_wattr.width;
	mwidth = (depth > 1)? width: (1 + width/8);
	/* XDestroyImage should free dp1 and dp2 as well */
	if (xip1) {XDestroyImage(xip1); XDestroyImage(xip2); }
	dp1 = malloc(mwidth);
	dp2 = malloc(mwidth);
	if (dp2 == NULL) {printf("malloc failed\n"); return; }

	xip1 = XCreateImage(draw_d, draw_v, depth, ZPixmap, 0, dp1, 
		width, 1, 8, mwidth);
	xip2 = XCreateImage(draw_d, draw_v, depth, ZPixmap, 0, dp2, 
		width, 1, 8, mwidth);
	xip1->byte_order = xip2->byte_order = MSBFirst;
	xip1->bitmap_bit_order = xip2->bitmap_bit_order = MSBFirst;
    }
    sx = x = -1.5; y = -1.5;
    incrx = -x/draw_wattr.width * 2;
    incry = -y/draw_wattr.height * 2;


for (iy = 0; iy < (1 + draw_wattr.height/2); iy++, y+= incry) {
    long i;
    x = sx;
    for (ix = 0; ix < draw_wattr.width; ix++, x+= incrx) {
        wx = x; wy = y;
        for (i = 0; i < iter; i++) {
	    if ((wx2 = wx * wx) + (wy2 = wy * wy) > 4) break;
	    wy = 2 * wx * wy + b;
	    wx = wx2 - wy2 + a;
        }
        if (depth > 1) {		/* color */
	    *(dp1 + ix) = i + bias;
	    *(dp2 + width - ix - 1) = i + bias;
	} else {
	    if (i == iter) {		 /* could also test if (i & 2) */
		dp1[ix/8] |= 1 << (7 - ix&7);
		dp2[(width - ix - 1)/8] |= 1 << (8 - (width - ix)&7);
	    }
	}
    }

   XPutImage(draw_d, s_win, draw_gc, xip1, 0,0, 0, iy, width, 1);
if (iy != draw_wattr.height - iy)
   XPutImage(draw_d,s_win,draw_gc,xip2,0,0,0, draw_wattr.height - iy, width, 1);
   if (depth == 1) for (i = 0 ; i < mwidth; i++) *(dp1 + i) = *(dp2 + i) = 0;
}

}
