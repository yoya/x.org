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
 * Urbana, IL    61801
 *
 * (C) Copyright 1987, 1988 by The University of Illinois Board of Trustees.
 * All rights reserved.
 *
 * Tool: X 11 Graphical Kernel System
 * Author: Gregory Scott Rogers
 * Author: Sung Hsien Ching Kelvin
 * Author: Yu Pan
 *
 * stroke.c - functions and data for GKS Stroke
 * 
 * $Header: stroke.c,v 4.0 89/08/31 16:24:58 amy Exp $
 *
 * $Source: /andrew/Xgks/source/xgks.bld/src/RCS/stroke.c,v $
 *
 * $Log:	stroke.c,v $
 * Revision 4.0  89/08/31  16:24:58  amy
 * Changed IBM copyright for MIT distribution.
 * 
 * Revision 3.33  89/06/05  10:07:47  bruce
 * DCR# d1:	Changed include file name from gks_implement.h
 * 		to gks_implem.h for AIX compiler.
 * PTR# c1179:	Changed exp field to tx_exp or ch_exp for AIX
 * 		compiler.  Moved variable declarations.  
 * 
 * Revision 3.32  89/05/22  09:25:32  bruce
 * PTR# c1177:  Made changes to code in the Update Prompt calls and in other places to 
 * 		insure that NULL is not dereferenced.
 * 
 * Revision 3.31  89/04/20  14:49:48  bruce
 * PTR# c2065:	Change all values for the default stroke buffer size from 1 to 64.
 * PTR# c2066:	Change all values for the default stroke interval from
 * 		0 to 0.001.
 * 
 * Revision 3.30  89/02/24  12:19:33  amy
 * PTR c1151	XgksStkUpdatePrompt:  Stroke echoing made faster
 * 		by drawing only new line/marker input, instead of
 * 		redrawing entire stroke.  Subroutine XgksXDrawLines
 * 		added to make multiple calls to XDrawLine.
 * 
 * Revision 3.29  89/02/04  15:29:45  amy
 * PTR c1147	Make global vars. and functions private, and static where possible.
 * 
 * Revision 3.28  89/01/17  17:22:40  amy
 * Moved data definitions added for PTR c1014 from within code to data section for ginitstroke.
 * 
 * Revision 3.27  88/12/19  17:11:38  amy
 * PTR c1148	XgksCreateDefStroke:  replace malloc with a call to XgksIDevNew.
 * 
 * Revision 3.26  88/12/16  12:43:32  amy
 * PTR c1133	XgksStkUpdatePrompt:  add event id to parameter list.
 * 		XgksEnqueueEvent calls:  add event id to parameter list.
 * 
 * Revision 3.25  88/12/13  10:22:24  amy
 * PTR c1049	ginitstroke:  After call to XChangeGC, an IF statement checks the
 * 		linestyle in the gc.  If this statement immediately follows the
 * 		call to XChangeGC, the program core dumps with a bus error.
 * 		The problem is avoided if the IF statement is moved to immediately
 * 		before the return.
 * 
 * Revision 3.24  88/12/09  11:41:04  amy
 * Changed WS_AVAIL_COLOR to WS_AVAIL_COLOUR.
 * 
 * Revision 3.23  88/12/08  13:16:20  amy
 * PTR c1130	GKSERROR and gerrorhand calls:  changed function name parameter
 * 		from character string to enum. type value.
 * Changed spelling of color to colour.
 * 
 * Revision 3.22  88/12/01  12:19:26  amy
 * New 4.3 C compiler--
 * 	ginitstroke:  calls to WS_AVAIL_COLOUR with an unsigned long parameter
 * 	are cast to type int.
 * 	Variable didx initialized to 0.
 * 
 * Revision 3.21  88/11/04  10:48:25  amy
 * PTR c1057	gsetstrokemode:  set up SIGALRM signal handler if mode = GEVENT.
 * 
 * Revision 3.20  88/11/02  08:45:25  amy
 * PTR c1120	XgksStkDelete:  do a switch on the pet to free the data
 * 		field of the Gstrokerec structure.
 * 
 * Revision 3.19  88/10/12  13:52:19  amy
 * PTR c1014  greqstroke was not gracefully handling bogus data in the
 *     data buffer, e.g., initial points = 0 and editpos > 1.
 *     Accept editpos values >= 1 and <= bufsize.
 * 
 * Revision 3.18  88/10/11  12:14:08  amy
 * No change.
 * 
 * Revision 3.17  88/09/26  09:08:35  amy
 * MIT	Declare local and global functions where used.
 * 	XgksStkUpdatePrompt:  call with correct number and types of parameters.
 * 	XgksEnqueueEvent:  recast data parameter in calls as a (char *).
 * 	Rename CreateDefStroke XgksCreateDefStroke to follow convention.
 * 
 * Revision 3.16  88/08/18  09:25:40  amy
 * AUG  gsamplestroke:  changed check for SAMPLE mode, 
 *        and removed init. of new device.
 * 
 * Revision 3.15  88/08/11  09:02:14  amy
 * PTR c1012  ginitstroke, ginqstrokest, greqstroke, gsamplestroke &
 * gsetstrokemode:  added call to VALID_WSID to check for error 20.
 * 
 * Revision 1.5  88/07/28  15:05:26  owens
 * added check for error 20 for PTR c1012
 * 
 * Revision 1.4  88/07/27  13:59:05  bruce
 * GKSERROR macros had to be either added or changed to handle
 * error code 63 and 64
 * 
 * 
 * Revision 1.3  88/07/27  13:46:27  bruce
 * 
 * 
 * Revision 1.2  88/07/26  17:54:09  owens
 * renamed/replaced VALID_WSID macro for PTR c1012 (DWO)
 * 
 * Revision 1.1  88/07/21  10:50:23  david
 * Initial revision
 *  
 *
 */

static char rcsid[]="$Header: stroke.c,v 4.0 89/08/31 16:24:58 amy Exp $";
 
#include "gks_implem.h"                 /* d1 */
#include <signal.h>		/*c1057*/
double fabs();

extern  XgksAwaitInterrupt();	/*  Declare  Interrupt function   PTR# c1057  */
 

/*
 * INITIALISE STROKE
 *
 * Returns: 0, 7, 20, 25, 38, 50, 51, 60, 63, 65, 66, 67, 69, 71, 92, 140, 141, 144, 145, 146, 152, 153
 *
 * See Also: ANSI Standard p.121
 */
 
ginitstroke( ws_id, dev, init, pet, area, record )
    Gint ws_id;
    Gint dev;
    Gstroke *init;
    Gint pet;
    Glimit *area;
    Gstrokerec *record;
{
	/*
	 * declare local functions
	 */
	static Bool XgksCreateDefStroke();	/*MIT*/

    WS_STATE_ENTRY *ws;
    INPUT_DEV *idev;
    XGCValues gcvalues;
    Gpoint *spt, *dpt;
    int i, didx;
    static Gpoint *ptr;				/* c1179 c1014 */
    static int cnt = 1;				/* c1179 c1014 */
    static Glimit *win;				/* c1179 c1014 */
    
    
 
/* STEP 1: check for errors. */
/* proper gks state? */
    GKSERROR( (xgks_state.gks_state == GGKCL || xgks_state.gks_state == GGKOP), 7, errginitstroke) /* c1147 */
 
/* check for invalid workstation id */
/* DWO 7/28/88  added check to differentiate between */
/*              error 20 and error 25 for PTR c1012  */
        GKSERROR ( (!VALID_WSID(ws_id)), 20, errginitstroke)   /* c1012 */

/* open wsid? */
/* DWO 7/26/88  changed macro name from VALID_WSID */
    GKSERROR( !(ws=OPEN_WSID(ws_id)), 25, errginitstroke)  /* c1012 */
 
/* valid workstation category */
    GKSERROR( (WS_CAT(ws) != GOUTIN && WS_CAT(ws) != GINPUT), 38, errginitstroke)
 
/* valid echo area */
        GKSERROR( (area->xmin > area->xmax || area->ymin > area->ymax), 51, errginitstroke )
 
/* valid stroke device number 	c1014 */
        GKSERROR( (dev < 1), 140, errginitstroke )
 
/* valid and supported prompt mode? */
        GKSERROR( pet != 1 && pet != 3 && pet != 4, 144, errginitstroke)
 
/* Echo inside display space? */
        GKSERROR( (area->xmin < 0 || area->xmax > ws->size.x
                || area->ymin < 0 || area->ymax > ws->size.y),
                145, errginitstroke )
 
/* valid data record */
    GKSERROR( record->pet1.bufsiz < 1, 146, errginitstroke)
    GKSERROR( record->pet1.editpos < 1, 146, errginitstroke)	/*c1014*/
  GKSERROR( record->pet1.editpos > record->pet1.bufsiz, 146, errginitstroke) /*c1014*/
    GKSERROR( record->pet1.time < 0.0, 146, errginitstroke)
    GKSERROR( init->n_points < 0, 152, errginitstroke)
    GKSERROR( init->n_points > record->pet1.bufsiz, 153, errginitstroke)
    GKSERROR( init->transform < 0, 50, errginitstroke)
    GKSERROR( init->transform > MAX_TRANS, 50, errginitstroke)

/* check that editpos is within the initial points (so that bogus data       */
/* won't be mistaken for valid points) for ptr c1014 (DWO).                  */
/* The standard doesn't really mention this restriction, but it seems needed.*/

    GKSERROR(record->pet1.editpos>init->n_points+1,152,errginitstroke) /*c1014*/

/* make sure initial points are within the transform in initstroke           */
    win = &xgks_state.ntrans_list[init->transform].ntrans.w; /* c1014 */ /* c1147 */
    for ( ptr = init->points, cnt = 1;                              /* c1014 */
          cnt <= init->n_points;                                    /* c1014 */
          ptr++,cnt++)                                              /* c1014 */
      GKSERROR( ((ptr->x < win->xmin) || (win->xmax < ptr->x)       /* c1014 */
              || (ptr->y < win->ymin) || (win->ymax < ptr->y)),     /* c1014 */
              146, errginitstroke)                                   /* c1014 */
 
    didx = 0;				/* New 4.3 C compiler:  initialize */
    switch( pet ) {
    default:
    case 1:
        break;
    case 3:    /* polymarker */
        if (record->pet3.acf == GSPECIFIED) {
            GKSERROR( record->pet3.mk.mark < 1 || record->pet3.mk.mark >= MAX_BUNDL_TBL, 66, errginitstroke)
            GKSERROR( !WS_MARKER_TYPE(record->pet3.mk.bundl.type), 69, errginitstroke)
            GKSERROR( record->pet3.mk.bundl.size < 0.0, 71, errginitstroke)
            GKSERROR( !WS_AVAIL_COLOUR(ws, record->pet3.mk.bundl.colour), 92, errginitstroke)
        }
        else {
            record->pet3.mk.type = xgks_state.gks_mkattr.type;		/* c1147 */
            record->pet3.mk.size = xgks_state.gks_mkattr.size;		/* c1147 */
            record->pet3.mk.colour = xgks_state.gks_mkattr.colour;	/* c1147 */
            record->pet3.mk.mark = xgks_state.gks_mkattr.mark;		/* c1147 */
            record->pet3.mk.bundl = xgks_state.gks_mkattr.bundl;	/* c1147 */
        }
    /* Bind attributes to device by storing in pet3.mk.bundl the attributes
     * based on the state of the ASF flags.
     */
#define MKBUND    ws->mkbundl_table[record->pet3.mk.mark]
        if (record->pet3.mk.type == GBUNDLED)
            record->pet3.mk.bundl.type = MKBUND.type;
        if (record->pet3.mk.size == GBUNDLED)
            record->pet3.mk.bundl.size = MKBUND.size;
        if (record->pet3.mk.colour == GBUNDLED)
            record->pet3.mk.bundl.colour = MKBUND.colour;
            
    /* Bind values into the GC */
        gcvalues.function = GXinvert;
        gcvalues.foreground = record->pet3.mk.bundl.colour;
        if ( !WS_AVAIL_COLOUR( ws, (int)gcvalues.foreground) ) /* New 4.3 compiler */
            gcvalues.foreground = ws->wsfg;
        gcvalues.line_width = 0;
        gcvalues.line_style = LineSolid;
#undef MKBUND
        break;
    case 4:    /* polyline */
        if (record->pet4.acf == GSPECIFIED) {
            GKSERROR( record->pet4.ln.line < 1 || record->pet4.ln.line >= MAX_BUNDL_TBL, 60, errginitstroke)
            GKSERROR( record->pet4.ln.bundl.type == 0, 63, errginitstroke)  /* c1013 */
            GKSERROR( !WS_LINE_TYPE(record->pet4.ln.bundl.type), 64, errginitstroke) /* c1013 */
            GKSERROR( record->pet4.ln.bundl.width < 0.0, 65, errginitstroke)
            GKSERROR( !WS_AVAIL_COLOUR(ws, record->pet4.ln.bundl.colour), 92, errginitstroke)
        }
        else {
            record->pet4.ln.type = xgks_state.gks_lnattr.type;		/* c1147 */
            record->pet4.ln.width = xgks_state.gks_lnattr.width;	/* c1147 */
            record->pet4.ln.colour = xgks_state.gks_lnattr.colour;	/* c1147 */
            record->pet4.ln.line = xgks_state.gks_lnattr.line;		/* c1147 */
            record->pet4.ln.bundl = xgks_state.gks_lnattr.bundl;	/* c1147 */
        }
    /* Bind attributes to device by storing in pet3.mk.bundl the attributes
     * based on the state of the ASF flags.
     */
#define LNBUND    ws->lnbundl_table[record->pet4.ln.line]
        if (record->pet4.ln.type == GBUNDLED)
            record->pet4.ln.bundl.type = LNBUND.type;
        if (record->pet4.ln.width == GBUNDLED)
            record->pet4.ln.bundl.width = LNBUND.width;
        if (record->pet4.ln.colour == GBUNDLED)
            record->pet4.ln.bundl.colour = LNBUND.colour;
            
    /* Bind values into the GC */
        gcvalues.function = GXinvert;
        gcvalues.line_style = (int) record->pet4.ln.bundl.type;
        didx = gcvalues.line_style;
        if (didx < 0 ) didx += 3;
        else if (didx > 0) didx += 1;
        /* if (didx == 0) doesn't matter */
 
        gcvalues.line_style = (gcvalues.line_style == GLN_SOLID) ?
            LineSolid : LineOnOffDash;
        gcvalues.line_width = (int) record->pet4.ln.bundl.width;
        if (gcvalues.line_style == LineSolid && gcvalues.line_width == 1)
            gcvalues.line_width = 0;
        gcvalues.foreground = record->pet4.ln.bundl.colour;
        if ( !WS_AVAIL_COLOUR( ws, (int)gcvalues.foreground) )  /* New 4.3 compiler */
            gcvalues.foreground = ws->wsfg;
#undef LNBUNDLE
        break;
    }
    if ((idev = XgksIDevLookup( ws, dev, GISTROKE)) == NULL) {
        GKSERROR( XgksCreateDefStroke( ws, dev, &idev ), 300, errginitstroke)
    }
    else {
        GKSERROR( idev->data.stk.initst.mode != GREQUEST, 141, errginitstroke)
    }
    gcvalues.background = ws->wsbg;
    XChangeGC( ws->dpy, idev->gc,
        GCFunction | GCForeground |GCBackground | GCLineWidth | GCLineStyle,
        &gcvalues);

/* copy points to initial stroke */
    if ( idev->data.stk.initst.stroke.points != NULL)
        free( idev->data.stk.initst.stroke.points );
    idev->data.stk.initst.stroke = *init;
    dpt = idev->data.stk.initst.stroke.points = 
               (Gpoint *)malloc( sizeof(Gpoint) * init->n_points );
    GKSERROR( dpt == NULL, 300, errginitstroke)
    for( i=0, spt = init->points; i<init->n_points; i++, spt++, dpt++)
        *dpt = *spt;
    
/* copy other initialization data */
    idev->data.stk.initst.pet    = pet;
    idev->data.stk.initst.e_area    = *area;
    idev->data.stk.initst.record    = *record;

     /* moved this if from after ChangeGC to avoid core dump problem.
       don't know why, but this fixes it - bg   PTR# c1049 */
    if (gcvalues.line_style == LineOnOffDash)
        XSetDashes( ws->dpy, idev->gc, 0, xgksDASHES[didx].dashl, xgksDASHES[didx].dn); /* c1147 */
 
    return(0);
}

/*
 * SET STROKE MODE
 *
 * Returns: 0, 7, 20, 25, 38, 140, 143, 2000
 *
 * See Also: ANSI Standard p.129
 */
 
gsetstrokemode(ws_id, dev, mode, echo)
    Gint ws_id;
    Gint dev;
    Gimode mode;
    Gesw echo;
{
	/*
	 * declare local functions
	 */
	static Bool XgksCreateDefStroke();	/*MIT*/

    WS_STATE_ENTRY *ws;
    INPUT_DEV *idev;
    Gpoint *spt, *dpt, ndcpt, wcpt;
    int i;
 
/* STEP 1: check for errors. */
/* proper gks state? */
        GKSERROR( (xgks_state.gks_state == GGKCL || xgks_state.gks_state == GGKOP), 7, errgsetstrokemode ) /* c1147 */
 
/* check for invalid workstation id */
/* DWO 7/28/88  added check to differentiate between */
/*              error 20 and error 25 for PTR c1012  */
        GKSERROR ( (!VALID_WSID(ws_id)), 20, errgsetstrokemode)   /* c1012 */

/* open wsid? */
/* DWO 7/26/88  changed macro name from VALID_WSID */
        GKSERROR( !(ws=OPEN_WSID(ws_id)), 25, errgsetstrokemode )  /* c1012 */
 
/* valid workstation type */
        GKSERROR( (WS_CAT(ws) != GOUTIN && WS_CAT(ws) != GINPUT), 38, errgsetstrokemode)
 
/* valid stroke device number */
        GKSERROR( (dev < 1), 140, errgsetstrokemode )
 
/* check enumerations */
        GKSERROR( ((mode != GREQUEST && mode != GSAMPLE && mode != GEVENT)
                        || (echo != GECHO && echo != GNOECHO)),
                2000, errgsetstrokemode )
 
        if ((idev = XgksIDevLookup( ws, dev, GISTROKE)) == NULL) {
        /* Create the Input Device structure */
                GKSERROR( XgksCreateDefStroke( ws, dev, &idev ), 300, errgsetstrokemode)
        }
        else {
                if ((idev->active == True) && (idev->data.stk.initst.esw == GECHO))
                        XgksStkUpdatePrompt( ws, idev, PROMPTOFF,(Gpoint *)NULL,
                             (XMotionEvent *)NULL,-1 ); /*MIT*/ /* PTR c1133 */
        }
        idev->data.stk.initst.mode = mode;
        idev->data.stk.initst.esw = echo;
 
        if (mode == GSAMPLE || mode == GEVENT) 
           {
    /* copy & transform initial points to current stroke */
        if ( idev->data.stk.stkbuf != NULL)
            free( idev->data.stk.stkbuf );
        dpt = idev->data.stk.stkbuf = (Gpoint *)malloc( sizeof(Gpoint) * idev->data.stk.initst.record.pet1.bufsiz );
        GKSERROR( dpt == NULL, 300, errgsetstrokemode)
        for( i=0, spt = idev->data.stk.initst.stroke.points;
            i<idev->data.stk.initst.stroke.n_points; i++, spt++, dpt++) 
        {
            NtWcToNdc( idev->data.stk.initst.stroke.transform, spt, &ndcpt );
            NdcToDc( ws, &ndcpt, dpt );
        }
        NtWcToNdc( idev->data.stk.initst.stroke.transform,
            &(idev->data.stk.initst.record.pet1.interval),
            &(idev->data.stk.interval) );
        wcpt.x = 0.0; wcpt.y = 0.0;
        NtWcToNdc( idev->data.stk.initst.stroke.transform, &wcpt, &ndcpt);
        idev->data.stk.interval.x = fabs( (double)(idev->data.stk.interval.x
                                                        - ndcpt.x) );
        idev->data.stk.interval.y = fabs( (double)(idev->data.stk.interval.y
                                                        - ndcpt.y) );
 
                idev->data.stk.editpos = 
                    idev->data.stk.initst.record.pet1.editpos-1;   /* c1014 */
		if ( mode == GEVENT )
	  	    signal( SIGALRM, XgksAwaitInterrupt); /* Set signal handler for 						           event mode  c1057  */
                idev->active = True;
                if ( echo == GECHO )
                        XgksStkUpdatePrompt( ws, idev, PROMPTON, &(idev->data.stk.stkbuf[idev->data.stk.editpos]),
                           (XMotionEvent *)NULL,-1 ); /*MIT*/ /* PTR c1133 c1177 */
        }
        else    /* GREQUEST */
                idev->active = False;
 
    return(0);
}

/*
 * REQUEST STROKE
 *
 * Returns: 0, 7, 20, 25, 38, 140, 141
 *
 * See Also: ANSI Standard p.132
 */
 
greqstroke(ws_id, dev, response )
    Gint ws_id;
    Gint dev;
    Gqstroke *response;
{
	/*
	 * declare local functions
	 */
	static Bool XgksCreateDefStroke();	/*MIT*/
	/*
	 * declare global functions
	 */
	Bool XgksFindNTransNpts();		/*MIT	c1014*/

    WS_STATE_ENTRY *ws;
    INPUT_DEV *idev;
    Gpoint *spt, *dpt, ndcpt, *ndcpts, wcpt;
    int i;
 
/* STEP 1: check for errors. */
/* proper gks state? */
        GKSERROR( (xgks_state.gks_state == GGKCL || xgks_state.gks_state == GGKOP), 7, errgreqstroke ) /* c1147 */
 
/* check for invalid workstation id */
/* DWO 7/28/88  added check to differentiate between */
/*              error 20 and error 25 for PTR c1012  */
        GKSERROR ( (!VALID_WSID(ws_id)), 20, errgreqstroke)   /* c1012 */

/* open wsid? */
/* DWO 7/26/88  changed macro name from VALID_WSID */
        GKSERROR( !(ws=OPEN_WSID(ws_id)), 25, errgreqstroke )  /* c1012 */
 
/* valid workstation type */
        GKSERROR( (WS_CAT(ws) != GOUTIN && WS_CAT(ws) != GINPUT), 38, errgreqstroke)
 
/* valid locator device number */
        GKSERROR( (dev < 1), 140, errgreqstroke )
 
        if ((idev = XgksIDevLookup( ws, dev, GISTROKE)) == NULL) {
        /* Create the Input Device structure */
                GKSERROR( XgksCreateDefStroke( ws, dev, &idev ), 300, errgreqstroke)
        }
        else {
        GKSERROR( (idev->data.stk.initst.mode != GREQUEST), 141, errgreqstroke)
    }
/* Make sure the workstation is up to date */
    gupdatews( ws_id, GPERFORM );
 
/* copy & transform initial points to current stroke */
    if ( idev->data.stk.stkbuf != NULL)
        free( idev->data.stk.stkbuf );
    dpt = idev->data.stk.stkbuf = (Gpoint *)malloc( sizeof(Gpoint) * idev->data.stk.initst.record.pet1.bufsiz );
    GKSERROR( dpt == NULL, 300, errgreqstroke)
    for( i=0, spt = idev->data.stk.initst.stroke.points;
        i<idev->data.stk.initst.stroke.n_points; i++, spt++, dpt++) {
        NtWcToNdc( idev->data.stk.initst.stroke.transform, spt, &ndcpt );
        NdcToDc( ws, &ndcpt, dpt );
    }
    NtWcToNdc( idev->data.stk.initst.stroke.transform,
        &(idev->data.stk.initst.record.pet1.interval),
        &(idev->data.stk.interval) );
    wcpt.x = 0.0; wcpt.y = 0.0;
    NtWcToNdc( idev->data.stk.initst.stroke.transform, &wcpt, &ndcpt);
    idev->data.stk.interval.x = fabs( (double)(idev->data.stk.interval.x - ndcpt.x) );
    idev->data.stk.interval.y = fabs( (double)(idev->data.stk.interval.y - ndcpt.y) );
 
    /* set editpos to the editpos specified in initstroke             */
    /* NOTE: this will be incremented by incoming data!  From here on */
    /*       do not confuse it with the initstroke version of editpos */
    idev->data.stk.editpos = 
           idev->data.stk.initst.record.pet1.editpos-1;           /* c1014 */
 
    idev->active = True; /* activate the stroke device */
    /* if echo is set to on... */
    if ( idev->data.stk.initst.esw == GECHO )
	/* update prompt (display initial points etc?) */
         XgksStkUpdatePrompt( ws, idev, PROMPTON, &(idev->data.stk.stkbuf[idev->data.stk.editpos]),
             (XMotionEvent *)NULL,-1 );	/*MIT*/ /* PTR c1133 c1177 */
 
/* wait for trigger or break */
        idev->touched = False;
        idev->breakhit = False;
        while( (idev->touched == False) && (idev->breakhit == False) )
                sigpause( 0 );
 
	/* deactivate stroke device */
        idev->active = False;
        if ( idev->data.stk.initst.esw == GECHO )
                XgksStkUpdatePrompt( ws, idev, PROMPTOFF, (Gpoint *)NULL, 
                   (XMotionEvent *)NULL,-1 ); /*MIT*/ /* PTR c1133 */
 
	/* if user hit <break>, return GNONE, else return GOK */
        if ( (idev->breakhit == True) ) {
                response->status = GNONE;
        }
        else {
                response->status = GOK;
        /* get a buffer to store the points in */
        spt= ndcpts= (Gpoint *)malloc( sizeof(Gpoint) *idev->data.stk.editpos );
        GKSERROR( spt == NULL, 300, errgreqstroke)

	 /* convert all points from DC to NDC (and copy into ndcpts) */
        for( i=0, dpt = idev->data.stk.stkbuf;
            i<idev->data.stk.editpos; i++, spt++, dpt++) {
            DcToNdc( ws, dpt, spt);
        }
        /* find the ntrans and WC points */
        if ( idev->data.stk.editpos > 0 )
            XgksFindNTransNpts( idev->data.stk.editpos, ndcpts, 
                                    &(response->stroke.transform), 
                                    response->stroke.points );
        response->stroke.n_points = idev->data.stk.editpos;
        free( ndcpts );
	/* NOTE: editpos at this point is not the editpos set in initstroke. */
        /*       editpos is incremented as new points are added to the       */
        /*       buffer.  It is reset at the beginning of request stroke to  */
        /*       the initialized value.  (It is also one less - the user     */
        /*       refers to the edit position starting at 1, while internally */
        /*       it starts at 0)                                             */
    }
 
    return( OK );
}

/*
 * SAMPLE STROKE
 *
 * Returns: 0, 7, 20, 25, 38, 140, 142
 *
 * See Also: ANSI Standard p.135
 */
 
gsamplestroke( ws_id, dev, response )
    Gint ws_id;
    Gint dev;
    Gstroke *response;
{
	/*
	 * declare local functions
	 */
	static Bool XgksCreateDefStroke();	/*MIT*/
	/*
	 * declare global functions
	 */
	Bool XgksFindNTransNpts();		/*MIT	c1014*/

    WS_STATE_ENTRY *ws;
    INPUT_DEV *idev;
    Gpoint *spt, *dpt, *ndcpts;
    int i;
 
/* STEP 1: check for errors. */
/* proper gks state? */
        GKSERROR( (xgks_state.gks_state == GGKCL || xgks_state.gks_state == GGKOP), 7, errgsamplestroke ) /* c1147 */
 
/* check for invalid workstation id */
/* DWO 7/28/88  added check to differentiate between */
/*              error 20 and error 25 for PTR c1012  */
        GKSERROR ( (!VALID_WSID(ws_id)), 20, errgsamplestroke)   /* c1012 */

/* open wsid? */
/* DWO 7/26/88  changed macro name from VALID_WSID */
        GKSERROR( !(ws=OPEN_WSID(ws_id)), 25, errgsamplestroke )  /* c1012 */
 
/* valid workstation type */
        GKSERROR( (WS_CAT(ws) != GOUTIN && WS_CAT(ws) != GINPUT), 38, errgsamplestroke)
 
/* valid locator device number */
        GKSERROR( (dev < 1), 140, errgsamplestroke )
 
	idev = XgksIDevLookup( ws, dev, GISTROKE);	/* AUG */
	GKSERROR( (idev == NULL) || (idev->data.stk.initst.mode != GSAMPLE), 142, errgsamplestroke)  /* AUG */

/* Make sure the workstation is up to date */
    gupdatews( ws_id, GPERFORM );
 
/* Convert current measure to WC space */
    spt = ndcpts = (Gpoint *)malloc( sizeof(Gpoint) * idev->data.stk.editpos );
    GKSERROR( spt == NULL, 300, errgsamplestroke)
    for( i=0, dpt = idev->data.stk.stkbuf;
        i<idev->data.stk.editpos; i++, spt++, dpt++) {
        DcToNdc( ws, dpt, spt);
    }
    /* find the ntrans and WC points */
    if ( idev->data.stk.editpos > 0)
        XgksFindNTransNpts( idev->data.stk.editpos, ndcpts, &(response->transform), response->points );
    response->n_points = idev->data.stk.editpos;
    free( ndcpts );
 
    return( OK );
}

/*
 * INQUIRE STROKE DEVICE STATE
 *
 * GKS mallocs space for the list of initial stroke points, this memory
 * should be freeed by the user when no longer needed.
 *
 * Returns: 0, 7, 20, 25, 38, 140, 2000
 *
 * See Also: ANSI Standard p.167
 */
 
ginqstrokest(ws_id, dev, type, state )
    Gint ws_id;
    Gint dev;
    Gqtype type;
    Gstrokest *state;
{
	/*
	 * declare local functions
	 */
	static Bool XgksCreateDefStroke();	/*MIT*/

    WS_STATE_ENTRY *ws;
    INPUT_DEV *idev;
    Gpoint *spt, *dpt;
    int i;
 
/* STEP 1: check for errors. */
/* proper gks state? */
        GKSERROR( (xgks_state.gks_state == GGKCL || xgks_state.gks_state == GGKOP), 7, errginqstrokest ) /* c1147 */
 
/* check for invalid workstation id */
/* DWO 7/28/88  added check to differentiate between */
/*              error 20 and error 25 for PTR c1012  */
        GKSERROR ( (!VALID_WSID(ws_id)), 20, errginqstrokest)   /* c1012 */

/* open wsid? */
/* DWO 7/26/88  changed macro name from VALID_WSID */
        GKSERROR( !(ws=OPEN_WSID(ws_id)), 25, errginqstrokest )  /* c1012 */
 
/* valid workstation type */
        GKSERROR( (WS_CAT(ws) != GOUTIN && WS_CAT(ws) != GINPUT), 38, errginqstrokest)
 
/* valid locator device number */
        GKSERROR( (dev < 1), 140, errginqstrokest )
 
/* valid enumeration */
    GKSERROR( (type != GSET) && (type != GREALIZED), 2000, errginqstrokest)
 
        if ((idev = XgksIDevLookup( ws, dev, GISTROKE)) == NULL) {
        /* Create the Input Device structure */
                GKSERROR( XgksCreateDefStroke( ws, dev, &idev ), 300, errginqstrokest)
        }
    *state = idev->data.stk.initst;
    dpt = state->stroke.points = (Gpoint *)malloc( sizeof(Gpoint) * state->stroke.n_points );
    GKSERROR( state->stroke.points == NULL, 300, errginqstrokest)
    for( i=0, spt = idev->data.stk.initst.stroke.points;
        i<state->stroke.n_points; i++, spt++, dpt++)
        *dpt = *spt;
 
    /* if idev->data.stk.initst.record.pet?.data pointed to anything it
     * would be copied here.
     */
 
     return( OK );
}

/*
 * INQUIRE DEFAULT STROKE DEVICE DATA
 *
 * GKS mallocs space for the list of supported prompt types, this memory should
 * be freeed by the user when no longer needed.
 *
 * Returns: 0, 8, 22, 23, 38, 140, 300
 *
 * See Also: ANSI Standard p.186
 */
 
ginqdefstroke( type, dev, data )
    Gchar *type;
    Gint dev;
    Gdefstroke *data;
{
        EWSTYPE ewstype;
 
/* STEP 1: check for errors */
        GKSERROR( (xgks_state.gks_state == GGKCL), 8, errginqdefstroke) /* c1147 */
 
/* valid wsid? */
        ewstype = XgksWsTypeToEnum( type );
        GKSERROR( ewstype == WST_INVALID, 22, errginqdefstroke )
        GKSERROR( ewstype != X_WIN, 38, errginqdefstroke )
 
/* valid stroke dev number */
        GKSERROR( dev < 1, 140, errginqdefstroke )
 
/* STEP 2: set up the return values */
    data->bufsiz = 64;          /* c2065 */
 
    data->pets.number = 3;
    data->pets.integers = (Gint *) malloc( sizeof( Gint ) * 3);
        GKSERROR( data->pets.integers == NULL, 300, errginqdefstroke )
    data->pets.integers[0] = 1;
    data->pets.integers[1] = 3;
    data->pets.integers[2] = 4;
 
    data->e_area.xmin = 0.0;
    data->e_area.xmax = WS_MAX_DCX;
    data->e_area.ymin = 0.0;
    data->e_area.ymax = WS_MAX_DCY;
 
    data->record.pet1.bufsiz = 64;            /* c2065 */
    data->record.pet1.editpos = 1;            /* c1014 */
    data->record.pet1.interval.x = 0.001;     /* c2066 */
    data->record.pet1.interval.y = 0.001;     /* c2066 */
    data->record.pet1.time = 0.0;
    data->record.pet1.data = NULL;
 
    return( OK );
}

/*
 * create a default string device. returns True if cannot creat.
 */
static Bool
XgksCreateDefStroke( ws, dev, idevp )
    WS_STATE_ENTRY *ws;
    Gint dev;
    INPUT_DEV **idevp;
{
    XGCValues gcvalues;
    INPUT_DEV *idev;
    
    idev = XgksIDevNew();    /*c1148*/
    if (idev == NULL)
        return( True );
    idev->class = GISTROKE;
    idev->dev = dev;
    idev->active = False;
    gcvalues.function = GXinvert;
    gcvalues.foreground = ws->wsfg;
    gcvalues.background = ws->wsbg;
    gcvalues.line_width = 0;
    gcvalues.line_style = LineSolid;
    gcvalues.fill_style = FillSolid;
    idev->gc = XCreateGC( ws->dpy, ws->win,
        GCFunction | GCForeground | GCBackground |
        GCLineWidth | GCLineStyle | GCFillStyle,
        &gcvalues);
    idev->data.stk.initst.mode = GREQUEST;
    idev->data.stk.initst.esw = GECHO;
    idev->data.stk.initst.stroke.transform = 0;
    idev->data.stk.initst.stroke.n_points = 0;
    idev->data.stk.initst.stroke.points = NULL;
    idev->data.stk.initst.pet = 1;
    idev->data.stk.initst.e_area.xmin = 0.0;
    idev->data.stk.initst.e_area.xmax = ws->size.x;
    idev->data.stk.initst.e_area.ymin = 0.0;
    idev->data.stk.initst.e_area.ymax = ws->size.y;
    idev->data.stk.initst.record.pet1.bufsiz = 64;              /* c2065 */
    idev->data.stk.initst.record.pet1.editpos = 1;              /* c1014 */
    idev->data.stk.initst.record.pet1.interval.x = 0.001;       /* c2066 */
    idev->data.stk.initst.record.pet1.interval.y = 0.001;       /* c2066 */
    idev->data.stk.initst.record.pet1.time = 0.0;
    idev->data.stk.initst.record.pet1.data = NULL;
 
    idev->data.stk.stkbuf = (Gpoint *) malloc( sizeof( Gpoint ) * 64);/*c2065*/
    if (idev->data.stk.stkbuf == NULL)
        return( True );
    idev->data.stk.stkbuf[0].x = 0.5;
    idev->data.stk.stkbuf[0].y = 0.5;
    idev->data.stk.interval.x = 0.001;                          /* c2066 */
    idev->data.stk.interval.y = 0.001;                          /* c2066 */
    idev->data.stk.editpos = 0;
 
/* link the new device into the list */
        idev->next = ws->in_dev_list;
        ws->in_dev_list = idev;
 
        *idevp = idev;
        return( False );
}


/*
 * XgksStkUpdatePrompt - update the stroke prompt
 */
 
XgksStkUpdatePrompt( ws, idev, pstate, newdcpt, xmev, event_id ) /* PTR c1133 */
    WS_STATE_ENTRY *ws;
    INPUT_DEV *idev;
    PromptStatus pstate;
    Gpoint *newdcpt;
    XMotionEvent *xmev;
    int event_id;        /* PTR c1133 */
{
	/*
	 * declare global functions
	 */
	Bool XgksFindNTransNpts();	/*MIT	c1014*/

    Gpoint prev, ndcpt, *ndcpts, *spt, *dpt;
    Gstroke *data;
    XRectangle rect;
    XPoint *xpts, *xpoints;
    int i;
 
#define STKBUFSIZ(P)    (idev->data.stk.initst.record.P.bufsiz)
#define STKEDITPOS    (idev->data.stk.editpos)
#define MTYPE    (idev->data.stk.initst.record.pet3.mk.bundl.type)
#define MSIZE    (idev->data.stk.initst.record.pet3.mk.bundl.size)
 
    rect.x = 0;
    rect.y = 0;
    rect.width = ws->wbound.x;
    rect.height = ws->wbound.y;
    XSetClipRectangles(ws->dpy, idev->gc, 0, 0, &rect, 1, Unsorted);
 
    switch( pstate ) {
    case PROMPTON:
    /* transform the points */
        xpoints = (XPoint *)malloc( sizeof( XPoint ) * STKEDITPOS );
        for( i=0, ndcpts=idev->data.stk.stkbuf, xpts=xpoints;
            i< STKEDITPOS; i++, ndcpts++, xpts++ ) {
            DcToX( ws, ndcpts, xpts );
        }
        switch( idev->data.stk.initst.pet ) {
        default:
        case 1:        /* tracking cross uses X cursor */
            break;
        case 3:    /* polymarkers */
            XgksDrawMarkers( ws->dpy, ws->win, idev->gc, xpoints, STKEDITPOS, MTYPE, MSIZE );
            break;
        case 4:    /* polyline */
            if (STKEDITPOS > 1)
                XgksXDrawLines( ws->dpy, ws->win, idev->gc, xpoints, /* c1151 */
                         STKEDITPOS, CoordModeOrigin );              /* c1151 */
            break;
        }
        free( xpoints );
        break;
    case PROMPTOFF:
    /* transform the points */
        xpoints = (XPoint *)malloc( sizeof( XPoint ) * STKEDITPOS );
        for( i=0, ndcpts=idev->data.stk.stkbuf, xpts=xpoints;
            i< STKEDITPOS; i++, ndcpts++, xpts++ ) {
            DcToX( ws, ndcpts, xpts );
        }
        switch( idev->data.stk.initst.pet ) {
        default:
        case 1:        /* tracking cross uses X cursor */
            break;
        case 3:    /* polymarker */
            XgksDrawMarkers( ws->dpy, ws->win, idev->gc, xpoints, STKEDITPOS, MTYPE, MSIZE );
            break;
        case 4:    /* polyline */
            if (STKEDITPOS > 1)
                XgksXDrawLines( ws->dpy, ws->win, idev->gc, xpoints, /* c1151 */
                         STKEDITPOS, CoordModeOrigin );              /* c1151 */
            break;
        }
        free( xpoints );
        break;
    case PROMPTMOVE:
    /* stroke point must lie within the workstation window */
        DcToNdc( ws, newdcpt, &ndcpt);
#ifdef STKDEBUG
        fprintf(stderr,"XgksStkUpdatePrompt( DC= %f %f NDC %f %f )\n", newdcpt->x, newdcpt->y, ndcpt.x, ndcpt.y);
        fprintf(stderr,"    editpos %d bufsiz %d mtype %d msize %f\n",
            STKEDITPOS, STKBUFSIZ(pet1), MTYPE, MSIZE);
#endif
 
        if (ndcpt.x < ws->wsti.current.w.xmin
                || ndcpt.x > ws->wsti.current.w.xmax
                || ndcpt.y < ws->wsti.current.w.ymin
                || ndcpt.y > ws->wsti.current.w.ymax )
            return( 0 );
 
        if (STKEDITPOS > 0) {
            DcToNdc( ws, &(idev->data.stk.stkbuf[STKEDITPOS-1]), &prev);
#ifdef STKDEBUG
            fprintf(stderr, "XgksStkUpdatePrompt: dx %f dy %f\n",
                fabs((double)(ndcpt.x - prev.x)), fabs((double)(ndcpt.y - prev.y)) );
#endif
        }
        if ( (STKEDITPOS == 0)
            || (fabs((double)(ndcpt.x - prev.x)) >= idev->data.stk.interval.x)
            || (fabs((double)(ndcpt.y - prev.y)) >= idev->data.stk.interval.y) ) {
        /* transform the points */
            xpoints=(XPoint *)malloc(sizeof(XPoint)*(STKEDITPOS+1)); /* c1151 */
            for( i=0, ndcpts=idev->data.stk.stkbuf, xpts=xpoints;
                i< STKEDITPOS; i++, ndcpts++, xpts++ ) {
                DcToX( ws, ndcpts, xpts );
            }
            switch( idev->data.stk.initst.pet ) {
            default:
            case 1:        /* tracking cross uses X cursor */
                if (STKEDITPOS < STKBUFSIZ(pet1)) {
                    idev->data.stk.stkbuf[ STKEDITPOS ] = *newdcpt;
                    STKEDITPOS++;
                }
                break;
            case 3:    /* polymarker */                              /* c1151 */
                if (STKEDITPOS < STKBUFSIZ(pet3)) {                  /* c1151 */
                   idev->data.stk.stkbuf[ STKEDITPOS ] = *newdcpt;   /* c1151 */
                   DcToX( ws, &(idev->data.stk.stkbuf[ STKEDITPOS ]),/* c1151 */
                             &(xpoints[ STKEDITPOS ]) );             /* c1151 */
                   STKEDITPOS++;                                     /* c1151 */
                   if(idev->data.stk.initst.esw==GECHO)              /* c1151 */
                      XgksDrawMarkers( ws->dpy, ws->win, idev->gc,   /* c1151 */
                            &(xpoints[STKEDITPOS-1]),1,MTYPE,MSIZE );/* c1151 */
                }
                break;
            case 4:    /* polyline */
                if (STKEDITPOS < STKBUFSIZ(pet4)) {                  /* c1151 */
                  idev->data.stk.stkbuf[ STKEDITPOS ] = *newdcpt;    /* c1151 */
                  DcToX( ws, &(idev->data.stk.stkbuf[STKEDITPOS]),   /* c1151 */
                           &(xpoints[ STKEDITPOS ]) );               /* c1151 */
                  STKEDITPOS++;                                      /* c1151 */
                  if (idev->data.stk.initst.esw == GECHO             /* c1151 */
                             && STKEDITPOS > 1)                      /* c1151 */
                     XgksXDrawLines( ws->dpy, ws->win, idev->gc,     /* c1151 */
                        &(xpoints[STKEDITPOS-2]),2,CoordModeOrigin); /* c1151 */
                }
                break;
            }
            free( xpoints );
        }
        switch(idev->data.stk.initst.mode ) {
        default:
        case GREQUEST:
            if (xmev->type != ButtonRelease)
                break;
            idev->touched = True;
            XBell( ws->dpy, 0);
            break;
        case GSAMPLE:
            break;
        case GEVENT:
            if (xmev->type != ButtonRelease)
                break;
            data = (Gstroke *) malloc( sizeof( Gstroke ));
            if ( data == NULL )
	    {
                gerrorhand( 300, errXgksStkUpdatePrompt, xgks_state.gks_err_file ); /* c1147 */
	        return(300);
	    }
            else {
                XBell( ws->dpy, 0);
            /* Convert current measure to WC space */
                spt = ndcpts = (Gpoint *)malloc( sizeof(Gpoint) * STKEDITPOS );
                data->points = (Gpoint *)malloc( sizeof(Gpoint) * STKEDITPOS );
                GKSERROR( spt == NULL || data->points == NULL, 300, errXgksStkUpdatePrompt)
                for( i=0, dpt = idev->data.stk.stkbuf;
                    i<STKEDITPOS; i++, spt++, dpt++) {
                    DcToNdc( ws, dpt, spt);
                }
                /* find the ntrans and WC points */
                if ( STKEDITPOS > 0 )
                    XgksFindNTransNpts( STKEDITPOS, ndcpts, &(data->transform), data->points );
                data->n_points = STKEDITPOS;
                free( ndcpts );
                XgksEnqueueEvent( ws->ws_id, idev->dev, GISTROKE, 
                  (char *)data,event_id); /*MIT*/ /* PTR c1133 */
            }
            break;
        }
        break;
    default:
        break;
    }
 
    XFlush(ws->dpy);
    return( 0 );
}

/*
 * free all memory associate with a stroke logical input device
 */
XgksStkDelete( ws, idev )
    WS_STATE_ENTRY *ws;
    INPUT_DEV *idev;
{
    XFreeGC( ws->dpy, idev->gc );
    if ( idev->data.stk.initst.stroke.points != NULL )
        free( idev->data.stk.initst.stroke.points );
    
/* this used to say:                                             */
    /*   if ( idev->data.stk.initst.record.pet1.data != NULL )       */
    /*     free( idev->data.stk.initst.record.pet1.data );           */
    /* ...but... record.pet1, .pet2, .pet3, and .pet4 are a union.   */
    /* In many cases in GKS some part of the union is refered to     */
    /* by .pet1.??? regardless of which pet is currently appropriate.*/
    /* This works sometimes (when the .??? part is the same in both  */
    /* pets and in the same position, and with the same amount of    */
    /* memory defined above it).  This is not the case here.         */
    /* (It's probably not a good practice to assume the union will   */
    /* always remain the way it is today anyway!)     (DWO)          */
    switch ( idev->data.stk.initst.pet )                       /* PTR c1120 */
      {                                                        /* PTR c1120 */
      case 1: /* pet 1 */                                      /* PTR c1120 */
         if ( idev->data.stk.initst.record.pet1.data != NULL ) /* PTR c1120 */
           free( idev->data.stk.initst.record.pet1.data );     /* PTR c1120 */
         break;                                                /* PTR c1120 */
      case 2: /* pet 2 */                                      /* PTR c1120 */
         if ( idev->data.stk.initst.record.pet2.data != NULL ) /* PTR c1120 */
           free( idev->data.stk.initst.record.pet2.data );     /* PTR c1120 */
         break;                                                /* PTR c1120 */
      case 3: /* pet 3 */                                      /* PTR c1120 */
         if ( idev->data.stk.initst.record.pet3.data != NULL ) /* PTR c1120 */
           free( idev->data.stk.initst.record.pet3.data );     /* PTR c1120 */
         break;                                                /* PTR c1120 */
      case 4: /* pet 4 */                                      /* PTR c1120 */
         if ( idev->data.stk.initst.record.pet4.data != NULL ) /* PTR c1120 */
           free( idev->data.stk.initst.record.pet4.data );     /* PTR c1120 */
         break;                                                /* PTR c1120 */
      }                                                        /* PTR c1120 */

    if ( idev->data.stk.stkbuf != NULL )
        free( idev->data.stk.stkbuf );
}

/* this function replaces calls to XDrawLines for stroke input devices  */
/* because...  XDrawLines makes nice corners and intelligently handles  */
/* lines that cross over themselves.  This is great if your line is     */
/* defined before it is drawn.  But, as in the case of our stroke, our  */
/* polyline is changing.  We need to be able to extend an existing line */
/* and be able to erase it without leaving trash behind.  So... this    */
/* function will make multiple calls to XDrawLine.                      */

XgksXDrawLines( dpy, win, gc, xpts, npts, mode )         /* c1151 */
  Display *dpy;                                          /* c1151 */
  Drawable win;                                          /* c1151 */
  GC gc;                                                 /* c1151 */
  XPoint *xpts;                                          /* c1151 */
  int npts;                                              /* c1151 */
  int mode;                                              /* c1151 */
  {                                                      /* c1151 */
  int i;                                                 /* c1151 */
                                                         /* c1151 */
  if (npts > 1)                                          /* c1151 */
    {                                                    /* c1151 */
    if (mode != CoordModeOrigin)                         /* c1151 */
      /* convert to CoordModeOrigin */                   /* c1151 */
      for (i=1; i<npts; i++)                             /* c1151 */
        {                                                /* c1151 */
        xpts[i].x += xpts[i-1].x;                        /* c1151 */
        xpts[i].y += xpts[i-1].y;                        /* c1151 */
        }                                                /* c1151 */
                                                         /* c1151 */
    for (i=1; i<npts; i++)                               /* c1151 */
      XDrawLine (dpy,win,gc, xpts[i-1].x,xpts[i-1].y,    /* c1151 */
                             xpts[i].x,  xpts[i].y);     /* c1151 */
                                                         /* c1151 */
    if (mode != CoordModeOrigin)                         /* c1151 */
      /* convert back to CoordModePrevious */            /* c1151 */
      for (i=npts-1; i>0; i--)                           /* c1151 */
        {                                                /* c1151 */
        xpts[i].x -= xpts[i-1].x;                        /* c1151 */
        xpts[i].y -= xpts[i-1].y;                        /* c1151 */
        }                                                /* c1151 */
    }                                                    /* c1151 */
  }                                                      /* c1151 */
