/* $XConsortium: fnt_text.c,v 1.0 93/11/22 12:27:13 rws Exp $ */

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

char *path_name[] =
{
    "PEXPathRight",
    "PEXPathLeft",
    "PEXPathUp",
    "PEXPathDown"
};

char *halign_name[] =
{
    "PEXHAlignNormal",
    "PEXHAlignLeft",
    "PEXHAlignCenter",
    "PEXHAlignRight"
};

char *valign_name[] =
{
    "PEXVAlignNormal",
    "PEXVAlignTop",
    "PEXVAlignCap",
    "PEXVAlignHalf",
    "PEXVAlignBase",
    "PEXVAlignBottom"
};

void tag(Display *display, XID rdr, PEXOCRequestType type,
	 unsigned int count, PEXCoord *points)
{
    unsigned int i;

    for (i = 0; i < count; i++)
    {
	PEXCoord temp[2];

	temp[0] = temp[1] = points[i];
	temp[0].x -= 0.002;
	temp[0].y -= 0.002;
	temp[1].x += 0.002;
	temp[1].y += 0.002;
	PEXPolyline(display, rdr, type, 2, temp);

	temp[0].y += 0.004;
	temp[1].y -= 0.004;
	PEXPolyline(display, rdr, type, 2, temp);
    }
}

void do_model(Display *display, XID res, PEXOCRequestType req_type)
{
    PEXEncodedTextData encoded_text[2];
    PEXCoord base, coord;
    PEXVector v1 = {1, 0, 0}, v2 = {0, 1, 0};
    int path, halign, valign;
    double spacing;

    base.z = 0.0;
    for (path = PEXPathRight, base.y = 0.75;
	 path <= PEXPathDown;
	 path++, base.y -= 0.25)
    {
	begin_structure(display, &res, req_type);

	PEXSetTextPath(display, res, req_type, path);

	/*
	  spacing
	*/
	begin_structure(display, &res, req_type);
	base.x = 0.05;
	base.y += 0.05;
	for (spacing = 1.0, coord = base;
	     spacing >= -2.0;
	     spacing -= 1.0, coord.x += 0.05, coord.y += 0.05)
	{
	    PEXSetCharSpacing(display, res, req_type, spacing);
	    PEXText(display, res, req_type, &coord, &v1, &v2,
		    strlen("foo"),  "foo");
	    tag(display, res, req_type, 1, &coord);
	}
	base.y -= 0.05;
	end_structure(display, &res, req_type);

	/*
	  horizontal alignment
	*/
	begin_structure(display, &res, req_type);
	base.x = 0.3;
	base.y += 0.05;
	for (halign = PEXHAlignNormal, coord = base;
	     halign <= PEXHAlignRight;
	     halign++, coord.x += 0.05, coord.y += 0.05)
	{
	    PEXSetTextAlignment(display, res, req_type, halign,
				 PEXVAlignNormal);
	    PEXText(display, res, req_type, &coord, &v1, &v2,
		    strlen("foo"),  "foo");
	    tag(display, res, req_type, 1, &coord);
	}
	base.y -= 0.05;
	end_structure(display, &res, req_type);
	
	/*
	  vertical alignment
	*/
	begin_structure(display, &res, req_type);
	base.x = 0.5 + 0.25 / 7;
	base.y += 0.25 / 7;
	for (valign = PEXVAlignNormal, coord = base;
	     valign <= PEXVAlignBottom;
	     valign++, coord.x += 0.25 / 7, coord.y += 0.25 / 7)
	{
	    PEXSetTextAlignment(display, res, req_type,
				 PEXHAlignNormal, valign);
	    PEXText(display, res, req_type, &coord, &v1, &v2,
		    strlen("foo"),  "foo");
	    tag(display, res, req_type, 1, &coord);
	}
	base.y -= 0.25 / 7;
	end_structure(display, &res, req_type);

	end_structure(display, &res, req_type);
    }

    /*
      char expansion
    */
    begin_structure(display, &res, req_type);
    coord.x = 0.80;
    coord.y = 0.90;
    PEXSetCharExpansion(display, res, req_type, 2.0);
    PEXText(display, res, req_type, &coord, &v1, &v2,
	    strlen("foo"),  "foo");
    tag(display, res, req_type, 1, &coord);

    coord.y -= 0.05;
    PEXSetCharExpansion(display, res, req_type, -0.5);
    PEXText(display, res, req_type, &coord, &v1, &v2,
	    strlen("foo"),  "foo");
    tag(display, res, req_type, 1, &coord);
    end_structure(display, &res, req_type);

    /*
      proportional text
    */
    begin_structure(display, &res, req_type);
    coord.y -= 0.05;
    PEXSetTextFontIndex(display, res, req_type, 2);
    PEXText(display, res, req_type, &coord, &v1, &v2,
	    strlen("foo"),  "foo");
    tag(display, res, req_type, 1, &coord);
    end_structure(display, &res, req_type);

    /*
      character height
    */
    begin_structure(display, &res, req_type);
    coord.y -= 0.05;
    PEXSetCharHeight(display, res, req_type, -0.02);
    PEXText(display, res, req_type, &coord, &v1, &v2,
	    strlen("foo"),  "foo");
    tag(display, res, req_type, 1, &coord);
    end_structure(display, &res, req_type);

    /*
      encoded text
    */
    encoded_text[0].character_set = 1;
    encoded_text[0].character_set_width = PEXCSByte;
    encoded_text[0].ch = "foo";
    encoded_text[0].length = strlen(encoded_text[0].ch);

    coord.y -= 0.05;
    PEXEncodedText(display, res, req_type, &coord, &v1, &v2,
		   1, encoded_text);
    tag(display, res, req_type, 1, &coord);

    encoded_text[1] = encoded_text[0];

    coord.y -= 0.05;
    PEXEncodedText(display, res, req_type, &coord, &v1, &v2,
		   2, encoded_text);
    tag(display, res, req_type, 1, &coord);

    /*
      edge testing
    */
    coord.y -= 0.05;
    PEXText(display, res, req_type, &coord, &v1, &v2,
	    17, "-\040-\041-\176-\177-\240-\241-\376-\377-");
    tag(display, res, req_type, 1, &coord);

    /*
      control characters
    */
    coord.y -= 0.05;
    PEXText(display, res, req_type, &coord, &v1, &v2,
	    9, "-\000-\037-\200-\237-");
    tag(display, res, req_type, 1, &coord);

    /*
      up vector
    */
    coord.y -= 0.05;
    {
	PEXVector2D up = {-1.0, 1.0};

	begin_structure(display, &res, req_type);
	PEXSetCharUpVector(display, res, req_type, &up);
	PEXText(display, res, req_type, &coord, &v1, &v2,
		strlen("foo"),  "foo");
	tag(display, res, req_type, 1, &coord);
	end_structure(display, &res, req_type);
    }

    /*
      non-standard direction vectors
    */
    coord.y -= 0.05;
    v1.z = 1;
    PEXText(display, res, req_type, &coord, &v1, &v2,
	    strlen("foo"), "foo");
    tag(display, res, req_type, 1, &coord);
    v1.z = 0;

    coord.y -= 0.05;
    v1.y = 1;
    PEXText(display, res, req_type, &coord, &v1, &v2,
	    strlen("foo"), "foo");
    tag(display, res, req_type, 1, &coord);
    v1.y = 0;

    coord.y -= 0.05;
    v1.z = 1;
    v2 = v1;
    PEXText(display, res, req_type, &coord, &v1, &v2,
	    strlen("foo"), "foo");
    tag(display, res, req_type, 1, &coord);
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "fnt_text");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "fnt_text");
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
    describe_test("\nPEXText Mainline Test\n\n");
    describe_test("  The left most 3/4 of the screen tests PEXText with different combinations\n");
    describe_test("  of TextAlignment and TextPath.  The rightmost column tests CharExpansion,\n");
    describe_test("  FontIndex, CharHeight, EncodedText (foofoo), font edge testing, control\n");
    describe_test("  characters, and UpVectors\n\n");
    describe_test("  To see a larger image, execute:\n");
    describe_test("    fnt_text -v =800x800\n\n");

    do_model(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    image_testproc("fnt_text", "fnt_text", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
