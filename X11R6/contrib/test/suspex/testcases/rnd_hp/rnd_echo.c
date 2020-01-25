/* $XConsortium: rnd_echo.c,v 1.0 93/11/22 12:39:36 rws Exp $ */

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


#include <stdio.h>
#include <string.h>

#include <X11/X.h>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>
#include <misc.h>

#define BLACK        0
#define WHITE        1
#define RED          2
#define YELLOW       3
#define GREEN        4
#define CYAN         5
#define BLUE         6
#define MAGENTA      7

#define X_INCREMENT   0.17
#define Y_INCREMENT   0.15
#define X_START       0.10

#define OFFSET_HORIZONTAL                                      \
    {                                                          \
    int i;                                                     \
							       \
    line1[0].x    += current_x_pos;                            \
    line1[1].x    += current_x_pos;                            \
    line2[0].x    += current_x_pos;                            \
    line2[1].x    += current_x_pos;                            \
    text_origin.x += current_x_pos;                            \
    marker_loc.x  += current_x_pos;                            \
    for (i=0; i<5; i++)                                        \
	{                                                      \
        front_face[i].x += current_x_pos;                      \
        back_face[i].x  += current_x_pos;                      \
	}                                                      \
    }

static float   current_x_pos;
Window         local_win;

void setup_attrs(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXSetLineColorIndex(dpy, resource_id, request_type, RED);
    PEXSetLineType      (dpy, resource_id, request_type, PEXLineTypeDotted);
    PEXSetPolylineInterpMethod(dpy, resource_id, request_type, 
			       PEXPolylineInterpColor);
    PEXSetBFSurfaceColorIndex(dpy, resource_id, request_type, RED);
    PEXSetSurfaceEdgeColorIndex(dpy, resource_id, request_type, RED);
    PEXSetFacetDistinguishFlag(dpy, resource_id, request_type, True);
    PEXSetSurfaceEdgeFlag(dpy, resource_id, request_type, PEXOn);
    PEXSetInteriorStyle  (dpy, resource_id, request_type, PEXInteriorStyleSolid);
    PEXSetBFInteriorStyle(dpy, resource_id, request_type, PEXInteriorStyleSolid);
    PEXSetSurfaceColorIndex  (dpy, resource_id, request_type, BLUE);
    PEXSetBFSurfaceColorIndex(dpy, resource_id, request_type, GREEN);
    PEXSetMarkerColorIndex(dpy, resource_id, request_type, CYAN);
    PEXSetMarkerScale(dpy, resource_id, request_type, 5.0);
    }

void draw_prims(Display *dpy, XID resource_id, PEXOCRequestType request_type)
    {
    PEXCoord line1[2]    = {0.0, 0.9, 0.0,
			    0.1, 0.9, 0.0};

    PEXCoord line2[2]    = {0.0, 0.8, 0.0,
			    0.1, 0.8, 0.0};

    PEXCoord2D front_face[5] = {0.0, 0.7,
				0.0, 0.6,
				0.1, 0.6, 
				0.1, 0.7,
				0.0, 0.7};

    PEXCoord2D back_face[5]  = {0.0, 0.5,
				0.1, 0.5,
				0.1, 0.4, 
				0.0, 0.4,
				0.0, 0.5};

    PEXCoord2D text_origin = { 0.0 , 0.2 };
    PEXCoord   marker_loc  = { 0.06, 0.1 , 0.0};

    PEXListOfVertex  v_list;
    PEXVertexRGB v_data[2];
    PEXName names[1] = {1};

    v_list.count          = 2;
    v_list.vertices.rgb   = v_data;

    OFFSET_HORIZONTAL

    PEXAddToNameSet(dpy, resource_id, request_type, 1, names);
    PEXPolyline         (dpy, resource_id, request_type, 2, line1);

    v_data[0].point.x   = line2[0].x;
    v_data[0].point.y   = line2[0].y;
    v_data[0].point.z   = line2[0].z;
    v_data[0].rgb.red   = 0.0;
    v_data[0].rgb.green = 0.0;
    v_data[0].rgb.blue  = 0.0;
    v_data[1].point.x   = line2[1].x;
    v_data[1].point.y   = line2[1].y;
    v_data[1].point.z   = line2[1].z;
    v_data[1].rgb.red   = 1.0;
    v_data[1].rgb.green = 1.0;
    v_data[1].rgb.blue  = 1.0;
    PEXPolylineSetWithData(dpy, resource_id, request_type, PEXGAColor,
			   PEXColorTypeRGB, 1, &v_list);

    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, front_face);
    PEXFillArea2D(dpy, resource_id, request_type, PEXShapeUnknown,
		False, 5, back_face);

    PEXSetTextPrecision(dpy, resource_id, request_type, PEXStrokePrecision); 
    PEXSetCharHeight(dpy, resource_id, request_type, 0.04);
    PEXSetTextColorIndex(dpy, resource_id, request_type, MAGENTA);
    PEXText2D(dpy, resource_id, request_type, &text_origin, 3, "PEX");

    PEXMarkers(dpy, resource_id, request_type, 1, &marker_loc);
    }


void echo_image(Display         *dpy, 
		XID              rid, 
		PEXOCRequestType req_type,
		Window           window, 
		int              cur_image)
    {
    PEXStructure sids[5];
    PEXElementRef current_path_elems;
    PEXRendererAttributes rdr_attrs;
    PEXNameSet highlight_filter;
    PEXName names[1] = {1};

    switch (cur_image)
	{
	case 1:
	/*
	     Image #1 -- Echo mode
	*/
	current_x_pos = X_START;
        describe_test( "\nImage 1\n");
        describe_test( "    Column #1 -- Standard Attributes\n");
        describe_test( "      Red dashed line\n");
        describe_test( "      Black to white interpolated dashed line\n");
        describe_test( "      Blue  solid (red edged) front facing polygon\n");
        describe_test( "      Green solid (red edged) back  facing polygon\n");
        describe_test( "      Magenta text\n");
        describe_test( "      Cyan asterisk\n");
        describe_test( "    Column #2 -- Echo Attributes (overriding default)\n");
        describe_test( "      All colors should be identical (XOR)\n");
        describe_test( "      No line interpolation\n");
        describe_test( "      Interior hollow \n");
        describe_test( "      Asterisk at nominal scale\n");
        describe_test( "    Column #3 -- Echo Attributes (disabling attrs)\n");
        describe_test( "      Same as column #2 except asterisk larger\n");
        describe_test( "    Column #4 -- Echo / UnEcho\n");
        describe_test( "      Column should be BLANK!\n");
        describe_test( "    Column #5 -- Back to Standard Attributes\n");
        describe_test( "      Column should identical to Column #1\n");

	/* Do a begin / end rendering to clear the image */
	XSynchronize (dpy, True);
	PEXBeginRendering (dpy, window, rid);
	PEXEndRendering   (dpy, rid, True);

	/* Now turn off clear_image so we can do multiple */
	/* begin / end pairs to the same image */

	rdr_attrs.clear_image = False;
        PEXChangeRenderer (dpy, rid, PEXRAClearImage, &rdr_attrs);
	/************************************************************/
	/*                                                          */
	/*  Column 1 -- Normal attributes                           */
	/*                                                          */
	/************************************************************/
	PEXBeginRendering (dpy, window, rid);
	setup_attrs       (dpy, rid, PEXOCRender);
	draw_prims        (dpy, rid, PEXOCRender);
	PEXEndRendering   (dpy, rid, True);

	/************************************************************/
	/*                                                          */
	/*  Column 2 -- Echo mode enabled                           */
	/*              (overriding default attrs)                  */
	/*                                                          */
	/************************************************************/
	current_x_pos += X_INCREMENT;
	rdr_attrs.echo_mode = PEXEcho;
        PEXChangeRenderer (dpy, rid, PEXRAEchoMode, &rdr_attrs);
	PEXBeginRendering (dpy, window, rid);
	draw_prims        (dpy, rid, PEXOCRender);
	PEXEndRendering   (dpy, rid, True);

	/************************************************************/
	/*                                                          */
	/*  Column 3 -- Echo mode enabled                           */
	/*              (disable setting of new attrs)              */
	/*                                                          */
	/************************************************************/
	current_x_pos += X_INCREMENT;
	rdr_attrs.echo_mode = PEXEcho;
        PEXChangeRenderer (dpy, rid, PEXRAEchoMode, &rdr_attrs);
	PEXBeginRendering (dpy, window, rid);
	setup_attrs       (dpy, rid, PEXOCRender);
	draw_prims        (dpy, rid, PEXOCRender);
	PEXEndRendering   (dpy, rid, True);

	/************************************************************/
	/*                                                          */
	/*  Column 4 -- Echo followed by UnEcho (should be blank)   */
	/*                                                          */
	/************************************************************/
	PEXBeginRendering (dpy, window, rid);
	current_x_pos += X_INCREMENT;
	draw_prims(dpy, rid, PEXOCRender);
	PEXEndRendering   (dpy, rid, True);

	rdr_attrs.echo_mode = PEXUnEcho;
        PEXChangeRenderer (dpy, rid, PEXRAEchoMode, &rdr_attrs);
	PEXBeginRendering (dpy, window, rid);
	draw_prims(dpy, rid, PEXOCRender);
	PEXEndRendering   (dpy, rid, True);

	/************************************************************/
	/*                                                          */
	/*  Column 5 -- Back to normal attrs                        */
	/*                                                          */
	/************************************************************/
	current_x_pos += X_INCREMENT;
	rdr_attrs.echo_mode = PEXNoEcho;
        PEXChangeRenderer (dpy, rid, PEXRAEchoMode, &rdr_attrs);
	PEXBeginRendering (dpy, window, rid);
	setup_attrs       (dpy, rid, PEXOCRender);
	draw_prims        (dpy, rid, PEXOCRender);
	PEXEndRendering(dpy, rid, True);

	break;

	case 2:
	/*
	     Image #2 -- immediate mode highlighting
	*/

        /* create the highlight filter nameset */
        highlight_filter = PEXCreateNameSet(dpy);
        PEXChangeNameSet(dpy, highlight_filter, PEXNSAdd, 1, names);

	current_x_pos = X_START;
        describe_test( "\nImage 2\n");
        describe_test( "    Column #1 -- Standard Attributes\n");
        describe_test( "      Red dashed line\n");
        describe_test( "      Black to white interpolated dashed line\n");
        describe_test( "      Blue  solid (red edged) front facing polygon\n");
        describe_test( "      Green solid (red edged) back  facing polygon\n");
        describe_test( "      Magenta text\n");
        describe_test( "      Cyan asterisk\n");
        describe_test( "    Column #2 -- Highlight Attributes\n");
        describe_test( "                 (override default attrs)\n");
        describe_test( "      Everything should be white\n");
        describe_test( "      No line interpolation\n");
        describe_test( "      Interior Hollow \n");
        describe_test( "      Asterisk default scale \n");
        describe_test( "    Column #3 -- Highlight Attributes\n");
        describe_test( "                 (disable new attr installation)\n");
        describe_test( "      Everything should be white\n");
        describe_test( "      No line interpolation\n");
        describe_test( "      Interior Solid \n");
        describe_test( "      Asterisk same scale as Column #2\n");
        describe_test( "    Column #4 -- Back to Standard Attributes\n");
        describe_test( "      Column should be identical to Column #1\n");

	/*  clear the image via begin/end rendering with clear_image set */
	rdr_attrs.clear_image = True;
        PEXChangeRenderer (dpy, rid, PEXRAClearImage, &rdr_attrs);
	PEXBeginRendering (dpy, window, rid);
	PEXEndRendering   (dpy, rid, True);
       
	/*  Again turn off clear image for multiple begin / end renders */
	rdr_attrs.clear_image = False;
        PEXChangeRenderer (dpy, rid, PEXRAClearImage, &rdr_attrs);

	/************************************************************/
	/*                                                          */
	/*  Column 1 -- Normal attributes                           */
	/*                                                          */
	/************************************************************/
	PEXBeginRendering (dpy, window, rid);
	setup_attrs       (dpy, rid, PEXOCRender); 
	draw_prims        (dpy, rid, PEXOCRender);
	PEXEndRendering   (dpy, rid, True);

	/************************************************************/
	/*                                                          */
	/*  Column 2 -- Highlight mode overriding existing attrs    */
	/*                                                          */
	/************************************************************/
	current_x_pos += X_INCREMENT;
	rdr_attrs.highlight_incl = highlight_filter;
        PEXChangeRenderer (dpy, rid, PEXRAHighlightIncl, &rdr_attrs);
	PEXBeginRendering (dpy, window, rid);
	draw_prims        (dpy, rid, PEXOCRender);
	PEXEndRendering   (dpy, rid, True);

	/************************************************************/
	/*                                                          */
	/*  Column 3 -- Highlight mode disabling new attribute      */
	/*              installation                                */
	/*                                                          */
	/************************************************************/
	current_x_pos += X_INCREMENT;
	rdr_attrs.highlight_incl = highlight_filter;
        PEXChangeRenderer (dpy, rid, PEXRAHighlightIncl, &rdr_attrs);
	PEXBeginRendering (dpy, window, rid);
	setup_attrs       (dpy, rid, PEXOCRender);
	draw_prims        (dpy, rid, PEXOCRender);
	PEXEndRendering   (dpy, rid, True);

	/************************************************************/
	/*                                                          */
	/*  Column 4 -- Back to normal attributes                   */
	/*              (without re-setting)                        */
	/*                                                          */
	/************************************************************/
	current_x_pos += X_INCREMENT;
	rdr_attrs.highlight_incl = NULL;
        PEXChangeRenderer (dpy, rid, PEXRAHighlightIncl, &rdr_attrs);

	PEXBeginRendering (dpy, window, rid);
	setup_attrs       (dpy, rid, PEXOCRender);
	draw_prims        (dpy, rid, PEXOCRender);
	PEXEndRendering   (dpy, rid, True);

	}
    }


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "rnd_echo");
    *num_images      = 2;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "rnd_echo");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    local_win = window;
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    echo_image(dpy, resourceID, req_type, local_win, cur_image);
    }


void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];
    
    sprintf(image_name,"rnd_echo_%d", cur_image);
    sprintf(test_name, "rnd_echo #%d", cur_image);
     image_testproc(image_name, test_name, img_desc,
                      dpy, window, 0, 0,
                      glob_window_width, glob_window_height);
    }
