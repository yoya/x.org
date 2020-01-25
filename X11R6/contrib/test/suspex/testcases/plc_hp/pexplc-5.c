/* $XConsortium: pexplc-5.c,v 1.0 93/11/22 12:38:48 rws Exp $ */

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
 * PEXlib Pipeline Context Module:  Test Case 5  --  PEXFreePipelineContext
 *
 */

test_image(Display         *dpy,
	   XID              resourceID,
           PEXOCRequestType req_type)
    {
      PEXRendererAttributes		rndAttrs, *rndAttrs2;
      unsigned long			item_mask = 0;
      PEXCoord				points[5], mpoints[5], fpoints[5];
      unsigned long			valuemask[3];
      PEXPipelineContext                plc;
      PEXPCAttributes			pc_values, *pc_values2 = NULL;

      /* Test the initialization of the default Pipeline Context by */
      /* exercising the Renderer's pointer to it.                   */

      valuemask[0] = 0;
      valuemask[1] = 0;
      valuemask[2] = 0;

      /* First a simple free test. */
      plc = PEXCreatePipelineContext(dpy, valuemask, &pc_values);
      PEXFreePipelineContext(dpy, plc);

      valuemask[0] = 0x0000000F;
      /* Now see if we can get the plc - this should generate an error. */
      _hppex_stderr_print("pexplc-5: Expecting an error message - BadPEXPipelineContext\n\n");
      pc_values2 = PEXGetPipelineContext(dpy, plc, valuemask);

      if (pc_values2 != NULL)
        _hppex_stderr_print("ERROR 1 in PEXlib test pexplc-5:  PEXFreePipelineContext()\n");

      /* Now, a more elaborate free test. */
      valuemask[0] = 0;
      plc = PEXCreatePipelineContext(dpy, valuemask, &pc_values);

      item_mask = (PEXRAPipelineContext | PEXRABackgroundColor | PEXRAClearImage);
      rndAttrs.pipeline_context = plc; 
      rndAttrs.background_color.type = PEXColorTypeRGB;
      rndAttrs.background_color.value.rgb.red = 0.0;
      rndAttrs.background_color.value.rgb.green = 0.0;
      rndAttrs.background_color.value.rgb.blue = 0.0;
      rndAttrs.clear_image = True;

      PEXChangeRenderer(dpy, resourceID, item_mask, &rndAttrs);
      PEXFreePipelineContext(dpy, plc);

      rndAttrs2 = PEXGetRendererAttributes(dpy, resourceID, item_mask);
      if (rndAttrs2->pipeline_context != PEXAlreadyFreed)
        _hppex_stderr_print("ERROR 2 in PEXlib test pexplc-5:  PEXFreePipelineContext()\n");
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexplc-5");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "pexplc-5");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("pexplc-5.stderr", "pexplc-5");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPipeline Context Test #5 (PEXFreePipelineContext)\n\n");
    describe_test(" This tests freeing the pipeline context and if \n");
    describe_test(" correct, generates an expected error message to the\n");
    describe_test(" stderr file. \n\n");

    test_image(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("pexplc-5.stderr", "pexplc-5");
    }
