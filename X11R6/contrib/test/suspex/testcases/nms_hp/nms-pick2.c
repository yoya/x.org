/* $XConsortium: nms-pick2.c,v 1.0 93/11/22 12:30:06 rws Exp $ */

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

Window local_win;

/* 
 * PEXlib Namesets Module:  Test Case ??  --  Pick filter functioning
 *											  in structure mode!!!!
 *
 */

test_pick(Display *dpy, PEXRenderer rid, Window win)
    {
      PEXStructure strid;
      PEXNameSet incl_filter, excl_filter;
      PEXRendererAttributes rndAttrs;
      unsigned long item_mask;
      PEXName names[3];
      unsigned long count;
      PEXRendererAttributes rdr_attrs;
      PEXPickRecord pick_record;
      PEXPickPath *pick_path;
      PEXPDNPCHitVolume vol = {{0,0,0}, {1,1,1}};
      PEXPickRecord pickRecord;
      int status_return, more_return;
      unsigned long count_return;
      
      PEXElementRef ele_ref;

      /* create the filter namesets */
      incl_filter = PEXCreateNameSet(dpy);
      excl_filter = PEXCreateNameSet(dpy);
      count =1; names[0]=1;
      PEXChangeNameSet(dpy, incl_filter, PEXNSAdd, count, names);
      PEXChangeNameSet(dpy, excl_filter, PEXNSAdd, count, names);

      /* create the structure */
      strid = PEXCreateStructure(dpy);
      /* fill the structure */
      draw_stuff(dpy, strid);


      rdr_attrs.pick_start_path.count = 1;
      ele_ref.structure = strid;
      ele_ref.offset = 0;
      rdr_attrs.pick_start_path.elements = &ele_ref;
      PEXChangeRenderer(dpy, rid, PEXRAPickStartPath,
		      &rdr_attrs); 

      pickRecord.volume = vol;

      _hppex_stderr_print("Notes:\n\t Namset 1 contains a red polyline.\n");
      _hppex_stderr_print("\t Namset 2 contains a green polyline.\n");
      _hppex_stderr_print("\t Namset 3 contains a blue polyline.\n");
      _hppex_stderr_print("\t Expected ouput will be of the form:\n");
      _hppex_stderr_print("\t\tincl=<included sets>, excl=<excluded sets>, number of picks\n");
      _hppex_stderr_print("\n");

      /* 
       * Image 1: print image with no filters set
       */


      /* print the image with no filters set */
      _hppex_stderr_print("Image 1:\n");
      _hppex_stderr_print("\t\tincl=NULL, excl=NULL, expecting NO picks\n");
      pick_path = PEXPickAll(dpy, win, rid, PEXPickAllAll, 100, 
			     PEXPickDeviceNPCHitVolume, &pickRecord,
			      &status_return,&more_return, &count_return);
      if (status_return == PEXPick)
        {
	  _hppex_stderr_print("\tPicked %d primitives.\n", count_return);
        }
      else
        {
	  _hppex_stderr_print("\tPicked NO primitives.\n");
        }
      PEXFreePickPaths(count_return, pick_path);

      /* 
       * Image 2: print image with inclusion filter = [1]
       *                           exclusion filter = []
       */

      /* set filters */
      rdr_attrs.pick_start_path.count = 1;
      ele_ref.structure = strid;
      ele_ref.offset = 0;
      rdr_attrs.pick_start_path.elements = &ele_ref;
      PEXChangeRenderer(dpy, rid, PEXRAPickStartPath,
		      &rdr_attrs); 

      pickRecord.volume = vol;


      /* Change filter to set names */
      count =1; names[0]=1;
      PEXChangeNameSet(dpy, incl_filter, PEXNSReplace, count, names);
      rndAttrs.pick_incl = incl_filter;
      item_mask = 0;
      item_mask = PEXRAPickIncl;
      PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
      _hppex_stderr_print("Image 2:\n");
      _hppex_stderr_print("\t\tincl=[1], excl=NULL, expecting 1 pick\n");
      pick_path = PEXPickAll(dpy, win, rid, PEXPickAllAll, 100, 
			     PEXPickDeviceNPCHitVolume, &pickRecord,
			      &status_return,&more_return, &count_return);
      if (status_return == PEXPick)
        {
	  _hppex_stderr_print("\tPicked %d primitives.\n", count_return);
        }
      else
        {
	  _hppex_stderr_print("\tPicked NO primitives.\n");
        }
      PEXFreePickPaths(count_return, pick_path);

      /* 
       * Image 3: print image with inclusion filter = [1,2,3]
       *                           exclusion filter = []
       */

      /* set filters */
      rdr_attrs.pick_start_path.count = 1;
      ele_ref.structure = strid;
      ele_ref.offset = 0;
      rdr_attrs.pick_start_path.elements = &ele_ref;
      PEXChangeRenderer(dpy, rid, PEXRAPickStartPath,
		      &rdr_attrs); 

      pickRecord.volume = vol;


      /* Change filter to set names */
      count =3; names[0]=1; names[1]=2; names[2]=3;
      PEXChangeNameSet(dpy, incl_filter, PEXNSReplace, count, names);
      rndAttrs.pick_incl = incl_filter;
      item_mask = 0;
      item_mask = PEXRAPickIncl;
      PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
      _hppex_stderr_print("Image 3:\n");
      _hppex_stderr_print("\t\tincl=[1,2,3], excl=NULL, expecting 3 picks\n");
      pick_path = PEXPickAll(dpy, win, rid, PEXPickAllAll, 100, 
			     PEXPickDeviceNPCHitVolume, &pickRecord,
			      &status_return,&more_return, &count_return);
      if (status_return == PEXPick)
        {
	  _hppex_stderr_print("\tPicked %d primitives.\n", count_return);
        }
      else
        {
	  _hppex_stderr_print("\tPicked NO primitives.\n");
        }
      PEXFreePickPaths(count_return, pick_path);

      /* 
       * Image 4: print image with inclusion filter = [1,3]
       *                           exclusion filter = [2]
       */

      /* set filters */
      rdr_attrs.pick_start_path.count = 1;
      ele_ref.structure = strid;
      ele_ref.offset = 0;
      rdr_attrs.pick_start_path.elements = &ele_ref;
      PEXChangeRenderer(dpy, rid, PEXRAPickStartPath,
		      &rdr_attrs); 

      pickRecord.volume = vol;


      /* Change filter to set names */
      count =2; names[0]=1; names[1]=3; 
      PEXChangeNameSet(dpy, incl_filter, PEXNSReplace, count, names);
      count =1; names[0]=2; 
      PEXChangeNameSet(dpy, excl_filter, PEXNSReplace, count, names);
      rndAttrs.pick_incl = incl_filter;
      rndAttrs.pick_excl = excl_filter;
      item_mask = 0;
      item_mask = PEXRAPickIncl|PEXRAPickExcl;
      PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
      _hppex_stderr_print("Image 4:\n");
      _hppex_stderr_print("\t\tincl=[1,3], excl=[2], expecting 2 picks\n");
      pick_path = PEXPickAll(dpy, win, rid, PEXPickAllAll, 100, 
			     PEXPickDeviceNPCHitVolume, &pickRecord,
			      &status_return,&more_return, &count_return);
      if (status_return == PEXPick)
        {
	  _hppex_stderr_print("\tPicked %d primitives.\n", count_return);
        }
      else
        {
	  _hppex_stderr_print("\tPicked NO primitives.\n");
        }
      PEXFreePickPaths(count_return, pick_path);


      /* 
       * Image 5: print image with inclusion filter = [1,2,3]
       *                           exclusion filter = [1]
       */

      /* set filters */
      rdr_attrs.pick_start_path.count = 1;
      ele_ref.structure = strid;
      ele_ref.offset = 0;
      rdr_attrs.pick_start_path.elements = &ele_ref;
      PEXChangeRenderer(dpy, rid, PEXRAPickStartPath,
		      &rdr_attrs); 

      pickRecord.volume = vol;


      /* Change filter to set names */
      count =3; names[0]=1; names[1]=2; names[2]=3; 
      PEXChangeNameSet(dpy, incl_filter, PEXNSReplace, count, names);
      count =1; names[0]=1; 
      PEXChangeNameSet(dpy, excl_filter, PEXNSReplace, count, names);
      rndAttrs.pick_incl = incl_filter;
      rndAttrs.pick_excl = excl_filter;
      item_mask = 0;
      item_mask = PEXRAPickIncl|PEXRAPickExcl;
      PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
      _hppex_stderr_print("Image 5:\n");
      _hppex_stderr_print("\t\tincl=[1,2,3], excl=[1],  expecting 2 picks\n");
      pick_path = PEXPickAll(dpy, win, rid, PEXPickAllAll, 100, 
			     PEXPickDeviceNPCHitVolume, &pickRecord,
			      &status_return,&more_return, &count_return);
      if (status_return == PEXPick)
        {
	  _hppex_stderr_print("\tPicked %d primitives.\n", count_return);
        }
      else
        {
	  _hppex_stderr_print("\tPicked NO primitives.\n");
        }
      PEXFreePickPaths(count_return, pick_path);
   

      /* 
       * Image 6: print image with inclusion filter = [1,2,3]
       *                           exclusion filter = [1,2,3]
       */

      /* set filters */
      rdr_attrs.pick_start_path.count = 1;
      ele_ref.structure = strid;
      ele_ref.offset = 0;
      rdr_attrs.pick_start_path.elements = &ele_ref;
      PEXChangeRenderer(dpy, rid, PEXRAPickStartPath,
		      &rdr_attrs); 

      pickRecord.volume = vol;


      /* Change filter to set names */
      count =3; names[0]=1; names[1]=2; names[2]=3; 
      PEXChangeNameSet(dpy, incl_filter, PEXNSReplace, count, names);
      PEXChangeNameSet(dpy, excl_filter, PEXNSReplace, count, names);
      rndAttrs.pick_incl = incl_filter;
      rndAttrs.pick_excl = excl_filter;
      item_mask = 0;
      item_mask = PEXRAPickIncl|PEXRAPickExcl;
      PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
      _hppex_stderr_print("Image 6:\n");
      _hppex_stderr_print("\t\tincl=[1,2,3], excl=[1,2,3],  expecting NO picks\n");
      pick_path = PEXPickAll(dpy, win, rid, PEXPickAllAll, 100, 
			     PEXPickDeviceNPCHitVolume, &pickRecord,
			      &status_return,&more_return, &count_return);
      if (status_return == PEXPick)
        {
	  _hppex_stderr_print("\tPicked %d primitives.\n", count_return);
        }
      else
        {
	  _hppex_stderr_print("\tPicked NO primitives.\n");
        }
      PEXFreePickPaths(count_return, pick_path);

      /* 
       * Image 7: print image with inclusion filter = []
       *                           exclusion filter = [1,2,3]
       */

      /* set filters */
      rdr_attrs.pick_start_path.count = 1;
      ele_ref.structure = strid;
      ele_ref.offset = 0;
      rdr_attrs.pick_start_path.elements = &ele_ref;
      PEXChangeRenderer(dpy, rid, PEXRAPickStartPath,
		      &rdr_attrs); 

      pickRecord.volume = vol;


      /* Change filter to set names */
      count =3; names[0]=1; names[1]=2; names[2]=3; 
      PEXChangeNameSet(dpy, incl_filter, PEXNSRemove, count, names);
      PEXChangeNameSet(dpy, excl_filter, PEXNSReplace, count, names);
      rndAttrs.pick_incl = incl_filter;
      rndAttrs.pick_excl = excl_filter;
      item_mask = 0;
      item_mask = PEXRAPickIncl|PEXRAPickExcl;
      PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
      _hppex_stderr_print("Image 7:\n");
      _hppex_stderr_print("\t\tincl=NUL, excl=[1,2,3],  expecting NO picks\n");
      pick_path = PEXPickAll(dpy, win, rid, PEXPickAllAll, 100, 
			     PEXPickDeviceNPCHitVolume, &pickRecord,
			      &status_return,&more_return, &count_return);
      if (status_return == PEXPick)
        {
	  _hppex_stderr_print("\tPicked %d primitives.\n", count_return);
        }
      else
        {
	  _hppex_stderr_print("\tPicked NO primitives.\n");
        }
      PEXFreePickPaths(count_return, pick_path);
   

      /* 
       * Image 8: print image with inclusion filter = [1,2,3]
       *                           exclusion filter = []
       */

      /* set filters */
      rdr_attrs.pick_start_path.count = 1;
      ele_ref.structure = strid;
      ele_ref.offset = 0;
      rdr_attrs.pick_start_path.elements = &ele_ref;
      PEXChangeRenderer(dpy, rid, PEXRAPickStartPath,
		      &rdr_attrs); 

      pickRecord.volume = vol;


      /* Change filter to set names */
      count =3; names[0]=1; names[1]=2; names[2]=3; 
      PEXChangeNameSet(dpy, incl_filter, PEXNSReplace, count, names);
      PEXChangeNameSet(dpy, excl_filter, PEXNSRemove, count, names);
      rndAttrs.pick_incl = incl_filter;
      rndAttrs.pick_excl = excl_filter;
      item_mask = 0;
      item_mask = PEXRAPickIncl|PEXRAPickExcl;
      PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
      _hppex_stderr_print("Image 8:\n");
      _hppex_stderr_print("\t\tincl=[1,2,3], excl=NULL,  expecting 3 picks\n");
      pick_path = PEXPickAll(dpy, win, rid, PEXPickAllAll, 100, 
			     PEXPickDeviceNPCHitVolume, &pickRecord,
			      &status_return,&more_return, &count_return);
      if (status_return == PEXPick)
        {
	  _hppex_stderr_print("\tPicked %d primitives.\n", count_return);
        }
      else
        {
	  _hppex_stderr_print("\tPicked NO primitives.\n");
        }
      PEXFreePickPaths(count_return, pick_path);
   

      /* 
       * Image 9: print image with inclusion filter = []
       *                           exclusion filter = []
       */

      /* set filters */
      rdr_attrs.pick_start_path.count = 1;
      ele_ref.structure = strid;
      ele_ref.offset = 0;
      rdr_attrs.pick_start_path.elements = &ele_ref;
      PEXChangeRenderer(dpy, rid, PEXRAPickStartPath,
		      &rdr_attrs); 

      pickRecord.volume = vol;


      /* Change filter to set names */
      count =3; names[0]=1; names[1]=2; names[2]=3; 
      PEXChangeNameSet(dpy, incl_filter, PEXNSRemove, count, names);
      PEXChangeNameSet(dpy, excl_filter, PEXNSRemove, count, names);
      rndAttrs.pick_incl = incl_filter;
      rndAttrs.pick_excl = excl_filter;
      item_mask = 0;
      item_mask = PEXRAPickIncl|PEXRAPickExcl;
      PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
      _hppex_stderr_print("Image 9:\n");
      _hppex_stderr_print("\t\tincl=NULL, excl=NULL,  expecting NO picks\n");
      pick_path = PEXPickAll(dpy, win, rid, PEXPickAllAll, 100, 
			     PEXPickDeviceNPCHitVolume, &pickRecord,
			      &status_return,&more_return, &count_return);
      if (status_return == PEXPick)
        {
	  _hppex_stderr_print("\tPicked %d primitives.\n", count_return);
        }
      else
        {
	  _hppex_stderr_print("\tPicked NO primitives.\n");
        }
      PEXFreePickPaths(count_return, pick_path);

      PEXDestroyStructures(dpy,0,&strid);
      PEXFreeRenderer(dpy,rid);
      PEXFreeNameSet(dpy,incl_filter);
      PEXFreeNameSet(dpy,excl_filter);

    }

draw_stuff(Display *dpy, PEXStructure strid)
{
    PEXCoord line1pts[3];
    PEXCoord line2pts[3];
    PEXCoord line3pts[3];
    PEXColor reddish, greenish, blueish;
    PEXName names[3];

    /* set points for polylines */

    line1pts[0].x=0.1;    line1pts[0].y=0.7;    line1pts[0].z=0.0;
    line1pts[1].x=0.7;    line1pts[1].y=0.7;    line1pts[1].z=0.0;
    line1pts[2].x=0.7;    line1pts[2].y=0.9;    line1pts[2].z=0.0;

    line2pts[0].x=0.1;    line2pts[0].y=0.4;    line2pts[0].z=0.0;
    line2pts[1].x=0.7;    line2pts[1].y=0.4;    line2pts[1].z=0.0;
    line2pts[2].x=0.7;    line2pts[2].y=0.6;    line2pts[2].z=0.0;

    line3pts[0].x=0.1;    line3pts[0].y=0.1;    line3pts[0].z=0.0;
    line3pts[1].x=0.7;    line3pts[1].y=0.1;    line3pts[1].z=0.0;
    line3pts[2].x=0.7;    line3pts[2].y=0.3;    line3pts[2].z=0.0;


    /* set line colors */
    reddish.rgb.red =1.0; reddish.rgb.green = 0.0;  reddish.rgb.blue = 0.0;
   greenish.rgb.red =0.0;greenish.rgb.green = 1.0; greenish.rgb.blue = 0.0;
    blueish.rgb.red =0.0; blueish.rgb.green = 0.0;  blueish.rgb.blue = 1.0;

    /* set transforms */
    local_identity(dpy, strid);
    PEXSetLineType(dpy,strid,PEXOCStore, PEXLineTypeSolid);
    
    /* draw polyline1, in red */
    names[0]=1;
    PEXAddToNameSet(dpy, strid, PEXOCStore, 1, names);
    PEXSetLineColor(dpy,strid,PEXOCStore,PEXColorTypeRGB,&reddish);
    PEXPolyline(dpy, strid, PEXOCStore, 3, line1pts);

    /* draw polyline2, in green */
    names[0]=1;
    PEXRemoveFromNameSet(dpy, strid, PEXOCStore, 1, names);
    names[0]=2;
    PEXAddToNameSet(dpy, strid, PEXOCStore, 1, names);
    PEXSetLineColor(dpy,strid,PEXOCStore,PEXColorTypeRGB,&greenish);
    PEXPolyline(dpy, strid, PEXOCStore, 3, line2pts);

    /* draw polyline3, in blue */
    /* draw polyline2, in green */
    names[0]=2;
    PEXRemoveFromNameSet(dpy, strid, PEXOCStore, 1, names);
    names[0]=3;
    PEXAddToNameSet(dpy, strid, PEXOCStore, 1, names);
    PEXSetLineColor(dpy,strid,PEXOCStore,PEXColorTypeRGB,&blueish);
    PEXPolyline(dpy, strid, PEXOCStore, 3, line3pts);


}

local_identity(Display *dpy, PEXStructure strid)
    {
    float scale_matrix[4][4];

    scale_matrix[0][0] = 1;
    scale_matrix[0][1] = 0;
    scale_matrix[0][2] = 0;
    scale_matrix[0][3] = 0;
    scale_matrix[1][0] = 0;
    scale_matrix[1][1] = 1;
    scale_matrix[1][2] = 0;
    scale_matrix[1][3] = 0;
    scale_matrix[2][0] = 0;
    scale_matrix[2][1] = 0;
    scale_matrix[2][2] = 1;
    scale_matrix[2][3] = 0;
    scale_matrix[3][0] = 0;
    scale_matrix[3][1] = 0;
    scale_matrix[3][2] = 0;
    scale_matrix[3][3] = 1;

    PEXSetLocalTransform(dpy, strid, PEXOCStore, PEXReplace,
                         scale_matrix);
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "nms-pick2");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "nms-pick2");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    _hppex_set_stderr("nms-pick2.stderr", "nms-pick2");
    local_win = window;
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nPEXlib Picking -- pick filter functioning (structure mode)\n\n");
    describe_test("  This test generates stderr output for 9 tests.\n");
    describe_test("  The output for each test contains the expected\n");
    describe_test("  number of picks followed by the actual number received.\n\n");
    fflush (stdout);

    test_pick(dpy, resourceID, local_win);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    file_testproc("nms-pick2.stderr", "nms-pick2");
    }
