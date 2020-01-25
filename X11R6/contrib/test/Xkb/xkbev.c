/* $XConsortium: xkbev.c,v 1.8 94/04/17 20:46:00 erik Exp $ */
/*

Copyright (c) 1988  X Consortium

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/

/*
 * Author:  Jim Fulton, MIT X Consortium
 */

#include <stdio.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include <ctype.h>

#include <X11/extensions/XKBproto.h>
#include <X11/XKBlib.h>

#define OUTER_WINDOW_MIN_WIDTH 100
#define OUTER_WINDOW_MIN_HEIGHT 100
#define OUTER_WINDOW_DEF_WIDTH (OUTER_WINDOW_MIN_WIDTH + 100)
#define OUTER_WINDOW_DEF_HEIGHT (OUTER_WINDOW_MIN_HEIGHT + 100)
#define OUTER_WINDOW_DEF_X 100
#define OUTER_WINDOW_DEF_Y 100
				

typedef unsigned long Pixel;

char *Yes = "YES";
char *No = "NO";
char *Unknown = "unknown";

char *ProgramName;
Display *dpy;
int screen;
unsigned long wanted =	KeyPressMask|KeyReleaseMask|KeymapStateMask|
			ButtonPressMask|ButtonReleaseMask|OwnerGrabButtonMask;
int synch= 0;
int ignore= 0;
int xkbReqBase;
int xkbEventBase;

usage ()
{
    static char *msg[] = {
"    -display displayname                X server to contact",
"    -geometry geom                      size and location of window",
"    -bw pixels                          border width in pixels",
"    -id windowid                        use existing window",
"    -name string                        window name",
"    -synch                              synchronize requests",
"    -ignore                             ignore XKB",
"    -B                                  want button press/release events",
"    -b[1-5]                             want button motion events",
"    -m                                  want motion events",
"    -e                                  want enter events",
"    -l                                  want leave events",
NULL};
    char **cpp;

    fprintf (stderr, "usage:  %s [-options ...]\n", ProgramName);
    fprintf (stderr, "where options include:\n");

    for (cpp = msg; *cpp; cpp++) {
	fprintf (stderr, "%s\n", *cpp);
    }

    exit (1);
}

prologue (eventp, event_name)
    XkbEvent *eventp;
    char *event_name;
{
    XAnyEvent *e = &eventp->core.xany;

    printf ("\n%s event, serial %ld, synthetic %s, window 0x%lx,\n",
	    event_name, e->serial, e->send_event ? Yes : No, e->window);
    return;
}

xkb_prologue (eventp, event_name)
    XkbEvent *eventp;
    char *event_name;
{
    XkbAnyEvent *e = &eventp->any;

    printf ("\n%s event, serial %ld, synthetic %s, device %d, time %ld,\n",
	    event_name, e->serial, e->send_event ? Yes : No, e->device,e->time);
    return;
}

char *
eventTypeToString(evType)
    int evType;
{
static char name[20];
    switch (evType) {
	case KeyPress:	strcpy(name,"KeyPress"); break;
	case KeyRelease:strcpy(name,"KeyRelease"); break;
	default: strcpy(name,"unknown"); break;
    }
    return name;
}

void
do_XkbStateNotify(xkbev)
    XkbEvent	*xkbev;
{
    XkbStateNotifyEvent *state= &xkbev->state;

    if (state->keycode!=0)
	 printf("    keycode %d, eventType %s,",
		state->keycode,eventTypeToString(state->event_type));
    else printf("    request %d/%d,",state->req_minor,state->req_minor);
    printf(" compatState = 0x%02x%s\n",
		state->compat_state,(state->changed&XkbCompatStateMask?"*":""));
    printf("    group= %d%s, base= %d%s, latched= %d%s, locked= %d%s,\n",
		state->group, (state->changed&XkbGroupStateMask?"*":""),
		state->base_group,(state->changed&XkbGroupBaseMask?"*":""),
		state->latched_group,(state->changed&XkbGroupLatchMask?"*":""),
		state->locked_group,(state->changed&XkbGroupLockMask?"*":""));
    printf("    mods= 0x%02x%s, base= 0x%02x%s, latched= 0x%02x%s, locked= 0x%02x%s\n",
		state->mods, (state->changed&XkbModifierStateMask?"*":""),
		state->base_mods,(state->changed&XkbModifierBaseMask?"*":""),
	       state->latched_mods,(state->changed&XkbModifierLatchMask?"*":""),
		state->locked_mods,(state->changed&XkbModifierLockMask?"*":""));
    return;
}

static void
do_map_message(what,first,num,eol)
    char *	what;
    int		first;
    int		num;
    int		eol;
{
    if (num>1)
	 printf("%ss %d..%d changed%s",what,first,first+num-1,(eol?"\n":""));
    else printf("%s %d changed%s",what,first,(eol?"\n":""));
}

void
do_XkbMapNotify(xkbev)
    XkbEvent	*xkbev;
{
    XkbMapNotifyEvent *map = &xkbev->map;
    if (map->changed&XkbKeyTypesMask) {
	do_map_message("key type",map->first_type,map->num_types,0);
	if (map->resized&XkbKeyTypesMask)
	     printf(" [possibly resized]\n");
	else printf("\n");
    }
    if (map->changed&XkbKeySymsMask) {
	do_map_message("symbols for key",map->first_key_sym,map->num_key_syms,
						1);
    }
    if (map->changed&XkbKeyActionsMask) {
	do_map_message("acts for key",map->first_key_act,
					 	map->num_key_acts,1);
    }
    if (map->changed&XkbKeyBehaviorsMask) {
	do_map_message("behavior for key",map->first_key_behavior,
						map->num_key_behaviors,1);
    }
    if (map->changed&XkbVirtualModsMask) {
	printf("    virtual modifiers changed (0x%04x)\n",map->vmods);
    }
    if (map->changed&XkbExplicitComponentsMask) {
	do_map_message("explicit components for key",map->first_key_explicit,
						map->num_key_explicit,1);
    }
    return;
}

void
do_XkbControlsNotify(xkbev)
    XkbEvent	*xkbev;
{
    XkbControlsNotifyEvent *ctrls = &xkbev->ctrls;
    printf("    changed= 0x%x, enabled= 0x%x, enabledChanges= 0x%x\n",
			ctrls->changed_ctrls,ctrls->enabled_ctrls,
			ctrls->enabled_ctrl_changes);
    if (ctrls->keycode!=0)
	 printf("    keycode %d, eventType %s,",
		ctrls->keycode,eventTypeToString(ctrls->event_type));
    else printf("    request %d/%d%s\n",ctrls->req_major,ctrls->req_minor,
			(ctrls->req_minor!=xkbReqBase?" (NON-XKB)":""));
    return;
}

void
do_XkbIndicatorNotify(xkbev)
    XkbEvent	*xkbev;
{
    XkbIndicatorNotifyEvent *leds = &xkbev->indicators;
    printf("    stateChanged= 0x%08x, new state= 0x%08x\n",leds->state_changed,
								leds->state);
    printf("    mapsChanged= 0x%08x\n",leds->map_changed);
    return;
}

void
do_XkbBellNotify(xkbev)
    XkbEvent	*xkbev;
{
    XkbBellNotifyEvent *bell = &xkbev->bell;
    printf("    bell class= %d, id= %d\n",bell->bell_class,bell->bell_id);
    printf("    percent= %d, pitch= %d, duration= %d",
				bell->percent,bell->pitch,bell->duration);
    if (bell->name!=None) {
	 char *name = XGetAtomName (dpy, bell->name);
	 printf("\n    name= \"%s\"\n",(name?name:""));
	 if (name)
	    XFree(name);
    }
    else printf(", no name\n");
    printf("    window= 0x%x\n",bell->window);
    return;
}

void
do_XkbSlowKeyNotify(xkbev)
    XkbEvent	*xkbev;
{
    XkbSlowKeyNotifyEvent *sk = &xkbev->slow_key;
    char *what;
    switch (sk->slow_key_state) {
	case XkbSK_Press:  what= "press"; break;
	case XkbSK_Accept: what= "accept"; break;
	case XkbSK_Reject: what= "reject"; break;
	case XkbSK_Release: what= "release"; break;
	default: {
	    static char buf[20];
	    sprintf(buf,"unknown(%d)",sk->slow_key_state);
	    what= buf;
	    break;
	}
    }
    printf("    keycode= %d, xkbType= %s, delay= %d\n",sk->keycode,what,
								sk->delay);
    return;
}

do_XkbNamesNotify(xkbev)
    XkbEvent	*xkbev;
{
    XkbNamesNotifyEvent *names = &xkbev->names;

    if (names->changed&
		(XkbKeycodesNameMask|XkbGeometryNameMask|XkbSymbolsNameMask)) {
	int needComma= 0;
	printf("    ");
	if (names->changed&XkbKeycodesNameMask) {
	    printf("keycodes");
	    needComma++;
	}
	if (names->changed&XkbGeometryNameMask) {
	    printf("%sgeometry",(needComma?", ":""));
	    needComma++;
	}
	if (names->changed&XkbSymbolsNameMask) {
	    printf("%ssymbols",(needComma?", ":""));
	    needComma++;
	}
	printf(" name%s changed\n",(needComma>1?"s":""));
    }
    if (names->changed&XkbKeyTypeNamesMask) {
	do_map_message("key type name",names->first_type,names->num_types,1);
    }
    if (names->changed&XkbKTLevelNamesMask) {
	do_map_message("level names for key type",
					names->first_lvl,names->num_lvls,1);
    }
    if (names->changed&XkbRGNamesMask) {
	printf("radio group name",names->first_radio_group,
					names->num_radio_groups,1);
    }
    if (names->changed&XkbIndicatorNamesMask) {
	printf("    names of indicators in 0x%08x changed\n",
						names->changed_indicators);
    }
    if (names->changed&XkbModifierNamesMask) {
	printf("    modifier names in 0x%02x changed\n",names->changed_mods);
    }
    if (names->changed&XkbVirtualModNamesMask) {
	printf("    names of virtual modifiers in 0x%04x changed\n",
						names->changed_vmods);
    }
    if (names->changed&XkbCharSetsMask) {
	 printf("    character set names changed (%d character sets)\n",
						names->num_char_sets);
    }
    else printf("    keyboard has %d character sets\n",names->num_char_sets);
    return;
}

void
do_XkbCompatMapNotify(xkbev)
    XkbEvent	*xkbev;
{
    XkbCompatMapNotifyEvent *map = &xkbev->compat;

    if (map->changed_mods)
	printf("    maps for modifiers in 0x%02x changed\n",map->changed_mods);
    if (map->changed_vmods)
	printf("    maps for virtual modifiers in 0x%04x changed\n",
						map->changed_vmods);
    if (map->num_si>0) {
	printf("    symbol interpretations %d..%d (of %d) changed\n",
				map->first_si,map->first_si+map->num_si-1,
							map->num_total_si);
    }
    else printf("   keyboard has %d symbol interpretations\n",
							map->num_total_si);
    return;
}

void
do_XkbAlternateSymsNotify(xkbev)
    XkbEvent	*xkbev;
{
    XkbAlternateSymsNotifyEvent *altSyms= &xkbev->alt_syms;
    printf("    alternate symbol set: %d\n",altSyms->alt_syms_id);
    printf("    definitions of keys %d..%d changed\n",altSyms->first_key,
					altSyms->first_key+altSyms->num_keys-1);
    return;
}

void
do_XkbActionMessage(xkbev)
    XkbEvent	*xkbev;
{
    XkbActionMessageEvent *msg= &xkbev->message;
    printf("    message: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
					msg->message[0],msg->message[1],
					msg->message[2],msg->message[3],
					msg->message[4],msg->message[5]);
    printf("    key %d, event: %s,  follows: %s\n",msg->keycode,
				     (msg->press?"press":"release"),
				     (msg->key_event_follows?"yes":"no"));
    return;
}

main (argc, argv)
    int argc;
    char **argv;
{
    char *displayname = NULL;
    char *geom = NULL;
    int i,i1,i2,i3,i4;
    XSizeHints hints;
    int borderwidth = 2;
    Window w;
    XSetWindowAttributes attr;
    XWindowAttributes wattr;
    unsigned long mask = 0L;
    int done;
    char *name = "XKB Event Tester";
    unsigned long back, fore;

    w = 0;
    ProgramName = argv[0];
    for (i = 1; i < argc; i++) {
	char *arg = argv[i];

	if (strcmp(arg,"-synch")==0)
	    synch= 1;
	else if (strcmp(arg,"-ignore")==0)
	    ignore= 1;
	else if (arg[0] == '-') {
	    switch (arg[1]) {
	      case 'd':			/* -display host:dpy */
		if (++i >= argc) usage ();
		displayname = argv[i];
		continue;
	      case 'g':			/* -geometry geom */
		if (++i >= argc) usage ();
		geom = argv[i];
		continue;
	      case 'B':
		wanted|= ButtonPressMask|ButtonReleaseMask;
		continue;
	      case 'b':
		switch (arg[2]) {
		  case 'w':		/* -bw pixels */
		    if (++i >= argc) usage ();
		    borderwidth = atoi (argv[i]);
		    continue;
		  case '1':
		    wanted|= Button1MotionMask;
		    continue;
		  case '2':
		    wanted|= Button2MotionMask;
		    continue;
		  case '3':
		    wanted|= Button3MotionMask;
		    continue;
		  case '4':
		    wanted|= Button4MotionMask;
		    continue;
		  case '5':
		    wanted|= Button5MotionMask;
		    continue;
		  case 'm':
		    wanted|= ButtonMotionMask;
		    continue;
		  default:
		    usage ();
		}
	      case 'm':
		wanted|= PointerMotionMask;
		continue;
	      case 'e':
		wanted|= EnterWindowMask|LeaveWindowMask;
		continue;
	      case 'i':			/* -id */
		if (++i >= argc) usage ();
		sscanf(argv[i], "0x%lx", &w);
		if (!w)
		    sscanf(argv[i], "%ld", &w);
		if (!w)
		    usage ();
		continue;
	      case 'n':			/* -name */
		if (++i >= argc) usage ();
		name = argv[i];
		continue;
	      default:
		usage ();
	    }				/* end switch on - */
	} else 
	  usage ();
    }					/* end for over argc */

    if (ignore)
	XkbIgnoreExtension(1);
    dpy = XOpenDisplay (displayname);
    if (!dpy) {
	fprintf (stderr, "%s:  unable to open display '%s'\n",
		 ProgramName, XDisplayName (displayname));
	exit (1);
    }
    if (synch)
	XSynchronize(dpy,1);
    printf("%s compiled with XKB version %d.%02d\n",argv[0],XkbMajorVersion,
							XkbMinorVersion);
    XkbLibraryVersion(&i1,&i2);
    printf("X library supports XKB version %d.%02d\n",i1,i2);
    if ( !XkbQueryExtension(dpy,&xkbReqBase,&xkbEventBase,&i2,&i3,&i4)>0 ) {
	printf("XKB Extension not present on %s\n",XDisplayName(displayname));
	ignore= 1;
    }
    if (ignore)
	 printf("Intentionally ignoring XKB\n");
    else if ( !XkbUseExtension(dpy) ) {
	 printf("X server supports XKB version %d.02d (MISMATCH)\n",i3,i4);
	 ignore= 1;
    }
    else printf("versions match\n");


    screen = DefaultScreen (dpy);

    /* select for all events */
    if (!ignore)
	XkbSelectEvents(dpy,XkbUseCoreKbd,XkbAllEventsMask,XkbAllEventsMask);
    attr.event_mask = wanted;
    if (w) {
	XGetWindowAttributes(dpy, w, &wattr);
	if (wattr.all_event_masks & ButtonPressMask)
	    attr.event_mask &= ~ButtonPressMask;
	attr.event_mask &= ~SubstructureRedirectMask;
	XSelectInput(dpy, w, attr.event_mask);
    } else {
	set_sizehints (&hints, OUTER_WINDOW_MIN_WIDTH, OUTER_WINDOW_MIN_HEIGHT,
		       OUTER_WINDOW_DEF_WIDTH, OUTER_WINDOW_DEF_HEIGHT, 
		       OUTER_WINDOW_DEF_X, OUTER_WINDOW_DEF_Y, geom);

	back = WhitePixel(dpy,screen);
	fore = BlackPixel(dpy,screen);

	attr.background_pixel = back;
	attr.border_pixel = fore;
	mask |= (CWBackPixel | CWBorderPixel | CWEventMask);

	w = XCreateWindow (dpy, RootWindow (dpy, screen), hints.x, hints.y,
			   hints.width, hints.height, borderwidth, 0,
			   InputOutput, (Visual *)CopyFromParent,
			   mask, &attr);

	XSetStandardProperties (dpy, w, name, NULL, (Pixmap) 0,
				argv, argc, &hints);

	XMapWindow (dpy, w);
    }

    for (done = 0; !done; ) {
	XkbEvent event;

	XNextEvent (dpy, &event.core);

	if (event.type==xkbEventBase) {
	    XkbEvent	*xkbev = (XkbEvent *)&event;
	    switch (xkbev->any.xkb_type) {
		case XkbStateNotify:
		    xkb_prologue( xkbev, "XkbStateNotify" );
		    do_XkbStateNotify(xkbev);
		    break;
		case XkbMapNotify:
		    xkb_prologue( xkbev, "XkbMapNotify" );
		    do_XkbMapNotify(xkbev);
		    break;
		case XkbControlsNotify:
		    xkb_prologue( xkbev, "XkbControlsNotify" );
		    do_XkbControlsNotify(xkbev);
		    break;
		case XkbIndicatorMapNotify:
		    xkb_prologue( xkbev, "XkbIndicatorMapNotify" );
		    do_XkbIndicatorNotify(xkbev);
		    break;
		case XkbIndicatorStateNotify:
		    xkb_prologue( xkbev, "XkbIndicatorStateNotify" );
		    do_XkbIndicatorNotify(xkbev);
		    break;
		case XkbBellNotify:
		    xkb_prologue( xkbev, "XkbBellNotify" );
		    do_XkbBellNotify(xkbev);
		    break;
		case XkbSlowKeyNotify:
		    xkb_prologue( xkbev, "XkbSlowKeyNotify" );
		    do_XkbSlowKeyNotify(xkbev);
		    break;
		case XkbNamesNotify:
		    xkb_prologue( xkbev, "XkbNamesNotify" );
		    do_XkbNamesNotify(xkbev);
		    break;
		case XkbCompatMapNotify:
		    xkb_prologue( xkbev, "XkbCompatMapNotify" );
		    do_XkbCompatMapNotify(xkbev);
		    break;
		case XkbAlternateSymsNotify:
		    xkb_prologue( xkbev, "XkbAlternateSymsNotify" );
		    do_XkbAlternateSymsNotify(xkbev);
		    break;
		case XkbActionMessage:
		    xkb_prologue( xkbev, "XkbActionMessage" );
		    do_XkbActionMessage(xkbev);
		default:
		    xkb_prologue( xkbev, "XKB_UNKNOWN!!!" );
		    break;
	    }
	}
	else switch (event.type) {
	  case KeyPress:
	    prologue (&event, "KeyPress");
	    do_KeyPress (&event);
	    break;
	  case KeyRelease:
	    prologue (&event, "KeyRelease");
	    do_KeyRelease (&event);
	    break;
	  case ButtonPress:
	    prologue (&event, "ButtonPress");
	    do_ButtonPress (&event);
	    break;
	  case ButtonRelease:
	    prologue (&event, "ButtonRelease");
	    do_ButtonRelease (&event);
	    break;
	  case MotionNotify:
	    prologue (&event, "MotionNotify");
	    do_MotionNotify (&event);
	    break;
	  case EnterNotify:
	    prologue (&event, "EnterNotify");
	    do_EnterNotify (&event);
	    break;
	  case LeaveNotify:
	    prologue (&event, "LeaveNotify");
	    do_LeaveNotify (&event);
	    break;
	  case KeymapNotify:
	    prologue (&event, "KeymapNotify");
	    do_KeymapNotify (&event);
	    break;
	  case ClientMessage:
	    prologue (&event, "ClientMessage");
	    do_ClientMessage (&event);
	    break;
	  case MappingNotify:
	    prologue (&event, "MappingNotify");
	    do_MappingNotify (&event);
	    break;
	  default:
	    printf ("Unknown event type %d\n", event.type);
	    break;
	}
    }

    XCloseDisplay (dpy);
    exit (0);
}

do_KeyEvent (eventp,compose)
    XkbEvent *eventp;
{
    XKeyEvent *e = &eventp->core.xkey;
    KeySym ks;
    char *ksname;
    int nbytes;
    char str[256+1];
    static XComposeStatus status;

    nbytes = XLookupString (e, str, 256, &ks, (compose?&status:NULL));
    if (ks == NoSymbol)
	ksname = "NoSymbol";
    else if (!(ksname = XKeysymToString (ks)))
	ksname = "(no name)";
    printf ("    root 0x%lx, subw 0x%lx, time %lu, (%d,%d), root:(%d,%d),\n",
	    e->root, e->subwindow, e->time, e->x, e->y, e->x_root, e->y_root);
    printf ("    state 0x%x, group= %d, keycode %u (keysym 0x%x, %s)\n",
	    	e->state&0x1FFF, (e->state>>13)&0x7, e->keycode, ks, ksname);
    printf ("    same_screen %s,\n",e->same_screen ? Yes : No);
    if (nbytes < 0) nbytes = 0;
    if (nbytes > 256) nbytes = 256;
    str[nbytes] = '\0';
    printf ("    XLookupString gives %d characters:  \"%s\"\n", nbytes, str);

    return;
}

do_KeyPress (eventp)
    XkbEvent *eventp;
{
    do_KeyEvent (eventp, 1);
    return;
}

do_KeyRelease (eventp)
    XkbEvent *eventp;
{
    do_KeyEvent (eventp,0);
    return;
}

do_ButtonPress (eventp)
    XkbEvent *eventp;
{
    XButtonEvent *e = &eventp->core.xbutton;

    printf ("    root 0x%lx, subw 0x%lx, time %lu, (%d,%d), root:(%d,%d),\n",
	    e->root, e->subwindow, e->time, e->x, e->y, e->x_root, e->y_root);
    printf ("    state 0x%x, group= %d, button %u, same_screen %s\n",
	    e->state&0x1FFF, (e->state>>13)&0x7, e->button, 
	    e->same_screen ? Yes : No);

    return;
}

do_ButtonRelease (eventp)
    XkbEvent *eventp;
{
    do_ButtonPress (eventp);		/* since it has the same info */
    return;
}

do_MotionNotify (eventp)
    XkbEvent *eventp;
{
    XMotionEvent *e = &eventp->core.xmotion;

    printf ("    root 0x%lx, subw 0x%lx, time %lu, (%d,%d), root:(%d,%d),\n",
	    e->root, e->subwindow, e->time, e->x, e->y, e->x_root, e->y_root);
    printf ("    state 0x%x, group= %d, is_hint %u, same_screen %s\n",
	    e->state&0x1FFF, (e->state>>13)&0x7, e->is_hint, 
	    e->same_screen ? Yes : No);

    return;
}

do_EnterNotify (eventp)
    XkbEvent *eventp;
{
    XCrossingEvent *e = &eventp->core.xcrossing;
    char *mode, *detail;
    char dmode[10], ddetail[10];

    switch (e->mode) {
      case NotifyNormal:  mode = "NotifyNormal"; break;
      case NotifyGrab:  mode = "NotifyGrab"; break;
      case NotifyUngrab:  mode = "NotifyUngrab"; break;
      case NotifyWhileGrabbed:  mode = "NotifyWhileGrabbed"; break;
      default:  mode = dmode, sprintf (dmode, "%u", e->mode); break;
    }

    switch (e->detail) {
      case NotifyAncestor:  detail = "NotifyAncestor"; break;
      case NotifyVirtual:  detail = "NotifyVirtual"; break;
      case NotifyInferior:  detail = "NotifyInferior"; break;
      case NotifyNonlinear:  detail = "NotifyNonlinear"; break;
      case NotifyNonlinearVirtual:  detail = "NotifyNonlinearVirtual"; break;
      case NotifyPointer:  detail = "NotifyPointer"; break;
      case NotifyPointerRoot:  detail = "NotifyPointerRoot"; break;
      case NotifyDetailNone:  detail = "NotifyDetailNone"; break;
      default:  detail = ddetail; sprintf (ddetail, "%u", e->detail); break;
    }

    printf ("    root 0x%lx, subw 0x%lx, time %lu, (%d,%d), root:(%d,%d),\n",
	    e->root, e->subwindow, e->time, e->x, e->y, e->x_root, e->y_root);
    printf ("    mode %s, detail %s, same_screen %s,\n",
	    mode, detail, e->same_screen ? Yes : No);
    printf ("    focus %s, state %u, group= %d\n", e->focus ? Yes : No, 
	    e->state&0x1FFF, (e->state>>13)&7 );

    return;
}

do_LeaveNotify (eventp)
    XkbEvent *eventp;
{
    do_EnterNotify (eventp);		/* since it has same information */
    return;
}

do_KeymapNotify (eventp)
    XkbEvent *eventp;
{
    XKeymapEvent *e = &eventp->core.xkeymap;
    int i;

    printf ("    keys:  ");
    for (i = 0; i < 32; i++) {
	if (i == 16) printf ("\n           ");
	printf ("%-3u ", (unsigned int) e->key_vector[i]);
    }
    printf ("\n");
    return;
}

do_ClientMessage (eventp)
    XkbEvent *eventp;
{
    XClientMessageEvent *e = &eventp->core.xclient;
    char *mname = XGetAtomName (dpy, e->message_type);

    printf ("    message_type 0x%lx (%s), format %d\n",
	    e->message_type, mname ? mname : Unknown, e->format);

    if (mname) XFree (mname);
    return;
}

do_MappingNotify (eventp)
    XkbEvent *eventp;
{
    XMappingEvent *e = &eventp->core.xmapping;
    char *r;
    char rdummy[10];

    switch (e->request) {
      case MappingModifier:  r = "MappingModifier"; break;
      case MappingKeyboard:  r = "MappingKeyboard"; break;
      case MappingPointer:  r = "MappingPointer"; break;
      default:  r = rdummy; sprintf (rdummy, "%d", e->request); break;
    }

    printf ("    request %s, first_keycode %d, count %d\n",
	    r, e->first_keycode, e->count);
    XRefreshKeyboardMapping(e);
    return;
}



set_sizehints (hintp, min_width, min_height,
	       defwidth, defheight, defx, defy, geom)
    XSizeHints *hintp;
    int min_width, min_height, defwidth, defheight, defx, defy;
    char *geom;
{
    int geom_result;

    /* set the size hints, algorithm from xlib xbiff */

    hintp->width = hintp->min_width = min_width;
    hintp->height = hintp->min_height = min_height;
    hintp->flags = PMinSize;
    hintp->x = hintp->y = 0;
    geom_result = NoValue;
    if (geom != NULL) {
        geom_result = XParseGeometry (geom, &hintp->x, &hintp->y,
				      (unsigned int *)&hintp->width,
				      (unsigned int *)&hintp->height);
	if ((geom_result & WidthValue) && (geom_result & HeightValue)) {
#define max(a,b) ((a) > (b) ? (a) : (b))
	    hintp->width = max (hintp->width, hintp->min_width);
	    hintp->height = max (hintp->height, hintp->min_height);
	    hintp->flags |= USSize;
	}
	if ((geom_result & XValue) && (geom_result & YValue)) {
	    hintp->flags += USPosition;
	}
    }
    if (!(hintp->flags & USSize)) {
	hintp->width = defwidth;
	hintp->height = defheight;
	hintp->flags |= PSize;
    }
/*
    if (!(hintp->flags & USPosition)) {
	hintp->x = defx;
	hintp->y = defy;
	hintp->flags |= PPosition;
    }
 */
    if (geom_result & XNegative) {
	hintp->x = DisplayWidth (dpy, DefaultScreen (dpy)) + hintp->x -
		    hintp->width;
    }
    if (geom_result & YNegative) {
	hintp->y = DisplayHeight (dpy, DefaultScreen (dpy)) + hintp->y -
		    hintp->height;
    }
    return;
}

