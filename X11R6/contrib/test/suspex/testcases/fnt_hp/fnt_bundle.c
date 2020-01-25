/* $XConsortium: fnt_bundle.c,v 1.0 93/11/22 12:27:30 rws Exp $ */

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
#ifdef SYSV
#include <string.h>
#else
#include <strings.h>
#endif
#include <X11/Xlib.h>
#include <X11/PEX5/PEXlib.h>
#include <X11/Xutil.h>
#include <misc.h>
#include <lut.h>
#include <fnt_common.h>

void tag(Display *display, XID rdr, PEXOCRequestType type,
	 unsigned int count, PEXCoord2D *points)
{
    unsigned int i;

    for (i = 0; i < count; i++)
    {
	PEXCoord2D temp[2];

	temp[0] = temp[1] = points[i];
	temp[0].x -= 0.002;
	temp[0].y -= 0.002;
	temp[1].x += 0.002;
	temp[1].y += 0.002;
	PEXPolyline2D(display, rdr, type, 2, temp);

	temp[0].y += 0.004;
	temp[1].y -= 0.004;
	PEXPolyline2D(display, rdr, type, 2, temp);
    }
}

void do_model(Display *display, XID res, PEXOCRequestType req_type)
{
    unsigned long asf;
    PEXCoord2D coord;

    coord.x = -0.1;
    coord.y = 1.0;

    coord.y = 0.9;
    coord.x = 0.1;
    PEXText2D(display, res, req_type, &coord,
	      strlen("TextFontIndex"), "TextFontIndex");

    coord.x += 0.2;
    PEXText2D(display, res, req_type, &coord,
	      strlen("TextPrec"), "TextPrec");

    coord.x += 0.2;
    PEXText2D(display, res, req_type, &coord,
	      strlen("CharExpansion"), "CharExpansion");

    coord.x += 0.2;
    PEXText2D(display, res, req_type, &coord,
	      strlen("CharSpacing"), "CharSpacing");

    coord.x += 0.2;
    PEXText2D(display, res, req_type, &coord,
	      strlen("TextColor"), "TextColor");

    coord.x = -0.1;

    for (asf = PEXASFTextFontIndex; asf <= PEXASFTextColor; asf <<= 1)
    {
	coord.x += 0.2;

	begin_structure(display, &res, req_type);

	coord.y = 0.8;
	PEXSetIndividualASF(display, res, req_type, asf, PEXBundled);
	PEXText2D(display, res, req_type, &coord,
		  strlen("lll"),  "lll");
	tag(display, res, req_type, 1, &coord);
	
	end_structure(display, &res, req_type);

	begin_structure(display, &res, req_type);

	coord.y -= 0.1;
	PEXSetIndividualASF(display, res, req_type, asf, PEXBundled);
	PEXSetTextBundleIndex(display, res, req_type, 2);
	PEXText2D(display, res, req_type, &coord,
		  strlen("lll"),  "lll");
	tag(display, res, req_type, 1, &coord);

	end_structure(display, &res, req_type);

	begin_structure(display, &res, req_type);

	coord.y -= 0.1;
	PEXSetTextBundleIndex(display, res, req_type, 2);
	PEXText2D(display, res, req_type, &coord,
		  strlen("lll"),  "lll");
	tag(display, res, req_type, 1, &coord);

	end_structure(display, &res, req_type);

	begin_structure(display, &res, req_type);

	coord.y -= 0.1;
	PEXSetTextBundleIndex(display, res, req_type, 2);
	PEXSetIndividualASF(display, res, req_type, asf, PEXBundled);
	PEXText2D(display, res, req_type, &coord,
		  strlen("lll"),  "lll");
	tag(display, res, req_type, 1, &coord);

	coord.y -= 0.1;
	PEXSetIndividualASF(display, res, req_type, asf, PEXIndividual);
	PEXText2D(display, res, req_type, &coord,
		  strlen("lll"),  "lll");
	tag(display, res, req_type, 1, &coord);

	end_structure(display, &res, req_type);
    }
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "fnt_bundle");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "fnt_bundle");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
	if (cur_image == 1)
		load_two_fonts (dpy, fontLUT);
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPEX Text Bundle Test\n\n");
    describe_test("  This test exercises switching back and forth between indivivual\n");
    describe_test("  and bundled text attributes. Rows 1,2, and 4 should all be the same\n");
    describe_test("  with the 111 text in the last column being yellow.  Rows 3 and 5\n");
    describe_test("  should also be the same.\n\n");
    describe_test("  To see a larger image, execute:\n");
    describe_test("    fnt_bundle -v =800x800\n\n");

    do_model(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    image_testproc("fnt_bundle", "fnt_bundle", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
