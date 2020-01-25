/* $XConsortium: pick_one.c,v 1.0 93/11/22 12:47:44 rws Exp $ */

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
#include <X11/Xlib.h>
#include <X11/PEX5/PEXlib.h>
#include <X11/Xutil.h>
#include <misc.h>

/*
  this test verifies pick one behavior.  It assumes:
      the current path is updated correctly for all OCs;
      pick id correctly updates current path (tested elsewhere);
      namesets work;
  
  pick:
      nothing
      the first primitive (top structure)
      a later primitive (top structure)
      a primitive (very) deep in the hierarchy
      multiple prims, and get the last one hit
      a primitive with a later, undetectable primitive

*/

Window local_win;

void start_pick(Display *display, Window window, PEXRenderer rdr)
{
    PEXPDNPCHitVolume vol = {{0,0,0}, {1,1,1}};
    PEXPickRecord pickRecord;

    pickRecord.volume = vol;

    PEXBeginPickOne(display, window, rdr, 1, PEXPickLast,
		    PEXPickDeviceNPCHitVolume, &pickRecord);
}


test_pick(Display         *display, 
	  XID              rdr, 
	  PEXOCRequestType req_type)
{
    PEXStructure str;
    int status, more, undetect;
    unsigned long count;
    PEXPickPath *path;
    PEXName pick_name[] = {1};
    PEXCoord coord[2] = {{0,0,0}, {1,1,1}};

    {
	PEXRendererAttributes rdr_attrs;
	
	rdr_attrs.clear_image = True;

	rdr_attrs.pick_incl = PEXCreateNameSet(display);
	PEXChangeNameSet(display, rdr_attrs.pick_incl, PEXNSReplace,
			 1, pick_name); 

	PEXChangeRenderer(display, rdr,
			  PEXRAClearImage | PEXRAPickIncl,
			  &rdr_attrs);

	PEXFreeNameSet(display, rdr_attrs.pick_incl);
    }

    /* pick nothing */
    {
	start_pick(display, local_win, rdr);

	PEXPolyline(display, rdr, PEXOCRender, 2, coord);
	
	path = PEXEndPickOne(display, rdr, &status, &undetect);

	if (status != PEXNoPick || undetect != True)
	    _hppex_stderr_print("ERROR - pick nothing failed\n");

	PEXFreePickPaths(0, path);
    }

    /* pick first */
    {
	start_pick(display, local_win, rdr);

	PEXAddToNameSet(display, rdr, PEXOCRender, 1, pick_name);

	PEXPolyline(display, rdr, PEXOCRender, 2, coord);
	
	path = PEXEndPickOne(display, rdr, &status, &undetect);

	if (!(status == PEXPick && undetect == False &&
	      path->count == 1 && path->elements->sid == 1 &&
	      path->elements->offset == 2))
	    _hppex_stderr_print("ERROR - pick first failed\n");

	PEXFreePickPaths(1, path);
    }

    /* pick later */
    {
	int i;

	start_pick(display, local_win, rdr);

	PEXAddToNameSet(display, rdr, PEXOCRender, 1, pick_name);

	for (i = 0; i < 99; i++)
	    PEXPolyline(display, rdr, PEXOCRender, 2, coord);
	
	path = PEXEndPickOne(display, rdr, &status, &undetect);

	if (!(status == PEXPick && undetect == False &&
	      path->count == 1 && path->elements->sid == 1 &&
	      path->elements->offset == 100))
	    _hppex_stderr_print("ERROR - pick later failed\n");

	PEXFreePickPaths(1, path);
    }

    /* pick deep */
    {
	int i, j;
	int errind;

	start_pick(display, local_win, rdr);

	PEXAddToNameSet(display, rdr, PEXOCRender, 1, pick_name);

	for (i = 0; i < 10; i++)
	{
	    PEXBeginStructure(display, rdr, i);
	    for (j = 0; j < 10; j++)
		PEXPolyline(display, rdr, PEXOCRender, 2, coord);
	}
	
	path = PEXEndPickOne(display, rdr, &status, &undetect);

	errind = 0;
	errind |= !(status == PEXPick && undetect == False &&
		    path->count == 11 &&
		    path->elements[0].sid == 1 &&
		    path->elements[0].offset == 2);

	if (!errind)
	{
	    for (i = 0; i < 9; i++)
		errind |= !(path->elements[i + 1].sid == i &&
			    path->elements[i + 1].offset == 11);
	    errind |= !(path->elements[10].sid == 9 &&
			path->elements[10].offset == 10);
	}

	if (errind)
	    _hppex_stderr_print("ERROR - pick deep failed\n");

	PEXFreePickPaths(1, path);
    }

    /* pick undetect */
    {
	start_pick(display, local_win, rdr);

	PEXAddToNameSet(display, rdr, PEXOCRender, 1, pick_name);

	PEXPolyline(display, rdr, PEXOCRender, 2, coord);
	
	PEXRemoveFromNameSet(display, rdr, PEXOCRender, 1, pick_name);

	PEXPolyline(display, rdr, PEXOCRender, 2, coord);
	
	path = PEXEndPickOne(display, rdr, &status, &undetect);

	if (!(status == PEXPick && undetect == True &&
	      path->count == 1 && path->elements->sid == 1 &&
	      path->elements->offset == 2))
	    _hppex_stderr_print("ERROR - pick undetect failed\n");

	PEXFreePickPaths(1, path);
    }

    str = PEXCreateStructure(display);
    PEXAddToNameSet(display, str, PEXOCStore, 1, pick_name);
    PEXPolyline(display, str, PEXOCStore, 2, coord);

    /* mixed mode */
    {
	start_pick(display, local_win, rdr);

	PEXExecuteStructure(display, rdr, PEXOCRender, str);

	path = PEXEndPickOne(display, rdr, &status, &undetect);

	if (!(status == PEXPick && undetect == False &&
	      path->count == 2 &&
	      path->elements[0].sid == 1 &&
	      path->elements[0].offset == 1 &&
	      path->elements[1].sid == str &&
	      path->elements[1].offset == 2))
	    _hppex_stderr_print("ERROR - mixed mode failed\n");

	PEXFreePickPaths(1, path);
    }

    /* dl mode */
    {
	PEXPDNPCHitVolume vol = {{0,0,0}, {1,1,1}};
	PEXPickRecord pickRecord;

	pickRecord.volume = vol;
	path = PEXPickOne(display, local_win, rdr, str, PEXPickLast,
			  PEXPickDeviceNPCHitVolume, &pickRecord,
			  &status, &undetect); 

	if (!(status == PEXPick && undetect == False &&
	      path->count == 1 &&
	      path->elements->sid == str &&
	      path->elements->offset == 2))
	    _hppex_stderr_print("ERROR - dl mode failed");

	PEXFreePickPaths(1, path);
    }
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pick_one");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "pick_one");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("pick_one.stderr", "pick_one");
    local_win = window;
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPick Test (Pick One) \n\n");
    describe_test("  This test verifies pick one behavior.  It assumes:\n");
    describe_test("    the current path is updated correctly for all OCs;\n");
    describe_test("    pick id correctly updates current path (tested elsewhere);\n");
    describe_test("    namesets work;\n");
    describe_test("   \n");
    describe_test("  pick:\n");
    describe_test("    nothing\n");
    describe_test("    the first primitive (top structure)\n");
    describe_test("    a later primitive (top structure)\n");
    describe_test("    a primitive (very) deep in the hierarchy\n");
    describe_test("    multiple prims, and get the last one hit\n");
    describe_test("    a primitive with a later, undetectable primitive\n");
    describe_test("  \n");
    describe_test(" This test is successful if no stderr output is printed.\n\n");

    test_pick(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("pick_one.stderr", "pick_one");
    }
