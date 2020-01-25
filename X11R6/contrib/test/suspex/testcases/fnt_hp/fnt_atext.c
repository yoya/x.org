/* $XConsortium: fnt_atext.c,v 1.0 93/11/22 12:27:19 rws Exp $ */

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
    PEXCoord offset = {0, 0, 0};
    int path, halign, valign;
    double spacing;

    base.z = 0.0;
    for (path = PEXPathRight, base.y = 0.75;
	 path <= PEXPathDown;
	 path++, base.y -= 0.25)
    {
	begin_structure(display, &res, req_type);

	PEXSetATextPath(display, res, req_type, path);

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
	    PEXAnnotationText(display, res, req_type, &coord, &offset,
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
	    PEXSetATextAlignment(display, res, req_type, halign,
				 PEXVAlignNormal);
	    PEXAnnotationText(display, res, req_type, &coord, &offset,
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
	    PEXSetATextAlignment(display, res, req_type,
				 PEXHAlignNormal, valign);
	    PEXAnnotationText(display, res, req_type, &coord, &offset,
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
    PEXAnnotationText(display, res, req_type, &coord, &offset,
		      strlen("foo"),  "foo");
    tag(display, res, req_type, 1, &coord);

    coord.y -= 0.05;
    PEXSetCharExpansion(display, res, req_type, -0.5);
    PEXAnnotationText(display, res, req_type, &coord, &offset,
		     strlen("foo"),  "foo");
    tag(display, res, req_type, 1, &coord);
    end_structure(display, &res, req_type);

    /*
      proportional text
    */
    begin_structure(display, &res, req_type);
    coord.y -= 0.05;
    PEXSetTextFontIndex(display, res, req_type, 2);
    PEXAnnotationText(display, res, req_type, &coord, &offset,
		      strlen("foo"),  "foo");
    tag(display, res, req_type, 1, &coord);
    end_structure(display, &res, req_type);

    /*
      character height
    */
    begin_structure(display, &res, req_type);
    coord.y -= 0.05;
    PEXSetATextHeight(display, res, req_type, -0.02);
    PEXAnnotationText(display, res, req_type, &coord, &offset,
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
    PEXEncodedAnnoText(display, res, req_type, &coord, &offset,
		       1, encoded_text);
    tag(display, res, req_type, 1, &coord);

    encoded_text[1] = encoded_text[0];

    coord.y -= 0.05;
    PEXEncodedAnnoText(display, res, req_type, &coord, &offset,
		       2, encoded_text);
    tag(display, res, req_type, 1, &coord);

    /*
      up vector
    */
    coord.y -= 0.05;
    {
	PEXVector2D up = {-1.0, 1.0};

	begin_structure(display, &res, req_type);
	PEXSetATextUpVector(display, res, req_type, &up);
	PEXAnnotationText(display, res, req_type, &coord, &offset,
			  strlen("foo"),  "foo");
	tag(display, res, req_type, 1, &coord);
	end_structure(display, &res, req_type);
    }

    /*
      offsets
    */
    coord.y -= 0.05;
    offset.x = 0.02;
    PEXAnnotationText(display, res, req_type, &coord, &offset,
		      strlen("foo"),  "foo"); 
    tag(display, res, req_type, 1, &coord);

    coord.y -= 0.05;
    offset.x = 0;
    offset.y = -0.02;
    PEXAnnotationText(display, res, req_type, &coord, &offset,
		      strlen("foo"),  "foo"); 
    tag(display, res, req_type, 1, &coord);

    offset.y = 0.0;

    /*
      rotated text
    */
    coord.y -= 0.05;
    {
	PEXVector trans = {0,0,0};
	PEXVector scale = {1,1,1};
	PEXMatrix matrix;

	PEXBuildTransform(&coord, &trans, 0.0, 0.0, 3.14159 / 2,
			  &scale, matrix);

	PEXSetLocalTransform(display, res, req_type, PEXReplace,
			     matrix);
	PEXAnnotationText(display, res, req_type, &coord, &offset,
			  strlen("foo"),  "foo"); 
	tag(display, res, req_type, 1, &coord);

	coord.y -= 0.05;
	PEXBuildTransform(&coord, &trans, 0.0, 3.14159 / 2, 0.0,
			  &scale, matrix);

	PEXSetLocalTransform(display, res, req_type, PEXReplace,
			     matrix);
	PEXAnnotationText(display, res, req_type, &coord, &offset,
			  strlen("foo"),  "foo"); 
	tag(display, res, req_type, 1, &coord);
    }
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "fnt_atext");
    *num_images      = 1;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "fnt_atext");
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
    describe_test("\nPEXAnnotationText Mainline Test\n\n");
    describe_test("  The left most 3/4 of the screen tests PEXAnnotationText with different \n");
    describe_test("  combinations of TextAlignment and TextPath.  The rightmost column tests n");
    describe_test("  CharExpansion, Proportional Text, CharHeight, EncodedText (foofoo), \n");
    describe_test("  UpVector, and Rotation (of ref marker only)\n\n");
    describe_test("  To see a larger image, execute:\n");
    describe_test("    fnt_atext -v =800x800\n\n");

    do_model(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    image_testproc("fnt_atext", "fnt_atext", img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
