/* $XConsortium: multiDraw.c,v 1.0 93/11/22 12:50:39 rws Exp $ */

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
#include "line.h"
#include "misc.h"

char test_description[] = "\
Multiple Drawable Tests:\n\n\
A wide blue dotted line in rendered on two drawables.\n\
";

/* utility functions we use */
extern void		find_best_visual();
extern int		get_standard_colormap();
extern void		set_stdcmap_approx();
extern Window		create_window();
extern PEXRenderer	setup_renderer();
extern PEXWorkstation	setup_workstation();

extern char *optarg;            /* for getopt */
extern int optind, opterr;

void			outputCommands();
void			testproc();

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

main (argc, argv)
    int		argc;
    char	*argv[];
{
    Display		*dpy;
    Window		window1,window2;
    XVisualInfo		vis_info;
    XStandardColormap	cmap_info;
    PEXColorApproxEntry capx_info;
    PEXExtensionInfo	*pexinfo;
    int			pexmode = IMM;
    PEXRenderer		renderer1, renderer2;
    PEXWorkstation	workstation1, workstation2;
    PEXStructure	structure;
    int			c;
    unsigned int        x,y,w,h;


    if (!(dpy = XOpenDisplay(NULL))) {
	fprintf(stderr, "Could not open display %s\n", 
		DisplayString(dpy));
    
	exit(1);
    }

    if (PEXInitialize(dpy, &pexinfo, ERROR_LEN, error_string)) {
	fprintf(stderr, "%s\n", error_string);
	exit(2);
    }

    /* process command line options */
    while ((c = getopt(argc,argv,"ipsv")) != EOF) {
        switch(c) {
        case 'i':
            pexmode = IMM;
            break;
        case 'p':
            pexmode = PWS;
            break;
        case 's':
            pexmode = STR;
            break;
        case 'v':
            printf("\n%s\n",test_description);
            break;
	case '?':
	    printf("usage: %s: [-i]\n",argv[0]);
	    printf("-i		immediate mode\n");
            printf("-p          PHIGS workstation mode\n");
            printf("-s          structure rendering mode\n");
            printf("-v              verbose mode (test descriptions)\n");
	    printf("default	immediate mode\n");
	    printf("This test run in immediate mode only\n");
	    exit(1);
	}
    }

    /* Determine the best visual to use */
    find_best_visual(dpy, &vis_info);

    /* Get a standard colormap for the visual */
    if (!get_standard_colormap(dpy, &vis_info, &cmap_info)) {
	if (!create_colormap(dpy, &vis_info, &cmap_info, &capx_info)) {
	    fprintf(stderr, "Fails to create colormap\n");
	    exit(3);
	}
    } else
        set_stdcmap_approx(&vis_info, &cmap_info, &capx_info);

    /* Set up the window */
    window1 = setup_window(dpy, &vis_info, &cmap_info, 50,50,200,200);
    window2 = setup_window(dpy, &vis_info, &cmap_info, 300,50,200,200);

    setup_LUT(dpy, window1, &capx_info);
    setup_LUT(dpy, window2, &capx_info);
    pexinfo = PEXGetExtensionInfo(dpy);

    if (pexmode == IMM) {

    	if (!IMM_MODE_SPT(pexinfo)) {
	    fprintf(stderr, "No immediate mode support\n");
	    exit(4);
    	}
    	renderer1 = setup_renderer(dpy, window1);
    	renderer2 = setup_renderer(dpy, window2);

        PEXBeginRendering(dpy, window2, renderer2);
        outputCommands(dpy, (XID)renderer2, PEXOCRender);
        PEXEndRendering(dpy, renderer2, True);

        PEXBeginRendering(dpy, window1, renderer1);
        outputCommands(dpy, (XID)renderer1, PEXOCRender);
        PEXEndRendering(dpy, renderer1, True);

    } else if (pexmode == PWS) {

        if (!PWS_MODE_SPT(pexinfo)) {
            fprintf(stderr, "No PHIGS workstation mode support\n");
            exit(4);
        }
        workstation1 = setup_workstation(dpy, window1);
        workstation2 = setup_workstation(dpy, window2);
        structure = PEXCreateStructure(dpy);
        outputCommands(dpy, (XID)structure, PEXOCStore);
        PEXPostStructure(dpy, workstation1, structure, 1.0);
        PEXPostStructure(dpy, workstation2, structure, 1.0);

   } else {
        if (!STR_MODE_SPT(pexinfo)) {
            fprintf(stderr, "No structure rendering support\n");
            exit(4);
        }
        renderer1 = setup_renderer(dpy, window1); 
	renderer2 = setup_renderer(dpy, window2); 
	structure = PEXCreateStructure(dpy);
        outputCommands(dpy, (XID)structure, PEXOCStore);

        PEXRenderNetwork(dpy, window1, renderer1, structure);
        PEXRenderNetwork(dpy, window2, renderer2, structure);
  
   }
 
    XFlush(dpy);
    XSync(dpy, 0);
    testproc(dpy,window1,window2,0,0,200,200);

    XCloseDisplay(dpy);
    return 0;
}


void
outputCommands(dpy, resourceID, req_type)
    Display		*dpy;
    XID			resourceID;
    PEXOCRequestType	req_type;
{	
    PEXColor		colr;
    int			i;

    static		PEXCoord pts1[] = { 
	0.1, 0.3, 0.0,
	0.3, 0.2, 0.0,
	0.1, 0.1, 0.0,
    };

    SET_LINE_TYPE(PEXLineTypeDotted)
    SET_LINE_COLOR_INDEX(4);
    SET_LINE_WIDTH(3.0);
    POLYLINE(3, pts1);
}

void
testproc(dpy,window1,window2,x,y,w,h)
    Display		*dpy;
    Window		window1,window2;
    int			x,y,w,h;
{
    extern int		pausebetweenframe;

    image_testproc("multiDraw.1", "multiple drawable", test_description,
        dpy,window1,x,y,w,h);
    image_testproc("multiDraw.2", "multiple drawable", test_description,
        dpy,window2,x,y,w,h);
} 

