/* $XConsortium: pick_all.c,v 1.0 93/11/22 12:47:33 rws Exp $ */

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

Window local_win;

/*
  this test verifies pick all behavior.  It assumes:
      the current path is updated correctly for all OCs;
      pick id correctly updates current path (tested elsewhere);
      namesets work;
  
  pick:
      nothing
      the first primitive (top structure)
      a later primitive (top structure)
      multiple prims
      primitives (very) deep in the hierarchy
      more primitives than will fit (MaxHitsReached)

  check destroy notification
  check pick start path in all tests
*/

void start_pick(Display *display, Window window, PEXRenderer rdr)
{
    PEXRendererAttributes rdr_attrs;
    PEXElementRef ele_ref = {123, 456};
    PEXPDNPCHitVolume vol = {{0,0,0}, {1,1,1}};
    PEXPickRecord pickRecord;

    rdr_attrs.pick_start_path.count = 1;
    rdr_attrs.pick_start_path.elements = &ele_ref;

    PEXChangeRenderer(display, rdr, PEXRAPickStartPath,
		      &rdr_attrs); 

    pickRecord.volume = vol;

    PEXBeginPickAll(display, window, rdr, 1, PEXPickAllAll, False, 100,
		    PEXPickDeviceNPCHitVolume, &pickRecord);
}

int check_pick_start_path(Display *display, PEXRenderer rdr, int more, 
			  unsigned long count, PEXPickPath *path)
{
    int i, status;
    PEXRendererAttributes *rdr_attrs;

    rdr_attrs =
	PEXGetRendererAttributes(display, rdr, PEXRAPickStartPath);

    if (more == PEXNoMoreHits)
    {
	status = rdr_attrs->pick_start_path.count == 0;
	PEXFreeRendererAttributes(rdr_attrs);
	return status;
    }
	
    if (rdr_attrs->pick_start_path.count != path[count - 1].count)
    {
	PEXFreeRendererAttributes(rdr_attrs);
	return 0;
    }
    
    status = 1;
    for (i = 0; i < path[count - 1].count; i++)
	status &=
	    (rdr_attrs->pick_start_path.elements[i].structure ==
	     path[count - 1].elements[i].sid &&
	     rdr_attrs->pick_start_path.elements[i].offset ==
	     path[count - 1].elements[i].offset);

    PEXFreeRendererAttributes(rdr_attrs);
    return status;
}


test_pick(Display         *display, 
	  XID              rdr, 
	  PEXOCRequestType req_type)
{
    PEXStructure str;
    PEXExtensionInfo *pex_info;
    int status, more;
    unsigned long count;
    PEXPickPath *path;
    PEXName pick_name[] = {1};
    PEXCoord coord[2] = {{0,0,0}, {1,1,1}};

    pex_info = PEXGetExtensionInfo(display);

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
	
	path = PEXEndPickAll(display, rdr, &status, &more, &count);

	if (!(status == PEXNoPick && count == 0 &&
	      more == PEXNoMoreHits &&
	      check_pick_start_path(display, rdr, more, count, path)))
	    _hppex_stderr_print("ERROR - pick nothing failed\n");

	PEXFreePickPaths(count, path);
    }

    /* pick first */
    {
	start_pick(display, local_win, rdr);

	PEXAddToNameSet(display, rdr, PEXOCRender, 1, pick_name);

	PEXPolyline(display, rdr, PEXOCRender, 2, coord);
	
	path = PEXEndPickAll(display, rdr, &status, &more, &count);

	if (!(status == PEXPick && more == PEXNoMoreHits && count == 1
	      && path->count == 1 && path->elements->sid == 1 &&
	      path->elements->offset == 2 &&
	      check_pick_start_path(display, rdr, more, count, path)))
	    _hppex_stderr_print("ERROR - pick first failed\n");

	PEXFreePickPaths(count, path);
    }

    /* pick later */
    {
	int i;

	start_pick(display, local_win, rdr);

	for (i = 0; i < 99; i++)
	    PEXPolyline(display, rdr, PEXOCRender, 2, coord);

	PEXAddToNameSet(display, rdr, PEXOCRender, 1, pick_name);
	
	PEXPolyline(display, rdr, PEXOCRender, 2, coord);

	path = PEXEndPickAll(display, rdr, &status, &more, &count);

	if (!(status == PEXPick && more == PEXNoMoreHits && count == 1
	      && path->count == 1 && path->elements->sid == 1 &&
	      path->elements->offset == 101 &&
	      check_pick_start_path(display, rdr, more, count, path)))
	    _hppex_stderr_print("ERROR - pick later failed\n");

	PEXFreePickPaths(count, path);
    }

    /* pick multiple */
    {
	int i;
	int errind;

	start_pick(display, local_win, rdr);

	PEXAddToNameSet(display, rdr, PEXOCRender, 1, pick_name);
	
	for (i = 0; i < 99; i++)
	    PEXPolyline(display, rdr, PEXOCRender, 2, coord);

	path = PEXEndPickAll(display, rdr, &status, &more, &count);

	errind = !(status == PEXPick && more == PEXNoMoreHits &&
		   count == 99 &&
		   check_pick_start_path(display, rdr, more, count, path));
	if (!errind)
	    for (i = 0; i < 99; i++)
		errind |= !(path[i].count == 1 &&
			    path[i].elements->sid == 1 &&
			    path[i].elements->offset == i + 2);

	if (errind)
	    _hppex_stderr_print("ERROR - pick multiple failed\n");

	PEXFreePickPaths(count, path);
    }

    /* pick deep */
    {
	int i;
	int errind;

	start_pick(display, local_win, rdr);

	PEXAddToNameSet(display, rdr, PEXOCRender, 1, pick_name);

	for (i = 0; i < 10; i++)
	    PEXBeginStructure(display, rdr, i);

	for (i = 0; i < 99; i++)
	    PEXPolyline(display, rdr, PEXOCRender, 2, coord);
	
	path = PEXEndPickAll(display, rdr, &status, &more, &count);

	errind = !(status == PEXPick && more == PEXNoMoreHits &&
		   count == 99 &&
		   check_pick_start_path(display, rdr, more, count, path));
	if (!errind)
	    for (i = 0; i < 99; i++)
		errind |= !(path[i].count == 11 &&
			    path[i].elements[10].sid == 9 &&
			    path[i].elements[10].offset == i + 1);

	if (errind)
	    _hppex_stderr_print("ERROR - pick deep failed\n");

	PEXFreePickPaths(count, path);
    }

    /* maybe more hits */
    {
	int i;
	int errind;

	start_pick(display, local_win, rdr);

	PEXAddToNameSet(display, rdr, PEXOCRender, 1, pick_name);
	
	for (i = 0; i < 100; i++)
	    PEXPolyline(display, rdr, PEXOCRender, 2, coord);

	path = PEXEndPickAll(display, rdr, &status, &more, &count);

	errind = !(status == PEXPick && more == PEXMayBeMoreHits &&
		   count == 100 &&
		   check_pick_start_path(display, rdr, more, count, path));
	if (!errind)
	    for (i = 0; i < 100; i++)
		errind |= !(path[i].count == 1 &&
			    path[i].elements->sid == 1 &&
			    path[i].elements->offset == i + 2);

	if (errind)
	    _hppex_stderr_print("ERROR - maybe more hits failed\n");

	PEXFreePickPaths(count, path);
    }

    /* more hits, with max hits reached */
    {
	int i;
	int errind;
	int max_hits_sent, sent_offset;

	{
	    PEXRendererAttributes rdr_attrs;
	    PEXElementRef ele_ref = {123, 456};
	    PEXPDNPCHitVolume vol = {{0,0,0}, {1,1,1}};
	    PEXPickRecord pickRecord;

	    rdr_attrs.pick_start_path.count = 1;
	    rdr_attrs.pick_start_path.elements = &ele_ref;

	    PEXChangeRenderer(display, rdr, PEXRAPickStartPath,
			      &rdr_attrs); 

	    pickRecord.volume = vol;

	    PEXBeginPickAll(display, local_win, rdr, 1, PEXPickAllAll,
			    True, 100, PEXPickDeviceNPCHitVolume,
			    &pickRecord); 
	}

	PEXAddToNameSet(display, rdr, PEXOCRender, 1, pick_name);

	XSynchronize(display, True);
	
	max_hits_sent = 0;
	for (i = 0; i < 110; i++)
	{
	    XEvent ev;

	    PEXPolyline(display, rdr, PEXOCRender, 2, coord);
	    if (XEventsQueued(display, QueuedAfterReading))
	    {
		XNextEvent(display, &ev);
		if (ev.type == pex_info->first_event)
		{
		    max_hits_sent++;
		    sent_offset = i;
		}
	    }
	}

	XSynchronize(display, False);

	path = PEXEndPickAll(display, rdr, &status, &more, &count);

	errind = !(status == PEXPick && more == PEXMoreHits &&
		   count == 100 && max_hits_sent == 1 &&
		   sent_offset == 99 &&
		   check_pick_start_path(display, rdr, more, count, path));
	if (!errind)
	    for (i = 0; i < 100; i++)
		errind |= !(path[i].count == 1 &&
			    path[i].elements->sid == 1 &&
			    path[i].elements->offset == i + 2);

	if (errind)
	    _hppex_stderr_print("ERROR - more hits failed\n");

	PEXFreePickPaths(count, path);
    }

    str = PEXCreateStructure(display);
    PEXAddToNameSet(display, str, PEXOCStore, 1, pick_name);
    PEXPolyline(display, str, PEXOCStore, 2, coord);

    /* mixed mode */
    {
	start_pick(display, local_win, rdr);

	PEXExecuteStructure(display, rdr, PEXOCRender, str);

	path = PEXEndPickAll(display, rdr, &status, &more, &count);

	if (!(status == PEXPick && more == PEXNoMoreHits &&
	      check_pick_start_path(display, rdr, more, count, path) &&
	      count == 1 &&
	      path->count == 2 &&
	      path->elements[0].sid == 1 &&
	      path->elements[0].offset == 1 &&
	      path->elements[1].sid == str &&
	      path->elements[1].offset == 2))
	    _hppex_stderr_print("ERROR - mixed mode failed\n");

	PEXFreePickPaths(0, path);
    }

    /* dl mode */
    {
	PEXRendererAttributes rdr_attrs;
	PEXElementRef ele_ref;
	PEXPDNPCHitVolume vol = {{0,0,0}, {1,1,1}};
	PEXPickRecord pickRecord;

	ele_ref.structure = str;
	ele_ref.offset = 0;
	rdr_attrs.pick_start_path.count = 1;
	rdr_attrs.pick_start_path.elements = &ele_ref;

	PEXChangeRenderer(display, rdr, PEXRAPickStartPath,
			  &rdr_attrs); 

	pickRecord.volume = vol;
	path = PEXPickAll(display, local_win, rdr, PEXPickAllAll,
			  100, 
			  PEXPickDeviceNPCHitVolume, &pickRecord,
			  &status, &more, &count);

	if (!(status == PEXPick && more == PEXNoMoreHits &&
	      check_pick_start_path(display, rdr, more, count, path) &&
	      count == 1 &&
	      path->count == 1 &&
	      path->elements->sid == str &&
	      path->elements->offset == 2))
	    _hppex_stderr_print("ERROR - dl mode failed");

	PEXFreePickPaths(0, path);
    }

    /* dl with max hits */
    {
	PEXRendererAttributes rdr_attrs;
	PEXElementRef ele_ref;
	PEXPDNPCHitVolume vol = {{0,0,0}, {1,1,1}};
	PEXPickRecord pickRecord;

	ele_ref.structure = str;
	ele_ref.offset = 0;
	rdr_attrs.pick_start_path.count = 1;
	rdr_attrs.pick_start_path.elements = &ele_ref;

	PEXChangeRenderer(display, rdr, PEXRAPickStartPath,
			  &rdr_attrs); 

	pickRecord.volume = vol;
	path = PEXPickAll(display, local_win, rdr, PEXPickAllAll,
			  1,
			  PEXPickDeviceNPCHitVolume, &pickRecord,
			  &status, &more, &count);

	if (!(status == PEXPick && more == PEXMayBeMoreHits &&
	      check_pick_start_path(display, rdr, more, count, path) &&
	      count == 1 &&
	      path->count == 1 &&
	      path->elements->sid == str &&
	      path->elements->offset == 2))
	    _hppex_stderr_print("ERROR - dl mode failed");

	PEXFreePickPaths(0, path);
    }
}


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pick_all");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "pick_all");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("pick_all.stderr", "pick_all");
    local_win = window;
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPick Test #1 \n\n");
    describe_test("This test verifies pick all behavior.  It assumes:\n");
    describe_test("  the current path is updated correctly for all OCs;\n");
    describe_test("  pick id correctly updates current path (tested elsewhere);\n");
    describe_test("  namesets work;\n");
    describe_test(" \n");
    describe_test("pick:\n");
    describe_test("  nothing\n");
    describe_test("  the first primitive (top structure)\n");
    describe_test("  a later primitive (top structure)\n");
    describe_test("  multiple prims\n");
    describe_test("  primitives (very) deep in the hierarchy\n");
    describe_test("  more primitives than will fit (MaxHitsReached)\n");
    describe_test("\n");
    describe_test("check destroy notification\n");
    describe_test("check pick start path in all tests\n\n");
    describe_test("This test is successful if no stderr output is printed.\n\n");

    test_pick(dpy, resourceID, req_type);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("pick_all.stderr", "pick_all");
    }
