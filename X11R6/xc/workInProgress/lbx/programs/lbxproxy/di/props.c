/* $XConsortium: props.c,v 1.6 94/12/01 20:42:57 mor Exp $ */
/*
 * Copyright 1994 Network Computing Devices, Inc.
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
 * $NCDId: @(#)props.c,v 1.13 1994/09/07 00:43:39 lemke Exp $
 */
/*
 * property handling
 */


#include	<stdio.h>
#define NEED_REPLIES
#define NEED_EVENTS
#include	<X11/Xproto.h>
#include	"assert.h"
#include	"lbxdata.h"
#include	"util.h"
#include	"tags.h"
#include	"lbx.h"		/* gets dixstruct.h */
#include	"resource.h"
#include	"wire.h"
#include	"swap.h"
#define _XLBX_SERVER_
#include	"lbxstr.h"	/* gets dixstruct.h */

/*
 * XXX
 *
 * GetProperty notes
 *
 * GetProperty is handled by using X_LbxGetProperty to get the
 * entire property.  if the client asked for a subset, we only
 * ship that subset
 *
 * XXX is this a good idea?  will we transfer data that never gets
 * used?  or is it so useful that offset & length fields aren't
 * required in X_LbxGetProperty?
 */

/*
 * wrapper for tags
 *
 * since this stuff has pointers, have to package it for
 * tag cache
 */
static Bool
propTagStoreData(tid, dlen, swapit, ptdp)
    XID         tid;
    unsigned long dlen;
    Bool	swapit;
    PropertyTagDataPtr ptdp;
{
    PropertyTagDataPtr new,
                tmp;
    Bool        ret;

/* XXX this whole thing is a mess.  too many copies, too much mangling */
    new = (PropertyTagDataPtr) xalloc(dlen + sizeof(PropertyTagDataRec));
    if (!new)
	return FALSE;
    *new = *ptdp;
    new->data = (pointer) (new + 1);
    bcopy((char *) ptdp->data, (char *) new->data, dlen);
    /* save data in proxy format */
    if (swapit) {
	switch (new->format) {
	case 32:
	    SwapLongs((CARD32 *) new->data, dlen / 4);
	    break;
	case 16:
	    SwapShorts((short *) new->data, dlen / 2);
	    break;
	default:
	    break;
	}
    }
    ret = TagStoreData(prop_cache, tid,
		       (dlen + sizeof(PropertyTagDataRec)),
		       LbxTagTypeProperty, new);
    /*
     * XXX barf.  since the saved tag has a pointer to its own stuff, have to
     * tweak it after saving
     */
    if (ret) {
	tmp = (PropertyTagDataPtr) TagGetData(prop_cache, tid);
	tmp->data = (pointer) (tmp + 1);
    }
    xfree(new);
    return TRUE;
}

/*
 * lots of brains need to live here.  this decides whether or not
 * to use an LbxChangeProperty
 *
 * heuristics:
 *
 *	- don't rewrite if its 'small' -- roundtrip swamps bandwidth
 *	- don't rewrite if its a WM property and the WM isn't sharing
 *	  the proxy
 *	- don't rewrite if the prop is on a window owned by a non-proxy
 *	  client
 *
 *	- do rewrite if its a prop on one of the client's windows
 *	- do rewrite if its a Selection property (hope is that
 *	  receiver is also using proxy)
 */
static Bool
rewrite_change_prop(req)
    xChangePropertyReq *req;
{
    extern Bool lbxUseTags;

    /* if tags are turned off, don't try */
    if (!lbxUseTags)
	return FALSE;
    /* XXX don't even try append for now */
    if (req->mode != PropModeReplace)
	return FALSE;
    return TRUE;		/* XXX */
}

static int
change_property_req(client, data)
    ClientPtr   client;
    char       *data;
{
    ReplyStuffPtr nr;
    xChangePropertyReq *req;
    int         size;
    pointer     datacopy;
    char	n;
    Atom	type, property;
    Window	win;
    CARD32	nUnits;

    req = (xChangePropertyReq *) data;

    win = req->window;
    property = req->property;
    type = req->type;
    nUnits = req->nUnits;

    if (client->swapped) {
    	swapl(&win, n);
    	swapl(&property, n);
    	swapl(&type, n);
    	swapl(&nUnits, n);
    }

    size = nUnits * req->format / 8;
    datacopy = (pointer) xalloc(size);
    if (!datacopy)
	return REQ_NOCHANGE;
    bcopy((char *) &req[1], (char *) datacopy, size);

    nr = NewReply(client);
    if (!nr) {
	xfree(datacopy);
	return REQ_NOCHANGE;
    }
    nr->sequenceNumber = LBXSequenceNumber(client);
    nr->request = X_LbxChangeProperty;
    nr->lbx_req = TRUE;
    nr->extension = client->server->lbxReq;
    nr->request_info.lbxchangeprop.ptd.length = size;
    nr->request_info.lbxchangeprop.ptd.type = type;
    nr->request_info.lbxchangeprop.ptd.format = req->format;
    nr->request_info.lbxchangeprop.ptd.data = datacopy;

    /*
     * this expects a reply.  since we write the data here, we have to be sure
     * the seq number is in sync first
     */
    ForceSequenceUpdate(client);
    SendChangeProperty(client, win, property, type,
		       req->format, req->mode, nUnits);

    return REQ_REPLACE;
}

int
ProcLBXChangeProperty(client)
    ClientPtr   client;
{
    int         yank;

    REQUEST(xChangePropertyReq);

    /* we may want to leave it as X_ChangeProperty */
    if (!rewrite_change_prop(stuff)) {
	return ProcStandardRequest(client);
    }
    yank = change_property_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

Bool
GetLbxChangePropertyReply(client, data)
    ClientPtr   client;
    char       *data;
{
    xLbxChangePropertyReply *rep;
    ReplyStuffPtr nr;
    PropertyTagDataPtr ptdp;
    char	n;

    rep = (xLbxChangePropertyReply *) data;
    if (client->swapped) {
    	swapl(&rep->tag, n);
    }

    nr = GetMatchingReply(client, rep->sequenceNumber);
    assert(nr);
    ptdp = &nr->request_info.lbxchangeprop.ptd;
    if (rep->tag) {
	if (!propTagStoreData(rep->tag, ptdp->length, client->swapped, ptdp)) {
	    SendInvalidateTag(client, rep->tag);
/* XXX is this good enough?  or should we try to send the data on to
 * the server?
 */
	    SendErrorToClient(client, X_ChangeProperty, 0, ptdp->length, BadAlloc);
	}
    }
    xfree(ptdp->data);		/* propStore gets its own copy */
    return TRUE;
}

static int
get_property_req(client, data)
    ClientPtr   client;
    char       *data;
{
    ReplyStuffPtr nr;
    xGetPropertyReq *req;

    req = (xGetPropertyReq *) data;
    nr = NewReply(client);
    if (nr) {
	if (client->swapped) {
	    char        n;

	    swapl(&req->window, n);
	    swapl(&req->property, n);
	    swapl(&req->type, n);
	    swapl(&req->longOffset, n);
	    swapl(&req->longLength, n);
	}
	nr->sequenceNumber = LBXSequenceNumber(client);
	nr->request = X_LbxGetProperty;
	nr->lbx_req = TRUE;
	nr->extension = client->server->lbxReq;
	nr->request_info.lbxgetprop.offset = req->longOffset;
	nr->request_info.lbxgetprop.length = req->longLength;

	/*
	 * this expects a reply.  since we write the data here, we have to be
	 * sure the seq number is in sync first
	 */
	ForceSequenceUpdate(client);
	/*
	 * note we ask for the whole damn thing -- any truncating is done when
	 * we pass it on
	 */
	SendGetProperty(client, req->window, req->property, req->type,
			req->delete, 0, 1000000);

	return REQ_REPLACE;
    } else
	return REQ_NOCHANGE;
}

void
FinishGetPropertyReply(client, seqnum, offset, length, ptdp, pdata)
    ClientPtr   client;
    int         seqnum;
    CARD32      offset;
    CARD32      length;
    PropertyTagDataPtr ptdp;
    pointer     pdata;
{
    xGetPropertyReply reply;
    int         bytesafter,
                t,
                dlen;

    t = ptdp->length - (4 * offset);
    dlen = min(t, (4 * length));
    bytesafter = ptdp->length - (dlen + 4 * offset);

    if (!pdata)
	pdata = ptdp->data;

    pdata = (pointer) ((char *) pdata + (4 * offset));

    reply.type = X_Reply;
    reply.sequenceNumber = seqnum;
    reply.format = ptdp->format;
    reply.length = (dlen + 3) >> 2;;
    reply.propertyType = ptdp->type;
    reply.bytesAfter = bytesafter;
    if (dlen && ptdp->format)	/* avoid div-by-zero if no data */
	reply.nItems = dlen / (ptdp->format / 8);
    else
	reply.nItems = 0;

    if (client->swapped) {
	switch (reply.format) {
	case 32:
	    SwapLongs((CARD32 *) pdata, dlen / 4);
	    break;
	case 16:
	    SwapShorts((short *) pdata, dlen / 2);
	    break;
	default:
	    break;
	}
	SwapGetPropertyReply(&reply);
    }
    WriteToClient(client, sizeof(xGetPropertyReply), (char *) &reply);
    if (dlen)
	WriteToClient(client, dlen, pdata);
}

Bool
GetLbxGetPropertyReply(client, data)
    ClientPtr   client;
    char       *data;
{
    xLbxGetPropertyReply *rep;
    int         len;
    ReplyStuffPtr nr;
    PropertyTagDataRec ptd;
    PropertyTagDataPtr ptdp;
    QueryTagRec qt;
    pointer     pdata = NULL;
    char        n;
    CARD32      tag,
                nItems;

    rep = (xLbxGetPropertyReply *) data;

    nr = GetMatchingReply(client, rep->sequenceNumber);
    assert(nr);

    ptd.type = rep->propertyType;
    ptd.format = rep->format;
    ptd.data = NULL;
    len = rep->length << 2;
    tag = rep->tag;
    nItems = rep->nItems;
    if (client->swapped) {
	swapl(&tag, n);
	swapl(&nItems, n);
	swapl(&ptd.type, n);
    }
    if (len)
	ptd.length = nItems * (rep->format / 8);
    else
	ptd.length = 0;
    ptdp = &ptd;

    if (tag) {
	if (rep->length) {	/* first time, set tag */

#ifdef LBX_STATS
	    getprop_full++;
#endif

	    ptd.data = (pointer) &rep[1];
	    if (!propTagStoreData(tag, len, client->swapped, &ptd)) {
		/* tell server we lost it */
		SendInvalidateTag(client, rep->tag);
	    }
	} else {

	    ptdp = (PropertyTagDataPtr) TagGetData(prop_cache, tag);
	    if (!ptdp) {
		/* lost data -- ask again for tag value */

		qt.tag = tag;
		qt.tagtype = LbxTagTypeProperty;
		qt.typedata.getprop.offset = nr->request_info.lbxgetprop.offset;
		qt.typedata.getprop.length = nr->request_info.lbxgetprop.length;
		qt.typedata.getprop.ptd = ptd;
		QueryTag(client, &qt);

		/* XXX what is the right way to stack Queries? */
		return TRUE;
	    }

#ifdef LBX_STATS
	    getprop_tag++;
	    tag_bytes_unsent += ptdp->length;
#endif

	}
    } else {

#ifdef LBX_STATS
	getprop_full++;
#endif

	/*
	 * server didn't send us a tag -- either can't store it or no prop
	 * data
	 */
	if (rep->length)
	    ptd.data = (pointer) &rep[1];
	if (client->swapped) {
	    pdata = (pointer) ALLOCATE_LOCAL(ptdp->length);
	    bcopy((char *) ptd.data, (char *) pdata, ptdp->length);
	    if (pdata) {
		switch (rep->format) {
		case 32:
		    SwapLongs((CARD32 *) pdata, ptdp->length / 4);
		    break;
		case 16:
		    SwapShorts((short *) pdata, ptdp->length / 2);
		    break;
		default:
		    break;
		}
	    }
	}
    }

    FinishGetPropertyReply(client, rep->sequenceNumber,
			   nr->request_info.lbxgetprop.offset,
			   nr->request_info.lbxgetprop.length,
			   ptdp, pdata);
    if (pdata)
	DEALLOCATE_LOCAL(pdata);

    return TRUE;
}

int
ProcLBXGetProperty(client)
    ClientPtr   client;
{
    int         yank;

    REQUEST(xGetPropertyReq);

    /* just pass through 'delete' messages */
    if (stuff->delete) {

#ifdef DEBUG
	fprintf(stderr, "Got GetProperty with 'delete' set -- passing through\n");
#endif

	return ProcStandardRequest(client);
    }
    yank = get_property_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}
