/* $XConsortium: copy_nameset.c,v 1.0 93/11/22 12:53:49 rws Exp $ */

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
#include "oc.h"
extern FILE *   suspex_open_file();
extern int      suspex_close_file();

char test_description[] = "\
Copy Nameset Tests:\n\n\
This test generates a text output file which contains a dump of contents of\n\
the source and the destination nameset\n\
";


void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    unsigned long	resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;
    XID			id1, id2;
    PEXName		names[2];
    unsigned long	count;
    PEXName		*pname ;
    int			i;
    FILE		*fp;

    id1 = PEXCreateNameSet(dpy);
    id2 = PEXCreateNameSet(dpy);
    names[0] = 3;
    names[1] = 7;
    PEXChangeNameSet(dpy, id1, PEXNSAdd, 2, names);
    PEXCopyNameSet(dpy, id1, id2);
    PEXGetNameSet(dpy, id2, &count, &pname);
    fp = suspex_open_file("copy_nameset","Copy Nameset");
    fprintf(fp, "Contents of source Namset:       ( 3 7 )\n");
    fprintf(fp, "Contents of destination Nameset: ( ");
    for (i=0; i<count; i++, pname++) {
	fprintf(fp, "%d ", *pname);
    }
    fprintf(fp, ")\n");
    suspex_close_file(fp);
    PEXFreeNameSet(dpy, id1);
    PEXFreeNameSet(dpy, id2);
}
 
void
testproc(dpy,window,x,y,w,h)
    Display		*dpy;
    Window		window;
    int			x,y,w,h;
{
    file_testproc("copy_nameset", "Copy Nameset");
} 

void
misc_setup(dpy, window, renderer)
Display		*dpy;
Window		window;
PEXRenderer	renderer;
{
}
