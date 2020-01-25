/* $XConsortium: element_search.c,v 1.0 93/11/22 12:52:11 rws Exp $ */

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
Element Search Tests:\n\n\
This test generates a text output file that contains status and \n\
element offset informations returned from Element Search calls.\n\
";

PEXCoord 	line_pts[4] = {
	{0.1, 0.1, 0.0},
	{0.2, 0.1, 0.0},
	{0.3, 0.2, 0.0},
	{0.4, 0.2, 0.0}};

PEXVector vector = {0.5, 0.5, 0.0};

PEXMatrix transform;
PEXStructure line, object, nothing;
extern FILE *   suspex_open_file();
extern int      suspex_close_file();

void
setup(dpy)
    Display             *dpy;
{
    /* create the line structure
     */
    line = PEXCreateStructure(dpy);
    PEXSetLineWidth(dpy, line, PEXOCStore, 0.5);
    PEXPolyline(dpy, line, PEXOCStore, 4, line_pts);

    object = PEXCreateStructure(dpy);
    PEXExecuteStructure(dpy, object, PEXOCStore, object);
    PEXTranslate(&vector, transform);
    PEXSetLocalTransform(dpy, object, PEXOCStore, PEXReplace, transform);
    PEXExecuteStructure(dpy, object, PEXOCStore, object);
}

void
outputCommands(dpy, resourceID, req_type)
    Display             *dpy;
    XID                 resourceID;
    PEXOCRequestType    req_type;
{
}

void
testproc(dpy,window,x,y,w,h)
    Display             *dpy;
    Window              window;
    int                 x,y,w,h;
{
    unsigned short	incl_list[10], excl_list[10];
    Status		status;
    unsigned long	elem_offset;
    FILE                *fp;
    static char *status_string[3] = {
        "",
        "PEXNotFound",
        "PEXFound",
    };


    fp = suspex_open_file("element_search","Element Search");
    incl_list[0] = PEXOCAll;
    status = PEXElementSearch(dpy, object, PEXBeginning, 0,
	PEXForward, 1, incl_list, 0, excl_list, &elem_offset);
    fprintf(fp,"incl_list- ALL, excl_list=Nil, status=%s offset=%d\n", 
	status_string[(int)status], elem_offset);

    excl_list[0] = PEXOCExecuteStructure;
    status = PEXElementSearch(dpy, object, PEXBeginning, 0,
	PEXForward, 1, incl_list, 1, excl_list, &elem_offset);
   fprintf(fp, "incl_list- ALL, excl_list=ExecuteStructure, status=%s offset=%d\n", status_string[(int)status], elem_offset);

    excl_list[1] = PEXOCLocalTransform;
    status = PEXElementSearch(dpy, object, PEXBeginning, 0,
	PEXForward, 1, incl_list, 2, excl_list, &elem_offset);

    fprintf(fp,"incl_list- ALL, excl_list=ExecuteStructure, LocalTransform, status=%s offset=%d\n", status_string[(int)status], elem_offset);

    incl_list[0] = PEXOCPolyline;
    status = PEXElementSearch(dpy, line, PEXBeginning, 0,
	PEXForward, 1, incl_list, 2, excl_list, &elem_offset);

    fprintf(fp,"incl_list- Polyline, excl_list=ExecuteStructure, LocalTransform, status=%s offset=%d\n", status_string[(int)status], elem_offset);
    suspex_close_file(fp);

    file_testproc("element_search", "Element Search");
}
