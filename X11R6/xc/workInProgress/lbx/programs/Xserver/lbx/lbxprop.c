/* $XConsortium: lbxprop.c,v 1.7 94/12/01 20:29:09 mor Exp $ */
/*
 * Copyright 1993 Network Computing Devices, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name Network Computing Devices, Inc. not be
 * used in advertising or publicity pertaining to distribution of this
 * software without specific, written prior permission.
 *
 * THIS SOFTWARE IS PROVIDED `AS-IS'.  NETWORK COMPUTING DEVICES, INC.,
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING WITHOUT
 * LIMITATION ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NONINFRINGEMENT.  IN NO EVENT SHALL NETWORK
 * COMPUTING DEVICES, INC., BE LIABLE FOR ANY DAMAGES WHATSOEVER, INCLUDING
 * SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, INCLUDING LOSS OF USE, DATA,
 * OR PROFITS, EVEN IF ADVISED OF THE POSSIBILITY THEREOF, AND REGARDLESS OF
 * WHETHER IN AN ACTION IN CONTRACT, TORT OR NEGLIGENCE, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $NCDId: @(#)lbxprop.c,v 1.12 1994/09/23 20:51:51 lemke Exp $
 *
 * Author:  Dave Lemke, Network Computing Devices
 */

/* various bits of DIX-level mangling */

#include <sys/types.h>
#include <stdio.h>
#define NEED_REPLIES
#define NEED_EVENTS
#include "X.h"
#include "Xproto.h"
#include "misc.h"
#include "os.h"
#include "dixstruct.h"
#include "resource.h"
#include "servermd.h"
#include "propertyst.h"
#include "windowstr.h"
#define _XLBX_SERVER_
#include "lbxstr.h"
#include "lbxserve.h"
#include "lbxtags.h"
#include "Xfuncproto.h"

extern int  (*ProcVector[256]) ();
extern void (*ReplySwapVector[256]) ();
extern void CopySwap16Write(), CopySwap32Write(), Swap32Write();
extern int  fWriteToClient();

#ifdef NCD			/* XXX this should be elsewhere */
#define	memmove(dst,src,len)	bcopy(src,dst,len)
#endif

void
LbxStallPropRequest(client, pProp)
    ClientPtr   client;
    PropertyPtr pProp;
{
    LbxQueryTagData(client, pProp->owner_pid, pProp->tag_id, LbxTagTypeProperty, pProp);
    LbxResetCurrentRequest(client);
    client->sequence--;

/* XXX this won't work too well went done to a proxy client.
 * need finer grain of control
 */
    LbxIgnoreClient(client);
}

int
LbxChangeWindowProperty(client, pWin, property, type, format, mode, len,
			have_data, value, sendevent, tag)
    ClientPtr   client;
    WindowPtr   pWin;
    Atom        property,
                type;
    int         format,
                mode;
    unsigned long len;
    Bool        have_data;
    pointer     value;
    Bool        sendevent;
    XID        *tag;
{
    PropertyPtr pProp;
    xEvent      event;
    int         sizeInBytes;
    int         totalSize;
    pointer     data;
    XID         tid = 0;
    int         owner = 0;

    sizeInBytes = format >> 3;
    totalSize = len * sizeInBytes;

    /* first see if property already exists */

    pProp = wUserProps(pWin);
    while (pProp) {
	if (pProp->propertyName == property)
	    break;
	pProp = pProp->next;
    }
    if (!pProp) {		/* just add to list */
	if (!pWin->optional && !MakeWindowOptional(pWin))
	    return (BadAlloc);
	pProp = (PropertyPtr) xalloc(sizeof(PropertyRec));
	if (!pProp)
	    return (BadAlloc);
	data = (pointer) xalloc(totalSize);
	if (!data && len) {
	    xfree(pProp);
	    return (BadAlloc);
	}
	pProp->propertyName = property;
	pProp->type = type;
	pProp->format = format;
	pProp->data = data;
	if (have_data) {
	    if (len)
		memmove((char *) data, (char *) value, totalSize);
	} else {
	    owner = LbxProxyID(client);
	    tid = TagNewTag();
	    if (!tid) {
		xfree(pProp);
		xfree(pProp->data);
		return BadAlloc;
	    }
	    if (!TagSaveTag(tid, LbxTagTypeProperty, format, totalSize,
			    pProp->data)) {
		xfree(pProp);
		xfree(pProp->data);
		return BadAlloc;
	    }
	    TagMarkProxy(tid, owner);
	}
	pProp->tag_id = tid;
	pProp->owner_pid = owner;
	pProp->size = len;
	pProp->next = pWin->optional->userProps;
	pWin->optional->userProps = pProp;
    } else {
	/*
	 * To append or prepend to a property the request format and type must
	 * match those of the already defined property.  The existing format
	 * and type are irrelevant when using the mode "PropModeReplace" since
	 * they will be written over.
	 */

	if ((format != pProp->format) && (mode != PropModeReplace))
	    return (BadMatch);
	if ((pProp->type != type) && (mode != PropModeReplace))
	    return (BadMatch);

	/*
	 * if its a modify instead of replace, make sure we have the current
	 * value
	 */
	if ((mode != PropModeReplace) && (pProp->owner_pid != 0)) {
	    LbxStallPropRequest(client, pProp);
	    return (client->noClientException);
	}
	/* make sure any old tag is flushed first */
	if (pProp->tag_id) {
	    LbxFlushPropertyTag(pProp->tag_id);
	    pProp->tag_id = 0;
	}
	if (mode == PropModeReplace) {
	    if (totalSize != pProp->size * (pProp->format >> 3)) {
		data = (pointer) xrealloc(pProp->data, totalSize);
		if (!data && len)
		    return (BadAlloc);
		pProp->data = data;
	    }
	    if (have_data) {
		if (len)
		    memmove((char *) pProp->data, (char *) value, totalSize);
	    } else {
		owner = LbxProxyID(client);
		tid = TagNewTag();
		if (!tid) {
		    xfree(pProp);
		    xfree(pProp->data);
		    return BadAlloc;
		}
		if (!TagSaveTag(tid, LbxTagTypeProperty, format, totalSize,
				pProp->data)) {
		    xfree(pProp);
		    xfree(pProp->data);
		    return BadAlloc;
		}
		TagMarkProxy(tid, owner);
	    }
	    pProp->tag_id = tid;
	    pProp->owner_pid = owner;
	    pProp->size = len;
	    pProp->type = type;
	    pProp->format = format;
	} else if (len == 0) {
	    /* do nothing */
	} else if (mode == PropModeAppend) {
	    data = (pointer) xrealloc(pProp->data,
				      sizeInBytes * (len + pProp->size));
	    if (!data)
		return (BadAlloc);
	    pProp->data = data;
	    memmove(&((char *) data)[pProp->size * sizeInBytes],
		    (char *) value,
		    totalSize);
	    pProp->size += len;
	} else if (mode == PropModePrepend) {
	    data = (pointer) xalloc(sizeInBytes * (len + pProp->size));
	    if (!data)
		return (BadAlloc);
	    memmove(&((char *) data)[totalSize], (char *) pProp->data,
		    (int) (pProp->size * sizeInBytes));
	    memmove((char *) data, (char *) value, totalSize);
	    xfree(pProp->data);
	    pProp->data = data;
	    pProp->size += len;
	}
    }
    if (sendevent) {
	event.u.u.type = PropertyNotify;
	event.u.property.window = pWin->drawable.id;
	event.u.property.state = PropertyNewValue;
	event.u.property.atom = pProp->propertyName;
	event.u.property.time = currentTime.milliseconds;
	DeliverEvents(pWin, &event, 1, (WindowPtr) NULL);
    }
    if (tag)
	*tag = tid;
    return (Success);
}

int
LbxChangeProperty(client)
    ClientPtr   client;
{
    WindowPtr   pWin;
    char        format,
                mode;
    unsigned long len;
    int         err;
    XID         newtag;
    xLbxChangePropertyReply rep;

    REQUEST(xLbxChangePropertyReq);

    REQUEST_AT_LEAST_SIZE(xLbxChangePropertyReq);
    UpdateCurrentTime();
    format = stuff->format;
    mode = stuff->mode;
    if ((mode != PropModeReplace) && (mode != PropModeAppend) &&
	    (mode != PropModePrepend)) {
	client->errorValue = mode;
	return BadValue;
    }
    if ((format != 8) && (format != 16) && (format != 32)) {
	client->errorValue = format;
	return BadValue;
    }
    len = stuff->nUnits;
    if (len > ((0xffffffff - sizeof(xChangePropertyReq)) >> 2))
	return BadLength;

    pWin = (WindowPtr) LookupWindow(stuff->window, client);
    if (!pWin)
	return (BadWindow);
    if (!ValidAtom(stuff->property)) {
	client->errorValue = stuff->property;
	return (BadAtom);
    }
    if (!ValidAtom(stuff->type)) {
	client->errorValue = stuff->type;
	return (BadAtom);
    }
    err = LbxChangeWindowProperty(client, pWin, stuff->property, stuff->type,
		   (int) format, (int) mode, len, FALSE, (pointer) &stuff[1],
				  TRUE, &newtag);
    if (err)
	return err;

    rep.type = X_Reply;
    rep.sequenceNumber = client->sequence;
    rep.length = 0;
    rep.tag = newtag;
    if (client->swapped) {
	int         n;

	swaps(&rep.sequenceNumber, n);
	swapl(&rep.length, n);
	swapl(&rep.tag, n);
    }
    WriteToClient(client, sizeof(xLbxChangePropertyReply), &rep);

    return client->noClientException;
}

static void
LbxWriteGetpropReply(client, rep)
    ClientPtr   client;
    xLbxGetPropertyReply *rep;
{
    int         n;

    if (client->swapped) {
	swaps(&rep->sequenceNumber, n);
	swapl(&rep->length, n);
	swapl(&rep->propertyType, n);
	swapl(&rep->bytesAfter, n);
	swapl(&rep->nItems, n);
	swapl(&rep->tag, n);
    }
    WriteToClient(client, sizeof(xLbxGetPropertyReply), rep);
}

int
LbxGetProperty(client)
    ClientPtr   client;
{
    PropertyPtr pProp,
                prevProp;
    unsigned long n,
                len,
                ind;
    WindowPtr   pWin;
    xLbxGetPropertyReply reply;
    XID         tid;
    Bool        send_data = FALSE;

    REQUEST(xLbxGetPropertyReq);

    REQUEST_SIZE_MATCH(xLbxGetPropertyReq);
    if (stuff->delete)
	UpdateCurrentTime();
    pWin = (WindowPtr) LookupWindow(stuff->window, client);
    if (pWin) {
	if (!ValidAtom(stuff->property)) {
	    client->errorValue = stuff->property;
	    return (BadAtom);
	}
	if ((stuff->delete != xTrue) && (stuff->delete != xFalse)) {
	    client->errorValue = stuff->delete;
	    return (BadValue);
	}
	if ((stuff->type == AnyPropertyType) || ValidAtom(stuff->type)) {
	    pProp = wUserProps(pWin);
	    prevProp = (PropertyPtr) NULL;
	    while (pProp) {
		if (pProp->propertyName == stuff->property)
		    break;
		prevProp = pProp;
		pProp = pProp->next;
	    }
	    reply.type = X_Reply;
	    reply.sequenceNumber = client->sequence;
	    if (pProp) {

		tid = pProp->tag_id;
		/*
		 * If the request type and actual type don't match. Return the
		 * property information, but not the data.
		 */

		if ((stuff->type != pProp->type) &&
			(stuff->type != AnyPropertyType)) {
		    reply.bytesAfter = pProp->size;
		    reply.format = pProp->format;
		    reply.length = 0;
		    reply.nItems = 0;
		    reply.propertyType = pProp->type;
		    reply.tag = 0;
		    LbxWriteGetpropReply(client, &reply);
		    return (Success);
		}
		/* make sure we have the current value */
		if ((pProp->owner_pid != 0) &&
			(pProp->owner_pid != LbxProxyID(client))) {
		    LbxStallPropRequest(client, pProp);
		    return (client->noClientException);
		}
		/*
		 * Return type, format, value to client
		 */
		n = (pProp->format / 8) * pProp->size;	/* size (bytes) of prop */
		ind = stuff->longOffset << 2;

		/*
		 * If longOffset is invalid such that it causes "len" to be
		 * negative, it's a value error.
		 */

		if (n < ind) {
		    client->errorValue = stuff->longOffset;
		    return BadValue;
		}
		len = min(n - ind, 4 * stuff->longLength);

		if (!tid) {
		    tid = TagNewTag();
		    if (!TagSaveTag(tid, LbxTagTypeProperty, pProp->format,
                    		len, ((char *) pProp->data + ind))) {
			tid = 0;
		    }
		    send_data = TRUE;
		} else
		    send_data = !TagProxyMarked(tid, LbxProxyID(client));
		pProp->tag_id = tid;
		TagMarkProxy(tid, LbxProxyID(client));

		reply.bytesAfter = n - (ind + len);
		reply.format = pProp->format;
		if (send_data)
		    reply.length = (len + 3) >> 2;
		else
		    reply.length = 0;
		reply.nItems = len / (pProp->format / 8);
		reply.propertyType = pProp->type;
		reply.tag = pProp->tag_id;

		if (stuff->delete && (reply.bytesAfter == 0)) {
		    LbxFlushPropertyTag(tid);
		}
		if (stuff->delete && (reply.bytesAfter == 0)) {	/* send the event */
		    xEvent      event;

		    event.u.u.type = PropertyNotify;
		    event.u.property.window = pWin->drawable.id;
		    event.u.property.state = PropertyDelete;
		    event.u.property.atom = pProp->propertyName;
		    event.u.property.time = currentTime.milliseconds;
		    DeliverEvents(pWin, &event, 1, (WindowPtr) NULL);
		}
		LbxWriteGetpropReply(client, &reply);
		if (send_data && len) {
		    switch (reply.format) {
		    case 32:
			client->pSwapReplyFunc = CopySwap32Write;
			break;
		    case 16:
			client->pSwapReplyFunc = CopySwap16Write;
			break;
		    default:
			client->pSwapReplyFunc = (void (*) ()) fWriteToClient;
			break;
		    }
		    WriteSwappedDataToClient(client, len,
					     (char *) pProp->data + ind);
		}
		if (stuff->delete && (reply.bytesAfter == 0)) {	/* delete the Property */
		    if (prevProp == (PropertyPtr) NULL) {	/* takes care of head */
			if (!(pWin->optional->userProps = pProp->next))
			    CheckWindowOptionalNeed(pWin);
		    } else
			prevProp->next = pProp->next;
		    TagDeleteTag(tid);
		    xfree(pProp->data);
		    xfree(pProp);
		}
	    } else {
		reply.nItems = 0;
		reply.length = 0;
		reply.bytesAfter = 0;
		reply.propertyType = None;
		reply.format = 0;
		reply.tag = 0;
		LbxWriteGetpropReply(client, &reply);
	    }
	    return (client->noClientException);

	} else {
	    client->errorValue = stuff->type;
	    return (BadAtom);
	}
    } else
	return (BadWindow);
}

int
LbxFlushPropertyTag(tid)
    XID         tid;
{
    LbxSendInvalidateTagToProxies(tid, LbxTagTypeProperty);
    TagDeleteTag(tid);
}
