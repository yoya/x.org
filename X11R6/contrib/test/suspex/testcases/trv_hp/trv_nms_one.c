/* $XConsortium: trv_nms_one.c,v 1.0 93/11/22 12:40:52 rws Exp $ */

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

/* Nameset containing all primitives. */
#define ALL_PRIMS 1
#define YELLOW_SQUARE 2
#define BLUE_SQUARES 3
#define GREEN_SQUARES 4
#define EXECUTE_STRUCTURE 0
#define RENDER_ELEMENTS 1
#define PICK_ONE        2

Display *display;
Window window;
int screen;
PEXColor lineColor;
PEXRenderer           rid;
PEXRendererAttributes rndAttrs;
PEXStructure structs[4];
unsigned long item_mask;


#define TOTAL_IMAGES 19

int current_frame = 1;
int testing_complete = False;
void testproc(Display *dpy, Window window, int cur_image);

do_test()
{

PEXVector trans_vector;
PEXName name, names[5];
int error;
void pline_square(Display *display, XID id);
void local_identity(Display *display, XID id);
void global_translate(Display *display, XID id, PEXVector trans_vector);
void local_translate(Display *display, XID id, PEXVector trans_vector);

/*** Start of PEXlib PickOne tests. ***/

/* Build structs[3] */

    structs[3] = PEXCreateStructure(display);

    name = YELLOW_SQUARE;
    PEXAddToNameSet(display, structs[3], PEXOCStore, 1, &name);

    name = ALL_PRIMS;
    PEXRemoveFromNameSet(display, structs[3], PEXOCStore, 1, &name);

    lineColor.rgb.red = 1.0;
    lineColor.rgb.green = 1.0;
    lineColor.rgb.blue = 0.0;

    PEXSetLineColor(
      display, structs[3], PEXOCStore, PEXColorTypeRGB, &lineColor);
    
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
    trans_vector.y = 0.3;
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
    
    trans_vector.x = 0.25;
    trans_vector.y = 0.0;
    trans_vector.z = 0.0;
    global_translate(display, structs[1], trans_vector); 
    local_identity(display, structs[1]);
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

   /* Make all primitives pickable. */
    name = ALL_PRIMS;
    PEXAddToNameSet(display, structs[0], PEXOCStore, 1, &name);  

    lineColor.rgb.red = 1.0;
    lineColor.rgb.green = 0.0;
    lineColor.rgb.blue = 0.0;
    PEXSetLineColor(
      display, structs[0], PEXOCStore, PEXColorTypeRGB, &lineColor);

    trans_vector.x = 0.1;
    trans_vector.y = 0.0;
    trans_vector.z = 0.0;
    global_translate(display, structs[0], trans_vector); 
    local_identity(display, structs[0]);

    pline_square(display, structs[0]);
    PEXExecuteStructure(display, structs[0], PEXOCStore, structs[1]);
    PEXExecuteStructure(display, structs[0], PEXOCStore, structs[2]);

 
/*
    Render all of the elements in the root structure, so we have 
    something to pick from.
*/

    rndAttrs.clear_image = True;
    rndAttrs.clear_z = True;
    rndAttrs.background_color.type = PEXColorTypeRGB;
    rndAttrs.background_color.value.rgb.red = 0.0;
    rndAttrs.background_color.value.rgb.green = 0.0;
    rndAttrs.background_color.value.rgb.blue = 0.0;
    rndAttrs.hlhsr_mode = PEXHLHSROff;
    rndAttrs.pick_incl = PEXCreateNameSet( display);
    rndAttrs.pick_excl = PEXCreateNameSet( display);

    /* Note that there are no pickable primitives in the first test. */

    item_mask = 0;
    item_mask = PEXRAClearImage|PEXRABackgroundColor|PEXRAHLHSRMode|
                PEXRAClearZ|PEXRAPickIncl|PEXRAPickExcl;

    PEXChangeRenderer(display,rid,item_mask,&rndAttrs);

    PEXRenderNetwork(display, window, rid, structs[0]);

    /* We won't want to clear the image when we show the picked primitives. */
    rndAttrs.clear_image = False;
    rndAttrs.clear_z = False;
    PEXChangeRenderer(display, rid, item_mask, &rndAttrs);

    describe_test("\n\nPickOne/Nameset interaction tests.\n");
    describe_test("First the entire structure is drawn.\n");
    describe_test("Next, hit primitives will be redrawn in white.\n");

/*  Verify that network is being drawn correctly. */

    describe_test("\nVerify that the network is drawn correctly.\n");
    describe_test("Column 1: 1 red square, 2 blue squares.\n");
    describe_test("Column 2: 2 green squares, 2 blue squares.\n");
    describe_test("Column 3: 1 yellow square.\n");
    XSync(display, True);
    testproc(display, window, current_frame++);

    describe_test("\nFirst test, default, no hits.\n\n");
    error = check_path_to_primitive(structs[0], PEXNoPick, True);
    if (error)
       _hppex_stderr_print(
        "\tError occurred in default, no-hit case.\n\n");

    names[0] = YELLOW_SQUARE;
    names[1] = BLUE_SQUARES;
    PEXChangeNameSet(display, rndAttrs.pick_excl, PEXNSAdd, 2, names);
    item_mask = PEXRAPickExcl;
    PEXChangeRenderer(display, rid, item_mask, &rndAttrs);

    names[0] = GREEN_SQUARES;
    PEXChangeNameSet(display, rndAttrs.pick_incl, PEXNSAdd, 1, names);
    item_mask = PEXRAPickIncl;
    PEXChangeRenderer(display, rid, item_mask, &rndAttrs);

    describe_test("\nSecond test: Put yellow and green square in  pick inclusion set.\n"); 
    describe_test("Yellow is also in exclusion set, so only top green square is hit.\n");
    describe_test("First, the structure network is drawn. \n");
    describe_test("Next, hits are redrawn in white.\n");

    error = check_path_to_primitive(structs[0], PEXPick, True);
    if (error)
       _hppex_stderr_print(
        "\tError occurred on hit of one eligible, green square.\n\n");

    names[0] = ALL_PRIMS;
    PEXChangeNameSet(display, rndAttrs.pick_incl, PEXNSAdd, 1, names);
    item_mask = PEXRAPickIncl;
    PEXChangeRenderer(display, rid, item_mask, &rndAttrs);
    names[0] = BLUE_SQUARES;
    item_mask = PEXRAPickExcl;
    PEXChangeNameSet(display, rndAttrs.pick_excl, PEXNSRemove, 1, names);
    PEXChangeRenderer(display, rid, item_mask, &rndAttrs);

    describe_test(
      "\nWe should find the path to the topmost, leftmost blue square.\n");
    describe_test(
      "This test is verifying that pop worked.\n");
    error = check_path_to_primitive(structs[0], PEXPick, False);
    if (error)
       _hppex_stderr_print(
        "\tError occurred on expected hit of topmost, leftmost, \nblue square (last primitive) in network.\n\n");

/*  Cleanup. */

    PEXDestroyStructures(display, 4, structs);
    PEXFreeRenderer(display, rid);
    PEXFreeNameSet(display, rndAttrs.pick_incl);
    PEXFreeNameSet(display, rndAttrs.pick_excl);
    XDestroyWindow(display, window);
}
     
void global_translate(Display *display, XID id, PEXVector trans_vect)

    {
    PEXMatrix xlate_matrix;
    
    PEXTranslate(&trans_vect, xlate_matrix);
    PEXSetGlobalTransform(display, id, PEXOCStore, xlate_matrix);
    }


void local_translate(Display *display, XID id, PEXVector trans_vect)

    {
    PEXMatrix xlate_matrix;
    
    PEXTranslate(&trans_vect, xlate_matrix);
    PEXSetLocalTransform(display, id, PEXOCStore, PEXReplace, 
       xlate_matrix);
    }




void pline_square(Display *display, XID id)

    {
    PEXCoord pts[5];

    pts[ 0].x =  0.05; pts[ 0].y =  0.05; pts[ 0].z =  0.0;
    pts[ 1].x =  0.15; pts[ 1].y =  0.05; pts[ 1].z =  0.0;
    pts[ 2].x =  0.15; pts[ 2].y =  0.15; pts[ 2].z =  0.0;
    pts[ 3].x =  0.05; pts[ 3].y =  0.15; pts[ 3].z =  0.0;
    pts[ 4].x =  0.05; pts[ 4].y =  0.05; pts[ 4].z =  0.0;

    PEXPolyline(display, id, PEXOCStore, 5, pts);
    }

void local_identity(Display *display, XID id)

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

int check_path_to_primitive(PEXStructure str_id, int expected_status,
        int expected_better) 

{
    int               aperture_type;
    PEXPickRecord     aperture; 
    int               status, better, error=0, method, i;
    PEXPickPath       *pick_path=NULL;

    /* Pick-last is always available. */
    method = PEXPickLast;

    /* 
       First set of tests:
          Use a small pick aperture on an entire structure network.
          Set the pick aperture to find the yellow square in the middle of
          the network.  Then, verify that the correct paths are found.
     */

    aperture_type = PEXPickDeviceNPCHitVolume;
    aperture.volume.min.x = 0.0;
    aperture.volume.min.y = 0.0;
    aperture.volume.min.z = 0.0;
    aperture.volume.max.x = 1.0;
    aperture.volume.max.y = 1.0;
    aperture.volume.max.z = 1.0;

    /* Test ExecuteStructure pick traversal. */
    describe_test("\nTrying this case with an ExecuteStructure traversal.\n");
    PEXBeginPickOne(display, window, rid, 0, PEXPickLast,
       aperture_type, &aperture);

       PEXExecuteStructure(display, rid, PEXOCRender, str_id);

    pick_path = PEXEndPickOne(display, rid, &status, &better);

    error = 
      report_single_path_errs(
          status, expected_status, better, expected_better, pick_path, 0, 
          EXECUTE_STRUCTURE);
    if (error)
       _hppex_stderr_print(
          "\tError occurred in ExecuteStructure traversal. \n");
    else if (pick_path != NULL) {
       PEXFreePickPaths(1L, pick_path);
       pick_path = NULL;
    }

    /* Test RenderElements pick traversal. */

    describe_test("\nTrying this case with a RenderElements traversal.\n");   
    PEXBeginPickOne(display, window, rid, 0, PEXPickLast,
       aperture_type, &aperture);

       PEXRenderElements(display, rid, str_id, PEXBeginning, 0,
          PEXEnd, 0);

    pick_path = PEXEndPickOne(display, rid, &status, &better);
    error = 
      report_single_path_errs(
          status, expected_status, better, expected_better, pick_path, 
          0, RENDER_ELEMENTS);
    if (error)
       _hppex_stderr_print(
          "\tError occured in RenderElements traversal. \n");
    else if (pick_path != NULL) {
       PEXFreePickPaths(1L, pick_path);
       pick_path = NULL;
    }

    /* PickOne test. */
    describe_test("\nTrying this case with a PEXPickOne call.\n");
    pick_path = PEXPickOne(display, window, rid, str_id, PEXPickLast,
       aperture_type, &aperture, &status, &better);
    
    error = 
        report_single_path_errs(
            status, expected_status, better, expected_better, pick_path, 
            0, PICK_ONE);
    if (error)
       _hppex_stderr_print(
          "\tError occured in PEXPickOne traversal. \n");
    else if (pick_path != NULL) {
       PEXFreePickPaths(1L, pick_path);
       pick_path = NULL;
    }

    return(error);

}






int report_single_path_errs(int status, int expected_status, int better,
    int expected_better, PEXPickPath *pick_path, 
    PEXStructure str_id, int trv_type)
{
    int error=0, i, count, offset;
    PEXElementRef accum_state_path[5], *p_path;
    PEXStructure element_sid;

    if (status != expected_status) {
       _hppex_stderr_print(
          "Error: Incorrect status value returned.\n");
       error = 1;
    }

    if (better != expected_better)
    {
       _hppex_stderr_print(
          "Error: Incorrect better value returned.\n");
       error = 1;
    }


    if (error)
       return(error);

    /* Show the picked primitive by changing the line style. */

    PEXRenderNetwork(display, window, rid, structs[0]);
    XSync(display, True);
    testproc(display, window, current_frame++);

    if (status == PEXNoPick) {
       XSync(display, True);
       testproc(display, window, current_frame++);
       return(error);
    }

    for (i=0; i<pick_path->count; i++)
    {
       /* For RenderElements traversals, "0" is passed in as the
        * structure that starts the pick path.  We need to substitute
        * a real structure id, which in this case will always be the
        * root structure.
        */

        if (pick_path->elements[i].sid == 0)
           accum_state_path[i].structure = structs[0];
        else
           accum_state_path[i].structure = pick_path->elements[i].sid;

        accum_state_path[i].offset = pick_path->elements[i].offset;
    }


    if (trv_type == EXECUTE_STRUCTURE) {
        count = pick_path->count -1;
        p_path = accum_state_path +1;
    } else {
        count = pick_path->count;
        p_path = accum_state_path;
    }

    offset = p_path[count-1].offset;
    element_sid = p_path[count-1].structure;

    p_path[count-1].offset--;

    PEXBeginRendering(display, window, rid);

    /* Accumulate state up to the hit primitive. */
    PEXAccumulateState(display, rid, count, p_path);

    lineColor.rgb.red = 1.0;
    lineColor.rgb.green = 1.0;
    lineColor.rgb.blue = 1.0;
    /* Change line type to show which element is picked. */
    PEXSetLineColor(display, rid, PEXOCRender, PEXColorTypeRGB,
       &lineColor);

    /* Render the last element with white lines. */
    PEXRenderElements(display, rid, element_sid, PEXBeginning,
       offset, PEXBeginning, offset);
    PEXEndRendering(display, rid, False);

    XSync(display, True);
    testproc(display, window, current_frame++);


    return (error);


}




void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "trv_nms_one");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "trv_nms_one");
    }

void misc_setup(Display *dpy, Window test_win, PEXRenderer renderer,
		int cur_image)
    {
    window  = test_win;
    rid     = renderer;
    display = dpy;
    _hppex_set_stderr("trv_nms_one.stderr", "trv_nms_one");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    do_test();
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    if (!testing_complete)
	{
        sprintf(image_name,"trv_nms_one_%d", cur_image);
        sprintf(test_name, "trv_nms_one #%d", cur_image);
         image_testproc(image_name, test_name,  img_desc,
		          dpy,window, 0, 0, 
		          glob_window_width, glob_window_height);

        if (cur_image == TOTAL_IMAGES)
	    {
            file_testproc("trv_nms_one.stderr", "trv_nms_one");
	    testing_complete = True;
	    }
	}
    }

