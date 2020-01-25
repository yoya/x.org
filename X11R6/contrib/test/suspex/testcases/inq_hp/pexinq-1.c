/* $XConsortium: pexinq-1.c,v 1.0 93/11/22 12:48:02 rws Exp $ */

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
 * PEXlib Inquiries Module:  Test Case 1  --  Initialization
 *                           and PEXGetExtensionInfo
 *
 */

test_image(Display         *dpy, 
	   XID              resourceID, 
	   PEXOCRequestType req_type)
    {
    PEXExtensionInfo    *pex_info;
    PEXExtensionInfo   *info_return;
    int                 values[20];

    pex_info = PEXGetExtensionInfo(dpy);
    _hppex_stderr_print("major_version   = %d\n",pex_info->major_version);
    _hppex_stderr_print("minor_version   = %d\n",pex_info->minor_version);
    _hppex_stderr_print("release         = %d\n",pex_info->release);
    _hppex_stderr_print("subset_info     = %d\n",pex_info->subset_info);
    _hppex_stderr_print("vendor_name     = %s\n",pex_info->vendor_name);
    _hppex_stderr_print("\n");
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexinq-1");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "pexinq-1");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("pexinq-1.stderr", "pexinq-1");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nInquiries Test #1 (PEXGetExtensionInfo)\n\n");
    describe_test(" This tests lists PEXGetExtensionInfo output to the stderr\n");
    describe_test(" file.  Results will be implementation dependant.\n\n");

    test_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("pexinq-1.stderr", "pexinq-1");
    }
