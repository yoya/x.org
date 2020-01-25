/* $XConsortium: fnt_load.c,v 1.0 93/11/22 12:27:53 rws Exp $ */

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

void printFontInfo(Display *display, const PEXFontInfo *info)
{
    unsigned int i;
    
    _hppex_stderr_print("\tfirstGlyph   = %x\n", info->first_glyph);
    _hppex_stderr_print("\tlastGlyph    = %x\n", info->last_glyph);
    _hppex_stderr_print("\tdefaultGlyph = %x\n", info->default_glyph);

    for (i = 0; i < info->count; i++)
	_hppex_stderr_print("\t\t%s = %s\n",
		XGetAtomName(display, info->props[i].name),
		XGetAtomName(display, info->props[i].value));
}

void printExtent(PEXTextExtent *extent)
{
    _hppex_stderr_print("\tll = (%f, %f)\n",
	   extent->lower_left.x, extent->lower_left.y); 
    _hppex_stderr_print("\tur = (%f, %f)\n",
	   extent->upper_right.x, extent->upper_right.y); 
    _hppex_stderr_print("\tcc = (%f, %f)\n",
	   extent->concat_point.x, extent->concat_point.y);
}

void font_info(Display *display, Window window)
{
    char **names;
    unsigned long count;
    PEXLookupTable lut;
    PEXFontInfo *info;
    
    names = PEXListFontsWithInfo(display, "*", 1, &count, &info);

    _hppex_stderr_print("name = %s\n", *names);
    _hppex_stderr_print("info from load:\n");
    printFontInfo(display, info);

    PEXFreeFontInfo(count, info);

    lut = PEXCreateLookupTable(display, window, PEXLUTTextFont);

    {
	PEXFont font = PEXLoadFont(display, *names);
	PEXTextFontEntry entry;
	PEXTextExtent *extent;
	PEXStringData stringData = {3, "foo"};

	info = PEXQueryFont(display, font);

	_hppex_stderr_print("info from query:\n");
	printFontInfo(display, info);

	PEXFreeFontInfo(1, info);

	entry.count = 1;
	entry.fonts = &font;
	PEXSetTableEntries(display, lut, 1, 1, PEXLUTTextFont,
			   &entry);

	PEXUnloadFont(display, font);

	extent = PEXQueryTextExtents(display, lut, 1, PEXPathRight,
				     1.0, 0.0, 0.01, PEXHAlignNormal,
				     PEXVAlignNormal, 1, &stringData);

	printExtent(extent);

	XFree(extent);
    }
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "fnt_load");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "fnt_load");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    local_win = window;
    _hppex_set_stderr("fnt_load.stderr", "fnt_load");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPEXListFontsWithInfo / PEXLoadFont / PEXQueryFont Test\n\n");
    describe_test("  The stderr file (only printed with the -v option) will contain\n");
    describe_test("  font info before and after loading the font.\n\n");
    fflush(stdout);

    font_info(dpy, local_win);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("fnt_load.stderr", "fnt_load");
    }
