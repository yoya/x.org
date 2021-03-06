/*
 *		Copyright IBM Corporation 1989
 *
 *                      All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of IBM not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 *
 * IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
 *
 * University of Illinois at Urbana-Champaign
 * Department of Computer Science
 * 1304 W. Springfield Ave.
 * Urbana, IL	61801
 *
 * (C) Copyright 1987, 1988 by The University of Illinois Board of Trustees.
 * All rights reserved.
 *
 * Tool: X 11 Graphical Kernel System
 * Author: Gregory Scott Rogers
 * Author: Sung Hsien Ching Kelvin
 * Author: Yu Pan
 * 
 * fillarea.c - functions and data for gks fill areas.
 *		gfillarea ()
 *		gsetfillcolourind ()
 *		gsetfillind ()
 *		gsetfillintstyle ()
 *		gsetfillrep ()
 *		gsetfillstyleind ()
 *		gsetpatrefpt()
 *		gsetpatrep()
 *		gsetpatsize()
 *		
 * utility routines:
 *		XgksInitGksFillArea()
 *		XgksInitWssFillArea()
 *
 * $Header: fillarea.c,v 4.0 89/08/31 16:20:43 amy Exp $
 *
 * $Source: /andrew/Xgks/source/xgks.bld/src/RCS/fillarea.c,v $
 *
 * $Log:	fillarea.c,v $
 * Revision 4.0  89/08/31  16:20:43  amy
 * Changed IBM copyright for MIT distribution.
 * 
 * Revision 3.23  89/06/05  10:16:15  bruce
 * DCR# d1:	Changed include file name from gks_implement.h
 * 		to gks_implem.h for AIX compiler.
 * 
 * Revision 3.22  89/05/22  09:26:17  bruce
 * PTR# c1174:	Made changes to implement negative hatch styles.
 * 		Had to change macro call to WS_FILL_TYPE.
 * 		Also removed default pattern variable and made it global
 * 		to cover compiler glitch.  - PTR# c1178.
 * 
 * Revision 3.21  89/02/04  15:23:39  amy
 * PTR c1147	Make global vars. and functions private, and static where possible.
 * 
 * Revision 3.20  88/12/28  14:24:13  amy
 * PTR c1143	XgksInitGksFillArea:  Added new predefined fill area representation
 * 		2 - 5, as per the ANSI standard, and changed the initialization
 * 		functions to initialize new workstations with the five predefined
 * 		representations as the first five reps.
 * 
 * Revision 3.19  88/12/28  14:22:12  amy
 * PTR c1075	XgksInitWssFillArea:  Created and initialized default attribute
 * 		bundle, which is now used to initialize the GKS state list when
 * 		GKS is opened, and the workstation state list when a new
 * 		workstation is opened.
 * 
 * Revision 3.18  88/12/22  13:53:53  amy
 * PTR c1144	Define pattern attributes for GKS state list.
 * 		Implement gsetpatrefpt and gsetpatsize.
 * 
 * Revision 3.17  88/12/13  10:55:49  amy
 * Added stubs for gsetpatsize and gsetpatrefpt.
 * 
 * Revision 3.16  88/12/07  16:46:48  amy
 * Change spelling of color to colour.
 * 
 * Revision 3.15  88/12/06  17:22:23  amy
 * PTR c1130	GKSERROR calls:  changed function name parameter from character
 * 		string to enum. type value.
 * 
 * Revision 3.14  88/09/21  08:41:22  amy
 * MIT	XgksInitWssFillArea:  Added a return(0) statement.
 * 
 * Revision 3.13  88/08/17  07:01:35  amy
 * AUG  XgksInitWssFillArea:  removed ifdef section.
 *      gsetpatrep:  added different way of checking valid colour indices
 *      (presently commented out).
 * 
 * Revision 3.12  88/08/15  08:41:59  amy
 * PTRs c1100, c1099 & c1007: changed gsetpatrep and gsetfillrep to check
 * correctly for valid pattern and hatch indices.
 * 
 * Revision 1.6  88/08/11  16:38:01  mike
 * PTR #1007:  No changes made, since Todd's fix for PTR #1100 also resolved
 * this one.
 * Mike
 * 
 * Revision 1.5  88/08/09  10:03:48  todd
 * Fixed Ptr c1099 and c1100.
 * Added error check for interior style of pattern or hatch 
 * and a style index of zero (error 84).
 * Corrected return code for invalid interior style (error 83).
 * Corrected return code for invalid pattern index (error 85).
 * 
 * Changes made to gsetfillrep.
 * 
 * Revision 1.4  88/07/28  17:40:41  bruce
 * Added a for loop with a GKSERROR macro to check each colour index in
 * the array passed to gsetpatrep()
 * PTR# c1102
 * 
 * Revision 1.3  88/07/28  11:20:51  owens
 * added check for error 20 for PTR c1012
 * 
 * Revision 1.2  88/07/26  17:51:56  owens
 * renamed/replaced VALID_WSID macro for PTR c1012 (DWO)
 * 
 * Revision 1.1  88/07/21  10:48:45  david
 * Initial revision
 *  
 *
 */
 
static char *rcsid = "$Header: fillarea.c,v 4.0 89/08/31 16:20:43 amy Exp $";

#include "gks_implem.h" /* d1 */

/* 
 * XgksInitGksFillArea: initialise gks state list fill area stuff
 */

/* c1143:  Created predefined reps 1-5 */

/* c1147:  Put them into fillarea.h */
#include "fillarea.h"

XgksInitGksFillArea()
{
	/* c1147: changed this function to use gks state list structure */
	xgks_state.gks_flattr.fill   = 1;		/* fill index */ /* c1143 */
	xgks_state.gks_flattr.style  = GINDIVIDUAL;	/* style ASF  */ /* c1143 */
	xgks_state.gks_flattr.inter  = GINDIVIDUAL;	/* inter ASF  */ /* c1143 */
	xgks_state.gks_flattr.colour = GINDIVIDUAL;	/* colour ASF */ /* c1143 */
	/* fill bundle */
	xgks_state.gks_flattr.bundl.inter  = def_flbundl[0].inter;	/* c1143 */
	xgks_state.gks_flattr.bundl.style  = def_flbundl[0].style;	/* c1143 */
	xgks_state.gks_flattr.bundl.colour = def_flbundl[0].colour;	/* c1143 */

	xgks_state.gks_ptattr.widthvec  = def_ptattr.widthvec;		/* c1144 */
        xgks_state.gks_ptattr.heightvec = def_ptattr.heightvec;		/* c1144 */
        xgks_state.gks_ptattr.ptp 	= def_ptattr.ptp;     		/* c1144 */
}
/*
 * XgksInitWssFillArea(ws) - send predefined attributes and Bundle index to
 *	newly opened workstation ws;
 */


/* The data for predefined patterns */

static								/* c1147 */
Gint pttn0[] = {1, 0, 0, 0,
		1, 1, 0, 0,
		1, 1, 1, 0,
		1, 1, 1, 1};
static								/* c1147 */
Gint pttn1[] = {1, 1, 1, 1,
		1, 0, 0, 1,
		1, 0, 0, 1,
		1, 1, 1, 1};
static								/* c1147 */
Gint pttn2[] = {0, 0, 0, 0,
		0, 1, 1, 0,
		0, 1, 1, 0,
		0, 0, 0, 0};
static								/* c1147 */
Gint pttn3[] = {0, 0, 1, 1,
		0, 0, 1, 1,
		1, 1, 0, 0,
		1, 1, 0, 0};
static								/* c1147 */
Gint pttn4[] = {1, 0, 0, 0,
		1, 0, 0, 0,
		1, 0, 0, 0,
		1, 0, 0, 0};
static								/* c1147 */
Gint pttn5[] = {0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		1, 1, 1, 1};
static								/* c1147 */
Gint pttn6[] = {1, 1, 1, 0,
		1, 1, 1, 0,
		1, 1, 1, 0,
		1, 1, 1, 0};
static								/* c1147 */
Gint pttn7[] = {1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		0, 0, 0, 0};
static								/* c1147 */
Gint pttn8[] = {1, 1, 1, 0,
		1, 1, 0, 1,
		1, 0, 1, 1,
		0, 1, 1, 1};
static								/* c1147 */
Gint pttn9[] = {0, 0, 0, 1,
		0, 0, 1, 0,
		0, 1, 0, 0,
		1, 0, 0, 0};
static								/* c1147 */
Gint pttn10[] = {0, 1, 1, 1,
		1, 0, 1, 1,
		1, 1, 0, 1,
		1, 1, 1, 0};
static								/* c1147 */
Gint pttn11[] = {1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1};
static								/* c1147 */
Gint pttn12[] = {1, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 
	        1, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1};
static								/* c1147 */ 
Gint pttn13[] = {1, 0, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 0, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 1, 0, 0,
		0, 0, 0, 1, 1, 0, 0, 0, 
	        0, 0, 0, 1, 1, 0, 0, 0,
		0, 0, 1, 0, 0, 1, 0, 0,
		0, 1, 0, 0, 0, 0, 1, 0,
		1, 0, 0, 0, 0, 0, 0, 1};
static								/* c1147 */ 
Gint pttn14[] = {0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 
	        1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1};
static								/* c1147 */ 
Gint pttn15[] = {1, 0, 0, 1, 0, 0, 0, 0,
		1, 0, 0, 1, 0, 0, 0, 0,
		1, 0, 0, 1, 0, 0, 0, 0,
		1, 0, 0, 1, 0, 0, 0, 0, 
	        1, 0, 0, 1, 0, 0, 0, 0,
		1, 0, 0, 1, 0, 0, 0, 0,
		1, 0, 0, 1, 0, 0, 0, 0,
		1, 0, 0, 1, 0, 0, 0, 0};
static								/* c1147 */ 
Gint pttn16[] = {
	1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1,
	1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1,
	1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1,
	0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1,
	1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0,
	1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
	0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1,
	1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1,
	1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1,
	1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1,
	0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1,
	1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0,
	1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
	0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1
};
static								/* c1147 */
Gint pttn17[] = {
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1,
	1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1,
	1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0,
	0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
	1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1,
	1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1,
	1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0,
	0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
	1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0
};
static								/* c1147 */
Gint pttn18[] = {
	1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0,
	0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
	1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,
	1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
	1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0,
	0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
	1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,
	1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0
};
static								/* c1147 */
Gint pttn19[] = {
	1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1
};
Gptbundl xgks_xpttn[] = {		/*c1147*/
	{{4, 4}, pttn0},
	{{4, 4}, pttn1},
	{{4, 4}, pttn2},
	{{4, 4}, pttn3},
	{{4, 4}, pttn4},
	{{4, 4}, pttn5},
	{{4, 4}, pttn6},
	{{4, 4}, pttn7},
	{{4, 4}, pttn8},
	{{4, 4}, pttn9},
	{{4, 4}, pttn10},
	{{4, 4}, pttn11},
	{{8, 8}, pttn12},
	{{8, 8}, pttn13},
	{{8, 8}, pttn14},
	{{8, 8}, pttn15},
	{{16, 16}, pttn16},
	{{16, 16}, pttn17},
	{{16, 16}, pttn18},
	{{16, 16}, pttn19},
	{{4, 4}, pttn0}
};


XgksInitWssFillArea(ws)
	WS_STATE_PTR ws;
{
	Gint i, j, k;

	/* c1143: flbundl_table[0] never used but initialized anyway */
	ws->flbundl_table[0] = def_flbundl[0];				/* c1143 */

	/* c1143: initialize predefined representation bundles */
	for (i=1; i<=PDF_FILL_BNDLS; i++)				/* c1143 */
	   /* c1075: use implementation defaults, not current attributes */
	   ws->flbundl_table[i] = def_flbundl[i-1];			/* c1143 */

	/* c1143: initialize rest of representation bundle array */
	for (i=PDF_FILL_BNDLS+1; i<MAX_BUNDL_TBL; i++)			/* c1143 */
	   ws->flbundl_table[i] = def_flbundl[0];			/* c1143 */

	for (i=0; i<MAX_BUNDL_TBL; i++) {                  /* set default pattern table values */
		j = xgks_xpttn[i].size.x * xgks_xpttn[i].size.y;	/*c1147*/
		ws->ptbundl_table[i] = xgks_xpttn[i];			/*c1147*/
		ws->ptbundl_table[i].array = (Gint *)malloc( j * sizeof(int));
		GKSERROR( (ws->ptbundl_table[i].array == NULL), 300, errXgksInitWssFillArea);

		for (k=0; k < j; k++)
	          ws->ptbundl_table[i].array[k] = xgks_xpttn[i].array[k]; /*c1147*/
	}
	return(0);			/*MIT*/

}

/*$F
 * gfillarea(num_pts, pts) - FILL AREA
 *
 * Gint num_pts;		Number of points that will define a polygon, >2
 * Gpoint *pts;			Array of those points.
 *
 * returns 0=OK or one of 5, 100.
 *
 * See Also: ANSI Standard p.83
 */
gfillarea(num_pts, pts)
	Gint num_pts;
	Gpoint *pts;
{
	Gpoint *ndc_pt;
	OUT_PRIMI *flarea;
	Gint cnt;

/* check for proper operating state */
	GKSERROR ((xgks_state.gks_state != GWSAC && xgks_state.gks_state != GSGOP), 5, errgfillarea) /*c1147*/
	
/* check for a valid number of points */
	GKSERROR ((num_pts < 3) , 100, errgfillarea)

/* open an primitive structure */
	GKSERROR (( (flarea = XgksNewPrimi()) == NULL ), 300, errgfillarea);

	flarea->pid = FILL_AREA;
	flarea->primi.fill_area.num_pts = num_pts;

/* now get memory for transformed points */
	GKSERROR ((((flarea->primi.fill_area.pts) = 
		  (Gpoint *) malloc((unsigned)num_pts * sizeof(Gpoint))) == NULL) , 300, errgfillarea)

	ndc_pt = flarea->primi.fill_area.pts;
/* transform the WC to NDC */
	for (cnt=0; cnt<num_pts; cnt++, pts++, ndc_pt++) 
		WcToNdc(pts, ndc_pt);	/* convert to ndc coordinate */
	
	flarea->primi.fill_area.flattr = xgks_state.gks_flattr;		/*c1147*/
	flarea->primi.fill_area.ptattr = xgks_state.gks_ptattr;		/*c1147*/   

	XgksProcessPrimi(flarea);

	if (MO_OPENED == TRUE) XgksMoGraphicOutput (14, num_pts, flarea->primi.fill_area.pts);

	free (flarea->primi.fill_area.pts);
	free (flarea);

	return(0);
}

/*$F
 * gsetfillcolourind (colour) - SET FILL AREA COLOUR INDEX
 *
 * Gint colour;		new colour for INDIVIDUALly specified fill areas.
 *
 * returns 0=OK, or one of 8, 92
 *
 * See Also: ANSI Standard p.98
 */
gsetfillcolourind (colour)
	Gint colour;
{
/* check for proper operating state */
	GKSERROR ((xgks_state.gks_state == GGKCL) , 8, errgsetfillcolourind) /*c1147*/

/* check for valid colour index */
	GKSERROR ((colour < 0) , 92, errgsetfillcolourind)

/* change the colour */
	xgks_state.gks_flattr.bundl.colour = colour;		/*c1147*/

	if (MO_OPENED == TRUE) XgksMoSetGraphicAttr (40, colour);

	return(0);
}

/*$F
 * gsetfillind (idx) - SET FILL AREA INDEX
 *
 * Gint idx;		new fill area bundle entry for INDIVIDUAL aspects.
 *
 * returns 0=OK, or one of 8, 80
 *
 * See Also: ANSI Standard p.96
 */
gsetfillind (idx)
	Gint idx;
{
/* check for proper operating state */
	GKSERROR ((xgks_state.gks_state == GGKCL) , 8, errgsetfillind) /*c1147*/

/* check for valid index */
	GKSERROR ((idx < 1 ), 80, errgsetfillind)

/* ok to change the idx */
	xgks_state.gks_flattr.fill = idx;			/*c1147*/

	if (MO_OPENED == TRUE) XgksMoSetGraphicAttr (37, idx);

	return(0);
}

/*$F
 * gsetfillintstyle(style) - SET FILL AREA INTERIOR STYLE
 *
 * Gflinter style;		new style for INDIVIDUALly specified fill areas
 *				GHOLLOW | GSOLID | GPATTERN | GHATCH
 *
 * returns 0=OK, or 8
 *
 * See Also: ANSI Standard p.96
 */
gsetfillintstyle(style)
	Gflinter style;
{
/* check for proper operating state */
	GKSERROR ((xgks_state.gks_state == GGKCL) , 8, errgsetfillintstyle) /*c1147*/

/* check for valid style */
	if (style != GHOLLOW && style != GSOLID && style != GPATTERN && style != GHATCH )
		style = GHOLLOW;

	xgks_state.gks_flattr.bundl.inter = style;		/*c1147*/

	if (MO_OPENED == TRUE) XgksMoSetFillIntStyle (style);

	return(0);
}

/*$F
 * gsetfillrep (ws_id, idx, rep) - SET FILL AREA REPRESENTATION
 *
 * Gint ws_id;			workstation identifier.
 * Gint	idx,			fill area bundle entry to set.
 * Gflbundl *rep		fill area bundle info
 *
 * returns 0=OK, or one of 7, 20, 25, 33, 35, 36, 80, 83, 85, 86, 93, 2000
 *
 * See Also: ANSI Standard p.103
 */
gsetfillrep (ws_id, idx, rep)
	Gint ws_id;
	Gint idx;
	Gflbundl *rep;
{
	WS_STATE_PTR ws;

/* check for proper gks state */
	GKSERROR ((xgks_state.gks_state == GGKCL || xgks_state.gks_state == GGKOP) , 7, errgsetfillrep) /*c1147*/

/* check for invalid workstation id */
/* DWO 7/28/88  added check to differentiate between */
/*              error 20 and error 25 for PTR c1012  */
        GKSERROR ( (!VALID_WSID(ws_id)), 20, errgsetfillrep)   /* c1012 */

/* check for open workstation identifier */
/* DWO 7/26/88  changed macro name from VALID_WSID */
	GKSERROR (((ws=OPEN_WSID(ws_id)) == NULL) , 25, errgsetfillrep)  /* c1012 */

/* Check for valid workstation category */
	GKSERROR ((WS_CAT(ws) == GMI), 33, errgsetfillrep)
	GKSERROR ((WS_CAT(ws) == GINPUT), 35, errgsetfillrep)
	GKSERROR ((WS_CAT(ws) == GWISS), 36, errgsetfillrep)

/* check for valid fill area bundle table index */
	GKSERROR ((idx < 1 || idx >= MAX_BUNDL_TBL) , 80, errgsetfillrep)

/* check for valid interior style */                        /* c1100 */
	GKSERROR((rep->inter != GHOLLOW && rep->inter != GSOLID && rep->inter != GPATTERN && rep->inter != GHATCH ) , 83, errgsetfillrep)  /* c1100 */

/* check for style index equal to zero */                   /* c1100 */
	if ((rep->inter == GPATTERN) || (rep->inter == GHATCH))     /* c1100 */
	        GKSERROR ((rep->style == 0), 84, errgsetfillrep);    /* c1100 */

/* valid pattern index */
	if (rep->inter == GPATTERN)
		GKSERROR ((!WS_FILL_TYPE(rep->inter,rep->style)), 85, errgsetfillrep); /* c1100 c1174 */

/* valid hatch index */
	if (rep->inter == GHATCH)
		GKSERROR ((!WS_FILL_TYPE(rep->inter,rep->style)), 86, errgsetfillrep); /* c1174 */

/* check for valid colour index */
	GKSERROR ((!WS_AVAIL_COLOUR(ws,rep->colour)) , 93, errgsetfillrep)

	if (ws->ewstype == MO)
		XgksMoSetFillRep (ws, idx, rep);

/* finally get to tell the workstation */
	ws->flbundl_table[idx] = (*rep);

	return(0);
}

/*$F
 * gsetfillstyleind (idx) - SET FILL AREA STYLE INDEX
 *
 * Gint idx;	new fill area style index for INDIVIDUALly specified fills
 *
 * returns 0=OK, or one of 8, 84
 *
 * See Also: ANSI Standard p.97
 */
gsetfillstyleind (idx)
	Gint idx;
{
/* check for proper operating state */
	GKSERROR ((xgks_state.gks_state == GGKCL) , 8, errgsetfillstyleind) /*c1147*/

/* check for valid style index */
	GKSERROR ((idx == 0) , 84, errgsetfillstyleind)

/* ok to proceed */
	xgks_state.gks_flattr.bundl.style = idx;		/*c1147*/

	if (MO_OPENED == TRUE) XgksMoSetGraphicAttr (39, idx);

	return(0);
}

/*$F
 * gsetpatrefpt (ref) - SET PATTERN REFERENCE POINT	Added for ptr c1144
 *
 * Gpoint *ref; new pattern reference point
 *
 * returns 0=OK, or 8
 *
 * See Also: ANSI Standard p.98
 */

gsetpatrefpt (ref)				/*c1144*/
Gpoint *ref;
{
/* check for proper operating state */
        GKSERROR ((xgks_state.gks_state == GGKCL) , 8, errgsetpatrefpt ) /*c1147*/

/* ok to proceed */
        xgks_state.gks_ptattr.ptp = *ref;		/*c1147*/

        if (MO_OPENED == TRUE) XgksMoSetPatRef ();

        return(0);
}



/*$F
 * gsetpatsize (siz) - SET PATTERN SIZE		Added for ptr c1144
 *
 * Gpoint   siz;  new pattern size to be set in GKS state list.
 *
 * returns 0=OK, or one of 8, 87
 *
 * See Also: ANSI Standard p.98
 */
gsetpatsize (siz)						/*c1144*/
        Gpoint  *siz;
{
/* check for proper operating state */
        GKSERROR ((xgks_state.gks_state == GGKCL) , 8, errgsetpatsize ) /*c1147*/

/* check for valid style index */
        GKSERROR ( ((siz->x < 0) || (siz->y < 0)) , 87, errgsetpatsize )

/* ok to proceed */
        xgks_state.gks_ptattr.widthvec.x = siz->x;	/*c1147*/
        xgks_state.gks_ptattr.widthvec.y = 0.0;		/*c1147*/
        xgks_state.gks_ptattr.heightvec.x = 0.0;	/*c1147*/
        xgks_state.gks_ptattr.heightvec.y = siz->y;	/*c1147*/

        if (MO_OPENED == TRUE) XgksMoSetPatSize ();

        return(0);
}


/*$F
 * gsetpatrep(ws_id, idx, rep) - SET PATTERN REPRESENTATION
 *
 * Gint ws_id;			workstation identifier.
 * Gint	idx;			pattern index.
 * Gptbundl *rep;               pattern representation pointer
 *
 * returns 0, 7, 20, 25, 33, 35, 36, 79, 83, 84, 86
 *
 * See Also: ANSI Standard p.104
 */

gsetpatrep(ws_id, idx, rep)
Gint ws_id;
Gint idx;
Gptbundl *rep;
{
	WS_STATE_PTR ws;
	Gint i, j;
        Gint *slot;     /* c1102 */

/* check for proper gks state */
	GKSERROR ((xgks_state.gks_state == GGKCL || xgks_state.gks_state == GGKOP) , 7, errgsetpatrep) /*c1147*/

/* check for invalid workstation id */
/* DWO 7/28/88  added check to differentiate between */
/*              error 20 and error 25 for PTR c1012  */
        GKSERROR ( (!VALID_WSID(ws_id)), 20, errgsetpatrep)   /* c1012 */

/* check for open workstation */
/* DWO 7/26/88  changed macro name from VALID_WSID */
	GKSERROR ((!(ws=OPEN_WSID(ws_id))) , 25, errgsetpatrep)  /* c1012 */

/* check proper workstation category */
	GKSERROR ((WS_CAT(ws) == GMI), 33, errgsetpatrep)
	GKSERROR ((WS_CAT(ws) == GWISS), 36, errgsetpatrep)

/* valid pattern index? */
	GKSERROR ((idx < 1 || idx >= MAX_BUNDL_TBL) , 85, errgsetpatrep)

/* valid dimensions */
	GKSERROR ((rep->size.x < 1 || rep->size.y < 1) , 91, errgsetpatrep) /*c1101*/

/* valid colour indices */                                /* c1102 */
	i = rep->size.x * rep->size.y;                   /* c1102 */
        for (j=0,slot=rep->array; j < i; j++,slot++)     /* c1102 */
           GKSERROR ((*slot < 0), 93, errgsetpatrep)      /* c1102 */

/* valid colour values? */
/*	i = ws->ptbundl_table[idx-1].size.x * ws->ptbundl_table[idx-1].size.y;
 *	for (j=0; j < i; j++)
 *		GKSERROR( !WS_AVAIL_COLOUR(ws,rep->array[j]), 93, errgsetpatrep);
 * AUG
 */

/* STEP 2: set the pattern bundle table values */
	free(ws->ptbundl_table[idx-1].array);
	ws->ptbundl_table[idx-1] = *rep;
	ws->ptbundl_table[idx-1].array = (Gint *)malloc( i * sizeof(int));
	GKSERROR( (ws->ptbundl_table[idx-1].array == NULL), 300, errgsetpatrep);

	for (j=0; j < i; j++)
		ws->ptbundl_table[idx-1].array[j] = rep->array[j];

	if (ws->ewstype == MO)
		XgksMoSetPatRep (ws, idx, rep);
	
	return (OK);
}
