/* 
 * $Locker:  $ 
 */ 
static char     *rcsid = "$Header: test.c,v 1.2 87/06/17 16:08:52 swick Locked ";

/*
 *			WARNING
 *	The Coding Standards Authority has determined that
 *	this program is in imminent danger of collapse.
 *	Further hacking on it is entirely at your own risk.
 *	It is a test program for certain window-manager
 *	related server facilities,  not a viable part of
 *	your user interface.
 */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/X10.h>
#include "test.h"
#include "externs.h"
#include "dblArrow.h"
#include "growBox.h"
#include "cross.h"

char *ProgramName;

XAssocTable *frameInfo;
XFontStruct *font;
Display *dpy;
int ErrorFunc();

extern char *malloc();

void MoveOutline();

RootInfoRec *
findRootInfo(win)
    Window win;
{
    int	i;

    for (i = 0; i < ScreenCount(dpy); i++) {
	if (RootInfo[i].root == win)
	    return (&(RootInfo[i]));
    }
    return (NULL);
}

#ifdef	debug
static char *eventtype[] = {
	"zero",
	"one",
	"KeyPress",
	"KeyRelease",
	"ButtonPress",
	"ButtonRelease",
	"MotionNotify",
	"EnterNotify",
	"LeaveNotify",
	"FocusIn",
	"FocusOut",
	"KeymapNotify",
	"Expose",
	"GraphicsExpose",
	"NoExpose",
	"VisibilityNotify",
	"CreateNotify",
	"DestroyNotify",
	"UnmapNotify",
	"MapNotify",
	"MapRequest",
	"ReparentNotify",
	"ConfigureNotify",
	"ConfigureRequest",
	"GravityNotify",
	"ResizeRequest",
	"CirculateNotify",
	"CirculateRequest",
	"PropertyNotify",
	"SelectionClear",
	"SelectionRequest",
	"SelectionNotify",
	"ColormapNotify",
	"ClientMessage",
	"MappingNotify",
};
#endif	debug
Pixmap
MakeNewPixmap(dpy, root, opix, width, height, depth, fg, bg)
    Display *dpy;
    Window root;
    Pixmap opix;
    unsigned long width, height, depth, fg, bg;
{
    GC gc;
    XGCValues gcv;
    Pixmap pix;

    pix = XCreatePixmap(dpy, root, width, height, depth);
    if (!pix)
      return(0);
#define	MNP_GCV_FLAGS	(GCForeground|GCBackground|GCStipple|GCFillStyle)
    gcv.foreground = fg;
    gcv.background = bg;
    gcv.stipple = opix;
    gcv.fill_style = FillOpaqueStippled;
    gc = XCreateGC(dpy, pix, MNP_GCV_FLAGS, &gcv);
    XFillRectangle(dpy, pix, gc, 0, 0, width, height);
    XFreeGC(dpy, gc);
    return(pix);
}

main(argc, argv) int argc; char *argv[];
{
    Window      parent;
    Window     *children;
    int         nchildren;
    int         i;
    Window      dragWindow = None;
    Window      resizeWindow = None;
    Window      iconifyWindow = None;
    int         dragx, dragy;
    int         iconPressx, iconPressy;
    int         dragHeight, dragWidth;
    int         origx, origy, origWidth, origHeight;
    int         clampTop, clampBottom, clampLeft, clampRight;
    int         buttonsPressed = 0;
    int         buttonsDown[256];
    int         hdrWidth;

    XSetWindowAttributes attributes;
    XGCValues   gcv;

    ProgramName = argv[0];

    for (i = 0; i < ((sizeof buttonsDown)/(sizeof buttonsDown[0])); i++)
	buttonsDown[i] = 0;
    StartConnectionToServer(argc, argv);
    XSetErrorHandler(ErrorHandler);
    SetupDefaults(argc, argv, dpy);
#ifdef debug
    XSynchronize(dpy, 1);
#endif
    frameInfo = XCreateAssocTable(ASSOCTABLESZ);
    if (!frameInfo) {
	Error(1, "XCreateAssocTable failed\n");
    }

    font = XLoadQueryFont(dpy, fontName);
    if (!font)
	Error(1, "XFont failed\n");
#ifdef debug
    printf("descent=%d, ascent=%d, width=%d\n",
	   font->max_bounds.descent, font->max_bounds.ascent,
	   font->max_bounds.width);
#endif

    RootInfo = (RootInfoRec *) malloc(ScreenCount(dpy) * sizeof(RootInfoRec));
    for (i = 0; i < ScreenCount(dpy); i++) {
	RootInfo[i].root = RootWindow(dpy, i);
	RootInfo[i].background = FindColor(dpy, i, BackgroundPixelName,
					   BlackPixel(dpy, i));
	RootInfo[i].foreground = FindColor(dpy, i, ForegroundPixelName,
					   WhitePixel(dpy, i));
	RootInfo[i].borderpixel = FindColor(dpy, i, BorderPixelName,
					   WhitePixel(dpy, i));
	RootInfo[i].doubleArrow = MakePixmap(RootInfo[i].root,
					     dblArrow_bits,
					     dblArrow_width,
					     dblArrow_height);
	RootInfo[i].growBox = MakePixmap(RootInfo[i].root,
					 growBox_bits,
					 growBox_width,
					 growBox_height);
	RootInfo[i].cross = MakePixmap(RootInfo[i].root,
				       cross_bits,
				       cross_width,
				       cross_height);
	gcv.font = font->fid;
	gcv.foreground = RootInfo[i].foreground;
	gcv.background = RootInfo[i].background;
	RootInfo[i].headerGC = XCreateGC(dpy, RootInfo[i].root,
					 GCForeground | GCBackground | GCFont, &gcv);
	gcv.function = GXinvert;
	gcv.subwindow_mode = IncludeInferiors;
	RootInfo[i].xorGC = XCreateGC(dpy, RootInfo[i].root,
				      GCFunction | GCSubwindowMode, &gcv);
	{
	    Window      retroot;
	    int         bw, depth;
	    XGetGeometry(dpy, (Drawable) RootInfo[i].root, &retroot,
			 &RootInfo[i].rootx, &RootInfo[i].rooty,
	    &RootInfo[i].rootwidth, &RootInfo[i].rootheight, &bw, &depth);
	}
	/*
	 * select Substructure Redirect on the root to have MapRequest
	 * events generated instead of windows getting mapped 
	 */
	errorStatus = False;
#define	ROOT_EVENT_MASK ( \
			  SubstructureRedirectMask \
			  |ButtonPressMask \
			  |ButtonReleaseMask \
			  |FocusChangeMask \
			  |EnterWindowMask \
			  )
	attributes.event_mask = ROOT_EVENT_MASK;
	XChangeWindowAttributes(dpy, RootInfo[i].root, CWEventMask, &attributes);
	XSync(dpy, False);
	if (errorStatus == True) {
	    fprintf(stderr, "Are you running another window manager?\n");
	    exit(1);
	}
	if (XQueryTree(dpy, RootInfo[i].root, &RootInfo[i].root, &parent,
		       &children, &nchildren)) {
	    int         i;

	    for (i = 0; i < nchildren; i++) {
		if (MappedNotOverride(children[i])) {
		    AddGizmos(children[i], True);
		}
	    }
	    if (children)
		XFree((char *)children);
	}
	XFlush(dpy);
    }
    XSetErrorHandler(ErrorFunc);
    while (1) {
	XEvent      ev;
	XNextEvent(dpy, &ev);
#ifdef debug
	printf("event %s ", eventtype[ev.type]);
#endif
	switch (ev.type) {
#ifdef	notdef
	case CreateNotify:{
		Window      w;
		GenericAssoc *ga;
		w = ev.xmaprequest.window;
		ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, (XID) w);
		/* XXX - should do something with size info in CreateNotify */
		if (ga) {
#ifdef	debug
		    printf("already known window 0x%x created!\n", w);
#endif
		}
		else {
#ifdef debug
		    printf("delivered to %s of frame 0x%x\n",
			   WindowType(w), Frame(w));
#endif
		}
		break;
	}
#endif
	case MapRequest:{
		Window      w;
		GenericAssoc *ga;
		w = ev.xmaprequest.window;
		ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, (XID) w);
		if (ga) {
		    XMapWindow(dpy, ga->frame);
#ifdef debug
		    printf("Frame mapped %s for frame 0x%x\n",
			   WindowType(w), Frame(w));
#endif
		}
		else {
		    AddGizmos(w, False);
		    ga = (GenericAssoc *)
			XLookUpAssoc(dpy, frameInfo, (XID) w);
		    if (ga) {
#ifdef debug
		    	printf("window initially mapped %s of frame 0x%x\n",
			   WindowType(w), Frame(w));
#endif
		    } else {
			XMapWindow(dpy, w);
		    }
		}
		break;
	    }
	case UnmapNotify:{
		GenericAssoc *ga;
		Window      w;
		w = ev.xunmap.window;
		ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, (XID) w);
		if (!ga) {
#ifdef debug
		    printf("Window 0x%x unmapped\n", w);
#endif
		    break;
		}
#ifdef debug
		printf("delivered to %s of frame 0x%x\n",
		       WindowType(w), Frame(w));
#endif
		if (ga->client == w) {
#ifdef	debug
		    printf("Unmap the frame\n");
#endif
		    XUnmapWindow(dpy, ga->frame);
		}
		break;
	    }
	case MapNotify:{
		GenericAssoc *ga;
		Window      w;
		w = ev.xmap.window;
#ifdef debug
		printf("delivered to %s of frame 0x%x\n",
		       WindowType(w), Frame(w));
#endif
		ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, (XID) w);
		if (ga && (ga->client == w)) {
#ifdef	debug
		    printf("Map the frame\n");
#endif
		    XMapWindow(dpy, ga->frame);
		}
		break;
	    }
	case DestroyNotify:{
		GenericAssoc *ga;
		Window      w;
		w = ev.xdestroywindow.window;
		ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, (XID) w);
		if (!ga) {
#ifdef debug
		    printf("Window 0x%x destroyed\n", w);
#endif
		    break;
		}
#ifdef debug
		printf("delivered to %s of frame 0x%x\n",
		       WindowType(w), Frame(w));
#endif
		if (ga->client == w)
		    RemoveGizmos(ga);
		break;
	    }
	case PropertyNotify:{
		GenericAssoc *ga;
		Window      w;

		w = ev.xproperty.window;
		ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, (XID) w);
		if (!ga || w != ga->client /* || ev.xproperty.state == Deleted */)
		    break;
		switch (ev.xproperty.atom) {
		case XA_WM_NAME:
		    ProcessNewName(ga->root, ga, w);
		    break;
		case XA_WM_ICON_NAME:
		    ProcessNewIconName(ga->root, ga, w);
		    break;
		case XA_WM_NORMAL_HINTS:
		    ProcessNewSizeHints(ga->root, ga, XA_WM_NORMAL_HINTS, w);
		    break;
		case XA_WM_HINTS:
		    ProcessNewWMHints(ga->root, ga, w);
		    break;
		case XA_WM_COMMAND:
		    ProcessNewCommand(ga->root, ga, w);
		    break;
		case XA_WM_CLASS:
		    ProcessNewClass(ga->root, ga, w);
		    break;
		case XA_WM_TRANSIENT_FOR:
		/* XXX - do something */
		default:
		    break;
		}
		break;
	    }
	case ButtonPress:{
		GenericAssoc *ga;
		Window      w;
		w = ev.xbutton.window;
#ifdef debug
		printf("delivered to %s of frame 0x%x\n",
		       WindowType(w), Frame(w));
#endif
		if (buttonsDown[ev.xbutton.button])
		   break;
		buttonsDown[ev.xbutton.button] = 1;
		if (buttonsPressed++) {
		    iconifyWindow = None;
		    dragWindow = None;
		    resizeWindow = None;
		    MoveOutline(ev.xbutton.root,
				ev.xbutton.x_root - dragx - BORDERWIDTH,
				ev.xbutton.y_root - dragy - BORDERWIDTH,
				0, 0);
		    XUngrabServer(dpy);
		    break;
		}
		ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, (XID) w);
		if (ga && (ga->rbox == w) && resizeWindow == None) {
		    Window      junkRoot;
		    int         junkbw, junkDepth;
		    resizeWindow = ga->frame;
		    XGrabServer(dpy);
		    XGetGeometry(dpy, (Drawable) resizeWindow, &junkRoot,
			&dragx, &dragy, &dragWidth, &dragHeight, &junkbw,
				 &junkDepth);
		    dragx += BW;
		    dragy += BW;
		    origx = dragx;
		    origy = dragy;
		    origWidth = dragWidth;
		    origHeight = dragHeight;
		    clampTop = clampBottom = clampLeft = clampRight = 0;
		}
		else if (ga && (ga->iconWindow == w)) {
		    iconifyWindow = w;
		    if (ev.xbutton.state & ShiftMask) {
			Window      junkRoot;
			int         junkX, junkY, junkbw, junkDepth;
			dragWindow = w;
			dragx = ev.xbutton.x;
			dragy = ev.xbutton.y;
			XGrabServer(dpy);
			XGetGeometry(dpy, (Drawable) dragWindow,
				     &junkRoot, &junkX,
				&junkY, &dragWidth, &dragHeight, &junkbw,
				     &junkDepth);
#ifdef debug
			printf("From: x=%d, y=%d, w=%d, h=%d\n",
			       junkX, junkY, dragWidth, dragHeight);
#endif
			MoveOutline(ev.xbutton.root,
				 ev.xbutton.x_root - dragx - BORDERWIDTH,
				 ev.xbutton.y_root - dragy - BORDERWIDTH,
				    dragWidth + 2 * BORDERWIDTH,
				    dragHeight + 2 * BORDERWIDTH);
		    }
		}
		else if (ga && (ga->upbox == w)) {
		    XWindowChanges wc;
		    wc.stack_mode = Opposite;
		    XConfigureWindow(dpy, ga->frame, CWStackMode, &wc);
		}
		else if (ga && (ga->iconify == w) && (iconifyWindow == None)) {
		    Window      junkRoot;
		    int         junkX, junkY, junkbw, junkDepth;
		    iconifyWindow = w;
		    iconPressx = ev.xbutton.x_root;
		    iconPressy = ev.xbutton.y_root;
		    dragx = 0;
		    dragy = 0;
		    dragWindow = ga->iconWindow;
		    XGrabServer(dpy);
		    XGetGeometry(dpy, (Drawable) dragWindow,
				 &junkRoot, &junkX,
				 &junkY, &dragWidth, &dragHeight, &junkbw,
				 &junkDepth);
#ifdef debug
		    printf("From: x=%d, y=%d, w=%d, h=%d\n",
			   junkX, junkY, dragWidth, dragHeight);
#endif
		    MoveOutline(ev.xbutton.root,
				ev.xbutton.x_root - dragx - BORDERWIDTH,
				ev.xbutton.y_root - dragy - BORDERWIDTH,
				dragWidth + 2 * BORDERWIDTH,
				dragHeight + 2 * BORDERWIDTH);
		}
		else if (ga && (ga->header == w) && dragWindow == None) {
		    Window      junkRoot;
		    int         junkX, junkY, junkbw, junkDepth;
		    dragWindow = ga->frame;
		    dragx = ev.xbutton.x + UPBOXWIDTH + ICONIFYWIDTH + BORDERWIDTH;
		    dragy = ev.xbutton.y;
		    hdrWidth = ga->headerWidth;
		    XGrabServer(dpy);
		    XGetGeometry(dpy, (Drawable) dragWindow, &junkRoot, &junkX,
				 &junkY, &dragWidth, &dragHeight, &junkbw,
				 &junkDepth);
#ifdef debug
		    printf("From: x=%d, y=%d, w=%d, h=%d\n",
			   junkX, junkY, dragWidth, dragHeight);
#endif
		    MoveOutline(ev.xbutton.root,
				ev.xbutton.x_root - dragx - BORDERWIDTH,
				ev.xbutton.y_root - dragy - BORDERWIDTH,
				dragWidth + 2 * BORDERWIDTH,
				dragHeight + 2 * BORDERWIDTH);
		}
		else {
		    int newx, newy;
		    Window newRoot;
		    RootInfoRec *ro = findRootInfo(ev.xbutton.root);
		    RootInfoRec *rn = ro;

		    if (++rn >= (RootInfo + ScreenCount(dpy)))
			rn = RootInfo;
#ifdef	debug
		    printf("Roots 0x%x: old 0x%x, new 0x%x\n", ev.xbutton.root, ro, rn);
#endif
		    if (rn != ro) {
#ifdef	debug
			printf("Warp from %d to %d new root is 0x%x\n",
			       (ro - RootInfo), (rn - RootInfo), rn->root);
#endif
		        newx = (((ev.xbutton.x_root - ro->rootx)*100/(ro->rootwidth))
			    *(rn->rootwidth))/100 + rn->rootx;
		        newy = (((ev.xbutton.y_root - ro->rooty)*100/(ro->rootheight))
			    *(rn->rootheight))/100 + rn->rooty;
		        XWarpPointer(dpy, None, rn->root, 0, 0, 0, 0, newx, newy);
		    }
		}
		break;
	    }
	case MotionNotify:{
		Window      junkRoot, junkChild;
		int         junkx, junky, junkrx, junkry;
		unsigned int junkMask;
		Window 	    root;
#ifdef debug
		printf("\n");
#endif
		if (QLength(dpy)) {
		    XEvent      rete;
		    XPeekEvent(dpy, &rete);
		    if (rete.type == MotionNotify &&
			rete.xmotion.window == ev.xmotion.window) {
			/* OK not to call QueryPointer() - we'll be back */
			break;
		    }
		}
		root = ev.xmotion.root;
		if (dragWindow != None) {
		    int         x, y;
		    x = ev.xmotion.x_root - dragx;
		    y = ev.xmotion.y_root - dragy;
		    if (iconifyWindow != None)
			MakeReachable(root, &x, &y, dragWidth,
				      dragHeight);
		    else {
			x += UPBOXWIDTH + ICONIFYWIDTH + BW;
			MakeReachable(root, &x, &y, hdrWidth,
				      TITLEHEIGHT);
			x -= UPBOXWIDTH + ICONIFYWIDTH + BW;
		    }
		    MoveOutline(root,
				x - BORDERWIDTH, y - BORDERWIDTH,
				dragWidth + 2 * BORDERWIDTH,
				dragHeight + 2 * BORDERWIDTH);
		}
		else if (resizeWindow != None) {
		    int         action = 0;
		    if (clampTop) {
			int         delta = ev.xmotion.y_root - dragy;
			if (dragHeight - delta < MINHEIGHT) {
			    delta = dragHeight - MINHEIGHT;
			    clampTop = 0;
			}
			dragy += delta;
			dragHeight -= delta;
			action = 1;
		    }
		    else if (ev.xmotion.y_root <= dragy ||
			     ev.xmotion.y_root == findRootInfo(root)->rooty) {
			dragy = ev.xmotion.y_root;
			dragHeight = origy + origHeight -
			    ev.xmotion.y_root;
			clampBottom = 0;
			clampTop = 1;
			action = 1;
		    }
		    if (clampLeft) {
			int         delta = ev.xmotion.x_root - dragx;
			if (dragWidth - delta < MINWIDTH) {
			    delta = dragWidth - MINWIDTH;
			    clampLeft = 0;
			}
			dragx += delta;
			dragWidth -= delta;
			action = 1;
		    }
		    else if (ev.xmotion.x_root <= dragx ||
			     ev.xmotion.x_root == findRootInfo(root)->rootx) {
			dragx = ev.xmotion.x_root;
			dragWidth = origx + origWidth -
			    ev.xmotion.x_root;
			clampRight = 0;
			clampLeft = 1;
			action = 1;
		    }
		    if (clampBottom) {
			int         delta = ev.xmotion.y_root - dragy - dragHeight;
			if (dragHeight + delta < MINHEIGHT) {
			    delta = MINHEIGHT - dragHeight;
			    clampBottom = 0;
			}
			dragHeight += delta;
			action = 1;
		    }
		    else if (ev.xmotion.y_root >= dragy + dragHeight - 1 ||
			   ev.xmotion.y_root == findRootInfo(root)->rooty
			   + findRootInfo(root)->rootheight - 1) {
			dragy = origy;
			dragHeight = 1 + ev.xmotion.y_root - dragy;
			clampTop = 0;
			clampBottom = 1;
			action = 1;
		    }
		    if (clampRight) {
			int         delta = ev.xmotion.x_root - dragx - dragWidth;
			if (dragWidth + delta < MINWIDTH) {
			    delta = MINWIDTH - dragWidth;
			    clampRight = 0;
			}
			dragWidth += delta;
			action = 1;
		    }
		    else if (ev.xmotion.x_root >= dragx + dragWidth - 1 ||
			     ev.xmotion.x_root == findRootInfo(root)->rootx +
			     findRootInfo(root)->rootwidth - 1) {
			dragx = origx;
			dragWidth = 1 + ev.xmotion.x_root - origx;
			clampLeft = 0;
			clampRight = 1;
			action = 1;
		    }
		    if (action) {
			MoveOutline(root,
				    dragx - BORDERWIDTH,
				    dragy - BORDERWIDTH,
				    dragWidth + 2 * BORDERWIDTH,
				    dragHeight + 2 * BORDERWIDTH);
		    }
		}
		XQueryPointer(dpy, root, &junkRoot, &junkChild,
				&junkrx, &junkry,
				&junkx, &junky, &junkMask);
		break;
	    }
	case ButtonRelease:{
		Window      w;
		GenericAssoc *ga;
		if (!buttonsDown[ev.xbutton.button])
		   break;
		buttonsDown[ev.xbutton.button] = 0;
		buttonsPressed--;
		w = ev.xbutton.window;
		ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo,
						   (XID) w);

		if (ga && (ga->header == w)
		    && (ga->frame == dragWindow)) {
		    XWindowChanges wc;
		    wc.x = ev.xbutton.x_root - dragx;
		    wc.y = ev.xbutton.y_root - dragy;
		    wc.x += UPBOXWIDTH + ICONIFYWIDTH + BW;
		    MakeReachable(ev.xbutton.root, &wc.x, &wc.y, ga->headerWidth,
				  TITLEHEIGHT);
		    wc.x -= UPBOXWIDTH + ICONIFYWIDTH + 2 * BW;
		    /*
		     * the extra BW is because XReconfigureWindow includes
		     * the border width in x and y, but not in width and
		     * height 
		     */
		    wc.y -= BW;
#ifdef debug
		    printf("delivered to %s of frame 0x%x\n",
			   WindowType(w), Frame(w));
#endif
		    MoveOutline(ev.xbutton.root, 0, 0, 0, 0);
		    XUngrabServer(dpy);
#ifdef debug
		    printf("To x=%d, y=%d\n",
			   ev.xbutton.x_root - dragx,
			   ev.xbutton.y_root - dragy);
#endif
		    XConfigureWindow(dpy, dragWindow,
				       CWX | CWY, &wc);
		    dragWindow = None;
		}
		else if (ga && (ga->rbox == w)
			 && (ga->frame == resizeWindow)) {
#ifdef debug
		    printf("delivered to %s of frame 0x%x\n",
			   WindowType(w), Frame(w));
#endif
		    MoveOutline(ev.xbutton.root, 0, 0, 0, 0);
		    XUngrabServer(dpy);
		    ReconfigureFrameAndClient(ev.xbutton.root, ga, dragx,
			dragy + TITLEHEIGHT + BORDERWIDTH, dragWidth,
				 dragHeight - TITLEHEIGHT - BORDERWIDTH);
		    resizeWindow = None;
		}
		else if (ga && (ga->iconify == w) && (iconifyWindow == w)) {
		    XWindowChanges wc;
#ifdef debug
		    printf("delivered to %s of frame 0x%x\n",
			   WindowType(w), Frame(w));
#endif
		    if (ga->iconknown && iconPressx == ev.xbutton.x_root
			&& iconPressy == ev.xbutton.y_root) {
			wc.x = ga->iconx;
			wc.y = ga->icony;
		    }
		    else {
			ga->iconknown = True;
			wc.x = ga->iconx = ev.xbutton.x_root;
			wc.y = ga->icony = ev.xbutton.y_root;
		    }
		    MakeReachable(ev.xbutton.root, &wc.x, &wc.y,
				  dragWidth, dragHeight);
		    wc.x -= BW;
		    wc.y -= BW;
		    MoveOutline(ev.xbutton.root, 0, 0, 0, 0);
		    XUngrabServer(dpy);
		    dragWindow = None;
		    iconifyWindow = None;
		    XConfigureWindow(dpy, ga->iconWindow, CWX | CWY, &wc);
		    XUnmapWindow(dpy, ga->frame);
#ifdef	debug
		    printf("Unmap the frame 0x%x\n", ga->frame);
#endif
		    XMapWindow(dpy, ga->iconWindow);
		}
		else if (ga && (ga->iconWindow == w) && (iconifyWindow == w)) {
		    if (dragWindow == w) {
			XWindowChanges wc;
			wc.x = ev.xbutton.x_root - dragx;
			wc.y = ev.xbutton.y_root - dragy;
#ifdef debug
			printf("delivered to %s of frame 0x%x\n",
			       WindowType(w), Frame(w));
#endif
			MoveOutline(ev.xbutton.root, 0, 0, 0, 0);
			XUngrabServer(dpy);
			ga->iconx = wc.x;
			ga->icony = wc.y;
#ifdef debug
			printf("To x=%d, y=%d\n",
			       ev.xbutton.x_root - dragx,
			       ev.xbutton.y_root - dragy);
#endif
			MakeReachable(ev.xbutton.root, &wc.x, &wc.y, dragWidth,
				      dragHeight);
			wc.x -= BW;
			wc.y -= BW;
			XConfigureWindow(
					dpy, dragWindow, CWX | CWY, &wc);
			dragWindow = None;
		    }
		    else {
			XUnmapWindow(dpy, ga->iconWindow);
			XMapWindow(dpy, ga->frame);
		    }
		    iconifyWindow = None;
		}
		else {
#ifdef debug
		    printf("delivered to ???\n");
#endif
		}
		break;
	    }
	case EnterNotify:{
		Window      w;
		GenericAssoc *ga;
		XEnterWindowEvent evc;
		evc = ev.xcrossing;
		w = evc.window;
		ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, (XID) w);
		if (!ga) {
#ifdef debug
		    printf("enter root - set input focus\n");
#endif
		    XSetInputFocus(dpy, PointerRoot, NULL, CurrentTime);
		}
		else if (ga->frame == w) {
#ifdef debug
		    printf("delivered to %s of frame 0x%x install 0x%x\n",
			   WindowType(w), Frame(w), ga->ColorMap);
#endif
		    if (ga->ColorMap)
			XInstallColormap(dpy, ga->ColorMap);
		}
		else {
#ifdef debug
		    printf("Not delivered to %s of frame 0x%x\n",
			   WindowType(w), Frame(w));
#endif
		}
		break;
	    }
	case ColormapNotify:{
		Window      w;
		GenericAssoc *ga;
		XColormapEvent evc;
		evc = ev.xcolormap;
		w = evc.window;
		ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, (XID) w);
		if (!ga) {
#ifdef debug
		    printf("delivered to unknown client\n");
#endif
		}
		else if (ga->frame == w) {
#ifdef debug
		    printf("delivered to %s of frame 0x%x\n",
			   WindowType(w), Frame(w));
#endif
		    if (evc.new) {
			if (evc.colormap == None)
			    ga->ColorMap = NULL;
			else
			    ga->ColorMap = evc.colormap;
		    }
		}
		else {
#ifdef debug
		    printf("Not delivered to %s of frame 0x%x\n",
			   WindowType(w), Frame(w));
#endif
		}
		break;
	    }
	case ConfigureRequest:{
		Window      w;
		GenericAssoc *ga;
		XWindowChanges wc;
		XConfigureRequestEvent evc;
		evc = ev.xconfigurerequest;
		w = evc.window;
		ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, (XID) w);
		if (!ga) {
#ifdef debug
		    printf("delivered to unknown client\n");
#endif
		    wc.x = evc.x;
		    wc.y = evc.y;
		    wc.width = evc.width;
		    wc.height = evc.height;
		    wc.border_width = evc.border_width;
		    wc.sibling = evc.above;
		    wc.stack_mode = Above;
		    XConfigureWindow(dpy, w, CWX | CWY | CWWidth |
				       CWBorderWidth | CWSibling | CWStackMode,
				       &wc);
		}
		else if (ga->client == w) {
		    int         x, y;
		    Window      jc;
		    int         ht, wd, bw, d, x1, y1;
#ifdef debug
		    printf("delivered to %s of frame 0x%x\n",
			   WindowType(w), Frame(w));
#endif
		    XTranslateCoordinates(dpy, w, ga->root, 0, 0, &x, &y, &jc);
		    ReconfigureFrameAndClient
			(ga->root, ga, x, y, evc.width, evc.height);
		}
		else {
#ifdef debug
		    printf("Not delivered to %s of frame 0x%x\n",
			   WindowType(w), Frame(w));
#endif
		}
		break;
	    }
	case Expose:{
		Window      w = ev.xexpose.window;
		GenericAssoc *ga;

#ifdef debug
		printf("delivered to %s of frame 0x%x\n",
		       WindowType(w), Frame(w));
#endif
		ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, (XID) w);
		if (ga && ga->header == w) {
		    int         x;
		    x = (ga->headerWidth -
			 XTextWidth(font, ga->name, ga->namelen)) / 2;
		    XDrawString(dpy, w, findRootInfo(ga->root)->headerGC, x,
			  font->max_bounds.ascent + 5,
			  ga->name, ga->namelen);
		}
		else if (ga && ga->iconWindow == w) {
		    if (ga->ownIcon)
			break;
		    if (ga->iconnamelen == 0)
			XDrawString(dpy, w, findRootInfo(ga->root)->headerGC, 2,
			      font->max_bounds.ascent + 5,
			      ga->name, ga->namelen);
		    else
			XDrawString(dpy, w, findRootInfo(ga->root)->headerGC, 2,
			      font->max_bounds.ascent + 5,
			      ga->iconname, ga->iconnamelen);
		}		/* else if (ga && (ga->upbox == w)) {
				 * XCopyArea(dpy, doubleArrow, w,
				 * headerGC, 0, 0, UPBOXWIDTH,
				 * TITLEHEIGHT, 0, 0); } else if (ga &&
				 * (ga->rbox == w)) { XCopyArea(dpy,
				 * growBox, w, headerGC, 0, 0, UPBOXWIDTH,
				 * TITLEHEIGHT, 0, 0); } else if (ga &&
				 * (ga->iconify == w)) { XCopyArea(dpy,
				 * cross, w, headerGC, 0, 0, UPBOXWIDTH,
				 * TITLEHEIGHT, 0, 0); } */
		break;
	    }
	case FocusIn: break;    /* just ignore */
	case FocusOut: {
		if (ev.xfocus.detail == NotifyPointerRoot)
		    XSetInputFocus(dpy, PointerRoot, None, CurrentTime);
		break;
	    }
	default:
#ifdef debug
	    printf("\n");
#endif
	    ;
	}
    }
}

int
ErrorFunc(dpy, event)
	Display *dpy;
	XErrorEvent *event;
{
    switch (event->error_code) {
	case BadWindow:
	case BadDrawable:
		(void) fprintf(stderr, "wm: Window #0x%x disappeared!\n",
			event->resourceid);
		break;
	default:
		_XDefaultError(dpy, event);
    }
    return (0);
}

Bool
MappedNotOverride(w) Window w;
{
XWindowAttributes wa;
	XGetWindowAttributes(dpy, w, &wa);
	return ((wa.map_state != IsUnmapped) && (wa.override_redirect != True));
}

static void
MoveOutline(root, x, y, width, height)
    Window root;
    int x, y, width, height;
{
    static int  lastx = 0;
    static int  lasty = 0;
    static int  lastWidth = 0;
    static int  lastHeight = 0;
    XRectangle  outline[8];
    XRectangle *r = outline;

    if (x == lastx && y == lasty && width == lastWidth && height == lastHeight)
	return;
    if (lastWidth || lastHeight) {
	r->x = lastx;
	r->y = lasty;
	r->width = lastWidth;
	r++->height = BORDERWIDTH;
	r->x = lastx;
	r->y = lasty + lastHeight - BORDERWIDTH;
	r->width = lastWidth;
	r++->height = BORDERWIDTH;
	r->x = lastx;
	r->y = lasty + BORDERWIDTH;
	r->width = BORDERWIDTH;
	r++->height = lastHeight - 2 * BORDERWIDTH;
	r->x = lastx + lastWidth - BORDERWIDTH;
	r->y = lasty + BORDERWIDTH;
	r->width = BORDERWIDTH;
	r++->height = lastHeight - 2 * BORDERWIDTH;
    }
    lastx = x;
    lasty = y;
    lastWidth = width;
    lastHeight = height;
    if (lastWidth || lastHeight) {
	r->x = lastx;
	r->y = lasty;
	r->width = lastWidth;
	r++->height = BORDERWIDTH;
	r->x = lastx;
	r->y = lasty + lastHeight - BORDERWIDTH;
	r->width = lastWidth;
	r++->height = BORDERWIDTH;
	r->x = lastx;
	r->y = lasty + BORDERWIDTH;
	r->width = BORDERWIDTH;
	r++->height = lastHeight - 2 * BORDERWIDTH;
	r->x = lastx + lastWidth - BORDERWIDTH;
	r->y = lasty + BORDERWIDTH;
	r->width = BORDERWIDTH;
	r++->height = lastHeight - 2 * BORDERWIDTH;
    }
    if (r != outline) {
	XFillRectangles(dpy, root, findRootInfo(root)->xorGC, outline, r - outline);
    }
}
		
void
AddGizmos(w, mapit)
    Window w;
    Bool mapit;
{
    XWindowAttributes wa;
    XSetWindowAttributes swa;
    Window      root, fw, gw, lw, uw, iw, iconw, hpw, junkw;
    int         hw;
    Bool        resize = False;
    int		x, y, wd, h, bw, d;

#ifdef	debug
    printf("AddGizmos(0x%x, 0x%x)\n", w, mapit);
#endif
    XGetWindowAttributes(dpy, w, &wa);
    /*
     * Do we need to notice this guy?
     */
    if (wa.class == InputOnly || wa.override_redirect)
	return;
    if (XGetTransientForHint(dpy, w, &junkw) != 0)
	return;

    /*
     * When we reparent w to fw, below, make sure w gets reparented back when
     * fw goes away.  We do this early, in case we die, otherwise the map
     * request in the client will hang forever. 
     */
    XChangeSaveSet(dpy, w, SetModeInsert);

    /* set the border of the original window to 0 - XXX should put this back*/
    ChangeBorderWidth(w, 0);
    /* make sure this window is big enough */
    if (wa.width < MINWIDTH) {
	resize = True;
	wa.width = MINWIDTH;
    }
    if (wa.height + TITLEHEIGHT + 2 * BW < MINHEIGHT) {
	resize = True;
	wa.height = MINHEIGHT;
    }
    if (resize == True) {
	XResizeWindow(dpy, w, wa.width, wa.height);
    }
#ifndef	notdef
    /* Listen for property changes on the window - esp WM_HINTS */
    swa.event_mask = (PropertyChangeMask);
    XChangeWindowAttributes(dpy, w, CWEventMask, &swa);
#endif
    /* MakeFrame makes sure part of the header is on-screen */
    fw = MakeFrame(wa.root, wa.x, wa.y - TITLEHEIGHT - BW, wa.width,
		   wa.height + TITLEHEIGHT + BW);
#ifdef	debug
    printf ("Window 0x%x is a Frame\n", fw);
#endif
    if (!fw)
	return;
    hw = wa.width - GROWBOXWIDTH - UPBOXWIDTH - ICONIFYWIDTH - 2 * BW;
    hpw = MakeHeaderParent(wa.root, fw, hw);
#ifdef	debug
    printf ("Window 0x%x is a HeaderParent\n", hpw);
#endif
    gw = MakeHeader(wa.root, hpw, hw);
#ifdef	debug
    printf ("Window 0x%x is a Header\n", gw);
#endif
    lw = MakeGrowBox(wa.root, fw, wa.width - GROWBOXWIDTH);
#ifdef	debug
    printf ("Window 0x%x is a GrowBox\n", lw);
#endif
    iw = MakeIconBox(wa.root, fw, UPBOXWIDTH);
#ifdef	debug
    printf ("Window 0x%x is a IconBox\n", iw);
#endif
    uw = MakeUpBox(wa.root, fw);
#ifdef	debug
    printf ("Window 0x%x is a UpBox\n", uw);
#endif
    iconw = MakeIcon(wa.root, fw);
#ifdef	debug
    printf ("Window 0x%x is an Icon\n", iconw);
#endif
    XSync(dpy, False);
    errorStatus = False;
    {
	XWindowChanges wc;

	wc.sibling = w;
	wc.stack_mode = Above;
	XConfigureWindow(dpy, fw, CWSibling|CWStackMode, &wc);
    }
    /*  This will cause an UnmapNotify and a MapNotify on the window */
    XReparentWindow(dpy, w, fw, 0, TITLEHEIGHT + BW);
    XSync(dpy, False);
    if (errorStatus == True) {
	XDestroyWindow(dpy, fw);
	return;
    }
    /* register fw, w, gw, and lw in the association table */
    RegisterCompleteWindow(wa.root, fw, w, gw, hpw, lw, iw, uw, iconw, hw,
			   wa.colormap);
    /* map the subwindows - wait for the application to map the real window */
    XMapSubwindows(dpy, fw);
    if (mapit) {
	XMapWindow(dpy, fw);
    }
}

void
ReconfigureFrameAndClient(root, ga, x, y, width, height)
    Window root;
	GenericAssoc *ga;
	int x, y, width, height;
{
    XWindowAttributes wa;
    XWindowChanges wc;
    unsigned long mask;

    if (width < MINWIDTH)
	width = MINWIDTH;
    if (height < TITLEHEIGHT)
	height = TITLEHEIGHT;

    /* Reconfigure client */
    wc.x = 0;
    wc.y = TITLEHEIGHT + BW;
    wc.width = width;
    wc.height = height;
    wc.border_width = 0;
    mask = CWX | CWY | CWWidth | CWHeight | CWBorderWidth;
    XConfigureWindow(dpy, ga->client, mask, &wc);
    /* Reconfigure raise button */
    wc.x = 0;
    wc.y = 0;
    wc.width = UPBOXWIDTH;
    wc.height = TITLEHEIGHT;
    mask = CWX | CWY | CWWidth | CWHeight;
    XConfigureWindow(dpy, ga->upbox, mask, &wc);
    /* Reconfigure header */
    wc.x = 0;
    wc.y = 0;
    wc.width = width - UPBOXWIDTH - GROWBOXWIDTH - 2 * BW - ICONIFYWIDTH;
    wc.height = TITLEHEIGHT;
    mask = CWX | CWY | CWWidth | CWHeight;
    XConfigureWindow(dpy, ga->header, mask, &wc);
    /* Reconfigure resize button */
    wc.x = width - GROWBOXWIDTH;
    wc.y = 0;
    wc.width = GROWBOXWIDTH;
    wc.height = TITLEHEIGHT;
    mask = CWX | CWY | CWWidth | CWHeight;
    XConfigureWindow(dpy, ga->rbox, mask, &wc);
    /* Reconfigure iconify button */
    wc.x = UPBOXWIDTH;
    wc.y = 0;
    wc.width = ICONIFYWIDTH;
    wc.height = TITLEHEIGHT;
    mask = CWX | CWY | CWWidth | CWHeight;
    XConfigureWindow(dpy, ga->iconify, mask, &wc);
    /* Reconfigure header parent */
    wc.x = UPBOXWIDTH + BW + ICONIFYWIDTH;
    wc.y = 0;
    wc.width = width - UPBOXWIDTH - GROWBOXWIDTH - 2 * BW - ICONIFYWIDTH;
    ga->headerWidth = wc.width;
    wc.height = TITLEHEIGHT;
    mask = CWX | CWY | CWWidth | CWHeight;
    XConfigureWindow(dpy, ga->headerParent, mask, &wc);
    /* Reconfigure frame */
    wc.x = x + UPBOXWIDTH + ICONIFYWIDTH + BW;
    wc.y = y - TITLEHEIGHT - BW;
    MakeReachable(root, &wc.x, &wc.y, width - UPBOXWIDTH - GROWBOXWIDTH -
		  ICONIFYWIDTH - 2 * BW, TITLEHEIGHT);
    wc.x -= UPBOXWIDTH + ICONIFYWIDTH + 2 * BW;
    /* the extra BW is because XReconfigureWindow includes the border */
    /* width in x and y, but not in width and height */
    wc.y -= BW;
    wc.width = width;
    wc.height = height + TITLEHEIGHT + BW;
    mask = CWX | CWY | CWWidth | CWHeight;
    XConfigureWindow(dpy, ga->frame, mask, &wc);
}

Window
MakeGrowBox(root, fw, lx)
    Window root;
    Window fw;
    int lx;
{
    unsigned long valuemask;
    XSetWindowAttributes attributes;
    RootInfoRec *ri = findRootInfo(root);
    int scr = ri - RootInfo;

    valuemask = CWEventMask | CWBackPixmap;
    attributes.background_pixmap = ri->growBox;
    attributes.event_mask =
	ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
	PointerMotionHintMask | ExposureMask;
    return XCreateWindow(dpy, fw, lx, 0, GROWBOXWIDTH, TITLEHEIGHT, 0,
		   DefaultDepth(dpy, scr), CopyFromParent,
		   DefaultVisual(dpy, scr), valuemask, &attributes);
}

Window
MakeUpBox(root, fw)
    Window root;
    Window fw;
{
    unsigned long valuemask;
    XSetWindowAttributes attributes;
    RootInfoRec *ri = findRootInfo(root);
    int scr = ri - RootInfo;

    valuemask = CWEventMask | CWBackPixmap;
    attributes.background_pixmap = ri->doubleArrow;
    attributes.event_mask =
	ButtonPressMask | ButtonReleaseMask | ExposureMask;
    return XCreateWindow(dpy, fw, 0, 0, UPBOXWIDTH, TITLEHEIGHT, 0,
	     DefaultDepth(dpy, scr), CopyFromParent, DefaultVisual(dpy, scr),
		   valuemask, &attributes);
}

Window
MakeIconBox(root, fw, ix)
    Window root;
    Window fw;
    int ix;
{
    unsigned long valuemask;
    XSetWindowAttributes attributes;
    RootInfoRec *ri = findRootInfo(root);
    int         scr = ri - RootInfo;

    valuemask = CWEventMask | CWBackPixmap;
    attributes.background_pixmap = ri->cross;
    attributes.event_mask =
	ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
	PointerMotionHintMask | ExposureMask;
    return XCreateWindow(dpy, fw, ix, 0, UPBOXWIDTH, TITLEHEIGHT, 0,
	     DefaultDepth(dpy, scr), CopyFromParent, DefaultVisual(dpy, scr),
		   valuemask, &attributes);
}
	
void
ChangeBorderWidth(w, newWidth) Window w; int newWidth;
{
XWindowChanges wch;
	wch.border_width = newWidth;
	XConfigureWindow(dpy, w, CWBorderWidth, &wch);
}

void
MapCompleteWindow(frame) Window frame;
{
}

void
RegisterCompleteWindow(root, frame, client, header, hp, rbox, iconify, upbox, iconw, headerWidth, cmap)
	Window root, frame, client, header, hp, rbox, upbox, iconify, iconw;
	int headerWidth;
	Colormap cmap;
{
    GenericAssoc *fa;

#define	CLIENT_EVENT_MASK (PropertyChangeMask | ColormapChangeMask)
    XSelectInput(dpy, client, CLIENT_EVENT_MASK);
    fa = GimmeAssocStruct();
    fa->frame = frame;
    fa->header = header;
    fa->headerParent = hp;
    fa->client = client;
    fa->rbox = rbox;
    fa->iconify = iconify;
    fa->upbox = upbox;
    fa->headerWidth = headerWidth;
    fa->iconWindow = iconw;
    fa->iconnamelen = fa->namelen = 0;
    fa->ColorMap = cmap;
    fa->root = root;
    fa->ownIcon = 0;
    XMakeAssoc(dpy, frameInfo, (XID) frame, (char *) fa);
    XMakeAssoc(dpy, frameInfo, (XID) header, (char *) fa);
    XMakeAssoc(dpy, frameInfo, (XID) hp, (char *) fa);
    XMakeAssoc(dpy, frameInfo, (XID) client, (char *) fa);
    XMakeAssoc(dpy, frameInfo, (XID) rbox, (char *) fa);
    XMakeAssoc(dpy, frameInfo, (XID) iconify, (char *) fa);
    XMakeAssoc(dpy, frameInfo, (XID) upbox, (char *) fa);
    XMakeAssoc(dpy, frameInfo, (XID) iconw, (char *) fa);
    ProcessNewIconName(root, fa, client);
    ProcessNewName(root, fa, client);
    ProcessNewSizeHints(root, fa, XA_WM_NORMAL_HINTS, client);
    ProcessNewWMHints(root, fa, client);
    ProcessNewCommand(root, fa, client);
    ProcessNewClass(root, fa, client);
}

Window
MakeHeaderParent(root, fw, width)
    Window root;
    Window fw;
    int width;
{
    unsigned long valuemask;
    XSetWindowAttributes attributes;
    Window      result;
    int scr = findRootInfo(root) - RootInfo;

    valuemask = CWBackPixel;
    attributes.background_pixel = RootInfo[scr].background;
    result = XCreateWindow(dpy, fw, UPBOXWIDTH + ICONIFYWIDTH + BW, 0,
		     width, TITLEHEIGHT, 0,
		     DefaultDepth(dpy, scr), CopyFromParent,
		     DefaultVisual(dpy, scr), valuemask,
		     &attributes);
    return result;
}

Window
MakeHeader(root, hw, width)
    Window root;
    Window hw;
    int width;
{
    unsigned long    valuemask;
    XSetWindowAttributes attributes;
    Window      result;
    int         scr = findRootInfo(root) - RootInfo;

    valuemask = CWEventMask | CWBackPixel;
    attributes.event_mask =
	ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
	ExposureMask | PointerMotionHintMask;
    attributes.background_pixel = RootInfo[scr].background;
    result = XCreateWindow(dpy, hw, 0, 0, width, TITLEHEIGHT, 0,
		     DefaultDepth(dpy, scr), CopyFromParent,
		     DefaultVisual(dpy, scr), valuemask,
		     &attributes);
    return result;
}

/* XXX fw unused? */
MakeIcon(root, fw)
    Window root;
    Window fw;
{
    unsigned long  valuemask;
    XSetWindowAttributes attributes;
    Window      result;
    int         scr = findRootInfo(root) - RootInfo;

    valuemask = CWEventMask | CWBackPixel | CWBorderPixel;
#define	ICON_EVENT_MASK	\
	ButtonPressMask \
	| ButtonReleaseMask \
	| ButtonMotionMask \
	| ExposureMask \
	| PointerMotionHintMask

    attributes.event_mask = ICON_EVENT_MASK;
    attributes.background_pixel = RootInfo[scr].background;
    attributes.border_pixel = RootInfo[scr].foreground;
    result = XCreateWindow(dpy, root, 0, 0, ICONWIDTH, TITLEHEIGHT, BW,
	     DefaultDepth(dpy, scr), CopyFromParent, DefaultVisual(dpy, scr),
		     valuemask, &attributes);
    return result;
}


Window
MakeFrame(root, x, y, width, height)
    Window root;
	int x, y, width, height;
{
    unsigned long valuemask;
    XSetWindowAttributes attributes;
    int         hx, hy;
    int         scr;
    RootInfoRec *ri = findRootInfo(root);

    if (!ri)
	return (NULL);
    scr = ri - RootInfo;
    hx = x + ICONIFYWIDTH + UPBOXWIDTH + BW;
    hy = y;
    MakeReachable(root, &hx, &hy, width - UPBOXWIDTH - GROWBOXWIDTH - ICONIFYWIDTH
		  - 2 * BW, TITLEHEIGHT);
    x = hx - (ICONIFYWIDTH + UPBOXWIDTH + BW) - BW;
    y = hy - BW;
    valuemask = CWEventMask | CWBorderPixel | CWBackPixel;
#define	FRAME_EVENT_MASK	\
	(SubstructureRedirectMask | \
	 SubstructureNotifyMask | \
	 EnterWindowMask)
    attributes.event_mask = FRAME_EVENT_MASK;
    attributes.background_pixel = ri->background;
    attributes.border_pixel = ri->foreground;
    attributes.bit_gravity = NorthWestGravity;
    return XCreateWindow(dpy, root, x, y, width, height, BORDERWIDTH,
	     DefaultDepth(dpy, scr), CopyFromParent, DefaultVisual(dpy, scr),
		   valuemask, &attributes);
}

void
RemoveGizmos(ga) GenericAssoc *ga;
{
	XDeleteAssoc(dpy, frameInfo, ga->frame);
	XDeleteAssoc(dpy, frameInfo, ga->header);
	XDeleteAssoc(dpy, frameInfo, ga->headerParent);
	XDeleteAssoc(dpy, frameInfo, ga->rbox);
	XDeleteAssoc(dpy, frameInfo, ga->iconify);
	XDeleteAssoc(dpy, frameInfo, ga->upbox);
	XDeleteAssoc(dpy, frameInfo, ga->iconWindow);
	XDeleteAssoc(dpy, frameInfo, ga->client);
	XDestroyWindow(dpy, ga->frame);
	XDestroyWindow(dpy, ga->iconWindow);
	XFree((char *) ga);
}


StartConnectionToServer(argc, argv)
	int	argc;
	char	*argv[];
{
    char       *display;
    int         i;

    display = NULL;
    for (i = 1; i < argc; i++) {
	char *arg = argv[i];

	if (arg[0] == '-') {
	    switch (arg[1]) {
		case 'd':				/* -display host:dpy */
		    if (++i >= argc) goto usage;
		    display = argv[i];
		    continue;
		default:
		    goto usage;
	    }
	} else {
	  usage:
	    fprintf (stderr, "usage:  %s [-display host:dpy]\n\n",
		     ProgramName);
	    exit (1);
	}
    }
    if (!(dpy = XOpenDisplay(display))) {
	Error(1, "Cannot open display\n");
    }
}

Error(status, message) int status; char *message;
{
	printf("%s", message);
	if (!status) return;
	else exit(1);
}

GenericAssoc *
GimmeAssocStruct(){
	return (GenericAssoc *) malloc(sizeof(GenericAssoc));
}


char *
WindowType(w) Window w;
{
GenericAssoc *ga;
	ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, w);
	if (ga && w == ga->frame) return "frame";
	if (ga && w == ga->header) return "header";
	if (ga && w == ga->client) return "client";
	if (ga && w == ga->rbox) return "resize box";
	if (ga && w == ga->iconify) return "iconify box";
	if (ga && w == ga->upbox) return "raise box";
	if (ga && w == ga->iconWindow) return "icon";
	if (ga && w == ga->headerParent) return "header parent";
	if (ga) {
		Error(0, "WindowType: error in association table routines\n");
		return "error";
	}
	Error(0, "WindowType: window 0x%x not found in assoc table\n", w);
	return "error";
}

Window
Frame(w) Window w;
{
GenericAssoc *ga;
	ga = (GenericAssoc *) XLookUpAssoc(dpy, frameInfo, w);
	if (ga) return ga->frame;
	Error(0, "Frame: window 0x%x not found in assoc table\n", w);
	return None;
}

void
MakeReachable(root, x, y, width, height)
    Window root;
	int *x, *y;
	int width, height;
{
    RootInfoRec *ri = findRootInfo(root);

    if (!ri)
	return;
    if (*x <= MARGIN + ri->rootx - width)
	*x = MARGIN + ri->rootx - width + 1;
    if (*y <= MARGIN + ri->rooty - height)
	*y = MARGIN + ri->rooty - height + 1;
    if (*x >= ri->rootx + ri->rootwidth - MARGIN)
	*x = ri->rootx + ri->rootwidth - MARGIN - 1;
    if (*y >= ri->rooty + ri->rootheight - MARGIN)
	*y = ri->rooty + ri->rootheight - MARGIN - 1;
}

Bool
FillName(ga) GenericAssoc *ga;
{
    Status      status;
    char       *name;
    Bool        noname = False;
    int         len;
    Bool        res;
    extern char *strncpy();

    status = XFetchName(dpy, ga->client, &name);
    if (!status || !name || !(*name)) {
	name = "No name";
	noname = True;
    }

    len = strlen(name);
    if (len >= MAXNAME)
	len = MAXNAME;
    if (len != ga->namelen || strncmp(name, ga->name, len)) {
	ga->namelen = len;
	(void) strncpy(ga->name, name, ga->namelen);
	res = True;
    }
    else
	res = False;
    if (!noname)
	XFree(name);
    return res;
}

void
FillIconName(ga) GenericAssoc *ga;
{
Status status;
long len2;
char *pd;
Atom at;
int af;
long ba;
	status = XGetWindowProperty(dpy, ga->client, XA_WM_ICON_NAME,
		0,  MAXNAME, False,
		XA_STRING, &at, &af, &len2, &ba, &pd);
	if (status != Success) {
		ga->iconnamelen = 0;
	} else {
		switch (af) {
		case 8:
			ga->iconnamelen = len2;
			if (ga->iconnamelen >= MAXNAME) ga->iconnamelen = MAXNAME;
			strncpy(ga->iconname, pd, ga->iconnamelen);
			XFree (pd);
			break;
		default:
			ga->iconnamelen = 0;
			if (pd) XFree(pd);
		}
	} 
}

void
ProcessNewName(root, ga, w)
    Window root;
    GenericAssoc *ga;
    Window w;
{
#ifdef	debug
	printf("new name\n");
#endif
	if (!FillName(ga)) return;
	if (ga->iconnamelen == 0) {
		Bool wasMapped;
		int width;
		wasMapped = MappedNotOverride(ga->iconWindow);
		width = XTextWidth(font, ga->name, ga->namelen) + 2*BW;
		MakeReachable(root, &ga->iconx, &ga->icony, width, TITLEHEIGHT);
		if (wasMapped) XUnmapWindow(dpy, ga->iconWindow); 
		XMoveResizeWindow(dpy, ga->iconWindow, ga->iconx - BW,
			ga->icony - BW, width, TITLEHEIGHT);
		if (wasMapped) XMapWindow(dpy, ga->iconWindow);
	}
	XUnmapWindow(dpy, ga->header);
	XMapWindow(dpy, ga->header);
}

void
ProcessNewIconName(root, ga, w)
    Window root;
    GenericAssoc *ga;
    Window w;
{
    int         oldIconLen;
    Bool        wasMapped;
    int         width;

#ifdef	debug
	printf("new icon name\n");
#endif
    wasMapped = MappedNotOverride(ga->iconWindow);
    oldIconLen = ga->iconnamelen;
    FillIconName(ga);
    if (ga->iconnamelen == 0) {
	if (oldIconLen == 0)
	    return;
	width = XTextWidth(font, ga->name, ga->namelen) + 2 * BW;
	MakeReachable(root, &ga->iconx, &ga->icony, width, TITLEHEIGHT);
	if (wasMapped)
	    XUnmapWindow(dpy, ga->iconWindow);
	XMoveResizeWindow(dpy, ga->iconWindow, ga->iconx - BW,
			 ga->icony - BW, width, TITLEHEIGHT);
	if (wasMapped)
	    XMapWindow(dpy, ga->iconWindow);
	return;
    }
    width = XTextWidth(font, ga->iconname, ga->iconnamelen) + 2 * BW;
    MakeReachable(root, &ga->iconx, &ga->icony, width, TITLEHEIGHT);
    if (wasMapped)
	XUnmapWindow(dpy, ga->iconWindow);
    XMoveResizeWindow(dpy, ga->iconWindow, ga->iconx - BW,
		     ga->icony - BW, width, TITLEHEIGHT);
    if (wasMapped)
	XMapWindow(dpy, ga->iconWindow);
}

ProcessNewSizeHints(root, ga, which, w)
    Window root;
    GenericAssoc *ga;
    Atom which;
    Window w;
{
    int i, f;

#ifdef	debug
	printf("new size hints for %s of frame 0x%x\n", WindowType(w), ga->frame);
#endif
    ga->sh.flags = 0;
    (void) XGetNormalHints(dpy, w, &(ga->sh));
    for (i = 1, f = ga->sh.flags; f; f &= ~i, i<<= 1) switch (f & i) {
	case USPosition:
	    break;
	case USSize:
	    break;
	case PPosition:
	    break;
	case PSize:
	    break;
	case PMinSize:
	    break;
	case PMaxSize:
	    break;
	case PResizeInc:
	    break;
	case PAspect:
	    break;
    }
}

ProcessNewWMHints(root, ga, w)
    Window root;
    GenericAssoc *ga;
    Window w;
{
    int         i, f;
    RootInfoRec *ri = findRootInfo(root);
    Window      newwin = NULL;

#ifdef	debug
    printf("new wm hints for %s of frame 0x%x\n", WindowType(w), ga->frame);
#endif
    ga->wmh = XGetWMHints(dpy, w);
    ga->iconknown = False;
    if (ga->wmh)
	for (i = 1, f = ga->wmh->flags; f; f &= ~i, i <<= 1)
	    switch (f & i) {
	    case InputHint:
		break;
	    case StateHint:
		break;
	    case IconPixmapHint:
		{
		    int         x, y, width, height, junkbw, junkdepth;
		    Window      junkroot;
		    XSetWindowAttributes xswa;
		    XWMHints   *xwmh;
		    int         scr = findRootInfo(root) - RootInfo;

		    ga->ownIcon = 0;
		    if (XGetGeometry(dpy, ga->wmh->icon_pixmap, &junkroot,
				     &x, &y, &width, &height, &junkbw,
				     &junkdepth) == 0) {
			/* The turkey gave us a dud pixmap */
			break;
		    }
		    if (junkroot != root) {
			/* The turkey built a pixmap for the wrong root */
			break;
		    }
		    xswa.background_pixmap = ga->wmh->icon_pixmap;
		    if (junkdepth != DefaultDepth(dpy, scr)) {
			/* The turkey built a pixmap the wrong depth */
			if (junkdepth == 1) {
			    /* but its depth 1,  so we can fix it */
			    xswa.background_pixmap =
			        MakeNewPixmap(dpy, root, ga->wmh->icon_pixmap,
					      width, height,
					      DefaultDepth(dpy, scr),
					      ri->foreground, ri->background);
			} else
			    break;
		    }
		    xswa.border_pixel = ri->borderpixel;
		    newwin = XCreateWindow(dpy, root, ga->iconx, ga->icony,
					   width, height, BORDERWIDTH,
					   DefaultDepth(dpy, scr),
					   CopyFromParent,
					   DefaultVisual(dpy, scr),
					   CWBackPixmap | CWBorderPixel,
					   &xswa);
		    if ((xwmh = XGetWMHints(dpy, ga->client)) != NULL) {
			xwmh->window_group = ga->client;
			xwmh->flags |= WindowGroupHint;
			XSetWMHints(dpy, newwin, xwmh);
			XFree(xwmh);
		    }
		}
		/* FALL THROUGH */
	    case IconWindowHint:
		{
		    int         x, y, width, height, junkbw, junkdepth;
		    Window      junkroot;
		    XSetWindowAttributes xswa;

		    if (newwin == NULL) {
			ga->ownIcon = 0;
			if (XGetGeometry(dpy, ga->wmh->icon_window,
					 &junkroot, &x, &y,
			    		 &width, &height, &junkbw,
					 &junkdepth) == 0) {
			    /* The turkey gave us a dud window */
			    break;
			}
			if (junkroot != root) {
			    /*
			     * The turkey built a window for the wrong
			     * root (XXX - this may not be a problem)
			     */
			    break;
			}
			newwin = ga->wmh->icon_window;
		    }
		    if (newwin) {
			if (ga->iconWindow) {
			    XDestroyWindow(dpy, ga->iconWindow);
			    XDeleteAssoc(dpy, frameInfo, ga->iconWindow);
			}
			ga->ownIcon = 1;
			ga->iconWindow = newwin;
			XMakeAssoc(dpy, frameInfo, (XID) newwin, (char *) ga);
			xswa.event_mask = ICON_EVENT_MASK;
			XChangeWindowAttributes(dpy, newwin, CWEventMask,
						&xswa);
			XSetTransientForHint(dpy, newwin, w);
		    }
		}
		break;
	    case IconPositionHint:
		ga->iconx = ga->wmh->icon_x;
		ga->icony = ga->wmh->icon_y;
		ga->iconknown = True;
		break;
	    case IconMaskHint:
		break;
	    case WindowGroupHint:
		break;
	    }
}

ProcessNewCommand(root, ga, w)
    Window root;
    GenericAssoc *ga;
    Window w;
{
#ifdef	debug
	printf("new command for %s of frame 0x%x\n", WindowType(w), ga->frame);
#endif
	/* XXX */
}

ProcessNewClass(root, ga, w)
    Window root;
    GenericAssoc *ga;
    Window w;
{
#ifdef	debug
	printf("new class for %s of frame 0x%x\n", WindowType(w), ga->frame);
#endif
	/* XXX */
}

static
SetupDefaults(argc, argv, dpy)
int	argc;
char	**argv;
Display	*dpy;
{
    char	*temp;
    Colormap	cmap;
    XColor	color;

    /* XXX - should scan arg list too. */
    if ((fontName = XGetDefault(dpy, argv[0], "font")) == NULL)
	fontName = "fixed";
    BackgroundPixelName = XGetDefault(dpy, argv[0], "background");
    ForegroundPixelName = XGetDefault(dpy, argv[0], "foreground");
    BorderPixelName = XGetDefault(dpy, argv[0], "border");
    BorderWidth = BORDERWIDTH;
}

static int
FindColor(dpy, scr, name, dflt)
Display	*dpy;
int	scr;
char	*name;
int	dflt;
{
    Colormap    cmap;
    XColor      color;

    cmap = DefaultColormap(dpy, scr);
    if (!name || XParseColor(dpy, cmap, name, &color) == 0
	|| XAllocColor(dpy, cmap, &color) == 0)
	return (dflt);
    return (color.pixel);
}

