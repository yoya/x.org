#ifndef lint
static char rcsid[] = "$Header: TMstate.c,v 1.48 88/02/26 12:48:52 swick Exp $";
#endif lint
/*LINTLIBRARY*/

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

/* TMstate.c -- maintains the state table of actions for the translation 
 *              manager.
 */

#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/StringDefs.h>
#include <stdio.h>
#include "IntrinsicI.h"
#include "TMprivate.h"
#include <X11/Convert.h>


#define StringToAction(string)	((XtAction) StringToQuark(string))

static void FreeActions(action)
  register ActionPtr action;
{
    while (action != NULL) {
	register ActionPtr next = action->next;

	if (action->params != NULL) {
	    register Cardinal i;

	    for (i=0; i<action->num_params; i++) XtFree(action->params[i]);
	    XtFree((char *)action->params);
	}

	XtFree(action->token);
	XtFree((char *)action);
	action = next;
    }
}

static String PrintModifiers(str, mask, mod)
    String str;
    unsigned long mask, mod;
{
    if (mask & ShiftMask)
	(void) sprintf(str, "%sShift", ((mod & ShiftMask) ? "" : "~"));
    if (mask & ControlMask)
	(void) sprintf(str, "%sCtrl", ((mod & ControlMask) ? "" : "~"));
    if (mask & LockMask)
	(void) sprintf(str, "%sLock", ((mod & LockMask) ? "" : "~"));
    if (mask & Mod1Mask)
	(void) sprintf(str, "%sMeta", ((mod & Mod1Mask) ? "" : "~"));
    if (mask & Mod2Mask)
	(void) sprintf(str, "%sMod2", ((mod & Mod2Mask) ? "" : "~"));
    if (mask & Mod3Mask)
	(void) sprintf(str, "%sMod3", ((mod & Mod3Mask) ? "" : "~"));
    if (mask & Mod4Mask)
	(void) sprintf(str, "%sMod4", ((mod & Mod4Mask) ? "" : "~"));
    if (mask & Mod5Mask)
	(void) sprintf(str, "%sMod5", ((mod & Mod5Mask) ? "" : "~"));
    str += strlen(str);
    return str;
}

static String PrintEventType(str, event)
    register String str;
    unsigned long event;
{
    switch (event) {
#define PRINTEVENT(event) case event: (void) sprintf(str, "<event>"); break;
	PRINTEVENT(KeyPress)
	PRINTEVENT(KeyRelease)
	PRINTEVENT(ButtonPress)
	PRINTEVENT(ButtonRelease)
	PRINTEVENT(MotionNotify)
	PRINTEVENT(EnterNotify)
	PRINTEVENT(LeaveNotify)
	PRINTEVENT(FocusIn)
	PRINTEVENT(FocusOut)
	PRINTEVENT(KeymapNotify)
	PRINTEVENT(Expose)
	PRINTEVENT(GraphicsExpose)
	PRINTEVENT(NoExpose)
	PRINTEVENT(VisibilityNotify)
	PRINTEVENT(CreateNotify)
	PRINTEVENT(DestroyNotify)
	PRINTEVENT(UnmapNotify)
	PRINTEVENT(MapNotify)
	PRINTEVENT(MapRequest)
	PRINTEVENT(ReparentNotify)
	PRINTEVENT(ConfigureNotify)
	PRINTEVENT(ConfigureRequest)
	PRINTEVENT(GravityNotify)
	PRINTEVENT(ResizeRequest)
	PRINTEVENT(CirculateNotify)
	PRINTEVENT(CirculateRequest)
	PRINTEVENT(PropertyNotify)
	PRINTEVENT(SelectionClear)
	PRINTEVENT(SelectionRequest)
	PRINTEVENT(SelectionNotify)
	PRINTEVENT(ColormapNotify)
	PRINTEVENT(ClientMessage)
	case _XtEventTimerEventType: (void) sprintf(str,"<EventTimer>"); break;
	case _XtTimerEventType: (void) sprintf(str, "<Timer>"); break;
	default: (void) sprintf(str, "<0x%x>", (int) event);
#undef PRINTEVENT
    }
    str += strlen(str);
    return str;
}

static String PrintCode(str, mask, code)
    register String str;
    unsigned long mask, code;
{
    if (mask != 0) {
	if (mask != (unsigned long)~0L)
	    (void) sprintf(str, "0x%lx:0x%lx", mask, code);
	else (void) sprintf(str, "0x%lx", code);
	str += strlen(str);
    }
    return str;
}

static String PrintEvent(str, event)
    register String str;
    register Event *event;
{
    str = PrintModifiers(str, event->modifierMask, event->modifiers);
    str = PrintEventType(str, event->eventType);
    str = PrintCode(str, event->eventCodeMask, event->eventCode);
    return str;
}

static String PrintParams(str, params, num_params)
    register String str, *params;
    Cardinal num_params;
{
    register Cardinal i;
    for (i = 0; i<num_params; i++) {
	if (i != 0) (void) sprintf(str, ", ");
	str += strlen(str);
	(void) sprintf(str, "\"%s\"", params[i]);
	str += strlen(str);
    }
    return str;
}

static String PrintActions(str, actions, quarkTable)
    register String str;
    register ActionPtr actions;
    XrmQuark* quarkTable;
{
    while (actions != NULL && actions->token != NULL) {
        (void) sprintf(
            str, " %s(", XrmQuarkToString(quarkTable[actions->index]));
	str += strlen(str);
	str = PrintParams(str, actions->params, (Cardinal)actions->num_params);
	str += strlen(str);
	(void) sprintf(str, ")");
	str += strlen(str);
	actions = actions->next;
    }
    return str;
}

static int MatchEvent(translations, eventSeq) 
  XtTranslations translations;
  register EventSeqPtr eventSeq;
{
    register EventObjPtr eventTbl = translations->eventObjTbl;
    register int i;

/*
 * The use of "Any" as a modifier can cause obscure bugs since an incoming
 * event may match the "Any" alternative even though a more specific (and
 * correct) event is in the table. It's hard to know which event in the table
 * to match since either could be correct, depending on the circumstances.
 * It's unfortunate that there isn't a unique identifier for a given event...
 * The "any" should be used only when all else fails, but this complicates
 * the algorithms quite a bit. Relying on the order of the productions in the
 * translation table helps, but is not sufficient, both because the earlier
 * specific event may not apply to the current state, and because we can
 * merge translations, resulting in events in the table that are "out of
 * order"
 */
    for (i=0; i < translations->numEvents; i++) {
        if (
	       (eventTbl[i].event.eventType ==
	        (eventSeq->event.eventType & 0x7f)) /* stip send-event bit */
	    && (eventTbl[i].event.eventCode ==
		(eventTbl[i].event.eventCodeMask & eventSeq->event.eventCode))
	    && ((eventTbl[i].event.modifiers
		          & eventTbl[i].event.modifierMask)
		    == (eventSeq->event.modifiers
		          & eventTbl[i].event.modifierMask))
	   )
		return(i);
   }
    return(-1);
}


/*
 * there are certain cases where you want to ignore the event and stay
 * in the same state.
 */
static Boolean Ignore(event)
  EventSeqPtr event;   
{
    if (
           event->event.eventType == MotionNotify
/*
        || event->event.eventType == ButtonPress
	|| event->event.eventType == ButtonRelease
*/
       )
	    return TRUE;
    else
	    return FALSE;
}


static void XEventToTMEvent(event, tmEvent)
    register XEvent *event;
    register EventPtr tmEvent;
{
    tmEvent->event.eventCodeMask = 0;
    tmEvent->event.eventCode = 0;
    tmEvent->event.modifierMask = 0;
    tmEvent->event.modifiers = 0;
    tmEvent->event.eventType = event->type;

    switch (event->type) {

	case KeyPress:
	case KeyRelease:
	    tmEvent->event.modifiers = event->xkey.state;
	    event->xkey.state = 0;
	    tmEvent->event.eventCode = XLookupKeysym(&event->xkey, 0);
	    event->xkey.state = tmEvent->event.modifiers;
	    break;

	case ButtonPress:
 	    tmEvent->event.eventCode = event->xbutton.button;
	    tmEvent->event.modifiers = event->xbutton.state;
	    break;

	case ButtonRelease:
	    tmEvent->event.eventCode = event->xbutton.button;
	    tmEvent->event.modifiers = event->xbutton.state;
	    break;

	case MotionNotify:
	    tmEvent->event.modifiers = event->xmotion.state;
	    break;

	case EnterNotify:
	case LeaveNotify:
	    tmEvent->event.modifiers = event->xcrossing.state;
	    break;

	default:
	    break;
    }
}


static unsigned long GetTime(tm, event)
    TMRec* tm;
    register XEvent *event;
{
    switch (event->type) {

        case KeyPress:
	case KeyRelease:
	    return event->xkey.time;

        case ButtonPress:
	case ButtonRelease:
	    return event->xbutton.time;

#ifdef notdef
        case EnterWindow:
	case LeaveWindow:
	    return event->xcrossing.time;
#endif

#ifdef notdef
	case MotionNotify:
	    return tm->lastEventTime;
#endif
	default:
	    return tm->lastEventTime;

    }

}


/* ARGSUSED */
static void _XtTranslateEvent (w, closure, event)
    Widget w;
    caddr_t closure;
    register    XEvent * event;
{
    register XtTranslations stateTable = ((TMRec*)closure)->translations;
    StatePtr oldState;
    EventRec curEvent;
    StatePtr current_state = ((TMRec*)closure)->current_state;
    int     index;
    register ActionPtr actions;
    XtActionProc* proc_table = ((TMRec*)closure)->proc_table;
    TMRec* tm = (TMRec*)closure;
/* gross disgusting special case ||| */
    if ((event->type == EnterNotify || event->type == LeaveNotify)
        && event->xcrossing.detail == NotifyInferior)
	return;

    XEventToTMEvent (event, &curEvent);

#ifdef notdef
    if (event->type == ButtonPress
	    && stateTable->buttonUp
	    && stateTable->curState != NULL)
	if (
		(tm->lastEventTime + stateTable->clickTime)
		< event->xbutton.time
	    )
	    stateTable->curState = NULL;

    if (event->type == ButtonRelease)
	stateTable->buttonUp = TRUE;
    else if (event->type != MotionNotify)
        stateTable->buttonUp = FALSE;
#endif

    index = MatchEvent (stateTable, &curEvent);
    if (index == -1)
	/* some event came in that we don't have any states for */
	/* ignore it. */
	return;

    /* are we currently in some state other than ground? */
    if (current_state != NULL) {

	oldState = current_state;

	/* find this event in the current level */
	while (current_state != NULL) {
	    Event *ev;
	    /* does this state's index match? --> done */
	    if (current_state->index == index) break;

	    /* is this an event timer? */
	    ev = &stateTable->eventObjTbl[
		current_state->index].event;
	    if (ev->eventType == _XtEventTimerEventType) {

		/* does the succeeding state match? */
		StatePtr nextState = current_state->nextLevel;

		/* is it within the timeout? */
		if (nextState != NULL && nextState->index == index) {
		    unsigned long time = GetTime(tm, event);
		    unsigned long delta = ev->eventCode;
		    if (delta == 0) delta = stateTable->clickTime;
		    if (tm->lastEventTime + delta >= time) {
			current_state = nextState;
			break;
		    }
		}
	    }

	    /* go to next state */
	    current_state = current_state->next;
	}

	if (current_state == NULL)
	    /* couldn't find it... */
	    if (Ignore(&curEvent)) {
		/* ignore it. */
	        current_state = oldState;
		return;
	    } /* do ground state */
    }

    if (current_state == NULL) {
	/* check ground level */
	current_state = stateTable->eventObjTbl[index].state;
	if (current_state == NULL) return;
    }

    tm->lastEventTime = GetTime (tm, event);

    /* perform any actions */
    actions = current_state->actions;
    while (actions != NULL) {
	/* perform any actions */
     if (proc_table[actions->index] != NULL)
        (*(proc_table[actions->index]))(
		w, event, actions->params, &actions->num_params);
	actions = actions->next;
    }

    /* move into successor state */
    ((TMRec*)tm)->current_state = current_state->nextLevel;
}

static Boolean EqualEvents(event1, event2)
    Event *event1, *event2;
{
    return (
	   event1->eventType     == event2->eventType
	&& event1->eventCode     == event2->eventCode
	&& event1->eventCodeMask == event2->eventCodeMask
	&& event1->modifiers     == event2->modifiers
	&& event1->modifierMask  == event2->modifierMask);
}

static int GetEventIndex(stateTable, event)
    XtTranslations stateTable;
    register EventPtr event;
{
    register int	index;
    register EventObjPtr new;
    register EventObjPtr eventTbl = stateTable->eventObjTbl;

    for (index=0; index < stateTable->numEvents; index++)
        if (EqualEvents(&eventTbl[index].event, &event->event)) return(index);

    if (stateTable->numEvents == stateTable->eventTblSize) {
        stateTable->eventTblSize += 10;
	stateTable->eventObjTbl = (EventObjPtr) XtRealloc(
	    (char *)stateTable->eventObjTbl, 
	    stateTable->eventTblSize*sizeof(EventObjRec));
    }

    new = &stateTable->eventObjTbl[stateTable->numEvents];

    new->event = event->event;
    new->state = NULL;

    return stateTable->numEvents++;
}

static StatePtr NewState(index, stateTable)
    int index;
    XtTranslations stateTable;
{
    register StatePtr state = XtNew(StateRec);

    state->index = index;
    state->nextLevel = NULL;
    state->next = NULL;
    state->actions = NULL;
    state->forw = stateTable->head;
    state->cycle = FALSE;
    stateTable->head = state;
/*
    state->back = NULL;
    if (state->forw != NULL) state->forw->back = state;
*/

    return state;
}

typedef NameValueRec CompiledAction;
typedef NameValueTable CompiledActionTable;

#ifdef lint
Opaque _CompileActionTable(actions, count)
#else
CompiledActionTable _CompileActionTable(actions, count)
#endif
    register struct _XtActionsRec *actions;
    register Cardinal count;
{
    register int i;
    register CompiledActionTable compiledActionTable;

    compiledActionTable = (CompiledActionTable) XtCalloc(
	count+1, (unsigned) sizeof(CompiledAction));

    for (i=0; i<count; i++) {
	compiledActionTable[i].name = actions[i].string;
	compiledActionTable[i].signature = StringToAction(actions[i].string);
	compiledActionTable[i].value = (Value) actions[i].proc;
    }

    compiledActionTable[count].name = NULL;
    compiledActionTable[count].signature = NULL;
    compiledActionTable[count].value = NULL;

#ifdef lint
    return (Opaque) compiledActionTable;
#else
    return compiledActionTable;
#endif
}

static EventMask EventToMask(event)
    EventObjPtr	event;
{
static EventMask masks[] = {
        0,			    /* Error, should never see  */
        0,			    /* Reply, should never see  */
        KeyPressMask,		    /* KeyPress			*/
        KeyReleaseMask,		    /* KeyRelease		*/
        ButtonPressMask,	    /* ButtonPress		*/
        ButtonReleaseMask,	    /* ButtonRelease		*/
        PointerMotionMask	    /* MotionNotify		*/
		| Button1MotionMask
		| Button2MotionMask
		| Button3MotionMask
		| Button4MotionMask
		| Button5MotionMask
		| ButtonMotionMask,
        EnterWindowMask,	    /* EnterNotify		*/
        LeaveWindowMask,	    /* LeaveNotify		*/
        FocusChangeMask,	    /* FocusIn			*/
        FocusChangeMask,	    /* FocusOut			*/
        KeymapStateMask,	    /* KeymapNotify		*/
        ExposureMask,		    /* Expose			*/
        0,			    /* GraphicsExpose, in GC    */
        0,			    /* NoExpose, in GC		*/
        VisibilityChangeMask,       /* VisibilityNotify		*/
        SubstructureNotifyMask,     /* CreateNotify		*/
        StructureNotifyMask,	    /* DestroyNotify		*/
/*		| SubstructureNotifyMask, */
        StructureNotifyMask,	    /* UnmapNotify		*/
/*		| SubstructureNotifyMask, */
        StructureNotifyMask,	    /* MapNotify		*/
/*		| SubstructureNotifyMask, */
        SubstructureRedirectMask,   /* MapRequest		*/
        StructureNotifyMask,	    /* ReparentNotify		*/
/*		| SubstructureNotifyMask, */
        StructureNotifyMask,	    /* ConfigureNotify		*/
/*		| SubstructureNotifyMask, */
        SubstructureRedirectMask,   /* ConfigureRequest		*/
        StructureNotifyMask,	    /* GravityNotify		*/
/*		| SubstructureNotifyMask, */
        ResizeRedirectMask,	    /* ResizeRequest		*/
        StructureNotifyMask,	    /* CirculateNotify		*/
/*		| SubstructureNotifyMask, */
        SubstructureRedirectMask,   /* CirculateRequest		*/
        PropertyChangeMask,	    /* PropertyNotify		*/
        0,			    /* SelectionClear		*/
        0,			    /* SelectionRequest		*/
        0,			    /* SelectionNotify		*/
        ColormapChangeMask,	    /* ColormapNotify		*/
        0,			    /* ClientMessage		*/
        0 ,			    /* MappingNotify		*/
    };

    /* Events sent with XSendEvent will have high bit set. */
    unsigned long eventType = event->event.eventType & 0x7f;

    return ((eventType >= XtNumber(masks)) ?  0 : masks[eventType]);
}
/*** Public procedures ***/

void _XtInstallTranslations(widget, stateTable)
    Widget widget;
    XtTranslations stateTable;
{
    register EventMask	eventMask = 0;
    register Boolean	nonMaskable = FALSE;
    register Cardinal	i;
    static struct {
        unsigned long	modifier;
	EventMask	mask;
      } buttonMotionMask[] = {
	{Button1Mask, Button1MotionMask}, 
	{Button2Mask, Button2MotionMask}, 
	{Button3Mask, Button3MotionMask}, 
	{Button4Mask, Button4MotionMask}, 
	{Button5Mask, Button5MotionMask},
      };

/*    widget->core.translations = stateTable; */
    if (stateTable == NULL) return;

    for (i = 0; i < stateTable->numEvents; i++) {
	register EventObjPtr eventObj = &stateTable->eventObjTbl[i];
	register EventMask mask = EventToMask(eventObj);
	unsigned long modifiers = eventObj->event.modifiers;

	if ((eventObj->event.eventType == MotionNotify)
	    && (modifiers & AnyButtonModifier)) {
	    /* optimize traffic when PointerMotion only with button down */
	    if (modifiers == AnyButtonModifier)
	        mask = ButtonMotionMask;
	    else {
	        int mod;
	        mask = 0;
		for (mod = 0; mod < XtNumber(buttonMotionMask); mod++) {
		    if (modifiers & buttonMotionMask[mod].modifier)
		        mask |= buttonMotionMask[mod].mask;
		}
	    }
	}

	eventMask |= mask;
	nonMaskable |= (mask == 0);
    }

    /* double click needs to make sure that you have selected on both
	button down and up. */

    if (eventMask & ButtonPressMask) eventMask |= ButtonReleaseMask;
    if (eventMask & ButtonReleaseMask) eventMask |= ButtonPressMask;

    XtAddEventHandler(
        widget, eventMask, nonMaskable,
             _XtTranslateEvent, (caddr_t)&widget->core.tm);

}

typedef struct _ActionListRec *ActionList;
typedef struct _ActionListRec {
    ActionList		next;
    CompiledActionTable table;
} ActionListRec;

static ActionList globalActionList = NULL;

static void ReportUnboundActions(tm, stateTable)
    TMRec* tm;
    XtTranslations stateTable;
{
    Cardinal num_unbound;
    char     message[10000];
    register Cardinal num_chars;
    register Cardinal i;

    num_unbound = 0;
    (void) strcpy(message, "Actions not found: ");
    num_chars = strlen(message);

    for (i=0; i < stateTable->numQuarks; i++) {
	if (tm->proc_table[i] == NULL) {
	    String s = XrmQuarkToString(stateTable->quarkTable[i]);
	    if (num_unbound != 0) {
		(void) strcpy(&message[num_chars], ", ");
		num_chars = num_chars + 2;
	    }
	    (void) strcpy(&message[num_chars], s);
	    num_chars += strlen(s);
	    num_unbound++;
	}
    }
    message[num_chars] = '\0';
    if (num_unbound != 0) XtWarning(message);
}


static int BindActions(tm, compiledActionTable,index)
    TMRec* tm;
    CompiledActionTable compiledActionTable;
    Cardinal index;
{
    XtTranslations stateTable=tm->translations;
    int unbound = stateTable->numQuarks;
    int i;

    for ( ; index < stateTable->numQuarks; index++) {
       if (tm->proc_table[index] == NULL) {
           /* attempt to bind it */
           register XrmQuark q = stateTable->quarkTable[index];
           for (i = 0; compiledActionTable[i].name != NULL; i++) {
               if (compiledActionTable[i].signature == q) {
                   tm->proc_table[index] =
                     (XtActionProc) compiledActionTable[i].value;
                   unbound--;
                   break;
               }
           }
       } else {
           /* already bound, leave it alone */
           unbound--;
       }
     }
     return(unbound);
}


void _XtBindActions(widget,tm,index)
    Widget	    widget;
    TMRec*          tm;
    Cardinal        index;
{
    XtTranslations  stateTable=tm->translations;
    register Widget	    w;
    register WidgetClass    class;
    register ActionList     actionList;
    int unbound = -1; /* initialize to non-zero */

/* ||| Kludge error that Leo depends upon */
w = widget;
if (stateTable == NULL) return;
if (tm->proc_table == NULL) 
    tm->proc_table= (XtActionProc*) XtCalloc(
                      stateTable->numQuarks,sizeof(XtActionProc));
do {
/* ||| */
    class = w->core.widget_class;
    do {
        if (class->core_class.actions != NULL)
         unbound = BindActions(
	    tm,(CompiledActionTable)class->core_class.actions, index);
	class = class->core_class.superclass;
    } while (unbound != 0 && class != NULL);
/* ||| Kludge error that Leo depends upon */
w = w->core.parent;
} while (unbound != 0 && w != NULL);
/* ||| */

    actionList = globalActionList;
    for (actionList = globalActionList; 
	 unbound != 0 && actionList != NULL;
	 actionList = actionList->next) {
	unbound = BindActions(tm, actionList->table,index);
    }
    if (unbound != 0) ReportUnboundActions(tm, stateTable);
}

void XtAddActions(actions, num_actions)
    XtActionList actions;
    Cardinal num_actions;
{
    register ActionList rec;

    rec = XtNew(ActionListRec);
    rec->next = globalActionList;
    globalActionList = rec;
    rec->table = (CompiledActionTable)_CompileActionTable(actions, num_actions);
}

void _XtInitializeStateTable(pStateTable)
    XtTranslations *pStateTable;
{
    register XtTranslations  stateTable;

    (*pStateTable) = stateTable = XtNew(TranslationData);
    stateTable->numEvents = 0;
    stateTable->eventTblSize = 0;
    stateTable->eventObjTbl = NULL;
    stateTable->clickTime = 200; /* ||| need some way of setting this !!! */
    stateTable->head = NULL;
    stateTable->quarkTable =
        (XrmQuark *)XtCalloc((unsigned)sizeof(XrmQuark), 20);
    stateTable->quarkTblSize = 20;
    stateTable->numQuarks = 0;
}

void _XtAddEventSeqToStateTable(eventSeq, stateTable)
    register EventSeqPtr eventSeq;
    XtTranslations stateTable;
{
    register int     index;
    register StatePtr *state;

    if (eventSeq == NULL) return;

    /* initialize event index and state ptr */
    /* note that all states in the event seq passed in start out null */
    /* we fill them in with the matching state as we traverse the list */

    index = GetEventIndex (stateTable, eventSeq);
    state = &stateTable->eventObjTbl[index].state;

    for (;;) {
    /* index is eventIndex for event */
    /* *state is head of state chain for current state */

	while (*state != NULL && (*state)->index != index)
	    state = &(*state)->next;
	if (*state == NULL) *state = NewState (index, stateTable);

	/* *state now points at state record matching event */
	eventSeq->state = *state;

	if (eventSeq->actions != NULL) {
	    if ((*state)->actions != NULL) {
		XtWarning ("Overriding earlier translation manager actions.");
		FreeActions((*state)->actions);
	    }
	    (*state)->actions = eventSeq->actions;
	}

    /* are we done? */
	eventSeq = eventSeq->next;
	if (eventSeq == NULL) break;
	if (eventSeq->state != NULL) {
	    /* we've been here before... must be a cycle in the event seq. */
	    (*state)->nextLevel = eventSeq->state;
	    (*state)->cycle = TRUE;
	    break;
	}

	if ((*state)->cycle) {

	    /* unroll the loop one state */
	    /* this code hurts my head... ||| think about multiple */
	    /* states pointing at same "next" state record */

	    StatePtr oldNextLevel = (*state)->nextLevel;
	    register StatePtr newNextLevel =
		NewState(oldNextLevel->index, stateTable);

	    newNextLevel->actions = oldNextLevel->actions;
	    newNextLevel->nextLevel = oldNextLevel->nextLevel;
	    newNextLevel->next = oldNextLevel->next;
	    newNextLevel->cycle = TRUE;
	    (*state)->cycle = FALSE;
	    (*state)->nextLevel = newNextLevel;
	}
	state = &(*state)->nextLevel;
	index = GetEventIndex (stateTable, eventSeq);
    }
}


typedef struct _StateMapRec *StateMap;
typedef struct _StateMapRec {
    StatePtr	old, new;
    StateMap	next;
} StateMapRec;

static void MergeStates(old, new, override, indexMap,
                           quarkIndexMap, oldTable, stateMap)
    register StatePtr *old, new;
    Boolean override;
    Cardinal *indexMap, *quarkIndexMap;
    XtTranslations oldTable;
    StateMap stateMap;
{
    register StatePtr state;
    StateMap oldStateMap = stateMap;
    ActionRec *a,**aa,*b;

    while (new != NULL) {
	register int index = indexMap[new->index];

	/* make sure old and new match */
	for (state = *old; ; state=state->next) {
	    if (state == NULL) {
		/* corresponding arc doesn't exist, add it */
		state = NewState(index, oldTable);
		state->next = *old;
		*old = state;
		break;
	    }

	    if (state->index == index) /* found it */ break;
	}
    
	/* state and new are pointing at corresponding state records */
	{
	    StateMap temp = XtNew(StateMapRec);
	    temp->next = stateMap;
	    stateMap = temp;
	    temp->old = state;
	    temp->new = new;
	}
    
	/* merge the actions */
	while (state->actions != NULL && override) {
	   a = state->actions;
	   state->actions=a->next;
	   XtFree((char *)a);
	}
      if (state->actions == NULL) {
        aa = &(state->actions);
        b = new->actions;
        while (b != NULL) {
           a = XtNew(ActionRec); 
           a->token = NULL;
           a->index = quarkIndexMap[b->index];
           a->params = b->params;
           a->num_params=b->num_params;
           a->next = NULL;
           *aa = a;
           aa = &a->next;
           b=b->next;
        }
      }


                     
	if (new->cycle) {
	    /* we're in a cycle, search state map for corresponding state */
	    register StateMap temp;
	    for (
		temp=stateMap;
		temp->new != new->nextLevel;
		temp=temp->next)
	        if (temp == NULL) XtError(
"Trying to merge translation tables with cycles, and can't resolve this cycle."
);
	    (*old)->nextLevel = temp->old;
	} else if (! (*old)->cycle || override) {
	    if ((*old)->cycle) (*old)->nextLevel = NULL;
	    MergeStates(
	        &(*old)->nextLevel,
		new->nextLevel,
		override,
		indexMap,quarkIndexMap,
		oldTable,
		stateMap);
	}
    
	new = new->next;
    }
    while (stateMap != oldStateMap) {
	StateMap temp = stateMap;
	stateMap = stateMap->next;
	XtFree((char *)temp);
    }
}


static void MergeTables(old, new, override)
    register XtTranslations old, new;
    Boolean override;
{
    register Cardinal i;
    Cardinal *indexMap,*quarkIndexMap;

    if (new == NULL) return;
    if (old == NULL) {
	XtWarning("Old translation table was null, cannot modify.");
	return;
    }

    indexMap = (Cardinal *)XtCalloc(new->eventTblSize, sizeof(Cardinal));

    for (i=0; i < new->numEvents; i++) {
	register Cardinal j;
	EventObjPtr newEvent = &new->eventObjTbl[i];

	for (j=0; j < old->numEvents; j++)
	    if (EqualEvents(
	        &newEvent->event, &old->eventObjTbl[j].event)) break;

	if (j==old->numEvents) {
	    if (j == old->eventTblSize) {
		old->eventTblSize += 10;
		old->eventObjTbl = (EventObjPtr) XtRealloc(
		    (char *)old->eventObjTbl, 
		    old->eventTblSize*sizeof(EventObjRec));
	    }
	    old->eventObjTbl[j] = *newEvent;
	    old->eventObjTbl[j].state = NULL;
	    old->numEvents++;
	}
	indexMap[i] = j;
    }
/* merge quark tables */
  quarkIndexMap = (Cardinal *)XtCalloc(new->quarkTblSize, sizeof(Cardinal));


    for (i=0; i < new->numQuarks; i++) {
        register Cardinal j;

       for (j=0; j < old->numQuarks; j++)
            if (old->quarkTable[j] == new->quarkTable[i]) break;
                

       if (j==old->numQuarks) {
            if (j == old->quarkTblSize) {
                old->quarkTblSize += 20;
                old->quarkTable = (XrmQuark*) XtRealloc(
                    (char *)old->quarkTable,
                    old->quarkTblSize*sizeof(int));
                  }
            old->quarkTable[j]=new->quarkTable[i];
            old->numQuarks++;
        }
        quarkIndexMap[i] = j;
    }




    for (i=0; i < new->numEvents; i++)
	MergeStates(
	    &old->eventObjTbl[indexMap[i]].state,
	    new->eventObjTbl[i].state,
	    override,
	    indexMap,quarkIndexMap,
	    old,
	    (StateMap) NULL);
   XtFree((char *)indexMap);
   XtFree((char *)quarkIndexMap);
}


void _XtOverrideTranslations(old, new,merged)
    XtTranslations old, new,*merged;
{
    XtTranslations temp;

    _XtInitializeStateTable(&temp);
    temp->clickTime = new->clickTime;
    /* merge in new table, overriding any existing bindings from old */
    MergeTables(temp, new, FALSE);
    MergeTables(temp, old, FALSE);
    *merged= temp;
}


void _XtAugmentTranslations(old, new,merged)
    XtTranslations old, new,*merged;
{
    /* merge in extra bindings, keeping old binding if any */
    XtTranslations temp;
    _XtInitializeStateTable(&temp);
    temp->clickTime = old->clickTime;
    MergeTables(temp, old, FALSE);
    MergeTables(temp, new, FALSE);
    *merged= temp;
}

/* ARGSUSED */
void _MergeTranslations (args, num_args, from, to)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr from,to;
{
    static XtTranslations merged;
    XtTranslations old,new;
    TMkind operation;

    if (*num_args != 0)
	XtWarning("MergeTM to TranslationTable needs no extra arguments");

    old = ((TMConvertRec*)from->addr)->old;
    new = ((TMConvertRec*)from->addr)->new;
    operation = ((TMConvertRec*)from->addr)->operation;
    if (operation == override)
    _XtOverrideTranslations(old, new,&merged);
    else
    if (operation == augment)
    _XtAugmentTranslations(old,new,&merged);
     to->addr= (caddr_t)&merged;
     to->size=sizeof(XtTranslations);
}

void XtOverrideTranslations(widget, new)
    Widget widget;
    XtTranslations new;
{
/*
    MergeTables(widget->core.translations, new, TRUE);
*/
     Cardinal  numQuarks =0;
    XrmValue from,to;
    TMConvertRec foo;
    from.addr = (caddr_t)&foo;
    from.size = sizeof(TMConvertRec);
    foo.old = widget->core.tm.translations;
    foo.new = new;
    foo.operation = override;
     if (widget->core.tm.translations != NULL)
      numQuarks = widget->core.tm.translations->numQuarks;
    XtDirectConvert((XtConverter) _MergeTranslations, (XrmValuePtr) NULL,
	    0, &from, &to);
/*    _XtOverrideTranslations(widget->core.tm.translations, new);*/
      widget->core.tm.translations =(*(XtTranslations*)to.addr);
     if (XtIsRealized(widget))
        _XtBindActions(widget,&widget->core.tm,numQuarks);
        
}

void XtAugmentTranslations(widget, new)
    Widget widget;
    XtTranslations new;
{
    Cardinal  numQuarks =0;
    XrmValue from,to;
    TMConvertRec foo;
    from.addr = (caddr_t)&foo;
    from.size = sizeof(TMConvertRec);
    foo.old = widget->core.tm.translations;
    foo.new = new;
    foo.operation = augment;
     if (widget->core.tm.translations != NULL)
      numQuarks = widget->core.tm.translations->numQuarks;
    XtDirectConvert((XtConverter) _MergeTranslations, (XrmValuePtr) NULL,
	    0, &from, &to);
/*    _XtAugmentTranslations(widget->core.tm.translations, new);*/
     widget->core.tm.translations = (*(XtTranslations*)to.addr);
     if (XtIsRealized(widget)) 
        _XtBindActions(widget,&widget->core.tm,numQuarks);

}

static void PrintState(start, str, state, quarkTable, eot)
    register String start, str;
    StatePtr state;
    XrmQuark* quarkTable;
    EventObjPtr eot;
{
    register String old = str;
    /* print the current state */
    if (state == NULL) return;

    str = PrintEvent(str, &eot[state->index].event);
    str += strlen(str);
    if (state->actions != NULL) {
	String temp = str;
	(void) sprintf(str, "%s: ", (state->cycle ? "(+)" : ""));
	while (*str) str++;
	(void) PrintActions(str, state->actions, quarkTable);
	(void) printf("%s\n", start);
	str = temp; *str = '\0';
    }

    /* print succeeding states */
    if (!state->cycle)
	PrintState(start, str, state->nextLevel, quarkTable, eot);

    str = old; *str = '\0';

    /* print sibling states */
    PrintState(start, str, state->next, quarkTable, eot);
    *str = '\0';

}

#ifdef lint
void TranslateTablePrint(translations)
#else
static void TranslateTablePrint(translations)
#endif
    XtTranslations translations;
{
    register Cardinal i;
    char buf[1000];

    for (i = 0; i < translations->numEvents; i++) {
	buf[0] = '\0';
	PrintState(
	   &buf[0],
	   &buf[0],
	   translations->eventObjTbl[i].state,
           translations->quarkTable,
	   translations->eventObjTbl);
    }
}

/***********************************************************************
 *
 * Pop-up and Grab stuff
 *
 ***********************************************************************/

static Boolean XtConvertStringToBoolean(s, bP)
    String s;
    Boolean *bP;
{
    XrmValue fromVal, toVal;

    fromVal.addr = s;
    fromVal.size = strlen(s)+1;

    XtConvert((Widget) NULL, XtRString, &fromVal, XtRBoolean, &toVal);
    if (toVal.addr == NULL) return FALSE;

    *bP = (Boolean) *(int *)toVal.addr;

    return TRUE;
}

static Widget _XtFindPopup(widget, name)
    Widget widget;
    String name;
{
    register Cardinal i;
    register XrmQuark q;
    register Widget w;

    q = XrmStringToQuark(name);

    for (w=widget; w != NULL; w=w->core.parent)
	for (i=0; i<w->core.num_popups; i++)
	    if (w->core.popup_list[i]->core.xrm_name == q)
		return w->core.popup_list[i];

    return NULL;
}

/*ARGSUSED*/
static void _XtMenuPopup(widget, event, params, num_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal num_params;
{
    Boolean spring_loaded;
    register Widget popup_shell;

    if (num_params != 2)
	XtError("_XtMenuPopup called with num_params != 2.");

    if (! XtConvertStringToBoolean(params[1], &spring_loaded))
	XtError("Bad first parameter to _XtMenuPopup.");

    popup_shell = _XtFindPopup(widget, params[0]);
    if (popup_shell == NULL) XtError("Can't find popup in _XtMenuPopup");

    if (spring_loaded) _XtPopup(popup_shell, XtGrabExclusive, TRUE);
    else _XtPopup(popup_shell, XtGrabNonexclusive, FALSE);
}

static void _XtMenuPopupAction(widget, event, params, num_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    String newParams[2];

    if (*num_params != 1) XtError("MenuPopup wants exactly one argument.");

    newParams[0] = params[0];
    if (event->type == ButtonPress) newParams[1] = "True";
    else if (event->type == EnterNotify) newParams[1] = "False";
    else {
	XtWarning(
"Pop-up menu creation is only supported on ButtonPress or EnterNotify events."
);
	newParams[1] = "True";
    }

    _XtMenuPopup(widget, event, newParams, 2);
}


/*ARGSUSED*/
static void _XtMenuPopdownAction(widget, event, params, num_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    XtPopdown(widget);
}


void _XtRegisterGrabs(widget,tm)
    Widget widget;
    TMRec*  tm;
{
    XtTranslations stateTable=tm->translations;
    unsigned int count;

    if (! XtIsRealized(widget)) return;

    /* walk the widget instance action bindings table looking for */
    /* _XtMenuPopupAction */
    /* when you find one, do a grab on the triggering event */

    if (stateTable == NULL) return;
    for (count=0; count < stateTable->numQuarks; count++) {
       if (tm->proc_table[count] ==
            (XtActionProc)(_XtMenuPopupAction)) {
	    register StatePtr state;
	    /* we've found a "grabber" in the action table. Find the */
	    /* states that call this action. */
	    /* note that if there is more than one "grabber" in the action */
	    /* table, we end up searching all of the states multiple times. */
	    for (state=stateTable->head; state != NULL; state=state->forw) {
		register ActionPtr action;
	        for (
		    action = state->actions;
		    action != NULL;
		    action=action->next) {
		    if (action->index == count) {
			/* this action is a "grabber" */
			register Event *event;
			event = &stateTable->eventObjTbl[state->index].event;
			switch (event->eventType) {
			    case ButtonPress:
			    case ButtonRelease:
				XGrabButton(
				    XtDisplay(widget),
				    event->eventCode,
				    event->modifiers,
				    XtWindow(widget),
				    TRUE,
				    NULL,
				    GrabModeAsync,
				    GrabModeAsync,
				    None,
				    None
				);
				break;
	    
			    case KeyPress:
			    case KeyRelease:
				XGrabKey(
				    XtDisplay(widget),
				    event->eventCode,
				    event->modifiers,
				    XtWindow(widget),
				    TRUE,
				    NULL,
				    GrabModeAsync,
				    GrabModeAsync
				);
				break;
	    
			    default: XtWarning(
	       "Popping up menus is only supported on key and button events.");
			    break;
			}
		    }
		}
	    }
	}
    }
}

static XtActionsRec tmActions[] = {
    {"MenuPopup", _XtMenuPopupAction},
    {"MenuPopdown", _XtMenuPopdownAction},
};


void _XtPopupInitialize() { XtAddActions(tmActions, XtNumber(tmActions)); }
