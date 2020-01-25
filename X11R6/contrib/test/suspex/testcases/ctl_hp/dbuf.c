/* $XConsortium: dbuf.c,v 1.0 93/11/22 12:26:50 rws Exp $ */

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
#include <misc.h>
#include <lut.h>
#include <pexutdb.h>

/* 
 * Double Buffering Test.  Ensure that E&S or MBX double buffering is
 * supported.  Render to back buffer, switch, repeat.  Render to front
 * buffer using Dfront
 */
draw_image  (Display          *dpy,
	     XID              resourceID,
	     PEXOCRequestType req_type)
{
      PEXVertexNormal			points_1[3], points_2[2];
      PEXVertexNormal		        points_3[1];
      PEXCoord				points_4[4];
      PEXArrayOfVertex			vertex_1, vertex_2, vertex_3;
      PEXArrayOfVertex			vertex_4, dummy;
      PEXFacetData			facet_3, facet_4;
      PEXColor				p_surface_color, p_edge_color;
      PEXTableIndex			lights_on[4];

}



void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
{
    strcpy(test_name, "dbuf");
    *num_images      = 4;
    *supported_modes = IMM_MODE;

    strcpy(win_title, "dbuf");
}

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
{
    unsigned long         mask = PEXRAHLHSRMode;
    PEXRendererAttributes attrs;
    PEXLightEntry	  lights[2];

    attrs.hlhsr_mode = PEXHLHSRZBuffer;
    PEXChangeRenderer(dpy, renderer, mask, &attrs);

    /* Define the ambient light. */
    lights[0].type = PEXLightAmbient;
    lights[0].color.type = PEXColorTypeRGB;
    lights[0].color.value.rgb.red = .3;
    lights[0].color.value.rgb.green = .3;
    lights[0].color.value.rgb.blue = .3;

    /* Define the point light. */
    lights[1].type = PEXLightWCPoint;
    lights[1].point.x = 0.2;
    lights[1].point.y = 0.2;
    lights[1].point.z = 0.5;
    lights[1].attenuation1 = 1.0;
    lights[1].attenuation2 = 1.0;
    lights[1].color.type = PEXColorTypeRGB;
    lights[1].color.value.rgb.red = 1.0;
    lights[1].color.value.rgb.green = 1.0;
    lights[1].color.value.rgb.blue = 1.0;

    PEXSetTableEntries(dpy, lightLUT, 1, 2, PEXLUTLight, lights);
}

void testproc(Display *dpy, Window window, int cur_image)
{
    char image_name[80], test_name[80];

    sprintf(image_name,"dbuf_%d", cur_image);
    sprintf(test_name, "dbuf_#%d", cur_image);
    image_testproc(image_name, test_name, img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
}

void execute_test(Display *dpy, Window window, PEXRenderer renderer, 
	       PEXOCRequestType req_type, int cur_image)
{
    PEXCoord2D coord1, coord2;
    Drawable renderBuf;
    XColor returned_background;
    int i=1;
    int dfront_status,dfront_ok;

    coord1.x = 0.1; 
    coord1.y = 0.1;
    coord2.x = 0.1; 
    coord2.y = 0.5;
    
    if ( PEXUtDBConfig(dpy, window, True, 
	&returned_background, False, &renderBuf ))
    {
	fprintf (stderr, "No double buffer available\n");
	exit(4);
    }
    
    img_desc[0] = '\0';
    describe_test(" First, draw into the back buffer.  Nothing should be\n");
    describe_test(" in this frame.\n");
    describe_test(" \n");

    PEXBeginRendering(dpy, renderBuf, renderer);
    PEXSetCharHeight(dpy, renderer, req_type, 0.05);
    PEXSetTextColorIndex(dpy, renderer, req_type, MAGENTA);
    PEXSetTextPrecision(dpy, renderer, req_type, PEXStrokePrecision);
    PEXText2D(dpy, renderer, req_type, &coord1,  strlen("Buffer 1"),
    "Buffer 1");
    PEXEndRendering(dpy, renderer, True);
    XFlush(dpy);
    XSync(dpy, 0);
    testproc(dpy, renderBuf, i++);

    img_desc[0] = '\0';
    describe_test(" Now, switch buffers -- the text 'Buffer 1' should be ");
    describe_test(" visible.\n");
    describe_test(" \n");
    renderBuf = PEXUtDBSwap(dpy, window, False);
    XFlush(dpy);
    XSync(dpy, 0);
    testproc(dpy, renderBuf, i++);

    img_desc[0] = '\0';
    describe_test(" Draw into the back buffer the string 'Buffer 2' and\n");
    describe_test(" swap buffers.  The string 'Buffer 2' should be seen.\n");
    describe_test(" \n");
    PEXBeginRendering(dpy, renderBuf, renderer);
    PEXSetCharHeight(dpy, renderer, req_type, 0.05);
    PEXText2D(dpy, renderer, req_type, &coord2,  strlen("Buffer 2"),
    "Buffer 2");
    PEXEndRendering(dpy, renderer, True);
    renderBuf = PEXUtDBSwap(dpy, window, False);
    XFlush(dpy);
    XSync(dpy, 0);
    testproc(dpy, renderBuf, i++);

    img_desc[0] = '\0';
    describe_test(" Draw into the front buffer.  Now both text strings \n");
    describe_test(" should be visible.\n");
    describe_test(" \n");
    dfront_status = PEXUtDBFront( dpy, window, 1, &renderBuf );
    if (dfront_status == PEXUTDBOKAY)
    {
	dfront_ok = 1;
    }
    else
    {
	dfront_ok = 0;
	fprintf(stderr,"Rendering to front buffer not supported!\n");
    }
    if( dfront_ok)
    {
	PEXBeginRendering(dpy, renderBuf, renderer);
	PEXSetCharHeight(dpy, renderer, req_type, 0.05);
	PEXText2D(dpy, renderer, req_type, &coord1,  strlen("Buffer 1"),
	"Buffer 1");
	sleep(1);
	PEXText2D(dpy, renderer, req_type, &coord2,  strlen("Buffer 2"),
	"Buffer 2");
	PEXEndRendering(dpy, renderer, True);
	XFlush(dpy);
	XSync(dpy, 0);
	testproc(dpy, renderBuf, i++);

	img_desc[0] = '\0';
	describe_test(" Switch buffers once more -- The magenta 'Buffer 1'\n");
	describe_test(" text should be visable again.\n");
	describe_test(" \n");
	renderBuf = PEXUtDBSwap(dpy, window, False);
	XFlush(dpy);
	XSync(dpy, 0);
	testproc(dpy, renderBuf, i++);
    }
}


void run_test_imm(Display *dpy, Window window, 
		  int supported_modes, int num_images);

/* utility functions we use */
/*
extern Window		create_window();
extern PEXRenderer	setup_renderer();
extern PEXWorkstation	setup_workstation();
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

    inquire_test_params(test_name, &num_images, &supported_modes, win_title);

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


    i = 0;
    img_desc[0] = '\0';
    misc_setup(dpy, window, renderer, i);
    execute_test(dpy, window, (XID)renderer, PEXOCRender, i);


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
