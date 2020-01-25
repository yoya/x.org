/* $XConsortium: trv_disp.c,v 1.0 93/11/22 12:40:29 rws Exp $ */

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

Window local_win;
     
global_translate(display, id, trans_vect)
    Display *display;
    XID      id;
    PEXVector trans_vect;

    {
    PEXMatrix xlate_matrix;
    
    PEXTranslate(&trans_vect, xlate_matrix);
    PEXSetGlobalTransform(display, id, PEXOCStore, xlate_matrix);
    }


local_translate(display, id, trans_vect)
    Display *display;
    XID      id;
    PEXVector trans_vect;

    {
    PEXMatrix xlate_matrix;
    
    PEXTranslate(&trans_vect, xlate_matrix);
    PEXSetLocalTransform(display, id, PEXOCStore, PEXPreConcatenate, 
       xlate_matrix);
    }




pline_square(display, id)

    Display *display;
    XID      id;

    {
    PEXCoord pts[5];

    pts[ 0].x =  0.05; pts[ 0].y =  0.05; pts[ 0].z =  0.0;
    pts[ 1].x =  0.15; pts[ 1].y =  0.05; pts[ 1].z =  0.0;
    pts[ 2].x =  0.15; pts[ 2].y =  0.15; pts[ 2].z =  0.0;
    pts[ 3].x =  0.05; pts[ 3].y =  0.15; pts[ 3].z =  0.0;
    pts[ 4].x =  0.05; pts[ 4].y =  0.05; pts[ 4].z =  0.0;

    PEXPolyline(display, id, PEXOCStore, 5, pts);
    }

local_identity(display, id)
    
    Display *display;
    XID      id;

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
    strcpy(test_name, "trv_disp");
    *num_images      = 8;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "trv_disp");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    local_win = window;
    }

PEXStructure structs[5];

void execute_test(Display *display, XID rid, 
	       PEXOCRequestType req_type, int cur_image)
    {
    PEXVector trans_vector;
    PEXElementRef accum_state_path[3];
    PEXColor lineColor;
    PEXRendererAttributes rndAttrs;
    unsigned long         item_mask;

    switch (cur_image)
	{
	case 1:
        /* Build structs[3] */

        structs[3] = PEXCreateStructure(display);

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
    
        trans_vector.x = 0.1;
        trans_vector.y = 0.0;
        trans_vector.z = 0.0;
        global_translate(display, structs[0], trans_vector); 
        pline_square(display, structs[0]);
        PEXExecuteStructure(display, structs[0], PEXOCStore, structs[1]);
        PEXExecuteStructure(display, structs[0], PEXOCStore, structs[2]);
 
        /*  Render entire structure network */
        describe_test("RenderNetworks test.\n");
        describe_test("This test draws three columns of squares, bottom to top:\n");
        describe_test("Column 1: 1 red square, 2 blue squares.\n");
        describe_test("Column 2: 2 green squares, 2 blue squares.\n");
        describe_test("Column 3: 1 yellow square.\n");
        PEXRenderNetwork(display, local_win, rid, structs[0]);
	break;

	case 2:
        /*
            Render all of the elements in the root structure.  This image should
            be the same as the previous image.
        */

        PEXBeginRendering(display, local_win, rid);
        PEXRenderElements(display, rid, structs[0], PEXBeginning, 0, PEXEnd, 0);
        PEXEndRendering(display, rid, False);

        describe_test("\nRenderElements test, range includes all elements in root structure.\n");
        describe_test("This test draws three columns of squares, bottom to top:\n");
        describe_test("Column 1: 1 red square, 2 blue squares.\n");
        describe_test("Column 2: 2 green squares, 2 blue squares.\n");
        describe_test("Column 3: 1 yellow square.\n");
	break;

	case 3:
        /*  
            A RenderElements mainline test. 
        */
        describe_test("\nUse three separate RenderElements calls to render the network.\n");
        describe_test("This test draws three columns of squares, bottom to top:\n");
        describe_test("Column 1: 1 red square, 2 blue squares.\n");
        describe_test("Column 2: 2 green squares, 2 blue squares.\n");
        describe_test("Column 3: 1 yellow square.\n");
  
        PEXBeginRendering(display, local_win, rid);
        PEXRenderElements(display, rid, structs[0], PEXBeginning, 0, PEXEnd, -2);
        PEXRenderElements(display, rid, structs[0], PEXEnd, -2, PEXEnd, -1);
        PEXRenderElements(display, rid, structs[0], PEXEnd, -1, PEXEnd, 0);
        PEXEndRendering(display, rid, False);
	break;

	case 4:
        /*  
            An ExecuteStructure mainline test.
            Render each of the structures by making an immediate mode 
            ExecuteStructure call.
        */

        describe_test("\nA series of tests using ExecuteStructure.\n\n");
        describe_test("Not all of the structure elements will be in the same\n");
        describe_test("positions in the window, as in the previous images.\n");

        describe_test("\nThe structure containing the yellow square.\n");
        PEXBeginRendering(display, local_win, rid);
        PEXExecuteStructure(display, rid, PEXOCRender, structs[3]);
        PEXEndRendering(display, rid, False);
	break;

	case 5:
        describe_test("\nThe structure containing the two blue squares.\n");
        describe_test("The blue squares are on the left side of the window.\n");
        PEXBeginRendering(display, local_win, rid);
        PEXExecuteStructure(display, rid, PEXOCRender, structs[2]);
        PEXEndRendering(display, rid, False);
	break;

	case 6:
        describe_test("\nA structure that executes another structure.\n");
        describe_test("First column: two green squares, two blue squares.\n");
        describe_test("<Space between the columns>.\n");
        describe_test("One yellow square.\n");
        PEXBeginRendering(display, local_win, rid);
        PEXExecuteStructure(display, rid, PEXOCRender, structs[1]);
        PEXEndRendering(display, rid, False);
	break;

	case 7:
        describe_test("\nImmediate mode execution of the root structure.\n");
        describe_test("This test draws three columns of squares, bottom to top:\n");
        describe_test("Column 1: 1 red square, 2 blue squares.\n");
        describe_test("Column 2: 2 green squares, 2 blue squares.\n");
        describe_test("Column 3: 1 yellow square.\n");
        PEXBeginRendering(display, local_win, rid);
        PEXExecuteStructure(display, rid, PEXOCRender, structs[0]);
        PEXEndRendering(display, rid, False);
	break;

	case 8:
        describe_test(
         "\nCombination AccumulateState/RenderEle/ExecuteStructure/RenderElements.\n");
        describe_test(
	 "This test draws three columns of squares, bottom to top:\n");
        describe_test("Column 1: 1 red square, 2 blue squares.\n");
        describe_test("Column 2: 2 green squares, 2 blue squares.\n");
        describe_test("Column 3: 1 yellow square.\n");

        accum_state_path[0].structure = structs[0];
        accum_state_path[0].offset = 4;
        accum_state_path[1].structure = structs[1];
        accum_state_path[1].offset = 2;

        rndAttrs.clear_image = False;
        rndAttrs.clear_z = False;
        PEXChangeRenderer(display, rid, PEXRAClearImage|PEXRAClearZ, &rndAttrs);

        PEXBeginRendering(display, local_win, rid);
        PEXAccumulateState(display, rid, 2, accum_state_path);
        PEXRenderElements(display, rid, structs[1], PEXBeginning, 3,
                          PEXBeginning, 5);
        PEXExecuteStructure(display, rid, PEXOCRender, structs[2]);
        PEXExecuteStructure(display, rid, PEXOCRender, structs[3]);
        PEXRenderElements(display, rid, structs[2], PEXBeginning, 0, PEXEnd, 0);
        PEXEndRendering(display, rid, True);
	break;

	default:
	describe_test("Invalid current image passed to execute_test\n");
	}
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    sprintf(image_name,"trv_disp_%d", cur_image);
    sprintf(test_name, "trv_disp #%d", cur_image);
     image_testproc(image_name, test_name,  img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
