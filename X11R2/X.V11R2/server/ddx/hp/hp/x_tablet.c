/*

Copyright (c) 1986, 1987 by Hewlett-Packard Company
Copyright (c) 1986, 1987 by the Massachusetts Institute of Technology

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD
TO THIS SOFWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE.  Hewlett-Packard shall not be liable for errors 
contained herein or direct, indirect, special, incidental or 
consequential damages in connection with the furnishing, 
performance, or use of this material.

This software is not subject to any license of the American
Telephone and Telegraph Company or of the Regents of the
University of California.

*/
/***********************************************************************
 * file: x-tablet.c
 *
 *
 *  ******************************************************************
 *  *  (c) Copyright Hewlett-Packard Company, 1984.  All rights are  *
 *  *  reserved.  Copying or other reproduction of this program      *
 *  *  except for archival purposes is prohibited without prior      *
 *  *  written consent of Hewlett-Packard Company.		     *
 *  ******************************************************************
 *
 *  Program purpose -- input driver for HIL devices for X-server.
 *		Puts graphics tablets/ digitizers in the X environment.
 *		Makes them behave like a mouse.
 *		Hewlett Packard -- Corvallis Workstation Operation
 *		Project -- port of X to HP9000S300
 *		Sankar L. Chakrabarti -- MTS.
 *	3/5/87:
 *	   Being Modified for incorporation in V11.
 *
 */

/********************************************************************
*	Problem: In version 10, there was only one display, so the
*	 mapping between the display and the tablet could be done
*	 only once at initialization. But in version 11, with multiple
*	 displays, the mapping has to be restablished whenever the
*	 pointer moves to a different display. This is a problem to be
*	 solved. It should be done as part of context shift whenever
*	 locator moves to a different display. The mi-lib should detect
*	 it and call init routines in this file. To support this, we may
*	 need to restructure the routine. Shall do it when, X supports 
*	 multiple screens.
*	    -- Sankar   3/10/87
***********************************************************************/


#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/hilioctl.h>
#include "hildef.h"
#include "HPDevices.h"
#include "Xmd.h"

#define NEED_EVENTS	/* So that xEvent is defined */
#include    "X.h"
#include    "Xproto.h"
#include    "scrnintstr.h"
#include    "screenint.h"
#include    "input.h"
#include    "cursorstr.h"
#include    "cursor.h"
#include    "pixmapstr.h"
#include    "pixmap.h"
#include    "gc.h"
#include    "gcstruct.h"
#include    "region.h"
#include    "colormap.h"
#include    "miscstruct.h"
#include    "dix.h"
#include    "mi.h"
#include "sun.h"

#include "x11_hildef.h"


unsigned int	hilEventTime();

/*
 * end of Version 11
 */

/*
 *  Global for tablet manipulation.
 */

int	tablet_active = FALSE,
	vmousex = -1,   /* tablet-x mapped to mouse positionX on display*/
	vmousey = -1;   /* tablet-y mapped to mouse positionY on display*/


u_char	dev_id,		/* id of the device			*/
	pending_tab_button; /* pending keycode of the tablet 	*/

/*
**	Externals
*/

extern	int	tablet_pending;
extern  int	queue_events_free;
#ifdef	V10
extern	vsEventQueue	*events_queue;
extern	vsBox		*motion_box;
extern  vsEvent		*allocate_event();
#endif	V10

extern	short   Xmouse_map[];	/*  see x_threebut.c		*/

extern struct	x11EventQueue	*events_queue;
extern box		*motion_box;
extern xEvent		*allocate_event();	/* file x_hil.c  */
#undef	VSE_TABLET
#undef	VSE_KBTUP
#undef	VSE_KBTDOWN
#define	VSE_TABLET	DEVICE_TABLET
#define	VSE_KBTUP	ButtonRelease
#define	VSE_KBTDOWN	ButtonPress
#define vsEvent	DDXEventRec

/*
 *	parse_tablet_data ()
 */
parse_tablet_data(hil_info)
struct	dev_info   *hil_info;
{
    int		axis_bytes;
    char	pollhdr;

    pollhdr = hil_info->poll_hdr;

    /** this should probably be precomputed or based on the device
     ** characteristics, but I'm not going to bother; D.B.
     **/
    axis_bytes = 4;
	
    if ((pollhdr & MOTION_MASK) && (pollhdr & SET1_KEY_MASK)) {
	pending_tab_button = hil_info->dev_data[axis_bytes];
	tab_but_motion (hil_info);
    }
    else if ( pollhdr & MOTION_MASK) {
	tab_motion (hil_info);
	hil_info->poll_hdr &= ~(MOTION_MASK);
    }
    else if ( pollhdr & SET1_KEY_MASK) {
	pending_tab_button = hil_info->dev_data[0];
	tab_button (hil_info);
	hil_info->poll_hdr &= ~(SET1_KEY_MASK);
    }
    else
	printf ("Unknown Type of Tablet  data \n");
}

/*
 *
 * tab_but_motion ()
 *	process a hil_info packet of button and motion data.
 *	Change the values of mouse_p;
 *
 */
tab_but_motion(hil_info)
struct dev_info	*hil_info;
{
    int	mx, my;
    int	j;
    char	*cp;
    u_char	prx;

    map_tablet_display(hil_info, &mx, &my);

    mouse_p->x = mx;
    mouse_p->y = my;

    move_mouse ( hilEventTime (hil_info->timestamp), VSE_TABLET);

    hil_info->poll_hdr &= (~MOTION_MASK);

    if ( !(tablet_active)) {
	/* check proximity */
	if ( pending_tab_button == PROXIMITY_IN) {
	    tablet_active = TRUE;
	    vmousex = mx;
	    vmousey = my;
	}
	else printf ("Error: Proximity in must be found. \n");
    }
    else {
	if (queue_events_free)
	    tab_button (hil_info);
	else	
	    tablet_pending++;
    }
}

/*
 *  map_tablet_display ()
 *    map the tablet coordinates to display pixels.
 */


map_tablet_display(hil_info, mousex, mousey)
struct	dev_info *hil_info;
int	*mousex, *mousey;
{
    unsigned  locx, locy;
    u_char	xlo, xhi, ylo, yhi;
    float tx, ty;
    HPInputDevice *cur_indevice;
    
    cur_indevice = hil_info->hil_dev;

    xlo = hil_info->dev_data[0];
    xhi = hil_info->dev_data[1];
    ylo = hil_info->dev_data[2];
    yhi = hil_info->dev_data[3];

    locx = xhi*256 + xlo;
    locy = yhi*256 + ylo;

    tx = locx * cur_indevice->scaleX;
    ty = locy * cur_indevice->scaleY;

    *mousex = tx;
    *mousey = cur_indevice->screenSizeY - ty;
    /* Note in display the Y-ordinate is reversed.	*/
}

/*
 * tab_motion ()
 *
 */
	
tab_motion (hil_info)
struct	dev_info  *hil_info;
{	int	mx, my;
	int	put_event = FALSE;

	map_tablet_display ( hil_info, &mx, &my);

	if ( mx != vmousex)
	   { vmousex = mx;
	     mouse_p->x = mx;		
	     put_event = TRUE;
	   }

	if ( my != vmousey)
	   { vmousey = my;
	     mouse_p->y = my;		
	     put_event = TRUE;
	   }
#ifdef	XMONITOR
	if ( (put_event) && (on_steal_input))
		steal_jump_data (mouse_p->x, mouse_p->y,  VSE_TABLET);
#endif	XMONITOR
	
	if (put_event) 
	    move_mouse (hil_info->timestamp, VSE_TABLET);

}
/*
**  tab_button ()
**
*/


tab_button (hil_info)
struct  dev_info   *hil_info;
{


	if ( pending_tab_button == PROXIMITY_OUT)
	  { tablet_active = FALSE;
	    vmousex = -1;
	    vmousey = -1;
	  }
	else
	   put_tab_button (hil_info);

}
		
/*
** put_tab_button ()
**	puts a tablet button to the event queue. The button is in the
**	variable pending_button;
**	It should be possible to merge this routine with processing of
**	pending button from the mouse_device.
**	But that for the later.
**	The caller must make sure that there is room in the events_queue.
*/

put_tab_button (hil_info)
struct	dev_info	*hil_info;
{	xEvent   *event;
	short	xmouse;

	u_char	keycode;
	char	keystate;

	/*
 	 * This cludge is to avoid crashing if some clown is using
	 *	a 4-7 button tablet. The cludge is to be removed when
	 *	X learns how to handle mouse codes above three.
	 *	Till then live with this ugliness ..
	 *	0x85 : HIL code for BUTTON3
	 */
#ifdef	V10
	if ( ( pending_tab_button >= MOUSE_BASE_CODE) &&
	       ( pending_tab_button <= 0x85))
#endif	V10
	{
	  xmouse =  Xmouse_map[ pending_tab_button - MOUSE_BASE_CODE];
	  keycode = (u_char) xmouse;
	  if ( pending_tab_button & UP_MASK)
			keystate = VSE_KBTUP;
		else	keystate = VSE_KBTDOWN;

#ifdef	XMONITOR
	  if (on_steal_input)
		steal_jump_data (keycode, keystate, VSE_TABLET);
#endif	XMONITOR
	  
	put_keyevent( keycode, keystate, 0, VSE_TABLET, mouse_p->x, mouse_p->y);
/*
 *	  event->time = (u_short) hilEventTime (hil_info->timestamp);
 */
	}
	
}
