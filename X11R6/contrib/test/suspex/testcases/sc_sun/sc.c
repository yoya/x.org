/* $XConsortium: sc.c,v 1.0 93/11/22 12:48:25 rws Exp $ */

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
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>
#include <stdio.h>

char test_description[] = "\
Seach Context Tests:\n\n\
This test reports failures when Get search Context returns info\n\
that does not match the expected.\n\
This test does not generate output files.\n\
";

/* utility functions we use */

extern void		setup();
extern void		testproc();
extern FILE *   suspex_open_file();
extern int      suspex_close_file();

extern char *optarg;            /* for getopt */
extern int optind, opterr;

#define IMM 0
#define PWS 1
#define STR 2

#define ERROR_LEN	80
char			error_string[ERROR_LEN];

#define IMM_MODE_SPT(info) \
	(((info)->subset_info & 0xFFFF) == PEXCompleteImplementation \
		|| (info)->subset_info & PEXImmediateMode)

#define PWS_MODE_SPT(info) \
	(((info)->subset_info & 0xFFFF) == PEXCompleteImplementation \
		|| (info)->subset_info & PEXWorkstationOnly)

#define STR_MODE_SPT(info) \
	(((info)->subset_info & 0xFFFF) == PEXCompleteImplementation \
		|| (info)->subset_info & PEXStructureMode)

PEXRenderer		renderer;

main (argc, argv)
    int		argc;
    char	*argv[];
{
    Display		*dpy;
    PEXExtensionInfo	*pexinfo;
    int			pexmode = IMM;
    PEXWorkstation	workstation;
    PEXStructure	structure;
    int			c;
    unsigned int        x,y,w,h;


    if (!(dpy = XOpenDisplay(NULL))) {
	fprintf(stderr, "Could not open display\n");
	exit(1);
    }

    if (PEXInitialize(dpy, &pexinfo, ERROR_LEN, error_string)) {
	fprintf(stderr, "%s\n", error_string);
	exit(2);
    }

    pexinfo = PEXGetExtensionInfo(dpy);
    /* process command line options */
    while ((c = getopt(argc,argv,"sv")) != EOF) {
        switch(c) {
        case 's':
                break;
        case 'v':
                printf("\n%s\n",test_description);
                break;
        case '?':
                printf("usage: %s: -[sv]\n",argv[0]);
                printf("-v          verbose mode (test descriptions)\n");
                printf("-s          structure mode\n");
                printf("default     structure mode\n");
                exit(1);
        }
    }    

    if (!STR_MODE_SPT(pexinfo)) {
	    fprintf(stderr, "No structure rendering support\n");
	    exit(4);
    }

    XFlush(dpy);
    XSync(dpy, 0);
    testproc(dpy,NULL,0,0,0,0);

    XCloseDisplay(dpy);
    return 0;
}

void
testproc(dpy, window, x, y, w, h)
    Display	*dpy;
    Window              window;
    int                 x,y,w,h;
{
    PEXSearchContext	sc, sc2;
    PEXSCAttributes	values, *ret_values;
    PEXElementRef	struct_path[2];
    PEXNameSetPair	ns;
    FILE                *fp;

    /* create structure path
     */
    struct_path[0].structure = PEXCreateStructure(dpy);
    struct_path[0].offset = 1;
    struct_path[1].structure = PEXCreateStructure(dpy);
    PEXExecuteStructure(dpy, struct_path[0].structure, PEXOCStore, 
				struct_path[1].structure);
    
    /* create normal filter
     */
    ns.inclusion = PEXCreateNameSet(dpy);
    ns.exclusion = PEXCreateNameSet(dpy);
    
    values.position.x = 0.5;
    values.position.y = 0.5;
    values.position.z = 0.5;
    values.distance   = 0.2;
    values.ceiling    = 10;
    values.model_clip_flag = 1;
    values.start_path.count = 2;
    values.start_path.elements = struct_path;
    values.normal.count = 1;
    values.normal.pairs = &ns;

    sc = PEXCreateSearchContext(dpy, 
	PEXSCPosition | PEXSCDistance | PEXSCCeiling | PEXSCModelClipFlag |
	PEXSCStartPath | PEXSCNormalList,
	&values);

    values.distance   = 0.5;
    values.ceiling    = 15;
    sc2 = PEXCreateSearchContext(dpy, 
	PEXSCDistance | PEXSCCeiling | PEXSCModelClipFlag,
	&values);

    PEXCopySearchContext(dpy, 
	PEXSCPosition | PEXSCStartPath | PEXSCNormalList, sc, sc2);

    ret_values = PEXGetSearchContext(dpy, sc2,
	PEXSCPosition | PEXSCDistance | PEXSCCeiling | 
	PEXSCModelClipFlag | PEXSCStartPath | PEXSCNormalList
	);

    fp = suspex_open_file("sc","Search Context");


    if (! ret_values) 
        fprintf(fp, "PEXGetSearchContext failed\n");
    else {
        fprintf(fp, "PEXGetSearchContext - \n");
        fprintf(fp, "position: [%f %f %f]\n",
		ret_values->position.x, ret_values->position.y,
		ret_values->position.z);
	fprintf(fp, "distance: %f\n", ret_values->distance);
	fprintf(fp,"ceiling: %d\n", ret_values->ceiling);
	fprintf(fp, "model clip flag: %d\n",ret_values->model_clip_flag);
	fprintf(fp, "start path count: %d\n",ret_values->start_path.count);
        if (ret_values->start_path.elements[1].structure !=
                struct_path[1].structure) 
            fprintf(fp, "**failed**: unexpected returned start path\n");
	fprintf(fp, "normal list count: %d\n",ret_values->normal.count);
        if (ret_values->normal.pairs->inclusion != ns.inclusion) 
            fprintf(fp, "**failed**: unexpected returned normal list\n");
    }

    suspex_close_file(fp);
    PEXFreeSCAttributes(ret_values);
    PEXFreeSearchContext(dpy, sc);
    PEXFreeSearchContext(dpy, sc2);

    file_testproc("sc", "Search Context");

}
