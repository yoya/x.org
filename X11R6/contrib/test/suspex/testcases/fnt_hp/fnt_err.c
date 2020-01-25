/* $XConsortium: fnt_err.c,v 1.0 93/11/22 12:27:36 rws Exp $ */

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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/PEX5/PEXlib.h>
#include <misc.h>

Window local_win;

void font_errors(Display *display, Window window)
    {
    PEXLookupTable lut;

    XSynchronize(display, True);

    _hppex_stderr_print("font name doesn't exist\n");
    {
	PEXFont font = PEXLoadFont(display, "foo");

	_hppex_stderr_print("unload null font\n");
	PEXUnloadFont(display, font);
    }

    lut = PEXCreateLookupTable(display, window, PEXLUTTextFont);
 
    _hppex_stderr_print("unload non-font resource\n");
    PEXUnloadFont(display, lut);

    _hppex_stderr_print("query non-font resource\n");
    PEXQueryFont(display, lut);

    {
	unsigned long count;
	char **names = PEXListFonts(display, "*", 1, &count);
	PEXFont font = PEXLoadFont(display, *names);
	PEXTextFontEntry entry, *ret;
	int table_type;

	XFree(names);

	entry.count = 1;
	entry.fonts = &font;
	PEXSetTableEntries(display, lut, 1, 1, PEXLUTTextFont,
			   &entry);

	PEXUnloadFont(display, font);

	_hppex_stderr_print("unload same font twice\n");
	PEXUnloadFont(display, font);

	PEXGetTableEntries(display, lut, 1, 1, PEXSetValue,
			   &table_type, (PEXPointer *)&ret);

	_hppex_stderr_print("pass bogus pointer to unload\n");
	PEXUnloadFont(display, (PEXFont)ret);
	_hppex_stderr_print("unload already freed font\n");
	PEXUnloadFont(display, *ret->fonts);

	PEXFreeTableEntries(PEXLUTTextFont, 1, ret);
    }

    /*
      QueryTextExtent, QueryEncodedTextExtent
    */
    {
	unsigned long count;
	char **names = PEXListFonts(display, "*", 1, &count);
	PEXFont font = PEXLoadFont(display, *names);
	PEXTextExtent *extent;
	PEXStringData text;

	text.ch = "foo";
	text.length = strlen(text.ch);

	/*
	  bad resource id
	*/
	_hppex_stderr_print("null resource id\n");
	extent = PEXQueryTextExtents(display, 0, 1, PEXPathRight,
				     1.0, 0.0, 0.01,
				     PEXHAlignNormal, PEXVAlignNormal,
				     1, &text);
	_hppex_stderr_print("non-LUT resource id\n");
	extent = PEXQueryTextExtents(display, font, 1, PEXPathRight,
				     1.0, 0.0, 0.01,
				     PEXHAlignNormal, PEXVAlignNormal,
				     1, &text);

	/*
	  bad path
	*/
	_hppex_stderr_print("bad text path -1\n");
	extent = PEXQueryTextExtents(display, lut, 1, -1,
				     1.0, 0.0, 0.01,
				     PEXHAlignNormal, PEXVAlignNormal,
				     1, &text);
	_hppex_stderr_print("bad text path 4\n");
	extent = PEXQueryTextExtents(display, lut, 1, 4,
				     1.0, 0.0, 0.01,
				     PEXHAlignNormal, PEXVAlignNormal,
				     1, &text);

	/*
	  bad horizontal alignment
	*/
	_hppex_stderr_print("bad horizontal alignment -1\n");
	extent = PEXQueryTextExtents(display, lut, 1, PEXPathRight,
				     1.0, 0.0, 0.01,
				     -1, PEXVAlignNormal,
				     1, &text);
	_hppex_stderr_print("bad horizontal alignment 4\n");
	extent = PEXQueryTextExtents(display, lut, 1, PEXPathRight,
				     1.0, 0.0, 0.01,
				     4, PEXVAlignNormal,
				     1, &text);

	/*
	  bad vertical alignment
	*/
	_hppex_stderr_print("bad vertical alignment -1\n");
	extent = PEXQueryTextExtents(display, lut, 1, PEXPathRight,
				     1.0, 0.0, 0.01,
				     PEXHAlignNormal, -1,
				     1, &text);
	_hppex_stderr_print("bad vertical alignment 6\n");
	extent = PEXQueryTextExtents(display, lut, 1, PEXPathRight,
				     1.0, 0.0, 0.01,
				     PEXHAlignNormal, 6,
				     1, &text);
    }
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "fnt_err");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "fnt_err");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    local_win = window;
    _hppex_set_stderr("fnt_err.stderr", "fnt_err");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPEX Text and Font Errors \n\n");
    describe_test("  The stderr file (only printed with the -v option) will contain\n");
    describe_test("  error condition descriptions followed by the trapped error\n");
    describe_test("  print out.\n\n");
    fflush(stdout);

    font_errors(dpy, local_win);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("fnt_err.stderr", "fnt_err");
    }
