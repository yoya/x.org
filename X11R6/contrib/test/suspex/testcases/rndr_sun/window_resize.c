/* $XConsortium: window_resize.c,v 1.0 93/11/22 12:54:41 rws Exp $ */

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

#include <X11/PEX5/PEXlib.h>
#include <stdio.h>

char test_description[] = "\
Window Resize Tests:\n\n\
A 200x200 square window is resized to a 400x300 rectangular window.\n\
A square-shaped fillarea is drawn. By default, the renderer automatically \n\
adjusts to the window size changes and maintains the aspect ratio of the \n\
NPC subvolume. The resulting figure is still a square. Next, we set the\n\
renderer viewport's use_drawable attribute to False (auto adjust disabled),\n\
and redraw the fillarea. The square now looks smaller and appears below and\n\
to the left of the previous square.\n\
";

 
PEXCoord square[4] = {
	{0.4, 0.4, 0.0},
	{0.4, 0.6, 0.0},
	{0.6, 0.6, 0.0},
	{0.6, 0.4, 0.0}
};


int new_w, new_h;
int orig_w, orig_h;

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    static int i = 0;
    PEXRendererAttributes rattrs, *rattrsp;
    unsigned long mask;
    PEXViewport vp = { {0, 0, 1.0}, {0, 0, 1.0}, 1, {0, 0, 0} };

    switch (i) {
    case 0: 
        /* 
	 * automatically adust to window size changes by default
         */
        PEXFillArea(dpy, resourceID, req_type, PEXShapeUnknown, 
		True,4, square);
        /* 
	 * Don't automatically adust to window size changes
 	 * Notice that the modified renderer attributes will only
	 * take effect at the next PEXBeginRendering
         */
        mask = PEXRAViewport;
	rattrs.viewport = vp;
	rattrs.viewport.max.x = orig_w;
	rattrs.viewport.max.y = orig_h;
	rattrs.viewport.use_drawable = 0;
        PEXChangeRenderer(dpy, resourceID, mask, &rattrs);
        break;
    case 1: 
        PEXFillArea(dpy, resourceID, req_type, PEXShapeUnknown, 
		True,4, square);
        break;
    default:
	break;
    }
    i++;
}
 

void
misc_setup(dpy, window, renderer)
Display         *dpy;
Window          window;
PEXRenderer     renderer;
{
    extern int 	rendering_loops;
    XWindowAttributes wattrs;

    rendering_loops = 2;
    /*
     * window resize
     */
    XGetWindowAttributes(dpy, window, &wattrs);
    orig_w = wattrs.width;
    orig_h = wattrs.height;
    new_w = orig_w + 200;
    new_h = orig_h + 100;
    XResizeWindow(dpy, window,(unsigned int)new_w,(unsigned int)new_h);
    XFlush(dpy);
}

void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    image_testproc("window_resize", "window resize", 
	test_description, dpy,window,x,y,new_w,new_h);
} 
