#include <X/mit-copyright.h>

/* $Header: xclock.c,v 10.18 86/11/19 18:40:33 jg Rel $ */
/* Copyright 1985 Massachusetts Institute of Technology */

/*
 * xclock.c MIT Project Athena, X Window system clock.
 *
 *  This program provides the user with a small
 * window contining a digital clock with day and date.
 * Parameters are variable from the command line.
 *
 *  Author:	Tony Della Fera, DEC
 *		September, 1984
 * Hacked up by a cast of thousands....
 */

#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <X/Xlib.h>

#define VERTICES_IN_HANDS	4	/* to draw triangle */
#define PHI			(PI / 16.)	/* half angle of hand tip */
#define PI			3.14159265358979
#define PI4			(PI / 4.)
#define	SINPHI			0.19509032201613
#define TWOPI			(2. * PI)

#define SEG_BUFF_SIZE		128

#define SECOND_HAND_FRACT	90
#define MINUTE_HAND_FRACT	70
#define HOUR_HAND_FRACT		40
#define HAND_WIDTH_FRACT	7
#define SECOND_WIDTH_FRACT	5
#define SECOND_HAND_TIME	30

#define DEF_UPDATE		60

#define DEF_BORDER		2
#define DEF_VECTOR_HEIGHT	1
#define DEF_VECTOR_WIDTH	1

#define DEF_DIGITAL_PADDING	10
#define DEF_DIGITAL_FONT	"6x10"
#define DEF_ANALOG_PADDING	8
#define DEF_ANALOG_WIDTH	164
#define DEF_ANALOG_HEIGHT	164

#define UNINIT			-1
#define FAILURE			0

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define abs(a) ((a) < 0 ? -(a) : (a))

typedef enum _bool {FALSE, TRUE} Bool;

Bool XClockDebug = FALSE;
Bool AnalogClock = FALSE;
Bool ShowSecondHand = FALSE;
Bool Reverse = FALSE;
Bool chime = FALSE;

int CenterX = 0;
int CenterY = 0;
int NumSegs = 0;

int FrgrndColor;
int BkgrndColor;
int BorderColor;
int HighColor;
int HandColor;

Vertex SegBuff[SEG_BUFF_SIZE];
Vertex *SegBuffPtr;

Window ClockWindow;

main(argc, argv)
	int argc;
	char **argv;
{
	char time_string[26];
	register char *time_ptr = time_string;
#ifdef DEBUG
	register Bool debug = XClockDebug;
#endif
	register int i;
	register int radius;
	register int padding = UNINIT;	/* UNINIT = parameter uninitialized. */
	int border = UNINIT;		/* UNINIT = parameter uninitialized. */
	int update = UNINIT;		/* UNINIT = parameter uninitialized. */
	int win_width = UNINIT;         /* UNINIT = parameter uninitialized. */
	int win_height = UNINIT;        /* UNINIT = parameter uninitialized. */
	int second_hand_length;
	int minute_hand_length;
	int hour_hand_length;
	int hand_width;
	int second_width;
	int readfds = 0;
	int maxfds = 0;
	int string_width;
	int status;
	Bool even_update = FALSE;

	long time_value;

	char *index();
	char *ctime(), asctim();
	char *strind;
	char *fn = DEF_DIGITAL_FONT;
	char *fore_color;
	char *back_color;
	char *high_color;
	char *hand_color;
	char *brdr_color;
	char *geom = NULL;
	char *display = NULL;
	int rvflag = 0;
	char beeped = 0;		/* beeped already this minute */

	Color cdef;

	struct tm *localtime();
	struct timeval timeout;
	struct tm tm; 
	struct tm otm; 

	Font font;
	FontInfo font_info; 
	XEvent event;
	char *def_val;

	def_val = XGetDefault(argv[0], "BorderWidth");
	if (def_val != NULL) border = atoi(def_val);

	def_val = XGetDefault(argv[0], "BodyFont");
	if (def_val != NULL) fn = def_val;

	fore_color = XGetDefault(argv[0], "Foreground");
	back_color = XGetDefault(argv[0], "Background");
	high_color = XGetDefault(argv[0], "Highlight");
	hand_color = XGetDefault(argv[0], "Hands");
	brdr_color = XGetDefault(argv[0], "Border");

	def_val = XGetDefault(argv[0], "ReverseVideo");
	if(def_val != NULL && strcmp(def_val, "on") == 0) rvflag++;

	def_val = XGetDefault(argv[0], "InternalBorder");
	if (def_val != NULL) padding = atoi(def_val);
	
	if ((def_val = XGetDefault(argv[0], "Mode")) != NULL) {
	    if (strcmp(def_val, "analog") == 0) AnalogClock = TRUE;
	    if (strcmp(def_val, "digital") == 0) AnalogClock = FALSE;
	    if (strcmp(def_val, "Analog") == 0) AnalogClock = TRUE;
	    if (strcmp(def_val, "Digital") == 0) AnalogClock = FALSE;
	}

	def_val = XGetDefault(argv[0],"Update");
	if (def_val != NULL) update = atoi(def_val);

	for (i = 1; i < argc; i++) {
		if (argv [i] [0] == '=') {
		        geom = argv[i];
		        continue;
		}
		strind = index(argv[i], ':');
		if(strind != NULL) {
		        display = argv[i];
			continue;
		}
		strind = index(argv [i], '-');
		if (strind == NULL) Syntax(argv[0]);
		if (strcmp(argv [i], "-a") == 0 ||
		    strcmp(argv [i], "-analog") == 0) {
			AnalogClock = TRUE;
			continue;
		}
		if (strcmp(argv [i], "-d") == 0 ||
		    strcmp(argv [i], "-digital") == 0) {
			AnalogClock = FALSE;
			continue;
		}
		if (strcmp(argv [i], "-bw") == 0 ||
		    strcmp(argv [i], "-border") == 0) {
			if (++i >= argc) Syntax(argv[0]);
			border = atoi(argv [i]);
			continue;
		}
		if (strcmp(argv[i], "-chime") == 0) {
			chime = TRUE;
			continue;
		}
#ifdef DEBUG
		if (strcmp(argv [i], "-debug") == 0) {
			XClockDebug = TRUE;
			debug = TRUE;
			continue;
		}
#endif
		if (strcmp(argv [i], "-fn") == 0 ||
		    strcmp(argv [i], "-font") == 0) {
			if (++i >= argc) Syntax(argv[0]);
			fn = argv [i];
			continue;
		}
		if (strcmp(argv [i], "-fg") == 0 ||
		    strcmp(argv [i], "-foreground") == 0) {
			if (++i >= argc) Syntax(argv[0]);
			fore_color = argv [i];
			continue;
		}
		if (strcmp(argv [i], "-bg") == 0 ||
		    strcmp(argv [i], "-background") == 0) {
			if (++i >= argc) Syntax(argv[0]);
			back_color = argv [i];
			continue;
		}
		if (strcmp(argv [i], "-hl") == 0 ||
		    strcmp(argv [i], "-highlight") == 0) {
			if (++i >= argc) Syntax(argv[0]);
			high_color = argv [i];
			continue;
		}
		if (strcmp(argv [i], "-hd") == 0 ||
		    strcmp(argv [i], "-hands") == 0) {
			if (++i >= argc) Syntax(argv[0]);
			hand_color = argv [i];
			continue;
		}
		if (strcmp(argv [i], "-bd") == 0 ||
		    strcmp(argv [i], "-bordercolor") == 0) {
			if (++i >= argc) Syntax(argv[0]);
			brdr_color = argv [i];
			continue;
		}
		if (strcmp(argv [i], "-help") == 0) {
			Syntax(argv[0]);
		}
		if (strcmp(argv [i], "-p") == 0 ||
		    strcmp(argv [i], "-padding") == 0) {
			if (++i >= argc) Syntax(argv[0]);
			padding = atoi(argv [i]);
			continue;
		}
		if (strcmp(argv [i], "-rv") == 0 ||
		    strcmp(argv [i], "-reverse") == 0) {
			rvflag++;
			continue;
		}
		if (strcmp(argv [i], "-u") == 0 ||
		    strcmp(argv [i], "-update") == 0) {
			if (++i >= argc) Syntax(argv[0]);
			update = atoi(argv [i]);
			continue;
		}
		Syntax(argv[0]);
	}

	/*
	 * Open up the display.
	 */
	if (XOpenDisplay(display) == NULL) {
		fprintf(stderr, "%s: Can't open display '%s'\n",argv[0], XDisplayName(display));
		exit(1);
	}
	if (rvflag) Reverse = TRUE;

	/*
	 * Set up colors and pixmaps.
	 */
	if (
	    DisplayCells() > 2 &&
	    brdr_color != NULL &&
	    XParseColor(brdr_color, &cdef) &&
	    XGetHardwareColor(&cdef)
	) BorderColor = cdef.pixel;
	else if (Reverse) BorderColor = WhitePixel;
	else BorderColor = BlackPixel;

	if (
	    DisplayCells() > 2 &&
	    fore_color != NULL &&
	    XParseColor(fore_color, &cdef) &&
	    XGetHardwareColor(&cdef)
	) FrgrndColor = cdef.pixel;
	else if (Reverse) FrgrndColor = WhitePixel;
	else FrgrndColor = BlackPixel;

	if (
	    DisplayCells() > 2 &&
	    back_color != NULL &&
	    XParseColor(back_color, &cdef) &&
	    XGetHardwareColor(&cdef)
	) BkgrndColor = cdef.pixel;
	else if (Reverse) BkgrndColor = BlackPixel;
	else BkgrndColor = WhitePixel;

	if (
	    AnalogClock &&
	    DisplayCells() > 2 &&
	    high_color != NULL &&
	    XParseColor(high_color, &cdef) &&
	    XGetHardwareColor(&cdef)
	) HighColor = cdef.pixel;
	else HighColor = FrgrndColor;

	if (
	    AnalogClock &&
	    DisplayCells() > 2 &&
	    hand_color != NULL &&
	    XParseColor(hand_color, &cdef) &&
	    XGetHardwareColor(&cdef)
	) HandColor = cdef.pixel;
	else HandColor = BkgrndColor;
	
	/*
	 * Set up analog and digital specific defaults.
	 */
	if (AnalogClock == TRUE) {
		/*
		 * Set analog defaults.
		 */
		if (padding == UNINIT) padding = DEF_ANALOG_PADDING;
		if (update == UNINIT) update = DEF_UPDATE;
		if (update <= SECOND_HAND_TIME) ShowSecondHand = TRUE;
	} 
	else {
		/* 
		 * Set digital defaults.
		 */

		if (padding == UNINIT) padding = DEF_DIGITAL_PADDING;
		if (update == UNINIT) update = DEF_UPDATE;

		/*
		 * Get font dependent information and determine window
		 * size from a test string.
		 */
		time(&time_value);
		time_ptr = ctime(&time_value);
		time_ptr[strlen(time_ptr) - 1] = 0;

		font = XGetFont(fn);
		if (font == FAILURE) XClockError("Can't get font");

		status = XQueryFont(font, &font_info);
		if (status == FAILURE) XClockError("Can't query font");

		string_width = XQueryWidth (time_ptr, font);
	
		}
	

	/* 
	 * Now set analog and digital independent defaults.
	 */
	if (border == UNINIT) border = DEF_BORDER;
	if (update > 1 && ((60 / update) * update == 60)) even_update = TRUE;

	/*
	 * Open the main window.
	 */
	{
	int min_width, min_height;
	char default_geom[20];
	OpaqueFrame frame;
	if (AnalogClock) {
		min_width = DEF_ANALOG_WIDTH/3;
		min_height = DEF_ANALOG_HEIGHT/3;
		sprintf (default_geom,
			 "%dx%d-0-0", DEF_ANALOG_WIDTH, DEF_ANALOG_HEIGHT);
		}
	else {
		min_width = string_width + (2 * padding);
		min_height = font_info.height + (2 * padding);
		sprintf (default_geom, "%dx%d-0-0", min_width, min_height);
		}
	frame.bdrwidth = border;
	frame.border = XMakeTile (BorderColor);
	frame.background = XMakeTile (BkgrndColor);
	ClockWindow = XCreate (
		AnalogClock ? "Analog XClock" : "Digital XClock",
		argv[0],
		geom,
		default_geom,
		&frame,
		min_width,
		min_height);
	if (ClockWindow == FAILURE)
		XClockError("Can't open clock window");
	}

	/*
	 * Select window exposure events to see if the contents of
	 * the window have been erased or altered.  Select unmap events so
	 * we can stop output when iconified.
	 */
	XSelectInput(ClockWindow, ExposeWindow|UnmapWindow);

	/*
	 * Map clock window to screen.
	 */
	XMapWindow(ClockWindow);

	/*
	 * Initialize the select system call's maximum file
	 * descriptor number to be one more than the file descriptor
	 * number of the X connection.
	 */
	maxfds = dpyno() + 1;

	/*
	 * Initialize the select timeout structure.
	 */
	timeout.tv_sec = update;
	timeout.tv_usec = 0;

	/*
	 * Synchronize X before proceeding.
	 */
	XSync(FALSE);

	/*
	 * Main clock loop.
	 */
	while (TRUE) {
		time(&time_value);
		tm = *localtime(&time_value);
		if (even_update) {
			/* Truncate to update interval, get new timeout */
			timeout.tv_sec = update - tm.tm_sec;
			tm.tm_sec = (tm.tm_sec / update) * update;
			timeout.tv_sec += tm.tm_sec;
		}
		/*
		 * Beep on the half hour; double-beep on the hour.
		 */
		if (chime == TRUE) {
			if (beeped && (tm.tm_min != 30) && (tm.tm_min != 0))
				beeped = 0;
			if (((tm.tm_min == 30) || (tm.tm_min == 0)) 
			    && (!beeped)) {
				beeped = 1;
				XFeep(0);	
				if (tm.tm_min == 0)
					XFeep(0);
			}
		}

		if (AnalogClock == FALSE) {	
			/* 
			 * See if there are any events pending that
			 * arn't supposed to be there.
			 */
			if (XPending() != 0) {
				/*
				 * There is an event pending so we must
				 * check to see if it is an ExposeWindow
				 * event, if it is anything else somthing
				 * went wrong!
				 */
				XNextEvent(&event);
				if (event.type == UnmapWindow) {
					XPeekEvent(&event);
					continue;
				}
				if (event.type != ExposeWindow) {
					XClockError("Unexpected X_Event (digital mode)");
				}
			}
			time_ptr = asctime(&tm);
			time_ptr[strlen(time_ptr) - 1] = 0;
			XTextPad (
				ClockWindow,
				padding, padding, 
			    	time_ptr, strlen(time_ptr),
				font, 0, 0, 
				FrgrndColor, BkgrndColor,
				GXcopy, AllPlanes
			);
		}
		else {
			/* 
			 * Look for an X_Event associated with xclock.
			 */
			if (XPending() != 0) {
				/*
				 * There is an event pending so we must
				 * check to see if it is an ExposeWindow
				 * event, if it is anything else, somthing
				 * went wrong!
				 */
				XNextEvent(&event);
				if (event.type == UnmapWindow) {
					XPeekEvent(&event);
					continue;
				}
				if (event.type == ExposeWindow) {
					/*
					 * Ok, we have a window exposure event,
					 * refresh the clock face.  Check to
					 * see if the window has changed size.
					 */
					XExposeWindowEvent *exp_event =
					 (XExposeWindowEvent *)&event;
					if ((exp_event->width != win_width) ||
				            (exp_event->height != win_height)) {
						win_width = exp_event->width;
						win_height = exp_event->height;
						radius = (min(win_width,
						 win_height)-(2 * padding)) / 2;
						second_hand_length = 
							 ((SECOND_HAND_FRACT *
							 radius)
							 / 100);
						minute_hand_length =
							 ((MINUTE_HAND_FRACT *
							 radius) / 100);
						hour_hand_length =
							((HOUR_HAND_FRACT *
							radius) / 100);
						hand_width =
							 ((HAND_WIDTH_FRACT *
							 radius) / 100);
						second_width =
							 ((SECOND_WIDTH_FRACT *
							 radius) / 100);
						CenterX = win_width / 2;
						CenterY = win_height / 2;
					}
					DrawClockFace(second_hand_length,
						radius);
				}
				else {
					/*
					 * We should never get here!
					 */
					XClockError("Unexpected X_Event (analog mode)");
				}
			}
			/*
			 * The second (or minute) hand is sec (or min) 
			 * sixtieths around the clock face. The hour hand is
			 * (hour + min/60) twelfths of the way around the
			 * clock-face.  The derivation is left as an excercise
			 * for the reader.
			 */

			/*
			 * 12 hour clock.
			 */
			if(tm.tm_hour > 12)
				tm.tm_hour -= 12;

			/*
			 * Erase old hands.
			 */
			if(NumSegs > 0) {
			    if (ShowSecondHand == TRUE) {
				XDraw(
				    ClockWindow,
				    &SegBuff[2 * VERTICES_IN_HANDS],
				    NumSegs - 2 * VERTICES_IN_HANDS,
				    DEF_VECTOR_WIDTH,
				    DEF_VECTOR_HEIGHT,
				    BkgrndColor, GXcopy, AllPlanes
				);
				if(HandColor != BkgrndColor) {
				    XDrawFilled(
					ClockWindow,
					&SegBuff[2*VERTICES_IN_HANDS],
					NumSegs - 2*VERTICES_IN_HANDS,
					BkgrndColor, GXcopy, AllPlanes
				    );
				}
			    }
			    if(
				tm.tm_min != otm.tm_min ||
				tm.tm_hour != otm.tm_hour
			    ) {
				XDraw(
				    ClockWindow, SegBuff,
				    2 * VERTICES_IN_HANDS,
				    DEF_VECTOR_WIDTH, DEF_VECTOR_HEIGHT,
				    BkgrndColor, GXcopy, AllPlanes
				);
				if(HandColor != BkgrndColor) {
				    XDrawFilled(
					ClockWindow,
					SegBuff, VERTICES_IN_HANDS,
					BkgrndColor, GXcopy, AllPlanes
				    );
				    XDrawFilled(
					ClockWindow,
					&SegBuff[VERTICES_IN_HANDS],
					VERTICES_IN_HANDS,
					BkgrndColor, GXcopy, AllPlanes
				    );
				}
			    }
			}

			if (
			    NumSegs == 0 ||
			    tm.tm_min != otm.tm_min ||
			    tm.tm_hour != otm.tm_hour
			) {
			    SegBuffPtr = SegBuff;
			    NumSegs = 0;
			    /*
			     * Calculate the hour hand, fill it in with its
			     * color and then outline it.  Next, do the same
			     * with the minute hand.  This is a cheap hidden
			     * line algorithm.
			     */
			    DrawHand(
				minute_hand_length, hand_width,
				((double) tm.tm_min)/60.0
			    );
			    if(HandColor != BkgrndColor)
				XDrawFilled(
				    ClockWindow,
				    SegBuff, VERTICES_IN_HANDS,
				    HandColor, GXcopy, AllPlanes
				);
			    XDraw(
				ClockWindow,
				SegBuff, VERTICES_IN_HANDS,
				DEF_VECTOR_WIDTH, DEF_VECTOR_HEIGHT,
				HighColor, GXcopy, AllPlanes
			    );

			    DrawHand(
				hour_hand_length, hand_width,
				((((double)tm.tm_hour) + 
				    (((double)tm.tm_min)/60.0)) / 12.0)
			    );
			    XDrawFilled(
				ClockWindow,
				&SegBuff[VERTICES_IN_HANDS], VERTICES_IN_HANDS,
				HandColor, GXcopy, AllPlanes
			    );

			    i = VERTICES_IN_HANDS;
			} else i = 2 * VERTICES_IN_HANDS;

			if (ShowSecondHand == TRUE) {
			    NumSegs = 2 * VERTICES_IN_HANDS;
			    SegBuffPtr = &SegBuff[NumSegs];
			    DrawSecond(
				second_hand_length - 2, second_width,
				minute_hand_length + 2,
				((double) tm.tm_sec)/60.0
			    );
			    if(HandColor != BkgrndColor)
				XDrawFilled(
				    ClockWindow,
				    &SegBuff[2 * VERTICES_IN_HANDS],
				    NumSegs - 2 * VERTICES_IN_HANDS,
				    HandColor, GXcopy, AllPlanes
			    );
			}
			XDraw(
			    ClockWindow, &SegBuff[i], NumSegs - i,
			    DEF_VECTOR_WIDTH, DEF_VECTOR_HEIGHT,
			    HighColor, GXcopy, AllPlanes
			);
			otm = tm;
		}
		XFlush ();

		/*
		 * Use the select system call on the file descriptor in
		 * the display structure to determine if there is work
		 * to be done.  If not block untill timeout.  Remember to
		 * reset the file descriptor before each select.
		 */
		readfds = 1 << dpyno();
		if (select(maxfds, &readfds, NULL, NULL, &timeout) == -1)
		XClockError("Error in select on display file descriptor");
	}
}

/*
 * DrawLine - Draws a line.
 *
 * blank_length is the distance from the center which the line begins.
 * length is the maximum length of the hand.
 * Fraction_of_a_circle is a fraction between 0 and 1 (inclusive) indicating
 * how far around the circle (clockwise) from high noon.
 *
 * The blank_length feature is because I wanted to draw tick-marks around the
 * circle (for seconds).  The obvious means of drawing lines from the center
 * to the perimeter, then erasing all but the outside most pixels doesn't
 * work because of round-off error (sigh).
 */
DrawLine(blank_length, length, fraction_of_a_circle)
	int blank_length;
	int length;
	double fraction_of_a_circle;
{

	register double angle, cosangle, sinangle;
	double cos();
	double sin();

	/*
	 *  A full circle is 2 PI radians.
	 *  Angles are measured from 12 o'clock, clockwise increasing.
	 *  Since in X, +x is to the right and +y is downward:
	 *
	 *	x = x0 + r * sin(theta)
	 *	y = y0 - r * cos(theta)
	 *
	 */
	angle = TWOPI * fraction_of_a_circle;
	cosangle = cos(angle);
	sinangle = sin(angle);

	/*
	 * Add the move instruction to the segment buffer and increment
	 * the "next" pointer.
	 */
	SetSeg(CenterX + (int)(blank_length * sinangle),
	 CenterY - (int)(blank_length * cosangle), VertexDontDraw);

	/*
	 * Add the new point to the buffer and increment the "next"
	 * pointer.
	 */
	SetSeg(CenterX + (int)(length * sinangle),
	 CenterY - (int)(length * cosangle), VertexDrawLastPoint);
}

/*
 * DrawHand - Draws a hand.
 *
 * length is the maximum length of the hand.
 * width is the half-width of the hand.
 * Fraction_of_a_circle is a fraction between 0 and 1 (inclusive) indicating
 * how far around the circle (clockwise) from high noon.
 *
 */
DrawHand(length, width, fraction_of_a_circle)
	int length, width;
	double fraction_of_a_circle;
{

	register double angle, cosangle, sinangle;
	register Vertex *tip;
	register double ws, wc;
	double cos();
	double sin();

	/*
	 *  A full circle is 2 PI radians.
	 *  Angles are measured from 12 o'clock, clockwise increasing.
	 *  Since in X, +x is to the right and +y is downward:
	 *
	 *	x = x0 + r * sin(theta)
	 *	y = y0 - r * cos(theta)
	 *
	 */
	angle = TWOPI * fraction_of_a_circle;
	cosangle = cos(angle);
	sinangle = sin(angle);
	/*
	 * Order of points when drawing the hand.
	 *
	 *		1,4
	 *		/ \
	 *	       /   \
	 *	      /     \
	 *	    2 ------- 3
	 */
	tip = SegBuffPtr;
	SetSeg(CenterX + round(length * sinangle),
	 CenterY - round(length * cosangle), VertexDontDraw);	/* 1 */

	wc = width * cosangle;
	ws = width * sinangle;
	/* 2 */
	SetSeg(CenterX - round(ws + wc), CenterY + round(wc - ws),
	 VertexDrawLastPoint);
	/* 3 */
	SetSeg(CenterX - round(ws - wc), CenterY + round(wc + ws),
	 VertexDrawLastPoint);

	SetSeg(tip->x, tip->y, VertexDrawLastPoint);	/* 4 */
}

/*
 * DrawSecond - Draws the second hand (diamond).
 *
 * length is the maximum length of the hand.
 * width is the half-width of the hand.
 * offset is direct distance from center to tail end.
 * Fraction_of_a_circle is a fraction between 0 and 1 (inclusive) indicating
 * how far around the circle (clockwise) from high noon.
 *
 */
DrawSecond(length, width, offset, fraction_of_a_circle)
	int length, width, offset;
	double fraction_of_a_circle;
{

	register double angle, cosangle, sinangle;
	register Vertex *tip;
	register double ms, mc, ws, wc;
	register int mid;
	double cos();
	double sin();

	/*
	 *  A full circle is 2 PI radians.
	 *  Angles are measured from 12 o'clock, clockwise increasing.
	 *  Since in X, +x is to the right and +y is downward:
	 *
	 *	x = x0 + r * sin(theta)
	 *	y = y0 - r * cos(theta)
	 *
	 */
	angle = TWOPI * fraction_of_a_circle;
	cosangle = cos(angle);
	sinangle = sin(angle);
	/*
	 * Order of points when drawing the hand.
	 *
	 *		1,5
	 *		/ \
	 *	       /   \
	 *	      /     \
	 *	    2<       >4
	 *	      \     /
	 *	       \   /
	 *		\ /
	 *	-	 3
	 *	|
	 *	|
	 *   offset
	 *	|
	 *	|
	 *	-	 + center
	 */
	tip = SegBuffPtr;
	SetSeg(CenterX + round(length * sinangle),
	 CenterY - round(length * cosangle), VertexDontDraw);	/* 1 */

	mid = (length + offset) / 2;
	mc = mid * cosangle;
	ms = mid * sinangle;
	wc = width * cosangle;
	ws = width * sinangle;
	/* 2 */
	SetSeg(CenterX + round(ms - wc), CenterY - round(mc + ws),
	 VertexDrawLastPoint);
	SetSeg(CenterX + round(offset * sinangle),
	 CenterY - round(offset * cosangle), VertexDrawLastPoint);	/* 3 */
	/* 4 */
	SetSeg(CenterX + round(ms + wc), CenterY - round(mc - ws),
	 VertexDrawLastPoint);

	SetSeg(tip->x, tip->y, VertexDrawLastPoint);	/* 5 */
}

SetSeg(x, y, flag)
int x, y, flag;
{
	SegBuffPtr->x = x;
	SegBuffPtr->y = y;
	(SegBuffPtr++)->flags = flag;
	NumSegs++;
}

/*
 *  Draw the clock face (every fifth tick-mark is longer
 *  than the others).
 */
DrawClockFace(second_hand, radius)
	int second_hand;
	int radius;
{
	register Bool debug = XClockDebug;
	register int i;
	register int delta = (radius - second_hand) / 3;
	
	XClear(ClockWindow);
	SegBuffPtr = SegBuff;
	NumSegs = 0;
	for (i = 0; i < 60; i++)
		DrawLine((i % 5) == 0 ? second_hand : (radius - delta), radius,
		 ((double) i)/60.);
	/*
	 * Go ahead and draw it.
	 */
	XDraw(ClockWindow, SegBuff, NumSegs, DEF_VECTOR_WIDTH,
	 DEF_VECTOR_HEIGHT, FrgrndColor, GXcopy, AllPlanes);
	SegBuffPtr = SegBuff;
	NumSegs = 0;
}

round(x)
double x;
{
	return(x >= 0 ? (int)(x + .5) : (int)(x - .5));
}

/*
 * Report the syntax for calling xclock.
 */
Syntax(call)
	char *call;
{
	printf ("Usage: %s [-analog] [-bw <pixels>] [-digital]\n", call);
	printf ("       [-fg <color>] [-bg <color>] [-hl <color>] [-bd <color>]\n");
	printf ("       [-fn <font_name>] [-help] [-padding <pixels>]\n");
	printf ("       [-rv] [-update <seconds>] [[<host>]:[<vs>]]\n");
	printf ("       [=[<width>][x<height>][<+-><xoff>[<+-><yoff>]]]\n\n");
	printf ("Default: %s -digital -bw %d -font %s -padding %d -update %d =-0-0\n\n",
		call, DEF_BORDER, DEF_DIGITAL_FONT, DEF_DIGITAL_PADDING, DEF_UPDATE);
	printf ("Default: %s -analog -bw %d -padding %d -update %d =%dx%d-0-0\n\n",
		call, DEF_BORDER, DEF_DIGITAL_PADDING, DEF_UPDATE,
		DEF_ANALOG_WIDTH, DEF_ANALOG_HEIGHT);
	printf ("Notes: The order in which switches are specified is not significant.\n");
	printf ("       In analog mode the second hand only appears for update times\n");
	printf ("       less than or equal to %d seconds.\n", SECOND_HAND_TIME);
	exit(0);
}


/*
 * XClockError - Fatal xclock error.
 */
XClockError (identifier)
	char *identifier;
{
	register Bool debug = XClockDebug;

	if (debug) printf("XClockError: Fatal xclock error encountered.\n");
	perror("xclock");
	fprintf(stderr, "xclock: %s\n", identifier);
	
	exit(1);
}

/*
 * End of xclock.c
 */
