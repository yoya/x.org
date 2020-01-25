/* $XConsortium: nms-addrm1.c,v 1.0 93/11/22 12:29:12 rws Exp $ */

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
 * PEXlib Namesets Module:  Test Case ??  --  Add/Remove current nameset funct. 
 *                             Black box version
 *
 */

Window   local_win;
PEXCoord linepts[10][3];
PEXColor reddish, greenish, blueish, yellowish, magentaish;


draw_stuff(Display *dpy, PEXRenderer rid)
{

    /* set points for polylines */

    linepts[0][0].x=0.1;    linepts[0][0].y=0.9;    linepts[0][0].z=0.0;
    linepts[0][1].x=0.3;    linepts[0][1].y=0.9;    linepts[0][1].z=0.0;
    linepts[0][2].x=0.3;    linepts[0][2].y=0.95;   linepts[0][2].z=0.0;

    linepts[1][0].x=0.1;    linepts[1][0].y=0.7;    linepts[1][0].z=0.0;
    linepts[1][1].x=0.3;    linepts[1][1].y=0.7;    linepts[1][1].z=0.0;
    linepts[1][2].x=0.3;    linepts[1][2].y=0.75;   linepts[1][2].z=0.0;

    linepts[2][0].x=0.1;    linepts[2][0].y=0.5;    linepts[2][0].z=0.0;
    linepts[2][1].x=0.3;    linepts[2][1].y=0.5;    linepts[2][1].z=0.0;
    linepts[2][2].x=0.3;    linepts[2][2].y=0.55;   linepts[2][2].z=0.0;

    /* set line colors */
    reddish.rgb.red =1.0; reddish.rgb.green = 0.0;  reddish.rgb.blue = 0.0;
   greenish.rgb.red =0.0;greenish.rgb.green = 1.0; greenish.rgb.blue = 0.0;
    blueish.rgb.red =0.0; blueish.rgb.green = 0.0;  blueish.rgb.blue = 1.0;

    /* set transforms */
    /*global_translate(); */
    local_identity();
    PEXSetLineType(dpy,rid,PEXOCRender, PEXLineTypeSolid);
    
    /* draw polyline1, in red */
    PEXSetLineColor(dpy,rid,PEXOCRender,PEXColorTypeRGB,&reddish);
    PEXPolyline(dpy, rid, PEXOCRender, 3, linepts[0]);
    /* draw polyline2, in green */
    PEXSetLineColor(dpy,rid,PEXOCRender,PEXColorTypeRGB,&greenish);
    PEXPolyline(dpy, rid, PEXOCRender, 3, linepts[1]);
    /* draw polyline3, in blue */
    PEXSetLineColor(dpy,rid,PEXOCRender,PEXColorTypeRGB,&blueish);
    PEXPolyline(dpy, rid, PEXOCRender, 3, linepts[2]);
}

local_identity(Display *dpy, PEXRenderer rid)
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

    PEXSetLocalTransform(dpy, rid, PEXOCRender, PEXReplace,
                         scale_matrix);
}

void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "nms-addrm1");
    *num_images      = 17;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "nms-addrm1");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    local_win = window;
    }

PEXNameSet incl_filter, excl_filter;
PEXName names[3];

void execute_test(Display *dpy, XID rid, 
	       PEXOCRequestType req_type, int cur_image)
    {
    PEXNameSet incl_filter, excl_filter;
    PEXRendererAttributes rndAttrs;
    unsigned long item_mask;
    PEXName names[10];
    unsigned long count;

    switch (cur_image)
	{
	case 1:
        /* create the filter namesets */
        incl_filter = PEXCreateNameSet(dpy);
        excl_filter = PEXCreateNameSet(dpy);
        count = (unsigned long) 2; 
	    names[0]= (unsigned long) 1; names[1]= (unsigned long) 63;
        PEXChangeNameSet(dpy, excl_filter, PEXNSAdd, count, names);
        
        count = (unsigned long) 5; 
	     names[0]= (unsigned long) 0;  names[1]= (unsigned long) 2; 
	     names[2]= (unsigned long) 32; names[3]= (unsigned long) 42; 
	     names[4]= (unsigned long) 64;
	    PEXChangeNameSet(dpy, incl_filter, PEXNSAdd, count, names);
  
        /* begin rendering */
        PEXBeginRendering(dpy,local_win,rid);
  
        describe_test("Notes:\n\t  there are 3 polylines (r,g,b) from top to bottom.\n");
        describe_test("Notes:\n\t  the inclusion set is initially NULL\n");
        describe_test("\n Initial state:\n");
        describe_test("\t\t set=NULL\tvis=[None]\n");
  
  
        /* 
         * Image 1: print image with no names set, no filters set. 
         */

        /* print the image with no filters set */
        describe_test("Image 1:\n");
        describe_test("\t\tadded=NULL, rem=NULL, set=NULL\tvis=[r,g,b]\n");
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

	case 2:
        /* 
         * Image 2: 
         *                           
         */
  
        /* set filters */
        describe_test("Changing filters, placing even namesets in inclusion set\n");
        describe_test("Changing filters, placing namesets [1, 63] in exclusion set\n");
  
  
  
        rndAttrs.highlight_incl = incl_filter;
        rndAttrs.highlight_excl = excl_filter;
        item_mask = 0;
        item_mask = PEXRAHighlightIncl|PEXRAHighlightExcl;
        PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
  
        describe_test("Image 2:\n");
        describe_test("\t\tadded=[0,2], rem=NULL, set=[0,2]\tvis=[w,w,w]\n");
        count=2; names[0]=0; names[1]=2;
        PEXAddToNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

	case 3:
	/* 
	 * Image 3: still same size
	 */

        describe_test("Image 3:\n");
        describe_test("\t\tadded=[3,31], rem=NULL, set=[0,2,3,31]\tvis=[w,w,w]\n");
        count=2; names[0]=3; names[1]=31;
        PEXAddToNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

	case 4:
	/* 
	 * Image 4: still same size
	 */

        describe_test("Image 4:\n");
        describe_test("\t\tadded=[1], rem=NULL, set=[0,1, 2,3,31]\tvis=[r,g,b]\n");
        count=1; names[0]=1; 
        PEXAddToNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

        case 5:
	/* 
	 * Image 5: still same size, removing
	 */
        
	describe_test("Image 5:\n");
        describe_test("\t\tadded=[], rem=[3], set=[0,1, 2,31]\tvis=[r,g,b]\n");
        count=1; names[0]=3; 
        PEXRemoveFromNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

	case 6:
	/* 
	 * Image 6: still same size, removing
	 */

        describe_test("Image 6:\n");
        describe_test("\t\tadded=[], rem=[1], set=[0, 2,31]\tvis=[w,w,w]\n");
        count=1; names[0]=1; 
        PEXRemoveFromNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
	XSync(dpy,0);
        break;

	case 7:
	/* 
	 * Image 7: still same size, adding what is already there, 
	 *          should be  no change
	 */

        describe_test("Image 7:\n");
        describe_test("\t\tadded=[0,2,31], rem=[], set=[0, 2,31]\tvis=[w,w,w]\n");
        count=3; names[0]=0; names[1]=2; names[2]=31;
        PEXAddToNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;


	case 8:
	/* 
	 * Image 8: still same size, removing  what is not there, 
	 *          should be  no change
	 */

        describe_test("Image 8:\n");
        describe_test("\t\tadded=[], rem=[1,3,5], set=[0, 2,31]\tvis=[w,w,w]\n");
        count=3; names[0]=1; names[1]=3; names[2]=5;
        PEXRemoveFromNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

	case 9:
	/* 
	 * Image 9: still same size, removing bit that is enabling highlighting , 
	 */

        describe_test("Image 9:\n");
        describe_test("\t\tadded=[], rem=[0,2], set=[31]\tvis=[r,g,b]\n");
        count=2; names[0]=0; names[1]=2; 
        PEXRemoveFromNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

	case 10:
	/* 
	 * Image 10: removing all  
	 */

        describe_test("Image 9:\n");
        describe_test("\t\tadded=[], rem=[31], set=[]\tvis=[r,g,b]\n");
        count=1; names[0]=31; 
        PEXRemoveFromNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

	case 11:
	/* 
	 * Image 11: adding back in to emptied set  
	 */

        describe_test("Image 11:\n");
        describe_test("\t\tadded=[0,3], rem=[], set=[0,3]\tvis=[w,w,w]\n");
        count=2; names[0]=0; names[1]=3;
        PEXAddToNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

	case 12:
	/* 
	 * Image 12:   grow the fool set
	 */

        describe_test("Image 12:\n");
        describe_test("\t\tadded=[63,64], rem=[], set=[0,3,63,64]\tvis=[r,g,b]\n");
        count=2; names[0]=63;  names[1]=64;
        PEXAddToNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

	case 13:
	/* 
	 * Image 13:   remove something from bottom of nameset 
	 */

        describe_test("Image 13:\n");
        describe_test("\t\tadded=[], rem=[0], set=[3,63,64]\tvis=[r,g,b]\n");
        count=1; names[0]=0; 
        PEXRemoveFromNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

	case 14:
	/* 
	 * Image 14:   remove something from top of nameset
	 */

        describe_test("Image 14:\n");
        describe_test("\t\tadded=[], rem=[64], set=[3,63]\tvis=[r,g,b]\n");
        count=1; names[0]=64; 
        PEXRemoveFromNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

	case 15:
	/* 
	 * Image 15:   add something to top of nameset
	 */

        describe_test("Image 15:\n");
        describe_test("\t\tadded=[66], rem=[], set=[3,63,66]\tvis=[r,g,b]\n");
        count=1; names[0]=66; 
        PEXAddToNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

	case 16:
	/* 
	 * Image 16:   add something to bottom of nameset
	 */
	describe_test("Image 16:\n");
        describe_test("\t\tadded=[4], rem=[], set=[4,3,63,66]\tvis=[r,g,b]\n");
        count=1; names[0]=4; 
        PEXAddToNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        break;

        case 17:
	/* 
	 * Image 17:   remove all, add something small again
	 */

        describe_test("Image 17:\n");
        describe_test("\t\tadded=[1], rem=[4,3,63,66], set=[1]\tvis=[r,g,b]\n");
        count=4; names[0]=4; names[1]=3; names[2]=63, names[3]=66;
        PEXRemoveFromNameSet(dpy,rid,PEXOCRender, count,names);
        count=1; names[0]=1; 
        PEXAddToNameSet(dpy,rid,PEXOCRender, count,names);
        draw_stuff(dpy, rid);
        XSync(dpy,0);
        PEXEndRendering(dpy,rid,1);
        break;

	default:
	fprintf(stderr, "Invalid current image passed to execute_test\n");
	}
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    sprintf(image_name,"nms-addrm1_%d", cur_image);
    sprintf(test_name, "nms-addrm1 #%d", cur_image);
    image_testproc(image_name, test_name, img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
