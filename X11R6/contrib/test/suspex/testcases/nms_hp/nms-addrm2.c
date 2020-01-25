/* $XConsortium: nms-addrm2.c,v 1.0 93/11/22 12:29:25 rws Exp $ */

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
#include <err_handler.h>

Window local_win;

/* 
 * PEXlib Namesets Module:  Test Case  --  Add/Remove Nameset - Errors
 *
 */



test_nameset_errors(Display *dpy, PEXRenderer rid, Window win)
    {
    	PEXNameSet PNS1;
        int action;
    	unsigned long count=10;
        PEXName names[10]= {1,2,3,4,5,6,7,8,9,10};
    	unsigned long item_mask;
    	PEXRendererAttributes rndAttrs;
        unsigned long returnedcount;
        PEXName *returnednames;
        PEXStructure strid;	

    	PNS1 = PEXCreateNameSet(dpy);

    	/* 
    	 * immediate mode 
    	 */

    	count=2;
    	names[0]=1; names[2]=30;

     	/* begin rendering */
    	PEXBeginRendering(dpy,win,rid);
        XSync(dpy,0);

    	_hppex_stderr_print(
    	 "Adding to Nameset with bad renderer(42), expecting a BadRenderer error:\n");
        PEXAddToNameSet(dpy,rid+1, PEXOCRender, count, names);
        XSync(dpy,0);
    	_hppex_stderr_print("Did one appear?\n\n");


    	_hppex_stderr_print(
    	 "Adding to Nameset with a bad renderer(nameset), expecting a BadNameSet error:\n");
        PEXAddToNameSet(dpy,PNS1, PEXOCRender, count, names);
        XSync(dpy,0);
    	_hppex_stderr_print("Did one appear?\n\n");


    	_hppex_stderr_print(
    	 "Adding to  Nameset with a good nameset, expecting no error:\n");
        PEXAddToNameSet(dpy,rid, PEXOCRender, count, names);
        XSync(dpy,0);
    	_hppex_stderr_print("Did one appear?\n\n");
    
    	_hppex_stderr_print(
    	 "Removing from Nameset with bad renderer(42), expecting a BadRenderer error:\n");
        PEXRemoveFromNameSet(dpy,rid+1,PEXOCRender,  count, names);
        XSync(dpy,0);
    	_hppex_stderr_print("Did one appear?\n\n");


    	_hppex_stderr_print(
    	 "Removing from Nameset with a bad renderer(nameset), expecting a BadNameSet error:\n");
        PEXRemoveFromNameSet(dpy,PNS1, PEXOCRender, count, names);
        XSync(dpy,0);
    	_hppex_stderr_print("Did one appear?\n\n");


    	_hppex_stderr_print(
    	 "Removing from Nameset with a good renderer, expecting no error:\n");
        PEXRemoveFromNameSet(dpy,rid, PEXOCRender, count, names);
        XSync(dpy,0);
    	_hppex_stderr_print("Did one appear?\n\n");

        /* end rendering */
        PEXEndRendering(dpy,rid,1);
        XSync(dpy,0);
    
    	/* 
    	 * structures mode 
    	 */

        strid = PEXCreateStructure(dpy);
		
    	_hppex_stderr_print(
    	 "Adding to Nameset with bad structure(42), expecting a BadRenderer error:\n");
        PEXAddToNameSet(dpy,strid+1+1, PEXOCStore, count, names);
        XSync(dpy,0);
    	_hppex_stderr_print("Did one appear?\n\n");


    	_hppex_stderr_print(
    	 "Adding to Nameset with a bad structure(nameset), expecting a BadNameSet error:\n");
        PEXAddToNameSet(dpy,PNS1, PEXOCStore, count, names);
        XSync(dpy,0);
    	_hppex_stderr_print("Did one appear?\n\n");


    	_hppex_stderr_print(
    	 "Adding to  Nameset with a good structure, expecting no error:\n");
        PEXAddToNameSet(dpy,strid, PEXOCStore, count, names);
        XSync(dpy,0);
    	_hppex_stderr_print("Did one appear?\n\n");

    	_hppex_stderr_print(
    	 "Removing from Nameset with bad structure(42), expecting a BadRenderer error:\n");
        PEXRemoveFromNameSet(dpy,strid+1, PEXOCStore, count, names);
        XSync(dpy,0);
    	_hppex_stderr_print("Did one appear?\n\n");


    	_hppex_stderr_print(
    	 "Removing from Nameset with a bad structure(nameset), expecting a BadNameSet error:\n");
        PEXRemoveFromNameSet(dpy,PNS1,PEXOCStore,  count, names);
        XSync(dpy,0);
    	_hppex_stderr_print("Did one appear?\n\n");


    	_hppex_stderr_print(
    	 "Removing from Nameset with a good structure, expecting no error:\n");
        PEXRemoveFromNameSet(dpy,strid, PEXOCStore, count, names);
        XSync(dpy,0);
    	_hppex_stderr_print("Did one appear?\n\n");

    	PEXDestroyStructures(dpy,0,&strid);
    	PEXFreeRenderer(dpy, rid);
        PEXFreeNameSet(dpy,PNS1);
    }

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "nms-addrm2");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "nms-addrm2");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("nms-addrm2.stderr", "nms-addrm2");
    local_win = window;
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nNamesets Add/Remove Nameset (Error Testing)\n\n");
    describe_test("  This test generates stderr output describing errors\n");
    describe_test("  to be expected before they occur.\n\n");
    fflush(stdout);

    test_nameset_errors(dpy, resourceID, local_win);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("nms-addrm2.stderr", "nms-addrm2");
    }
