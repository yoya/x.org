/* $XConsortium: lbxdix.c,v 1.7 94/12/01 20:29:36 mor Exp $ */
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
 * $NCDId: @(#)lbxdix.c,v 1.24 1994/09/23 20:51:28 lemke Exp $
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
#include "colormapst.h"
#include "resource.h"
#include "inputstr.h"
#include "servermd.h"
#include "dixfontstr.h"
#include "gcstruct.h"
#include "scrnintstr.h"
#include "windowstr.h"
#include "propertyst.h"
#define _XLBX_SERVER_
#include "lbxstr.h"
#include "lbxserve.h"
#include "lbxtags.h"
#include "lbxdata.h"
#include "assert.h"
#include "Xfuncproto.h"

extern void CopySwap32Write();
extern int  (*ProcVector[256]) ();
extern int  (*SwappedProcVector[256]) ();
extern void (*ReplySwapVector[256]) ();

extern void LbxWriteSConnSetupPrefix();

/* XXX should be per-proxy */
static int  motion_allowed_events = 0;

int         lbx_font_private;

void
LbxDixInit()
{
    TagInit();
    lbx_font_private = AllocateFontPrivateIndex();
}

/* ARGSUSED */
void
LbxAllowMotion(client, num)
    ClientPtr	client;
    int		num;
{
    /* XXX per-proxy */
    motion_allowed_events += num;
}

Bool
LbxIsLBXClient(client)
    ClientPtr   client;
{
    if (lbxClients[client->index])
	return TRUE;
    else
	return FALSE;
}

Bool
LbxThrottleMotionEvents(client, ev)
    ClientPtr   client;
    xEvent     *ev;
{
    /* XXX use client to check proxy */
    if (ev->u.u.type == MotionNotify) {
	if (motion_allowed_events == 0) {
	    DBG(DBG_CLIENT, (stderr, "throttling motion event for client %d\n", client->index));
	    return TRUE;
	} else {
	    motion_allowed_events--;
	}
    }
    return FALSE;
}

/*
 * this looks up the color associated with the pixel, and then calls
 * AllocColor()  a bit round-about, but it should work.
 */
void
LbxIncrementPixel(client, cmap, pixel, amount)
    int         client;
    Colormap    cmap;
    Pixel       pixel;
    int         amount;
{
    ColormapPtr pmap;
    EntryPtr    pent;
    unsigned short red,
                green,
                blue;
    int         i;

    pmap = (ColormapPtr) LookupIDByType(cmap, RT_COLORMAP);
    if (!pmap)
	return;
    switch (pmap->class) {
    case StaticColor:
    case StaticGray:
	red = pmap->red[pixel].co.local.red;
	green = pmap->red[pixel].co.local.green;
	blue = pmap->red[pixel].co.local.blue;
	break;
    case GrayScale:
    case PseudoColor:
	pent = pmap->red + pixel;
	red = pent->co.local.red;
	green = pent->co.local.green;
	blue = pent->co.local.blue;
	break;
    default:
/* XXX */
	return;
    }
    for (i = 0; i < amount; i++)
	AllocColor(pmap, &red, &green, &blue, &pixel, client);
}

static int  conn_info_tag;

extern WindowPtr *WindowTable;
extern xConnSetupPrefix connSetupPrefix;
extern char *ConnectionInfo;
extern int  connBlockScreenStart;

int
LbxSendConnSetup(client, reason)
    ClientPtr   client;
    char       *reason;
{
    xWindowRoot *root;
    TagData     td;
    pointer     tagdata;
    int         dlength;
    Bool        tag_known = FALSE,
                send_data;
    int         i;
    CARD32      dataBuf[1 + MAXSCREENS];
    xLbxConnSetupPrefix csp;

    if (reason)
	return SendConnSetup(client, reason);

    if (!conn_info_tag) {
	conn_info_tag = TagNewTag();
	tagdata = (pointer) ConnectionInfo;
	dlength = connSetupPrefix.length << 2;
	if (!TagSaveTag(conn_info_tag, LbxTagTypeConnInfo, 0, dlength,
			tagdata)) {
	    /* can't save it, so report no tag */
	    conn_info_tag = 0;
	}
    } else {
	td = TagGetTag(conn_info_tag);
	tagdata = td->tdata;
	dlength = td->size;
	tag_known = TagProxyMarked(conn_info_tag, LbxProxyID(client));
    }
    if (conn_info_tag)
	TagMarkProxy(conn_info_tag, LbxProxyID(client));

    send_data = (!conn_info_tag || !tag_known);

    IncrementClientCount();

    client->requestVector = client->swapped ? SwappedProcVector : ProcVector;
    client->sequence = 0;
    dataBuf[0] = ((xConnSetup *) ConnectionInfo)->ridBase = client->clientAsMask;
    ((xConnSetup *) ConnectionInfo)->ridMask = RESOURCE_ID_MASK;
    /* fill in the "currentInputMask" */
    root = (xWindowRoot *) (ConnectionInfo + connBlockScreenStart);
    for (i = 0; i < screenInfo.numScreens; i++) {
	register int j;
	register xDepth *pDepth;

	dataBuf[i + 1] = root->currentInputMask =
	    WindowTable[i]->eventMask | wOtherEventMasks(WindowTable[i]);
	pDepth = (xDepth *) (root + 1);
	for (j = 0; j < root->nDepths; j++) {
	    pDepth = (xDepth *) (((char *) (pDepth + 1)) +
				 pDepth->nVisuals * sizeof(xVisualType));
	}
	root = (xWindowRoot *) pDepth;
    }

    csp.success = TRUE;
    csp.tagOnly = !send_data;
    csp.majorVersion = connSetupPrefix.majorVersion;
    csp.minorVersion = connSetupPrefix.minorVersion;
    csp.tag = conn_info_tag;
    if (send_data) {
	csp.length = 1 + (dlength >> 2);
    } else {
	csp.length = 2 + screenInfo.numScreens;
	dlength = (1 + screenInfo.numScreens) << 2;
	tagdata = (pointer) dataBuf;
    }

    if (client->swapped) {
	LbxWriteSConnSetupPrefix(client, &csp);
	if (send_data) {
	    WriteSConnectionInfo(client, dlength, tagdata);
	} else {
	    SwapLongs((CARD32 *) tagdata, (1 + screenInfo.numScreens));
	    WriteToClient(client, dlength, tagdata);
	}
    } else {
	WriteToClient(client, sizeof(xLbxConnSetupPrefix), (char *) &csp);
	WriteToClient(client, dlength, tagdata);
    }

#ifndef NCD
    client->clientState = ClientStateRunning;
    if (ClientStateCallback)
	CallCallbacks(&ClientStateCallback, (pointer) client);
#endif

    return client->noClientException;
}

/*
 * This routine should be called if any of the ConnectionInfo changes
 */
LbxFlushConnInfoTag()
{

    if (conn_info_tag) {
	LbxSendInvalidateTagToProxies(conn_info_tag, LbxTagTypeConnInfo);
	TagDeleteTag(conn_info_tag);
	conn_info_tag = 0;
    }
}

extern InputInfo inputInfo;

static int  modifier_map_tag;

int
LbxGetModifierMapping(client)
    ClientPtr   client;
{
    TagData     td;
    pointer     tagdata;
    xLbxGetModifierMappingReply rep;
    register KeyClassPtr keyc = inputInfo.keyboard->key;
    int         dlength = keyc->maxKeysPerModifier << 3;
    Bool        tag_known = FALSE,
                send_data;
    int         n;

    if (!modifier_map_tag) {
	modifier_map_tag = TagNewTag();
	tagdata = (pointer) keyc->modifierKeyMap;
	if (!TagSaveTag(modifier_map_tag, LbxTagTypeModmap, 0, dlength,
			tagdata)) {
	    /* can't save it, so report no tag */
	    modifier_map_tag = 0;
	}
    } else {
	td = TagGetTag(modifier_map_tag);
	tagdata = td->tdata;
	tag_known = TagProxyMarked(modifier_map_tag, LbxProxyID(client));
    }
    if (modifier_map_tag)
	TagMarkProxy(modifier_map_tag, LbxProxyID(client));

    send_data = (!modifier_map_tag || !tag_known);

    rep.type = X_Reply;
    rep.keyspermod = keyc->maxKeysPerModifier;
    rep.sequenceNumber = client->sequence;
    rep.tag = modifier_map_tag;
    if (send_data)
	rep.length = dlength >> 2;
    else
	rep.length = 0;

    if (client->swapped) {
	swaps(&rep.sequenceNumber, n);
	swapl(&rep.length, n);
	swapl(&rep.tag, n);
    }
    WriteToClient(client, sizeof(xLbxGetModifierMappingReply), &rep);

    if (send_data)
	WriteToClient(client, dlength, (char *) tagdata);

    return client->noClientException;
}

LbxFlushModifierMapTag()
{

    if (modifier_map_tag) {
	LbxSendInvalidateTagToProxies(modifier_map_tag, LbxTagTypeModmap);
	TagDeleteTag(modifier_map_tag);
	modifier_map_tag = 0;
    }
}

static int  keyboard_map_tag;

int
LbxGetKeyboardMapping(client)
    ClientPtr   client;
{
    TagData     td;
    pointer     tagdata;
    xLbxGetKeyboardMappingReply rep;

    REQUEST(xLbxGetKeyboardMappingReq);
    KeySymsPtr  curKeySyms = &inputInfo.keyboard->key->curKeySyms;
    int         dlength;
    Bool        tag_known = FALSE,
                send_data;
    int         n;

    REQUEST_SIZE_MATCH(xLbxGetKeyboardMappingReq);

/* XXX do we need this?  proxy should be smart enough to do this right
 * all the time
 */
    if ((stuff->firstKeyCode < curKeySyms->minKeyCode) ||
	    (stuff->firstKeyCode > curKeySyms->maxKeyCode)) {
	client->errorValue = stuff->firstKeyCode;
	return BadValue;
    }
    if (stuff->firstKeyCode + stuff->count > curKeySyms->maxKeyCode + 1) {
	client->errorValue = stuff->count;
	return BadValue;
    }
    rep.type = X_Reply;
    rep.sequenceNumber = client->sequence;
    rep.keysperkeycode = curKeySyms->mapWidth;
    /* length is a count of 4 byte quantities and KeySyms are 4 bytes */

    if (!keyboard_map_tag) {
	keyboard_map_tag = TagNewTag();
	tagdata = (pointer) curKeySyms->map[(stuff->firstKeyCode -
			     curKeySyms->minKeyCode) * curKeySyms->mapWidth];
	dlength = (curKeySyms->mapWidth * stuff->count);
	if (!TagSaveTag(keyboard_map_tag, LbxTagTypeKeymap, 0, dlength,
			tagdata)) {
	    /* can't save it, so report no tag */
	    keyboard_map_tag = 0;
	}
    } else {
	td = TagGetTag(keyboard_map_tag);
	tagdata = td->tdata;
	tag_known = TagProxyMarked(keyboard_map_tag, LbxProxyID(client));
    }
    if (keyboard_map_tag)
	TagMarkProxy(keyboard_map_tag, LbxProxyID(client));

    send_data = (!keyboard_map_tag || !tag_known);

    rep.type = X_Reply;
    rep.keysperkeycode = curKeySyms->mapWidth;
    rep.sequenceNumber = client->sequence;
    rep.tag = keyboard_map_tag;
    if (send_data)
	rep.length = (curKeySyms->mapWidth * stuff->count);
    else
	rep.length = 0;

    if (client->swapped) {
	swaps(&rep.sequenceNumber, n);
	swapl(&rep.length, n);
	swapl(&rep.tag, n);
    }
    WriteToClient(client, sizeof(xLbxGetKeyboardMappingReply), &rep);

    if (send_data) {
	client->pSwapReplyFunc = CopySwap32Write;
	WriteSwappedDataToClient(client,
			curKeySyms->mapWidth * stuff->count * sizeof(KeySym),
	    &curKeySyms->map[(stuff->firstKeyCode - curKeySyms->minKeyCode) *
			     curKeySyms->mapWidth]);
    }
    return client->noClientException;
}

LbxFlushKeyboardMapTag()
{

    if (keyboard_map_tag) {
	LbxSendInvalidateTagToProxies(keyboard_map_tag, LbxTagTypeKeymap);
	TagDeleteTag(keyboard_map_tag);
	keyboard_map_tag = 0;
    }
}

int
LbxQueryFont(client)
    ClientPtr   client;
{
    xQueryFontReply *reply,
               *sreply = NULL;
    xLbxQueryFontReply lbxrep;
    FontPtr     pFont;
    register GC *pGC;
    Bool        send_data = FALSE;
    Bool        free_data = FALSE;
    int         rlength = 0;
    TagData     td = NULL;
    XID         tid;
    FontTagInfoPtr ftip;

    REQUEST(xLbxQueryFontReq);

    REQUEST_SIZE_MATCH(xLbxQueryFontReq);

    client->errorValue = stuff->fid;	/* EITHER font or gc */
    pFont = (FontPtr) LookupIDByType(stuff->fid, RT_FONT);
    if (!pFont) {
	/* can't use VERIFY_GC because it might return BadGC */
	pGC = (GC *) LookupIDByType(stuff->fid, RT_GC);
	if (!pGC || !pGC->font) {	/* catch a non-existent builtin font */
	    client->errorValue = stuff->fid;
	    return (BadFont);	/* procotol spec says only error is BadFont */
	}
	pFont = pGC->font;
    }
    /* get tag (if any) */
    td = (TagData) FontGetPrivate(pFont, lbx_font_private);

    if (!td) {
	xCharInfo  *pmax = FONTINKMAX(pFont);
	xCharInfo  *pmin = FONTINKMIN(pFont);
	int         nprotoxcistructs;

	nprotoxcistructs = (
			  pmax->rightSideBearing == pmin->rightSideBearing &&
			    pmax->leftSideBearing == pmin->leftSideBearing &&
			    pmax->descent == pmin->descent &&
			    pmax->ascent == pmin->ascent &&
			    pmax->characterWidth == pmin->characterWidth) ?
	    0 : N2dChars(pFont);

	rlength = sizeof(xQueryFontReply) +
	    FONTINFONPROPS(FONTCHARSET(pFont)) * sizeof(xFontProp) +
	    nprotoxcistructs * sizeof(xCharInfo);
	reply = (xQueryFontReply *) xalloc(rlength);
	if (!reply) {
	    return (BadAlloc);
	}
	free_data = TRUE;
	send_data = TRUE;
	QueryFont(pFont, reply, nprotoxcistructs);
    } else {			/* just get data from tag */
	ftip = (FontTagInfoPtr) td->tdata;
	reply = ftip->replydata;
	rlength = ftip->size;
    }

    if (!td) {
	/* data allocation is done when font is first queried */
	tid = TagNewTag();
	ftip = (FontTagInfoPtr) xalloc(sizeof(FontTagInfoRec));
	if (ftip && TagSaveTag(tid, LbxTagTypeFont, 0, sizeof(FontTagInfoRec),
			       (pointer) ftip)) {
	    td = TagGetTag(tid);
	    FontSetPrivate(pFont, lbx_font_private, (pointer) td);
	    ftip = (FontTagInfoPtr) td->tdata;
	    ftip->pfont = pFont;
	    ftip->size = rlength;
	    ftip->replydata = reply;
	    free_data = FALSE;
	} else {
	    xfree(ftip);
	}
    }
    if (td) {
	TagMarkProxy(td->tid, LbxProxyID(client));
	lbxrep.tag = td->tid;
    } else {
	lbxrep.tag = 0;
	send_data = TRUE;
    }

    lbxrep.type = X_Reply;
    lbxrep.sequenceNumber = client->sequence;
    if (send_data)
	lbxrep.length = rlength >> 2;
    else
	lbxrep.length = 0;

    if (client->swapped) {
	int         n;

	swaps(&lbxrep.sequenceNumber, n);
	swapl(&lbxrep.length, n);
	swapl(&lbxrep.tag, n);
	sreply = (xQueryFontReply *) ALLOCATE_LOCAL(rlength);
	if (!sreply)
	    return BadAlloc;
	bcopy((char *) reply, (char *) sreply, rlength);
	SwapFont((xQueryFontReply *) sreply, TRUE);
	reply = sreply;
    }
    WriteToClient(client, sizeof(xLbxQueryFontReply), (char *) &lbxrep);
    if (send_data)
	WriteToClient(client, rlength, reply);
    if (free_data)
	xfree(reply);
    if (sreply)
	DEALLOCATE_LOCAL(sreply);
    return (client->noClientException);
}

void
LbxFreeFontTag(pfont)
    FontPtr     pfont;
{
    TagData     td;

    td = (TagData) FontGetPrivate(pfont, lbx_font_private);
    if (td) {
	LbxSendInvalidateTagToProxies(td->tid, LbxTagTypeFont);
	TagDeleteTag(td->tid);
    }
}

int
LbxQueryTag(client, tag)
    ClientPtr   client;
    XID         tag;
{
    xLbxQueryTagReply rep;
    TagData     td;
    int         n;
    FontTagInfoPtr ftip;
    unsigned long size;
    pointer     data;
    pointer     sdata = NULL;

    td = TagGetTag(tag);

    rep.type = X_Reply;
    rep.sequenceNumber = client->sequence;
    if (!td) {
	rep.valid = 0;
	rep.length = 0;
	rep.real_length = 0;
    } else {
	if (td->data_type == LbxTagTypeFont) {
	    ftip = (FontTagInfoPtr) td->tdata;
	    size = ftip->size;
	    data = (pointer) ftip->replydata;
	} else if (td->data_type == LbxTagTypeProperty) {
	    size = td->size;
	    data = td->tdata;
	} else {
	    size = td->size;
	    data = td->tdata;
	}
	rep.valid = 1;
	rep.length = (size + 3) >> 2;
	rep.real_length = size;
    }
    if (rep.valid && client->swapped) {
	swaps(&rep.sequenceNumber, n);
	sdata = (pointer) ALLOCATE_LOCAL(rep.real_length);
	if (!sdata)
	    return BadAlloc;
	bcopy((char *) data, (char *) sdata, rep.real_length);
	switch (td->data_type) {
	case LbxTagTypeModmap:
	    /* no swapping necessary */
	    break;
	case LbxTagTypeKeymap:
	    SwapLongs((CARD32 *) sdata, rep.real_length / 4);
	    break;
	case LbxTagTypeFont:
	    SwapFont((xQueryFontReply *) sdata, TRUE);
	    break;
	case LbxTagTypeProperty:
	    switch (td->data_format) {
	    case LbxTagFormat32:
		SwapLongs((CARD32 *) sdata, rep.real_length / 4);
		break;
	    case LbxTagFormat16:
		SwapShorts((short *) sdata, rep.real_length / 2);
		break;
	    default:
		break;
	    }
	case LbxTagTypeConnInfo:
	    /* XXX */
	    break;
	}
	data = sdata;

	swapl(&rep.length, n);
	swapl(&rep.real_length, n);
    }
    WriteToClient(client, sizeof(xLbxQueryTagReply), (char *) &rep);
    if (td)
	WriteToClient(client, size, (char *) data);
    if (sdata)
	DEALLOCATE_LOCAL(sdata);

    return client->noClientException;
}

LbxInvalidateTag(client, tag)
    ClientPtr   client;
    XID         tag;
{
    TagClearProxy(tag, LbxProxyID(client));
    return client->noClientException;
}

LbxSendInvalidateTag(client, tag, tagtype)
    ClientPtr   client;
    XID         tag;
    int         tagtype;
{
    xLbxEvent   ev;
    int         n;
    extern int  LbxEventCode;

    ev.type = LbxEventCode;
    ev.lbxType = LbxInvalidateTagEvent;
    ev.sequenceNumber = client->sequence;
    ev.client = client->index;
    ev.detail1 = tag;
    ev.detail2 = tagtype;

    if (client->swapped) {
	swaps(&ev.sequenceNumber, n);
	swapl(&ev.client, n);
	swapl(&ev.detail1, n);
	swapl(&ev.detail2, n);
    }
    DBG(DBG_CLIENT, (stderr, "Invalidating tag  %d\n", tag));
    WriteToClient(client, sizeof(xLbxEvent), (char *) &ev);
}


LbxSendInvalidateTagToProxies(tag, tagtype)
    XID         tag;
    int         tagtype;
{
    LbxProxyPtr proxy;
    ClientPtr   client;
    LbxClientPtr lbxcp;
    extern LbxProxyPtr proxyList;

    /* send Invalidates to all proxies */
    proxy = proxyList;
    while (proxy) {
	/* find some client of the proxy to use */
	lbxcp = proxy->lbxClients[LbxMasterClientIndex];
	if (lbxcp && (client = lbxcp->client)) {
	    LbxSendInvalidateTag(client, tag, tagtype);
	    TagClearProxy(tag, LbxProxyID(client));
	}
	proxy = proxy->next;
    }
}

LbxProxyPtr
LbxPidToProxy(pid)
    int         pid;
{
    LbxProxyPtr proxy;
    extern LbxProxyPtr proxyList;

    proxy = proxyList;
    while (proxy) {
	if (proxy->pid == pid)
	    return proxy;
    }
    assert(0);
    return NULL;
}

void
LbxSendSendTagData(pid, tag, tagtype)
    int         pid;
    XID         tag;
    int         tagtype;
{
    xLbxEvent   ev;
    int         n;
    extern int  LbxEventCode;
    LbxProxyPtr proxy;
    ClientPtr   client;
    LbxClientPtr lbxcp;

    proxy = LbxPidToProxy(pid);
    lbxcp = proxy->lbxClients[LbxMasterClientIndex];
    if (lbxcp && (client = lbxcp->client)) {
	ev.type = LbxEventCode;
	ev.lbxType = LbxSendTagDataEvent;
	ev.sequenceNumber = client->sequence;
	ev.client = client->index;
	ev.detail1 = tag;
	ev.detail2 = tagtype;

	if (client->swapped) {
	    swaps(&ev.sequenceNumber, n);
	    swapl(&ev.client, n);
	    swapl(&ev.detail1, n);
	    swapl(&ev.detail2, n);
	}
	DBG(DBG_CLIENT, (stderr, "Requesting tag %d\n", tag));
	WriteToClient(client, sizeof(xLbxEvent), (char *) &ev);
    }
}

/*
 * keep track of clients stalled waiting for tags to come back from
 * a proxy.  since multiple clinets can be waiting for the same tag,
 * we have to keep a list of all of them.
 */

typedef struct _sendtagq {
    XID         tag;
    int         tagtype;	/* XXX unnecessary? */
    int         num_stalled;
    ClientPtr  *stalled_clients;
    pointer     infop;
    struct _sendtagq *next;
}           SendTagQRec, *SendTagQPtr;

static SendTagQPtr queried_tags = NULL;

#define	LbxSendTagFailed	-1
#define	LbxSendTagSendIt	0
#define	LbxSendTagAlreadySent	1

static Bool
LbxQueueSendTag(client, tag, tagtype, infop)
    ClientPtr   client;
    XID         tag;
    int         tagtype;
    pointer     infop;
{
    SendTagQPtr sqtp,
                prev = NULL,
                new;
    ClientPtr  *newlist;

    sqtp = queried_tags;
    /* see if we're asking for one already in the pipeline */
    while (sqtp) {
	if (sqtp->tag == tag) {
	    assert(sqtp->tagtype == tagtype);
	    assert(sqtp->infop == infop);
	    /* add new client to list */
	    newlist = (ClientPtr *) xrealloc(sqtp->stalled_clients,
			      (sizeof(ClientPtr) * (sqtp->num_stalled + 1)));
	    if (!newlist)
		return LbxSendTagFailed;
	    newlist[sqtp->num_stalled++] = client;
	    sqtp->stalled_clients = newlist;
	    DBG(DBG_CLIENT, (stderr, "Additional client requesting tag %d\n", tag));
	    return LbxSendTagAlreadySent;
	}
	prev = sqtp;
	sqtp = sqtp->next;
    }

    /* make new one */
    new = (SendTagQPtr) xalloc(sizeof(SendTagQRec));
    newlist = (ClientPtr *) xalloc(sizeof(ClientPtr));
    if (!new || !newlist) {
	xfree(new);
	xfree(newlist);
	return LbxSendTagFailed;
    }
    *newlist = client;
    new->stalled_clients = newlist;
    new->num_stalled = 1;
    new->tag = tag;
    new->tagtype = tagtype;
    new->infop = infop;
    new->next = NULL;

    /* stick on end of list */
    if (prev)
	prev->next = new;
    else
	queried_tags = new;
    return LbxSendTagSendIt;
}

static      SendTagQPtr
LbxFindQTag(tag)
{
    SendTagQPtr sqtp;

    sqtp = queried_tags;
    while (sqtp) {
	if (sqtp->tag == tag) {
	    return sqtp;
	}
	sqtp = sqtp->next;
    }
    assert(0);
    return (SendTagQPtr) NULL;
}

static void
LbxFreeQTag(sqtp)
    SendTagQPtr sqtp;
{
    xfree(sqtp->stalled_clients);
    xfree(sqtp);
}

static void
LbxRemoveQTag(tag)
    XID	tag;
{
    SendTagQPtr sqtp,
                prev = NULL;

    sqtp = queried_tags;
    while (sqtp) {
	if (sqtp->tag == tag) {
	    if (prev)
		prev->next = sqtp->next;
	    else
		queried_tags = sqtp->next;
	    LbxFreeQTag(sqtp);
	    return;
	}
	prev = sqtp;
	sqtp = sqtp->next;
    }
    assert(0);
}

/*
 * server sends this
 */

void
LbxQueryTagData(client, owner_pid, tag, tagtype, infop)
    ClientPtr   client;
    int         owner_pid;
    XID         tag;
    int         tagtype;
    pointer     infop;
{
    TagData     td;
    PropertyPtr pProp;

    /* make sure the proxy is there */
    if (!LbxPidToProxy(owner_pid)) {
	/*
	 * its dead, so pretend data is already there XXX value will be junk,
	 * but its better than gettting stuck
	 */
	td = TagGetTag(tag);
	switch (td->data_type) {
	case LbxTagTypeProperty:
	    /* mark it as known by server */
	    pProp = (PropertyPtr) infop;
	    pProp->owner_pid = 0;
	    break;
	default:
	    /* don't do others yet (ever?) */
	    break;
	}
	LbxAttendClient(client);
	return;
    }
    /* save the info and the client being stalled */
    if (LbxQueueSendTag(client, tag, tagtype, infop) == LbxSendTagSendIt) {
	LbxSendSendTagData(owner_pid, tag, tagtype);
    }
}

/*
 * server recieves this
 */
int
LbxTagData(client, tag, len, data)
    ClientPtr   client;
    XID         tag;
    unsigned long len;
    pointer     data;
{
    TagData     td;
    SendTagQPtr stqp;
    ClientPtr   cp;
    PropertyPtr pProp;

    stqp = LbxFindQTag(tag);

    td = TagGetTag(tag);
    assert(stqp->tagtype == td->data_type);

    if (stqp && td) {
	switch (td->data_type) {
	case LbxTagTypeProperty:
	    assert(len == td->size);
	    /* mark it as known by server */
	    pProp = (PropertyPtr) stqp->infop;
	    pProp->owner_pid = 0;
	    assert(pProp->format == td->data_format);
	    if (len == td->size) {
		/* didn't swap data up above, so do it here */
		if (client->swapped) {
		    switch (pProp->format) {
		    case 32:
			SwapLongs((CARD32 *) data, len / 4);
			break;
		    case 16:
			SwapShorts((short *) data, len / 2);
			break;
		    default:
			break;
		    }
		}
		bcopy((char *) data, (char *) td->tdata, len);
	    }
	    break;
	default:
	    /* don't do others yet (ever?) */
	    break;
	}

	/* wake up stalled clients */
	cp = *stqp->stalled_clients;
	while (stqp->num_stalled--) {
	    LbxAttendClient(cp);
	    cp++;
	}
	LbxRemoveQTag(tag);
    }
    return Success;
}

/*
 * clear out markers for proxies
 */
LbxFlushTags(proxy)
    LbxProxyPtr proxy;
{
    if (conn_info_tag)
	TagClearProxy(conn_info_tag, proxy->pid);
    if (modifier_map_tag)
	TagClearProxy(modifier_map_tag, proxy->pid);
    if (keyboard_map_tag)
	TagClearProxy(keyboard_map_tag, proxy->pid);
    /* XXX flush all font tags -- how? */
    /*
     * without this, a proxy re-connect will confuse things slightly -- the
     * proxy will need to do QueryTag to get in sync on font data & global
     * props
     */
}

/* when server resets, need to reset global tags */
LbxResetTags()
{
    SendTagQPtr stqp,
                next;

    conn_info_tag = 0;
    modifier_map_tag = 0;
    keyboard_map_tag = 0;

    /* clean out any pending tag requests */
    stqp = queried_tags;
    while (stqp) {
	next = stqp->next;
	LbxFreeQTag(stqp);
	stqp = next;
    }
}
