/*
 *	$Source: /u1/X/Sx/code/RCS/sxSelect.c,v $
 *	$Header: sxSelect.c,v 1.2 86/12/17 14:46:34 swick Exp $
 */

#ifndef lint
static char *rcsid_sxSelect_c = "$Header: sxSelect.c,v 1.2 86/12/17 14:46:34 swick Exp $";
#endif	lint

/* 
 * sxSelect.c --
 *
 *	This file implements a standard way of dealing with the
 *	current window selection.  It has two major features:
 *	1. Selections come in different formats:  a selection consists
 *	   of a format (which is a character string) and an arbitrary
 *	   bunch of bytes.
 *	2. The selection is accessed by callback procedures.  Rather
 *	   than storing the selection in a common place, the module
 *	   that "owns" the selection provides a procedure to call
 *	   whenever the contents of the selection are needed.
 *
 * Copyright (C) 1986 Regents of the University of California.
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: sxSelect.c,v 1.2 86/12/17 14:46:34 swick Exp $ SPRITE (Berkeley)";
#endif not lint

/* #include <X/Xlib.h> */
#include "XlibInternal.h"
#include <sys/time.h>
#include "sprite.h"
#include "byte.h"
#include "mem.h"
#include "string.h"
#include "sx.h"

/*
 * The information below keeps track of the current selection, if it's
 * owned by this client.
 */

static Boolean weveGotIt = FALSE;	/* TRUE means either this client owns
					 * selection;  if selFetchProc is NULL,
					 * then there is no selection. */
static int (*selFetchProc)() = NULL;	/* Procedure to call to fetch
					 * selection, or NULL. */
static void (*selChangeProc)();		/* Procedure to call when selection
					 * changes. */
static ClientData selClientData;	/* Argument to pass to procedures. */

/*
 * Each client keeps a tiny transparent window, which is used solely
 * for the purpose of signalling between clients.  One client signals
 * another by unmapping its window;  this generates an UnmapWindow
 * event to goose the other client into action.
 */

static Window signalWindow = 0;		/* Window that this client uses
					 * to receive signals.  0 means the
					 * window hasn't been created yet. */

/*
 * Three of the cut buffers are used to hold information involved
 * in passing the selection between clients:
 *
 * OWNER_BUFFER:	holds 4-byte window id for current selection
 *			owner's signalWindow.
 * REQUEST_BUFFER:	holds request for selection information, if
 *			there is an unprocessed request.
 * ANSWER_BUFFER:	holds response from current selection owner,
 *			if there is an unprocessed answer.
 */

#define OWNER_BUFFER 3
#define REQUEST_BUFFER 4
#define ANSWER_BUFFER 5

/*
 * Maximum number of bytes of selection that may be requested at once:
 */

#define BYTES_AT_ONCE 1024

/*
 * The following two data structures are used to pass selection
 * information between clients.  Request records are sent to
 * the current selection owner, and it responds with Answer
 * records.
 */

typedef struct {
    char desiredFormat[SX_FORMAT_SIZE];	/* Desired format for selection,
					 * null-terminated. */
    int firstByte;			/* First byte of selection to return.*/
    int numBytes;			/* How many bytes of selection to
					 * return.  A request size of -1
					 * means that selection ownership is
					 * switching to the requesting
					 * client. */
    Window signalWindow;		/* Where to signal when answer has
					 * been placed in ANSWER_BUFFER. */
} Request;

typedef struct {
    char actualFormat[SX_FORMAT_SIZE];	/* Actual format in which selection is
					 * being returned (null-terminated). */
    int numBytes;			/* Number of bytes of selection
					 * enclosed below. */
    char value[BYTES_AT_ONCE];		/* Selection contents. */
} Answer;

/*
 * Internal procedures referenced before they're defined:
 */

extern void	SelClaim();
extern void	SelMakeSignalWindow();
extern void	SelSignalProc();
extern int	SelTmpHandler();
extern Boolean	SelWaitForSignal();

/*
 *----------------------------------------------------------------------
 *
 * Sx_SelectionSet --
 *
 *	Change the selection.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The selection is changed.  All that happens is to save the
 *	name of the procedure to call when the selection is needed.
 *	FetchProc must be prepared to return part or all of the
 *	selection anytime it is called (up until the next time changeProc
 *	is called).  FetchProc has the following structure:
 *
 *	int
 *	fetchProc(clientData, desiredFormat, firstByte, numBytes,
 *	        valuePtr, formatPtr)
 *	    ClientData clientData;
 *	    char *desiredFormat;
 *	    int firstByte;
 *	    int numBytes;
 *	    char *valuePtr;
 *	    char *formatPtr;
 *	{
 *	}
 *
 *	In the calls to fetchProc, clientData will be a copy of
 *	the clientData parameter passed in here.  The other parameters,
 *	plus the return value, all have the same meanings as they do
 *	for Sx_SelectionGet.
 *
 *	ChangeProc is invoked in the following way:
 *
 *	void
 *	changeProc(clientData)
 *	    ClientData clientData;
 *	{
 *	}
 *
 *	The clientData parameter is identical to the clientData parameter
 *	passed in here.  ChangeProc will be invoked exactly once, the
 *	next time Sx_SelectionSet or Sx_SelectionClear is invoked.
 *
 *----------------------------------------------------------------------
 */

void
Sx_SelectionSet(fetchProc, changeProc, clientData)
    int (*fetchProc)();		/* Procedure to invoke whenever the
				 * contents of the selection are needed. */
    void (*changeProc)();	/* Procedure to invoke the next time the
				 * selection is changed.  This procedure
				 * will be invoked exactly once. */
    ClientData clientData;	/* Arbitrary value to pass to above
				 * procedures. */
{
    if (!weveGotIt) {
	SelClaim();
    } else {
	if (selFetchProc != NULL) {
	    (*selChangeProc)(selClientData);
	}
    }
    selFetchProc = fetchProc;
    selChangeProc = changeProc;
    selClientData = clientData;
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_SelectionClear --
 *
 *	Cancel any current selection.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	From now until the next call to Sx_SelectionSet, there will
 *	be no selection.
 *
 *----------------------------------------------------------------------
 */

void
Sx_SelectionClear()
{
    if (!weveGotIt) {
	SelClaim();
    } else {
	if (selFetchProc != NULL) {
	    (*selChangeProc)(selClientData);
	}
	selFetchProc = NULL;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Sx_SelectionGet --
 *
 *	Return part or all of the current selection.
 *
 * Results:
 *	If there is no selection at present, -1 is returned.  Otherwise,
 *	the return value indicates the number of bytes of selection
 *	information stored at *valuePtr (if it is less than numBytes,
 *	then the end of the selection was reached).  *ValuePtr gets
 *	filled in with bytes of the selection,  starting with the
 *	firstByte'th byte in the selection and containing either all
 *	the remaining bytes in the selection or numBytes bytes,
 *	whichever is less.  The selection will be returned in the
 *	closest possible form to what was requested with desiredFormat.
 *	The actual format in which the selection is returned will be
 *	placed at *formatPtr as an ASCII string containing not more
 *	than SX_FORMAT_SIZE characters (including the terminating 0).
 *
 * Side effects:
 *	None.
 *
 * Note:
 *	The formats of selections are completely up to the choice of the
 *	clients that manage them and will probably evolve over time.
 *	The one predefined format is "text":  in this case, the selection
 *	bytes consist of ASCII characters (not null-terminated).
 *
 *----------------------------------------------------------------------
 */

int
Sx_SelectionGet(desiredFormat, firstByte, numBytes, valuePtr, formatPtr)
    char *desiredFormat;	/* Form in which the selection should
				 * be returned, if possible. */
    int firstByte;		/* Index of the first byte of the selection
				 * that caller is interested in. */
    int numBytes;		/* Maximum number of bytes that may be
				 * stored at *valuePtr. */
    char *valuePtr;		/* Where to store selection information.
				 * Note: the information stored here
				 * will not necessarily be null-terminated.
				 * Depends on format of selection. */
    char *formatPtr;		/* Actual format of returned selection gets
				 * stored here, null-terminated.  Must be
				 * at least SX_FORMAT_SIZE bytes here. */
{
    Window owner;
    char *ownerBuffer;
    Request request;
    register Answer *ansPtr;
    int length, totalCount;

    if (weveGotIt) {
	if (selFetchProc == NULL) {
	    return -1;
	}
	return (*selFetchProc)(selClientData, desiredFormat, firstByte,
		numBytes, valuePtr, formatPtr);
    }

    /*
     * Some other client has the selection.  Fetch it from them using
     * the cut buffers for communication.  Get at most BYTES_AT_ONCE
     * at a time.
     */

    SelMakeSignalWindow();
    ownerBuffer = XFetchBuffer(&length, OWNER_BUFFER);
    if (length != sizeof(Window)) {
	if (ownerBuffer != NULL) {
	    Mem_Free((Address) ownerBuffer);
	    return -1;
	}
    }
    owner = *((Window *) ownerBuffer);
    Mem_Free((Address) ownerBuffer);

    String_NCopy(SX_FORMAT_SIZE, desiredFormat, request.desiredFormat);
    request.desiredFormat[SX_FORMAT_SIZE-1] = 0;
    totalCount = 0;
    while (numBytes > 0) {
	request.firstByte = firstByte;
	request.numBytes = numBytes;
	if (numBytes > BYTES_AT_ONCE) {
	    request.numBytes = BYTES_AT_ONCE;
	}
	request.signalWindow = signalWindow;
	XStoreBuffer((char *) &request, sizeof(request), REQUEST_BUFFER);
	if (!SelUnmapCarefully(owner) || !SelWaitForSignal()) {
	    return -1;
	}
	ansPtr = (Answer *) XFetchBuffer(&length, ANSWER_BUFFER);
	if (length < (sizeof(Answer) - BYTES_AT_ONCE)) {
	    if (ansPtr != NULL) {
		Mem_Free((Address) ansPtr);
		return -1;
	    }
	}
	ansPtr->numBytes;
	String_NCopy(SX_FORMAT_SIZE, ansPtr->actualFormat, formatPtr);
	formatPtr[SX_FORMAT_SIZE-1] = 0;
	if (ansPtr->numBytes > 0) {
	    Byte_Copy(ansPtr->numBytes, ansPtr->value, valuePtr);
	    valuePtr += ansPtr->numBytes;
	    firstByte += ansPtr->numBytes;
	    totalCount += ansPtr->numBytes;
	    numBytes -= ansPtr->numBytes;
	}
	Mem_Free((Address) ansPtr);

	if (ansPtr->numBytes < request.numBytes) {
	    if (ansPtr->numBytes < 0) {
		return -1;
	    } else {
		break;
	    }
	}
    }
    return totalCount;
}

/*
 *----------------------------------------------------------------------
 *
 * SelMakeSignalWindow --
 *
 *	Creates the dummy signalling window for this process, if it
 *	doesn't already exist.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static void
SelMakeSignalWindow()
{
    if (signalWindow != 0) {
	return;
    }
    signalWindow = XCreateTransparency(RootWindow, 0, 0, 1, 1);
    if (signalWindow == 0) {
	Sx_Panic("Sx selection module couldn't create window for signalling.");
    }
    (void) Sx_HandlerCreate(signalWindow, UnmapWindow, SelSignalProc,
	    (ClientData) NULL);
    XMapWindow(signalWindow);
}

/*
 *----------------------------------------------------------------------
 *
 * SelSignalProc --
 *
 *	This procedure is invoked by the Sx scheduler whenever someone
 *	unmaps our signalWindow to request the selection contents.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Stores part of the selection in a cut buffer, and signals the
 *	requester by unmapping its signal window.
 *
 *----------------------------------------------------------------------
 */

static void
SelSignalProc()
{
    register Request *reqPtr;
    Answer answer;
    int length;

    /*
     * Remap the signal window (so we can receive more requests, then
     * grab the request and see what we're to do.
     */
    
    XMapWindow(signalWindow);
    if (!weveGotIt) {
	return;
    }
    reqPtr = (Request *) XFetchBuffer(&length, REQUEST_BUFFER);
    if (length != sizeof(Request)) {
	/*
	 * Bad request;  ignore it.
	 */

	if (length > 0) {
	    Mem_Free((Address) reqPtr);
	}
	return;
    } else if (reqPtr->numBytes < 0) {
	/*
	 * Someone else is taking the selection.  If this client used
	 * to have it, then call the cleanup procedure.
	 */
	
	if (selFetchProc != NULL) {
	    (*selChangeProc)(selClientData);
	}
	selFetchProc = NULL;
	weveGotIt = FALSE;
	Mem_Free((Address) reqPtr);
	return;
    }

    /*
     * Someone's requesting part of the selection.  If we've got the
     * selection, then call the client routine that will return selection
     * information, and store the information in a cut buffer (just in
     * case the requesting process forgot to terminate the format name,
     * do it here).
     */
    
    if (selFetchProc == NULL) {
	answer.numBytes = -1;
    } else {
	reqPtr->desiredFormat[SX_FORMAT_SIZE-1] = 0;
	length = reqPtr->numBytes;
	if (length > BYTES_AT_ONCE) {
	    length = BYTES_AT_ONCE;
	}
	answer.numBytes = (*selFetchProc)(selClientData, reqPtr->desiredFormat,
		reqPtr->firstByte, length, answer.value, answer.actualFormat);
	answer.actualFormat[SX_FORMAT_SIZE-1] = 0;
    }
    length = sizeof(Answer) - BYTES_AT_ONCE;
    if (answer.numBytes > 0) {
	length += answer.numBytes;
    }
    XStoreBuffer((char *) &answer, length, ANSWER_BUFFER);
    (void) SelUnmapCarefully(reqPtr->signalWindow);
    Mem_Free((Address) reqPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * SelClaim --
 *
 *	If we aren't the current selection owner, claim the selection
 *	for us.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The current selection owner is notified that we want the
 *	selection.  "weveGotIt" gets set to TRUE.
 *
 *----------------------------------------------------------------------
 */

static void
SelClaim()
{
    char *ownerBuffer;
    int length;

    if (weveGotIt) {
	return;
    }
    SelMakeSignalWindow();

    /*
     * First, notify the previous owner of the selection, if any.
     */

    ownerBuffer = XFetchBuffer(&length, OWNER_BUFFER);
    if (length == sizeof(Window)) {
	Window owner;
	Request request;

	owner = *((Window *) ownerBuffer);
	request.firstByte = -1;
	request.numBytes = -1;
	XStoreBuffer((char *) &request, sizeof(Request), REQUEST_BUFFER);
	(void) SelUnmapCarefully(owner);
    }
    if (ownerBuffer != NULL) {
	Mem_Free((Address) ownerBuffer);
    }

    /*
     * Store us as the current owner of the selection.
     */
    
    XStoreBuffer((char *) &signalWindow, sizeof(Window), OWNER_BUFFER);
    weveGotIt = TRUE;
}

/*
 *----------------------------------------------------------------------
 *
 * SelWaitForSignal --
 *
 *	This procedure is called to wait until either a) this client
 *	gets signalled (by someone unmapping its signalWindow) or
 *	b) a timeout occurs.
 *
 * Results:
 *	If a timeout occurred then FALSE if returned.  If all's OK
 *	then TRUE is returned.
 *
 * Side effects:
 *	The signalWindow gets temporarily unmapped, but this procedure
 *	immediately remaps it.
 *
 * Note:
 *	This procedure is a hacked-up copy of the MIT XWindowEvent
 *	procedure (Copyright 1985 by the Massachusetts Institute of
 *	Technology).
 *
 *----------------------------------------------------------------------
 */

static Boolean
SelWaitForSignal()
{
    register Display *dpy;
    register _QEvent *prev, *qelt;
    XEvent event;
    extern _QEvent *_qfree;

    /* 
     * Flush output, then see if there's already an event in the
     * queue.
     */

    dpy = _XlibCurrentDisplay;
    _XFlush (dpy);
    for (prev = NULL, qelt = dpy->head;
	    qelt && (qelt->event.window != signalWindow);
	    prev = qelt, qelt = qelt->next)  {
	/* Null body. */
    }
    if (qelt) {
	if (prev) {
	    prev->next = qelt->next;
	    if (prev->next == NULL) {
		dpy->tail = prev;
	    }
	} else {
	    dpy->head = qelt->next;
	    if (dpy->head == NULL) {
		dpy->tail = NULL;
	    }
	}
	qelt->next = _qfree;
	_qfree = qelt;
	dpy->qlen--;
	goto gotSignal;
    }

    /*
     * Nothing there right now.  Wait for an event to come in, or
     * for several seconds to elapse.
     */

    while (1) {
	struct timeval timeout;
	int mask, numFound;

	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
	mask = 1 << dpy->fd;
	numFound = select(dpy->fd + 1, &mask, (int *) 0, (int *) 0, &timeout);
	if (numFound <= 0) {
	    return FALSE;
	}
	_XRead (dpy, &event, sizeof(XEvent));
	if (event.type == X_Error) {
	    _XError(dpy, (XErrorEvent *) &event);
	} else if (event.window == signalWindow) {
	    goto gotSignal;
	} else {
	    _XEnq(dpy, &event);
	}
    }

    /*
     * A signal came in:  remap the signalWindow, then return.
     */
    
    gotSignal:
    XMapWindow(signalWindow);
    return TRUE;
}

/*
 *----------------------------------------------------------------------
 *
 * SelUnmapCarefully --
 *
 *	Unmap a window, ignoring any errors that might occur because
 *	the window doesn't exist.
 *
 * Results:
 *	TRUE is returned if the window exists, FALSE if an error
 *	occurred while trying to unmap it.
 *
 * Side effects:
 *	Window gets unmapped, if it exists.
 *
 *----------------------------------------------------------------------
 */

static Window unmapWindow;	/* For communication between SelUnmapCarefully
				 * and SelTmpHandler. */
static Boolean unmappedOK;
static int (*savedHandler)();

static Boolean
SelUnmapCarefully(window)
    Window window;			/* Window to unmap. */
{

    savedHandler = _XErrorFunction;
    _XErrorFunction = SelTmpHandler;
    unmapWindow = window;
    unmappedOK = TRUE;
    XUnmapWindow(window);
    XSync(FALSE);
    _XErrorFunction = savedHandler;
    return unmappedOK;
}

    /* ARGSUSED */

static int
SelTmpHandler(dpyPtr, eventPtr)
    Display *dpyPtr;		/* Display where error happened. */
    XErrorEvent *eventPtr;	/* Describes error. */
{
    if ((eventPtr->request_code == X_UnmapWindow)
	    && (eventPtr->window == unmapWindow)) {
	unmappedOK = FALSE;
    } else {
	(*savedHandler)(dpyPtr, eventPtr);
    }
}
