/* $XConsortium: inspector.c,v 1.0 93/11/22 12:08:58 rws Exp $ */

/*
        NOTICE TO USER: The source code in this file is copyrighted under
        U.S. and international laws.  SunSoft, Inc., a Sun Microsystems,
        Inc. business of Mountain View, California owns the copyright. 
        Users and possessors of this source code are hereby granted a
        nonexclusive, royalty-free copyright and design patent license
        to use this code in individual and commercial software. 

        Any use of this source code must include, in the user documentation 
        and internal comments to the code, notices to the end user as follows:

(c) Copyright 1992, 1993 SunSoft, Inc., a Sun Microsystems, Inc. business.

        SUNSOFT, INC. AND SUN MICROSYSTEMS, INC. MAKE NO REPRESENTATIONS
        ABOUT THE SUITABILITY OF THIS SOURCE CODE FOR ANY PURPOSE. IT IS
        PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND. 
        SUNSOFT, INC., AND SUN MICROSYSTEMS, INC., SEVERALLY AND
        INDIVIDUALLY, DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOURCE
        CODE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
        FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUNSOFT, INC.
        OR SUN MICROSYSTEMS, INC., BE LIABLE FOR ANY SPECIAL, INDIRECT,
        INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER
        RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
        OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
        OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.
*/


/*
 * inspector.c - Image comparison tool 
 * 
 * System and library calls used (should be common to SysV and 4.2BSD):
 * printf(), fprintf(), system(),
 * opendir(), readdir()
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "X11/Xatom.h"
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#ifdef ATHENA
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Shell.h>
#endif

#include "imagelib.h"

#define TK_BUTTON_ACTIVE    0
#define TK_BUTTON_INACTIVE  1

#define DEBUG
#ifdef DEBUG
#define DebugSync(display) XSync(display,0)
#else
#define DebugSync(display)
#endif

char *getenv();

/* dimensions of various window components */
#define IMAGE_SIZE 400
#define IMAGE_WIDTH IMAGE_SIZE
#define IMAGE_HEIGHT IMAGE_SIZE
#define CONTROL_HEIGHT IMAGE_HEIGHT 
#define CONTROL_WIDTH 200
#define DESC_WIN_HEIGHT 60
#define DESC_WIN_WIDTH 790
#define TOOL_X 50
#define TOOL_Y 50
#define GAP 15

#define MAX_FILENAME_LEN 128 	/* max length of image file base name */
#define MAX_FILEPATH_LEN 256 	/* max length of image file base name */
#define MAX_BUF_LEN 1024	/* max len buffer */
#define MAX_IMAGE_TEXT_LEN 4096	/* max len of text description in image file*/


/* 
 * Xlib stuff
 */
GC	    gc;
Display	    *display;
Window      ref_win, curr_win, main_win_xid;
Window	    ref_pseudowin, curr_pseudowin;
XVisualInfo visinfo;
int	    screen;
I_image	    ref_image, curr_image, refdiff_image, currdiff_image;
I_cmap	    ref_cmap, curr_cmap;

/* 
 * toolkit stuff 
 */
#ifdef ATHENA
Widget toplevel,main_win,pshell;
Widget control_win, desc_win;
Widget filename_item, status_item, descr_item;
Widget accept_button, reject_button, unseen_button, delete_button, diffs_button;
Widget load_button, next_button, quit_button, rescan_button;
Widget viewport, desc_viewport, file_list;
Position x,y;
#endif

/*
 * parameters of images being displayed (reference and current)
 */
unsigned short rw, rh, rncol,   /* width, height, # of columns */
    cw, ch, cncol;
unsigned long rblack, cblack;	/* black pixel */

extern char *optarg;            /* for getopt */
extern int optind, opterr;

/* 
 * Advance declaration of event handling routines and other functions
 */
static void
    ref_win_repaint_proc(),
    curr_win_repaint_proc(),
    accept_notify(),
    reject_notify(),
    unseen_notify(),
    delete_notify(),
    diffs_notify(),
    load_notify(),
    rescan_notify(),
    quit_notify(),
    list_notify();
static void get_directory_info();


int 	numfiles;	
char    **filename_list;	/* list of file names */
char	filename_string[48],	/* image file name output string */    
	status_string[48];	/* status output string */
char	*ref_file;               /* pathname of reference image */
char    ref_file_path[MAX_FILEPATH_LEN]; 

int	currfile_index=0;	/* index in list of current file */
int	selected_index=0;	/* index in list of selected file */
char	*currfile_name=NULL;	/* name of current file (from list) */
char	*selected_name=NULL;	/* name of selected file (from list) */

char	*currdir_name,		/* names of current, reference image dirs*/
	*refdir_name,
	*baddir_name;

char	buff[MAX_BUF_LEN];	/* general usage string buffer */
int	err;			/* for return of error values */

int	diff_mode = 0;		/* true iff image differences are displayed */
int	diff_calculated = 0;	/* true iff differences have been computed */
int	twocmapused;		/* true iff two images can be loaded into */
				/* one color map */
FILE	*logfile = stdout;
int	fbdepth = 8;		/* frame buffer depth */
int	multi_window = 0;
Pixel_info pinfo;		/* pixel conversion info */


/*
 * main() - create two X drawables and the control panel, 
 * read the image directory, and pass control
 * to event handling loop.
 */
main(argc,argv)
int	argc;
char	*argv[];
{
    Window subwins[2];
    XSetWindowAttributes winattr;
    XSizeHints hints;
    XWMHints wmhints;
    Atom atom;
    int c;
    int i = 0;
    int visual_type;
    unsigned long border_pixel,background_pixel;

    /* process command line options */
    while ((c = getopt(argc,argv,"md:")) != EOF) {
        switch(c) {
            case 'd':
		fbdepth = atoi(optarg);
		argc--;
		i++;
	        break;
            case 'm':
		multi_window = 1;
		argc--;
	        break;
            case '?':
                printf("usage: %s: [-d 8 or 24] [-s]\n",argv[0]);
                printf("-d to specify depth of image windows, default is 8\n");
                printf("-s to create single parent window for all subwindows\n");
                exit(1);
        }
    }

    if (fbdepth != 8 && fbdepth != 24)
       fatal("ERROR: unsupported frame buffer depth %d, exiting...\n",fbdepth);
    /* 
     * set up the windows: main window, label window, 
     * control window.
     */

#ifdef ATHENA
    toplevel = XtInitialize(argv[0],"Inspector",NULL,0,(Cardinal*)&argc,&argv[i]);

    display = XtDisplay(toplevel);

if (multi_window) {

    control_win = XtVaCreateManagedWidget("control_win",
		formWidgetClass,
		toplevel,
		XtNheight, IMAGE_HEIGHT+DESC_WIN_HEIGHT+GAP,
		NULL);

    XtTranslateCoords(toplevel,
	(Position) TOOL_X,
	(Position) IMAGE_HEIGHT + 75,
	&x, &y);

    
    pshell = XtVaCreatePopupShell("Image Description", 
		transientShellWidgetClass,
		control_win,
		XtNx, x,
		XtNy, y,
		NULL);

    desc_win = XtVaCreateManagedWidget("desc_win",
		formWidgetClass,
		pshell,
		NULL);
} else {

    main_win = XtVaCreateManagedWidget("main_win",
		formWidgetClass,
		toplevel,
		XtNheight, IMAGE_HEIGHT+DESC_WIN_HEIGHT+GAP,
		XtNwidth, 2*IMAGE_WIDTH+CONTROL_WIDTH+GAP,
		NULL);

    desc_win = XtVaCreateManagedWidget("desc_win",
		formWidgetClass,
		main_win,
		XtNvertDistance, IMAGE_HEIGHT + GAP,
		XtNtop, XtChainTop,
		NULL);

    control_win = XtVaCreateManagedWidget("control_win",
		formWidgetClass,
		main_win,
		XtNhorizDistance, IMAGE_WIDTH * 2 + GAP,
		XtNheight, IMAGE_HEIGHT+DESC_WIN_HEIGHT+GAP,
		NULL);
}

    desc_viewport = XtVaCreateManagedWidget("desc_viewport",
                viewportWidgetClass,
                desc_win,
                XtNheight, 100,
                XtNallowVert,TRUE,
                XtNforceBars, TRUE,
		XtNwidth, DESC_WIN_WIDTH,
                NULL);

    descr_item = XtVaCreateManagedWidget("descr_item",
		labelWidgetClass,
		desc_viewport,
		XtNlabel, "",
	 	XtNjustify, XtJustifyLeft,
		NULL);    

    filename_item = XtVaCreateManagedWidget("filename_item",
		labelWidgetClass,
		control_win,
		XtNlabel, "File: none",
		XtNwidth, 200,
	 	XtNjustify, XtJustifyLeft,
		NULL);    

    status_item = XtVaCreateManagedWidget("status_item",
		labelWidgetClass,
		control_win,
		XtNlabel, "Status: ",
		XtNfromVert, filename_item,
		XtNwidth, 200,
	 	XtNjustify, XtJustifyLeft,
		NULL);    

    rescan_button = XtVaCreateManagedWidget("rescan_button",
		commandWidgetClass,
		control_win,
		XtNlabel, "rescan",
		XtNfromVert, status_item,
		NULL);
    XtAddCallback(rescan_button, XtNcallback, rescan_notify, NULL);

    diffs_button = XtVaCreateManagedWidget("diffs_button",
		commandWidgetClass,
		control_win,
		XtNlabel, "show diffs",
		XtNsensitive, FALSE,
		XtNfromVert, status_item,
		XtNfromHoriz, rescan_button,
		NULL);
    XtAddCallback(diffs_button, XtNcallback, diffs_notify, NULL);


    quit_button = XtVaCreateManagedWidget("quit_button",
		commandWidgetClass,
		control_win,
		XtNlabel, "quit",
		XtNfromVert, status_item,
		XtNfromHoriz, diffs_button,
		NULL);
    XtAddCallback(quit_button, XtNcallback, quit_notify, NULL);

    unseen_button = XtVaCreateManagedWidget("unseen_button",
		commandWidgetClass,
		control_win,
		XtNsensitive, FALSE,
		XtNlabel, "unseen",
		XtNfromVert, rescan_button,
		NULL);
    XtAddCallback(unseen_button, XtNcallback, unseen_notify, NULL);

    delete_button = XtVaCreateManagedWidget("delete_button",
		commandWidgetClass,
		control_win,
		XtNsensitive, FALSE,
		XtNlabel, "delete",
		XtNfromVert, quit_button,
		XtNfromHoriz, unseen_button,
		NULL);
    XtAddCallback(delete_button, XtNcallback, delete_notify, NULL);

    accept_button = XtVaCreateManagedWidget("accept_button",
		commandWidgetClass,
		control_win,
		XtNlabel, "accept",
		XtNsensitive, FALSE,
		XtNfromVert, unseen_button,
		NULL);
    XtAddCallback(accept_button, XtNcallback, accept_notify, NULL);

    reject_button = XtVaCreateManagedWidget("reject_button",
		commandWidgetClass,
		control_win,
		XtNsensitive, FALSE,
		XtNlabel, "reject",
		XtNfromVert, unseen_button,
		XtNfromHoriz, accept_button,
		NULL);
    XtAddCallback(reject_button, XtNcallback, reject_notify, NULL);


    load_button = XtVaCreateManagedWidget("load_button",
		commandWidgetClass,
		control_win,
		XtNlabel, "load",
		XtNfromVert, accept_button,
		NULL);
    XtAddCallback(load_button, XtNcallback, load_notify, NULL);

    next_button = XtVaCreateManagedWidget("next_button",
		commandWidgetClass,
		control_win,
		XtNlabel, "load next",
		XtNfromVert, accept_button,
		XtNfromHoriz, load_button,
		NULL);
    XtAddCallback(next_button, XtNcallback, load_notify, NULL);


    viewport = XtVaCreateManagedWidget("viewport",
		viewportWidgetClass,
		control_win,
		XtNheight, 360,
		XtNfromVert, next_button,
		XtNallowVert,TRUE,
		XtNforceBars, TRUE,
		NULL);

    get_directory_info();
    if (!numfiles) {
	printf("There are no images to verify\n");
	exit(1);
    }

    file_list = XtVaCreateManagedWidget("",
		listWidgetClass,
		viewport,
	 	XtNlongest, 100,
		XtNverticalList, TRUE,
		XtNlist, filename_list,
		XtNnumberStrings, numfiles,
		NULL);
    	selected_name = filename_list[selected_index];
    XawListHighlight(file_list,selected_index);
    XtAddCallback(file_list, XtNcallback, list_notify, NULL);

    XtRealizeWidget(toplevel);

if (multi_window) {
    XtPopup(pshell, XtGrabNone);
}

    main_win_xid = XtWindow(toplevel);

#endif


#ifdef ATHENA
if (multi_window) {
    hints.x = 2 * IMAGE_WIDTH + TOOL_X + GAP;
    hints.y = TOOL_Y;
} else {
    hints.x = TOOL_X; hints.y = TOOL_Y;
}
#endif
    hints.flags = USPosition;
    XSetNormalHints(display, main_win_xid, &hints);

    screen = DefaultScreen(display);

    if (fbdepth == 8) {
	visual_type = PseudoColor;
	background_pixel = WhitePixel(display,screen);
	border_pixel = BlackPixel(display,screen);
	if (!XMatchVisualInfo(display, screen, fbdepth, 
				visual_type, &visinfo)) {
		fatal("couldn't match Visual info\n");
	}
    } else  {
	background_pixel = 0xffffff;
	border_pixel = 0x0;
	visual_type = TrueColor;
	if (!XMatchVisualInfo(display, screen, fbdepth, 
				visual_type, &visinfo)) {
	    visual_type = DirectColor;
	    if (!XMatchVisualInfo(display, screen, fbdepth, 
				    visual_type, &visinfo)) {
		    fatal("couldn't match Visual info\n");
	    } 
	}

	pinfo.rmask = visinfo.red_mask;
	pinfo.gmask = visinfo.green_mask;
	pinfo.bmask = visinfo.blue_mask;
	for(pinfo.rshift = 0; ! (pinfo.rmask & (1 << pinfo.rshift)); 
	    pinfo.rshift++) ;
	for(pinfo.gshift = 0; ! (pinfo.gmask & (1 << pinfo.gshift)); 
	    pinfo.gshift++) ;
	for(pinfo.bshift = 0; ! (pinfo.bmask & (1 << pinfo.bshift)); 
	    pinfo.bshift++) ;
    }

    /* 
    * get PseudoColor visual information, create pseudocolor
    * windows for image display
    */

    winattr.colormap = 
	    XCreateColormap(display,main_win_xid,visinfo.visual,AllocNone);
    winattr.background_pixel = background_pixel;
    winattr.border_pixel = border_pixel;
    if (multi_window) {
	    /* image windows are separate X windows */
	    ref_win = XCreateWindow(display, 
			DefaultRootWindow(display),
			0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, 
			1, fbdepth, InputOutput, 
			visinfo.visual, CWColormap|CWBorderPixel|CWBackPixel, 
			&winattr);
	    /* inform window manager of name and geometry */
	    XStoreName(display, ref_win, "Reference Image");
	    hints.x = TOOL_X; hints.y = TOOL_Y;
	    hints.width = IMAGE_WIDTH; hints.height = IMAGE_HEIGHT;
	    hints.min_width = IMAGE_WIDTH/2; hints.min_height = IMAGE_HEIGHT/2;
	    hints.flags = USPosition | USSize | PMinSize;
	    XSetNormalHints(display, ref_win, &hints);
	    curr_win = XCreateWindow(display, 
			 DefaultRootWindow(display),
			 IMAGE_WIDTH+10, 0, IMAGE_WIDTH, IMAGE_HEIGHT, 
			 1, fbdepth, InputOutput, 
			 visinfo.visual, CWColormap|CWBorderPixel|CWBackPixel, 
			 &winattr);
	    XStoreName(display, curr_win, "Current Image");
	    hints.x += IMAGE_WIDTH+10;
	    XSetNormalHints(display, curr_win, &hints);
	    /* inform window manager that these windows are a group */
	    wmhints.window_group = main_win_xid;
	    wmhints.flags = WindowGroupHint;
	    XSetWMHints(display, main_win_xid, &wmhints);
	    XSetWMHints(display, ref_win, &wmhints);
	    XSetWMHints(display, curr_win, &wmhints);
    } else {
	    /* create the windows as subwindows of main win */
	    ref_win = XCreateWindow(display, main_win_xid,
			0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, 
			1, fbdepth, InputOutput, 
			visinfo.visual, CWColormap|CWBorderPixel|CWBackPixel, 
			&winattr);
	    curr_win = XCreateWindow(display, main_win_xid, 
			 IMAGE_WIDTH+2, 0, IMAGE_WIDTH, IMAGE_HEIGHT, 
			 1, fbdepth, InputOutput, 
			 visinfo.visual, CWColormap|CWBorderPixel|CWBackPixel, 
			 &winattr);
	    /*
	     * Set WM_COLORMAP_WINDOWS property so that window manager
	     * will install the cmaps of these subwindows.  (see ICCCM,
	     * sect. 4.1.8 on client colormaps)
	     */
	    atom = XInternAtom(display,"WM_COLORMAP_WINDOWS", False);
	    subwins[0] = ref_win;
	    subwins[1] = curr_win;
	    XChangeProperty(display, main_win_xid, atom, XA_WINDOW, 32,
			PropModePrepend, (const unsigned char*)subwins, 2);
    }

    XMapRaised(display,ref_win);
    XMapRaised(display, curr_win);

    /*
     * Start the main loop
     */
#ifdef ATHENA
    XtMainLoop();
#endif

    return 0;
}


/*
 * get_directory_info() - extract pathnames of the 3 image directories
 * from environment variables, confirm that they exist.  Read filenames
 * from current images directory into the toolkit file list.
 */
static void
get_directory_info()
{
    int i, len;
    struct dirent *dir_entry;	/* for directory reading */
    DIR	*dirp;
    static char basename[MAX_FILENAME_LEN];

    if ((refdir_name = getenv("REF_IMAGE_DIR")) == NULL) 
	fatal("must set environment var REF_IMAGE_DIR\n");

    if ((baddir_name = getenv("BAD_IMAGE_DIR")) == NULL) 
	fatal("must set environment var BAD_IMAGE_DIR\n");

    if ((currdir_name = getenv("CUR_IMAGE_DIR")) == NULL) 
	fatal("must set environment variable CUR_IMAGE_DIR\n");

    numfiles = 0;

#ifdef ATHENA
    /* the bad reference directory */
    if ((dirp = opendir(baddir_name)) == NULL) {
	fprintf(stderr, "Error - couldn't open bad image directory %s\n",
		baddir_name);
	exit(1);
    } 

    /* get number of files */
    for (dir_entry=readdir(dirp),numfiles=0; 
	dir_entry!=NULL;
	dir_entry=readdir(dirp)) {
	if (is_image(dir_entry->d_name,basename) && 
		is_rejected(basename)) { 
		numfiles++;
	}
    }
    closedir(dirp);

    /* The current images directory */
    if ((dirp = opendir(currdir_name)) == NULL) {
	fprintf(stderr, "Error - couldn't open current image directory %s\n",
		currdir_name);
	exit(1);	
    }
    /* get number of files */
    for (dir_entry=readdir(dirp); dir_entry!=NULL; dir_entry=readdir(dirp)) {
	if (is_image(dir_entry->d_name,basename)) { 
		numfiles++;
	}
    }
    closedir(dirp);
   
    filename_list = (String*) XtRealloc ((char*)filename_list,numfiles * sizeof(String));
   
    /* process bad image dir */
    if ((dirp = opendir(baddir_name)) == NULL) {
	fprintf(stderr, "Error - couldn't open bad image directory %s\n",
		baddir_name);
	exit(1);
    } 
    /* read directory entries into the toolkit file list */
    for (dir_entry=readdir(dirp), i=0;dir_entry!=NULL;dir_entry=readdir(dirp)) {
	if (is_image(dir_entry->d_name,basename) && is_rejected(basename)) { 
	    filename_list[i++] = XtNewString(basename);
	}
    }
    closedir(dirp);

    /* process current image dir */
    if ((dirp = opendir(currdir_name)) == NULL) {
	fprintf(stderr, "Error - couldn't open current image directory %s\n",
		currdir_name);
	exit(1);	
    }
    /* read directory entries into the toolkit file list */
    for (dir_entry=readdir(dirp);dir_entry!=NULL;dir_entry=readdir(dirp)) {
	if (is_image(dir_entry->d_name,basename)) { 
	    if (is_rejected(basename)) { 
	        fprintf(stderr, 
		    "Error - bad images found in current image directory %s\n",
		    currdir_name);
		exit(1);
	    }
	    filename_list[i++] = XtNewString(basename);
	}
    }
    closedir(dirp);
#endif

}

/*
 *
 * event-handling functions
 *
 */

/*
 * accept_notify() - attached to "accept" button.  Accept the current
 * image by moving it to the reference image directory.
 */
static void
accept_notify()
{
    static char newname[MAX_FILENAME_LEN];
    int result, len, i;

    /* Accept file by moving from unseen to good reference */
    /* or from bad reference to good reference */
    fprintf(logfile, "Accepted file %s\n", currfile_name);
    if (is_rejected(currfile_name)) {
	/* remove rejection marker */
        strncpy(newname, currfile_name, len = strlen(currfile_name) - 1);
	newname[len] = '\0';
        sprintf(buff, "mv %s/%s.img %s/%s.img", baddir_name, currfile_name,
	    refdir_name, newname);
    } else {
        sprintf(buff, "mv %s/%s.img %s/%s.img", currdir_name, currfile_name,
	    refdir_name, currfile_name);
    }
    result = system(buff);
    if (result) {
	fprintf(stderr, "error - couldn't execute command: %s\n", buff);
	exit (1);
    }


#ifdef ATHENA
    XtFree(filename_list[currfile_index]);
    for (i=currfile_index; i<numfiles-1; i++) {
	filename_list[i] = filename_list[i+1];
    }
    numfiles--;
    if (numfiles) {
	if (numfiles == currfile_index) /* just removed last item */
		currfile_index = --selected_index;
    	XawListChange(file_list, filename_list, numfiles, 0, TRUE);
    } else
    	XtVaSetValues(file_list, XtNnumberStrings, 0,
		XtNlist, NULL, NULL);
    XtVaSetValues(filename_item, XtNlabel, "File: none", NULL);
    XtVaSetValues(status_item, XtNlabel, " ", NULL);

    /* after acceptance, there is no current file */
    currfile_name = NULL;
    selected_name = numfiles ? filename_list[currfile_index] : NULL;
    if (numfiles) 
        XawListHighlight(file_list,currfile_index);
    else {
        XawListUnhighlight(file_list);
        XtSetSensitive(load_button,FALSE);
        XtSetSensitive(next_button,FALSE);
    }
    XtVaSetValues(descr_item, XtNlabel, "", NULL);
    XtVaSetValues(status_item, XtNlabel, "Status: ", NULL);
    XtSetSensitive(accept_button,FALSE);
    XtSetSensitive(reject_button,FALSE);
    XtSetSensitive(unseen_button,FALSE);
    XtSetSensitive(delete_button,FALSE);
    XtSetSensitive(diffs_button,FALSE);
#endif

    ref_win_repaint_proc();
    curr_win_repaint_proc();
}

static void
reject_notify()
{
    int result;
    static char newname[MAX_FILENAME_LEN];

    if (!is_rejected(currfile_name)) { 
	/* reject by moving to bad dir and adding "-" to filename */
        sprintf(newname, "%s-", currfile_name);

	sprintf(buff, "mv %s/%s.img %s/%s.img", currdir_name, currfile_name,
		baddir_name, newname);
	result = system(buff);
	if (result) {
	    fprintf(stderr, "error: couldn't execute command: %s\n", buff);
	    exit (1);
	}
	fprintf(logfile, "Rejected file %s\n", currfile_name);

#ifdef ATHENA
	XtFree(filename_list[currfile_index]);
	filename_list[currfile_index] = XtNewString(newname);
        XawListChange(file_list, filename_list, numfiles, 0, TRUE);
        XawListHighlight(file_list,currfile_index);
	currfile_name = selected_name = filename_list[currfile_index];
        XtVaSetValues(status_item, XtNlabel, "Status: Rejected", NULL);
#endif

    }

#ifdef ATHENA
    XtSetSensitive(reject_button,FALSE);
    XtSetSensitive(unseen_button,TRUE);
#endif

}

/*
 * unseen_notify() - attached to "mark unseen" button.  Marks
 * current file as unseen.
 */
static void
unseen_notify()
{
    static char newname[MAX_FILENAME_LEN];
    int result,len;

    if (is_rejected(currfile_name)) {
	/* change the filename to un-rejected version, removing trailing "-" */
	strncpy(newname, currfile_name, len = strlen(currfile_name)-1);
	newname[len] = '\0';
	sprintf(buff, "mv %s/%s.img %s/%s.img", baddir_name, currfile_name,
		currdir_name, newname);
	result = system(buff);
	if (result) {
	    fprintf(stderr, "error: couldn't execute command %s\n", buff);
	    exit (1);
	}

	fprintf(logfile, "Marked file %s unseen\n", currfile_name);

#ifdef ATHENA
	XtFree(filename_list[currfile_index]);
	filename_list[currfile_index] = XtNewString(newname);
        XawListChange(file_list, filename_list, numfiles, 0, TRUE);
        XawListHighlight(file_list,currfile_index);
	currfile_name = selected_name = filename_list[currfile_index];
        XtVaSetValues(status_item, XtNlabel, "Status: Unseen", NULL);
#endif

    }

#ifdef ATHENA
    XtSetSensitive(reject_button,TRUE);
    XtSetSensitive(unseen_button,FALSE);
#endif

}

/*
 * delete_notify() - attached to "delete" button.  
 * delete image by from reference or current image directory.
 */
static void
delete_notify()
{
    int result;
    int i;

    fprintf(logfile, "Deleted file %s\n", currfile_name);
    sprintf(buff, "rm %s/%s.img ", 
    	is_rejected(currfile_name)? baddir_name : currdir_name,
	    currfile_name);
    result = system(buff);
    if (result) {
	fprintf(stderr, "error - couldn't execute command: %s\n", buff);
	exit (1);
    }

#ifdef ATHENA
    XtFree(filename_list[currfile_index]);
    for (i=currfile_index; i<numfiles-1; i++) {
	filename_list[i] = filename_list[i+1];
    }
    numfiles--;
    if (numfiles) {
	if (numfiles == currfile_index) /* just removed last item */
		currfile_index = --selected_index;
    	XawListChange(file_list, filename_list, numfiles, 0, TRUE);
    } else
    	XtVaSetValues(file_list, XtNnumberStrings, 0,
		XtNlist, NULL, NULL);
    XtVaSetValues(filename_item, XtNlabel, "File: none", NULL);
    XtVaSetValues(status_item, XtNlabel, " ", NULL);

    /* after deletion, there is no current file */
    currfile_name = NULL;
    selected_name = numfiles ? filename_list[currfile_index] : NULL;
    if (numfiles)
    	XawListHighlight(file_list,currfile_index);
    else {
        XawListUnhighlight(file_list);
        XtSetSensitive(load_button,FALSE);
        XtSetSensitive(next_button,FALSE);
    }
    XtVaSetValues(descr_item, XtNlabel, "", NULL);
    XtVaSetValues(status_item, XtNlabel, "Status: ", NULL);
    XtSetSensitive(accept_button,FALSE);
    XtSetSensitive(reject_button,FALSE);
    XtSetSensitive(unseen_button,FALSE);
    XtSetSensitive(delete_button,FALSE);
    XtSetSensitive(diffs_button,FALSE);
#endif

    ref_win_repaint_proc();
    curr_win_repaint_proc();
}

/*
 * diffs_notify() -- attached to diffs button.  Toggles between diffs
 * mode, in which difference images are displayed in the two 
 * windows, and normal mode, in which the stored images themselves
 * are displayed.
 */
static void
diffs_notify()
{
    if (!diff_mode) {
	/* set up diff mode - calculate images if necessary */
	if (!diff_calculated) {
	    if (fbdepth == 8) {
	        if (twocmapused) {
		    i_diff_image(display, &ref_cmap, &curr_cmap, &ref_image, 
		    &curr_image, &refdiff_image, &currdiff_image, &err);
	        } else {
		    /* they share one color map */
		    i_diff_image(display, &curr_cmap, &curr_cmap, &ref_image, 
		    &curr_image, &refdiff_image, &currdiff_image, &err);
	        }
	    } else if (fbdepth == 24) {
		    i_diff_image24(display, &pinfo, &ref_image, 
		    &curr_image, &refdiff_image, &currdiff_image, &err);
	    }
	    if (err) {
	        fprintf(stderr, "Error - could not calculate differences\n");
		diff_calculated = 0;

#ifdef ATHENA
                XtSetSensitive(diffs_button,FALSE);
#endif
	    } else {
		diff_calculated = 1;		
	    }

	}
	/* only set diff_mode flag if diff calculation has worked */
	if (diff_calculated) {
	    diff_mode = 1;
#ifdef ATHENA
            XtVaSetValues(diffs_button,XtNlabel,"show normal",NULL);
#endif
	}
    } else {
	/* restore normal mode */
#ifdef ATHENA
        XtVaSetValues(diffs_button,XtNlabel,"show diffs",NULL);
#endif
	diff_mode = 0;
    }
    ref_win_repaint_proc();
    curr_win_repaint_proc();
    /* Synchronize so that button stays grayed until images shown,
     * to avoid user frustration
     */
    XSync(display,False); 
}

/*
 * load_notify() - attached to "load" and "load next" button.  
 * Load the image file currently selected (load) or the next one (load next), 
 * and the corresponding reference image if it exists.  
 * Enable buttons appropriately.
 */
static void
load_notify(w, client_data, call_data)
        Widget w;
        XtPointer client_data;
        XtPointer call_data;
{
    XSetWindowAttributes winattr;
    static char rtext[MAX_IMAGE_TEXT_LEN], *ctext;  /* textual image descr. */
    static char descr_string[MAX_IMAGE_TEXT_LEN+24] = "Image Description: ";
    int i,nrows; 
    int sel = 0;
    char *label;

    ctext = descr_string;
    
    if (currfile_name != NULL) {
	/* dispose of memory for images now displayed */
	i_free_image(&curr_image);
	if (fbdepth == 8) i_free_cmap(display, &curr_cmap);
	if (ref_file != NULL) {
	    i_free_image(&ref_image);
	    if (fbdepth == 8 && twocmapused) i_free_cmap(display, &ref_cmap);
	    if (diff_calculated) {
		i_free_image(&refdiff_image);
		i_free_image(&currdiff_image);
	    }
	}
    }
    /* get full path name of current image  */
#ifdef ATHENA
    XtVaGetValues(w, XtNlabel, &label, NULL);
    if (! strcmp(label,"load next")) {  /* load next button */
	/* advance to next file, if current file is not the last in the list */
        if (selected_index < numfiles - 1) {
	    selected_name = filename_list[++selected_index];
            XawListHighlight(file_list,selected_index);
	}
    }
    currfile_name = selected_name;
    currfile_index = selected_index;
#endif
    sprintf(buff, "%s/%s.img", 
	is_rejected(currfile_name)? baddir_name : currdir_name, 
	currfile_name);

    DebugSync(display);

    /* find appropriate reference image file and load the images */
    ref_file = ref_file_path;
    if (find_reference(currfile_name, ref_file)) {
        if (fbdepth == 8) {
	    i_load_image2(buff, ref_file, display,visinfo.visual, 
	    &curr_image, &ref_image, &curr_cmap, &ref_cmap,
	     ctext, rtext, &twocmapused, &err);
	} else if (fbdepth == 24) {
	    i_load_image24(buff,display,visinfo.visual,&pinfo,&curr_image,ctext,&err); 
            if (err) {
	        fprintf(stderr, "Error - could not load image file(s)\n");
	        return;
            }
	    i_load_image24(ref_file,display,visinfo.visual,&pinfo,&ref_image,rtext,&err); 
	}
	DebugSync(display);
    } else {
	/* no reference image found */
	ref_file = NULL;
        if (fbdepth == 8) {
	    i_load_image(buff,display,visinfo.visual, &curr_image, &curr_cmap, 
	    ctext, &err);
	} else if (fbdepth == 24) {
	    i_load_image24(buff,display,visinfo.visual,&pinfo,&curr_image,ctext,&err); 
	}
    }
    if (err) {
	fprintf(stderr, "Error - could not load image file(s)\n");
	return;
    }

    /* display image name & status, enable unseen or reject button */
    sprintf(filename_string,"file: %s", currfile_name);

#ifdef ATHENA
    XtVaSetValues(filename_item, XtNlabel, filename_string, NULL);
    XtVaSetValues(descr_item, XtNlabel, descr_string, NULL);
    if (is_rejected(currfile_name)) {
        XtVaSetValues(status_item, XtNlabel, "Status: Rejected", NULL);
        XtSetSensitive(unseen_button,TRUE);
        XtSetSensitive(reject_button,FALSE);
    } else {
        XtVaSetValues(status_item, XtNlabel, "Status: Unseen", NULL);
        XtSetSensitive(reject_button,TRUE);
        XtSetSensitive(unseen_button,FALSE);
    }

    /* update button status and flags */
    if (diff_mode) {
	XtVaSetValues(diffs_button, XtNlabel,"show diffs",NULL);
	diff_mode = 0;
    }
    diff_calculated = 0;
    XtSetSensitive(accept_button,TRUE);
    XtSetSensitive(delete_button,TRUE);
    if (ref_file != NULL) {
        XtSetSensitive(diffs_button,TRUE);
    }
#endif



    /* if image windows are separate X windows, raise them */
    if (multi_window) {
	XRaiseWindow(display, ref_win);
	XRaiseWindow(display, curr_win);
    }
    if (fbdepth == 8) {
        /*  update color maps & background pixels */
        winattr.colormap = curr_cmap.cmap;
        winattr.background_pixel = curr_cmap.blackpixel;
        XChangeWindowAttributes(display, curr_win, 
		CWColormap|CWBackPixel, &winattr);
        if (ref_file != NULL && twocmapused) {
	    winattr.colormap = ref_cmap.cmap;
            winattr.background_pixel = ref_cmap.blackpixel;
	}
        /* change ref window's attributes even if no reference image, */
        /* since its old cmap may have been destroyed */
        XChangeWindowAttributes(display, ref_win, 
		CWColormap|CWBackPixel, &winattr);
    }
    /*  redraw windows */
    ref_win_repaint_proc();
    curr_win_repaint_proc();
}

/*
 * rescan_notify() - attached to "rescan" button.  Rescan current
 * and bad image directory and reset inspector to a neutral state. 
 */
static void
rescan_notify()
{
    int i;

    for (i=0; i<numfiles; i++)
        XtFree(filename_list[i]);
    get_directory_info();
    if (! numfiles)
    	XtVaSetValues(file_list, XtNnumberStrings, 0, XtNlist, NULL, NULL);
    else {
    	XawListChange(file_list, filename_list, numfiles, 0, TRUE);
        selected_index = 0;
        selected_name = filename_list[selected_index];
        XawListHighlight(file_list,selected_index);
        XtSetSensitive(load_button,TRUE);
        XtSetSensitive(next_button,TRUE);
    }
    XtVaSetValues(descr_item, XtNlabel, "", NULL);
    XtVaSetValues(filename_item, XtNlabel, "File: none", NULL);
    XtVaSetValues(status_item, XtNlabel, "Status: ", NULL);
    XtSetSensitive(accept_button,FALSE);
    XtSetSensitive(reject_button,FALSE);
    XtSetSensitive(unseen_button,FALSE);
    XtSetSensitive(delete_button,FALSE);
    XtSetSensitive(diffs_button,FALSE);
    if (currfile_name != NULL) {
	currfile_name = NULL;
	i_free_image(&curr_image);
	if (fbdepth == 8) i_free_cmap(display, &curr_cmap);
	if (ref_file != NULL) {
	    i_free_image(&ref_image);
	    if (fbdepth == 8 && twocmapused) i_free_cmap(display, &ref_cmap);
	    if (diff_calculated) {
		i_free_image(&refdiff_image);
		i_free_image(&currdiff_image);
	    }
	}
    }
    ref_win_repaint_proc();
    curr_win_repaint_proc();
}

/*
 * list_notify() - get currfile index and name
 */
static void
list_notify(w,client_data,call_data)
        Widget w;
        XtPointer client_data;
        XtPointer call_data;
{
#ifdef ATHENA
    selected_name = ((XawListReturnStruct*)call_data)->string;
    selected_index = ((XawListReturnStruct*)call_data)->list_index;
#endif

}

/*
 * quit_notify() -- attached to quit button - quits!
 */
static void
quit_notify(w,client_data,call_data)
        Widget w;
        XtPointer client_data;
        XtPointer call_data;
{
    exit(0);
}

/*
 * ref_win_repaint_proc() - repaint the reference window.
 * if there is a reference image, draw either it or the the 
 * appropriate difference image, depending on the value of
 * diff_mode.
 */
static void
ref_win_repaint_proc()
{
    XClearWindow(display, ref_win);
    if (currfile_name != NULL && ref_file != NULL) {
	if (diff_mode) {
	    i_display_image(display, ref_win, &refdiff_image);
	} else { 
	    i_display_image(display, ref_win, &ref_image);
	}
	DebugSync(display);
    }
}

/*
 * curr_win_repaint_proc() - repaint the current image window.
 * If there is a current image, draw either it or the appropriate
 * difference image, depending on diff_mode.
 */
static void
curr_win_repaint_proc()
{
    XClearWindow(display, curr_win);
    if (currfile_name != NULL) {
	if (diff_mode) {
	    i_display_image(display, curr_win, &currdiff_image);
	} else  {
	    i_display_image(display, curr_win, &curr_image);
	}
	DebugSync(display);
    }
}


/* 
 * is_rejected - return 1 iff the named file is marked as rejected
 */
static int
is_rejected(name)
    char *name;
{
    return (name[strlen(name)-1] == '-');
}

/* 
 * is_image - return 1 iff the named file is an image file with .img extenstion 
 *            also return base image name (with .img stripped) in basename
 */
static int
is_image(name,basename)
    char *name;
    char *basename;
{
    int len;
    if ( (len = strlen (name)) > 4 && !strcmp(".img",name+len-4) ) {
	len -= 4;
        strncpy(basename,name,len);
	basename[len] = '\0';
        return 1;
    } else
        return 0;
}

/*
 * find_reference(image, pathout) - if a reference image for "image"
 * is found, copy the whole path into pathout and return true.
 * Otherwise, return false.
 */
static int
find_reference(imagename, pathout)
    char *imagename, *pathout;
{
    static char newname[MAX_FILENAME_LEN];
    int fd, len;

    if (is_rejected(imagename)) {
	/* reference file won't have rejection marker */
	strncpy(newname, imagename, len = strlen(imagename) - 1);
	newname[len] = '\0';
        sprintf(pathout, "%s/%s.img", refdir_name, newname);
    } else
        sprintf(pathout, "%s/%s.img", refdir_name, imagename);
    if ((fd = open(pathout, O_RDONLY)) != -1) {
	/* file exists */
	close(fd);
	return (1);
    }
    /* if we got this far, reference image not found in either place */
    return (0);
}

static int
fatal(msg) 
    char *msg;
{
    fprintf(stderr, "inspector error: %s", msg);
    exit(1);
}



