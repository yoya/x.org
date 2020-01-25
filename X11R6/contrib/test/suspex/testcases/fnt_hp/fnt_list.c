/* $XConsortium: fnt_list.c,v 1.0 93/11/22 12:27:47 rws Exp $ */

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


struct
{
    char *descr;
    char *pattern;
    unsigned int max_names;
} test[] =
{
    {"match all names",	"*",			1000},
    {"match simple",	"*-medium-*",		1000}, 
    {"match simple",	"*-medium-*-hp-roman8",	1000}, 
    {"match simple",	"-*-medium-*",		1000}, 
    {"match none",	"*-medium-*-accel-*",	1000},
    {"match exact",
	 "-hp-PEX stick-medium-r-normal-normal-0-0-0-0-m-0-hp-roman8",
	 1000},
    {"match with ?",	"*-medium-?-normal-*",	1000},
    {"redundant '*'",	"**",			1000},
    {"* matches nothing", "*-hp-roman8*",	1000},
    {"* matches nothing", "*-hp-PEX stick-*",	1000},
    {"limit to max",	"*",			1},
    {"limit to none",	"*",			0}
};
    
const unsigned int num_matches = 12;

void font_info(Display *display)
{
    int i;

    for (i = 0; i < num_matches; i++)
    {
	unsigned long count;
	char **names =
	    PEXListFonts(display, test[i].pattern,
			 test[i].max_names, &count);
	int j;

	_hppex_stderr_print("%s\n", test[i].descr);
	_hppex_stderr_print(
		"pattern = \"%s\", max_names = %d, count = %d\n",
		test[i].pattern, test[i].max_names, count);
	for (j = 0; j < count; j++)
	    _hppex_stderr_print("\t%s\n", names[j]);
	_hppex_stderr_print("\n");

	PEXFreeFontNames(count, names);
    }
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "fnt_list");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "fnt_list");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("fnt_list.stderr", "fnt_list");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPEXListFonts Test\n\n");
    describe_test("  The stderr file (only printed with the -v option) will contain\n");
    describe_test("  selected output from PEXListFonts\n\n");
    fflush(stdout);

    font_info(dpy);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("fnt_list.stderr", "fnt_list");
    }
