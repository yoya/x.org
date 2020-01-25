/* $XConsortium: trv_interact.c,v 1.0 93/11/22 12:40:41 rws Exp $ */

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

#define YELLOW_SQUARE 2
#define BLUE_SQUARES 3
#define GREEN_SQUARES 4
#define ROOT_SQUARES 5

Window window1;

do_test(Display *display, Window window)
    {
    PEXRenderer rid, rid2;
    PEXColor lineColor;
    PEXRendererAttributes rndAttrs;
    unsigned long item_mask;
    PEXName name;
    PEXStructure structs[5], structs2[5];
    PEXVector trans_vector;
    int status, better, more_return;
    unsigned long count;
    PEXElementRef accum_state_path[3];
    PEXPickRecord aperture;
    PEXPickPath *ret_path;
    PEXColorApproxEntry capx_info;
    Window window2;
    int window2_x, window2_y;

    XSynchronize(display,1);

    window2_x = (glob_window_x + glob_window_width+20);
    window2_y = (glob_window_y + glob_window_height+20);
    window2 = create_and_setup_window(display, "trv_interact 2", 
				      window2_x, window2_y,
				      glob_window_width, glob_window_height,
				      &capx_info);

    /* Begin display traversal tests. */

    item_mask = 0;
    rid = PEXCreateRenderer(display,window,item_mask,&rndAttrs);
    rid2 = PEXCreateRenderer(display, window, item_mask, &rndAttrs);

    rndAttrs.clear_image = True;
    rndAttrs.clear_z = True;
    rndAttrs.background_color.type = PEXColorTypeRGB;
    rndAttrs.background_color.value.rgb.red = 0.0;
    rndAttrs.background_color.value.rgb.green = 0.0;
    rndAttrs.background_color.value.rgb.blue = 0.0;
    rndAttrs.hlhsr_mode = PEXHLHSROff;

    item_mask = PEXRAClearImage|PEXRABackgroundColor|PEXRAHLHSRMode|
                PEXRAClearZ;

    PEXChangeRenderer(display,rid,item_mask,&rndAttrs);
    PEXChangeRenderer(display,rid2,item_mask,&rndAttrs);

    /* Build structs[3] */

    structs[3] = PEXCreateStructure(display);

    lineColor.rgb.red = 1.0;
    lineColor.rgb.green = 1.0;
    lineColor.rgb.blue = 0.0;

    PEXSetLineColor(
      display, structs[3], PEXOCStore, PEXColorTypeRGB, &lineColor);
    
    name = YELLOW_SQUARE;
    PEXAddToNameSet(display, structs[3], PEXOCStore, 1, &name);

    trans_vector.x = 0.4;
    trans_vector.y = 0.0;
    trans_vector.z = 0.0;
    global_translate(display, structs[3], trans_vector); 
    pline_square(display, structs[3]);


    /* Build structs[2] */

    structs[2] = PEXCreateStructure(display);

    lineColor.rgb.red = 0.0;
    lineColor.rgb.green = 0.0;
    lineColor.rgb.blue = 1.0;

    PEXSetLineColor(
      display, structs[2], PEXOCStore, PEXColorTypeRGB, &lineColor);

    name = BLUE_SQUARES;
    PEXAddToNameSet(display, structs[2], PEXOCStore, 1, &name);
    
    trans_vector.x = 0.0;
    trans_vector.y = 0.15;
    trans_vector.z = 0.0;
    local_translate(display, structs[2], trans_vector);
    pline_square(display, structs[2]);

    trans_vector.x = 0.0;
    trans_vector.y = 0.15;
    trans_vector.z = 0.0;
    local_translate(display, structs[2], trans_vector);
    pline_square(display, structs[2]);

    /* Build structs[1] */

    structs[1] = PEXCreateStructure(display);

    lineColor.rgb.red = 0.0;
    lineColor.rgb.green = 1.0;
    lineColor.rgb.blue = 0.0;

    PEXSetLineColor(
      display, structs[1], PEXOCStore, PEXColorTypeRGB, &lineColor);

    name = GREEN_SQUARES;
    PEXAddToNameSet(display, structs[1], PEXOCStore, 1, &name);
    
    trans_vector.x = 0.15;
    trans_vector.y = 0.0;
    trans_vector.z = 0.0;
    local_translate(display, structs[1], trans_vector); 
    pline_square(display, structs[1]);

    trans_vector.x = 0.0;
    trans_vector.y = 0.15;
    trans_vector.z = 0.0;
    local_translate(display, structs[1], trans_vector);
    pline_square(display, structs[1]);

    PEXExecuteStructure(display, structs[1], PEXOCStore, structs[2]); 
    PEXExecuteStructure(display, structs[1], PEXOCStore, structs[3]); 

    /*  Build Root Structure */

    structs[0] = PEXCreateStructure(display);

    lineColor.rgb.red = 1.0;
    lineColor.rgb.green = 0.0;
    lineColor.rgb.blue = 0.0;
    PEXSetLineColor(
      display, structs[0], PEXOCStore, PEXColorTypeRGB, &lineColor);

    name = ROOT_SQUARES;
    PEXAddToNameSet(display, structs[0], PEXOCStore, 1, &name);

    trans_vector.x = 0.1;
    trans_vector.y = 0.0;
    trans_vector.z = 0.0;
    global_translate(display, structs[0], trans_vector); 
    pline_square(display, structs[0]);
    PEXExecuteStructure(display, structs[0], PEXOCStore, structs[1]);
    PEXExecuteStructure(display, structs[0], PEXOCStore, structs[2]);

    /* Build structs2[3] */

    structs2[3] = PEXCreateStructure(display);

    lineColor.rgb.red = 1.0;
    lineColor.rgb.green = 1.0;
    lineColor.rgb.blue = 0.0;

    PEXSetLineColor(
      display, structs2[3], PEXOCStore, PEXColorTypeRGB, &lineColor);
    
    name = YELLOW_SQUARE;
    PEXAddToNameSet(display, structs2[3], PEXOCStore, 1, &name);

    trans_vector.x = 0.4;
    trans_vector.y = 0.0;
    trans_vector.z = 0.0;
    global_translate(display, structs2[3], trans_vector); 
    pline_square(display, structs2[3]);


    /* Build structs2[2] */

    structs2[2] = PEXCreateStructure(display);

    lineColor.rgb.red = 0.0;
    lineColor.rgb.green = 0.0;
    lineColor.rgb.blue = 1.0;

    PEXSetLineColor(
      display, structs2[2], PEXOCStore, PEXColorTypeRGB, &lineColor);

    name = BLUE_SQUARES;
    PEXAddToNameSet(display, structs2[2], PEXOCStore, 1, &name);
    
    trans_vector.x = 0.0;
    trans_vector.y = 0.15;
    trans_vector.z = 0.0;
    local_translate(display, structs2[2], trans_vector);
    pline_square(display, structs2[2]);

    trans_vector.x = 0.0;
    trans_vector.y = 0.15;
    trans_vector.z = 0.0;
    local_translate(display, structs2[2], trans_vector);
    pline_square(display, structs2[2]);

    /* Build structs2[1] */

    structs2[1] = PEXCreateStructure(display);

    lineColor.rgb.red = 0.0;
    lineColor.rgb.green = 1.0;
    lineColor.rgb.blue = 0.0;

    PEXSetLineColor(
      display, structs2[1], PEXOCStore, PEXColorTypeRGB, &lineColor);

    name = GREEN_SQUARES;
    PEXAddToNameSet(display, structs2[1], PEXOCStore, 1, &name);
    
    trans_vector.x = 0.15;
    trans_vector.y = 0.0;
    trans_vector.z = 0.0;
    local_translate(display, structs2[1], trans_vector); 
    pline_square(display, structs2[1]);

    trans_vector.x = 0.0;
    trans_vector.y = 0.15;
    trans_vector.z = 0.0;
    local_translate(display, structs2[1], trans_vector);
    pline_square(display, structs2[1]);

    PEXExecuteStructure(display, structs2[1], PEXOCStore, structs2[2]); 
    PEXExecuteStructure(display, structs2[1], PEXOCStore, structs2[3]); 

    /*  Build Root Structure */

    structs2[0] = PEXCreateStructure(display);

    lineColor.rgb.red = 1.0;
    lineColor.rgb.green = 0.0;
    lineColor.rgb.blue = 0.0;
    PEXSetLineColor(
      display, structs2[0], PEXOCStore, PEXColorTypeRGB, &lineColor);

    name = ROOT_SQUARES;
    PEXAddToNameSet(display, structs2[0], PEXOCStore, 1, &name);

    trans_vector.x = 0.1;
    trans_vector.y = 0.0;
    trans_vector.z = 0.0;
    global_translate(display, structs2[0], trans_vector); 
    pline_square(display, structs2[0]);
    PEXExecuteStructure(display, structs2[0], PEXOCStore, structs2[1]);
    PEXExecuteStructure(display, structs2[0], PEXOCStore, structs2[2]);


    rndAttrs.invisibility_incl = PEXCreateNameSet(display);
    rndAttrs.invisibility_excl = PEXCreateNameSet(display);
    rndAttrs.pick_incl = PEXCreateNameSet(display);

    name = ROOT_SQUARES;
    PEXChangeNameSet(display, rndAttrs.invisibility_incl, PEXNSAdd,
       1, &name);
    PEXChangeRenderer(display, rid, PEXRAInvisibilityIncl, &rndAttrs);

    PEXChangeNameSet(display, rndAttrs.invisibility_excl, PEXNSAdd,
       1, &name);
    PEXChangeRenderer(display, rid2, PEXRAInvisibilityExcl, &rndAttrs);

    PEXChangeNameSet(display, rndAttrs.pick_incl, PEXNSAdd,
       1, &name);
    PEXChangeRenderer(display, rid2, PEXRAPickIncl, &rndAttrs);

    describe_test(
     "\nCombination AccumulateState/RenderEle/ExecuteStructure/RenderElements.\n");
    describe_test("Only two columns are visible.\n");
    describe_test("This test shows two windows.\n"
);
    describe_test("In the leftmost window, there is one column of squares.\n");
    describe_test("Bottom to top: green, green, blue, blue.\n");
    describe_test("Next to that column is a yellow square.\n");

    PEXBeginRendering(display, window2, rid);
    PEXBeginRendering(display, window, rid2);

       accum_state_path[0].structure = structs[0];
       accum_state_path[0].offset = 5;
       accum_state_path[1].structure = structs[1];
       accum_state_path[1].offset = 2;
       PEXAccumulateState(display, rid, 2, accum_state_path);
       
       accum_state_path[0].structure = structs2[0];
       accum_state_path[0].offset = 5;
       accum_state_path[1].structure = structs2[1];
       accum_state_path[1].offset = 2;
       PEXAccumulateState(display, rid2, 2, accum_state_path);

       PEXRenderElements(display, rid, structs[1], PEXBeginning, 3,
           PEXBeginning, 6);
       PEXRenderElements(display, rid2, structs2[1], PEXBeginning, 3,
           PEXBeginning, 6);

       PEXExecuteStructure(display, rid, PEXOCRender, structs[2]);
       PEXExecuteStructure(display, rid2, PEXOCRender, structs2[2]);

       PEXExecuteStructure(display, rid, PEXOCRender, structs[3]);
       PEXExecuteStructure(display, rid2, PEXOCRender, structs2[3]);

       PEXRenderElements(display, rid, structs[2], PEXBeginning, 0,
           PEXEnd, 0);

    PEXEndRendering(display, rid, True);
    XSync(display, 0);
     image_testproc("trv_interact_1", "trv_interact",  img_desc,
		      display, window, 0, 0, 
		      glob_window_width, glob_window_height);
    

    PEXEndRendering(display, rid2, True);
    XSync(display, 0);
     image_testproc("trv_interact_2", "trv_interact",  img_desc,
		      display, window2, 0, 0, 
		      glob_window_width, glob_window_height);



   /* Do the same thing with picking. */

    aperture.volume.min.x = 0.0;
    aperture.volume.min.y = 0.0;
    aperture.volume.min.z = 0.0;
    aperture.volume.max.x = 1.0;
    aperture.volume.max.y = 1.0;
    aperture.volume.max.z = 1.0;
 
    PEXBeginPickOne(display, window, rid, structs[0], PEXPickLast,
        PEXPickDeviceNPCHitVolume, &aperture);

    PEXBeginPickOne(display, window2, rid2, structs2[0], PEXPickLast,
       PEXPickDeviceNPCHitVolume, &aperture);

       accum_state_path[0].structure = structs[0];
       accum_state_path[0].offset = 5;
       accum_state_path[1].structure = structs[1];
       accum_state_path[1].offset = 2;
       PEXAccumulateState(display, rid, 2, accum_state_path);

       accum_state_path[0].structure = structs2[0];
       accum_state_path[0].offset = 5;
       accum_state_path[1].structure = structs2[1];
       accum_state_path[1].offset = 2;
       PEXAccumulateState(display, rid2, 2, accum_state_path);

       PEXRenderElements(display, rid, structs[1], PEXBeginning, 3,
           PEXBeginning, 6);
       PEXRenderElements(display, rid2, structs2[1], PEXBeginning, 3,
           PEXBeginning, 6);

       PEXExecuteStructure(display, rid, PEXOCRender, structs[2]);
       PEXExecuteStructure(display, rid2, PEXOCRender, structs2[2]);

       PEXExecuteStructure(display, rid, PEXOCRender, structs[3]);
       PEXExecuteStructure(display, rid2, PEXOCRender, structs2[3]);

       PEXRenderElements(display, rid, structs[2], PEXBeginning, 0,
           PEXEnd, 0);

    describe_test("\nThe PickOne and PickAll interaction tests produce no visual output.\n");
    describe_test("An error will be printed if correct paths are not returned.\n");

    ret_path = PEXEndPickOne(display, rid, &status, &better);
    if (status != PEXNoPick )
       describe_test("ERROR: Invisible hits detected.\n");
    if (better != False)
       describe_test("ERROR: Undetectable hit value wrong, invisibility on.\n");
 
    ret_path = PEXEndPickOne(display, rid2, &status, &better);
    if (status != PEXPick)
       describe_test("ERROR: Did not detect a hit with exclusion filter on.\n");
    if (better != False)
       describe_test("ERROR: Undetectable hit value wrong.\n");

    if ((ret_path->count != 2) ||
        (ret_path->elements[0].sid != structs2[0]) ||
        (ret_path->elements[0].offset != 13)  ||
        (ret_path->elements[1].sid != structs2[3]) ||
        (ret_path->elements[1].offset != 4))
        describe_test("ERROR: Bad path returned from PEXEndPickOne.\n"); 

/* Do the same test PickAll. */

    aperture.volume.min.x = 0.0;
    aperture.volume.min.y = 0.6;
    aperture.volume.min.z = 0.0;
    aperture.volume.max.x = 1.0;
    aperture.volume.max.y = 1.0;
    aperture.volume.max.z = 1.0;
 
    PEXBeginPickAll(display, window, rid, structs[0], PEXPickAllAll,
        False, 10, PEXPickDeviceNPCHitVolume, &aperture);

    PEXBeginPickAll(display, window2, rid2, structs2[0], PEXPickAllAll,
       False, 10, PEXPickDeviceNPCHitVolume, &aperture);

       accum_state_path[0].structure = structs[0];
       accum_state_path[0].offset = 5;
       accum_state_path[1].structure = structs[1];
       accum_state_path[1].offset = 2;
       PEXAccumulateState(display, rid, 2, accum_state_path);

       accum_state_path[0].structure = structs2[0];
       accum_state_path[0].offset = 5;
       accum_state_path[1].structure = structs2[1];
       accum_state_path[1].offset = 2;
       PEXAccumulateState(display, rid2, 2, accum_state_path);

       PEXRenderElements(display, rid, structs[1], PEXBeginning, 3,
           PEXBeginning, 6);
       PEXRenderElements(display, rid2, structs2[1], PEXBeginning, 3,
           PEXBeginning, 6);

       PEXExecuteStructure(display, rid, PEXOCRender, structs[2]);
       PEXExecuteStructure(display, rid2, PEXOCRender, structs2[2]);

       PEXExecuteStructure(display, rid, PEXOCRender, structs[3]);
       PEXExecuteStructure(display, rid2, PEXOCRender, structs2[3]);

       PEXRenderElements(display, rid, structs[2], PEXBeginning, 0,
           PEXEnd, 0);

    ret_path = PEXEndPickAll(display, rid, &status, &more_return, &count);
    if (status != PEXNoPick )
       describe_test("ERROR: Invisible hits detected in PickAll.\n");
    if (more_return != PEXNoMoreHits)
       describe_test("ERROR: more_hits should be NoMoreHits.\n");
 
    ret_path = PEXEndPickAll(display, rid2, &status, &more_return, &count);
    if (status != PEXPick)
       describe_test("ERROR: Did not detect a hit with exclusion filter on.\n");
    if (more_return != PEXNoMoreHits)
       describe_test("ERROR: Undetectable hit value wrong in PickAll.\n");

    if ((ret_path->count != 2) ||
        (ret_path->elements[0].sid != structs2[0]) ||
        (ret_path->elements[0].offset != 12)  ||
        (ret_path->elements[1].sid != structs2[2]) ||
        (ret_path->elements[1].offset != 6))
        describe_test("ERROR: Bad path returned from PEXEndPickOne.\n"); 

    file_testproc("trv_interact.stderr", "trv_interact");

    PEXDestroyStructures(display, 4, structs);        
    PEXDestroyStructures(display, 4, structs2);
    PEXFreeRenderer(display,rid);
    PEXFreeRenderer(display, rid2);
    PEXFreeNameSet(display, rndAttrs.invisibility_incl);
    PEXFreeNameSet(display, rndAttrs.invisibility_excl);

    XDestroyWindow(display, window);
    XDestroyWindow(display, window2);
    }

     
global_translate(Display *display, XID id, PEXVector trans_vect)
    {
    PEXMatrix xlate_matrix;
    
    PEXTranslate(&trans_vect, xlate_matrix);
    PEXSetGlobalTransform(display, id, PEXOCStore, xlate_matrix);
    }


local_translate(Display *display, XID id, PEXVector trans_vect)
    {
    PEXMatrix xlate_matrix;
    
    PEXTranslate(&trans_vect, xlate_matrix);
    PEXSetLocalTransform(display, id, PEXOCStore, PEXPreConcatenate, 
       xlate_matrix);
    }




pline_square(Display *display, XID id)
    {
    PEXCoord pts[5];

    pts[ 0].x =  0.05; pts[ 0].y =  0.05; pts[ 0].z =  0.0;
    pts[ 1].x =  0.15; pts[ 1].y =  0.05; pts[ 1].z =  0.0;
    pts[ 2].x =  0.15; pts[ 2].y =  0.15; pts[ 2].z =  0.0;
    pts[ 3].x =  0.05; pts[ 3].y =  0.15; pts[ 3].z =  0.0;
    pts[ 4].x =  0.05; pts[ 4].y =  0.05; pts[ 4].z =  0.0;

    PEXPolyline(display, id, PEXOCStore, 5, pts);
    }

local_identity(Display *display, XID id)
    
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

    PEXSetLocalTransform(display, id, PEXOCStore, PEXReplace,
                         scale_matrix);
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "trv_interact");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "trv_interact");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    window1 = window;
    _hppex_set_stderr("trv_interact.stderr", "trv_interact");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    describe_test("\nTraversal Test \n\n");
    describe_test("  This test exercises traversals with multiple renderers.\n");
    do_test(dpy, window1);
    }

/*  testproc is a NOP because it is done in do_test() */
void testproc(Display *dpy, Window window, int cur_image)
    {
    }
