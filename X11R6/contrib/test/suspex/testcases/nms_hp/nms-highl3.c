/* $XConsortium: nms-highl3.c,v 1.0 93/11/22 12:29:19 rws Exp $ */

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
 * PEXlib Namesets Module:  Test Case ??  --  Highlight filter functioning
 *										  -- changing filters during rendering
 *
 */

draw_stuff1(Display *dpy, PEXRenderer rid)
{
    PEXCoord line1pts[3];
    PEXCoord line2pts[3];
    PEXCoord line3pts[3];
    PEXColor reddish, greenish, blueish;
    PEXName names[3];

    /* set points for polylines */

    line1pts[0].x=0.1;    line1pts[0].y=0.8;    line1pts[0].z=0.0;
    line1pts[1].x=0.7;    line1pts[1].y=0.8;    line1pts[1].z=0.0;
    line1pts[2].x=0.7;    line1pts[2].y=0.85;    line1pts[2].z=0.0;

    line2pts[0].x=0.1;    line2pts[0].y=0.7;    line2pts[0].z=0.0;
    line2pts[1].x=0.7;    line2pts[1].y=0.7;    line2pts[1].z=0.0;
    line2pts[2].x=0.7;    line2pts[2].y=0.75;    line2pts[2].z=0.0;

    line3pts[0].x=0.1;    line3pts[0].y=0.6;    line3pts[0].z=0.0;
    line3pts[1].x=0.7;    line3pts[1].y=0.6;    line3pts[1].z=0.0;
    line3pts[2].x=0.7;    line3pts[2].y=0.65;    line3pts[2].z=0.0;


    /* set line colors */
    reddish.rgb.red =1.0; reddish.rgb.green = 0.0;  reddish.rgb.blue = 0.0;
   greenish.rgb.red =0.0;greenish.rgb.green = 1.0; greenish.rgb.blue = 0.0;
    blueish.rgb.red =0.0; blueish.rgb.green = 0.0;  blueish.rgb.blue = 1.0;


    /* set transforms */
    local_identity(dpy, rid);
    PEXSetLineType(dpy,rid,PEXOCRender, PEXLineTypeSolid);
    
    /* draw polyline1, in red */
    names[0]=1;
    PEXAddToNameSet(dpy, rid, PEXOCRender, 1, names);
    PEXSetLineColor(dpy,rid,PEXOCRender,PEXColorTypeRGB,&reddish);
    PEXPolyline(dpy, rid, PEXOCRender, 3, line1pts);

    /* draw polyline2, in green */
    names[0]=1;
    PEXRemoveFromNameSet(dpy, rid, PEXOCRender, 1, names);
    names[0]=2;
    PEXAddToNameSet(dpy, rid, PEXOCRender, 1, names);
    PEXSetLineColor(dpy,rid,PEXOCRender,PEXColorTypeRGB,&greenish);
    PEXPolyline(dpy, rid, PEXOCRender, 3, line2pts);

    /* draw polyline3, in blue */
    /* draw polyline2, in green */
    names[0]=2;
    PEXRemoveFromNameSet(dpy, rid, PEXOCRender, 1, names);
    names[0]=3;
    PEXAddToNameSet(dpy, rid, PEXOCRender, 1, names);
    PEXSetLineColor(dpy,rid,PEXOCRender,PEXColorTypeRGB,&blueish);
    PEXPolyline(dpy, rid, PEXOCRender, 3, line3pts);
    PEXRemoveFromNameSet(dpy, rid, PEXOCRender, 1, names);


}

draw_stuff2(Display *dpy, PEXRenderer rid)
{
    PEXCoord line1pts[3];
    PEXCoord line2pts[3];
    PEXCoord line3pts[3];
    PEXColor reddish, greenish, blueish;
    PEXName names[3];

    /* set points for polylines */

    line1pts[0].x=0.1;    line1pts[0].y=0.5;    line1pts[0].z=0.0;
    line1pts[1].x=0.7;    line1pts[1].y=0.5;    line1pts[1].z=0.0;
    line1pts[2].x=0.7;    line1pts[2].y=0.55;    line1pts[2].z=0.0;

    line2pts[0].x=0.1;    line2pts[0].y=0.4;    line2pts[0].z=0.0;
    line2pts[1].x=0.7;    line2pts[1].y=0.4;    line2pts[1].z=0.0;
    line2pts[2].x=0.7;    line2pts[2].y=0.45;    line2pts[2].z=0.0;

    line3pts[0].x=0.1;    line3pts[0].y=0.3;    line3pts[0].z=0.0;
    line3pts[1].x=0.7;    line3pts[1].y=0.3;    line3pts[1].z=0.0;
    line3pts[2].x=0.7;    line3pts[2].y=0.35;    line3pts[2].z=0.0;


    /* set line colors */
    reddish.rgb.red =1.0; reddish.rgb.green = 0.0;  reddish.rgb.blue = 0.0;
   greenish.rgb.red =0.0;greenish.rgb.green = 1.0; greenish.rgb.blue = 0.0;
    blueish.rgb.red =0.0; blueish.rgb.green = 0.0;  blueish.rgb.blue = 1.0;


    /* set transforms */
    local_identity(dpy, rid);
    PEXSetLineType(dpy,rid,PEXOCRender, PEXLineTypeSolid);
    
    /* draw polyline1, in red */
    names[0]=1;
    PEXAddToNameSet(dpy, rid, PEXOCRender, 1, names);
    PEXSetLineColor(dpy,rid,PEXOCRender,PEXColorTypeRGB,&reddish);
    PEXPolyline(dpy, rid, PEXOCRender, 3, line1pts);

    /* draw polyline2, in green */
    names[0]=1;
    PEXRemoveFromNameSet(dpy, rid, PEXOCRender, 1, names);
    names[0]=2;
    PEXAddToNameSet(dpy, rid, PEXOCRender, 1, names);
    PEXSetLineColor(dpy,rid,PEXOCRender,PEXColorTypeRGB,&greenish);
    PEXPolyline(dpy, rid, PEXOCRender, 3, line2pts);

    /* draw polyline3, in blue */
    /* draw polyline2, in green */
    names[0]=2;
    PEXRemoveFromNameSet(dpy, rid, PEXOCRender, 1, names);
    names[0]=3;
    PEXAddToNameSet(dpy, rid, PEXOCRender, 1, names);
    PEXSetLineColor(dpy,rid,PEXOCRender,PEXColorTypeRGB,&blueish);
    PEXPolyline(dpy, rid, PEXOCRender, 3, line3pts);
    PEXRemoveFromNameSet(dpy, rid, PEXOCRender, 1, names);
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
    strcpy(test_name, "nms-highl3");
    *num_images      = 5;
    *supported_modes = NON_OC_MODE;

    strcpy(win_title, "nms-highl3");
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
    PEXRendererAttributes rndAttrs;
    unsigned long item_mask;
    unsigned long count;

    switch (cur_image)
	{
	case 1:
	describe_test("\nNamesets highlight filter test\n\n");
	describe_test("  (changing filters during rendering)\n\n");
        /* create the filter namesets */
        incl_filter = PEXCreateNameSet(dpy);
        excl_filter = PEXCreateNameSet(dpy);
        count =1; names[0]=1;
        PEXChangeNameSet(dpy, incl_filter, PEXNSAdd, count, names);
        PEXChangeNameSet(dpy, excl_filter, PEXNSAdd, count, names);
  
        describe_test("Notes:\n\t Namset 1 contains a red polyline.\n");
        describe_test("\t Namset 2 contains a green polyline.\n");
        describe_test("\t Namset 3 contains a blue polyline.\n");
        describe_test("\t Expected ouput will be of the form:\n");
        describe_test("\t\tInitial incl=<included sets>, excl=<excluded sets>, vis=<visible colors>\n");
        describe_test("\t\tChanged incl=<included sets>, excl=<excluded sets>, vis=<visible colors>\n");
        describe_test("\n");
  
        /* 
         * Image 1: print image with no filters set, no change
         */
  
  
        /* print the image with no filters set */
        describe_test("Image 1:\n");
        describe_test("\t\tInitial incl=NULL, excl=NULL, vis=[r,g,b]\n");
        describe_test("\t\tChanged incl=NULL, excl=NULL, vis=[r,g,b]\n");
        /* begin rendering */
        PEXBeginRendering(dpy,local_win,rid);
        draw_stuff1(dpy, rid);
        draw_stuff2(dpy, rid);
        /* end rendering */
        PEXEndRendering(dpy,rid,1);
	XSync(dpy,0);
        break;
  

        case 2:
        /* 
         * Image 2: print image with inclusion = [1],  exclusion filter = []
         *              changing to  inclusion = [1,3],  exclusion filter = []
         */
  
        /* set filters */
  
        /* Change filter to set names */
        count =1; names[0]=1;
        PEXChangeNameSet(dpy, incl_filter, PEXNSReplace, count, names);
        rndAttrs.highlight_incl = incl_filter;
        item_mask = 0;
        item_mask = PEXRAHighlightIncl;
        PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
        describe_test("Image 2:\n");
        describe_test("\t\tInitial incl=[1], excl=NULL, vis=[w,g,b]\n");
        describe_test("\t\tChanged incl=[1,3], excl=NULL, vis=[w,g,w]\n");
        /* begin rendering */
        PEXBeginRendering(dpy,local_win,rid);
        draw_stuff1(dpy, rid);
        /* Change filter to set names n middle!*/
        count =2; names[0]=1; names[1]=3;
        PEXChangeNameSet(dpy, incl_filter, PEXNSReplace, count, names);
        draw_stuff2(dpy, rid);
        /* end rendering */
        PEXEndRendering(dpy,rid,1);
	XSync(dpy,0);
        break;
   
        case 3:
        /* 
         * Image 3: print image with inclusion filter = [1,2,3]
         *                           exclusion filter = []
         */
  
        /* set filters */
  
        /* Change filter to set names */
        count =3; names[0]=1; names[1]=2; names[2]=3;
        PEXChangeNameSet(dpy, incl_filter, PEXNSReplace, count, names);
        rndAttrs.highlight_incl = incl_filter;
        item_mask = 0;
        item_mask = PEXRAHighlightIncl;
        PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
        describe_test("Image 3:\n");
        describe_test("\t\tInitial incl=[1,2,3], excl=NULL, vis=[w,w,w]\n");
        describe_test("\t\tChanged incl=NULL, excl=NULL, vis=[r,g,b]\n");
        /* begin rendering */
        PEXBeginRendering(dpy,local_win,rid);
        draw_stuff1(dpy, rid);
        /* Change filter to set names n middle!*/
        count =3; names[0]=1; names[1]=2; names[2]=3;
        PEXChangeNameSet(dpy, incl_filter, PEXNSRemove, count, names);
        draw_stuff2(dpy, rid);
        /* end rendering */
        PEXEndRendering(dpy,rid,1);
	XSync(dpy,0);
        break;
     
        case 4:
        /*  
         * Image 4: print image with inclusion filter = [1,3]
         *                           exclusion filter = [2]
         */
  
        /* set filters */
  
        /* Change filter to set names */
        count =2; names[0]=1; names[1]=3; 
        PEXChangeNameSet(dpy, incl_filter, PEXNSReplace, count, names);
        count =1; names[0]=2; 
        PEXChangeNameSet(dpy, excl_filter, PEXNSReplace, count, names);
        rndAttrs.highlight_incl = incl_filter;
        rndAttrs.highlight_excl = excl_filter;
        item_mask = 0;
        item_mask = PEXRAHighlightIncl|PEXRAHighlightExcl;
        PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
        describe_test("Image 4:\n");
        describe_test("\t\tInitial incl=[1,3], excl=[2], vis=[w,g,w]\n");
        describe_test("\t\tChanged incl=[1,3], excl=[2,3], vis=[w,g,b]\n");
        /* begin rendering */
        PEXBeginRendering(dpy,local_win,rid);
        draw_stuff1(dpy, rid);
        count =2; names[0]=2; names[0]=3;
        PEXChangeNameSet(dpy, excl_filter, PEXNSReplace, count, names);
        draw_stuff2(dpy, rid);
        /* end rendering */
        PEXEndRendering(dpy,rid,1);
	XSync(dpy,0);
        break;
 
	case 5:
        /* 
         * Image 5: print image with inclusion filter = [1,2,3]
         *                           exclusion filter = [1]
         */
  
        /* set filters */
  
        /* Change filter to set names */
        count =3; names[0]=1; names[1]=2; names[2]=3; 
        PEXChangeNameSet(dpy, incl_filter, PEXNSReplace, count, names);
        PEXChangeNameSet(dpy, excl_filter, PEXNSReplace, count, names);
        rndAttrs.highlight_incl = incl_filter;
        rndAttrs.highlight_excl = excl_filter;
        item_mask = 0;
        item_mask = PEXRAHighlightIncl|PEXRAHighlightExcl;
        PEXChangeRenderer(dpy,rid,item_mask,&rndAttrs);
        describe_test("Image 5:\n");
        describe_test("\t\tInitial incl=[1,2,3], excl=[1,2,3], vis=[r,g,b]\n");
        describe_test("\t\tChanged incl=[1,2,3], excl=NULL, vis=[w,w,w]\n");
        /* begin rendering */
        PEXBeginRendering(dpy,local_win,rid);
        draw_stuff1(dpy, rid);
        PEXChangeNameSet(dpy, excl_filter, PEXNSRemove, count, names);
        draw_stuff2(dpy, rid);
        /* end rendering */
        PEXEndRendering(dpy,rid,1);
	XSync(dpy,0);
  
	PEXFreeRenderer(dpy,rid);
	PEXFreeNameSet(dpy,incl_filter);
	PEXFreeNameSet(dpy,excl_filter);
	break;

	default:
	fprintf(stderr, "Invalid current image passed to execute_test\n");
	}
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    sprintf(image_name,"nms-highl3_%d", cur_image);
    sprintf(test_name, "nms-highl3 #%d", cur_image);
    image_testproc(image_name, test_name, img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
