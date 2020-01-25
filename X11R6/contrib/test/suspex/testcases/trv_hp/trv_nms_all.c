/* $XConsortium: trv_nms_all.c,v 1.0 93/11/22 12:40:47 rws Exp $ */

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

#define ALL_PRIMS 1
#define YELLOW_SQUARE 2
#define BLUE_SQUARES 3
#define GREEN_SQUARES 4

#define BAD_STATUS 0
#define BAD_MORE_HITS 1
#define BAD_COUNT 2

#define TOTAL_IMAGES 14

char *error_strings[3][40] = {"Bad status value returned.\n",
                            "Bad more hits value returned.\n",
                            "Bad count value returned.\n"};

Window window;
PEXColor lineColor;
PEXRendererAttributes rndAttrs;
PEXStructure structs[4];
unsigned long item_mask;

int current_frame = 1;
int testing_complete = False;
void testproc(Display *dpy, Window window, int cur_image);

do_test(Display *display, XID rid)
{

PEXVector trans_vector;
PEXPickRecord pick_record;
int status, more_hits, screen;
PEXName name;
unsigned long count;
int aperture_type, error, i, j;
PEXPickRecord aperture;
PEXPickPath *ret_path=NULL;
PEXPickElementRef elements[30], *element_ptr;
PEXStructurePath pick_start_path;
PEXElementRef start_path, accum_path[2];
void pline_square(Display *display, XID id);
void local_identity(Display *display, XID id);
void global_translate(Display *display, XID id, PEXVector trans_vector);
void local_translate(Display *display, XID id, PEXVector trans_vector);
int check_return_value(
       int status, int more_hits, int count, int expected_status,
       int expected_more_hits, int expected_count);
void highlight_all_paths(Display *display, Drawable window, XID rid, 
       unsigned long count, PEXPickPath *ret_path);

/***** Beginning of picking tests ****/

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
    Render all of the elements in the root structure, so we can see what
    we're picking from, and make all primitives pickable.
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
    rndAttrs.invisibility_incl = PEXCreateNameSet(display);

    item_mask = 0;
    item_mask = PEXRAClearImage|PEXRABackgroundColor|PEXRAHLHSRMode|
                PEXRAClearZ|PEXRAPickIncl|PEXRAPickExcl;

    PEXChangeRenderer(display,rid,item_mask, &rndAttrs);
    PEXBeginRendering(display,window,rid);
    PEXEndRendering(display,rid,True);    

    rndAttrs.clear_image = False;
    item_mask = 0;
    item_mask = PEXRAClearImage;
    PEXChangeRenderer(display,rid,item_mask, &rndAttrs);


/* Test the default - no hits. */

    /* Set an aperture that picks everything in the network. */
    aperture_type = PEXPickDeviceNPCHitVolume;
    aperture.volume.min.x = 0.0;
    aperture.volume.min.y = 0.0;
    aperture.volume.min.z = 0.0;	
    aperture.volume.max.x = 1.0;
    aperture.volume.max.y = 1.0;
    aperture.volume.max.z = 1.0;	


    /* 
     * See if it works with an immediate mode ExecuteStructure 
     * of root structure. 
     */    

    PEXBeginPickAll(display, window, rid, 0, PEXPickAllAll, False,
       10, aperture_type, &aperture);
    PEXExecuteStructure(display, rid, PEXOCRender, structs[0]);
    ret_path = PEXEndPickAll(display, rid, &status, &more_hits, &count);

    describe_test("Traversal/Nameset interaction tests.\n\n");
    describe_test("The first three tests produce no visual output.\n");
    describe_test("When an error occurs, a message is printed to stderr.\n");
    describe_test("No paths are picked.\n");

    error = check_return_value(status, more_hits, count, PEXNoPick,
               PEXNoMoreHits, 0);

    if (error)
    {
       _hppex_stderr_print("ERROR: Default case, ExecuteStructure fails.\n");
       if (ret_path)
          PEXFreePickPaths(count, ret_path);
    }

   /*
    * Now try RenderElements.  
    */

    PEXBeginPickAll(display, window, rid, 0, PEXPickAllAll, False,
       10, aperture_type, &aperture);
    PEXRenderElements(display, rid, structs[0], PEXBeginning, 0,
       PEXEnd, 0);
    ret_path = PEXEndPickAll(display, rid, &status, &more_hits, &count);

    error = check_return_value(status, more_hits, count,
               PEXNoPick, PEXNoMoreHits, 0);

    if (error)
    {
       _hppex_stderr_print("ERROR: Default case, RenderElements fails.\n");
       if (ret_path)
          PEXFreePickPaths(count, ret_path);
    }

   /*
    * Now try PickAll.  The paths will be like the RenderElements paths,
    * except the first element will have the root structure's id.
    */
   
   /* Reset the renderer's pick start path. */
    pick_start_path.count = 1;
    start_path.structure = structs[0];
    start_path.offset = 1;
    pick_start_path.elements = &start_path;
    rndAttrs.pick_start_path = pick_start_path;
    PEXChangeRenderer(display, rid, PEXRAPickStartPath, &rndAttrs);
    
    ret_path = PEXPickAll(display, window, rid, PEXPickAllAll, 
       10, aperture_type, &aperture, &status, &more_hits, &count);

    error = check_return_value(status, more_hits, count,
               PEXNoPick, PEXNoMoreHits, 0);

    if (error)
    {
       _hppex_stderr_print("ERROR:Default case, PickAll fails.\n");
       if (ret_path)
          PEXFreePickPaths(count, ret_path);
    }


  /* Now make all primitives pickable. */
    
    /* Make all primitives pickable. */
    name = ALL_PRIMS;
    PEXChangeNameSet(display, rndAttrs.pick_incl, PEXNSAdd, 1, &name);

    PEXChangeRenderer(display,rid,PEXRAPickIncl,&rndAttrs);

    describe_test("\n\nIn the next two sets of tests, the network is \n");
    describe_test("drawn first, then hit primitives are highlighted in.\n");
    describe_test("white.\n");

    /* 
     * See if it works with an immediate mode ExecuteStructure 
     * of root structure. 
     */    

    PEXBeginPickAll(display, window, rid, 0, PEXPickAllAll, False,
       10, aperture_type, &aperture);
    PEXExecuteStructure(display, rid, PEXOCRender, structs[0]);
    ret_path = PEXEndPickAll(display, rid, &status, &more_hits, &count);

    describe_test("\nPEXBeginPickAll with ExecuteStructure. \n");
    describe_test("All paths are picked.\n");

    error = check_return_value(status, more_hits, count,
               PEXPick, PEXNoMoreHits, 8);

    if (!error)
    { 
        highlight_all_paths(display, window, rid, count, ret_path);
        PEXFreePickPaths(count, ret_path);
        ret_path = NULL;
    }
    else
    {
       _hppex_stderr_print("ERROR: All primitives were pickable.\n");
       _hppex_stderr_print("ExecuteStructure case.\n");
    }

   /*
    * Now try RenderElements.  The paths will be different, because
    * RenderElements does not add an element to the renderer's current path
    * attribute.  It just bumps the offset, so there are less elements,
    * and the contents of the first element will be equal to the 
    * structure id passed into BeginPick*, with an offset equal to the
    * number of elements in the element range.
    */

    PEXBeginPickAll(display, window, rid, 0, PEXPickAllAll, False,
       10, aperture_type, &aperture);
    PEXRenderElements(display, rid, structs[0], PEXBeginning, 0,
       PEXEnd, 0);
    ret_path = PEXEndPickAll(display, rid, &status, &more_hits, &count);

    describe_test("\nPEXBeginPickAll with RenderElements. \n");
    describe_test("All paths are picked.\n");

    error = check_return_value(status, more_hits, count,
                PEXPick, PEXNoMoreHits, 8);

    if (!error)
    {
        highlight_all_paths(display, window, rid, count, ret_path);
        PEXFreePickPaths(count, ret_path);
        ret_path = NULL;
    }
    else
    {
        _hppex_stderr_print("ERROR: All primitives were pickable. \n");
        _hppex_stderr_print("RenderElements case.\n");
    }


   /*
    * Now try PickAll.  The paths will be like the RenderElements paths,
    * except the first element will have the root structure's id.
    */
   
   /* Reset the renderer's pick start path. */
    pick_start_path.count = 1;
    start_path.structure = structs[0];
    start_path.offset = 1;
    pick_start_path.elements = &start_path;
    rndAttrs.pick_start_path = pick_start_path;
    PEXChangeRenderer(display, rid, PEXRAPickStartPath, &rndAttrs);
    
    ret_path = PEXPickAll(display, window, rid, PEXPickAllAll, 
       10, aperture_type, &aperture, &status, &more_hits, &count);

    describe_test("\nPEXPickAll. \n");
    describe_test("All paths are picked.\n");

    error = check_return_value(status, more_hits, count,
                PEXPick, PEXNoMoreHits, 8);

    if (!error)
    {
        highlight_all_paths(display, window, rid, count, ret_path);
        PEXFreePickPaths(count, ret_path);
        ret_path = NULL;
    }
    else
    {
        _hppex_stderr_print("ERROR: All primitives were pickable. \n");
        _hppex_stderr_print("PickAll case.\n");
    }

    /* Make all primitives pickable. */ 
    name = ALL_PRIMS;
    PEXChangeNameSet(display, rndAttrs.pick_incl, PEXNSAdd, 1, &name);
    PEXChangeRenderer(display,rid,PEXRAPickIncl,&rndAttrs);
 
    /* Make blue primitives unpickable. */
    name = BLUE_SQUARES;
    PEXChangeNameSet(display, rndAttrs.pick_excl, PEXNSAdd, 1, &name);
    PEXChangeRenderer(display, rid, PEXRAPickExcl, &rndAttrs);

    /* Make the yellow square invisible. */
    name = YELLOW_SQUARE;
    PEXChangeNameSet(display, rndAttrs.invisibility_incl, PEXNSAdd, 1, &name);
    PEXChangeRenderer(display, rid, PEXRAInvisibilityIncl, &rndAttrs);
    

    /* 
     * See if it works with an immediate mode ExecuteStructure 
     * of root structure. 
     */    

    PEXBeginPickAll(display, window, rid, 0, PEXPickAllAll, False,
       10, aperture_type, &aperture);
    PEXExecuteStructure(display, rid, PEXOCRender, structs[0]);
    ret_path = PEXEndPickAll(display, rid, &status, &more_hits, &count);



    rndAttrs.clear_image = True;
    item_mask = 0;
    item_mask = PEXRAClearImage|PEXRABackgroundColor|PEXRAHLHSRMode|
                PEXRAClearZ|PEXRAPickIncl|PEXRAPickExcl;

    PEXChangeRenderer(display,rid,item_mask, &rndAttrs);
    PEXBeginRendering(display,window,rid);
    PEXEndRendering(display,rid,True);    

    rndAttrs.clear_image = False;
    item_mask = 0;
    item_mask = PEXRAClearImage;
    PEXChangeRenderer(display,rid,item_mask, &rndAttrs);

    describe_test("\nPEXBeginPickAll with ExecuteStructure. \n");
    describe_test("No blue or yellow squares picked.\n");
    describe_test("Blue is unpickable, yellow is invisible.\n");
    describe_test("In these tests, the red primitives and the two green\n");
    describe_test("squares are also hit.  The yellow square is not drawn.\n");
    error = check_return_value(status, more_hits, count,
               PEXPick, PEXNoMoreHits,  3);

    if (!error)
    {
       highlight_all_paths(display, window, rid, count, ret_path);
       PEXFreePickPaths(count, ret_path);
       ret_path = NULL;
    }
    else
    {
       _hppex_stderr_print("ERROR: invisibility, pick_exclusion test failed\n");
       _hppex_stderr_print("ExecuteStructure case.\n");
    }


   /*
    * Now try RenderElements.  The paths will be different, because
    * RenderElements does not add an element to the renderer's current path
    * attribute.  It just bumps the offset, so there are less elements,
    * and the contents of the first element will be equal to the 
    * structure id passed into BeginPick*, with an offset equal to the
    * number of elements in the element range.
    */

    PEXBeginPickAll(display, window, rid, 0, PEXPickAllAll, False,
       10, aperture_type, &aperture);
    PEXRenderElements(display, rid, structs[0], PEXBeginning, 0,
       PEXEnd, 0);
    ret_path = PEXEndPickAll(display, rid, &status, &more_hits, &count);

    describe_test("\nPEXBeginPickAll with RenderElements. \n");
    describe_test("No blue or yellow squares are picked.\n");

    error = check_return_value(status, more_hits, count,
               PEXPick, PEXNoMoreHits, 3);

    if (!error)
    {
        highlight_all_paths(display, window, rid, count, ret_path);
        PEXFreePickPaths(count, ret_path);
        ret_path = NULL;
    }
    else
    {
        _hppex_stderr_print("ERROR: invisibility, pick_exclusion test failed.\n");
        _hppex_stderr_print("RenderElements case.\n");
    }


   /*
    * Now try PickAll.  The paths will be like the RenderElements paths,
    * except the first element will have the root structure's id.
    */
   
   /* Reset the renderer's pick start path. */
    pick_start_path.count = 1;
    start_path.structure = structs[0];
    start_path.offset = 1;
    pick_start_path.elements = &start_path;
    rndAttrs.pick_start_path = pick_start_path;
    PEXChangeRenderer(display, rid, PEXRAPickStartPath, &rndAttrs);
    
    ret_path = PEXPickAll(display, window, rid, PEXPickAllAll, 
       10, aperture_type, &aperture, &status, &more_hits, &count);

    describe_test("\nPEXPickAll. \n");
    describe_test("No blue or yellow primitives are picked.\n");


    error = check_return_value(status, more_hits, count,
               PEXPick, PEXNoMoreHits, 3);

    if (!error)
    {
        highlight_all_paths(display, window, rid, count, ret_path);
        PEXFreePickPaths(count, ret_path);
        ret_path = NULL;
    }
    else
    {
        _hppex_stderr_print("ERROR: invisibility, pick_exclusion test failed.\n");
        _hppex_stderr_print("PickAll case.\n");
    }
   
    accum_path[0].structure = structs[0];
    accum_path[0].offset = 5; 
    describe_test("\nCombination RenderElements, ExecuteStructure.\n");

    PEXBeginPickAll(display, window, rid, structs[0], PEXPickAllAll, False, 10,
       aperture_type, &aperture);

       PEXRenderElements(display, rid, structs[0], PEXBeginning, 0, PEXBeginning, 5);
       PEXBeginStructure(display, rid, structs[1]);
       PEXRenderElements(display, rid, structs[1], PEXBeginning, 0,
          PEXEnd, 0);
       PEXExecuteStructure(display, rid, PEXOCRender, structs[2]);
       PEXEndStructure(display, rid);
 
    ret_path = PEXEndPickAll(display, rid, &status, &more_hits, &count);

    error = check_return_value(status, more_hits, count, PEXPick, 
              PEXNoMoreHits, 3);


    if (!error)
    {
        highlight_all_paths(display, window, rid, count, ret_path);
        PEXFreePickPaths(count, ret_path);
        ret_path = NULL;
    }
    else
    {
        _hppex_stderr_print("ERROR: invisibility, pick_exclusion test failed.\n");
        _hppex_stderr_print(
   "Combination RenderElements and ExecuteStructure case.\n");
    }

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

int check_return_value( 
        int status, int more_hits, int count, 
        int expected_status, int expected_more_hits, int expected_count)
{
    int error=0, i, j;

    if (status != expected_status) {
       _hppex_stderr_print(
          "Incorrect status returned. \n");
       error = BAD_STATUS;
    }

    if (more_hits != expected_more_hits) {
       _hppex_stderr_print(
          "Incorrect value for more_hits returned. \n");
       error = BAD_MORE_HITS;
    }
 
    if (count != expected_count) {
       _hppex_stderr_print(
          "Incorrect count returned. \n");
       error = BAD_COUNT;
    }

    return (error);
}



void highlight_all_paths(Display *display, Drawable window, XID rid, 
        unsigned long count, PEXPickPath *ret_path)
{

int i, j, ret_path_ind, accum_ind;
XID ele_sid;
unsigned long ele_offset;
PEXElementRef accum_state_path[20];

      /* Draw the network before showing the hit. */
       PEXRenderNetwork(display, window, rid, structs[0]);
       XSync(display, True);
       testproc(display, window, current_frame++);

       for (i=0; i<count; i++, ret_path++)
       {
           if ((ret_path->elements[0].sid == 0) && 
               (ret_path->elements[0].offset == 1))
              ret_path_ind = 1;
           else 
              ret_path_ind = 0;
          
           accum_ind = 0; 

           for (j=ret_path_ind; j<ret_path->count; j++, accum_ind++)
           {
              if (ret_path->elements[j].sid == 0)
                 accum_state_path[accum_ind].structure = structs[0];
              else
                 accum_state_path[accum_ind].structure = 
                     ret_path->elements[j].sid;
              accum_state_path[accum_ind].offset = 
                 ret_path->elements[j].offset;
           }

           accum_ind = accum_ind -1;

           ele_offset = accum_state_path[accum_ind].offset;
           ele_sid = accum_state_path[accum_ind].structure;

           accum_state_path[accum_ind].offset = 
              accum_state_path[accum_ind].offset - 1;

           accum_ind++;

           PEXBeginRendering(display, window, rid);
             /* AccumulateState up to the hit. */
              PEXAccumulateState(display, rid, accum_ind, accum_state_path);

              lineColor.rgb.red = 1.0;
              lineColor.rgb.green = 1.0;
              lineColor.rgb.blue = 1.0;
              PEXSetLineColor(display, rid, PEXOCRender, PEXColorTypeRGB,
                  &lineColor);

              PEXRenderElements(display, rid, ele_sid, PEXBeginning,
                  ele_offset, PEXBeginning, ele_offset);
           PEXEndRendering(display, rid, False);

         }
         XSync(display, True);
         testproc(display, window, current_frame++);
      }


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "trv_nms_all");
    *num_images      = 1;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "trv_nms_all");
    }

void misc_setup(Display *dpy, Window test_win, PEXRenderer renderer,
		int cur_image)
    {
    window = test_win;
    _hppex_set_stderr("trv_nms_all.stderr", "trv_nms_all");
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    do_test(dpy, resourceID);
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    if (!testing_complete)
	{
        sprintf(image_name,"trv_nms_all_%d", cur_image);
        sprintf(test_name, "trv_nms_all #%d", cur_image);
         image_testproc(image_name, test_name,  img_desc,
		          dpy,window, 0, 0, 
		          glob_window_width, glob_window_height);

        if (cur_image == TOTAL_IMAGES)
	    {
            file_testproc("trv_nms_all.stderr", "trv_nms_all");
	    testing_complete = True;
	    }
	}
    }
