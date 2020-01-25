/*
 * Copyright (c) 1994  Dimitrios P. Bouras and William K. W. Cheung
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of the X Consortium shall not be
 * used in advertising or otherwise to promote the sale, use or other dealings
 * in this Software without prior written authorization from the X Consortium.
 * 
 * Derived from the MIT X11R5 xbiff, written by Jim Fulton, which is
 * copyrighted (c) 1988 X Consortium.
 *
 * Mailbox XPM additions-modifications: Dimitrios P. Bouras
 *                     audio additions: William K. W. Cheung
 *
 */

#include <X11/IntrinsicP.h>		/* for toolkit stuff */
#include <X11/StringDefs.h>		/* for useful atom names */
#include <X11/cursorfont.h>		/* for cursor constants */
#include <X11/Xosdefs.h>		/* for X_NOT_POSIX def */
#include <sys/stat.h>			/* for stat() ** needs types.h ***/
#include <sys/wait.h>			/* for waitpid() */
#include <stdio.h>				/* for printing error messages */
#include <pwd.h>				/* for getting username */
#include <stdlib.h>				/* for getenv() */

#ifndef NO_AUDIO
#ifdef NCD_AUDIO
#include <audio/audiolib.h>
#include <audio/soundlib.h>

static AuServer *aud;			/* Audio server handler */
#elif defined(RPLAY_AUDIO)
#include <rplay.h>
#endif
#endif

#include <sys/file.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>

#ifndef NO_AUDIO
#ifdef SUN_AUDIO
#include <multimedia/audio_filehdr.h>
#include <sun/audioio.h>
#endif
#endif

#ifndef X_NOT_POSIX
#ifdef _POSIX_SOURCE
# include <sys/wait.h>
#else
#define _POSIX_SOURCE
# include <sys/wait.h>
#undef _POSIX_SOURCE
#endif
# define waitCode(w)	WEXITSTATUS(w)
# define waitSig(w)	WIFSIGNALED(w)
typedef int		waitType;
# define INTWAITTYPE
#else /* ! X_NOT_POSIX */
#ifdef SYSV
# define waitCode(w)	(((w) >> 8) & 0x7f)
# define waitSig(w)	((w) & 0xff)
typedef int		waitType;
# define INTWAITTYPE
#else
# include	<sys/wait.h>
# define waitCode(w)	((w).w_T.w_Retcode)
# define waitSig(w)	((w).w_T.w_Termsig)
typedef union wait	waitType;
#endif /* SYSV else */
#endif /* ! X_NOT_POSIX else */

#include "xmail.xpm"		/* for flag up (mail present) bits */
#include "xnomail.xpm"		/* for flag down (mail not here) */

#ifndef min
#define min(x,y)	((x) < (y)? (x): (y))
#endif

#include <X11/Xaw/XawInit.h>
#include "MailboxP.h"			/* for implementation mailbox stuff */
#include <X11/Xmu/Drawing.h>
#include <X11/extensions/shape.h>

/*
 * The default user interface is to have the mailbox turn itself off whenever
 * the user presses a button in it.  Expert users might want to make this 
 * happen on EnterWindow.  It might be nice to provide support for some sort of
 * exit callback so that you can do things like press q to quit.
 */

static char defaultTranslations[] = 
  "<ButtonPress>:  unset()";

static void Check(), Set(), Unset();

static XtActionsRec actionsList[] = { 
    { "check",	Check },
    { "unset",	Unset },
    { "set",	Set },
};

/*
 * Storage for the XPM images for the two mailbox states.
 */
static XpmImage mail_xpmimg;
static XpmImage nomail_xpmimg;

/* Initialization of defaults */

#define offset(field) XtOffsetOf(Mailbox_XPM_Rec, mailbox.field)
#define goffset(field) XtOffsetOf(WidgetRec, core.field)

static XtResource resources[] = {
    { XtNupdate, XtCInterval, XtRInt, sizeof (int),
	offset (update), XtRString, "30" },
    { XtNfile, XtCFile, XtRString, sizeof (String),
	offset (filename), XtRString, NULL },
    { XtNcheckCommand, XtCCheckCommand, XtRString, sizeof(char*),
	offset (check_command), XtRString, NULL},
    { XtNvolume, XtCVolume, XtRInt, sizeof(int),
	offset (volume), XtRString, "33"},
    { XtNonceOnly, XtCBoolean, XtRBoolean, sizeof(Boolean),
	offset (once_only), XtRImmediate, (XtPointer)False },
    { NmailXpmFile, CMailXpmFile, XtRString, sizeof (String),
	offset (mail_xpmfile), XtRString, NULL },
    { NnomailXpmFile, CNomailXpmFile, XtRString, sizeof (String),
	offset (nomail_xpmfile), XtRString, NULL },
    { NmailSndFile, CMailSndFile, XtRString, sizeof (String),
	offset (mail_sndfile), XtRString, NULL },
    { Nmailtool, CMailTool, XtRString, sizeof (String),
	offset (mail_tool), XtRString, NULL },
};

#undef offset

static void GetMailFile(), CloseDown(), ReadIconFile(), DefaultXpmImage();
static void check_mailbox(), redraw_mailbox(), beep();
static void Initialize(), Realize(), Destroy(), Redisplay();
static Boolean SetValues();

Mailbox_XPM_ClassRec mailboxClassRec = {
    { /* core fields */
    /* superclass				*/	(WidgetClass) &simpleClassRec,
    /* class_name				*/	"Mailbox",
    /* widget_size				*/	sizeof(Mailbox_XPM_Rec),
    /* class_initialize			*/	XawInitializeWidgetSet,
    /* class_part_initialize	*/	NULL,
    /* class_inited				*/	FALSE,
    /* initialize				*/	Initialize,
    /* initialize_hook			*/	NULL,
    /* realize					*/	Realize,
    /* actions					*/	actionsList,
    /* num_actions				*/	XtNumber(actionsList),
    /* resources				*/	resources,
    /* resource_count			*/	XtNumber(resources),
    /* xrm_class				*/	NULLQUARK,
    /* compress_motion			*/	TRUE,
    /* compress_exposure		*/	TRUE,
    /* compress_enterleave		*/	TRUE,
    /* visible_interest			*/	FALSE,
    /* destroy					*/	Destroy,
    /* resize					*/	NULL,
    /* expose					*/	Redisplay,
    /* set_values				*/	SetValues,
    /* set_values_hook			*/	NULL,
    /* set_values_almost		*/	XtInheritSetValuesAlmost,
    /* get_values_hook			*/	NULL,
    /* accept_focus				*/	NULL,
    /* version					*/	XtVersion,
    /* callback_private			*/	NULL,
    /* tm_table					*/	defaultTranslations,
    /* query_geometry			*/	XtInheritQueryGeometry,
    /* display_accelerator		*/	XtInheritDisplayAccelerator,
    /* extension				*/	NULL
    },
    { /* simple fields */
    /* change_sensitive         */	XtInheritChangeSensitive
    },
    { /* mailbox fields */
    /* ignore                   */	0
    }
};

WidgetClass mailboxWidgetClass = (WidgetClass) &mailboxClassRec;


/*
 * widget initialization
 */

static GC get_mailbox_gc (w)
    MailboxWidget w;
{
    XtGCMask valuemask;
    XGCValues xgcv;

    valuemask = GCFunction | GCGraphicsExposures;
    xgcv.function = GXcopy;
    xgcv.graphics_exposures = False;	/* this is Bool, not Boolean */
    return (XtGetGC ((Widget) w, valuemask, &xgcv));
}


/* ARGSUSED */
static void Initialize (request, new)
    Widget request, new;
{
    MailboxWidget w = (MailboxWidget) new;
    int shape_event_base, shape_error_base;

    if ( !XShapeQueryExtension (XtDisplay (w), &shape_event_base,
                                               &shape_error_base)) {
	fprintf (stderr, "%s:  shape extensions not supported!\n",
		 "Mailbox widget");
	CloseDown (w, 1);
    }

    w->mailbox.shape_cache.mask = None;
    w->mailbox.gc = get_mailbox_gc (w);
    w->mailbox.interval_id = (XtIntervalId) 0;
    w->mailbox.flag_up = FALSE;
    w->mailbox.last_size = 0;
    if (!w->mailbox.filename) GetMailFile (w);

    /*
     * read the XPM files from the resources, if any, and create
     * XpmImages, or create XpmImages from default image data
     */

    w->mailbox.full.xpmimg = None;
    if (w->mailbox.mail_xpmfile) ReadIconFile(w, True);
    if (w->mailbox.full.xpmimg == None) DefaultXpmImage(w, True);

    w->mailbox.empty.xpmimg = None;
    if (w->mailbox.nomail_xpmfile) ReadIconFile(w, False);
    if (w->mailbox.empty.xpmimg == None) DefaultXpmImage(w, False);

#define _MAX(x,y) ((x>y)?x:y)
    w->core.width = _MAX( w->mailbox.full.width, w->mailbox.empty.width);
    w->core.height = _MAX( w->mailbox.full.height, w->mailbox.empty.height);
#undef _MAX

    return;
}


/*
 * action procedures
 */

/*
 * pretend there is new mail; put widget in flagup state
 */

/* ARGSUSED */
static void Set (gw, event, params, nparams)
    Widget gw;
    XEvent *event;
    String *params;
    Cardinal *nparams;
{
    MailboxWidget w = (MailboxWidget) gw;

    w->mailbox.last_size = -1;

    check_mailbox (w, TRUE, FALSE);	/* redraw, no reset */

    return;
}


/*
 * ack the existing mail; put widget in flagdown state
 */

/* ARGSUSED */
static void Unset (gw, event, params, nparams)
    Widget gw;
    XEvent *event;
    String *params;
    Cardinal *nparams;
{
    MailboxWidget w = (MailboxWidget) gw;

    check_mailbox (w, TRUE, TRUE);	/* redraw, reset */

    return;
}


/*
 * look to see if there is new mail; if so, Set, else Unset
 */

/* ARGSUSED */
static void Check (gw, event, params, nparams)
    Widget gw;
    XEvent *event;
    String *params;
    Cardinal *nparams;
{
    MailboxWidget w = (MailboxWidget) gw;

    check_mailbox (w, TRUE, FALSE);	/* redraw, no reset */

    return;
}


/* ARGSUSED */
static void clock_tic (client_data, id)
    XtPointer client_data;
    XtIntervalId *id;
{
    MailboxWidget w = (MailboxWidget) client_data;

    check_mailbox (w, FALSE, FALSE);	/* no redraw, no reset */

    /*
     * and reset the timer
     */

    w->mailbox.interval_id =
	XtAppAddTimeOut (XtWidgetToApplicationContext((Widget) w),
			 w->mailbox.update * 1000, clock_tic, client_data);

    return;
}

static void Realize (gw, valuemaskp, attr)
    Widget					gw;
    XtValueMask				*valuemaskp;
    XSetWindowAttributes	*attr;
{
    MailboxWidget		w = (MailboxWidget) gw;
    register Display	*dpy = XtDisplay (w);
    XpmAttributes		xpm_attr;

    *valuemaskp |= (CWBitGravity | CWCursor);
    attr->bit_gravity = ForgetGravity;
    attr->cursor = XCreateFontCursor (dpy, XC_top_left_arrow);

    (*mailboxWidgetClass->core_class.superclass->core_class.realize)
	(gw, valuemaskp, attr);

    /*
     * build the pixmaps for the two mailbox
     * states from the saved XpmImages
     */
    xpm_attr.valuemask = 0;
    XpmCreatePixmapFromXpmImage (dpy, w->core.window, w->mailbox.full.xpmimg,
								 &w->mailbox.full.pixmap,
								 &w->mailbox.full.bitmap, &xpm_attr);
    xpm_attr.valuemask = 0;
    XpmCreatePixmapFromXpmImage (dpy, w->core.window, w->mailbox.empty.xpmimg,
								 &w->mailbox.empty.pixmap,
								 &w->mailbox.empty.bitmap, &xpm_attr);

    w->mailbox.interval_id = 
	XtAppAddTimeOut (XtWidgetToApplicationContext((Widget) w),
					 w->mailbox.update * 1000, clock_tic, (XtPointer) w);

    w->mailbox.shape_cache.mask = None;

    check_mailbox (w, TRUE, FALSE);

    return;
}


static void Destroy (gw)
    Widget gw;
{
    MailboxWidget w = (MailboxWidget) gw;
    Display *dpy = XtDisplay (gw);

    XtFree (w->mailbox.filename);
    if (w->mailbox.interval_id) XtRemoveTimeOut (w->mailbox.interval_id);
    XtReleaseGC(gw, w->mailbox.gc);
#define freepix(p) if (p) XFreePixmap (dpy, p)
    freepix (w->mailbox.full.bitmap);		/* until cvter does ref cnt */
    freepix (w->mailbox.full.pixmap);
    freepix (w->mailbox.empty.bitmap);		/* until cvter does ref cnt */
    freepix (w->mailbox.empty.pixmap);
    freepix (w->mailbox.shape_cache.mask);
#undef freepix
    return;
}


static void Redisplay (gw)
    Widget gw;
{
    MailboxWidget w = (MailboxWidget) gw;

    check_mailbox (w, TRUE, FALSE);
}


static void check_mailbox (w, force_redraw, reset)
    MailboxWidget w;
    Boolean force_redraw, reset;
{
    long mailboxsize = 0;
    Boolean readSinceLastWrite = FALSE;
    static int pid = -1;
    int status, tmp;

    if (w->mailbox.check_command != NULL) {
	waitType wait_status;
	int	check_status;
#ifdef INTWAITTYPE
	wait_status = system(w->mailbox.check_command);
#else
	wait_status.w_status = system(w->mailbox.check_command);
#endif
	check_status = waitCode(wait_status);

	/* error in sh checkCommand execution */
	if (waitSig(wait_status))
	    check_status = 2;		/* act as if there is no mail */

	switch (check_status) {
	  case 0:
	    mailboxsize = w->mailbox.last_size + 1;
	    break;
	  case 2:
	    mailboxsize = 0;
	    break;
	  default:		/* treat everything else as no change */
	    	        /* case 1 is no change */
	    mailboxsize = w->mailbox.last_size;
	}
    } else {
		struct stat st;
		if (stat (w->mailbox.filename, &st) == 0) {
		    mailboxsize = st.st_size;
		    readSinceLastWrite = (st.st_atime > st.st_mtime);
		}
    }

    /*
     * If a child process has been started for executing the
     * program specified in the mail_tool resource, retrieve its
     * status so we don't end up with a zombie process every time.
     */

    if ( pid > 0 ) {						/* if child has been started */
	tmp = waitpid( pid, &status, WNOHANG );	/* check if it's still there */
	if ( tmp != 0 )							/* 0: status not available */
		pid = tmp;							/* child pid (done) or -1 */
    }

    /*
     * Now check for changes.  If reset is set then we want to pretent that
     * there is no mail.  If the mailbox is empty then we want to turn off
     * the flag.  Otherwise if the mailbox has changed size then we want to
     * put the flag up, unless the mailbox has been read since the last 
     * write.
     *
     * The cases are:
     *    o  forced reset by user                        DOWN
     *    o  no mailbox or empty (zero-sized) mailbox    DOWN
     *    o  if read after most recent write 		 	DOWN
     *    o  same size as last time                      no change
     *    o  bigger than last time                       UP
     *    o  smaller than last time but non-zero         UP
     *
     * The last two cases can be expressed as different from last
     * time and non-zero.
     */

    if (reset) {						/* forced reset */
		w->mailbox.flag_up = FALSE;
		force_redraw = TRUE;
		if (w->mailbox.mail_tool) {
		    pid = fork();
		    if (pid == 0) {				/* Child process */
				system(w->mailbox.mail_tool);
				_exit(0);
		    }
		}
    } else if (mailboxsize == 0) {		/* no mailbox or empty */
		w->mailbox.flag_up = FALSE;
		if (w->mailbox.last_size > 0)
			force_redraw = TRUE;		/* if change */
    } else if (readSinceLastWrite) { 	/* only when checkCommand is NULL */
	/* mailbox has been read after most recent write */
		if (w->mailbox.flag_up) {
	    	w->mailbox.flag_up = FALSE;
	    	force_redraw = TRUE;
		}
    } else if (mailboxsize != w->mailbox.last_size) {  /* different size */
		if (!w->mailbox.once_only || !w->mailbox.flag_up)
	    	beep(w); 
		if (!w->mailbox.flag_up)
	    	force_redraw = w->mailbox.flag_up = TRUE;
    	} 
    	w->mailbox.last_size = mailboxsize;
    	if (force_redraw) redraw_mailbox (w);
    	return;
	}

/*
 * get user name for building mailbox
 */

static void GetMailFile (w)
    MailboxWidget w;
{
    char *getlogin();
    char *username;

    /* MAIL env var overrides any hard-coded mail dir */
	username = getenv( "MAIL" );
	if (username!=(char *)NULL)
	{
		w->mailbox.filename = (String) XtMalloc (strlen (username) + 1);
		strcpy (w->mailbox.filename, username);
		return;
	}

	username = getlogin();
	if (!username) {
		struct passwd *pw = getpwuid (getuid ());

		if (!pw) {
	    	fprintf (stderr, "%s:  unable to find a username for you.\n",
		     	"Mailbox widget");
	    	CloseDown (w, 1);
		}
		username = pw->pw_name;
    }
	w->mailbox.filename = (String) XtMalloc (strlen (MAILBOX_DIRECTORY) + 1 +
											 strlen (username) + 1);
	strcpy (w->mailbox.filename, MAILBOX_DIRECTORY);
	strcat (w->mailbox.filename, "/");
	strcat (w->mailbox.filename, username);
	return;
}

static void CloseDown (w, status)
    MailboxWidget w;
    int status;
{
    Display *dpy = XtDisplay (w);

    XtDestroyWidget ((Widget)w);
    XCloseDisplay (dpy);
    exit (status);
}

/*
 * Called by Initialize() to read XPM icon files
 * and create XPM image data from their contents
 */
static void ReadIconFile (w, flag)
    MailboxWidget w;
    Bool flag;
{
    String			name;
    int				code;
    struct _mbimage	*im;
    XpmImage		*imp;

    if ( flag ) {
		name = w->mailbox.mail_xpmfile; /* True: file for "mail" */
		im = &w->mailbox.full;
		imp = &mail_xpmimg;
    }
    else {
		name = w->mailbox.nomail_xpmfile; /* False: file for no "mail" */
		im = &w->mailbox.empty;
		imp = &nomail_xpmimg;
    }
    code = XpmReadFileToXpmImage( name, imp, (XpmInfo *)NULL );
    if ( code == XpmSuccess ) {
		im->xpmimg = imp;
		im->width = imp->width;
		im->height = imp->height;
    }
    else
	fprintf (stderr, "%s: ReadIconFile(%s): %s\n",
			 "Mailbox widget", name, XpmGetErrorString(code));
}

/*
 * Called by Initialize() to create XPM image
 * data from the default (hard-coded) XPM icons
 */
static void DefaultXpmImage(w, flag)
    MailboxWidget w;
    Bool flag;
{
    char			**datap;
    int				code;
    struct _mbimage	*im;
    XpmImage		*imp;

    if ( flag ) {
		datap = xmail_xpm; /* True: data for "mail" */
		im = &w->mailbox.full;
		imp = &mail_xpmimg;
    }
    else {
		datap = xnomail_xpm; /* False: data for "no mail" */
		im = &w->mailbox.empty;
		imp = &nomail_xpmimg;
    }
    code = XpmCreateXpmImageFromData (datap, imp, (XpmInfo *)NULL);

    if ( code == XpmSuccess ) {
		im->xpmimg = imp;
		im->width = imp->width;
		im->height = imp->height;
    }
    else {
		fprintf (stderr, "%s: DefaultXpmImage(): %s\n",
				 "Mailbox widget", XpmGetErrorString(code));
		CloseDown (w, 1);
    }
}

/* ARGSUSED */
static Boolean SetValues (gcurrent, grequest, gnew)
    Widget gcurrent, grequest, gnew;
{
    MailboxWidget current = (MailboxWidget) gcurrent;
    MailboxWidget new = (MailboxWidget) gnew;
    Boolean redisplay = FALSE;

    if (current->mailbox.update != new->mailbox.update) {
		if (current->mailbox.interval_id) 
		XtRemoveTimeOut (current->mailbox.interval_id);
		new->mailbox.interval_id =
			XtAppAddTimeOut (XtWidgetToApplicationContext(gnew),
							 new->mailbox.update * 1000, clock_tic,
							 (XtPointer) gnew);
    }

    return (redisplay);
}


/*
 * drawing code
 */

static void redraw_mailbox (w)
    MailboxWidget w;
{
    register Display	*dpy = XtDisplay (w);
    register Window		win = XtWindow (w);
    GC					gc = w->mailbox.gc;
    struct _mbimage		*im;
    Widget				parent;

    if (w->mailbox.flag_up) 		/* draw the "mail" icon */
		im = &w->mailbox.full;
    else							/* draw the "no mail" icon */
		im = &w->mailbox.empty;
    XClearWindow (dpy, win);
    XCopyArea (dpy, im->pixmap, win, gc, 0, 0, im->width, im->height, 0, 0);

    /*
     * XXX - temporary hack; walk up widget tree to find top most parent (which
     * will be a shell) and mash it to have our shape.  This will be replaced
     * by a special shell widget.
     */

	for (parent = (Widget) w; XtParent(parent);
		parent = XtParent(parent));

    if (im->bitmap != w->mailbox.shape_cache.mask) {
		XShapeCombineMask (XtDisplay(parent), XtWindow(parent),
						   ShapeBounding, 0, 0, im->bitmap, ShapeSet);
		w->mailbox.shape_cache.mask = im->bitmap;
    }

    return;
}


static void beep (w)
    MailboxWidget w;
{
#ifndef NO_AUDIO
#ifdef NCD_AUDIO
	char	*auservername = NULL;
#elif defined(RPLAY_AUDIO)
	char	*rplay_name;
#endif
	int		audiofd, filefd;
	int		rn, wn, len;
	unsigned char	buf[256];
#ifdef SUN_AUDIO
	Audio_filehdr	*au_hdr;
	audio_info_t 	ais;
#endif

	if (w->mailbox.mail_sndfile)
	{
#ifdef NCD_AUDIO
		aud = AuOpenServer(auservername, 0, NULL, 0, NULL, NULL);
		if (aud)
		{
			if (!AuSoundPlaySynchronousFromFile(aud, w->mailbox.mail_sndfile, 
												w->mailbox.volume))
				fprintf(stderr, "%s: Couldn't play file \"%s\"\n", 
						"Mailbox widget", w->mailbox.mail_sndfile);

			AuCloseServer(aud);
			return;
		}
#elif defined(RPLAY_AUDIO)
		rplay_name = rplay_default_host();
		if (rplay_host_volume(rplay_name, w->mailbox.mail_sndfile,
							  (int)(w->mailbox.volume*2.55)) < 0)
			fprintf(stderr, "%s: Rplay couldn't play file \"%s\"\n", 
					"Mailbox widget", w->mailbox.mail_sndfile);
		else
			return;
#endif
#ifdef SUN_AUDIO
		audiofd = open( "/dev/audio", O_WRONLY | O_NDELAY ); 
		if (audiofd < 0)
		{
			fprintf(stderr, "%s: Problem opening /dev/audio.\n",
					"Mailbox widget");
			close(audiofd);
			return;
		}

		if( ioctl( audiofd, AUDIO_GETINFO, &ais ) )
		{
			fprintf(stderr, "%s: Problem retrieving /dev/audio info.\n",
					"Mailbox widget");
			return;
		}
		ais.play.gain = w->mailbox.volume;
		if( ioctl( audiofd, AUDIO_SETINFO, &ais ) )
		{
			fprintf(stderr, "%s: Problem setting /dev/audio info.\n",
					"Mailbox widget");
			close(audiofd);
			return;
		}

		filefd = open(w->mailbox.mail_sndfile, O_RDONLY);
		if (filefd < 0)
		{
			fprintf(stderr, "%s: Couldn't play file \"%s\"\n", 
				"Mailbox widget", w->mailbox.mail_sndfile);
			close(audiofd);
			return;
		}

		/* Read in the audio header */
		rn = read(filefd, buf, sizeof(Audio_filehdr)); 

		if (rn > 0 && strncmp(buf, ".snd", 4) != 0)
		{
			close(filefd);
			close(audiofd);
			fprintf(stderr, "%s: Invalid audio file format.\n",
					"Mailbox widget");
			return;
		}

		/* Strip the header */
		au_hdr = (Audio_filehdr *)buf;
		rn = au_hdr->hdr_size - sizeof(Audio_filehdr);
		for( ; rn > 0; )
		{
			len = min(rn, sizeof(buf));
			len = read(filefd, buf, len); 
			rn -= len;	
		}

		while(1)
		{
			rn = read(filefd, buf, sizeof(buf));
			if (rn < 0)
			{
				fprintf(stderr, "%s: Error reading from file \"%s\"\n", 
					"Mailbox widget", w->mailbox.mail_sndfile);
				close(audiofd);
				close(filefd);
				return;
			}
			if (rn == 0)
				break;
			while(1)
			{
				wn = write(audiofd, buf, rn);
				if ( wn < 0 )
				{
					fprintf(stderr, "%s: Error writing to /dev/audio.\n", 
						"Mailbox widget");
					close(audiofd);
					close(filefd);
					break;
				}
				if ( wn != 0 )
					break;
				usleep(1000);
			}
		}
		close(audiofd);
		close(filefd);
#endif /* #ifdef SUN_AUDIO */
	}
	else
#endif /* #ifndef NO_AUDIO */
		XBell (XtDisplay (w), w->mailbox.volume);
    return;
}
