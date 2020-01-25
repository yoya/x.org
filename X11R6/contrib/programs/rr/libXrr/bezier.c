
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

/*
 *	@(#) bezier.c 9.1 93/06/14 SCOINC
 */
/*
 *  Ported from CGI to X11 on 20 Apr 1993 by rr@sco.com
 */
/*	X11 library module to allow drawing of curves in the plane
 *	utilizing the method due to P. Bezier, of the French firm
 *	Regie Renault.
 *
 *	Intended to be used as follows :
 *	Draw_bez(display,window,pixmap,gc,ctrl_pts,num_pts,num_steps)
 *	where "display" is the X11 display;
 *		  "window" is a drawable;
 *		  "pixmap" is a drawable;
 *        "gc" is a graphical context (GC)
 *	      "ctrl_pts" is an array of pointers each pointing to 
 *		an array of control points in the form ctrl_pts[i,j] 
 *		with i the i'th point and j the 
 *		j'th coordinate (of two) in VDC units;
 *	      "num_pts" is the number of control points.
 *	      "num_steps" is the number of steps to take in drawing the curve.
 *	
 *	Coded in C by Ron Record (sco!rr) Jan. 4, 1988
 */

#include <X11/Xlib.h>

/* 
 * C(n,i) is the binomial coefficient, C(n,i) = n!/(i!(n-i)!).
 */

static double mn[2];

long
C(n,i)
int n, i;
{
	int j;
	long a;

	a = 1;
	for (j=i+1;j<=n;j++)
		a*=j;
	for (j=2;j<=(n-i);j++)
		a/=j;
	return(a);
}

/*
 * B_blend() is the "blending function" representing the influence that
 * each control point has on the curve at time u
 */

double
B_blend(i,n,u)
int i, n;
double u;
{
	int j;
	double v;

	v = (double)C(n,i);
	for (j=1;j<=i;j++)
		v *= u;
	for (j=1;j<=(n-i);j++)
		v *= (1.0-u);
	return(v);
}

/*
 * Compute the xy coordinates of the curve at time t
 */

double *
Bezier(t,n,p)
double t;
int n;
int *p[];
{
	double b;
	double *xy;
	int i;

	xy = mn;
	xy[0] = xy[1] = 0.0;
	for (i=0;i<=n;i++) {
		b = B_blend(i,n,t);
		xy[0] += ((double)p[i][0]*b);
		xy[1] += ((double)p[i][1]*b);
	}
	return(xy);
}

/*
 * Draw_bez() is called from the application program with arguments an
 * opened display, a drawable, a graphical context, an array of pointers 
 * to integer coordinates specifying the control points, the number of 
 * control points minus one, and the number of steps for each curve.
 */

Draw_bez(dpy,w,p,gc,ctrl_pts,num_pts,num_steps)
Display *dpy;
Window w;
Pixmap p;
GC gc;
int *ctrl_pts[];
int num_pts,num_steps;
{
	int i;
	double u;
	double *coord;
	XPoint points[2];

	for (i=0;i<num_steps;i++) {
		u = (double)i/(double)num_steps;
		coord = Bezier(u,num_pts,ctrl_pts);
		if (i == 0) {
			points[0].x = (int)coord[0];
			points[0].y = (int)coord[1];
			points[1].x = (int)coord[0];
			points[1].y = (int)coord[1];
		}
		else {
			points[0].x = points[1].x;
			points[0].y = points[1].y;
			points[1].x = (int)coord[0];
			points[1].y = (int)coord[1];
		}
		if (p)
			XDrawLines(dpy, p, gc, points, sizeof(points)/sizeof(points[0]),
						CoordModeOrigin);
		XDrawLines(dpy, w, gc, points, sizeof(points)/sizeof(points[0]),
					CoordModeOrigin);
	}
}
