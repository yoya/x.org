/* $XConsortium: struct_hier.c,v 1.0 93/11/22 12:52:46 rws Exp $ */

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
This test generates a text file which contains structure paths showing\n\
the inquired descendants and ancestors information.\n\
";

PEXCoord 	line_pts[4] = {
	{0.1, 0.1, 0.0},
	{0.2, 0.1, 0.0},
	{0.3, 0.2, 0.0},
	{0.4, 0.2, 0.0}};

PEXVector vector = {0.5, 0.5, 0.0};

PEXMatrix transform;
PEXStructure line, object, nothing, picture;

extern FILE *   suspex_open_file();
extern int      suspex_close_file();

typedef enum {
    NOTHING,
    LINE,
    OBJECT,
    PICTURE,
    FOREIGN
} structure;
    
/* because structure ids can be different for each display
   connection, so make sure we print a constant value for
   each structure for file comparision
 */

PEXStructure sid[5];

static int
find_sid(structid)
    PEXStructure structid;
{
    int i;

    for (i = 0; i < 5; i++) {
	if (structid == sid[i])
	    return (i);
    }
    return(-1);
}

static int
print_path(fp, string, numPath, path)
    FILE		*fp;
    char 		*string;
    unsigned long	numPath;
    PEXStructurePath	*path;
{
    int i, j;
    PEXElementRef	*el;

    fprintf(fp, "%s\n", string);
    fprintf(fp, "numPaths= %d\n", numPath);
    for (i = 0; i < numPath; i++, path++) {
	el = path->elements;
        fprintf(fp, "num elements= %d\n", path->count);
	for (j = 0; j < path->count; j++, el++) 
	    fprintf(fp, "  [%x %d]\n", 
			find_sid(el->structure), el->offset);
    }
}
    
static int
print_struct_id(fp, string, numStructs, str)
    FILE		*fp;
    char 		*string;
    unsigned long	numStructs;
    PEXStructure    	*str;
{
    int i;
    fprintf(fp, "%s\n", string);
    fprintf(fp, "numStruct= %d\n", numStructs);
    for (i = 0; i < numStructs; i++, str++) {
	fprintf(fp, "  %x\n", find_sid(*str));
    }
}

void
setup(dpy)
    Display             *dpy;
{
    FILE	*fp;
    unsigned long	numPath, numStructs;
    PEXStructurePath	*path;
    PEXStructure	*structs;

    /* create the line structure
     */
    sid[LINE] = PEXCreateStructure(dpy);
    PEXPolyline(dpy, sid[LINE], PEXOCStore, 4, line_pts);

    /* create the nothing structure
     */
    sid[NOTHING] = PEXCreateStructure(dpy);

    /* create the foreign structure that executes line 
     */
    sid[FOREIGN] = PEXCreateStructure(dpy);
    PEXExecuteStructure(dpy, sid[FOREIGN], PEXOCStore, sid[LINE]);

    /* object executes line
     */  
    sid[OBJECT] = PEXCreateStructure(dpy);
    PEXSetLineWidth(dpy, sid[OBJECT], PEXOCStore, 4.0);
    PEXExecuteStructure(dpy, sid[OBJECT], PEXOCStore, sid[LINE]);
    PEXSetLineType(dpy, sid[OBJECT], PEXOCStore, PEXLineTypeDashed);
    PEXExecuteStructure(dpy, sid[OBJECT], PEXOCStore, sid[LINE]);
 
    /* picture executes object and line
     */  
    sid[PICTURE] = PEXCreateStructure(dpy);
    PEXExecuteStructure(dpy, sid[PICTURE], PEXOCStore, sid[OBJECT]);
    PEXTranslate(&vector, transform);
    PEXSetLocalTransform(dpy, sid[PICTURE], PEXOCStore, PEXReplace, transform);
    PEXExecuteStructure(dpy, sid[PICTURE], PEXOCStore, sid[LINE]);
 
 
    /* open the output file
     */  
    fp = suspex_open_file("struct_hier", "structure hierarchy");
 
    /* inquire descendents, return all path
     */  
    path = PEXGetDescendants(dpy, sid[PICTURE], PEXTopPart, 0, &numPath);
    print_path(fp, "descentants from picture - PEXTopPart, depth = 0",
                numPath, path);
    PEXFreeStructurePaths(numPath, path);

    /* inquire descendents, return 1 element deep
     */  
    path = PEXGetDescendants(dpy, sid[PICTURE], PEXTopPart, 1, &numPath);
    print_path(fp, "descentants from picture - PEXTopPart, depth = 1",
                numPath, path);
    PEXFreeStructurePaths(numPath, path);

    /* inquire descendents, bottom first, return 1 element deep
     */  
    path = PEXGetDescendants(dpy, sid[PICTURE], PEXBottomPart, 1, &numPath);
    print_path(fp, "descentants from picture - PEXBottomPart, depth = 1",
                numPath, path);
    PEXFreeStructurePaths(numPath, path);

    /* inquire ancestors of nothing, top first, return full path
     */
    path = PEXGetAncestors(dpy, sid[NOTHING], PEXTopPart, 0, &numPath);
    print_path(fp, "ancestors of nothing - PEXTopPart, depth = 0",
                numPath, path);
    PEXFreeStructurePaths(numPath, path);

    /* inquire ancestors of line, top first, return full path
     */
    path = PEXGetAncestors(dpy, sid[LINE], PEXTopPart, 0, &numPath);
    print_path(fp, "ancestors of line - PEXTopPart, depth = 0",
                numPath, path);
    PEXFreeStructurePaths(numPath, path);

    /* inquire ancestors of line, top first, return 1 element deep
     */
    path = PEXGetAncestors(dpy, sid[LINE], PEXTopPart, 1, &numPath);
    print_path(fp, "ancestors of line - PEXTopPart, depth = 1",
                numPath, path);
    PEXFreeStructurePaths(numPath, path);

    /* inquire ancestors of line, bottom first, return 2 elements deep
     */
    path = PEXGetAncestors(dpy, sid[LINE], PEXBottomPart, 2, &numPath);
    print_path(fp, "ancestors of line - PEXBottomPart, depth = 2",
                numPath, path);
    PEXFreeStructurePaths(numPath, path);

    structs = PEXGetStructuresInNetwork(dpy, sid[PICTURE], PEXAll, 
				&numStructs);
    print_struct_id(fp, "all structures reference of picture",
	numStructs, structs);
    XFree(structs);

    structs = PEXGetStructuresInNetwork(dpy, sid[PICTURE], PEXOrphans, 
				&numStructs);
    print_struct_id(fp, "orphan structures reference of picture",
	numStructs, structs);
    XFree(structs);

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
    file_testproc("struct_hier", "structure hierarchy information");
}
