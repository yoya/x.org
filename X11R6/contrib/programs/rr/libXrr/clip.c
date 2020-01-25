
/*************************************************************************
 *                                                                       *
 *  Copyright (c) 1993 Ronald Joe Record                           *
 *                                                                       *
 *      Ronald Joe Record (408) 458-3718                                 *
 *      212 Owen St., Santa Cruz, California 95062 USA                   *
 *                                                                       *
 *************************************************************************/
/*
 *	@(#) clip.c 9.1 93/06/14 SCOINC
 *
 * Written by rr@sco.com 25-May-1993
 * - performs clipping of line segments wrt a window in 2-D
 */
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
 * The Cohen-Sutherland clipping algorithm from pages 146-149 of the book
 * "Fundamentals of Interactive Computer Graphics" by Foley and Van Dam.
 * This algorithm is especially efficient when it is usually the case that
 * the entire line segment to be drawn lies either inside or outside
 * of the window (trivial acceptance or rejection). The algorithm relies on
 * assigning each endpoint a four-bit code as follows :
 *
 *    1001 | 1000 | 1010
 *   ------|------|------
 *    0001 | 0000 | 0010
 *   ------|------|------
 *    0101 | 0100 | 0110
 *
 * with the center square above representing the window. I have altered the
 * algorithm slightly to test for trivial acceptance first, followed by a
 * check for trivial rejection (my most common case is trivial acceptance).
 *
 * The clipping routine is called clipline() and is called as follows :
 * 
 * clipline(points, xmin, xmax, ymin, ymax);
 * where points is an XSegment containing the two endpoints of the segment
 * to be drawn, xmin and ymin represent the window minimums and xmax and ymax
 * the window maximums. The clipline() routine returns an XSegment containing
 * the endpoints of the clipped segment.
 */

#include <X11/Xlib.h>

reject_check(a, b)
int *a;
int *b;
{
	static int i;

	for (i=0;i<4;i++)
		if (a[i] && b[i]) /*both endpts above, below, left, or right of window*/
			return(1);
	return(0);
}

accept_check(a, b)
int *a;
int *b;
{
	static int i;

	for (i=0; i<4; i++)
		if (a[i] || b[i]) /* at least one endpoint not in window */
			return(0);
	return(1);
}

outcodes(x, y, xmin, xmax, ymin, ymax, out)
int x, y, xmin, xmax, ymin, ymax;
int *out;
{
	out[0] = out[1] = out[2] = out[3] = 0;
	if (y > ymax)
		out[0] = 1;
	if (y < ymin)
		out[1] = 1;
	if (x > xmax)
		out[2] = 1;
	if (x < xmin)
		out[3] = 1;
}

XSegment
clipline(pts,xmin,xmax,ymin,ymax)
XSegment pts;
int xmin, xmax, ymin, ymax;
{
	int outcode1[4], outcode2[4];
	int accept, reject, done, tmp;

	accept = reject = done = 0;

	for(;;) {
		outcodes(pts.x1, pts.y1, xmin, xmax, ymin, ymax, outcode1);
		outcodes(pts.x2, pts.y2, xmin, xmax, ymin, ymax, outcode2);
		accept = accept_check(outcode1, outcode2); /*check trivial accept*/
		if (accept) done = 1;
		else {	/* first check trivial reject */
			reject = reject_check(outcode1, outcode2); /*check trivial reject*/
			if (reject) done = 1;
			else { /* at most one endpoint is inside */
				/* first, if P1 is inside window, exchange points 1 and 2 */
				/* and their outcodes to guarantee that P1 is outside window */
				if(!(outcode1[0] || outcode1[1] || outcode1[2] || outcode1[3])){
					tmp = pts.x1; pts.x1 = pts.x2; pts.x2 = tmp;
					tmp = pts.y1; pts.y1 = pts.y2; pts.y2 = tmp;
				}
				/* move P1 to  the intersection point */
				if (outcode1[0]) {
					pts.x1=pts.x1+(pts.x2-pts.x1)*(ymax-pts.y1)/(pts.y2-pts.y1);
					pts.y1 = ymax;
				}
				else if (outcode1[1]) {
					pts.x1=pts.x1+(pts.x2-pts.x1)*(ymin-pts.y1)/(pts.y2-pts.y1);
					pts.y1 = ymin;
				}
				else if (outcode1[2]) {
					pts.y1=pts.y1+(pts.y2-pts.y1)*(xmax-pts.x1)/(pts.x2-pts.x1);
					pts.x1 = xmax;
				}
				else if (outcode1[3]) {
					pts.y1=pts.y1+(pts.y2-pts.y1)*(xmin-pts.x1)/(pts.x2-pts.x1);
					pts.x1 = xmin;
				}
			}
		}
		if (done)
			break;
	}
	return(pts);
}
