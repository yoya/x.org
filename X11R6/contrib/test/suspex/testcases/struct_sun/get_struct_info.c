/* $XConsortium: get_struct_info.c,v 1.0 93/11/22 12:52:28 rws Exp $ */

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
Get Structure Info Tests:\n\n\
This test generates a text file which contains the inquired structure info\n\
from 3 PEXGetStructureInfo calls.\n\
";

extern FILE *   suspex_open_file();
extern int      suspex_close_file();

#define GET_STRUCTURE_INFO(dpy,str) \
{                                       \
    PEXGetStructureInfo(dpy, str, PEXIEEE_754_32,       \
        PEXElementPtr | PEXNumElements | PEXLengthStructure |   \
                PEXHasRefs | PEXEditMode, &str_info);           \
    fprintf(fp, "str_info: \n");                                     \
    fprintf(fp, "\telement_count: %d\n", str_info.element_count);    \
    fprintf(fp, "\t         size: %d\n", str_info.size);             \
    fprintf(fp, "\t     has_refs: %d\n", str_info.has_refs);         \
    fprintf(fp, "\t    edit_mode: %d\n", str_info.edit_mode);        \
    fprintf(fp, "\telement_pointer: %d\n", str_info.element_pointer);\
}

void
setup(dpy)
    Display             *dpy;
{
    FILE		*fp;
    PEXStructure	str, str2;
    PEXStructureInfo	str_info;

    /* open the output file
     */  
    fp = suspex_open_file("get_struct_info", "Get Structure Info");
 
    /* create a structure
     */
    str = PEXCreateStructure(dpy);
    PEXSetLineType(dpy, str, PEXOCStore, PEXLineTypeDashed);
    PEXSetLineWidth(dpy, str, PEXOCStore, 2.0);
    GET_STRUCTURE_INFO(dpy, str);
 
    PEXSetEditingMode(dpy, str, PEXStructureReplace);
    PEXSetLineWidth(dpy, str, PEXOCStore, 1.0);
    GET_STRUCTURE_INFO(dpy, str);
 
    str2 = PEXCreateStructure(dpy);
    PEXExecuteStructure(dpy, str2, PEXOCStore, str);
    GET_STRUCTURE_INFO(dpy, str);

    PEXDestroyStructures(dpy, 1, &str);

    suspex_close_file(fp);

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
    file_testproc("get_struct_info", "Get Structure information");
}
