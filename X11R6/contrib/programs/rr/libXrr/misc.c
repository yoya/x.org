/*************************************************************************
 *                                                                       *
 *  Copyright (c) 1992, 1993 Ronald Joe Record                           *
 *                                                                       *
 *      Ronald Joe Record (408) 458-3718                                 *
 *      212 Owen St., Santa Cruz, California 95062 USA                   *
 *                                                                       *
 *************************************************************************/
 /*
 *  The X Consortium, and any party obtaining a copy of these files from
 *  the X Consortium, directly or indirectly, is granted, free of charge,
 *  a full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 *  nonexclusive right and license to deal in this software and
 *  documentation files (the "Software"), including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons who receive
 *  copies from any such party to do so.  This license includes without
 *  limitation a license to do the foregoing actions under any patents of
 *  the party supplying this software to the X Consortium.
 */
/* routines to center a window on a screen.
 * center_vertical() takes as arguments a display, window and window height.
 * it reconfigures the window to be centered vertically.
 */

#include <X11/Xlib.h>

void
center_vertical(dpy, w, height)
Display *dpy;
Window w;
int height;
{
	Window root, child;
	int top, x, y, rootX, rootY;
	unsigned int bw, d, new_w, new_h;
	XWindowChanges values;

    XGetGeometry(dpy,w,&root,&x,&y,&new_w,&new_h,&bw,&d);
	XTranslateCoordinates(dpy, w, root, 0, 0, &rootX, &rootY, &child);
	top = XDisplayHeight(dpy, DefaultScreen(dpy)) - height;
	y = (top/2) - rootY;
	if (y < 0)
		y = -y;
	if (y > 0) {
		values.y = (top - (2*y)) / 2,0;
		if (values.y < 0)
			values.y = 0;
		XConfigureWindow(dpy,w,CWY,&values);
	}
}

/* 
 * center_horizontal() takes as arguments a display, window and window width.
 * it reconfigures the window to be centered horizontally.
 */

void
center_horizontal(dpy, w, width)
Display *dpy;
Window w;
int width;
{
	Window root, child;
	int left, x, y, rootX, rootY;
	unsigned int bw, d, new_w, new_h;
	XWindowChanges values;

    XGetGeometry(dpy,w,&root,&x,&y,&new_w,&new_h,&bw,&d);
	XTranslateCoordinates(dpy, w, root, 0, 0, &rootX, &rootY, &child);
	left = XDisplayWidth(dpy, DefaultScreen(dpy)) - width;
	x = (left/2) - rootX;
	if (x < 0)
		x = -x;
	if (x > 0) {
		values.x = (left - (2*x)) / 2, 0;
		if (values.x < 0)
			values.x = 0;
		XConfigureWindow(dpy,w,CWX,&values);
	}
}
