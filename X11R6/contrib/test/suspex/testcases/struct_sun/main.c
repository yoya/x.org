/* $XConsortium: main.c,v 1.0 93/11/22 12:52:34 rws Exp $ */

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


/* utility functions we use */
extern void		find_best_visual();
extern int		get_standard_colormap();
extern void		set_stdcmap_approx();
extern Window		create_window();
extern PEXRenderer	setup_renderer();

extern void		setup();
extern void		outputCommands();
extern void		testproc();

extern char *optarg;            /* for getopt */
extern int optind, opterr;
extern char test_description[];

#define ERROR_LEN	80
char			error_string[ERROR_LEN];

#define STR_MODE_SPT(info) \
	(((info)->subset_info & 0xFFFF) == PEXCompleteImplementation \
		|| (info)->subset_info & PEXStructureMode)

PEXRenderer		renderer;

main (argc, argv)
    int		argc;
    char	*argv[];
{
    Display		*dpy;
    Window		window;
    XVisualInfo		vis_info;
    XStandardColormap	cmap_info;
    PEXColorApproxEntry capx_info;
    PEXExtensionInfo	*pexinfo;
    PEXWorkstation	workstation;
    PEXStructure	structure;
    int			c;
    int                 x = 50;
    int                 y = 50;
    int                 w = 200;
    int                 h = 200;


    if (!(dpy = XOpenDisplay(NULL))) {
	fprintf(stderr, "Could not open display\n");
	exit(1);
    }

    if (PEXInitialize(dpy, &pexinfo, ERROR_LEN, error_string)) {
	fprintf(stderr, "%s\n", error_string);
	exit(2);
    }

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

    /*  Handle window geometry string if present */
    {
    int          parse_status;
    int          window_x, window_y;
    unsigned int window_width, window_height;
    int          i;

    for (i=1; i<argc; i++)
        {
        if (argv[i][0] == '=')
            {
            parse_status = XParseGeometry(argv[i], &window_x, &window_y,
                                          &window_width, &window_height);
            if (parse_status & XValue)
                x = window_x;

            if (parse_status & YValue)
                y = window_y;

            if (parse_status & WidthValue)
                w = window_width;

            if (parse_status & HeightValue)
                h = window_height;
            }
        }    
    }    


    /* Set up the window */
    window = setup_window(dpy, &vis_info, &cmap_info, x,y,w,h);

    setup_LUT(dpy, window, &capx_info);
    pexinfo = PEXGetExtensionInfo(dpy);

    if (!STR_MODE_SPT(pexinfo)) {
	    fprintf(stderr, "No structure rendering support\n");
	    exit(4);
    }
    renderer = setup_renderer(dpy, window);
    structure = PEXCreateStructure(dpy);
    setup(dpy);
    outputCommands(dpy, (XID)structure, PEXOCStore);
    PEXRenderNetwork(dpy, window, renderer, structure);

    XFlush(dpy);
    XSync(dpy, 0);
    testproc(dpy,window,0,0,w,h);

    XCloseDisplay(dpy);
    return 0;
}


