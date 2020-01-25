/* $XConsortium: pexinq-4.c,v 1.0 93/11/22 12:48:19 rws Exp $ */

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

#include <X11/PEX5/PEXlib.h>
#include <misc.h>

/* 
 * PEXlib Inquiries Module:  Test Case 4  --  PEXGetProtocolFloatFormat
 *
 */

Window local_win;

test_image(Display         *dpy, 
	   XID              resourceID, 
	   PEXOCRequestType req_type)
    {
    int format;
    static char *mnemonics[]={"IEEE_754_32",
			      "DEC_F_Floating",
			      "IEEE_754_64",
			      "DEC_D_Floating"};

    format = PEXGetProtocolFloatFormat(dpy);

    _hppex_stderr_print("PEXGetProtocolFloatFormat returned %d", format);
    if ((format < 1) || (format > 3))
	_hppex_stderr_print(" (Unknown Type)\n\n");
    else
	_hppex_stderr_print(" (%s)\n\n", mnemonics[format-1]);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexinq-4");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "pexinq-4");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("pexinq-4.stderr", "pexinq-4");
    local_win = window;
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nInquiries Test #4 (PEXGetProtocolFloatFormat)\n\n");
    describe_test("  This test queries and prints to stderr the value \n");
    describe_test("  returned from PEXGetProtocolFloatFormat.\n\n");

    test_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("pexinq-4.stderr", "pexinq-4");
    }
