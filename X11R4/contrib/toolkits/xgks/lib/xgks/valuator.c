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
 * Valuator.c - routines for XGKS VALUATOR input device
 *
 *    ginitval()
 *    gsetvalmode()
 *    greqval()
 *    gsampleval()
 *    ggetval()
 *
 *    i_dev_pic_data()
 *    i_pic_state()
 * 
 * $Header: valuator.c,v 4.0 89/08/31 16:25:49 amy Exp $
 *
 * $Source: /andrew/Xgks/source/xgks.bld/src/RCS/valuator.c,v $
 *
 * $Log:	valuator.c,v $
 * Revision 4.0  89/08/31  16:25:49  amy
 * Changed IBM copyright for MIT distribution.
 * 
 * Revision 3.24  89/06/05  10:18:16  bruce
 * DCR# d1:	Changed include file name from gks_implement.h
 * 		to gks_implem.h for AIX compiler.
 * 
 * Revision 3.23  89/05/22  09:24:27  bruce
 * PTR# c1177:  Made changes to code in the Update Prompt calls and in other places to 
 * 		insure that NULL is not dereferenced.
 * 
 * Revision 3.22  89/02/04  15:31:10  amy
 * PTR c1147	Make global vars. and functions private, and static where possible.
 * 
 * Revision 3.21  88/12/16  12:44:52  amy
 * PTR c1133	XgksValUpdatePrompt:  add event id to parameter list.
 * 		XgksEnqueueEvent calls:  add event id to parameter list.
 * 
 * Revision 3.20  88/12/08  13:41:12  amy
 * PTR c1130	GKSERROR and gerrorhand calls:  changed function name parameter
 * 		from character string to enum. type value.
 * Changed spelling of color to colour.
 * 
 * Revision 3.19  88/12/01  11:48:29  amy
 * New 4.3 C compiler--	
 * 	ginqvalst:  remove definition of variable High.
 * 	Assign default value to Low.
 * 	XgksSetValEchoAttr:  assign default values to Low and High.
 * 
 * Revision 3.18  88/11/05  10:07:45  amy
 * PTR c1018	ginqvalst:  to return the initial value, first convert back from
 * 		device coordinates.
 * 		greqval:  when called with an uninitialized device, create 
 * 		new device and give it the number passed in to greqval.
 * 
 * Revision 3.17  88/11/04  10:47:53  amy
 * PTR c1057	gsetvalmode:  set up SIGALRM signal handler if mode = GEVENT.
 * 
 * Revision 3.16  88/11/01  13:32:13  amy
 * PTR c1087	ginqdefval:  added check for error 22, ws type invalid.
 * 
 * Revision 3.15  88/10/11  09:03:22  amy
 * PTR c1015:  Changed the valid pet check to accept 1 or 2.
 * 		 Added case labels for pet 2 in helper function XgksValUpdatePrompt
 * 		 so that pet 2 operates identically to pet 1.
 * 
 * Revision 3.14  88/09/26  09:38:00  amy
 * MIT	XgksValUpdatePrompt:  call with correct number of parameters.
 * 	XgksEnqueueEvent:  recast data parameter in call to a (char *).
 * 
 * Revision 3.13  88/08/18  14:31:35  amy
 * AUG  gsetvalmode:  added check for invalid device.
 *        When initializing device, sets
 *        device number to inputted value.
 *      gsampleval:  changed check for valid device number.
 *        Changed check for SAMPLE mode.
 * 
 * Revision 3.12  88/08/11  09:17:19  amy
 * PTR c1012  ginitval, ginqvalst, greqval, gsampleval & gsetvalmode:
 * added call to VALID_WSID to check for error 20.
 * 
 * Revision 1.3  88/07/28  15:15:59  owens
 * added check for error 20 for PTR c1012
 * 
 * Revision 1.2  88/07/26  17:54:35  owens
 * renamed/replaced VALID_WSID macro for PTR c1012 (DWO)
 * 
 * Revision 1.1  88/07/21  10:50:44  david
 * Initial revision
 *  
 *
 */
 
static char *rcsid = "$Header: valuator.c,v 4.0 89/08/31 16:25:49 amy Exp $";

#include "gks_implem.h"                 /* d1 */
#include <signal.h>		/*c1057*/

extern  XgksAwaitInterrupt();	/*  Declare  Interrupt function   PTR# c1057  */
 
 
/* As under current implementation pets 1 and 2 are supported */  /* c1015 */
#define SUPPORTED_VAL_PROMPT(pet)    ((pet==1) || (pet==2))       /* c1015 */
 
 
#define VAL_FOREGROUND    ws->wsfg
#define VAL_BACKGROUND    ws->wsbg
#define VAL_FILL_STYLE    FillSolid
 
 
#define DcToVal(dc, convert)    ( (dc)*(convert[0]) - (convert[1]) ) 
 
 
/*$F
 * ginitval(ws_id, dev, init, pet, area, record) - INITIALISE VAL
 *
 * Gint     ws_id;        workstation identifier
 * int        dev;        val device number 
 * Gfloat    *init;        initial val pointer
 * Gint        pet;        Prompt and echo type
 * Glimit   *area;        Echo area pointer in DC
 * Gvalrec *record;        val data record pointer
 *
 * returns: 0, 7, 20, 25, 38, 51, 140, 141, 144, 145, 146, 152
 *
 * See Also: ANSI Standard p.123
 */
ginitval(ws_id, dev, init, pet, area, record) 
    Gint ws_id, dev, pet;
    Gfloat  init;
    Glimit *area;
    Gvalrec *record;
{
    /* declare local functions */				/* c1147 */
    static void XgksSetValEchoAttr ();				/* c1147 */

    WS_STATE_PTR ws;
    INPUT_DEV *idev;
    XGCValues gcvalues;
 
/* STEP 1: check for errors */
/*    gks in proper state? */
    GKSERROR ((xgks_state.gks_state == GGKCL || xgks_state.gks_state == GGKOP) , 7, errginitval) /*c1147*/
 
/* check for invalid workstation id */
/* DWO 7/28/88  added check to differentiate between */
/*              error 20 and error 25 for PTR c1012  */
        GKSERROR ( (!VALID_WSID(ws_id)), 20, errginitval)   /* c1012 */

/* open workstation */
/* DWO 7/26/88  changed macro name from VALID_WSID */
    GKSERROR ((!(ws=OPEN_WSID(ws_id))) , 25, errginitval)  /* c1012 */
 
/* check category */
    GKSERROR (((WS_CAT(ws) != GOUTIN)&&(WS_CAT(ws) != GINPUT)) , 38, errginitval)
 
/* rectangle defintion ok */
    GKSERROR ((area->xmin >= area->xmax || area->ymin >= area->ymax) , 51, errginitval)
 
/* valid val device number */
    GKSERROR ((dev < 1), 140, errginitval)
 
/* prompt and echo type supported */
    GKSERROR ((!SUPPORTED_VAL_PROMPT(pet)) , 144, errginitval)
 
/* echo area within display space */
    GKSERROR ((area->xmin<0 || area->xmax>ws->size.x || area->ymin<0 || area->ymax>ws->size.y) , 145, errginitval)
 
/* check for valid data-record and initial values */
    switch (pet) {
        case 1: GKSERROR( (record->pet1.low>record->pet1.high), 146, errginitval);
            GKSERROR( (init>record->pet1.high || init<record->pet1.low), 152, errginitval);
            record->pet1.data = NULL;
            break;
        case 2: GKSERROR( (record->pet2.low>record->pet2.high), 146, errginitval);
            GKSERROR( (init>record->pet2.high || init<record->pet2.low), 152, errginitval);
            record->pet2.data = NULL;
            break;
        case 3: GKSERROR( (record->pet3.low>record->pet3.high), 146, errginitval);
            GKSERROR( (init>record->pet3.high || init<record->pet3.low), 152, errginitval);
            record->pet3.data = NULL;
            break;
    }
 
/* Check if the device already exist */
    if ( (idev = XgksIDevLookup(ws, dev, GVALUATOR)) == NULL) {
    /* Build a new input device structure */
        gcvalues.function = GXcopy;
        gcvalues.foreground = VAL_FOREGROUND;
        gcvalues.background = VAL_BACKGROUND;
        gcvalues.line_width = 0;
        gcvalues.line_style = LineSolid;
        gcvalues.fill_style = VAL_FILL_STYLE;
        idev = XgksIDevNew ();
        idev->gc = XCreateGC (ws->dpy, ws->win, 
                  GCFunction  | GCForeground | GCBackground |
                  GCLineWidth | GCLineStyle  | GCFillStyle, &gcvalues);
        idev->class = GVALUATOR;
        idev->dev = dev;
        idev->data.val.initst.mode = GREQUEST;    /* initialize to GREQUEST */
        idev->data.val.initst.esw  = GECHO;
    /* Add into workstation input device queue */
        XgksIDevAdd (ws, idev);
    } else {
    /* val device must be in REQUEST mode */
        GKSERROR ((idev->data.val.initst.mode!=GREQUEST), 141, errginitval)
    }
    idev->data.val.initst.val = init;
    idev->data.val.initst.pet = pet;
    idev->data.val.initst.e_area = *area;
    idev->data.val.initst.record = *record;
    XgksSetValEchoAttr(&(idev->data.val));
    return( OK );
}

/*$F
 * gsetvalmode(ws_id, dev, mode, echo) - SET VALUATOR MODE
 *
 * Gint ws_id;            workstation identifier
 * Gint    dev;            val device number
 * Gimode mode,            operating mode (GREQUEST | GSAMPLE | GEVENT)
 * Gesw  echo;            echo switch (GECHO | GNOECHO)
 *
 * Returns: 0, 7, 20, 25, 38, 140, 143, 2000
 *
 * See Also: ANSI Standard p.129
 */
gsetvalmode(ws_id, dev, mode, echo)
    Gint ws_id, dev;
    Gimode mode;
    Gesw echo;
{
    /* declare local functions */				/* c1147 */
    static void XgksSetValEchoAttr ();				/* c1147 */

    WS_STATE_PTR ws;
    INPUT_DEV *idev;
    XGCValues gcvalues;
 
/* STEP 1: check for errors */
/*     gks in proper state */
    GKSERROR ((xgks_state.gks_state == GGKCL || xgks_state.gks_state == GGKOP) , 7, errgsetvalmode) /*c1147*/
 
/* check for invalid workstation id */
/* DWO 7/28/88  added check to differentiate between */
/*              error 20 and error 25 for PTR c1012  */
        GKSERROR ( (!VALID_WSID(ws_id)), 20, errgsetvalmode)   /* c1012 */

/* workstation id open */
/* DWO 7/26/88  changed macro name from VALID_WSID */
    GKSERROR ((!(ws=OPEN_WSID(ws_id))) , 25, errgsetvalmode)  /* c1012 */
 
/* check category */
    GKSERROR (((WS_CAT(ws)!=GOUTIN)&&(WS_CAT(ws)!=GINPUT)), 38, errgsetvalmode)

/* valid locator device number */
	GKSERROR( (dev < 1), 140, errgsetvalmode )		/* AUG */
 
/* check enumerated type values */
    GKSERROR (((mode != GREQUEST && mode != GSAMPLE && mode != GEVENT) || 
           (echo != GECHO && echo != GNOECHO) ) , 2000, errgsetvalmode)
 
/* STEP 2: tell the workstation */
    if ( (idev = XgksIDevLookup(ws, dev, GVALUATOR)) == NULL ) {
    /* Create a default device */
        gcvalues.function = GXcopy;
        gcvalues.foreground = VAL_FOREGROUND;
        gcvalues.background = VAL_BACKGROUND;
        gcvalues.line_width = 0;
        gcvalues.line_style = LineSolid;
        gcvalues.fill_style = VAL_FILL_STYLE;
        idev = XgksIDevNew ();
        idev->gc = XCreateGC (ws->dpy, ws->win, 
                  GCFunction  | GCForeground | GCBackground |
                  GCLineWidth | GCLineStyle  | GCFillStyle, &gcvalues);
        idev->class = GVALUATOR;
        idev->dev = dev;	/* AUG */
        idev->data.val.initst.mode = GREQUEST;    /* initialize to GREQUEST */
        idev->data.val.initst.esw  = GECHO;
        idev->data.val.initst.val = 0.5;
        idev->data.val.initst.pet = 1;
        idev->data.val.initst.e_area.xmin = 200.0;
        idev->data.val.initst.e_area.xmax = 800.0;
        idev->data.val.initst.e_area.ymin = 50.0;
        idev->data.val.initst.e_area.ymax = 100.0;
        idev->data.val.initst.record.pet1.high = 1.0;
        idev->data.val.initst.record.pet1.low = 0.0;
        XgksSetValEchoAttr(&(idev->data.val));
    /* Add into workstation input device queue */
        XgksIDevAdd (ws, idev);
    }
    idev->data.val.initst.mode = mode;
    idev->data.val.initst.esw  = echo;
 
    if ( mode == GSAMPLE || mode == GEVENT ) {
    /* Have to initialize device state */
        idev->data.val.CurPos = idev->data.val.initst.val;
	if (mode == GEVENT) 
	   signal( SIGALRM, XgksAwaitInterrupt); /* Set up signal call PTR# c1057 */
        idev->active = TRUE;
        if (echo == GECHO)
            XgksValUpdatePrompt (ws, idev, PROMPTON, &(idev->data.val.CurPos),
			 (XMotionEvent *)NULL,-1); /*MIT*/ /* PTR c1133 c1177 */
    } else  /* GREQUEST */
        idev->active = FALSE;
 
    return(OK);
}

/*$F
 * greqval(ws_id, dev, respons) - REQUEST VALUATOR
 *
 * Gint ws_id;            workstation identifier
 * Gint    dev;            val device number
 * Gqval *respons        Response of the val
 *      status -- GOK, GNONE
 *      val     -- valuator result
 *
 * returns: 0, 7, 20, 25, 38, 140, 141 
 *
 * See Also: ANSI Standard p.133 
 */ 
greqval(ws_id, dev, response)
    Gint ws_id,dev;
    Gqval *response;
{
    /* declare local functions */				/* c1147 */
    static void XgksSetValEchoAttr ();				/* c1147 */

    WS_STATE_PTR ws;
    INPUT_DEV *idev;
    XGCValues gcvalues;
 
/* STEP 1: check for errors */
/*     gks in proper state */
    GKSERROR ((xgks_state.gks_state == GGKCL || xgks_state.gks_state == GGKOP) , 7, errgreqval) /*c1147*/
 
/* check for invalid workstation id */
/* DWO 7/28/88  added check to differentiate between */
/*              error 20 and error 25 for PTR c1012  */
        GKSERROR ( (!VALID_WSID(ws_id)), 20, errgreqval)   /* c1012 */

/* workstation id open */
/* DWO 7/26/88  changed macro name from VALID_WSID */
    GKSERROR ((!(ws=OPEN_WSID(ws_id))) , 25, errgreqval)  /* c1012 */
 
/* check category */
    GKSERROR (((WS_CAT(ws)!=GOUTIN) && (WS_CAT(ws)!=GINPUT)) , 38, errgreqval)
 
/* valid val device */
    GKSERROR ((dev < 1) , 140, errgreqval)
 
/* ask the workstation for the device */
        if ( (idev = XgksIDevLookup(ws, dev, GVALUATOR)) == NULL ) {
    /* Create a default device */
        gcvalues.function = GXcopy;
        gcvalues.foreground = VAL_FOREGROUND;
        gcvalues.background = VAL_BACKGROUND;
        gcvalues.line_width = 0;
        gcvalues.line_style = LineSolid;
        gcvalues.fill_style = VAL_FILL_STYLE;
        idev = XgksIDevNew ();
        idev->gc = XCreateGC (ws->dpy, ws->win, 
                  GCFunction  | GCForeground | GCBackground |
                  GCLineWidth | GCLineStyle  | GCFillStyle, &gcvalues);
        idev->class = GVALUATOR;
        idev->dev = dev; /* was being set to 1, should be dev  PTR# c1018  */
        idev->data.val.initst.mode = GREQUEST;    /* initialize to GREQUEST */
        idev->data.val.initst.esw  = GECHO;
        idev->data.val.initst.val = 0.5;
        idev->data.val.initst.pet = 1;
        idev->data.val.initst.e_area.xmin = 200.0;
        idev->data.val.initst.e_area.xmax = 800.0;
        idev->data.val.initst.e_area.ymin = 50.0;
        idev->data.val.initst.e_area.ymax = 100.0;
        idev->data.val.initst.record.pet1.high = 1.0;
        idev->data.val.initst.record.pet1.low = 0.0;
        XgksSetValEchoAttr(&(idev->data.val));
    /* Add into workstation input device queue */
        XgksIDevAdd (ws, idev);
        } 
    else {
        GKSERROR((idev->data.val.initst.mode !=GREQUEST), 141, errgreqval)
    }
/* Make sure the workstation is up to date */
    gupdatews( ws_id, GPERFORM );
 
/* initiate the device and show prompt */
    idev->data.val.CurPos = idev->data.val.initst.val;
    if (idev->data.val.initst.esw == GECHO)
        XgksValUpdatePrompt (ws, idev, PROMPTON, &(idev->data.val.CurPos),
		     (XMotionEvent *)NULL,-1);	/*MIT*/ /* PTR c1133 c1177 */
    idev->active = TRUE;
 
/* wait for trigger or break */
    idev->touched = FALSE;
    idev->breakhit = FALSE;
    while ( (idev->touched == FALSE) && (idev->breakhit == FALSE) )
        sigpause (0);
 
    idev->active = FALSE;
    
    if (idev->data.val.initst.esw == GECHO)
        XgksValUpdatePrompt (ws, idev, PROMPTOFF, &(idev->data.val.CurPos),
		    (XMotionEvent *)NULL,-1);	/*MIT*/ /* PTR c1133 c1177 */
 
    if (idev->breakhit == TRUE)
        response->status = GNONE;
    else response->status = GOK;
 
/* if status is ok, value in response is already in 
   valuator scale, so simply return will do */
 
    response->val = idev->data.val.val.val;
 
    return( OK );
}
/*$F
 * gsampleval(ws_id, dev, response) - SAMPLE VALUATOR
 *
 * Gint ws_id;            workstation identifier
 * Gint    dev;            val device number
 * Gfloat *respons        Response of the val
 *
 * Returns: 0, 7, 20, 25, 38, 140, 142
 *
 * See Also: ANSI Standard p.136
 */
gsampleval(ws_id, dev, response)
    Gint ws_id, dev;
    Gfloat *response;
{
    WS_STATE_PTR ws;
    INPUT_DEV *idev;
 
/* STEP 1: check for errors */
/*     gks in proper state */
    GKSERROR ((xgks_state.gks_state == GGKCL || xgks_state.gks_state == GGKOP) , 7, errgsampleval) /*c1147*/
 
/* check for invalid workstation id */
/* DWO 7/28/88  added check to differentiate between */
/*              error 20 and error 25 for PTR c1012  */
        GKSERROR ( (!VALID_WSID(ws_id)), 20, errgsampleval)   /* c1012 */

/* workstation id open */
/* DWO 7/26/88  changed macro name from VALID_WSID */
    GKSERROR ((!(ws=OPEN_WSID(ws_id))) , 25, errgsampleval)  /* c1012 */
 
/* check category */
    GKSERROR (((WS_CAT(ws)!=GOUTIN) && (WS_CAT(ws)!=GINPUT)) , 38, errgsampleval)
 
/* valid val device */
    GKSERROR ( (dev<1), 140, errgsampleval)	/* AUG */
    idev=XgksIDevLookup(ws, dev, GVALUATOR);	/* AUG */

/* is current mode SAMPLE ? */
	GKSERROR ((idev==NULL) || (idev->data.val.initst.mode != GSAMPLE), 142, errgsampleval)  /* AUG */
 
/* Make sure the workstation is up to date */
    gupdatews( ws_id, GPERFORM );
 
/* Grep the current valuator value */
    *response = idev->data.val.val.val;
 
    return( OK );
}
 
/*    Valuator inquiries */
 
/*$F
 * ginqvalst(ws_id, dev, state) - INQUIRE VALUATOR DEVICE STATE
 * Gint  ws_id        workstation identifier
 * Gint  dev;        pick device number
 * Gvalst *state    Oitput of current pick state
 *
 * Errors: 7, 20, 25, 37, 140
 *
 * See Also: ANSI Standard p.168
 */
ginqvalst (ws_id, dev, state)
Gint ws_id, dev;
Gvalst *state;
{
    WS_STATE_ENTRY *ws;
    INPUT_DEV *idev;
    Gfloat    Low;  	/* PTR# c1018 */	/* New 4.3 C compiler */
 
/* STEP 1: check for errors. */
/* proper gks state? */
    GKSERROR( (xgks_state.gks_state == GGKCL || xgks_state.gks_state == GGKOP), 7, errginqvalst ) /*c1147*/
 
/* check for invalid workstation id */
/* DWO 7/28/88  added check to differentiate between */
/*              error 20 and error 25 for PTR c1012  */
        GKSERROR ( (!VALID_WSID(ws_id)), 20, errginqvalst)   /* c1012 */

/* check for ws_id, if correspond to opened ws */
/* DWO 7/26/88  changed macro name from VALID_WSID */
    GKSERROR( !(ws=OPEN_WSID(ws_id)), 25, errginqvalst )  /* c1012 */
 
/* valid workstation type */
    GKSERROR( (WS_CAT(ws) != GOUTIN && WS_CAT(ws) != GINPUT), 38, errginqvalst)
 
/* valid locator device number */
    GKSERROR( (dev < 1), 140, errginqvalst )
 
/* Copy the data to the user's structure */
    if ((idev = XgksIDevLookup( ws, dev, GVALUATOR )) == NULL) {
         state->mode = GREQUEST;
                state->esw  = GECHO;
                state->pet = 1;
                state->val = 0.5;
                state->e_area.xmin = 200.0;
                state->e_area.xmax = 800.0;
                state->e_area.ymin = 50.0;
                state->e_area.ymax = 100.0;
                state->record.pet1.high = 1.0;
                state->record.pet1.low = 0.0;
                state->record.pet1.data = NULL;
    }
    else {
        *state = idev->data.val.initst;
	
	/* Initial value had to be converted back from device coordinates  c1018 */

	switch (idev->data.val.initst.pet) {
            case 1: Low  = idev->data.val.initst.record.pet1.low;
                break;
            case 2: Low  = idev->data.val.initst.record.pet2.low;
                break;
            case 3: Low  = idev->data.val.initst.record.pet3.low;
                break;
	    default:  Low = 0.0;		/* New 4.3 C compiler */
	        break;				/* New 4.3 C compiler */
        }

	if (idev->data.val.axis == VAL_HORIZ)
	   state->val = (state->val - idev->data.val.SlidRule[0].x) *
			idev->data.val.convert[0] + Low;
	else
	   state->val = (state->val - idev->data.val.SlidRule[0].y) *
			idev->data.val.convert[0] + Low;
        /* if idev->data.loc.initst.record pointed anywhere, it would
         * be copied here.
         */
    }
 
    return( OK );
}
 
/*$F
 *
 * ginqdefval(type, dev, data) - INQUIRE DEFAULT DEVICE DATA
 * Gchar *type;      workstation type string
 * Gint  dev;        pick device number
 * Gdefval *data;    output default pick device data structure
 *
 * Returns all information in the parameters.
 * Errors: 0, 8, 22, 23, 38, 140
 *
 * See Also: ANSI Standard p.186
 */
ginqdefval(type, dev ,data)
Gchar *type; 
Gint dev;
Gdefval *data;
{
    EWSTYPE ewstype;
    int i;
 
/* STEP 1: check for errors. */
/* proper gks state? */
    GKSERROR( (xgks_state.gks_state == GGKCL), 8, errginqdefval ) /*c1147*/
 
/* valid wsid? */
    ewstype = XgksWsTypeToEnum( type );
    GKSERROR( (ewstype == WST_INVALID), 22, errginqdefval )	/*c1087*/
 
/* valid workstation type (assumes all INPUT and OUTIN workstations are X_WIN */
    GKSERROR( ewstype != X_WIN, 38, errginqdefval)
 
/* valid locator device? */
    GKSERROR( (dev < 1), 140, errginqdefval)
 
/* STEP 2: set up the return values */
    data->pets.number    = 1;
    data->pets.integers    = (Gint *) malloc( sizeof( Gint ) * data->pets.number);
    if ( data->pets.integers==NULL) {
        free( data );
        gerrorhand( 300, errginqdefval, xgks_state.gks_err_file); /*c1147*/
	return( 300 );
    }
    for( i=0; i<1; i++)
        data->pets.integers[i] = i+1;
 
    data->value         = 0.5;
    data->e_area.xmin    = 200.0;
    data->e_area.xmax    = 800.0;
    data->e_area.ymin    = 50.0;
    data->e_area.ymax    = 100.0;
    data->record.pet1.high  = 1.0;
    data->record.pet1.low   = 0.0;
 
    return(OK);
}
 
#define RULE        idev->data.val.SlidRule
#define BAR_WIDTH   idev->data.val.BarWidth
#define BAR_HEIGHT  idev->data.val.BarHeight
#define XRANGE(pt)   ( ((pt)>RULE[1].x) ? (RULE[1].x) :  (((pt)<RULE[0].x) ? (RULE[0].x) : (pt)) )
#define YRANGE(pt)   ( ((pt)>RULE[1].y) ? (RULE[1].y) :  (((pt)<RULE[0].y) ? (RULE[0].y) : (pt)) )
#define VALID_COR   ((idev->data.val.axis==VAL_VERT) ? (YRANGE(newdcpt->y)) : XRANGE(newdcpt->x))
#define BUILD_BAR_RECT(dummy, point)    { \
    Gfloat xpt, ypt; \
    if (idev->data.val.axis == VAL_VERT) { \
        xpt=RULE[0].x; \
        ypt=(point); \
    } else { \
        xpt=(point); \
        ypt=RULE[0].y; \
    } \
    dcmin.x = xpt - BAR_WIDTH; \
           dcmin.y = ypt + BAR_HEIGHT; \
           DcToX( ws, &dcmin, &xptmin ); \
           (dummy).x = xptmin.x; \
           (dummy).y = xptmin.y; \
           dcmax.x = xpt + BAR_WIDTH; \
           dcmax.y = ypt - BAR_HEIGHT; \
           DcToX( ws, &dcmax, &xptmax ); \
           (dummy).width  = xptmax.x - (dummy).x; \
           (dummy).height = xptmax.y - (dummy).y; \
}
/*
 * XgksValUpdatePrompt - update the locator prompt
 */
XgksValUpdatePrompt( ws, idev, pstate, newdcpt, xmev, event_id ) /* PTR c1133 */
    WS_STATE_ENTRY *ws;
    INPUT_DEV *idev;
    PromptStatus pstate;
    Gpoint *newdcpt;
    XMotionEvent *xmev;
    int event_id;      						 /* PTR c1133 */
{
    Gfloat  *data, CurPos;
    Gpoint dcpt, dcmin, dcmax;
    XRectangle e_rect, old_rect, new_rect;
    XPoint    xpt, xptmin, xptmax;
 
/* set up echo area */
        dcpt.x = idev->data.val.initst.e_area.xmin;
        dcpt.y = idev->data.val.initst.e_area.ymax;
        DcToX( ws, &dcpt, &xpt );
        e_rect.x = xpt.x;
        e_rect.y = xpt.y;
 
        dcpt.x = idev->data.val.initst.e_area.xmax;
        dcpt.y = idev->data.val.initst.e_area.ymin;
        DcToX( ws, &dcpt, &xpt );
        e_rect.width = xpt.x - e_rect.x;
        e_rect.height = xpt.y - e_rect.y;
 
    CurPos = VALID_COR;
        if(idev->data.val.initst.esw==GECHO){
    switch (pstate) {
    case PROMPTON :
        switch (idev->data.val.initst.pet) {
        case 1:
	case 2:                                               /* c1015 */ 
            /* Set up the echo area first */
            XSetForeground (ws->dpy, idev->gc, VAL_FOREGROUND);
                        XFillRectangle( ws->dpy, ws->win, idev->gc, e_rect.x, e_rect.y, e_rect.width, e_rect.height);
        
            /* Set up Sliding Rule and Bar parameters */
            XSetForeground (ws->dpy, idev->gc, VAL_BACKGROUND);
            BUILD_BAR_RECT(new_rect, idev->data.val.CurPos);
            DcToX ( ws, &(RULE[0]), &xptmin );
            DcToX ( ws, &(RULE[1]), &xptmax );
 
            /* Draw the Sliding Rule */
              XDrawLine( ws->dpy, ws->win, idev->gc, xptmin.x, xptmin.y, xptmax.x, xptmax.y );
 
            /* Draw the Sliding Bar */
                        XFillRectangle( ws->dpy, ws->win, idev->gc, 
                    new_rect.x, new_rect.y, new_rect.width, new_rect.height);
            break;
        default: break;
         }
        break;
    case PROMPTOFF:
        switch (idev->data.val.initst.pet) {
        case 1:    /* Wipe out the echoed prompt with Background colour */
	case 2:                                               /* c1015 */
            XSetForeground (ws->dpy, idev->gc, VAL_BACKGROUND);
                        XFillRectangle( ws->dpy, ws->win, idev->gc, e_rect.x, e_rect.y, e_rect.width, e_rect.height);
            break;
        default: break;
        }
        break;
    case PROMPTMOVE:
        switch (idev->data.val.initst.pet) {
        case 1: /* Set up parameters for wiping out old Bar and drawing new one */
	case 2:                                               /* c1015 */
            XSetForeground (ws->dpy, idev->gc, VAL_FOREGROUND);
            BUILD_BAR_RECT(old_rect, idev->data.val.CurPos);
            BUILD_BAR_RECT(new_rect, CurPos); 
            DcToX ( ws, &(RULE[0]), &xptmin );
            DcToX ( ws, &(RULE[1]), &xptmax );
            
            /* Fist wipe out the Slide Bar by setting it to ForeGround colour */
                        XFillRectangle( ws->dpy, ws->win, idev->gc, 
                    old_rect.x, old_rect.y, old_rect.width, old_rect.height);
 
            /* Now draw Rule line */
            XSetForeground (ws->dpy, idev->gc, VAL_BACKGROUND);
              XDrawLine( ws->dpy, ws->win, idev->gc, xptmin.x, xptmin.y, xptmax.x, xptmax.y );
 
            /* Draw the Bar at newdcpt location */
                        XFillRectangle( ws->dpy, ws->win, idev->gc, 
                    new_rect.x, new_rect.y, new_rect.width, new_rect.height);
            break;
        default: break;
        }
        break;
    default: break;
    }
        }
    if (pstate == PROMPTMOVE) {    
        idev->data.val.CurPos = CurPos;
        switch (idev->data.val.initst.mode) {
            case GREQUEST:
                  if (xmev == NULL || xmev->type != ButtonRelease)  /* c1177 */
                    break;
                  idev->touched = TRUE;
                  idev->data.val.val.val = DcToVal (CurPos, (idev->data.val.convert));
                  idev->data.val.val.status = GOK;
                  break;
            case GSAMPLE : 
                  idev->data.val.val.val = DcToVal (CurPos, (idev->data.val.convert)); 
                  idev->data.val.val.status = GOK;
                  break;
            case GEVENT  :
                      if (xmev == NULL ||xmev->type != ButtonRelease)  /* c1177 */
                    break;
                  data = (Gfloat *) malloc( sizeof (Gfloat) );
                if (data == NULL)
		{
                    gerrorhand( 300, errXgksValUpdatePrompt, xgks_state.gks_err_file ); /*c1147*/
		    return (300);
  		}
                else {
                    *data = DcToVal (CurPos, (idev->data.val.convert)); 
                    XgksEnqueueEvent(ws->ws_id, idev->dev, GVALUATOR, 
                          (char *)data, event_id); /*MIT*/ /* PTR c1133 */
                }
                break;
            default : break;
        }
 
    }
    XFlush(ws->dpy);
    return( OK );
}
 
#undef RULE
#undef XRANGE
#undef YRANGE
#undef BAR_WIDTH 
#undef BAR_HEIGHT 
#undef VALID_COR   
#undef BUILD_BAR_RECT
 
/*
 * XgksValDelete -- Free Everything in the val-device structure ...
 *             BUT, BUT not the pointer to the structure itself
 *            calling program still needs that
 */
XgksValDelete (ws, idev)
    WS_STATE_ENTRY *ws;
    INPUT_DEV *idev;
{
    XFreeGC( ws->dpy, idev->gc );
}
/*
 * XgksSetValEchoAttr -- Set Valuator Echo attributes based on the values in 
 *             ptr->initst (e_area, init, pet, valrec);
 *             Results will be passed back in ptr->SlidRule, 
 *                            ptr->BarSize
 *                            ptr->SlidBar
 */
static								/* c1147 */
void								/* c1147 */
XgksSetValEchoAttr (ValPtr)
    WSVALUATOR *ValPtr;
{
    Glimit *e= &(ValPtr->initst.e_area);
    Gfloat Low, High;
    
    switch (ValPtr->initst.pet) {
        case 1: Low  = ValPtr->initst.record.pet1.low; 
            High = ValPtr->initst.record.pet1.high;
            break;
        case 2: Low  = ValPtr->initst.record.pet2.low; 
            High = ValPtr->initst.record.pet2.high;
            break;
        case 3: Low  = ValPtr->initst.record.pet3.low; 
            High = ValPtr->initst.record.pet3.high;
            break;
	default: Low = 0.0;			/* New 4.3 C compiler */
	    High = 1.0;				/* New 4.3 C compiler */
	    break;				/* New 4.3 C compiler */
    }
    if ( (e->xmax-e->xmin) >= (e->ymax-e->ymin) ) {
        ValPtr->axis = VAL_HORIZ;
    /* As the echo area is horizontal, y-values of SlidRule are the same */
        ValPtr->SlidRule[0].y = ValPtr->SlidRule[1].y = (e->ymax+e->ymin) * 0.5;
        ValPtr->BarHeight = (e->ymax-e->ymin) * 0.4;    /*  A little bit smaller than e_area */
        ValPtr->BarWidth = (e->xmax-e->xmin)*0.05;    /* 5% the echo length */
    /* Construct x-coordinate of SlidRule s.t. the min/max position 
         of sliding bar can never go out of echo area */
        ValPtr->SlidRule[0].x = e->xmin + 0.6*ValPtr->BarWidth;
        ValPtr->SlidRule[1].x = e->xmax - 0.6*ValPtr->BarWidth;
        ValPtr->BarWidth = ValPtr->BarWidth * 0.5;    /* Remember this is only offset from centre */
        ValPtr->convert[0] = (High - Low) / (ValPtr->SlidRule[1].x - ValPtr->SlidRule[0].x);
        ValPtr->convert[1] = (ValPtr->SlidRule[0].x * ValPtr->convert[0]) - Low;
        ValPtr->CurPos = ((ValPtr->initst.val-Low) / ValPtr->convert[0]) + ValPtr->SlidRule[0].x;
    } else {
        ValPtr->axis = VAL_VERT;
    /* As the echo area is horizontal, x-values of SlidRule are the same */
        ValPtr->SlidRule[0].x = ValPtr->SlidRule[1].x = (e->xmax+e->xmin) * 0.5;
        ValPtr->BarWidth  = (e->xmax-e->xmin) * 0.4;    /*  A little bit smaller than e_area */
        ValPtr->BarHeight = (e->ymax-e->ymin)*0.05;        /* 5% the echo length */
    /* Construct y-coordinate of SlidRule s.t. the min/max position 
         of sliding bar can never go out of echo area */
        ValPtr->SlidRule[0].y = e->ymin + 0.6*ValPtr->BarHeight;
        ValPtr->SlidRule[1].y = e->ymax - 0.6*ValPtr->BarHeight;
        ValPtr->BarHeight = ValPtr->BarHeight * 0.5;    /* Again this is offset from centre */
        ValPtr->convert[0] = (High - Low) / (ValPtr->SlidRule[1].y - ValPtr->SlidRule[0].y);
        ValPtr->convert[1] = (ValPtr->SlidRule[0].y * ValPtr->convert[0]) - Low;
        ValPtr->CurPos = ((ValPtr->initst.val-Low) / ValPtr->convert[0]) + ValPtr->SlidRule[0].y;
    } 
    ValPtr->initst.val = ValPtr->CurPos; /* Save initial value in DC */
}
