/* $XConsortium: nurbSurf.c,v 1.0 93/11/22 12:50:50 rws Exp $ */

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
Nurb Surface Tests:\n\n\
You should see two nurb surfaces trimmed in a different way.\n\
Left:  only the centre portion remains.\n\
Right: a hole is created\n\
These surfaces are red in color, has hollow interior and rendered\n\
with uniform iso parametric curves.\n\
";
static float	uknots[] = {0.0,0.0,0.0,1.0,1.0,1.0};
static float	vknots[] = {0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0};
static PEXCoord	pt_arr1[] = {
		    {0.3,0.2,0.0}, {0.4,0.25,0.1}, {0.5,0.2,0.0},
                    {0.2,0.3,0.1}, {0.3,0.35,0.3}, {0.4,0.3,0.2},
                    {0.3,0.4,0.0}, {0.4,0.45,0.2}, {0.5,0.4,0.0},
                    {0.2,0.5,0.2}, {0.3,0.55,0.0}, {0.4,0.5,0.2}};
static PEXCoord	pt_arr2[] = {
		    {0.7,0.2,0.0}, {0.8,0.25,0.1}, {0.9,0.2,0.0},
                    {0.6,0.3,0.1}, {0.7,0.35,0.3}, {0.8,0.3,0.2},
                    {0.7,0.4,0.0}, {0.8,0.45,0.2}, {0.9,0.4,0.0},
                    {0.6,0.5,0.2}, {0.7,0.55,0.0}, {0.8,0.5,0.2}};
static float	tc_knots[] = {0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0};
static float	tc_knots21[] = {0.0,0.0,1.0,2.0,3.0,4.0,4.0};
static float	tc_knots22[] = {0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0};
static PEXCoord2D	tc_cpts[]  = {
		    {0.35,0.05}, {0.95,0.95}, {0.25,0.95}, {0.35,0.05}};
static PEXCoord2D	tc_cpts21[]  = {
		    {0.0,0.0}, {1.0,0.0}, {1.0,1.0}, {0.0,1.0}, {0.0,0.0}};
static PEXCoord2D	tc_cpts22[]  = {
		    {0.35,0.05}, {0.25,0.95}, {0.95,0.95}, {0.35,0.05}};

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXPSCData		surf_characs;
    PEXArrayOfCoord	points;
    PEXListOfTrimCurve  tl[2];
    PEXTrimCurve	tc[2];
				

    PEXSetSurfaceColorIndex(dpy, resourceID, req_type, 2);
				
    surf_characs.iso_curves.placement_type = PEXUniformPlacement;
    surf_characs.iso_curves.u_count = 6;
    surf_characs.iso_curves.v_count = 9;
    PEXSetParaSurfCharacteristics(dpy, resourceID, req_type, 
				  PEXPSCIsoCurves, &surf_characs);

    /* NURB Surface with 1 trimloop consisting of 1 trimcurve */
    points.point = pt_arr1;
    tc[0].visibility = PEXOn;
    tc[0].order = 4;
    tc[0].rationality = PEXNonRational;
    tc[0].approx_method = PEXApproxConstantBetweenKnots;
    tc[0].tolerance = 10.0;
    tc[0].tmin = 0.0;
    tc[0].tmax = 1.0; 
    tc[0].knots.count = 8;
    tc[0].knots.floats = tc_knots;
    tc[0].count = 4;
    tc[0].control_points.point_2d = tc_cpts;
    tl[0].count = 1;
    tl[0].curves = tc;
    PEXNURBSurface(dpy, resourceID, req_type, PEXNonRational, 
		   3, 4, uknots, vknots, 3, 4, points, 1, tl);

    /* NURB Surface with  2 trimloops consisting of 1 trimcurves each */
    points.point = pt_arr2;
    tc[0].visibility = PEXOn;
    tc[1].visibility = PEXOn;
    tc[0].order = 2;
    tc[1].order = 4;
    tc[0].rationality = PEXNonRational;
    tc[1].rationality = PEXNonRational;
    tc[0].approx_method = PEXApproxConstantBetweenKnots;
    tc[1].approx_method = PEXApproxConstantBetweenKnots;
    tc[0].tolerance = 10.0;
    tc[1].tolerance = 10.0;
    tc[0].tmin = 0.0;
    tc[1].tmin = 0.0;
    tc[0].tmax = 4.0; 
    tc[1].tmax = 1.0; 
    tc[0].knots.count = 7;
    tc[0].knots.floats = tc_knots21;
    tc[1].knots.count = 8;
    tc[1].knots.floats = tc_knots22;
    tc[0].count = 5;
    tc[0].control_points.point_2d = tc_cpts21;
    tc[1].count = 4;
    tc[1].control_points.point_2d = tc_cpts22;
    tl[0].count = 1;
    tl[0].curves = &tc[0];
    tl[1].count = 1;
    tl[1].curves = &tc[1];
    PEXNURBSurface(dpy, resourceID, req_type, PEXNonRational, 
		   3, 4, uknots, vknots, 3, 4, points, 2, tl);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("nurbSurf", "nurb surface", test_description, 
		dpy,window,x,y,w,h);
} 

void
misc_setup(dpy, window, renderer)
Display         *dpy;
Window          window;
PEXRenderer     renderer;
{
}
