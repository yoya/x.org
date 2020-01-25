/* $XConsortium: pexplc-6.c,v 1.0 93/11/22 12:38:54 rws Exp $ */

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
 * PEXlib Pipeline Context Module:  Test Case 6  --  PEXFreePCAttributes
 *
 */

test_image(Display         *dpy,
	   XID              resourceID,
           PEXOCRequestType req_type)
    {
      unsigned long			item_mask = 0;
      PEXCoord				points[5], mpoints[5], fpoints[5];
      unsigned long			valuemask[3];
      PEXPipelineContext                plc;
      PEXPCAttributes			pc_values, *pc_values2 = NULL;

      /* Create a pipeline context and get the values of it. */

      valuemask[0] = 0;
      valuemask[1] = 0;
      valuemask[2] = 0;

      plc = PEXCreatePipelineContext(dpy, valuemask, &pc_values);

      valuemask[0] = 0xFFFFFFFF;
      valuemask[1] = 0xFFFFFFFF;
      pc_values2 = PEXGetPipelineContext(dpy, plc, valuemask);

      /* Now free the attributes */
      PEXFreePCAttributes(pc_values2);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexplc-6");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "pexplc-6");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("pexplc-6.stderr", "pexplc-6");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPipeline Context Test #6 (PEXFreePCAttributes)\n\n");
    describe_test(" This test produces only an empty stderr output file\n");
    describe_test(" if correct. \n\n");

    test_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("pexplc-6.stderr", "pexplc-6");
    }
