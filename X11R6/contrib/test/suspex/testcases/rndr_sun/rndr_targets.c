/* $XConsortium: rndr_targets.c,v 1.0 93/11/22 12:53:20 rws Exp $ */

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
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include <stdio.h>
 

char test_description[] = "\
MatchRendererTarget Tests:\n\n\
This test generates a text file that records:\n\
1. all supported targets\n\
2. any 24 bits targets\n\
in the form of <drawable depth>/<drawable type>/<visual class>\n\
";

extern FILE *   suspex_open_file();
extern int      suspex_close_file();
       FILE *   fp; 

void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
}
 
print_targets(fp, str, dpy, count, targets)
    FILE *fp;
    char *str;
    Display *dpy;
    int  count;
    PEXRenderingTarget *targets;

{ 
    int i, nitems;
    XVisualInfo *vinfo, template;
    PEXRenderingTarget *t;

    fprintf(fp, "%s\n", str);
    for (i = 0, t = targets; i < count; i++, t++) {
	template.visualid = t->visual->visualid;
	vinfo = XGetVisualInfo(dpy, VisualIDMask, &template, &nitems);
	if (vinfo == NULL)
	    return 0;
	fprintf(fp, "%d/%d/%d\n", t->depth, t->type, vinfo->class);
	XFree(vinfo);
    }
}

void
misc_setup(dpy, window, renderer)
Display         *dpy;
Window          window;
PEXRenderer     renderer;
{
    int status, nitems, i;
    unsigned long	count_return;
    PEXRenderingTarget  *targets;
  
    /* open the output file
     */
    fp = suspex_open_file("rndr_targets", "Match rendering targets");


    /* all supported targets
     */
    status = PEXMatchRenderingTargets(dpy, window, 0, PEXAnyDrawable, NULL,
	10, &count_return, &targets);
    print_targets(fp, "all supported targets", dpy, count_return, targets);
    XFree(targets);

    /* any 24 bits targets
     */
    status = PEXMatchRenderingTargets(dpy, window, 24, PEXAnyDrawable, NULL,
	10, &count_return, &targets);
    print_targets(fp, "any 24 bits targets", dpy, count_return, targets);
    XFree(targets);

    suspex_close_file(fp);
}

void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    file_testproc("rndr_targets", "Match Rendering Targets");
} 
