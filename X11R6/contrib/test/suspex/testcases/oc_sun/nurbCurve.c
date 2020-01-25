/* $XConsortium: nurbCurve.c,v 1.0 93/11/22 12:50:44 rws Exp $ */

/*
	NOTICE TO USER: The source code in this file is copyrighted under
	U.S. and international laws.  SunSoft, Inc., a Sun Microsystems,
	Inc. business of Mountain View, California owns the copyright. 
	Users and possessors of this source code are hereby granted a
	nonexclusive, royalty-free copyright and design patent license
	to use this code in individual and commercial software. 

	Any use of this source code must include, in the user documentation 
	and internal comments to the code, notices to the end user as follows:

(c) Copyright 1992, 1993 SunSoft, Inc., a Sun Microsystems, Inc. business.

	SUNSOFT, INC. AND SUN MICROSYSTEMS, INC. MAKE NO REPRESENTATIONS
	ABOUT THE SUITABILITY OF THIS SOURCE CODE FOR ANY PURPOSE. IT IS
	PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND. 
	SUNSOFT, INC., AND SUN MICROSYSTEMS, INC., SEVERALLY AND
	INDIVIDUALLY, DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOURCE
	CODE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
	FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUNSOFT, INC.
	OR SUN MICROSYSTEMS, INC., BE LIABLE FOR ANY SPECIAL, INDIRECT,
	INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER
	RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
	OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.
*/

#include <stdio.h>
#include <X11/PEX5/PEXlib.h>

char test_description[] = "\
Nurb Curve Tests:\n\n\
You should see a simple nurb curve on the lower left corner.\n\
";

static float	knots3[] = {0.0,0.0, 0.0,0.1, 0.1,0.1};
static PEXCoord	pts_arr[] = {{0.1,0.1,0.0}, 
			     {0.2,0.2,0.0}, 
			     {0.3,0.1,0.0}};
void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	

    PEXArrayOfCoord	cpts;
    

    PEXSetCurveApprox(dpy, resourceID, req_type, 
		      PEXApproxConstantBetweenKnots, 5.0);
   
    /* NURB Curve order 3 */
    cpts.point = pts_arr;
    PEXNURBCurve(dpy, resourceID, req_type, PEXNonRational, 3, knots3,
		 3, cpts, 0.0, 0.1);

}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("nurbCurve", "nurb curve", test_description, 
		dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display         *dpy;
Window          window;
PEXRenderer     renderer;
{
}
