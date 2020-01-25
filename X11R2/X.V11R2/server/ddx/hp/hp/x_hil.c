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
/* $Header: x_hil.c,v 1.4 88/02/26 13:19:48 rws Exp $ */

/***********************************************************************
 * file: x-hil.c
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
 *
 *		Hewlett Packard -- Corvallis Workstation Operation
 *		Project -- port of X (Version 11) to HP9000S300
 *		Sankar L. Chakrabarti -- MTS.
 *		Jack Palevich -- MTS.
 *		Don Bennett - MTS.  
 *
 * Routines accessed from outside of this file:
 *      InitInput
 *      ProcessInputEvents
 *      LegalModifier
 *      hilEventTime
 *      move_mouse
 *      put_keyevent
 *      TimeSinceLastInputEvent
 *      store_inputs
 */

#include <stdio.h>
#include <sys/hilioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <ndir.h>

#include "hildef.h"
#include "../cfb/cfb.h"


/*
 * Specially for X/V11.
 */

#define NEED_EVENTS	/* So that xEvent is defined */


#include    "X.h"
#include    "Xmd.h"
#include    "Xproto.h"
#include    "keysym.h"
#include    "scrnintstr.h"
#include    "screenint.h"
#include    "inputstr.h"
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
#include    "sun.h"
#include    "HPDevices.h"

#include "x11_hildef.h"

/*
 * End of X/V11
 */

# define QUEUE_FULL 	-1
# define QUEUE_EMPTY 	0
# define MAX_EVENTS	64  /* recommended size of queue  is 64*/
# define WR_EVENTS      (MAX_EVENTS - 1)


# define MOUSE_2_BUTTON	0x68
# define MOUSE		DEVICE_MOUSE
# define TABLET		DEVICE_TABLET
# define US_KBD		DEVICE_KEYBOARD

# define UP_LEFT_ARROW		0xf8	/* HIL key codes for arrow keys. */
# define DOWN_LEFT_ARROW	0xf9
# define UP_DOWN_ARROW		0xfa
# define DOWN_DOWN_ARROW	0xfb
# define UP_UP_ARROW		0xfc
# define DOWN_UP_ARROW		0xfd
# define UP_RIGHT_ARROW		0xfe
# define DOWN_RIGHT_ARROW	0xff
# define REPEAT_ARROW		0x2

# define OFFSET128_BIT		0x80  /* shift keycodes out of 0..7 */


# define MOUSE_KEY_UP		1
# define MOUSE_KEY_DOWN		0

# define MAX_HIL_DEVS		7
# define DEF_ACCELERATION	1			

# define TIME_POLL_BYTES	5	/* bytes indicating time and poll
					   information hil data packet. */
# define MOUSE_AXES		2	/* bytes indicating x,y of mouse */

static PtrPrivRec 	sysMousePriv;
PtrPrivRec		*mouse_p;
  
extern	int	errno;
extern  short   Xmouse_map[] ; 		/* see x_threebut.c	   */
extern  char    *display;		/* display number as a string */
extern CARD8    *hpMapRec[];
extern KeySymsRec      hpKeySyms[];
extern int screenIsSaved;

extern CARD16 keyModifiersList[];

#ifdef MULTI_X_HACK
/*  extern flag for the multi-x stuff;  */
extern int XMulti;

#endif MULTI_X_HACK

/*	 The event queue	 */

xEvent events_array[MAX_EVENTS];	/* input event buffer	*/

int		lastEventTime;
DevicePtr	hpPointer;   /* Current Pointer */
static DevicePtr	hpKeyboard;  /* Current Keyboard */
int		ZaphodMode = 1;


struct	inputs_selected
	{ int	num_readable;
	  int	input_mask;
	  int	bit_num;
	} sel_inputs, 
	  valid_inputs,
	 *selected_inputs,
	 *select_for_inputs();

struct	dev_info	hil_info,	/* holds hil_data during process. */
			saved_hilinfo;   

static char hilControlName[] = "/dev/rhil";
static int hilControlFD;

#define DEFAULT_DEVICE_COUNT	7
static char  *dev_names[] =
	{ "/dev/hil1",
	  "/dev/hil2",
	  "/dev/hil3",
	  "/dev/hil4",
	  "/dev/hil5",
	  "/dev/hil6",
	  "/dev/hil7"};

char	hil_files[MAX_HIL_DEVS][MAXNAMLEN+1];

/*
 *	Global variables for this file.
 *
 */

unsigned char	xhp_kbdid;
int	hil_devices;		/* number of hil_devices. */
int	queue_events_free = MAX_EVENTS;
int	pending_hildata = FALSE,
	mouse_pending = FALSE,
	tablet_pending = FALSE,
	saved_devdata  = FALSE,
	pending_index,
	pending_bytes;

int	acceleration = 0;	/* to control mouse motion	*/
int	threshold    = 0;
static int	last_arrow;	/* code of arrow key pressed last */

int left_mouse_key   = MOUSE_KEY_UP, /* state of mouse keys on the keyboard.*/
    right_mouse_key  = MOUSE_KEY_UP, /* they should be up or down	    */
    middle_mouse_key = MOUSE_KEY_UP; /* used in swallowing autorepeat	    */

struct  x11EventQueue	ev_queue,	/* structure to events_queue	*/
		*events_queue;		/* pointer to manipulate the above. */

box		*motion_box;		/* current mtion_box	  */

xEvent	*allocate_event();

HPInputDevice	input_files[MAX_HIL_DEVS];

unsigned   int	hilEventTime();
unsigned int hilGetTimeInMillis();
DevicePtr hpAddInputDevice();

int	max_events;			/* for experiments	*/

int	mousex, mousey,
	buttonx, buttony;

int	bytes_read;	/* debugging	*/

#define BELL_VOICE 1
#define CLICK_VOICE 2

static int keyboard_click;
static int bell_volume;
static int bell_pitch;
static int bell_duration;

/**************************/
/** Event queue routines **/
/**************************/

/*
 * init_events_queue (queue)
 *	called by XHPinit.c.
 */
static init_events_queue(queue)
struct  x11EventQueue	*queue;		
{
    queue->events = events_array;	
    queue->head = 0;
    queue->tail = 0;
    queue->size = MAX_EVENTS;
    events_queue = queue;
    max_events = WR_EVENTS;
}

/*
 * increment_tail (events_queue)
 *	Increase the tail pointer.
 *	 Number of slots available for writing;
 *	decrement the number of writable slots i.e. decrease queue_events_free.
 *	if possible. Also update the queue_events_free of the queue;
 */
static int increment_tail(events_queue)
struct x11EventQueue	*events_queue;
{
    if (events_queue->tail == WR_EVENTS)
	events_queue->tail = 0;
    else
	(events_queue->tail)++;

    queue_events_free--;
}

/*
 * allocate_event ()
 *	allocates the next available event to the caller and increments
 *	the tail pointer of the events queue; sets queue_events_free as needed.
 */
static xEvent  *allocate_event ()
{
    xEvent		*event;

    event = &( (events_queue->events)[events_queue->tail]);

    if ( events_queue->tail == WR_EVENTS)
	events_queue->tail = 0;
    else  (events_queue->tail)++;

    queue_events_free--;
    return (event);
}

/*********************************/
/** end of event queue routines **/
/*********************************/

/*
 * init_indevices ( )
 *  Initialize the input_files structure.
 *	Opens HIL devices in the non-blocking mode. Produces
 *	The input_file structure.
 *	Called from InitInput();
 */
static int init_indevices ()
{
    FILE	*fp;
    char	fname[MAXNAMLEN];
    int	fd, max_fd,
        fstat,
        dev_num,
        mask,
    
	i = 0,		/* index in HPInputDevices	*/
	j = 0;		/* index to device name array	*/

    HPInputDevice *ip;

    sprintf(fname, "/usr/lib/X11/X%sdevices",display);
    fp = fopen ( fname, "r");

    if ( fp) {
	dev_num = device_files (fp, dev_names);
	fclose (fp);
    }
    else dev_num = DEFAULT_DEVICE_COUNT;

#ifdef MULTI_X_HACK
    if (XMulti) {
	ipc_set_display_num(display);
	ipc_init();

	mask = 0;
	max_fd = 0;
	for (j=0; j<dev_num; j++) {
	    fd = ipc_try_to_open_hil ( dev_names[j], O_RDONLY | O_NDELAY);
	    if (fd > 0)
		mask |= (1 << fd);
	    if (fd > max_fd)
		max_fd = fd;
	}

	valid_inputs.input_mask = mask;
	valid_inputs.bit_num    = (max_fd + 1);
    }
    else {
#endif MULTI_X_HACK
	for(j=0; j<dev_num; j++) {
	    fd = open ( dev_names[j], O_RDONLY);
	    if (fd >= 0) {
		fstat = fcntl (fd, F_GETFL,fstat);
		if ( fstat >= 0) {
		    fstat |= O_NDELAY;
		    fstat = fcntl ( fd, F_SETFL, fstat);
		}
		if ( fstat < 0) 
		    printf (" Error in opening %s\n", dev_names[i]);
		
		if ( get_device_details( fd, &(input_files[i])) ) {
		    i++;
		    hil_devices = i;
		}
		else close (fd);
	    }
	}

	/** assert i==number of input devices **/
	mask = 0;
	for (j=0; j<i; j++)
	    mask |= (1 << (fd = input_files[j].file_ds));

	valid_inputs.input_mask = mask;
	valid_inputs.bit_num    = (fd + 1);
#ifdef MULTI_X_HACK	
    }
#endif MULTI_X_HACK	

    sel_inputs.bit_num = valid_inputs.bit_num;

/*
 * initialize the events queue
 */
    init_events_queue ( &ev_queue);
    init_cursor_motion(); 
    return (hil_devices) ;
}

static init_cursor_motion()
{
    mouse_p = &sysMousePriv;
}


/*
 * store_inputs ( ready_inputs)
 *	Assumption: when this routine is called, the events_queue must be
 *	 empty since the processing loop in the Receive () routine in
 *	 main.c breaks only when the head pointer and tail pointers are
 *	 equal.
 *	1. Flush any pending data;
 *	2. process data saved from previous store_inputs() call;
 *	3. Process data from the input devices till either no more
 *		data is obtained from devices or there is no more room in
 *		the events queue.
 */

store_inputs(ready_inputs)
long	ready_inputs;
{
    int     mask,
            checkmask,
            checkfd;

    if (screenIsSaved == SCREEN_SAVER_ON)
	SaveScreens(SCREEN_SAVER_OFF, ScreenSaverReset);

    sel_inputs.input_mask = ready_inputs  & valid_inputs.input_mask;
    mask = sel_inputs.input_mask;

    queue_events_free	= WR_EVENTS;

    flush_pending_data(&(hil_info));
    
    checkfd = sel_inputs.bit_num;
    checkmask = 1 << checkfd;
    while (mask && queue_events_free) {
	if (mask & checkmask) {
	    mask &= ~checkmask;
	    process_inputs(checkfd);
	}
	checkfd--;
	checkmask = checkmask >> 1;
    }
}

	
/*
 * process_inputs ( file_ds)
 *	Take care of the fact that HIL driver may send both motion and
 *	and button data within same data packet; for such a packet we
 *	have to generate two distinct events. This version contains this
 *	improvement.
 *	If processing stops because there is no more data from the device
 *	  then flush pending data if any.
 *	If input-processing has to stop because of queue being full,
 *	save the last data packet.
 */
static int process_inputs (file_ds)
int	file_ds;
{
    int	there_is_data = TRUE;
    int	i, dev_index;
    char	packet_size;
    u_char	file_type;
    HPInputDevice *indevice;
	
    for (i=0; i<hil_devices; i++)
	if (file_ds == input_files[i].file_ds) {
	    indevice = &(input_files[i]);
	    break;
	}
	
    /** if we didn't find an indevice, we may be in big trouble! **/

    while ((there_is_data > 0) && queue_events_free) {
	there_is_data = read (file_ds, &packet_size, 1);
	if (there_is_data) {
	    hil_info.hil_dev = indevice;
	    bytes_read = read (file_ds, &hil_info, packet_size-1);
	    if (bytes_read != (packet_size - 1))
		printf ("Mismatch in HILdevice reading. \n");
	    else {
		pending_index = 0;
		pending_bytes = bytes_read - TIME_POLL_BYTES;
		process_hil_data ( &(hil_info));
	    }
	}

	if (queue_events_free)
	    flush_pending_data ( &(hil_info));
    }

    if (queue_events_free)
	flush_pending_data ( &(hil_info));
}

/*
 * process_hil_data ( hil_info)
 *	process the data in the hilinfo structure and generate events
 *	as needed.
 */

static process_hil_data ( hil_info)
struct	dev_info	*hil_info;
{
    u_char	file_type;

    file_type = (hil_info->hil_dev)->file_type;

    lastEventTime = GetTimeInMillis();        /* Used for ScreenSaver */

    switch (file_type) {
      case	MOUSE :
	  parse_mouse(hil_info);
	  break;
      case	US_KBD :
          parse_keycode ( hil_info->timestamp,
				(hil_info->dev_data)[0], DEVICE_KEYBOARD);
	  break;
      case	TABLET :
	  parse_tablet_data ( hil_info);
	  break;
      default	: 
	  printf ( "INPUT ERROR: UNSUPPORTED INPUT DEVICE.\n");
	  break;
      }
}

/*
 * parse_mouse ( hil_info )
 *
 *	We are capturing the motion event straight in the
 *	in the DEVICE structure, also we are calling to move the mouse
 *	right from here. The X-server will only use the motion event as
 *	an hint.
 *	Altered to accomodate genuine three button mouse.
 */

static parse_mouse ( hil_info)
struct dev_info	*hil_info;
{
    char	pollhdr;
    u_char  buttoncode;

    pollhdr = hil_info->poll_hdr;

    if (pollhdr & (SET1_KEY_MASK|MOTION_MASK)) {
	if ( (hil_info->poll_hdr & SET1_KEY_MASK) 
	    && (hil_info->poll_hdr & MOTION_MASK)) {
	    /* data packet has both motion and keycode information.	*/
	    hil_info->poll_hdr &= ~(MOTION_MASK);
	    process_motion (hil_info);
	    pending_index = pending_index + MOUSE_AXES;
	    if (queue_events_free)
		process_mouse_button ( hil_info);
	    else
		pending_hildata++;
	}
	else {
	    if ( (hil_info->poll_hdr & MOTION_MASK) &&
		!(hil_info->poll_hdr & SET1_KEY_MASK) ) {
		hil_info->poll_hdr &= ~(MOTION_MASK); 
		process_motion (hil_info);
	    }
	    else {
		if ( (hil_info->poll_hdr & SET1_KEY_MASK)
		    && !(hil_info->poll_hdr & MOTION_MASK))
		    process_mouse_button (hil_info);
	    }
	}
    }
    else
	printf ( "HIL: Unknown Mouse operation : pollhdr %x \n", pollhdr);
}

/* 
 * parse_keycode ( time_stamp, hil_info,  dev_type)
 *   : Parse the keycode information.
 */

static int parse_keycode ( time_stamp, byte, dev_type)
char	time_stamp[];
u_char	byte; 
int		dev_type;
{
    xEvent	*event;
    char	key_direction;
    u_char	*kptr;
	/*
	 * Check if cursor keys are repeating.
	 */
	
    switch ( byte)
    {
	case UP_LEFT_ARROW		:
	case	DOWN_LEFT_ARROW		:
	case	UP_RIGHT_ARROW		:
	case	DOWN_RIGHT_ARROW	:
	case	UP_DOWN_ARROW		:
	case	DOWN_DOWN_ARROW		:
	case	UP_UP_ARROW		:
	case	DOWN_UP_ARROW		:
	    last_arrow = byte;
	    break;

	case	REPEAT_ARROW		:
	    byte = last_arrow;
	    break;
	  case    016:	/** reset key **/
	    kptr = ((DeviceIntPtr)hpKeyboard)->down;
	    /** check to see if either shift and ctrl are down **/
	    /** remember to offset by minkeycode               **/
	    /** check for bits ((4 or 5) and 6) + 8;           **/

	    if ((kptr[1] & 0x30) && kptr[1] & 0x40)
		GiveUp();

	  default:
	    break;
    }

    if ( byte & UP_MASK) 
    {
	if ( dev_type == DEVICE_KEYBOARD)
	    key_direction = KeyRelease;
	else
	    key_direction = ButtonRelease;
    }
    else   /* key/ button going down */
    {
	if ( dev_type == DEVICE_KEYBOARD)
	    key_direction = KeyPress;
	else
	    key_direction = ButtonPress;
    }

    /* XXX This must be changed some day.
     * The first arg to put_keyevent is ultimately used to index into the
     * KeySym table. In X10 and early versions of X11 we used the raw HIL
     * keycode as an index.  This was awkward because the least signifigant
     * bit of the hil keycode is the up/down flag.  It is cleaner to shift
     * right by one here.
     *
     * Note that the mouse Button codes should eventually be treated the same
     * way as the keycodes.  This will require changes to x_threebut.c, too.
     */

    if ( dev_type == DEVICE_KEYBOARD ) {
	byte >>= 1;
	byte += MIN_KEYCODE; /* offset avoids mouse codes.
				See also hpKeyMap.c */
	if (keyModifiersList[byte] & LockMask) {
	    if (key_direction == KeyRelease)
		return;
	    if (((DeviceIntPtr)hpKeyboard)->down[byte>>3] & (1<<(byte & 7)))
		key_direction = KeyRelease;
	}
    }

    if ( dev_type == DEVICE_MOUSE ) {
	byte &= 0x7f;
	byte >>= 1;
    }

    put_keyevent (byte, key_direction, 
	hilEventTime(time_stamp), dev_type,
	mouse_p->x, mouse_p->y);
}

static int get_device_details(file_ds, input_dev)
int file_ds;
HPInputDevice *input_dev;
{
    int dev_status;
    u_char	describe[11],
                hbyte,
                *io_record;
    u_char   id;

    struct	hil_desc_record		*hd;
    int	hi_resol =0,
        lo_resol = 0;

    int	i;

    int support_it = FALSE,
        supported_device = TRUE;

    dev_status = ioctl (file_ds, HILID, &(describe[0]));

    if ( dev_status >= 0) {
	hd = &(input_dev->hil_header);
	hd->id  = describe[0];
	/*
	 * Following braced region should be in a sepearate routine.
	 */
	
	{
	    id = hd->id;

	    /* keyboard device		*/
	    if ( (id >= 0xA0) && (id <= 0xFF)) {
		input_dev->file_type = US_KBD;
		input_dev->x_type    = DEVICE_KEYBOARD;
		support_it = TRUE;
		xhp_kbdid = id - 0xA0;
	    }
	    /* Check for mouse or quadrature devices	*/
	    if ( (id >= 0x60) && (id <= 0x6B) ) {
		input_dev->file_type = MOUSE;
		input_dev->x_type    = DEVICE_MOUSE;
		support_it = TRUE;
	    }
	    /*
	     * Check for graphics tablet and digitizers.
	     */
	    if ((id >= 0x90) && (id <= 0x97)) {
		input_dev->file_type = TABLET;
		input_dev->x_type    = DEVICE_TABLET;
		support_it = TRUE;
	    }
	}

	if ( support_it) {
	    input_dev->file_ds = file_ds;
	    hbyte = describe[1];
	    hd->flags = describe[1];

	    hd->num_cords   = FALSE;
	    hd->position    = FALSE;
	    hd->bits_axes   = FALSE;
	    hd->io_type     = FALSE;
	    hd->ext_command = FALSE;
	    hd->security    = FALSE;

	    if (hbyte &  HILDRH_TWO_AXES) hd->num_cords = TRUE;
	    if (hbyte & HILDRH_ABS_POS)   hd->position  =  TRUE;
	    if (hbyte & HILDRH_16BIT_POS) hd->bits_axes = TRUE;
	    if (hbyte & HILDRH_IODB)      hd->io_type  = TRUE;
	    if (hbyte & HILDRH_EDES)      hd->ext_command  = TRUE; 
	    if (hbyte & HILDRH_RSC)       hd->security = TRUE ;	    

	    hd->ax_num = (hbyte & HILDRH_AXES);

	   /*
	    * if # of axes indicate it is a positional device
	    *	then gather resolution.	
	    */

	   if ( hd->ax_num) {
	       lo_resol =  describe[2];
	       hi_resol =  describe[3];
	       hd->resolution = (hi_resol << 8) + lo_resol;
	       /* If it is an absolute device, gather size */
	       if ( hd->position ) {
		   switch ( hd->ax_num) {
		     case 3:
		       hd->size_z = (int)describe[8]|((int)describe[9] << 8);
		     case 2:
		       hd->size_y = (int)describe[6]|((int)describe[7] << 8);
		     case 1:
		       hd->size_x = (int)describe[4]|((int)describe[5] << 8);
		     default:
		       break;
		   }
		   io_record = &(describe[4 + hd->ax_num * 2]);
	       }
	       else
		   io_record = &(describe[4]);
	   }
	   else {
	       io_record = &(describe[2]);
	       hd->resolution = 0;
	   }
	   
	   if ( hd->io_type) {
	      hd->iob = *io_record;
	      hd->button_count = *io_record & HILIOB_BUTTONS ;

	      /*
	       * initialize structures for mapping 2-button mouse
	       *  three button mouse.
	       */

	      if (hd->button_count == 2)  X_mouse_three (file_ds);

	  }
	}
    }
    return ( support_it);
}

/*
 * process_motion (hil_info)
 *	Process mouse motion: move the mouse , decide if the
 *	event should be added to the queue. If the event should
 *	be added to the queue return true else return false.
 *	alters the values contained in motion_box;
 */


static process_motion ( hil_info)
struct	dev_info  *hil_info;
{
    xEvent	*event;
    short	x, y;
    short	newx, newy;

    x = hil_info->dev_data[0];
    y = hil_info->dev_data[1];

    /* compute x,y taking care of desired threshold and acceleration
     */

    if ( acceleration > DEF_ACCELERATION) {
	if ( (x - threshold) > 0)
	    x = threshold + (x  - threshold)*acceleration;
	else
	    if ( (x +threshold) < 0)
		x = (x + threshold)*acceleration - threshold;

	if ( (y - threshold) > 0)
	    y = threshold + (y  - threshold)*acceleration;
	else
	    if ( (y +threshold) < 0)
		y = (y + threshold)*acceleration - threshold;
    }
	
    mouse_p->x = mouse_p->x + x;
    mouse_p->y = mouse_p->y - y;


    /*
     * Active Zaphod implementation:
     *    increment or decrement the current screen
     *    if the x is to the right or the left of
     *    the current screen.
     */

    if (ZaphodMode && screenInfo.numScreens > 1 &&
	(mouse_p->x > mouse_p->pScreen->width ||
	 mouse_p->x < 0)) {

        (*((cfbPrivScreenPtr)(mouse_p->pScreen)->devPrivate)->CursorOff)
		(mouse_p->pScreen); /* sunRemoveCursor();	/* !!! ??COFF() */
	if (mouse_p->x < 0) { 
	    if (mouse_p->pScreen->myNum != 0)
		(mouse_p->pScreen)--;
	    else
		mouse_p->pScreen = &screenInfo.screen[screenInfo.numScreens -1];
	    
	    mouse_p->x += mouse_p->pScreen->width;
	}
	else {
	    mouse_p->x -= mouse_p->pScreen->width;
	    
	    if (mouse_p->pScreen->myNum != screenInfo.numScreens -1)
		(mouse_p->pScreen)++;
	    else
		mouse_p->pScreen = &screenInfo.screen[0];
	}

	NewCurrentScreen(mouse_p->pScreen, mouse_p->x, mouse_p->y);
    }

	/* Clip the cursor to stay within the bound of screen. */
    if (!hpConstrainXY(&mouse_p->x, &mouse_p->y)) return;

    move_mouse(hilEventTime(hil_info->timestamp), DEVICE_MOUSE);	
}

/*
 * SetAutoRepeat (onoff)
 *  Enable or disable the auto repeat feature of the hil devices.
 *  The only place input_files is used as a global. Normally the caller
 *	should supply the input_files, but in our case this would mean
 *	changing the device-independednt part of the server. So we do
 *	this way ..
 *  This will set the auto repeat mode for all hil devices. If not
 *	so desired it should be modified for selected devices.
 */

static int SetAutoRepeat (onoff)
int onoff;
{
    int	j;
    char ioctl_data[12];
    int state = HILDKR;
    
    if (onoff)
	state = HILER1;

    for ( j= 0; j < hil_devices; j++)
	ioctl ((input_files[j]).file_ds, state, ioctl_data);

    return (onoff);
}


#ifdef UNDEFINED
static SetMouseCharacteristics(new_threshold, new_acceleration)
int new_acceleration, new_threshold;
{
    acceleration = new_acceleration;
    threshold    = new_threshold;
}
#endif UNDEFINED

/*
 * flush_pending_data ( )
 *	flush data gathered from reading cycle which could not be 
 *	delivered because events queue was full.
 */

static flush_pending_data ( hil_info)
struct	dev_info  *hil_info;
{
    int	x, y;

    x = mouse_p->x;
    y = mouse_p->y;
	
    if ( mouse_pending) {
	if ( mouse_pending > 1) printf ("MOUSE BUTTON PENDING ERROR\n");
	mouse_pending--;
	put_button_event ();
    }

    if (( tablet_pending) && queue_events_free) {
	tablet_pending--;
	put_tab_button ( hil_info);
    }

    if ( (pending_hildata > 0) && queue_events_free) {
	if (pending_hildata > 1)
	    printf (" ERROR IN HIL_DATA MANAGEMENT. HIlCOUNT %d\n",
		    pending_hildata);
	pending_hildata--;	
	parse_mouse (hil_info);
    }
}

/*
 *  move_mouse ()
 *
 * if the x,y locations are in the motion_box then
 *	update the cursor position;
 *	else : allocate an event, fill motion data,
 *	increment queue tail and trash the motion box.
 */

move_mouse(timestamp, dev_type)
unsigned int timestamp;
int dev_type;
{
    xEvent     xE;
    PtrPrivPtr pPriv;
    
    pPriv = (PtrPrivPtr) hpPointer->devicePrivate;

    (*((cfbPrivScreenPtr)(pPriv->pScreen)->devPrivate)->MoveMouse)
	(pPriv->pScreen, pPriv->x,pPriv->y,1);

    xE.u.u.type = MotionNotify;
    xE.u.keyButtonPointer.time = timestamp;
    xE.u.keyButtonPointer.rootX = mouse_p->x;
    xE.u.keyButtonPointer.rootY = mouse_p->y;

    (* hpPointer->processInputProc) (&xE, hpPointer);
}

/*
 * process_mouse_button ()
 *  process the button event of a two button mouse as well as other
 *	kinds of mouse (1, 3, or more button ).
 */
static process_mouse_button (hil_info)
struct	dev_info *hil_info;
{
    int			done = FALSE;
    u_char		button;
    int			button_bytes;
    u_char		button_num;
    HPInputDevice	*current_indevice;

    current_indevice = hil_info->hil_dev;
    button_num = (current_indevice->hil_header).button_count;
    if ( pending_index >= pending_bytes) done = TRUE;

    while (queue_events_free && !(done)) {
	button = hil_info->dev_data[pending_index];
	pending_index++;
	if ( button_num == 2 )  /* two button mouse, map to three */
	    parse_button_three (hil_info, button);
	else
	    if ( button_num > 0)
		parse_keycode ( hil_info->timestamp, button,
			       current_indevice->x_type);

	if (pending_index >= pending_bytes) done = TRUE;
    }

    if (!(done)) pending_hildata++;
    else
	hil_info->poll_hdr &= ~(SET1_KEY_MASK);
}

/*
 * put_keyevent ( )
 *	put the key code in the event queue.
 */
put_keyevent (keycode, keystate, time_stamp,dev_type, mousex, mousey)
u_char	keycode;
char	keystate;
unsigned  int time_stamp;
int	dev_type;
short	mousex, mousey;
{
    xEvent	*event;


    /*
     * In V11 mouse button codes are obtained indirectly.
     *	So this Kludge. In this way the keymap remains compatible
     *	with Version 10.
     */
    if ( dev_type == DEVICE_MOUSE || dev_type == DEVICE_TABLET)
	keycode++;

    event = allocate_event ();
    
    event->u.u.type = keystate;
    event->u.u.detail = keycode;
    event->u.keyButtonPointer.time = time_stamp;
    event->u.keyButtonPointer.rootX = mousex;
    event->u.keyButtonPointer.rootY = mousey;
}

/*
 * Return a timestamp in miliseconds
 */

unsigned int hilEventTime ( timeStamp)
u_char	*timeStamp;
{
    unsigned int	time, *pTime;

    /* Note that HIL returns time in 10-millisecond ticks */

    pTime = (unsigned int *) timeStamp;  
    time = *pTime;
    return ( 10 * time );
}


/*
 * device_files ()
 *	prepares the name of hil-devices to be grabbed by the server
 *	The names are in the file 'fp'.
 */

static int device_files (fd, device_names)
FILE	*fd;
char	*device_names[];
{
    int i, dev_count;
    char buf[MAXNAMLEN+1];

    dev_count = 0;
    while (fgets(buf,MAXNAMLEN+1,fd) != NULL) {
	if (buf[strlen(buf)-1] == '\n')
	    buf[strlen(buf)-1] = 0;
	strcpy(hil_files[dev_count],buf);
	dev_count++;
    }

    for ( i=0; i < dev_count; i++) {
	device_names[i] = &(hil_files[i][0]);
    }

    return (dev_count);
}

ProcessInputEvents()
{
    int	head, tail;
    DevicePtr	pDevice;
    xEvent	*event;

    while ( events_queue->head != events_queue->tail) {
        event =  &((events_queue->events)[(events_queue->head)]);

	switch (event->u.u.type) {
	  case KeyPress:
	    if (keyboard_click)
		beep(CLICK_VOICE,800,keyboard_click,1);
	  case KeyRelease:
	    ProcessKeyboardEvent(event, hpKeyboard);
	    break;
	  default:
	    ProcessPointerEvent(event, hpPointer);
	    break;
	}

	if (events_queue->head == max_events)
	    events_queue->head = 0;
	else
	    events_queue->head++;
    }
/*!!!     sunRestoreCursor();	/* Put the cursor back on the screen */
  {
    register cfbPrivScreenPtr cfb;
    DevicePtr  pDev, LookupPointerDevice();
    ScreenPtr  pScreen;
  
    pDev = LookupPointerDevice();
    pScreen = ((PtrPrivPtr)pDev->devicePrivate)->pScreen;
    cfb = (cfbPrivScreenPtr)(pScreen->devPrivate);
  
    (*cfb->MoveMouse)(pScreen,mouse_p->x,mouse_p->y,0);
  }
}

static void hpChangePointerControl(pDevice, ctrl)
DevicePtr pDevice;
PtrCtrl *ctrl;
{
    threshold = ctrl->threshold;
    acceleration = ctrl->num;
    if (acceleration <= 0)
	acceleration = 1;
}

/*
 * The members of the keybdCtrl structure have the following values:
 *
 * click:	0(off) - 100 (loud);	-1 => default;
 * bell:	0(off) - 100 (loud); 	-1 => default;
 * bell_pitch:  Pitch of the bell in Hz;-1 => default;
 * bell_duration: in miliseconds;	-1 => default;
 */

static void hpChangeKeyboardControl(pDevice, ctrl)
DevicePtr pDevice;
KeybdCtrl *ctrl;
{
    keyboard_click = (int)((double)(ctrl->click) * 15.0 / 100.0);
    bell_volume = (int)((double)(ctrl->bell) * 15.0 / 100.0);
    bell_pitch = (int)(1023.0 - 83333.0/((double)ctrl->bell_pitch));
    bell_duration = ctrl->bell_duration/10;
    if (!bell_duration)
	bell_duration = 1;
    SetAutoRepeat(ctrl->autoRepeat);
}

/*  range values on beep:
 *   VOICE    : from 1 to 3                                                   *
 *   PITCH    : from 0 to 1023 (incl)                                         *
 *              To get the pitch value from frequency,
 *		pitch = 1023 - 83333/f ;
 *   VOLUME   : from 0 to 15   (incl). Zero turns voice off.                  *
 *   DURATION : from 0 to 255  (incl). Zero turns voice on continuously.      *
 */

static void hpBell(loud, pDevice)
DevicePtr pDevice;
int loud;
{
    loud = (int)((double)loud * 15.0 / 100.0);
    beep(BELL_VOICE, bell_pitch, loud, bell_duration);
}

static int hpGetMotionEvents();

static Bool hpMouseProc(pDev, onoff)
DevicePtr pDev;
int onoff;
{
    PtrPrivPtr pPtrPriv = (PtrPrivPtr) pDev->devicePrivate;
    HPInputDevice *pHPDev = (HPInputDevice *) pPtrPriv->devPrivate;
    BYTE map[4];

    switch (onoff)
    {
	case DEVICE_INIT: 
	    hpPointer = pDev;

	    /* If there are more than one pointer, this code won't work! */

	    sysMousePriv.pScreen = &screenInfo.screen[0];
	    sysMousePriv.x = sysMousePriv.pScreen->width / 2;
	    sysMousePriv.y = sysMousePriv.pScreen->height / 2;

	    sysMousePriv.devPrivate = hpPointer->devicePrivate;
	    hpPointer->devicePrivate = (pointer) &sysMousePriv;
	    hpPointer->on = FALSE;

	    map[1] = 1;
	    map[2] = 2;
	    map[3] = 3;
	    InitPointerDeviceStruct(hpPointer, map, 3, hpGetMotionEvents,
		hpChangePointerControl);
 	    break;
	case DEVICE_ON: 
	    pDev->on = TRUE;
	    if ( pHPDev != NULL) {
		AddEnabledDevice( pHPDev->file_ds );
		if ( pHPDev->hil_header.position ) { /* 1 = Absolute */
		    pHPDev->screenSizeX = pPtrPriv->pScreen->width;
		    pHPDev->scaleX = ((float)pPtrPriv->pScreen->width) /
			((float)pHPDev->hil_header.size_x);
		    pHPDev->screenSizeY = pPtrPriv->pScreen->height;
		    pHPDev->scaleY = ((float)pPtrPriv->pScreen->height) /
			((float)pHPDev->hil_header.size_y);
		}
	    }
	    break;
	case DEVICE_OFF: 
	    pDev->on = FALSE;
	    if ( pHPDev != NULL)
		RemoveEnabledDevice( pHPDev->file_ds);
	    break;
	case DEVICE_CLOSE: 
	    break;
    }
    return(Success);

}

static Bool hpKeybdProc(pDev, onoff)
DevicePtr pDev;
int onoff;
{
    HPInputDevice *pHPDev = (HPInputDevice *) pDev->devicePrivate;

    switch (onoff)
	{
	  case DEVICE_INIT: 
	    {
		u_char keyId;
		hpKeyboard = pDev;

		/* see the manual on using hp-hil devices with hpux for the keyboard
		 * nationality codes; they are the low order 5 bits of the device id;
		 * 0x1f is United States, so we'll subtract from 0x1f to give the
		 * U.S. a keyId of zero;
		 */

		if (pHPDev != NULL)
		    keyId = 0x1f - (pHPDev->hil_header.id & 0x1f);
		else
		    keyId = 0;

		InitKeyboardDeviceStruct(hpKeyboard,
					 &(hpKeySyms[keyId]),
					 (hpMapRec[keyId]),
					 hpBell,
					 hpChangeKeyboardControl);
		break;
	    }
	  case DEVICE_ON: 
	    pDev->on = TRUE;
	    if (pHPDev != NULL)
		AddEnabledDevice(pHPDev->file_ds);
	    break;
	  case DEVICE_OFF: 
	    pDev->on = FALSE;
	    if (pHPDev != NULL)
		RemoveEnabledDevice(pHPDev->file_ds);
	    break;
	  case DEVICE_CLOSE: 
	    break;
	}
    return(Success);
}

static int hpGetMotionEvents(buff, start, stop)
CARD32 start, stop;
xTimecoord *buff;
{
    return 0;
}

#define MOTION_BUFFER_SIZE 0
static int hil_ready = 0;

/*-
 *-----------------------------------------------------------------------
 * InitInput --
 *	Initialize all supported input devices...what else is there
 *	besides pointer and keyboard?
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Two DeviceRec's are allocated and registered as the system pointer
 *	and keyboard devices.
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
InitInput(argc, argv)
int     	  argc;
char    	  **argv;
{
    DevicePtr pXDev;
    int	result = -1;
    int	i;
    HPInputDevice *pHPDev;
    char pointer_found = 0;	/* Not-zero when a pointer is found */
    char keyboard_found = 0;    
    static int zero = 0;

    /* First we initialize the Human Interface Loop */

    if (!(hil_ready)) {
	if ((hilControlFD = open(hilControlName, O_RDWR)) < 0) {
	    ErrorF("Couldn't open HIL control device.\n");
	    perror(hilControlName);
	}
	result = init_indevices ();
#ifdef MULTI_X_HACK
	if (XMulti)
	    hil_ready = 1;
	else {
#endif MULTI_X_HACK
	    if ( result > 0) hil_ready = 1;
	    ErrorF("%d hil input devices found.\n", result);
#ifdef MULTI_X_HACK
	}
#endif MULTI_X_HACK
    }

    /*
     * Now see what kind of input devices we have.
     */

    for (i=0, pHPDev=input_files; i<hil_devices; i++,pHPDev++) {
	switch ( pHPDev->x_type ) {
	  case DEVICE_MOUSE:
	  case DEVICE_TABLET:
	    pXDev = hpAddInputDevice(hpMouseProc, TRUE, pHPDev);
	    if ( !pointer_found ) {
		pointer_found = 1;
		RegisterPointerDevice(pXDev, MOTION_BUFFER_SIZE);
	    }
	    break;
	  case DEVICE_KEYBOARD:
	    pXDev = hpAddInputDevice(hpKeybdProc, TRUE, pHPDev);
	    if (!keyboard_found) {
		keyboard_found = 1;
		RegisterKeyboardDevice(pXDev);
	    }
	    break;
	}
    }

    if (!pointer_found) {
	ErrorF("No pointer found.\n");
	pXDev = hpAddInputDevice(hpMouseProc, TRUE, NULL);
	RegisterPointerDevice(pXDev, MOTION_BUFFER_SIZE);
    }
    if (!keyboard_found) { 
	ErrorF("No keyboard found.\n");
	pXDev = hpAddInputDevice(hpKeybdProc, TRUE, NULL);
	RegisterKeyboardDevice(pXDev);
    }

    SetInputCheck (&zero, &isItTimeToYield);
}

/*
 * TimeSinceLastInputEvent()
 * - aparently returns time in miliseconds since last input event
 *
 */

TimeSinceLastInputEvent()
{
    if (lastEventTime == 0)
	lastEventTime = GetTimeInMillis();
    return GetTimeInMillis() - lastEventTime;
}

/*
 * Get hil "real" time in milliseconds
 */

#define TICKSPERDAY (100 * 60 * 60 * 24)
#define MSPERTICK 10

static unsigned int hilGetTimeInMillis()
{
    struct eft5 eft;

    if ( ioctl(hilControlFD, EFTRRT, &eft) < 0 ) {
	ErrorF("hil: can't get current time.\n");
	return(0);
    }
    /* The "real time" structure is five bytes long.
     * By experimenation, the values are:
     * b[0] = msb days
     * b[1] = lsb days
     * b[2] = msb time of day
     * b[3] = middle byte of time of day
     * b[4] = lsb time of day
     * Time of day is in 10 ms ticks, so we must multiply by 10 to get
     * milliseconds.
     */
    return (  MSPERTICK *
	    ( (eft.b[4] | (eft.b[3] << 8) | (eft.b[2] << 16)) /* Time of day */
	     + TICKSPERDAY * ((eft.b[0] << 8) | eft.b[0] ))); /* Days */
}

/*
 * hpAddInputDevice(deviceProc, autoStart, pHPDev)
 * create an X input device, then assign pHPDev to it's devicePrivate field.
 */

static DevicePtr hpAddInputDevice(deviceProc, autoStart, pHPDev)
DeviceProc deviceProc;
Bool autoStart;
HPInputDevice *pHPDev;
{
    DevicePtr pXDev;

    pXDev = AddInputDevice(deviceProc, autoStart);
    pXDev->devicePrivate = (pointer) pHPDev;
    return  pXDev;
}

/* Can the keyboard hardware support this scan code as a legal modifier? */
LegalModifier(key)
BYTE key;
{
    /* By emperical observation, our HIL keyboards support 6-key
     * roll-over on any key.  This means that essentially any key
     * can be used as a modifier.
     */

    return TRUE;
}

#ifdef MULTI_X_HACK
fixup_device_details(fd)
int fd;
{
    int status;

    status = get_device_details(fd, &(input_files[hil_devices]));
    hil_devices++;

    if (!status)
        close_hil(fd);
    else {
        valid_inputs.input_mask |= 1 << fd;
        if (fd+1 > valid_inputs.bit_num) {
            valid_inputs.bit_num = fd + 1;
            sel_inputs.bit_num = fd + 1;
        }
    }        

    return(status);
}

remove_details_entry(fd)
int fd;
{
    int i;

    for (i=0; i<hil_devices; i++)
       if (fd == input_files[i].file_ds)
           break;

    /** (i == hil_devices)   => fd was not in table;
     ** (i == hil_devices-1) => don't worry; removed device is at end;
     ** (i < hil_devices-1)  => copy entry hil_devices-1 to i;
     **/

    valid_inputs.input_mask &= ~(1 << fd);
    if (fd+1 == valid_inputs.bit_num) {
        valid_inputs.bit_num--;
        sel_inputs.bit_num--;
    }

    hil_devices--;
    if (i < hil_devices)
        memcpy(&(input_files[i]),&(input_files[hil_devices]),
	       sizeof(HPInputDevice));
}
#endif MULTI_X_HACK
