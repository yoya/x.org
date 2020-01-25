/* $XConsortium: pexinq-3.c,v 1.0 93/11/22 12:48:13 rws Exp $ */

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
 * PEXlib Inquiries Module:  Test Case 3  --  PEXGetImpDepConstants
 *
 */

    
Window local_win;

test_image(Display         *dpy, 
	   XID              resourceID, 
	   PEXOCRequestType req_type)
    {
    Status              status = 0;
    PEXImpDepConstant   *imp_dep_constants;
    unsigned short      imp_dep_names[3];
    PEXExtensionInfo    *info_return;
    int                 values[20];

    imp_dep_names[0] = PEXIDMaxMarkerSize;
    imp_dep_names[1] = PEXIDChromaticityGreenV;
    imp_dep_names[2] = PEXIDMaxHitsEventSupported;
    status = 0;
    status = PEXGetImpDepConstants(dpy, local_win, 3, imp_dep_names, &imp_dep_constants);

    if (status == 0)
      _hppex_stderr_print("ERROR - Zero Status from PEXGetImpDepConstants()\n");

    _hppex_stderr_print("MaxMarkerSize         = %d\n", 
			imp_dep_constants[0].integer);
    _hppex_stderr_print("ChromaticityGreenV    = %f\n", 
			imp_dep_constants[1].flt_point);
    _hppex_stderr_print("MAXHitsEventSupported = %d\n", 
			imp_dep_constants[2].integer);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexinq-3");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "pexinq-3");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("pexinq-3.stderr", "pexinq-3");
    local_win = window;
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nInquiries Test #3 (PEXGetImpDepConstants)\n\n");
    describe_test("  This test queries and prints to stderr constant info for\n");
    describe_test("  PEXIDMaxMarkerSize, PEXIDChromaticityGreenV, and,\n");
    describe_test("  PEXIDMaxHitsEventSupported.  Output will be implementation dependant.\n\n");

    test_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("pexinq-3.stderr", "pexinq-3");
    }
