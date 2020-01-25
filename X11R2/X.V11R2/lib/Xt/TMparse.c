#ifndef lint
static char rcsid[] = "$Header: TMparse.c,v 1.58 88/02/26 12:48:03 swick Exp $";
#endif lint

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
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

/* TMparse.c -- parse all X events into widget specific actions. */

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/StringDefs.h>
#include <stdio.h>
#include "IntrinsicI.h"
#include "TMprivate.h"
#include <X11/Convert.h>

/* Private definitions. */
#define LF 0x0a
#define BSLASH '\\'

typedef int		EventType;
typedef unsigned int	XtEventType;
typedef unsigned int	EventCode;

typedef String (*ParseProc)(); /* str, closure, event */
    /* String str; */
    /* Opaque closure; */
    /* EventPtr event; */

typedef struct _EventKey {
    char    	*event;
    XrmQuark	signature;
    EventType	eventType;
    ParseProc	parseDetail;
    Opaque	closure;
}EventKey, *EventKeys;

static char *currentProduction;

static NameValueRec modifiers[] = {
    {"None",    0,      None},
    {"Shift",	0,	ShiftMask},
    {"Lock",	0,	LockMask},
    {"Ctrl",	0,	ControlMask},
    {"Mod1",	0,	Mod1Mask},
    {"Mod2",	0,	Mod2Mask},
    {"Mod3",	0,	Mod3Mask},
    {"Mod4",	0,	Mod4Mask},
    {"Mod5",	0,	Mod5Mask},
    {"Meta",	0,	Mod1Mask},

    {"Button1",	0,	Button1Mask},
    {"Button2",	0,	Button2Mask},
    {"Button3",	0,	Button3Mask},
    {"Button4",	0,	Button4Mask},
    {"Button5",	0,	Button5Mask},

    {"Any",	0,	AnyModifier},

    {NULL, NULL, NULL},
};

static NameValueRec buttonNames[] = {
    {"Button1",	0,	Button1},
    {"Button2", 0,	Button2},
    {"Button3", 0,	Button3},
    {"Button4", 0,	Button4},
    {"Button5", 0,	Button5},
    {NULL, NULL, NULL},
};

static NameValueRec notifyModes[] = {
    {"Normal",		0,	NotifyNormal},
    {"Grab",		0,	NotifyGrab},
    {"Ungrab",		0,	NotifyUngrab},
    {"WhileGrabbed",    0,	NotifyWhileGrabbed},
    {NULL, NULL, NULL},
};

static NameValueRec notifyDetail[] = {
    {"Ancestor",	    0,	NotifyAncestor},
    {"Virtual",		    0,	NotifyVirtual},
    {"Inferior",	    0,	NotifyInferior},
    {"Nonlinear",	    0,	NotifyNonlinear},
    {"NonlinearVirtual",    0,	NotifyNonlinearVirtual},
    {"Pointer",		    0,	NotifyPointer},
    {"PointerRoot",	    0,	NotifyPointerRoot},
    {"DetailNone",	    0,	NotifyDetailNone},
    {NULL, NULL, NULL},
};

static NameValueRec visibilityNotify[] = {
    {"Unobscured",	    0,	VisibilityUnobscured},
    {"PartiallyObscured",   0,	VisibilityPartiallyObscured},
    {"FullyObscured",       0,	VisibilityFullyObscured},
    {NULL, NULL, NULL},
};

static NameValueRec circulation[] = {
    {"OnTop",       0,	PlaceOnTop},
    {"OnBottom",    0,	PlaceOnBottom},
    {NULL, NULL, NULL},
};

static NameValueRec propertyChanged[] = {
    {"NewValue",    0,	PropertyNewValue},
    {"Delete",      0,	PropertyDelete},
    {NULL, NULL, NULL},
};

static String ParseKeySym();
static String ParseKeyAndModifiers();
static String ParseTable();
static String ParseImmed();
static String ParseNone();
static String ParseModImmed();

static EventKey events[] = {

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{"KeyPress",	    NULL, KeyPress,	ParseKeySym,	NULL},
{"Key", 	    NULL, KeyPress,	ParseKeySym,	NULL},
{"KeyDown",	    NULL, KeyPress,	ParseKeySym,	NULL},
{"Ctrl",            NULL, KeyPress,  ParseKeyAndModifiers,(Opaque)ControlMask},
{"Meta",            NULL, KeyPress,     ParseKeyAndModifiers,(Opaque)Mod1Mask},
{"Shift",           NULL, KeyPress,    ParseKeyAndModifiers,(Opaque)ShiftMask},

{"KeyUp",	    NULL, KeyRelease,	ParseKeySym,	NULL},
{"KeyRelease",	    NULL, KeyRelease,	ParseKeySym,	NULL},

{"ButtonPress",     NULL, ButtonPress,    ParseTable,(Opaque)buttonNames},
{"BtnDown",	    NULL, ButtonPress,    ParseTable,(Opaque)buttonNames},
{"Btn1Down",	    NULL, ButtonPress,	ParseImmed,(Opaque)Button1},
{"Btn2Down", 	    NULL, ButtonPress,	ParseImmed,(Opaque)Button2},
{"Btn3Down", 	    NULL, ButtonPress,	ParseImmed,(Opaque)Button3},
{"Btn4Down", 	    NULL, ButtonPress,	ParseImmed,(Opaque)Button4},
{"Btn5Down", 	    NULL, ButtonPress,	ParseImmed,(Opaque)Button5},

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{"ButtonRelease",   NULL, ButtonRelease,    ParseTable,(Opaque)buttonNames},
{"BtnUp", 	    NULL, ButtonRelease,    ParseTable,(Opaque)buttonNames},
{"Btn1Up", 	    NULL, ButtonRelease,    ParseImmed,(Opaque)Button1},
{"Btn2Up", 	    NULL, ButtonRelease,    ParseImmed,(Opaque)Button2},
{"Btn3Up", 	    NULL, ButtonRelease,    ParseImmed,(Opaque)Button3},
{"Btn4Up", 	    NULL, ButtonRelease,    ParseImmed,(Opaque)Button4},
{"Btn5Up", 	    NULL, ButtonRelease,    ParseImmed,(Opaque)Button5},

{"MotionNotify",    NULL, MotionNotify,	ParseNone,	NULL},
{"PtrMoved", 	    NULL, MotionNotify,	ParseNone,	NULL},
{"Motion", 	    NULL, MotionNotify,	ParseNone,	NULL},
{"MouseMoved", 	    NULL, MotionNotify,	ParseNone,	NULL},
{"ButtonMotion", NULL, MotionNotify, ParseModImmed, (Opaque)AnyButtonModifier},
{"BtnMotion",    NULL, MotionNotify, ParseModImmed, (Opaque)AnyButtonModifier},
{"Button1Motion",   NULL, MotionNotify, ParseModImmed,	(Opaque)Button1Mask},
{"Btn1Motion",      NULL, MotionNotify, ParseModImmed,	(Opaque)Button1Mask},
{"Button2Motion",   NULL, MotionNotify, ParseModImmed,	(Opaque)Button2Mask},
{"Btn2Motion",      NULL, MotionNotify, ParseModImmed,	(Opaque)Button2Mask},
{"Button3Motion",   NULL, MotionNotify, ParseModImmed,	(Opaque)Button3Mask},
{"Btn3Motion",      NULL, MotionNotify, ParseModImmed,	(Opaque)Button3Mask},
{"Button4Motion",   NULL, MotionNotify, ParseModImmed,	(Opaque)Button4Mask},
{"Btn4Motion",      NULL, MotionNotify, ParseModImmed,	(Opaque)Button4Mask},
{"Button5Motion",   NULL, MotionNotify, ParseModImmed,	(Opaque)Button5Mask},
{"Btn5Motion",      NULL, MotionNotify, ParseModImmed,	(Opaque)Button5Mask},

{"EnterNotify",     NULL, EnterNotify,    ParseTable,(Opaque)notifyModes},
{"Enter",	    NULL, EnterNotify,    ParseTable,(Opaque)notifyModes},
{"EnterWindow",     NULL, EnterNotify,    ParseTable,(Opaque)notifyModes},

{"LeaveNotify",     NULL, LeaveNotify,    ParseTable,(Opaque)notifyModes},
{"LeaveWindow",     NULL, LeaveNotify,    ParseTable,(Opaque)notifyModes},
{"Leave",	    NULL, LeaveNotify,    ParseTable,(Opaque)notifyModes},

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{"FocusIn",	    NULL, FocusIn,	       ParseTable,(Opaque)notifyModes},

{"FocusOut",	    NULL, FocusOut,       ParseTable,(Opaque)notifyModes},

{"KeymapNotify",    NULL, KeymapNotify,	ParseNone,	NULL},
{"Keymap",	    NULL, KeymapNotify,	ParseNone,	NULL},

{"Expose", 	    NULL, Expose,		ParseNone,	NULL},

{"GraphicsExpose",  NULL, GraphicsExpose,	ParseNone,	NULL},
{"GrExp",	    NULL, GraphicsExpose,	ParseNone,	NULL},

{"NoExpose",	    NULL, NoExpose,	ParseNone,	NULL},
{"NoExp",	    NULL, NoExpose,	ParseNone,	NULL},

{"VisibilityNotify",NULL, VisibilityNotify,ParseNone,	NULL},
{"Visible",	    NULL, VisibilityNotify,ParseNone,	NULL},

{"CreateNotify",    NULL, CreateNotify,	ParseNone,	NULL},
{"Create",	    NULL, CreateNotify,	ParseNone,	NULL},

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{"DestroyNotify",   NULL, DestroyNotify,	ParseNone,	NULL},
{"Destroy",	    NULL, DestroyNotify,	ParseNone,	NULL},

{"UnmapNotify",     NULL, UnmapNotify,	ParseNone,	NULL},
{"Unmap",	    NULL, UnmapNotify,	ParseNone,	NULL},

{"MapNotify",	    NULL, MapNotify,	ParseNone,	NULL},
{"Map",		    NULL, MapNotify,	ParseNone,	NULL},

{"MapRequest",	    NULL, MapRequest,	ParseNone,	NULL},
{"MapReq",	    NULL, MapRequest,	ParseNone,	NULL},

{"ReparentNotify",  NULL, ReparentNotify,	ParseNone,	NULL},
{"Reparent",	    NULL, ReparentNotify,	ParseNone,	NULL},

{"ConfigureNotify", NULL, ConfigureNotify,	ParseNone,	NULL},
{"Configure",	    NULL, ConfigureNotify,	ParseNone,	NULL},

{"ConfigureRequest",NULL, ConfigureRequest,ParseNone,	NULL},
{"ConfigureReq",    NULL, ConfigureRequest,ParseNone,	NULL},

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{"GravityNotify",   NULL, GravityNotify,	ParseNone,	NULL},
{"Grav",	    NULL, GravityNotify,	ParseNone,	NULL},

{"ResizeRequest",   NULL, ResizeRequest,	ParseNone,	NULL},
{"ResReq",	    NULL, ResizeRequest,	ParseNone,	NULL},

{"CirculateNotify", NULL, CirculateNotify,	ParseNone,	NULL},
{"Circ",	    NULL, CirculateNotify,	ParseNone,	NULL},

{"CirculateRequest",NULL, CirculateRequest,ParseNone,	NULL},
{"CircReq",	    NULL, CirculateRequest,ParseNone,	NULL},

{"PropertyNotify",  NULL, PropertyNotify,	ParseNone,	NULL},
{"Prop",	    NULL, PropertyNotify,	ParseNone,	NULL},

{"SelectionClear",  NULL, SelectionClear,	ParseNone,	NULL},
{"SelClr",	    NULL, SelectionClear,	ParseNone,	NULL},

{"SelectionRequest",NULL, SelectionRequest,ParseNone,	NULL},
{"SelReq",	    NULL, SelectionRequest,ParseNone,	NULL},

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{"SelectionNotify", NULL, SelectionNotify,	ParseNone,	NULL},
{"Select",	    NULL, SelectionNotify,	ParseNone,	NULL},

{"ColormapNotify",  NULL, ColormapNotify,	ParseNone,	NULL},
{"Clrmap",	    NULL, ColormapNotify,	ParseNone,	NULL},

{"ClientMessage",   NULL, ClientMessage,	ParseNone,	NULL},
{"Message",	    NULL, ClientMessage,	ParseNone,	NULL},

{"MappingNotify",   NULL, 0/*mapping*/,	ParseNone,	NULL},
{"Mapping",	    NULL, 0/*mapping*/,	ParseNone,	NULL},

{"Timer",	    NULL, _XtTimerEventType,ParseNone,	NULL},

{"EventTimer",	    NULL, _XtEventTimerEventType,ParseNone,NULL},

/* Event Name,	  Quark, Event Type,	Detail Parser, Closure */

{ NULL, NULL, NULL, NULL, NULL}};

static Boolean initialized = FALSE;

static void FreeEventSeq(eventSeq)
    EventSeqPtr eventSeq;
{
    register EventSeqPtr evs = eventSeq;

    while (evs != NULL) {
	evs->state = (StatePtr) evs;
	if (evs->next != NULL
	    && evs->next->state == (StatePtr) evs->next)
	    evs->next = NULL;
	evs = evs->next;
    }

    evs = eventSeq;
    while (evs != NULL) {
	register EventPtr event = evs;
	evs = evs->next;
	if (evs == event) evs = NULL;
	XtFree((char *)event);
    }
}

static void CompileNameValueTable(table)
    NameValueTable table;
{
    register int i;

    for (i=0; table[i].name; i++)
        table[i].signature = StringToQuark(table[i].name);
}

static void Compile_XtEventTable(table)
    EventKeys	table;
{
    register int i;

    for (i=0; table[i].event; i++)
        table[i].signature = StringToQuark(table[i].event);
}

static Syntax(str)
    String str;
{
    char production[500], *eol;

    (void) fprintf(stderr,
     "Translation table syntax error: %s\n", str);

    strncpy( production, currentProduction, 500 );
    if ((eol = index(production, '\n')) != 0) *eol = '\0';
    (void) fprintf(stderr, "Found while parsing '%s'.\n", production);
}



static Cardinal LookupTMEventType(eventStr)
  String eventStr;

{
    register Cardinal   i;
    register XrmQuark	signature;

    signature = StringToQuark(eventStr);
    for (i = 0; events[i].signature != NULL; i++)
        if (events[i].signature == signature) return i;

    Syntax("Unknown event type.");
    return i;
}

/***********************************************************************
 * _XtLookupTableSym
 * Given a table and string, it fills in the value if found and returns
 * status
 ***********************************************************************/

Boolean _XtLookupTableSym(table, name, valueP)
    NameValueTable	table;
    String name;
    Value *valueP;
{
/* ||| should implement via hash or something else faster than linear search */

    register int i;
    register XrmQuark signature = StringToQuark(name);

    for (i=0; table[i].name != NULL; i++)
	if (table[i].signature == signature) {
	    *valueP = table[i].value;
	    return TRUE;
	}

    return FALSE;
}

static String ScanFor(str, ch)
    register String str;
    register char ch;
{
    while (*str != ch && *str != '\0' &&*str != '\n') str++;
    return str;
}

static String ScanNumeric(str)
    register String str;
{
    while ('0' <= *str && *str <= '9') str++;
    return str;
}

static String ScanAlphanumeric(str)
    register String str;
{
    while (
        ('A' <= *str && *str <= 'Z') || ('a' <= *str && *str <= 'z')
	|| ('0' <= *str && *str <= '9')) str++;
    return str;
}

static String ScanIdent(str)
    register String str;
{
    str = ScanAlphanumeric(str);
    while (
	   ('A' <= *str && *str <= 'Z')
	|| ('a' <= *str && *str <= 'z')
	|| ('0' <= *str && *str <= '9')
	|| (*str == '-')
	|| (*str == '_')
	|| (*str == '$')
	) str++;
    return str;
}

static String ScanWhitespace(str)
    register String str;
{
    while (*str == ' ' || *str == '\t') str++;
    return str;
}

static String ParseModifiers(str, event)
    register String str;
    EventPtr event;
{
    register String start;
    char modStr[100];
    Boolean notFlag, exclusive;
    Value maskBit;

 
    str = ScanWhitespace(str);
    start = str;
    str = ScanAlphanumeric(str);
    if (start != str) {
         (void) strncpy(modStr, start, str-start);
          modStr[str-start] = '\0';
          if (_XtLookupTableSym(modifiers, modStr, &maskBit))
	    if (maskBit== None) {
                event->event.modifierMask = ~0;
		event->event.modifiers = 0;
                str = ScanWhitespace(str);
	        return str;
            }
            if (maskBit == AnyModifier) {/*backward compatability*/
                event->event.modifierMask = 0;
                event->event.modifiers = 0;
                str = ScanWhitespace(str);
                return str;
            }
         str = start;
    }

    if (*str == '!') {
         exclusive = TRUE;
         str++;
         str = ScanWhitespace(str);
    }
    else exclusive = FALSE;

   
    while (*str != '<') {
        if (*str == '~') {
             notFlag = TRUE;
             str++;
          } else 
              notFlag = FALSE;
	start = str;
        str = ScanAlphanumeric(str);
        if (start == str) {
           Syntax("Modifier or '<' expected");
           return str;
        }
  	   (void) strncpy(modStr, start, str-start);
	   modStr[str-start] = '\0';
	   if (!_XtLookupTableSym(modifiers, modStr, &maskBit))
	       Syntax("Unknown modifier name.");
	    event->event.modifierMask |= maskBit;
	if (notFlag) event->event.modifiers &= ~maskBit;
	else event->event.modifiers |= maskBit;
        str = ScanWhitespace(str);
    }
    if (exclusive) event->event.modifierMask = ~0;
    return str;
}

static String ParseXtEventType(str, event, tmEventP)
    register String str;
    EventPtr event;
    Cardinal *tmEventP;
{
    String start = str;
    char eventTypeStr[100];

    str = ScanAlphanumeric(str);
    (void) strncpy(eventTypeStr, start, str-start);
    eventTypeStr[str-start] = '\0';
    *tmEventP = LookupTMEventType(eventTypeStr);
    event->event.eventType = events[*tmEventP].eventType;

    return str;
}

static unsigned int StrToHex(str)
    String str;
{
    register char   c;
    register int    val = 0;

    while (c = *str) {
	if ('0' <= c && c <= '9') val = val*16+c-'0';
	else if ('a' <= c && c <= 'z') val = val*16+c-'a'+10;
	else if ('A' <= c && c <= 'Z') val = val*16+c-'A'+10;
	else return -1;
	str++;
    }

    return val;
}

static unsigned int StrToOct(str)
    String str;
{
    register char c;
    register int  val = 0;

    while (c = *str) {
        if ('0' <= c && c <= '7') val = val*8+c-'0'; else return -1;
	str++;
    }

    return val;
}

static unsigned int StrToNum(str)
    String str;
{
    register char c;
    register int val = 0;

    if (*str == '0') {
	str++;
	if (*str == 'x' || *str == 'X') return StrToHex(++str);
	else return StrToOct(str);
    }

    while (c = *str) {
	if ('0' <= c && c <= '9') val = val*10+c-'0';
	else return -1;
	str++;
    }

    return val;
}

static KeySym XStringToKeySym(str)
    String str;
{

/* ||| replace this with real one when xlib has it... */

    if (str == NULL) return (KeySym) 0;
    if ('0' <= *str && *str <= '9') return (KeySym) StrToNum(str);
    if ('A' <= *str && *str <= 'Z') return (KeySym) *str+'a'-'A';
    return (KeySym) *str;
}

static String ParseImmed(str, closure, event)
    String str;
    Opaque closure;
    EventPtr event;
{
    event->event.eventCode = (unsigned long)closure;
    event->event.eventCodeMask = (unsigned long)~0L;

    return str;
}

static String ParseModImmed(str, closure, event)
    String str;
    Opaque closure;
    EventPtr event;
{
    event->event.modifiers |= (unsigned long)closure;
    event->event.modifierMask |= (unsigned long)closure;

    return str;
}

static String ParseKeyAndModifiers(str, closure, event)
    String str;
    Opaque closure;
    EventPtr event;
{
    str = ParseKeySym(str, closure, event);

    event->event.modifiers |= (unsigned long)closure;
    event->event.modifierMask |= (unsigned long)closure;

    return str;
}

/*ARGSUSED*/
static String ParseKeySym(str, closure, event)
    register String str;
    Opaque closure;
    EventPtr event;
{
    char keySymName[100], *start;

    str = ScanWhitespace(str);

    if (*str == '\\') {
	str++;
	keySymName[0] = *str;
	if (*str != '\0' && *str != '\n') str++;
	keySymName[1] = '\0';
	event->event.eventCode = XStringToKeySym(keySymName);
	event->event.eventCodeMask = ~0L;
    } else if (*str == ',' || *str == ':') {
	/* no detail */
	event->event.eventCode = 0L;
        event->event.eventCodeMask = 0L;
    } else {
	start = str;
	while (
		*str != ','
		&& *str != ':'
		&& *str != ' '
		&& *str != '\t'
                && *str != '\n'
		&& *str != '\0') str++;
	(void) strncpy(keySymName, start, str-start);
	keySymName[str-start] = '\0';
	event->event.eventCode = XStringToKeySym(keySymName);
	event->event.eventCodeMask = ~0L;
    }

    return str;
}

static String ParseTable(str, closure, event)
    register String str;
    Opaque closure;
    EventPtr event;
{
    register String start = str;
    char tableSymName[100];

    event->event.eventCode = 0L;
    str = ScanAlphanumeric(str);
    if (str == start) {event->event.eventCodeMask = 0L; return str; }
    (void) strncpy(tableSymName, start, str-start);
    tableSymName[str-start] = '\0';
    if (! _XtLookupTableSym(
        (NameValueTable)closure, tableSymName, (Value *)&event->event.eventCode))
	Syntax("Unknown Detail Type.");
    event->event.eventCodeMask = ~0L;

    return str;
}

/*ARGSUSED*/
static String ParseNone(str, closure, event)
    String str;
    Opaque closure;
    EventPtr event;
{
    event->event.eventCode = 0;
    event->event.eventCodeMask = 0;

    return str;
}

static ModifierMask buttonModifierMasks[] = {
    0, Button1Mask, Button2Mask, Button3Mask, Button4Mask, Button5Mask
};

static String ParseEvent(str, event)
    register String str;
    EventPtr	event;
{
    Cardinal	tmEvent;

    str = ParseModifiers(str, event);
    if (*str != '<') Syntax("Missing '<'."); else str++;
    str = ParseXtEventType(str, event, &tmEvent);
    if (*str != '>') Syntax("Missing '>'"); else str++;
    str = (*(events[tmEvent].parseDetail))(
        str, events[tmEvent].closure, event);

/* gross hack! ||| this kludge is related to the X11 protocol deficiency w.r.t.
 * modifiers in grabs.
 */
    if ((event->event.eventType == ButtonRelease)
	&& (event->event.modifiers |event->event.modifierMask != 0) /* any */
        && (event->event.modifiers != AnyModifier))
    {
	event->event.modifiers
	    |= buttonModifierMasks[event->event.eventCode];
	/* the button that is going up will always be in the modifiers... */
    }

    return str;
}

static String ParseQuotedStringEvent(str, event)
    register String str;
    register EventPtr event;
{
    register int j;

    ModifierMask ctrlMask;
    ModifierMask metaMask;
    ModifierMask shiftMask;
    register char	c;
    char	s[2];
    Cardinal	tmEvent;

    (void) _XtLookupTableSym(modifiers, "Ctrl", (Value *) &ctrlMask);
    (void) _XtLookupTableSym(modifiers, "Meta", (Value *) &metaMask);
    (void) _XtLookupTableSym(modifiers, "Shift", (Value *) &shiftMask);

    event->event.modifierMask = ctrlMask | metaMask | shiftMask;

    for (j=0; j < 2; j++)
	if (*str=='^' && !(event->event.modifiers | ctrlMask)) {
	    str++;
	    event->event.modifiers |= ctrlMask;
	} else if (*str == '$' && !(event->event.modifiers | metaMask)) {
	    str++;
	    event->event.modifiers |= metaMask;
	} else if (*str == '\\') {
	    str++;
	    c = *str;
	    if (*str != '\0' && *str != '\n') str++;
	    break;
	} else {
	    c = *str;
	    if (*str != '\0' && *str != '\n') str++;
	    break;
	}
    tmEvent = (EventType) LookupTMEventType("Key");
    event->event.eventType = events[tmEvent].eventType;
    if ('A' <= c && c <= 'Z') {
	event->event.modifiers |=  shiftMask;
	c += 'a' - 'A';
    }
    s[0] = c;
    s[1] = '\0';
    event->event.eventCode = XStringToKeySym(s);

    return str;
}

#ifdef notdef
static EventSeqPtr CopyEventSeq(seq)
    EventSeqPtr seq;
{
    EventSeqPtr head = NULL;
    EventSeqPtr *tail = &head;

    while (seq != NULL) {
	EventSeqPtr new = XtNew(EventSeqRec);
	if (head == NULL) head = new;
	*tail = new;
	tail = &new->next;
	*new = *seq;
	seq = seq->next;
    }
    return head;
}
#endif

static void RepeatDown(eventP, reps, actionsP)
    EventPtr *eventP;
    int reps;
    ActionPtr **actionsP;
{
    EventRec upEventRec;
    register EventPtr event, downEvent;
    EventPtr upEvent = &upEventRec;
    register int i;

    static EventSeqRec timerEventRec = {
	{0, 0, _XtEventTimerEventType, 0L, 0L},
	/* (StatePtr) -1 */ NULL,
	NULL,
	NULL
    };

    downEvent = event = *eventP;
    *upEvent = *downEvent;
    upEvent->event.eventType = ((event->event.eventType == ButtonPress) ?
	ButtonRelease : KeyRelease);
    if ((upEvent->event.eventType == ButtonRelease)
	&& (upEvent->event.modifiers != AnyModifier)
        && (upEvent->event.modifiers | upEvent->event.modifierMask !=0))
	upEvent->event.modifiers
	    |= buttonModifierMasks[event->event.eventCode];

    for (i=1; i<reps; i++) {

	/* up */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *upEvent;

	/* timer */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = timerEventRec;

	/* down */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *downEvent;

    }

    event->next = NULL;
    *eventP = event;
    *actionsP = &event->actions;
}

static void RepeatDownPlus(eventP, reps, actionsP)
    EventPtr *eventP;
    int reps;
    ActionPtr **actionsP;
{
    EventRec upEventRec;
    register EventPtr event, downEvent, lastDownEvent;
    EventPtr upEvent = &upEventRec;
    register int i;

    static EventSeqRec timerEventRec = {
	{0, 0, _XtEventTimerEventType, 0L, 0L},
	/* (StatePtr) -1 */ NULL,
	NULL,
	NULL
    };

    downEvent = event = *eventP;
    *upEvent = *downEvent;
    upEvent->event.eventType = ((event->event.eventType == ButtonPress) ?
	ButtonRelease : KeyRelease);
    if ((upEvent->event.eventType == ButtonRelease)
	&& (upEvent->event.modifiers != AnyModifier)
        && (upEvent->event.modifiers | upEvent->event.modifierMask != 0))
	upEvent->event.modifiers
	    |= buttonModifierMasks[event->event.eventCode];

    for (i=0; i<reps; i++) {

	if (i > 0) {
	/* down */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *downEvent;
	}
	lastDownEvent = event;

	/* up */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *upEvent;

	/* timer */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = timerEventRec;

    }

    event->next = lastDownEvent;
    *eventP = event;
    *actionsP = &lastDownEvent->actions;
}

static void RepeatUp(eventP, reps, actionsP)
    EventPtr *eventP;
    int reps;
    ActionPtr **actionsP;
{
    EventRec upEventRec;
    register EventPtr event, downEvent;
    EventPtr upEvent = &upEventRec;
    register int i;

    static EventSeqRec timerEventRec = {
	{0, 0, _XtEventTimerEventType, 0L, 0L},
	/* (StatePtr) -1 */ NULL,
	NULL,
	NULL
    };

    /* the event currently sitting in *eventP is an "up" event */
    /* we want to make it a "down" event followed by an "up" event, */
    /* so that sequence matching on the "state" side works correctly. */

    downEvent = event = *eventP;
    *upEvent = *downEvent;
    downEvent->event.eventType = ((event->event.eventType == ButtonRelease) ?
	ButtonPress : KeyPress);
    if ((downEvent->event.eventType == ButtonPress)
	&& (downEvent->event.modifiers != AnyModifier)
        && (downEvent->event.modifiers | downEvent->event.modifierMask != 0))
	downEvent->event.modifiers
	    &= ~buttonModifierMasks[event->event.eventCode];

    /* up */
    event->next = XtNew(EventSeqRec);
    event = event->next;
    *event = *upEvent;

    for (i=1; i<reps; i++) {

	/* timer */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = timerEventRec;

	/* down */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *downEvent;

	/* up */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *upEvent;

	}

    event->next = NULL;
    *eventP = event;
    *actionsP = &event->actions;
}

static void RepeatUpPlus(eventP, reps, actionsP)
    EventPtr *eventP;
    int reps;
    ActionPtr **actionsP;
{
    EventRec upEventRec;
    register EventPtr event, downEvent, lastUpEvent;
    EventPtr upEvent = &upEventRec;
    register int i;

    static EventSeqRec timerEventRec = {
	{0, 0, _XtEventTimerEventType, 0L, 0L},
	/* (StatePtr) -1 */ NULL,
	NULL,
	NULL
    };

    /* the event currently sitting in *eventP is an "up" event */
    /* we want to make it a "down" event followed by an "up" event, */
    /* so that sequence matching on the "state" side works correctly. */

    downEvent = event = *eventP;
    *upEvent = *downEvent;
    downEvent->event.eventType = ((event->event.eventType == ButtonRelease) ?
	ButtonPress : KeyPress);
    if ((downEvent->event.eventType == ButtonPress)
	&& (downEvent->event.modifiers != AnyModifier)
        && (downEvent->event.modifiers |downEvent->event.modifierMask !=0))
	downEvent->event.modifiers
	    &= ~buttonModifierMasks[event->event.eventCode];

    for (i=0; i<reps; i++) {

	/* up */
	event->next = XtNew(EventSeqRec);
	lastUpEvent = event = event->next;
	*event = *upEvent;

	/* timer */
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = timerEventRec;

	/* down */
	event->next = XtNew(EventSeqRec);
        event = event->next;
	*event = *downEvent;

	}

    event->next = lastUpEvent;
    *eventP = event;
    *actionsP = &lastUpEvent->actions;
}

static void RepeatOther(eventP, reps, actionsP)
    EventPtr *eventP;
    int reps;
    ActionPtr **actionsP;
{
    register EventPtr event, tempEvent;
    register int i;

    tempEvent = event = *eventP;

    for (i=1; i<reps; i++) {
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *tempEvent;
    }

    *eventP = event;
    *actionsP = &event->actions;
}

static void RepeatOtherPlus(eventP, reps, actionsP)
    EventPtr *eventP;
    int reps;
    ActionPtr **actionsP;
{
    register EventPtr event, tempEvent;
    register int i;

    tempEvent = event = *eventP;

    for (i=1; i<reps; i++) {
	event->next = XtNew(EventSeqRec);
	event = event->next;
	*event = *tempEvent;
    }

    event->next = event;
    *eventP = event;
    *actionsP = &event->actions;
}

static void RepeatEvent(eventP, reps, plus, actionsP)
    EventPtr *eventP;
    int reps;
    Boolean plus;
    ActionPtr **actionsP;
{
    switch ((*eventP)->event.eventType) {

	case ButtonPress:
	case KeyPress:
	    if (plus) RepeatDownPlus(eventP, reps, actionsP);
	    else RepeatDown(eventP, reps, actionsP);
	    break;

	case ButtonRelease:
	case KeyRelease:
	    if (plus) RepeatUpPlus(eventP, reps, actionsP);
	    else RepeatUp(eventP, reps, actionsP);
	    break;

	default:
	    if (plus) RepeatOtherPlus(eventP, reps, actionsP);
	    else RepeatOther(eventP, reps, actionsP);
    }
}

static String ParseRepeat(str, eventP, actionsP)
    register String str;
    EventPtr *eventP;
    ActionPtr **actionsP;
{
    int reps;
    Boolean plus = FALSE;

    /*** Parse the repetitions, for double click etc... ***/
    if (*str != '(') return str;
    str++;
    if (*str >= '0' && *str <= '9') {
	String start = str;
	char repStr[100];

	str = ScanNumeric(str);
	(void) strncpy(repStr, start, str-start);
	repStr[str-start] = '\0';
	reps = StrToNum(repStr);
    }
    else { Syntax("Missing number."); return ScanFor(str, ')'); }
    if (*str == '+') { plus = TRUE; str++; };
    if (*str == ')') str++;
    else { Syntax("Missing ')'."); return ScanFor(str, ')'); };

    if (reps > 1 || plus) RepeatEvent(eventP, reps, plus, actionsP);

    return str;
}

/***********************************************************************
 * ParseEventSeq
 * Parses the left hand side of a translation table production
 * up to, and consuming the ":".
 * Takes a pointer to a char* (where to start parsing) and returns an
 * event seq (in a passed in variable), having updated the String 
 **********************************************************************/

static String ParseEventSeq(str, eventSeqP, actionsP)
    register String str;
    EventSeqPtr *eventSeqP;
    ActionPtr	**actionsP;
{
    EventSeqPtr *nextEvent = eventSeqP;

    *eventSeqP = NULL;

    while (*str != ':' && *str != '\0' && *str != '\n') {
	static Event	nullEvent = {0, 0, 0, 0L, 0L};
	EventPtr	event;

	event = XtNew(EventRec);
        event->event = nullEvent;
	event->state = /* (StatePtr) -1 */ NULL;
        event->next = NULL;
        event->actions = NULL;

	str = ScanWhitespace(str);

	if (*str == '"') {
	    str++;
	    while (*str != '"' && *str != '\0' && *str != '\n') {
		str = ParseQuotedStringEvent(str, event);
		if (*nextEvent != NULL)
		    XtWarning("Events follow '+' repeat count.");
		*nextEvent = event;
		*actionsP = &event->actions;
		nextEvent = &event->next;
	    }
	    if (*str != '"') Syntax("Missing '\"'."); else str++;
	} else {
	    str = ParseEvent(str, event);
	    if (*nextEvent != NULL)
		XtWarning("Events follow '+' repeat count.");
	    *nextEvent = event;
	    *actionsP = &event->actions;
	    str = ParseRepeat(str, &event, actionsP);
	    nextEvent = &event->next;
	}
	str = ScanWhitespace(str);
	if (*str != ':')
	    if (*str != ',') {
		Syntax("',' or ':' expected.");
	    } else str++;
    }
    if (*str != ':') Syntax("Missing ':'."); else str++;

    return str;
}


static String ParseActionProc(str, actionProcNameP)
    register String str;
    String *actionProcNameP;
{
    register String start = str;
    char procName[100];

    str = ScanIdent(str);
    (void) strncpy(procName, start, str-start);
    procName[str-start] = '\0';

    *actionProcNameP = strncpy(
	XtMalloc((unsigned)(str-start+1)), procName, str-start+1);
    return str;
}


static String ParseString(str, strP)
    register String str;
    String *strP;
{
    register String start;

    if (*str == '"') {
	str++;
	start = str;
	while (*str != '"' && *str != '\0' && *str != '\n') str++;
	*strP = strncpy(XtMalloc((unsigned)(str-start+1)), start, str-start);
	(*strP)[str-start] = '\0';
	if (*str == '"') str++; else XtWarning("Missing '\"'.");
    } else {
	/* scan non-quoted string, stop on whitespace, ',' or ')' */
	start = str;
	while (*str != ' '
		&& *str != '\t'
		&& *str != ','
		&& *str != ')'
                && *str != '\n'
		&& *str != '\0') str++;
	*strP = strncpy(XtMalloc((unsigned)(str-start+1)), start, str-start);
	(*strP)[str-start] = '\0';
    }
    return str;
}


static String ParseParamSeq(str, paramSeqP, paramNumP)
    register String str;
    String **paramSeqP;
    unsigned long *paramNumP;
{
    typedef struct _ParamRec *ParamPtr;
    typedef struct _ParamRec {
	ParamPtr next;
	String	param;
    } ParamRec;

    ParamPtr params = NULL;
    register Cardinal num_params = 0;
    register Cardinal i;

    str = ScanWhitespace(str);
    while (*str != ')' && *str != '\0' && *str != '\n') {
	String newStr;
	str = ParseString(str, &newStr);
	if (newStr != NULL) {
	    ParamPtr temp = XtNew(ParamRec);

	    num_params++;
	    temp->next = params;
	    params = temp;
	    temp->param = newStr;
	    str = ScanWhitespace(str);
	    if (*str == ',') str = ScanWhitespace(++str);
	}
    }

    if (num_params != 0) {
	*paramSeqP = (String *)XtCalloc(
	    num_params+1, (unsigned) sizeof(String));
	*paramNumP = num_params;
	for (i=0; i < num_params; i++) {
	    ParamPtr temp = params;
	    (*paramSeqP)[num_params-i-1] = params->param;
	    params = params->next;
	    XtFree((char *)temp);
	}
	(*paramSeqP)[num_params] = NULL;
    } else {
	*paramSeqP = NULL;
	*paramNumP = 0;
    }

    return str;
}

static String ParseAction(str, actionP)
    String str;
    ActionPtr actionP;
{
    str = ParseActionProc(str, &actionP->token);
    if (*str == '(') {
	str++;
	str = ParseParamSeq(str, &actionP->params, &actionP->num_params);
    } else {
        Syntax("Missing '('");
        str = ")";		/* ignore rest of sequence */
    }

    if (*str == ')')
        str++;
    else {
        Syntax("Missing ')'");
	str = "";		/* ignore rest of sequence */
    }

    return str;
}


static String ParseActionSeq(stateTable,str, actionsP)
    XtTranslations stateTable;
    String str;
    ActionPtr *actionsP;
{
    ActionPtr *nextActionP = actionsP;
    int index;
    *actionsP = NULL;

    while (*str != '\0' && *str != '\n') {
	register ActionPtr	action;

	action = XtNew(ActionRec);
        action->token = NULL;
        action->index = -1;
        action->params = NULL;
        action->num_params = 0;
        action->next = NULL;

	str = ParseAction(str, action);
        index = stateTable->numQuarks++;
        if (index==stateTable->quarkTblSize) {
          stateTable->quarkTblSize +=20;
          stateTable->quarkTable=(XrmQuark*) XtRealloc(
                    (char*)stateTable->quarkTable,
                    stateTable->quarkTblSize*sizeof(int));
        }
        (stateTable->quarkTable)[index] = 
         StringToQuark(action->token);
        action->index=index;
	str = ScanWhitespace(str);
	*nextActionP = action;
	nextActionP = &action->next;
    }
    if (*str == '\n') str++;
    str = ScanWhitespace(str);
    return str;
}


/***********************************************************************
 * ParseTranslationTableProduction
 * Parses one line of event bindings.
 ***********************************************************************/

static String ParseTranslationTableProduction(stateTable, str)
  XtTranslations stateTable;
  register String str;
{
    EventSeqPtr	eventSeq = NULL;
    ActionPtr	*actionsP;

    currentProduction = str;	/* %%% a little nasty for multi-threading */

    str = ParseEventSeq(str, &eventSeq, &actionsP);
    str = ScanWhitespace(str);
    str = ParseActionSeq(stateTable,str, actionsP);

#ifdef notdef
    /* build the event mask and */
    /* put the actions at the end of the event chain */
    for (event = eventSeq; event != NULL; event=event->next) {
	event->state = NULL;
	if (event->next == NULL || event->next->state == NULL) {
	    event->actions = actions;
	    break;
	}
    }
#endif

    _XtAddEventSeqToStateTable(eventSeq, stateTable);

    FreeEventSeq(eventSeq);
    return (str);
}
XtTranslations _ParseTranslationTable (source)
    String   source;
{
    String str = source;
    XtTranslations stateTable;
    if (str == NULL) return ((XtTranslations)(NULL));
    _XtInitializeStateTable(&stateTable);


    while (str != NULL && *str != '\0') {
       str =  ParseTranslationTableProduction(stateTable,str);
    }
    return(stateTable);
}

/* ARGSUSED */
void _CompileTranslations (args, num_args, from, to)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr from,to;
{
    String str;
    static XtTranslations stateTable;

    if (*num_args != 0)
	XtWarning("String to TranslationTable needs no extra arguments");
     str = (String)(from->addr);
     if (str == NULL) {
         to->addr = NULL;
         to->size = 0;
         return;
     };
    _XtInitializeStateTable(&stateTable);


    while (str != NULL && *str != '\0') {
       str =  ParseTranslationTableProduction(stateTable,str);
    }
    to->addr= (caddr_t)&stateTable;
    to->size = sizeof(XtTranslations);
}

/*** public procedures ***/

/*
 * Parses a user's or applications translation table
 */
XtTranslations XtParseTranslationTable(source)
    String source;
{
    XrmValue from,to;
    from.addr = source;
    from.size = strlen(source)+1;
    XtDirectConvert((XtConverter) _CompileTranslations, (XrmValuePtr) NULL,
	    0, &from, &to);
    return (*(XtTranslations*)(to.addr));

}

void _XtTranslateInitialize()
{
    if (initialized) {
	XtWarning("Intializing Translation manager twice.");
	return;
    }

    initialized = TRUE;

    Compile_XtEventTable( events );
    CompileNameValueTable( modifiers );
    CompileNameValueTable( buttonNames );
    CompileNameValueTable( notifyModes );
    CompileNameValueTable( notifyDetail );
    CompileNameValueTable( visibilityNotify );
    CompileNameValueTable( circulation );
    CompileNameValueTable( propertyChanged );
    XtAddConverter(XtRString, XtRTranslationTable,
	    (XtConverter) _CompileTranslations, (XtConvertArgList) NULL, 0);
    _XtPopupInitialize();
}
