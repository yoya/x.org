/*
 * Copyright 1987 by Tektronix Inc, Beaverton, Oregon.
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Tektronix not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.
 *
 *
 * TEKTRONIX DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * TEKTRONIX BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */
/* 
 * $Header: sequence.c,v 4.1 88/02/14 17:05:59 toddb Exp $ 
 * $Locker:  $ 
 */

/*
 * These routines maintain the x11 request sequence numbering of x10
 * requests.  A circular list (named 'list') is kept for each
 * "outstanding" x10 request.  An outstanding request is a request that we
 * have not received an x11 reply for (several X11 requests have no
 * reply).  An x10 request is no longer outstanding if an x11 reply has
 * been received whose sequence number is greater than or equal to the x10
 * request sequence number.
 */
#include "X11/Xlib.h"
#include "X11/Xproto.h"
#include "resource.h"
#include <stdio.h>

extern u_char		Xstatus;

/*
 * Put a request on the queue expanding if necessary.
 */
MarkStartOfRequest(client, req)
	int	client;
	X10Req	*req;
{
	Clientptr	cp = clist[ client ];
	Sequenceptr	headp;

	/*
	 * Allocate space for a new circular list.
	 */
	TruncateSequences(client);
	if (cp->seq.last == 0) {
		cp->seq.circList = (Sequenceptr)
			Xalloc(sizeof(Sequence) * SEQ_INCREMENT);
		cp->seq.last = SEQ_INCREMENT - 1;
		cp->seq.head = cp->seq.tail = 0;
		cp->seq.lastx11seq = NextRequest(cp->dpy);
	}
	/*
	 * Stretch the old one
	 */
	else if (cp->seq.head == cp->seq.tail) {
		int	oldlast, tailToEnd, startToHead;
		Sequenceptr	newList;

		oldlast = cp->seq.last;
		cp->seq.last += SEQ_INCREMENT;
		newList = (Sequenceptr)
			Xalloc(sizeof(Sequence) * (cp->seq.last+1));

		tailToEnd = oldlast - cp->seq.tail + 1;
		startToHead = cp->seq.head;

		startToHead *= sizeof(Sequence);
		bcopy(cp->seq.circList, newList + tailToEnd,
			startToHead);
		tailToEnd *= sizeof(Sequence);
		bcopy(cp->seq.circList + cp->seq.tail, newList,
			tailToEnd);

		free(cp->seq.circList);
		cp->seq.circList = newList;
		cp->seq.head = oldlast + 1;
		cp->seq.tail = 0;
	}

	/*
	 * Record the x11 sequence number and x10 request.
	 */
	headp = cp->seq.circList + cp->seq.head;
	bcopy(req, &headp->req, sizeof(X10Req));
	headp->x11seq = NextRequest(cp->dpy);
	cp->seq.head = NextSeqPtr(cp, cp->seq.head);
	if (debugged(req->code))
		debug(XSequenceDebug, ("Seq #%d: ", headp->x11seq));
}

u_char BadMap[] = {
	0,		/* 0 */
	X10BadRequest,	/* BadRequest */
	X10BadValue,	/* BadValue */
	X10BadWindow,	/* BadWindow */
	X10BadPixmap,	/* BadPixmap */
	X10BadValue,	/* BadAtom */
	X10BadCursor,	/* BadCursor */
	X10BadFont,	/* BadFont */
	X10BadMatch,	/* BadMatch */
	X10BadTile,	/* BadDrawable */
	X10BadAccess,	/* BadAccess */
	X10BadAlloc,	/* BadAlloc */
	X10BadColor,	/* BadColor */
	X10BadWindow,	/* BadGC */
	X10BadValue,	/* BadIDChoice */
	X10BadValue,	/* BadName */
	X10BadValue,	/* BadLength */
	X10BadValue,	/* BadImplementation */
};

/*
 * Called Before reading any X11 replies/events.  If there has been a skip in
 * the sequence of events, then we must assume that those missing
 * received replies: Xlib does not call the "after" function for
 * requests that receive replies.
 * Then we can move the tail marker up to the head.
 */
TruncateSequences(client)
	int		client;
{
	Clientptr	cp = clist[ client ];
	Display		*dpy = cp->dpy;
	xReq		*req;
	int		prev, tail, head;
	Sequenceptr	ptr;

	tail = cp->seq.tail;
	head = cp->seq.head;
	if (tail == head || cp->seq.last == 0)
		return;
	prev = tail;
	tail = NextSeqPtr(cp, tail);
	while (tail != head) {
		ptr = cp->seq.circList + tail;
		/*
		 * Find the first request that has not been read...
		 * Throw away everything before that.
		 */
		if (LastKnownRequestProcessed(dpy) < ptr->x11seq) {
			/* dont't bother if nothing is going to change. */ 
			if (prev == cp->seq.tail)
				break;
			cp->seq.tail = prev;
			debug(XSequenceDebug, (" chop @#%d\n",
				(cp->seq.circList + prev)->x11seq));
			break;
		}
		prev = tail;
		tail = NextSeqPtr(cp, tail);
	}
}

X11ErrorHandler(dpy, event)
        Display *dpy;
        XErrorEvent *event;
{
	int		client = (int) clist[ ConnectionNumber(dpy) ],
			tail, last;
	Clientptr	cp = clist[ client ];
	extern char	*X10ProtoNames[];
	Sequenceptr	ptr;

	XDefaultError(dpy, event);
	debug(XSequenceDebug, ("Error on Seq %d: ", event->serial));

	tail = cp->seq.tail;
	last = tail;
	while (tail != cp->seq.head) {
		ptr = cp->seq.circList + tail;
		debug(XSequenceDebug, ("\nSeq #%d: %s ",
			ptr->x11seq, X10ProtoNames[ ptr->req.code ]));
		if (event->serial <= ptr->x11seq)
			break;
		last = tail;
		tail = NextSeqPtr(cp, tail);
	}
	ptr = cp->seq.circList + last;
	debug(XSequenceDebug, ("\n"));
	ShowReqName(client, &ptr->req);
	ShowReq(client, &ptr->req, NULL);
	/*
	 * Clear outstanding requests before this.
	 */
	cp->seq.tail = NextSeqPtr(cp, last);
	Xstatus = BadMap[ event->error_code ];
	Oops(client, &ptr->req);
	return(0);
}

/*
 * A copy of the standard Xlib default Error... without the fatal error.
 */
XDefaultError(dpy, event)
        Display *dpy;
        XErrorEvent *event;
{
	char buffer[BUFSIZ];
	char mesg[BUFSIZ];
	char number[32];
	char *mtype = "XlibMessage";
	XGetErrorText(dpy, event->error_code, buffer, BUFSIZ);
	XGetErrorDatabaseText(dpy, mtype, "XError", "X Error", mesg, BUFSIZ);
	(void) fprintf(stderr, "%s: %s\n  ", mesg, buffer);
	XGetErrorDatabaseText(dpy, mtype, "MajorCode", "Request Major code %d", 
	    mesg, BUFSIZ);
	(void) fprintf(stderr, mesg, event->request_code);
	sprintf(number, "%d", event->request_code);
	XGetErrorDatabaseText(dpy, "XRequest", number, "", 	buffer, BUFSIZ);
	(void) fprintf(stderr, " %s", buffer);
	fputs("\n  ", stderr);
	XGetErrorDatabaseText(dpy, mtype, "MinorCode", "Request Minor code", 
	    mesg, BUFSIZ);
	(void) fprintf(stderr, mesg, event->minor_code);
	fputs("\n  ", stderr);
	XGetErrorDatabaseText(dpy, mtype, "ResourceID", "ResourceID 0x%x",
	    mesg, BUFSIZ);
	(void) fprintf(stderr, mesg, event->resourceid);
	fputs("\n  ", stderr);
	XGetErrorDatabaseText(dpy, mtype, "ErrorSerial", "Error Serial #%d", 
	    mesg, BUFSIZ);
	(void) fprintf(stderr, mesg, event->serial);
	fputs("\n  ", stderr);
	XGetErrorDatabaseText(dpy, mtype, "CurrentSerial", "Current Serial #%d",
	    mesg, BUFSIZ);
	(void) fprintf(stderr, mesg, NextRequest(dpy));
	fputs("\n  ", stderr);
	return (0);
}

X11IOErrorHandler(dpy)
        Display *dpy;
{
	int		client = (int) clist[ ConnectionNumber(dpy) ];
	static int	active;

	if (active)
		return;

	active++;
	Notice("IO error: shutting down client #%d\n", client);
	Free_client_resources (client);
	close(client);
	active = 0;

	RestartDispatcher();
}
