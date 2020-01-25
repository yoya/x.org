/*
* $Header: TMprivate.h,v 1.21 88/02/26 12:48:30 swick Exp $
*/

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

/* 
 * TMprivate.h - Header file private to translation management
 * 
 * Author:	Charles Haynes
 * 		Digital Equipment Corporation
 * 		Western Research Laboratory
 * Date:	Sat Aug 29 1987
 */

typedef short ModifierMask;

#define AnyButtonModifier \
    (Button1Mask|Button2Mask|Button3Mask|Button4Mask|Button5Mask)

typedef struct _EventObjRec *EventObjPtr;
typedef struct _EventRec {
    unsigned long modifiers;
    unsigned long modifierMask;
    unsigned long eventType;
    unsigned long eventCode;
    unsigned long eventCodeMask;
} Event;

typedef enum _TMkind {override,augment} TMkind;

typedef struct _TMConvertRec {
   XtTranslations old; /* table to merge into */
   XtTranslations new; /* table to merge from */
   TMkind  operation; /* merge or augment     */
} TMConvertRec;

typedef struct _EventObjRec {
    Event event;	/* X event description */
    StatePtr state;	/* pointer to linked lists of state info */
} EventObjRec;
typedef struct _ActionsRec *ActionPtr;
typedef struct _ActionsRec {
    char * token;		/* string token for procedure name */
    unsigned int index;         /* index into quarkTable to find proc */
    String *params;		/* pointer to array of params */
    unsigned long num_params;	/* number of params */
    ActionPtr next;		/* next action to perform */
} ActionRec;

typedef struct _StateRec {
    int index;		/* index of event into EventObj table */
    ActionPtr actions;	/* rhs   list of actions to perform */
    StatePtr nextLevel;	/* the next level points to the next event
			   in one event sequence */
    StatePtr next;	/* points to other event state at same level */
    StatePtr forw;	/* points to next state in list of all states */
    Boolean cycle;	/* true iff nextLevel is a loop */
}  StateRec;

typedef struct _TranslationData {
    unsigned int	numEvents;
    unsigned int	eventTblSize;
    EventObjPtr		eventObjTbl;
    unsigned long	clickTime;
    unsigned long	lastEventTime;
    unsigned int        numQuarks;   /* # of entries in quarkTable */
    unsigned int        quarkTblSize; /*total size of quarkTable */
    XrmQuark*           quarkTable;  /* table of quarkified rhs*/
    StatePtr		head;	/* head of list of all states */
    
} TranslationData;

#define _XtEventTimerEventType ((unsigned long)-1L)
#define _XtTimerEventType ((unsigned long)-2L)

typedef struct _EventSeqRec *EventSeqPtr;
typedef struct _EventSeqRec {
    Event event;	/* X event description */
    StatePtr state;	/* private to state table builder */
    EventSeqPtr next;	/* next event on line */
    ActionPtr actions;	/* r.h.s.   list of actions to perform */
} EventSeqRec;

typedef EventSeqRec EventRec;
typedef EventSeqPtr EventPtr;

extern void _XtAddEventSeqToStateTable();
extern void _XtInitializeStateTable(); /* stateTable */
    /* _XtTranslations *stateTable; */

typedef XrmQuark XtAction;

typedef unsigned int	Value;
typedef struct {
    char	*name;
    XrmQuark	signature;
    Value	value;
} NameValueRec, *NameValueTable;

extern Boolean _XtLookupTableSym(); /* table, name, valueP */
    /* NameValueTable	table; */
    /* String name; */
    /* Value *valueP; */

extern void _XtPopupInitialize();


