/***********************************************************
Copyright 1991 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

			All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/*
** File: 
**
**   xmvex7.c --- MVEX test program 7
**   
** Author: 
**
**   David Carver (Digital Workstation Engineering/Project Athena)
**   Ported to MVEX by Todd Brunhoff (Tektronix Research Labs)
**
** Revisions:
**
**   01.08.91 Brunhoff
**     - ported to MVEX
**	 
*/
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/MVEX/MVEXlib.h>

static char *ViolationAction();
static char *ViolationState();
static char *OverrideState();
static char *MvexEventName();

int Priority = 100;


main(argc, argv)
int	    argc;
char	    *argv[];
{
    char chr;
    int ii, jj, status, in_motion;
    unsigned int mask;
    int screen;
    int dx, dy, dw, dh, px, py, cx, cy, vx, vy, vw, vh;
    int rx,ry,wx,wy;
    int delta;

    Display *dpy;
    Visual *vis,*def_vis;
    XVisualInfo *p_vis_info, vis_info_tmpl;
    unsigned long vis_id;
    unsigned long depth;
    XGCValues gc_attr;
    GC gc,stillgc;
    XSetWindowAttributes win_attr;
    Window root,main_win,child;
    Pixmap mskpix;
    Colormap cmap;
    XEvent event;
    XColor scolor,ecolor;

    Atom port;
    MVEXVin *pXvins, *vin;
    VideoIn vid;
    int nvins;
    MVEXOwner owner;
    MVEXVideoValues vv;
    int mvexBase;
    int nHue, nSat, nContrast, nBright;
    int iHue, iSat, iContrast, iBright;
    int iHueNormal, iSatNormal, iContrastNormal, iBrightNormal;
    MVEXFraction *hueList, *satList, *contrastList, *brightList;
    MVEXFraction lower, higher;
    int vflags;

    int shft;
    int ctrl;

    printf("\n	Welcome to Mvex test program #7\n\n");
    printf("  This program tests adjustments.  When the video window\n");
    printf("  appears, use h and H to decrease and increate hue, s and S\n");
    printf("  to decrease and increase saturation, b and B to decrease and\n");
    printf("  increase brightness, and c and C to decrease and increase\n");
    printf("  contrast.	 Hit r or R to reset values and ^c to exit\n");

    printf("\n> Press return to continue...");
    chr = getc(stdin);

    dpy = XOpenDisplay(0);
    if (!dpy)
    {
	printf("\n  Couldn't open display\n");
	printf("\n  Mvex test program #7 terminated\n");
	exit();
    }

    if (MVEXMajorVersion(dpy) < 1) {
	printf("\n  Your display has no MVEX extension.\n");
	exit();
    }

    screen = XDefaultScreen(dpy);
    root = XDefaultRootWindow(dpy);
    pXvins = MVEXGetVins( dpy, screen, &nvins);
    mvexBase = MVEXBaseEvent(dpy);
    XSynchronize(dpy, True);

    printf("MVEX version %d.%d\n",
	MVEXMajorVersion(dpy),
	MVEXMinorVersion(dpy));

    if (!nvins)
    {
	printf("\n  Your display has no video inputs.\n");
	printf("\n  MVEX test program #2 terminated.\n");
	exit();
    }

    if (!Setup(argc, argv, dpy, nvins, pXvins,
	       &vin, &vid, &port, &depth, &vis_id))
    {
	printf("\n  Setup failed.\n");
	printf("\n  MVEX test program #7 terminated\n");
	exit();
    }

    vis_info_tmpl.visualid = vis_id;
    p_vis_info = XGetVisualInfo(dpy, VisualIDMask, &vis_info_tmpl, &ii);
    if (!p_vis_info)
    {
	printf("      Error: Couldn't find visual ");
	printf("#%x listed for video input.\n", vis_id);
	printf("\n  Mvex test program #7 terminated\n");
	return;
    }

    vis = p_vis_info->visual;
    def_vis = XDefaultVisual(dpy,screen);

    if (vis->visualid == def_vis->visualid)
	cmap = XDefaultColormap(dpy,screen);
    else
	cmap = XCreateColormap(dpy, root, vis, AllocNone);

    XAllocNamedColor(dpy, cmap, "midnight blue", &scolor, &ecolor);

    win_attr.colormap = cmap;
    win_attr.background_pixel = scolor.pixel;
    win_attr.event_mask = KeyPressMask | VisibilityChangeMask;
    win_attr.border_pixel = scolor.pixel;

    main_win = XCreateWindow(dpy, root, 0, 0, 672, 512, 0, 
	depth, InputOutput, vis,
	CWColormap | CWBackPixel | CWEventMask |
	CWBorderPixel,
	&win_attr);

    XMapWindow(dpy, main_win);

    printf("\n	Waiting for window to become visible...\n");
    while (1)
    {
	XNextEvent(dpy, &event);
	if (event.type == VisibilityNotify) break;
    }

    gc_attr.foreground = 0xffffff;
    gc = XCreateGC(dpy, main_win, 
	GCForeground,
	&gc_attr);

    printf("For video id 0x%x\n", vid);
    if (vin->ability.n_hue) {
	hueList = MVEXFractionList(vin->ability.hue,
				   vin->ability.n_hue, &nHue);
	printf("  Normal Hue = %f, ranges from %f to %f in %d steps\n",
	    FracToFloat(&vin->ability.normal_hue),
	    FracToFloat(&hueList[0]), 
	    FracToFloat(&hueList[nHue-1]),
	    nHue);
	(void) MVEXMatchFractionInList(vin->ability.normal_hue,
				       hueList, nHue, &lower, &higher,
				       &iHue);
    }
    if (vin->ability.n_saturation) {
	satList = MVEXFractionList(vin->ability.saturation,
				   vin->ability.n_saturation, &nSat);
	printf("  Normal Saturation = %f, ranges from %f to %f in %d steps\n",
	    FracToFloat(&vin->ability.normal_saturation),
	    FracToFloat(&satList[0]), 
	    FracToFloat(&satList[nSat-1]),
	    nSat);
	(void) MVEXMatchFractionInList(vin->ability.normal_saturation,
				       satList, nSat, &lower, &higher,
				       &iSat);
    }
    if (vin->ability.n_bright) {
	brightList = MVEXFractionList(vin->ability.bright,
				   vin->ability.n_bright, &nBright);
	printf("  Normal Brightness = %f, ranges from %f to %f in %d steps\n",
	    FracToFloat(&vin->ability.normal_bright),
	    FracToFloat(&brightList[0]), 
	    FracToFloat(&brightList[nSat-1]),
	    nBright);
	(void) MVEXMatchFractionInList(vin->ability.normal_bright,
				       brightList, nBright, &lower, &higher,
				       &iBright);
    }
    if (vin->ability.n_contrast) {
	contrastList = MVEXFractionList(vin->ability.contrast,
				   vin->ability.n_contrast, &nContrast);
	printf("  Normal Contrast = %f, ranges from %f to %f in %d steps\n",
	    FracToFloat(&vin->ability.normal_contrast),
	    FracToFloat(&contrastList[0]), 
	    FracToFloat(&contrastList[nContrast-1]),
	    nContrast);
	(void) MVEXMatchFractionInList(vin->ability.normal_contrast,
				       contrastList, nContrast, &lower, &higher,
				       &iContrast);
    }

    printf("\n  MVEXSelectEvents on window 0x%x %s\n",
	main_win, "(Override and Violation)");
    MVEXSelectEvents(dpy, main_win, MVEXOverrideMask | MVEXViolationMask);
    printf("  MVEXSelectEvents on vid 0x%x(Sync)\n", vid);
    MVEXSelectEvents(dpy, vid, MVEXSyncMask);

    vv.priority = Priority;
    vv.hue = hueList[ iHueNormal = iHue ];
    vv.saturation = satList[ iSatNormal = iSat ];
    vv.bright = brightList[ iBrightNormal = iBright ];
    vv.contrast = contrastList[ iContrastNormal = iContrast ];
    vflags = VRPriority | VRHue | VRSaturation | VRBrightness | VRContrast;
    MVEXRenderVideo(dpy, vid, main_win, gc, port,
		    0, 0, 640, 480, 16, 16, 640, 480, vflags, &vv);

    printf("\n\n");
    ShowAdjustments(&vv);
    while (1)
    {
	XNextEvent(dpy, &event);

	if (event.type == KeyPress)
	{
	    ctrl = event.xkey.state & ControlMask;
	    shft = event.xkey.state & (ShiftMask | LockMask);

	    chr = XLookupKeysym(&event.xkey,0);
	    if (chr=='c' && ctrl)
	    {
		break;
	    }
	    else if (chr=='c' && !shft)
	    {
		if (--iContrast < 0) iContrast = 0;
		vv.contrast = contrastList[ iContrast ];
	    }
	    else if (chr=='c' && shft)
	    {
		if (++iContrast >= nContrast) iContrast = nContrast-1;
		vv.contrast = contrastList[ iContrast ];
	    }
	    else if (chr=='h' && !shft)
	    {
		if (--iHue < 0) iHue = 0;
		vv.hue = hueList[ iHue ];
	    }
	    else if (chr=='h' && shft)
	    {
		if (++iHue >= nContrast) iHue = nContrast-1;
		vv.hue = hueList[ iHue ];
	    }
	    else if (chr=='s' && !shft)
	    {
		if (--iSat < 0) iSat = 0;
		vv.saturation = satList[ iSat ];
	    }
	    else if (chr=='s' && shft)
	    {
		if (++iSat >= nContrast) iSat = nContrast-1;
		vv.saturation = satList[ iSat ];
	    }
	    else if (chr=='b' && !shft)
	    {
		if (--iBright < 0) iBright = 0;
		vv.bright = brightList[ iBright ];
	    }
	    else if (chr=='b' && shft)
	    {
		if (++iBright >= nContrast) iBright = nContrast-1;
		vv.bright = brightList[ iBright ];
	    }
	    else if (chr=='r')
	    {
		vv.contrast = contrastList[ iContrast = iContrastNormal ];
		vv.hue = hueList[ iHue = iHueNormal ];
		vv.saturation = satList[ iSat = iSatNormal ];
		vv.bright = brightList[ iBright = iBrightNormal ];
	    }
	    else
		continue;
	    MVEXRenderVideo(dpy, vid, main_win, gc, port,
		0, 0, 640, 480, 16, 16, 640, 480, vflags, &vv);
	    ShowAdjustments(&vv);
	}
	else if (event.type >= mvexBase)
	{
	    union {
		MVEXViolationEvent *violate;
		MVEXSetupEvent *setup;
		MVEXSyncEvent *sync;
		MVEXOverrideEvent *override;
		MVEXRequestEvent *request;
		XEvent *core;
	    } u;

	    u.core = &event;
	    switch(event.type - mvexBase) {
	    case MVEXViolation:
		printf("\n**Mvex %s Event, ",
		    MvexEventName(event.type - mvexBase));
		printf("drawable=0x%x,vid=0x%x,time=0x%x\n",
		    u.violate->drawable,
		    u.violate->vid,
		    u.violate->time);
		printf("  action=%s,state=%s\n",
		    ViolationAction(u.violate->action_mask),
		    ViolationState(u.violate->state));
		break;

	    case MVEXSetup:
		printf("\n**Mvex %s Event, ",
		    MvexEventName(event.type - mvexBase));
		printf("vid=0x%x,port=0x%x,time=0x%x,\n",
		    u.setup->vid,
		    u.setup->port,
		    u.setup->time);
		printf("  frames %savailable\n",
		    u.setup->frames_available ? "" : "not ");
		printf("  timecodes %savailable\n",
		    u.setup->timecodes_available ? "" : "not ");
		printf("  %sframe accurate\n",
		    u.setup->frame_accurate ? "" : "not ");
		break;

	    case MVEXSync:
		if (u.sync->state == MVEXSyncAcquired)
		    break;	/* cut down on needles output */
		printf("\n**Mvex %s Event, ",
		    MvexEventName(event.type - mvexBase));
		printf("drawable=0x%x,vid=0x%x,time=0x%x\n",
		    u.sync->drawable,
		    u.sync->vid,
		    u.sync->time);
		printf("  state=%s\n",
		    u.sync->state == MVEXSyncAcquired
		      ? "acquired" : "lost");
		break;

	    case MVEXOverride:
		if (u.override->vid == vid)
		    break;	/* cut down on needles output */
		printf("\n**Mvex %s Event, ",
		    MvexEventName(event.type - mvexBase));
		printf("drawable=0x%x,vid=0x%x,time=0x%x\n",
		    u.override->drawable,
		    u.override->vid,
		    u.override->time);
		printf("  state=%s", OverrideState(u.override->state));
		if (u.override->state == MVEXRequestOverride)
		    printf(",override id=0x%x", u.override->override_id);
		printf("\n");
		if (u.override->state == MVEXRequestOverride
		 && u.override->override_id != vid) {
		    printf("Override by another client!\n");
		    printf("Waiting for input before starting again\n");
		}
		break;

	    case MVEXRequest:
		printf("\n**Mvex %s Event, ",
		    MvexEventName(event.type - mvexBase));
		printf("drawable=0x%x,vid=0x%x,time=0x%x\n",
		    u.override->drawable,
		    u.override->vid,
		    u.override->time);
		break;

	    default:
		printf("\n**Mvex %s Event, ",
		    MvexEventName(event.type - mvexBase));
		printf("\n");
		break;
	    }
	}
    }

    printf("\n	Mvex test program #7 finished\n");

}

Setup(argc, argv, dpy, nvins, pXvins, p_vin, p_vid, p_port, p_depth, p_vis_id)
    int	    argc;
    char    *argv[];
    Display *dpy;
    int nvins;
    MVEXVin *pXvins;
    MVEXVin **p_vin;
    VideoIn *p_vid;
    Atom *p_port;
    unsigned long *p_depth;
    unsigned long *p_vis_id;
{
    int ii;
    int depth, visual_id, vref, portnum;
    Atom port;
    VideoIn vid;
    MVEXVin *pXvin;
    MVEXRenderModel *pModel;

    vid = vref = portnum = depth = visual_id = -1;

    /* LOOK THROUGH COMMAND LINE ARGUMENTS */

    for ( ii = 1; ii < argc; ii++ )
    {
	if (strcmp( argv[ii], "-vref") == 0)
	{
	    if(++ii < argc)
		vref = atoi(argv[ii]);
	    else
		UseMsg(argv);
	}
	else if (strcmp( argv[ii], "-port") == 0)
	{
	    if(++ii < argc)
		portnum = atoi(argv[ii]);
	    else
		UseMsg(argv);
	}
	else if (strcmp( argv[ii], "-depth") == 0)
	{
	    if(++ii < argc)
		depth = atoi(argv[ii]);
	    else
		UseMsg(argv);
	}
	else if (strcmp( argv[ii], "-visual") == 0)
	{
	    if(++ii < argc)
		visual_id = atoi(argv[ii]);
	    else
		UseMsg(argv);
	}
	else if (strcmp( argv[ii], "-priority") == 0)
	{
	    if(++ii < argc)
		Priority = atoi(argv[ii]);
	    else
		UseMsg(argv);
	}
	else if (strcmp( argv[ii], "-help") == 0)
	{
	    UseMsg(argv);
	}
    }

    if (vref < 0) {
	vref = 1;
	pXvin = pXvins;
    }
    else
    {
	if (vref > nvins)
	{
	    printf("\n	vref #%d doesn't exist (there's only %d).\n",
		vref, nvins);
	    return False;
	}
	pXvin = pXvins+(vref-1);
    }

    /*
     * make two video ids to tell the difference between the overrides
     */
    vid = MVEXCreateVideo(dpy, pXvins->vref);

    if (portnum < 0)
    {
	port = pXvin->ports[ 0 ];
    }
    else
    {
	if (portnum > pXvin->n_ports)
	{
	    printf("\n	Port #%d doesn't exist for video input #%d.\n", 
		portnum, vref);
	    return False;
	}
	port = pXvin->ports[ portnum-1 ];
    }

    pModel = &pXvin->model_list[ 0 ];
    if (depth < 0)
    {
	for (ii=0; ii<pXvin->n_models; ii++, pModel++)
	    if (pModel->model_mask & MVEXWindowModelMask)
	    {
		depth = pModel->depth;
		break;
	    }
	if (depth < 0) {
	    printf("\n  no window depths available for video input #%d\n",
		vref);
	    return False;
	}
    }
    else
    {
	for (ii=0; ii<pXvin->n_models; ii++, pModel++)
	    if (pModel->depth == depth)
		break;

	if (ii >= pXvin->n_models)
	{
	    printf("\n	Depth %d not supported by video input #%d.\n",
		depth, vref);
	    return False;
	}
    }

    if (visual_id < 0)
    {
	visual_id = pModel->visualid;
    }
    else
    {
	pModel = &pXvin->model_list[ 0 ];
	for (ii=0; ii<pXvin->n_models; ii++, pModel++)
	    if ((pModel->visualid == visual_id) && pModel->depth == depth)
		break;

	if (ii >= pXvin->n_models)
	{
	    printf("\n	Visual-id %d at depth %d not supported ",
		depth, visual_id);
	    printf("by video input #%d.\n", vref);
	    return False;
	}
    }

    *p_vin = pXvin;
    *p_vid = vid;
    *p_port = port;
    *p_depth = depth;
    *p_vis_id = visual_id;

    return True;
}

UseMsg(argv)
char	    *argv[];
{
    printf("use: %s [option]\n", argv[0]);
    printf("\t-vref #		       vref number (i.e., 1, 2, 3...)\n");
    printf("\t-port #		       port (i.e., 1, 2, 3...)\n");
    printf("\t-depth #		       drawable depth (planes)\n");
    printf("\t-visual #		       drawable visual (id)\n");
    printf("\t-priority #	       priority level 1-100\n");
    printf("\t-copyarea                use XCopyArea instead of XGetImage\n");
    exit();
}

char *MvexEventNames[] = {
    "Violation",
    "Setup",
    "Sync",
    "Override",
    "Request"
};

char *ViolationActions[] = {
    "scale",
    "placement",
    "clip",
    "overlap",
    "hue",
    "saturation",
    "contrast",
    "brightness",
};

char *ViolationStates[] = {
    "success",
    "fail",
    "subset"
};

char *OverrideStates[] = {
    "stopped",
    "drawable change",
    "marker reached",
    "request override",
    "hardware error",
    "reused",
};

#define AllViolationBits       (MVEXScaleViolation	\
			      | MVEXPlacementViolation	\
			      | MVEXClipViolation	\
			      | MVEXOverlapViolation	\
			      | MVEXHueViolation	\
			      | MVEXSaturationViolation	\
			      | MVEXContrastViolation	\
			      | MVEXBrightnessViolation)

static char *ViolationAction(action)
    int action;
{
    static char buf[ BUFSIZ ];
    int i, bits;

    buf[0] = '\0';
    bits = 0;
    for (i = 0; (1<<i) < AllViolationBits; i++) {
	if ((1<<i) & action) {
	    sprintf(buf + strlen(buf), "%s%s",
		bits ? "," : "", ViolationActions[ i ]);
	    bits++;
	}
    }
    if (action & (~AllViolationBits))
	    sprintf(buf + strlen(buf), "%s ?? bits", bits ? "and" : "");
    return (buf);
}

static char *ViolationState(state)
    int state;
{
    if (state >= sizeof(ViolationStates) / sizeof(ViolationStates[0]))
	return ("??");
    return (ViolationStates[ state ]);
}

static char *OverrideState(state)
    int state;
{
    if (state >= sizeof(OverrideStates) / sizeof(OverrideStates[0]))
	return ("??");
    return (OverrideStates[ state ]);
}

static char *MvexEventName(mvexType)
    int mvexType;
{
    if (mvexType >= sizeof(MvexEventNames) / sizeof(MvexEventNames[0]))
	return("unknown");
    else
	return(MvexEventNames[ mvexType ]);
}

ShowAdjustments(pvv)
    MVEXVideoValues *pvv;
{
    char buf[ BUFSIZ ];
    static int lastLength;
    int i;

    for (i = lastLength; i > 0; i--)
	putchar('\b');
    sprintf(buf, "contrast=%f, hue=%f, saturation=%f, bright=%f",
		FracToFloat(&pvv->contrast),
		FracToFloat(&pvv->hue),
		FracToFloat(&pvv->saturation),
		FracToFloat(&pvv->bright));
    lastLength = strlen(buf);
    fputs(buf, stdout);
    fflush(stdout);
}
