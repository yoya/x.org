/* $XConsortium: fnt_ref.c,v 1.0 93/11/22 12:27:59 rws Exp $ */

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


void test_fonts(Display *display)
{
    int i;
    unsigned long count;
    char **names;
    PEXFont font[100];

    names = PEXListFonts(display, "*", 1, &count);

    for (i = 0; i < 100; i++)
    {
	_hppex_stderr_print(".");
	PEXUnloadFont(display,
		      PEXLoadFont(display, *names));
    }
    _hppex_stderr_print("\n");

    for (i = 0; i < 100; i++)
    {
	_hppex_stderr_print("+");
	font[i] = PEXLoadFont(display, *names);
    }
    _hppex_stderr_print("\n");

    for (i = 0; i < 100; i++)
    {
	_hppex_stderr_print("-");
	PEXUnloadFont(display, font[i]);
    }
    _hppex_stderr_print("\n");
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "fnt_ref");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "fnt_ref");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("fnt_ref.stderr", "fnt_ref");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPEXLoadFont / PEXUnloadFont Test\n\n");
    describe_test("  If successful, 100 '.' characters, followed by 100 '+'\n");
    describe_test("  characters, followed by 100 '=' characters will be written\n");
    describe_test("  to the stderr file.\n\n");

    test_fonts(dpy);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("fnt_ref.stderr", "fnt_ref");
    }
