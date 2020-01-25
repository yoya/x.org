/* $XConsortium: fnt_info.c,v 1.0 93/11/22 12:27:42 rws Exp $ */

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
#include <X11/PEX5/PEXlib.h>
#include <misc.h>


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

void font_info(Display *display)
{
    int i;
    char **names;
    PEXFontInfo *info;
    unsigned long count;
    PEXExtensionInfo *ext_info;

    names = PEXListFontsWithInfo(display, "*", 999, &count, &info);

    for (i = 0; i < count; i++)
    {
	_hppex_stderr_print("name = %s\n", names[i]);
	printFontInfo(display, info + i);
    }

    PEXFreeFontNames(count, names);
    PEXFreeFontInfo(count, info);
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "fnt_info");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "fnt_info");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("fnt_info.stderr", "fnt_info");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPEXListFontsWithInfo Test\n\n");
    describe_test("  The stderr file (only printed with the -v option) will contain\n");
    describe_test("  info on all fonts returned from PEXListFontsWithInfo\n\n");
    fflush(stdout);

    font_info(dpy);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("fnt_info.stderr", "fnt_info");
    }
