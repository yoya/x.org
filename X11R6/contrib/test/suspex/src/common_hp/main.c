/* $XConsortium: main.c,v 1.0 93/11/22 12:22:05 rws Exp $ */

/******************************************************************************/
/*                                                                            */
/*  (c) Copyright Hewlett-Packard Company, 1993, Fort Collins, Colorado       */
/*                                                                            */
/*                            All Rights Reserved                             */
/*                                                                            */
/*  Permission to use, copy, modify, and distribute this software and its     */
/*  documentation for any purpose and without fee is hereby granted,          */
/*  provided that the above copyright notices appear in all copies and that   */
/*  both the copyright notices and this permission notice appear in           */
/*  supporting documentation, and that the name of Hewlett-Packard not be     */
/*  used in advertising or publicity pertaining to distribution of the        */
/*  software without specific, written prior permission.                      */
/*                                                                            */
/*  HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THIS         */
/*  SOFTWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Hewlett-Packard    */
/*  shall not be liable for errors contained herein or direct, indirect,      */
/*  special, incidental or consequential damages in connection with the       */
/*  furnishing, performance or use of this software.                          */
/*                                                                            */
/******************************************************************************/

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>
#include <stdio.h>
#include <stdlib.h>

#include <misc.h>
#include <common_hp.h>

void run_test_imm(Display *dpy, Window window, 
		  int supported_modes, int num_images);
void run_test_noc(Display *dpy, Window window, 
		  int supported_modes, int num_images);
void run_test_str(Display *dpy, Window window, 
		  int supported_modes, int num_images);
void run_test_wks(Display *dpy, Window window, 
		  int supported_modes, int num_images);

/* utility functions we use */
/*
extern Window		create_window();
extern PEXRenderer	setup_renderer();
extern PEXWorkstation	setup_workstation();
extern void		execute_test();
extern void		misc_setup();
*/

#ifdef XSYSTEM
include <XSystemTest.h>
XSystemTestingControlStructure xtc;
int values[20];
#endif


#define STRING_LEN	80
char			error_string[STRING_LEN];
char			win_title[STRING_LEN];
char                    test_name[STRING_LEN];

int image_depth = 0;

#define IMM_MODE_SPT(info) \
	(((info)->subset_info & 0xFFFF) == PEXCompleteImplementation \
		|| (info)->subset_info & PEXImmediateMode)

#define WKS_MODE_SPT(info) \
	(((info)->subset_info & 0xFFFF) == PEXCompleteImplementation \
		|| (info)->subset_info & PEXWorkstationOnly)

#define STR_MODE_SPT(info) \
	(((info)->subset_info & 0xFFFF) == PEXCompleteImplementation \
		|| (info)->subset_info & PEXStructureMode)

main (int argc, char **argv)
{
    Display		*dpy;
    Window		window;
    XVisualInfo		vis_info;
    XStandardColormap	cmap_info;
    PEXColorApproxEntry capx_info;
    PEXExtensionInfo	*pexinfo;
    int			pexmode = 0;
    PEXRenderer		renderer;
    PEXWorkstation	workstation;
    PEXStructure	structure;
    int                 num_images;
    int                 supported_modes;


#ifdef XSYSTEM
    openXSystemTesting(argc, argv, &xtc, 0, 0);
    dpy = xtc.display;

#else
    if (!(dpy = XOpenDisplay(NULL))) {
	fprintf(stderr, "Could not open display\n");
	exit(1);
    }
#endif

    if (PEXInitialize(dpy, &pexinfo, STRING_LEN, error_string)) {
	fprintf(stderr, "%s\n", error_string);
	exit(2);
    }

    /* Set up test error handler */
    _hppex_test_init_error_handler(); 

    inquire_test_params(&test_name, &num_images, &supported_modes, &win_title);

    /*  Handle window geometry string if present */
    parse_window_geometry(argc, argv);

#ifndef XSYSTEM
    /* Handle other arguments. */
    if (parse_options (argc, argv, &pexmode, &image_depth))
	exit (1);

    window = create_and_setup_window(dpy, 
				     win_title, 
				     image_depth,
				     glob_window_x, 
				     glob_window_y,
				     glob_window_width,
				     glob_window_height, 
				     &capx_info);

    setup_LUT(dpy, window, &capx_info);

    /*  If no test mode is explicitly selected, then set to IMM_MODE if */
    /*  if it is supported, otherwise NON_OC_MODE.                      */
    /*  (All tests should support one or the other)                     */

    if (pexmode == 0) {
	if (supported_modes & IMM_MODE) {
	    pexmode = IMM_MODE;
	} else {
	    pexmode = NON_OC_MODE;
	}
    }

    pexinfo = PEXGetExtensionInfo(dpy);

    if (pexmode & IMM_MODE) {

    	if (!IMM_MODE_SPT(pexinfo)) {
	    fprintf(stderr, "No immediate mode support\n");
	    exit(4);
    	}
	run_test_imm(dpy, window, supported_modes, num_images);
    } 
    if (pexmode & WKS_MODE) {

    	if (!WKS_MODE_SPT(pexinfo)) {
	    fprintf(stderr, "No PHIGS workstation mode support\n");
	    exit(4);
    	}
	run_test_wks(dpy, window, supported_modes, num_images);
    } 
    if (pexmode & STR_MODE) {
    	if (!STR_MODE_SPT(pexinfo)) {
	    fprintf(stderr, "No structure rendering support\n");
	    exit(4);
    	}
	run_test_str(dpy, window, supported_modes, num_images);
    }
    if (pexmode & NON_OC_MODE) {
	run_test_noc(dpy, window, supported_modes, num_images);
    } 
#else  
    /* XSystem Mode                                            */
    /* If both immediate and structure modes are supported for */
    /* an XSystem test, then implement 2-pass with pass 1      */
    /* immediate and pass 2 structure mode.                    */

    window = create_and_setup_window(dpy, 
				     win_title, 
				     image_depth,
				     glob_window_x, 
				     glob_window_y,
				     glob_window_width,
				     glob_window_height, 
				     &capx_info);
    setup_LUT(dpy, window, &capx_info);

    if ((supported_modes & (IMM_MODE | STR_MODE)) == (IMM_MODE | STR_MODE)) {
        run_test_imm(dpy, window, supported_modes, num_images);

	/*  Reset archive file frame number to 1 because imm & str */
	/*  modes should have identical archives                   */
        values[0] = 1;
        defineXSystemTestControlParm(&xtc, RESET_FRAME_NUMBER, values);

        run_test_str(dpy, window, supported_modes, num_images);
        closeXSystemTesting(&xtc);
    }
    else {
        if (supported_modes & IMM_MODE) {
    	    if (!IMM_MODE_SPT(pexinfo)) {
	        fprintf(stderr, "No immediate mode support\n");
	        exit(4);
    	    }
	    run_test_imm(dpy, window, supported_modes, num_images);
        } 
        if (supported_modes & WKS_MODE) {
    	    if (!WKS_MODE_SPT(pexinfo)) {
	        fprintf(stderr, "No PHIGS workstation mode support\n");
	        exit(4);
    	    }
	    run_test_wks(dpy, window, supported_modes, num_images);
        } 
        if (supported_modes & STR_MODE) {
    	    if (!STR_MODE_SPT(pexinfo)) {
	        fprintf(stderr, "No structure rendering support\n");
	        exit(4);
    	    }
	    run_test_str(dpy, window, supported_modes, num_images);
        } 
        if (supported_modes & NON_OC_MODE) {
	    run_test_noc(dpy, window, supported_modes, num_images);
        } 
        closeXSystemTesting(&xtc);
    }
#endif

    XCloseDisplay(dpy);
    return 0;
}


/*
 *  Support function to run a single test in immediate mode.
 */
void run_test_imm(Display *dpy, 
		  Window   window, 
		  int      supported_modes,
		  int      num_images)
{
    int i;
    PEXRenderer renderer;
  
    if (!(supported_modes & IMM_MODE)) {
        fprintf(stderr, "%s Does NOT support Immediate Mode\n", test_name);
	return;
    }

    glob_test_mode = IMM_MODE;
    renderer = setup_renderer(dpy, window);

    for (i=1; i<=num_images; i++) {
	img_desc[0] = '\0';
        misc_setup(dpy, window, renderer, i);
        PEXBeginRendering(dpy, window, renderer);
        execute_test(dpy, (XID)renderer, PEXOCRender, i);
        PEXEndRendering(dpy, renderer, True);
        XFlush(dpy);
        XSync(dpy, 0);
        testproc(dpy, window, i);
    }
}

/*
 *  Support function to run a single test in non-oc mode.
 */
void run_test_noc(Display *dpy, 
		  Window   window, 
		  int      supported_modes,
		  int      num_images)
{
    int i;
    PEXRenderer renderer;
  
    if (!(supported_modes & NON_OC_MODE)) {
        fprintf(stderr, "%s Does NOT support NON-OC Mode\n", test_name);
	return;
    }

    glob_test_mode = NON_OC_MODE;
    renderer = setup_renderer(dpy, window);

    for (i=1; i<=num_images; i++) {
	img_desc[0] = '\0';
        misc_setup(dpy, window, renderer, i);
        execute_test(dpy, (XID)renderer, PEXOCRender, i);
        XFlush(dpy);
        XSync(dpy, 0);
        testproc(dpy,window,i);
    }
}

/*
 *  Support function to run a single test in structure mode.
 */
void run_test_str(Display *dpy, 
		  Window   window, 
		  int      supported_modes,
		  int      num_images)
{
    int i;
    PEXRenderer  renderer;
    PEXStructure structure;

    if (!(supported_modes & STR_MODE)) {
        fprintf(stderr, "%s Does NOT support Structure Mode\n", test_name);
	return;
    }

    glob_test_mode = STR_MODE;
    renderer = setup_renderer(dpy, window);

    for (i=1; i<=num_images; i++) {
	img_desc[0] = '\0';
	misc_setup(dpy, window, renderer, i);
    	structure = PEXCreateStructure(dpy);
        execute_test(dpy, (XID)structure, PEXOCStore, i);
	PEXRenderNetwork(dpy, window, renderer, structure);
        XFlush(dpy);
        XSync(dpy, 0);
        testproc(dpy,window,i);
    }
}

/*
 *  Support function to run a single test in workstation mode.
 */
void run_test_wks(Display *dpy, 
		  Window   window, 
		  int      supported_modes,
		  int      num_images)
{
    int i;
    PEXWorkstation workstation;
    PEXStructure   structure;

    if (!(supported_modes & WKS_MODE)) {
        fprintf(stderr, "%s Does NOT support Workstation Mode\n", test_name);
	return;
    }

    glob_test_mode = WKS_MODE;

    for (i=1; i<=num_images; i++) {
	workstation = setup_workstation(dpy, window);
	img_desc[0] = '\0';
	misc_setup(dpy, window, workstation, i);
	structure = PEXCreateStructure(dpy);
	execute_test(dpy, (XID)structure, PEXOCStore);
	PEXPostStructure(dpy, workstation, structure, 1.0);
        XFlush(dpy);
        XSync(dpy, 0);
        testproc(dpy,window,i);
    }
}

#ifdef XSYSTEM
/*
 *  standard_testproc() entrypoint for XSystem tests
 *  just call XSystemCheck
 */
void standard_testproc(char    *testname, 
		       char    *testdesc,
		       Display *dpy,
		       Window   window,
		       int      x,y,w,h)
    {
    XSync(dpy, 0);
    XSystemCheck("", &xtc);
    }

/*
 *  file_testproc is a NOP for XSystem because stderr
 *  is always saved even if XSystemCheck is not invoked
 */
void file_testproc(char    *testname, 
		   char    *testdesc)
    {
    }
#endif
