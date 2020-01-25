/* $XConsortium: trv_err.c,v 1.0 93/11/22 12:40:35 rws Exp $ */

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
#include <X11/Xutil.h>
#include <misc.h>
#include <err_handler.h>

Window local_win;

trv_err_test(Display   *display, 
	    PEXRenderer rid, 
	    Window      window)
    {
    PEXStructure structs[5];

    /*
        Render all of the elements in the root structure.  
    */

    PEXBeginRendering(display, window, rid);

    /**** Check rendering errors. ****/

    /* Renderer is not a resource. */
    _hppex_test_set_expected_error(PEX_ERR, BadPEXRenderer, 
         PEXRCRenderOutputCommands, 0, PEXOCExecuteStructure);
    PEXExecuteStructure(display, 0, PEXOCRender, structs[0]); 
    XSync(display, 0);
    _hppex_test_check_expected_error(PEX_ERR|REQUEST_CODE);

    /* Executed structure is not a resource. */
    _hppex_test_set_expected_error(PEX_ERR, BadPEXOutputCommand,
         PEXRCRenderOutputCommands, 0, PEXOCExecuteStructure);
    PEXExecuteStructure(display, rid, PEXOCRender, structs[1]); 
    XSync(display, 0);
    _hppex_test_check_expected_error(PEX_ERR|REQUEST_CODE|OC_CODE);

    _hppex_test_set_expected_error(PEX_ERR, BadPEXRenderer,
         PEXRCRenderOutputCommands, 0, PEXOCExecuteStructure);
    /* Renderer is not a renderer. */
    PEXExecuteStructure(display, structs[0], PEXOCRender, structs[1]); 
    XSync(display, 0);
    _hppex_test_check_expected_error(PEX_ERR|REQUEST_CODE);

    /*** Now try to store the element. ****/

    /* Structure is not a resource. */
    _hppex_test_set_expected_error(PEX_ERR, BadPEXStructure,
        PEXRCStoreElements, 0, PEXOCExecuteStructure);
    PEXExecuteStructure(display, structs[1], PEXOCStore, structs[5]); 
    XSync(display, 0);
    _hppex_test_check_expected_error(PEX_ERR|REQUEST_CODE);

    /* Executed Structure is not a resource.*/
    _hppex_test_set_expected_error(PEX_ERR, BadPEXOutputCommand,
         PEXRCStoreElements, 0, PEXOCExecuteStructure);
    PEXExecuteStructure(display, structs[0], PEXOCStore, structs[1]); 
    XSync(display, 0);
   _hppex_test_check_expected_error(PEX_ERR|REQUEST_CODE|OC_CODE);

    /* Structure is not a structure. */
    _hppex_test_set_expected_error(PEX_ERR, BadPEXStructure,
         PEXRCStoreElements, 0, PEXOCExecuteStructure);
    /* Structure is not a structure. */
    PEXExecuteStructure(display, rid, PEXOCStore, structs[1]);
    XSync(display, 0);
   _hppex_test_check_expected_error(PEX_ERR|REQUEST_CODE);

    PEXEndRendering(display, rid, False);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "trv_err");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "trv_err");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    local_win = window;
    _hppex_set_stderr("trv_err.stderr", "trv_err");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nTraversal Errors Test\n\n");
    describe_test("   This test traps intentional traversal errors and \n");
    describe_test("   should print nothing to the stderr file if successful\n\n");

    trv_err_test(dpy, resourceID, local_win);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("trv_err.stderr", "trv_err");
    }
