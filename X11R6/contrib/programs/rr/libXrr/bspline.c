
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
 *	@(#) bspline.c 9.1 93/06/14 SCOINC
 */
/*
 *  Ported from CGI to X11 20 Apr 1993 by rr@sco.com
 */
/*	X11 library module to allow drawing of curves in the plane
 *	utilizing the non-periodic B-spline method.
 *
 *	Intended to be used as follows :
 *	Draw_spline(dpy,win,pix,gc,ctrl_pts,num_pts,num_steps,ord_cont)
 *	where "dpy" is an open display;
 *        "win" is a drawable
 *        "pix" is a drawable
 *        "gc" is a graphical context (GC)
 *	      "ctrl_pts" is an array of pointers each pointing to 
 *		an array of control points in the form ctrl_pts[i,j] 
 *		with i the i'th point and j the 
 *		j'th coordinate (of two) in VDC units;
 *	      "num_pts" is the number of control points minus one.
 *	      "num_steps" is the number of linear pieces in the curve.
 *	      "ord_cont" is the desired order of continuity of the curve.
 *	
 *	Coded in C by Ron Record (sco!rr) Jan. 8, 1988
 */

#include <X11/Xlib.h>

static double mn[2];
static int K, N;

/*
 * C_ord() calculates the values which are used to control the order of
 * continuity of the curve.
 */

C_ord(i)
int i;
{
	int t;

	if (i < K)
		t = 0;
	else if (i > N)
		t = N - K + 2;
	else
		t = i - K + 1;
	return(t);
}

/*
 * N_blend() is the "blending function" representing the influence that
 * each control point has on the curve at time u
 */

double
N_blend(i,k,u)
int i, k;
double u;
{
	int j;
	double v;

	if (k == 1) {
		v = 0;
		if ((C_ord(i) <= u) && (u < C_ord(i+1)))
			v = 1;
	}
	else {
		v = 0;
		j = C_ord(i+k-1) - C_ord(i);
		if (j != 0)
			v = (u - C_ord(i))*N_blend(i,k-1,u)/j;
		j = C_ord(i+k) - C_ord(i+1);
		if (j != 0)
			v = v + (C_ord(i+k) - u) * N_blend(i+1,k-1,u)/j;
	}
	return(v);
}

/*
 * Compute the xy coordinates of the curve at time t
 */

double *
B_spline(u,n,k,p)
double u;
int n,k;
int *p[];
{
	double b;
	double *xy;
	int i;

	xy = mn;
	K = k; N = n;
	xy[0] = xy[1] = 0.0;
	for (i=0;i<=n;i++) {
		b = N_blend(i,k,u);
		xy[0] += ((double)p[i][0]*b);
		xy[1] += ((double)p[i][1]*b);
	}
	return(xy);
}

/*
 * Draw_spline() is called from the application program with arguments an
 * opened display, a drawable, a graphical context, an array of pointers to 
 * integer coordinates specifying the control points, the number of control 
 * points minus one, the number of steps for each curve, and the desired order 
 * of continuity. The parameter, u, varies over the interval 
 * [0,num_pts - ord_cont + 2).
 */

void
Draw_spline(dpy,w,p,gc,ctrl_pts,num_pts,num_steps,ord_cont)
Display *dpy;
Window w;
Pixmap p;
GC gc;
int *ctrl_pts[];
int num_pts,num_steps,ord_cont;
{
	int i;
	double u;
	double *coord;
	XPoint points[2];

	for (i=0;i<num_steps;i++) {
		u = (double)i/(double)num_steps;
		u *= (double)(num_pts - ord_cont +2);	
		coord = B_spline(u,num_pts,ord_cont,ctrl_pts);
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
