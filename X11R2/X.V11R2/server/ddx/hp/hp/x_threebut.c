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
 * file: x-threebut.c
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
 *			Converts a two button mouse to a soft-three
 *			button mouse. 
 *		Hewlett Packard -- Corvallis Workstation Operation
 *		Project -- port of X to HP9000S300
 *		Sankar L. Chakrabarti -- MTS.
 */

# include <stdio.h>
# include "HPDevices.h"
# include <fcntl.h>
# include <sys/types.h>
# include "hildef.h"
/*
 * For V11 only
 */

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

# define NO_OPKEY	0x0
# define DOWN		0
# define LOOKAHEAD_SEC	0		/* look ahead delay in seconds */
# define LOOKAHEAD_USEC 100000		/* microsecond	*/
# define REPORTABLE	1		/* characterizes transition type */
# define NOT_REPORTABLE  0

/*
**  Mouse state defines:
**	Lup : Left button is up; Ldn : Left button is down.
**	R : Right button; M: Middle button :
**	Mhalf:	Middle button is on the process of going half.
**
*/

# define Lup_Rup_Mup	0
# define Lup_Rdn_Mup	1
# define Ldn_Rup_Mup	2
# define Lup_Rup_Mdn	3		/* physically Ldn_Rdn	*/
# define Lup_Rdn_Mhalf	4
# define Ldn_Rup_Mhalf	5
# define Ldn_Rdn_Mup	6
# define Ldn_Rdn_Mdn	7		/* Physically impossible, transient*/
# define Ldn_Rup_Mdn	ILLEGAL
# define Lup_Rdn_Mdn	ILLEGAL


/*
**	Stimulii : To change the mouse state.
**
*/

# define  Ldn		0
# define  Lup		1
# define  Rdn		2
# define  Rup		3
# define  Mdn		4
# define  Mup		5
# define  Mhalf		7	/* Middle button halfway up. 	*/



/*
**	Globals: 
*/
int	mouse_state = Lup_Rup_Mup;

int	s_mask = 0,
	mouse_ds,		/* file descriptor of two button mouse. */
	s_max;

struct	timeval	  wait_time =
	{ LOOKAHEAD_SEC,
	  LOOKAHEAD_USEC} ;


struct	mouse_result
	{  short	keycode;	/* 0,1,2, 3 */
	   short	keystate;	/* UP or DOWN	*/
	   short	transition_type; /* reportable to X server? */
	}  mouse_key,
	   look_ahead;

struct	mouse_two_button
	{  struct	mouse_result  button_state;
	   short	locx,
			locy;
	   int		timestamp;
	}	two_button;




short	Xmouse_map[ ] =		/* maps HP mouse code X code	*/
	{  XMOUSE_LEFT,
	   XMOUSE_LEFT,
	   XMOUSE_RIGHT,
	   XMOUSE_RIGHT,
	   XMOUSE_MIDDLE,
	   XMOUSE_MIDDLE,	   
	   ILLEGAL,		/* illegal should not happen	*/
	   XMOUSE_MIDDLE_HALF
	};


struct	stat_button
	{	short	state;
		short	button;
		short   transition_type;
	};

/*
** State Table for the state machine.
**
*/

struct	stat_button stat_table [7] [4] = {
/**	 Ldn		     Lup	   Rdn		        Rup	 **/
/** Lup_Rup_Mup	**/
 {  
	/* Ldn	*/	{Ldn_Rup_Mup, Ldn, REPORTABLE },
	/* Lup	*/	{Lup_Rup_Mup, Lup, REPORTABLE},
	/* Rdn	*/	{Lup_Rdn_Mup, Rdn, REPORTABLE},
	/* Rup	*/	{Lup_Rup_Mup, Rup, REPORTABLE}  },

/** Lup_Rdn_Mup **/
 { 	/* Ldn	*/	{Ldn_Rdn_Mup, Ldn, REPORTABLE},
	/* Lup	*/	{Lup_Rdn_Mup, Rdn, /* Lup, */ REPORTABLE},
	/* Rdn	*/	{Lup_Rdn_Mup, Rdn, REPORTABLE},
	/* Rup	*/	{Lup_Rup_Mup, Rup, REPORTABLE} },

/** Ldn_Rup_Mup **/
 {	/* Ldn	*/	{Ldn_Rup_Mup, Ldn, REPORTABLE},
	/* Lup	*/	{Lup_Rup_Mup, Lup, REPORTABLE},
	/* Rdn	*/	{Ldn_Rdn_Mup, Rdn, REPORTABLE},
	/* Rup	*/	{Ldn_Rup_Mup, Ldn /* Rup */ , REPORTABLE} },
	
/** Lup_Rup_Mdn **/
 {	/* Ldn 	*/	{Ldn_Rup_Mdn, Ldn, REPORTABLE},
	/* Lup	*/	{Lup_Rdn_Mhalf, Mhalf, NOT_REPORTABLE},
	/* Rdn	*/	{Lup_Rdn_Mdn, Rdn, REPORTABLE},   
	/* Rup	*/	{Ldn_Rup_Mhalf, Mhalf, NOT_REPORTABLE} },

/** Lup_Rdn_Mhalf **/
 {	/* Ldn	*/	{Lup_Rup_Mdn, Mdn, NOT_REPORTABLE},
	/* Lup	*/	{Lup_Rdn_Mhalf, Lup, NOT_REPORTABLE},
	/* Rdn	*/	{Lup_Rdn_Mhalf, Rdn, NOT_REPORTABLE}, 
	/* Rup	*/	{Lup_Rup_Mup, Mup, REPORTABLE} },

/** Ldn_Rup_Mhalf **/
 {	/* Ldn	*/	{Ldn_Rup_Mhalf, Ldn, NOT_REPORTABLE},
	/* Lup	*/	{Lup_Rup_Mup, Mup, REPORTABLE},
	/* Rdn	*/	{Lup_Rup_Mdn, Mdn, NOT_REPORTABLE},
	/* Rup	*/	{Ldn_Rup_Mhalf, Rup, NOT_REPORTABLE} },

/** Ldn_Rdn_Mup	  **/
{	/* Ldn	*/	{Ldn_Rdn_Mup, Ldn, REPORTABLE},	/* Can it happen ?*/
	/* Lup	*/	{Lup_Rdn_Mup, Lup, REPORTABLE},	
	/* Rdn	*/	{Ldn_Rdn_Mup, Rdn, REPORTABLE},	/* Can it happen ?*/
	/* Rup	*/	{Ldn_Rup_Mup, Rup, REPORTABLE} }
};

u_char	next_mouse_key();	
/*
**	Externals;
*/
extern	int	pending_hildata,
		mouse_pending,
		mousex, mousey,
		buttonx, buttony;

extern  int	queue_events_free;
extern struct	x11EventQueue	*events_queue;
extern box		*motion_box;
extern xEvent	*allocate_event();	/* file x_hil.c  */

#define	VSE_KBTUP	ButtonRelease
#define	VSE_KBTDOWN	ButtonPress


X_mouse_three ( file_ds)
int	file_ds;
{
	mouse_ds = file_ds;
	s_max = mouse_ds + 1;
	s_mask = 1 << mouse_ds;
}


/*
**	next_mouse_key ( hil_info)
*/

u_char
next_mouse_key ( hil_info)
struct	dev_info	*hil_info;
{	u_char	this_key = NO_OPKEY;

	int	there_is_data =0,
		bytes_read;

	char	packet_size = 0,
		pollhdr;

	int	fstat, fstat1;

	there_is_data = read ( mouse_ds, &packet_size, 1);
	if ( there_is_data)
	{  bytes_read = read (mouse_ds, hil_info, (packet_size - 1));
	   pollhdr = hil_info->poll_hdr;
	   if ( ( pollhdr & SET1_KEY_MASK) &&
		( pollhdr & MOTION_MASK) )
		{ this_key =  hil_info->dev_data[2];
		  hil_info->poll_hdr &= ~(SET1_KEY_MASK);
		  pending_hildata++;
		  mousex = mouse_p->x;
		  mousey = mouse_p->y;

		}
	    else
	    { if (pollhdr & MOTION_MASK)
		{ pending_hildata++;
		  mousex = mouse_p->x;
		  mousey = mouse_p->y;
		}
		
	      else
	      { if ( pollhdr & SET1_KEY_MASK)
		   this_key =  hil_info->dev_data[0];
		 else
		  printf ("NExt Key error. \n");
	      }
	     }
	  }

	   return ( this_key);
}
			
/*
** next_mouse_state ( state, stimulus)
**	:: return next state;
**	Aleters the globlas: mouse_state, keystate;
*/
int
next_mouse_state (state, stimulus, mouse_key)
int	state;
u_char	stimulus;
struct	mouse_result	*mouse_key;
{	int	keycode;
	struct	stat_button	*new_state;


	keycode =  ( (int) stimulus) - MOUSE_BASE_CODE;
	new_state = &(stat_table [state] [keycode]);
	if ( (new_state->button < 0) |
		(new_state->button > 7) )
	{   printf ("ILLEGAL New state Button %d. \n",
			new_state->button);
	    printf ( "Input State %d ; Input Keycode %d, Stimulus %x\n",
			state, keycode);
	    printf ("Current State : State %d,  button %d \n",
			new_state->state, new_state->button);
	    getchar();
	}
	mouse_key->keycode = Xmouse_map [new_state->button];
	if ((mouse_key->keycode < 0) |
		(mouse_key->keycode > 3))
	  printf ("ILLEGAL MOUSE CODE %d BEING SENT \n", 
			mouse_key->keycode);

	if ((new_state->button) & UP_MASK)
			mouse_key->keystate = ButtonRelease;
	  else
			mouse_key->keystate = ButtonPress;
	if ( new_state->button == Mdn)
			mouse_key->keystate = ButtonPress;
	if ( new_state->button == Mup)
			mouse_key->keystate = ButtonRelease;
	
	mouse_key->transition_type = new_state->transition_type;
	if (new_state->state < 0)
	 { printf ("Error: Mouse entered illegal state. \n");
	   printf (" Entering state =%d, keycode =%d\n", state, keycode);
	 }
	return (new_state->state);
}

/*
** parse_button_three ()	
**	 :: compute current state of the mouse, save it;
**	Look ahead for the next state; if the two states are to be merged
**	to generate a new state (like middle button) then do so and report
**	the merged state.
**	Else report both the current state and look_ahead state;
*/

int
parse_button_three ( hil_info, keycode)
struct	dev_info   *hil_info;
u_char		keycode;
{	u_char	next_key = NO_OPKEY;
	int     num_read = 0;
	int	curstate;
	int	mask;


	curstate = next_mouse_state ( mouse_state,keycode, &(mouse_key));
	/*
	 * fill up the two_button mouse structure.
	 */
	two_button.button_state = mouse_key;
	two_button.locx	=  mouse_p->x;	
	two_button.locy =  mouse_p->y; 
	two_button.timestamp = hilEventTime (hil_info->timestamp);
	if  ( ( (curstate == Lup_Rdn_Mup) || (curstate == Ldn_Rup_Mup))
		&& ( mouse_state == Lup_Rup_Mup))
	{	/* look ahead for the next key */
		mouse_state = curstate;
		mask = s_mask;

	  	num_read = select (s_max, &(mask),
				(int *) NULL, (int *) NULL, 
						&(wait_time));
	        if (num_read > 0)
		{ next_key = next_mouse_key ( hil_info );
	  	  if ( next_key > NO_OPKEY)
		  { curstate = next_mouse_state ( curstate, next_key,
						&look_ahead);
		    if (curstate == Ldn_Rdn_Mup)
		     { /* map it to nonexistent third button.	*/
		       mouse_state = Lup_Rup_Mdn;
		       mouse_key.keycode = XMOUSE_MIDDLE;
		       mouse_key.keystate= ButtonPress;
		       mouse_key.keystate= ButtonPress;
		       mouse_key.transition_type = REPORTABLE;
		       two_button.button_state = mouse_key;
		       two_button.timestamp = hilEventTime( hil_info->timestamp);
		     }
		    else
		     { mouse_state = curstate;
		     /*
		      * report the saved event.
		      */
		     put_button_event();
		     mouse_key = look_ahead;
		     two_button.button_state = mouse_key;
		     two_button.timestamp = hilEventTime(hil_info->timestamp);
		    }
		 }
	       }
	}
	else
	{
	    mouse_state = curstate;
	}
	if (queue_events_free)
	    put_button_event ();
	else {
	    mouse_pending++;
	    buttonx = mouse_p->x;
	    buttony = mouse_p->y;
	}
	return (mouse_state);
}


/*
** put_button_event ()
*	put the mouse button event on the event queue.
*	The caller must be sure that there is place on te queue;
*	This routine does not check. It merely grabs an event and writes
*	on it. It increments tail however.
*/

put_button_event()
{	xEvent	*event;
	u_char	keycode;
	char	keystate;
	int	dev_type;

	if ( two_button.button_state.transition_type == REPORTABLE)
	{ keycode = two_button.button_state.keycode;
	  keystate = two_button.button_state.keystate;
	  dev_type = DEVICE_MOUSE;

	    put_keyevent (two_button.button_state.keycode,
			  two_button.button_state.keystate,
			  (u_short) two_button.timestamp,
			  DEVICE_MOUSE, 
			  two_button.locx, two_button.locy);
	}
}

