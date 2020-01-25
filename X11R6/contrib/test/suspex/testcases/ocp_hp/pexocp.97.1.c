/* $XConsortium: pexocp.97.1.c,v 1.0 93/11/22 12:37:32 rws Exp $ */

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
#include <misc.h>

/* 
********************************************************************
** OVERVIEW:
**
** Test cases: RGB/Vertex, Edge Flags/Vertex, Facet/Set
**             Edges ON/OFF, Facet BF Cull ON/OFF.
**             Immediate Mode and Structure Mode.
**
********************************************************************
*/

/* DEFINE DATA */

static PEXVertexRGB vertices[] =
{
       0.10, 0.10, 0.00, 1.00, 0.00, 0.00,
       0.30, 0.10, 0.00, 0.00, 1.00, 0.00,
       0.30, 0.30, 0.00, 0.00, 0.00, 1.00,
       0.10, 0.30, 0.00, 1.00, 1.00, 1.00,

       0.70, 0.10, 0.00, 1.00, 0.00, 0.00,
       0.90, 0.10, 0.00, 0.00, 1.00, 0.00,
       0.90, 0.30, 0.00, 0.00, 0.00, 1.00,
       0.70, 0.30, 0.00, 1.00, 1.00, 1.00,

       0.70, 0.70, 0.00, 1.00, 0.00, 0.00,
       0.90, 0.70, 0.00, 0.00, 1.00, 0.00,
       0.90, 0.90, 0.00, 0.00, 0.00, 1.00,
       0.70, 0.90, 0.00, 1.00, 1.00, 1.00,

       0.10, 0.70, 0.00, 1.00, 0.00, 0.00,
       0.30, 0.70, 0.00, 0.00, 1.00, 0.00,
       0.30, 0.90, 0.00, 0.00, 0.00, 1.00,
       0.10, 0.90, 0.00, 1.00, 1.00, 1.00,

       0.45, 0.45, 0.50, 0.00, 0.00, 0.00,
       0.50, 0.25, 0.50, 1.00, 0.00, 0.00,
       0.55, 0.45, 0.50, 0.00, 0.00, 0.00,
       0.75, 0.50, 0.50, 0.00, 1.00, 0.00,
       0.55, 0.55, 0.50, 0.00, 0.00, 0.00,
       0.50, 0.75, 0.50, 0.00, 0.00, 1.00,
       0.45, 0.55, 0.50, 0.00, 0.00, 0.00,
       0.25, 0.50, 0.50, 1.00, 1.00, 1.00,

       0.40, 0.40, 0.00, 0.00, 1.00, 1.00,
       0.60, 0.40, 0.00, 0.00, 1.00, 1.00,
       0.60, 0.60, 0.00, 1.00, 0.00, 1.00,
       0.40, 0.60, 0.00, 1.00, 0.00, 1.00,
};

#define NVERTICES (sizeof(vertices)/sizeof(PEXVertexRGB))

static PEXVertexNormal vertices2[] =
{
       0.47, 0.47, 0.00, 0.00, 0.00, 1.00,
       0.53, 0.47, 0.00, 0.00, 0.00, 1.00,
       0.53, 0.53, 0.00, 0.00, 0.00, 1.00,
       0.47, 0.53, 0.00, 0.00, 0.00, 1.00,
};

#define NVERTICES2 (sizeof(vertices2)/sizeof(PEXVertexNormal))

static unsigned short poly1[] = {  0,  1,  2,  3 };
static unsigned short poly2[] = {  4,  5,  6,  7 };
static unsigned short poly3[] = {  8,  9, 10, 11 };
static unsigned short poly4[] = { 12, 13, 14, 15 };
static unsigned short poly5[] = { 16, 17, 18, 19, 20, 21, 22, 23 };
static unsigned short poly6[] = { 24, 25, 26, 27 };

#define NPOLY1 (sizeof(poly1)/sizeof(unsigned short))
#define NPOLY2 (sizeof(poly2)/sizeof(unsigned short))
#define NPOLY3 (sizeof(poly3)/sizeof(unsigned short))
#define NPOLY4 (sizeof(poly4)/sizeof(unsigned short))
#define NPOLY5 (sizeof(poly5)/sizeof(unsigned short))
#define NPOLY6 (sizeof(poly6)/sizeof(unsigned short))

static PEXSwitch edge_flags[] =
{
    /* poly1 */ 0,1,0,1,
    /* poly2 */ 0,1,0,1,
    /* poly3 */ 0,1,0,1,
    /* poly4 */ 0,1,0,1,
    /* poly5 */ 0,1,0,1,0,1,0,1,
    /* poly6 */ 0,1,0,1,
};

#define NEDGEFLAGS (sizeof(edge_flags)/sizeof(PEXSwitch))

static PEXListOfUShort fillarea1[] =
{
    NPOLY1, poly1,
    NPOLY2, poly2,
    NPOLY3, poly3,
    NPOLY4, poly4,
};

#define NFILLAREA1 (sizeof(fillarea1)/sizeof(PEXListOfUShort))

static PEXListOfUShort fillarea2[] =
{
    NPOLY5, poly5,
    NPOLY6, poly6,
};

#define NFILLAREA2 (sizeof(fillarea2)/sizeof(PEXListOfUShort))

static PEXListOfUShort fillarea3[] =
{
    NPOLY1, poly1
};

#define NFILLAREA3 (sizeof(fillarea3)/sizeof(PEXListOfUShort))

static PEXConnectivityData connect_data[] =
{
    NFILLAREA1, fillarea1,
    NFILLAREA2, fillarea2,
};

#define NSETS (sizeof(connect_data)/sizeof(PEXConnectivityData))

static PEXConnectivityData connect_data2[] =
{
    NFILLAREA3, fillarea3,
    NFILLAREA3, fillarea3,
    NFILLAREA3, fillarea3,
    NFILLAREA3, fillarea3,
    NFILLAREA3, fillarea3,
    NFILLAREA3, fillarea3,
    NFILLAREA3, fillarea3,
    NFILLAREA3, fillarea3,
    NFILLAREA3, fillarea3,
    NFILLAREA3, fillarea3,
};

#define NSETS2 (sizeof(connect_data2)/sizeof(PEXConnectivityData))

static PEXVector facets[NSETS] =
{
    0.0, 0.0, 1.0,
    0.0, 0.0, -1.0,
};

/* PRIMITIVE DRAW SUBROUTINE */

static void DrawSOFAS(
    Display             *dpy,
    XID                 rid,
    PEXOCRequestType    reqType,
    int                 edgeFlag,
    int                 bfCull
)
{
    PEXArrayOfVertex    pVertices;
    PEXArrayOfFacetData pFacets;
    PEXColor            edge_color;
    PEXColor            surface_color;
    int                 i;

    PEXSetInteriorStyle(dpy,rid,reqType,PEXInteriorStyleSolid);
    PEXSetSurfaceInterpMethod(dpy,rid,reqType,PEXSurfaceInterpColor);

    if (bfCull) 
        PEXSetFacetCullingMode(dpy,rid,reqType,PEXOn);
    else
        PEXSetFacetCullingMode(dpy,rid,reqType,PEXOff);
  
    if (edgeFlag)
    {
        PEXSetSurfaceEdgeFlag(dpy,rid,reqType,PEXOn);
        edge_color.rgb.red = 1.0;
        edge_color.rgb.green = 1.0;
        edge_color.rgb.blue = 0.0;
        PEXSetSurfaceEdgeColor(dpy,rid,reqType,PEXColorTypeRGB,&edge_color);
    }

    surface_color.rgb.red = 0.0;
    surface_color.rgb.green = 0.0;
    surface_color.rgb.blue = 1.0;
    PEXSetSurfaceColor(dpy,rid,reqType,PEXColorTypeRGB,&surface_color);
  
    for (i = 0; i < 30; i++)
    {
        /* Appease the ANSI-C gods */

		pFacets.index = (PEXColorIndexed *) NULL;
        pVertices.normal = vertices2;

        PEXSetOfFillAreaSets(dpy,rid,reqType,
            PEXShapeNonConvex,
            PEXGANone,         /* Facet */
            PEXGANormal,       /* Vertex */
            PEXGANone,         /* Edge */
            PEXContourDisjoint,
            False,
            PEXColorTypeRGB,
            NSETS2,
            pFacets,           /* NULL!! */
            NVERTICES2,
            pVertices,
            NSETS2*NPOLY1,
            NULL,
            connect_data2
        );

        /* Appease the ANSI-C gods */

        pVertices.rgb = vertices;
    	pFacets.normal = facets;

        PEXSetOfFillAreaSets(dpy,rid,reqType,
            PEXShapeNonConvex,
            PEXGANormal,       /* Facet */
            PEXGAColor,        /* Vertex */
            PEXGAEdges,        /* Edge */
            PEXContourDisjoint,
            False,
            PEXColorTypeRGB,
            NSETS,
            pFacets,
            NVERTICES,
            pVertices,
            NEDGEFLAGS,
            edge_flags,
            connect_data
        );
    }

    XSync(dpy,0);
}


void inquire_test_params(char *test_name, 
			 int  *num_images, 
			 int  *supported_modes, 
			 char *win_title)
    {
    strcpy(test_name, "pexocp.97.1");
    *num_images      = 2;
    *supported_modes = IMM_MODE | STR_MODE | WKS_MODE;

    strcpy(win_title, "pexocp.97.1");
    }

void misc_setup(Display *dpy, Window window, PEXRenderer renderer,
		int cur_image)
    {
    unsigned long         mask = PEXRAHLHSRMode;
    PEXRendererAttributes attrs;

    attrs.hlhsr_mode = PEXHLHSRZBuffer;
    PEXChangeRenderer(dpy, renderer, mask, &attrs);
    }

void execute_test(Display *dpy, XID resourceID, 
	       PEXOCRequestType req_type, int cur_image)
    {
    switch (cur_image)
	{
	case 1:
        describe_test("\nSofas test image #1\n\n");
        describe_test("Edges on -- culling off\n\n");
        describe_test(" The image you should see consists of 4 interpolated\n");
        describe_test(" squares with yellow vertical edges in the window corners.\n");
        describe_test(" They are colored as follows:\n");
        describe_test("     white            blue\n");
        describe_test("     red              green\n");
        describe_test(" In the center of the image, a multi color shaded\n");
        describe_test(" star with alternating yellow edges lies on top of\n");
        describe_test(" multi color square with yellow vertical edges. \n");
        describe_test(" edges.   The intersection of these is black.\n");
        describe_test(" In the very center is a small blue square with\n");
        describe_test(" white edges.\n\n");
        DrawSOFAS(dpy, resourceID, req_type, True, False);
	break;

	case 2:
        describe_test("\nSofas test image #2\n\n");
        describe_test("Edges off -- culling on\n\n");
        describe_test(" The image you should see consists of 4 interpolated\n");
        describe_test(" squares without edges in the window corners.\n");
        describe_test(" They are colored as follows:\n");
        describe_test("     white            blue\n");
        describe_test("     red              green\n");
        describe_test(" In the center of the image there should be a small\n");
        describe_test(" blue square without edges.\n\n");
        DrawSOFAS(dpy, resourceID, req_type, False, True);
	break;

	default:
	_hppex_stderr_print("Invalid current image passed to execute_test\n");
	}
    }

void testproc(Display *dpy, Window window, int cur_image)
    {
    char image_name[80], test_name[80];

    sprintf(image_name,"pexocp.97.1_%d", cur_image);
    sprintf(test_name, "pexocp.97.1 #%d", cur_image);
     image_testproc(image_name, test_name,  img_desc,
		      dpy,window, 0, 0, 
		      glob_window_width, glob_window_height);
    }
