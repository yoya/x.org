/* $XConsortium: miSOFAS.c,v 5.2 91/05/01 14:43:04 hersh Exp $ */


/***********************************************************
Copyright 1989, 1990, 1991 by Sun Microsystems, Inc. and the X Consortium.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Sun Microsystems,
the X Consortium, and MIT not be used in advertising or publicity
pertaining to distribution of the software without specific, written
prior permission.

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
#include "mipex.h"
#include "misc.h"
#include "miscstruct.h"
#include "ddpex3.h"
#include "PEXErr.h"
#include "miStruct.h"
#include "PEXprotost.h"
#include "miRender.h"
#include "extnsionst.h"
#include "gcstruct.h"
#include "ddpex2.h"
#include "miLight.h"

#include <stdio.h>

/*++
 |
 |  Function Name:	miSOFAS
 |
 |  Function Description:
 |
 |  Note(s):
 |
 --*/

ddpex3rtn
miSOFAS(pRend, pExecuteOC)
/* in */
    ddRendererPtr       pRend;	  /* renderer handle */
    miGenericStr       *pExecuteOC;
/* out */
{
/* calls */
      extern ocTableType	InitExecuteOCTable[];

/* Local variable definitions */
      miSOFASStruct	*ddSOFAS = (miSOFASStruct *)(pExecuteOC+1);
      miFillAreaStruct	*ddFill;
      miGenericStr	*pGStr;
      listofddFacet	*input_facet = &ddSOFAS->pFacets;    /* facets */
      miConnHeader	*index_list_hdr = &ddSOFAS->connects; /* vertex index */
      miConnListList	*index_list_list = index_list_hdr->data;
      miConnList	*index_list;
      ddUSHORT		*index;
      ddUCHAR		*edge_ptr = ddSOFAS->edgeData;
      miDDContext	*pddc = (miDDContext *)(pRend->pDDContext);
      miListHeader	*fillarea;
      listofddFacet	*fillareafacet;
      listofddPoint	*pddilist, *pddolist;
      char		*in_pt;
      ddPointUnion	out_pt;
      ddPointer		in_fct, out_fct;
      int		i, j, k;
      int		point_size, out_point_size, facet_size;
      ddpex3rtn		status = Success;

      /*
       * This implementation does not implement SOFAS directly.
       * Instead, the SOFAS data is re-organized to describe a
       * fill area set, and the fill area set code is then called
       * to perform the rendering.
       */
      /* Allocate storage for the fill area command block */
      if (!(pGStr = (miGenericStr *) (Xalloc(sizeof(miGenericStr) +
                                             sizeof(miFillAreaStruct)))))
	return(BadAlloc);

      pGStr->elementType = PEXOCFillAreaSet;
      /* The length data is ignored by the rendering routine and hence is */
      /* left as whatever GARBAGE that will be present at the alloc time. */

      ddFill = (miFillAreaStruct *) (pGStr + 1);

      /* Initialize constant part of fill area command structure */
      ddFill->shape = ddSOFAS->shape;		/* shape hint */
      ddFill->ignoreEdges = PEXOff;		/* edge flag */
      ddFill->contourHint = ddSOFAS->contourHint;/* contour hint */
      ddFill->points.type = ddSOFAS->points.type;
      if (ddSOFAS->edgeAttribs) DD_SetVertEdge(ddFill->points.type);
      ddFill->points.flags = ddSOFAS->points.flags;
      ddFill->pFacets = NULL;

      DD_VertPointSize( ddSOFAS->points.type, point_size);
      DD_VertPointSize( ddFill->points.type,  out_point_size);
      DDFacetSIZE( input_facet->type, facet_size);

      /* Only one input vertex list */
      pddilist = ddSOFAS->points.ddList;
      in_pt = pddilist->pts.ptr;
      if (input_facet->type != DD_FACET_NONE) 
        in_fct = input_facet->facets.pNoFacet;
      else in_fct = NULL;

      /* one fill area set call per facet */
      for (i = 0; i < ddSOFAS->numFAS; i++) {

	/* get next set of vertex indices */
	index_list = index_list_list->pConnLists;
        ddFill->points.numLists = index_list_list->numLists;

	/* Get next free vertex list header */
	fillarea = MI_NEXTTEMPDATALIST(pddc);
	MI_ALLOCLISTHEADER(fillarea, 
			   MI_ROUND_LISTHEADERCOUNT(index_list_list->numLists));
	if (!(pddolist = fillarea->ddList)) {
	  status = BadAlloc;
	  goto exit;
	}

	/* get next free facet list header */
	if (in_fct) {
	  fillareafacet = MI_NEXTTEMPFACETLIST(pddc);
	  MI_ALLOCLISTOFDDFACET(fillareafacet, 1, facet_size);
	  ddFill->pFacets = fillareafacet;
	  fillareafacet->type = input_facet->type;
	  fillareafacet->numFacets = 1;
	  out_fct = fillareafacet->facets.pNoFacet;
        } 

	/* Now, transform each list */
	for (j = 0; j < index_list_list->numLists; j++) {
	   /*
	    * Insure sufficient room for each vertex 
	    */
	    MI_ALLOCLISTOFDDPOINT(pddolist,index_list->numLists,out_point_size);
	    if (!(pddolist->pts.ptr)) {
		status = BadAlloc;
		goto exit;
	    }

	    out_pt = pddolist->pts;

	    index = index_list->pConnects;

	    for (k = 0; k < index_list->numLists; k++) {
		bcopy((in_pt+(*index * point_size)), out_pt.ptr, point_size);
		out_pt.ptr += point_size;
		if (ddSOFAS->edgeAttribs) *(out_pt.pEdge++) = *(edge_ptr++);
		index++;
	    }

	    pddolist->numPoints = index_list->numLists;

	    /* Prepare for next bound */
	    pddolist++;
	    /* get indices for next bound */
	    index_list++;
	}

	/* Copy facet data for FAS */
	if (in_fct) {
	  bcopy((char *)in_fct, (char *)out_fct, facet_size);
	  out_fct += facet_size;
	  in_fct += facet_size;
	}

	/* Render fill area */
	ddFill->points.numLists = index_list_list->numLists;
	ddFill->points.ddList = fillarea->ddList;
	if (status=InitExecuteOCTable[(int)(pGStr->elementType)](pRend, pGStr))
	  goto exit;

	index_list_list++;
      }

exit:
      Xfree(pGStr);

      return(status);

}
