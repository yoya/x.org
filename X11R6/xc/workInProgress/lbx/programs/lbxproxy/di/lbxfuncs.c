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
 * $NCDId: @(#)lbxfuncs.c,v 1.40 1994/11/18 20:39:18 lemke Exp $
 */

/* $XConsortium: lbxfuncs.c,v 1.7 94/12/01 20:50:00 mor Exp $ */

/*
 * top level LBX request & reply handling
 */

/*
 * XXX
 *
 * byte swapping is ugly -- might be better to call SwapFooReply()
 * twice in short-circuiting stuff, rather than swap out what we care
 * about.  same for requests.  probably depends on how reencoding falls
 * out.
 *
 * requests copy out interesting stuff and then swap so original data
 * is left alone as much as possible
 *
 * replied data is copied yet again before swapping because the data
 * may be stored as a tag result and we don't want to change that.
 */


#include	<stdio.h>
#define NEED_REPLIES
#define NEED_EVENTS
#include	<X11/X.h>	/* for KeymapNotify */
#include	<X11/Xproto.h>
#include	"assert.h"
#include	"lbxdata.h"
#include	"atomcache.h"
#include	"util.h"
#include	"tags.h"
#include	"colormap.h"
#include	"cmapst.h"
#include	"lbx.h"		/* gets dixstruct.h */
#include	"resource.h"
#include	"wire.h"
#include	"swap.h"
#include	"reqtype.h"
#define _XLBX_SERVER_
#include	"lbxstr.h"
#include	"lbxext.h"

#define reply_length(cp,rep) ((rep)->type==X_Reply ? \
        32 + (HostUnswapLong((cp),(rep)->length) << 2) \
	: 32)

#define	server_resource(client, xid)	(CLIENT_ID(xid) == 0)

extern int  xMajorVersion;
extern int  xMinorVersion;

void
FinishSetupReply(client, setup_len, setup_data, changes, majorVer, minorVer)
    ClientPtr   client;
    int         setup_len;
    xConnSetup *setup_data;
    pointer     changes;
    CARD16      majorVer,
                minorVer;
{
    xConnSetupPrefix reply;

    reply.success = TRUE;
    reply.majorVersion = majorVer;
    reply.minorVersion = minorVer;
    reply.length = setup_len >> 2;

    GetConnectionInfo(client, setup_data, changes);
    if (client->swapped) {
	SwapConnSetupPrefix(&reply);
    }
    WriteToClient(client, sizeof(xConnSetupPrefix), &reply);
    if (client->swapped) {
	WriteSConnectionInfo(client, setup_len, (char *) setup_data);
    } else {
	WriteToClient(client, setup_len, setup_data);
    }
}

static void
get_tagged_setup_reply(client, data)
    ClientPtr   client;
    char       *data;
{
    register xLbxConnSetupPrefix *rep;
    TagData     td;
    pointer     tag_data;
    pointer     change_list = NULL;
    int         len;
    QueryTagRec qt;

    rep = (xLbxConnSetupPrefix *) data;
    if (client->swapped) {
	char        n;

	SwapLbxConnSetupPrefix(rep);
	swaps(&rep->length, n);
    }
    len = (rep->length - 1) << 2;
    if (rep->tag) {
	if (!rep->tagOnly) {	/* first time, set tag */

#ifdef LBX_STATS
	    getsetup_full++;
#endif

	    tag_data = (pointer) &rep[1];
	    if (client->swapped) {
		SwapConnectionInfo((xConnSetup *) tag_data);
	    }
	    if (!TagStoreData(global_cache, rep->tag, len,
			      LbxTagTypeConnInfo, tag_data)) {
		/* tell server we lost it */
		SendInvalidateTag(client, rep->tag);
	    }
	} else {
	    td = TagGetTag(global_cache, rep->tag);
	    if (!td) {
		qt.tag = rep->tag;
		qt.tagtype = LbxTagTypeConnInfo;
		qt.typedata.setup.majorVersion = rep->majorVersion;
		qt.typedata.setup.minorVersion = rep->minorVersion;
		qt.typedata.setup.changes = (pointer) xalloc(len);
		if (!qt.typedata.setup.changes)
		    CloseDownClient(client);
		bcopy((char *) &rep[1], qt.typedata.setup.changes, len);
		/* lost data -- ask again for tag value */
		QueryTag(client, &qt);

		/* XXX what is the right way to stack Queries? */
		return;
	    }

#ifdef LBX_STATS
	    getsetup_tag++;
	    tag_bytes_unsent += td->size - len;
#endif

	    tag_data = td->tdata;
	    len = td->size;
	    change_list = (pointer) &rep[1];
	    if (client->swapped) {
		SwapLongs((CARD32 *) change_list, 2);
	    }
	}
    } else {

#ifdef LBX_STATS
	getsetup_full++;
#endif

	/* server didn't send us a tag for some reason -- just pass on data */
	tag_data = (pointer) &rep[1];
	if (client->swapped) {
	    SwapConnectionInfo((xConnSetup *) tag_data);
	}
    }

    FinishSetupReply(client, len, tag_data, change_list,
		     rep->majorVersion, rep->minorVersion);

    return;
}

static int
intern_atom_req(client, data)
    ClientPtr   client;
    char       *data;
{
    xInternAtomReq *req;
    char       *s;
    Atom        atom,
                a = None;
    xInternAtomReply reply;
    ReplyStuffPtr nr;
    char        n;
    CARD16      nbytes;

    req = (xInternAtomReq *) data;

    nbytes = req->nbytes;
    if (client->swapped) {
	swaps(&nbytes, n);
    }
    if (nbytes > MAX_ATOM_LENGTH)
	return REQ_NOCHANGE;

    s = data + sizeof(xInternAtomReq);

    atom = LbxMakeAtom(s, nbytes, a, FALSE);
    if (atom != None && LBXCanDelayReply(client)) {
	reply.type = X_Reply;
	reply.length = 0;
	reply.sequenceNumber = LBXSequenceNumber(client);
	reply.atom = atom;
	if (client->swapped) {
	    SwapInternAtomReply(&reply);
	}
	if (LBXCacheSafe(client))
	    WriteToClient(client, sizeof(xInternAtomReply), &reply);
	else			/* store for later */
	    SaveReplyData(client, (xReply *) & reply, 0, NULL);

#ifdef LBX_STATS
	intern_good++;
#endif

	return REQ_YANK;
    } else if (nbytes < MAX_ATOM_LENGTH) {
	nr = NewReply(client);
	if (nr) {
	    strncpy(nr->request_info.lbxatom.str, s, nbytes);
	    nr->request_info.lbxatom.str[nbytes] = '\0';
	    nr->request_info.lbxatom.len = nbytes;
	    nr->sequenceNumber = LBXSequenceNumber(client);
	    nr->request = X_InternAtom;
	}
    }

#ifdef LBX_STATS
    intern_miss++;
#endif

    return REQ_NOCHANGE;
}

static Bool
intern_atom_reply(client, data)
    ClientPtr   client;
    char       *data;
{
    Atom        atom;
    char       *str;
    xInternAtomReply *reply;
    int         len;
    ReplyStuffPtr nr;
    char        n;

    reply = (xInternAtomReply *) data;

    nr = GetMatchingReply(client, reply->sequenceNumber);
    assert(nr);
    str = nr->request_info.lbxatom.str;
    len = nr->request_info.lbxatom.len;

    atom = reply->atom;
    if (client->swapped) {
	swapl(&atom, n);
    }
    if (atom != None) {
	/* make sure it gets stuffed in the DB */
	(void) LbxMakeAtom(str, len, atom, TRUE);
    }
    return TRUE;
}

static int
get_atom_name_req(client, data)
    ClientPtr   client;
    char       *data;
{
    xResourceReq *req;
    char       *str;
    xGetAtomNameReply reply;
    int         len;
    ReplyStuffPtr nr;
    char        n;
    Atom        id;

    req = (xResourceReq *) data;

    id = req->id;
    if (client->swapped) {
	swapl(&id, n);
    }
    str = NameForAtom(id);

    if (str && LBXCanDelayReply(client)) {	/* found the value */
	len = strlen(str);
	reply.type = X_Reply;
	reply.length = (len + 3) >> 2;
	reply.sequenceNumber = LBXSequenceNumber(client);
	reply.nameLength = len;
	if (client->swapped) {
	    SwapGetAtomNameReply(&reply);
	}
	if (LBXCacheSafe(client)) {
	    WriteToClient(client, sizeof(xGetAtomNameReply), &reply);
	    WriteToClientPad(client, len, str);
	} else {
	    SaveReplyData(client, (xReply *) & reply, len, str);
	}

#ifdef LBX_STATS
	getatom_good++;
#endif

	return REQ_YANK;
    } else {
	nr = NewReply(client);
	if (nr) {
	    nr->request_info.lbxatom.atom = id;
	    nr->sequenceNumber = LBXSequenceNumber(client);
	    nr->request = X_GetAtomName;
	}
    }

#ifdef LBX_STATS
    getatom_miss++;
#endif

    return REQ_NOCHANGE;
}

static Bool
get_atom_name_reply(client, data)
    ClientPtr   client;
    char       *data;
{
    Atom        atom;
    char       *s;
    xGetAtomNameReply *reply;
    int         len;
    ReplyStuffPtr nr;

    reply = (xGetAtomNameReply *) data;

    if ((reply->length << 2) > MAX_ATOM_LENGTH)
	return FALSE;

    len = (reply->length << 2) + sizeof(xGetAtomNameReply);

    s = data + sizeof(xGetAtomNameReply);

    len -= sizeof(xGetAtomNameReply);
    nr = GetMatchingReply(client, reply->sequenceNumber);
    assert(nr);
    atom = nr->request_info.lbxatom.atom;

    /* make sure it gets stuffed in the DB */
    (void) LbxMakeAtom(s, len, atom, TRUE);
    return TRUE;
}

static int
lookup_color_req(client, data)
    ClientPtr   client;
    char       *data;
{
    xLookupColorReq *req;
    xLookupColorReply reply;
    ReplyStuffPtr nr;
    RGBEntryPtr rgbe;
    int         len;
    char        n;
    Colormap    cmap;

    req = (xLookupColorReq *) data;

    len = req->nbytes;
    cmap = req->cmap;
    if (client->swapped) {
	swapl(&cmap, n);
	swaps(&len, n);
    }
    if (len > MAX_COLORNAME_LENGTH)
	return REQ_NOCHANGE;

/* XXX for now, ignore attempts to anything but default colormaps */
    if (!server_resource(client, cmap))
	return REQ_NOCHANGE;

    rgbe = FindColorName((char *) &req[1], len, cmap);

    if (rgbe && LBXCanDelayReply(client)) {	/* found the value */
	reply.type = X_Reply;
	reply.length = 0;
	reply.sequenceNumber = LBXSequenceNumber(client);

	reply.exactRed = rgbe->xred;
	reply.exactBlue = rgbe->xblue;
	reply.exactGreen = rgbe->xgreen;

	reply.screenRed = rgbe->vred;
	reply.screenBlue = rgbe->vblue;
	reply.screenGreen = rgbe->vgreen;

	if (client->swapped) {
	    SwapLookupColorReply(&reply);
	}
	if (LBXCacheSafe(client))
	    WriteToClient(client, sizeof(xLookupColorReply), &reply);
	else
	    SaveReplyData(client, (xReply *) & reply, 0, NULL);

#ifdef LBX_STATS
	luc_good++;
#endif

	return REQ_YANK;
    } else {
	nr = NewReply(client);
	if (nr) {
	    strncpy(nr->request_info.lbxlookupcolor.name,
		    (char *) &req[1], len);
	    nr->request_info.lbxlookupcolor.namelen = len;
	    nr->request_info.lbxlookupcolor.cmap = cmap;
	    nr->sequenceNumber = LBXSequenceNumber(client);
	    nr->request = X_LookupColor;
	}
    }

#ifdef LBX_STATS
    luc_miss++;
#endif

    return REQ_NOCHANGE;
}

static Bool
lookup_color_reply(client, data)
    ClientPtr   client;
    char       *data;
{
    xLookupColorReply *reply;
    ReplyStuffPtr nr;
    RGBEntryRec rgbe;
    char        n;

    reply = (xLookupColorReply *) data;

    nr = GetMatchingReply(client, reply->sequenceNumber);
    assert(nr);

    rgbe.xred = reply->exactRed;
    rgbe.xblue = reply->exactBlue;
    rgbe.xgreen = reply->exactGreen;
    rgbe.vred = reply->screenRed;
    rgbe.vblue = reply->screenBlue;
    rgbe.vgreen = reply->screenGreen;

    rgbe.cmap = nr->request_info.lbxlookupcolor.cmap;
    if (client->swapped) {
	swapl(&rgbe.cmap, n);
	swaps(&rgbe.xred, n);
	swaps(&rgbe.xgreen, n);
	swaps(&rgbe.xblue, n);
	swaps(&rgbe.vred, n);
	swaps(&rgbe.xgreen, n);
	swaps(&rgbe.xblue, n);
    }
    AddColorName(nr->request_info.lbxlookupcolor.name,
		 nr->request_info.lbxlookupcolor.namelen,
		 &rgbe);
    return TRUE;
}

static int
alloc_color_req(client, data)
    ClientPtr   client;
    char       *data;
{
    xAllocColorReq *req;
    xAllocColorReply reply;
    Entry      *pent;
    ReplyStuffPtr nr;
    Colormap    cmap;
    CARD16      red,
                green,
                blue;
    char        n;

    req = (xAllocColorReq *) data;

    cmap = req->cmap;
    red = req->red;
    green = req->green;
    blue = req->blue;
    if (client->swapped) {
	swapl(&cmap, n);
	swaps(&red, n);
	swaps(&green, n);
	swaps(&blue, n);
    }
    FindPixel(client, cmap, (int) red, (int) green,
	      (int) blue, &pent);
    if (pent && LBXCanDelayReply(client)) {	/* found the value */
	/* always inc the pixel, so our refcounts match the server's */
	IncrementPixel(client, cmap, pent);

	/* must tell server to bump refcnt */
	SendIncrementPixel(client, req->cmap, pent->pixel);

	reply.type = X_Reply;
	reply.length = 0;
	reply.sequenceNumber = LBXSequenceNumber(client);

	reply.red = pent->rep_red;
	reply.green = pent->rep_green;
	reply.blue = pent->rep_blue;
	reply.pixel = pent->pixel;

	if (client->swapped) {
	    SwapAllocColorReply(&reply);
	}
	if (LBXCacheSafe(client))
	    WriteToClient(client, sizeof(xAllocColorReply), &reply);
	else
	    SaveReplyData(client, (xReply *) & reply, 0, NULL);

#ifdef LBX_STATS
	ac_good++;
#endif

	return REQ_REPLACE;	/* packet sent anyways */
    } else {
	nr = NewReply(client);
	if (nr) {
	    nr->request_info.lbxalloccolor.cmap = cmap;
	    nr->request_info.lbxalloccolor.red = red;
	    nr->request_info.lbxalloccolor.green = green;
	    nr->request_info.lbxalloccolor.blue = blue;
	    nr->sequenceNumber = LBXSequenceNumber(client);
	    nr->request = X_AllocColor;
	}
    }

#ifdef LBX_STATS
    ac_miss++;
#endif

    return REQ_NOCHANGE;
}

static Bool
alloc_color_reply(client, data)
    ClientPtr   client;
    char       *data;
{
    xAllocColorReply *reply;
    ReplyStuffPtr nr;
    CARD32      pixel;
    char        n;
    CARD16      red,
                green,
                blue;

    reply = (xAllocColorReply *) data;

    nr = GetMatchingReply(client, reply->sequenceNumber);
    assert(nr);

    pixel = reply->pixel;
    red = reply->red;
    green = reply->green;
    blue = reply->blue;
    if (client->swapped) {
	swapl(&pixel, n);
	swaps(&red, n);
	swaps(&green, n);
	swaps(&blue, n);
    }
    /*
     * save requested color, since they're more likely to ask for the same
     * thing again.  also have to store reply RGB.
     */
    StorePixel(client, nr->request_info.lbxalloccolor.cmap,
	       nr->request_info.lbxalloccolor.red,
	       nr->request_info.lbxalloccolor.green,
	       nr->request_info.lbxalloccolor.blue,
	       (int) red, (int) green, (int) blue, pixel);

    return TRUE;
}

static int
alloc_named_color_req(client, data)
    ClientPtr   client;
    char       *data;
{
    xAllocNamedColorReq *req;
    xAllocNamedColorReply reply;
    Entry      *pent;
    RGBEntryPtr rgbe;
    ReplyStuffPtr nr;
    CARD16      nbytes;
    Colormap    cmap;
    char        n;

    req = (xAllocNamedColorReq *) data;

    cmap = req->cmap;
    nbytes = req->nbytes;
    if (client->swapped) {
	swapl(&cmap, n);
	swaps(&nbytes, n);
    }
    if (nbytes > MAX_COLORNAME_LENGTH)
	return REQ_NOCHANGE;

    FindNamedPixel(client, cmap, (char *) &req[1], nbytes, &pent);
    rgbe = FindColorName((char *) &req[1], nbytes, cmap);

    /* better get both if we got one... */
    if (pent && rgbe && LBXCanDelayReply(client)) {	/* found the value */

	/* always inc the pixel, so our refcounts match the server's */
	IncrementPixel(client, cmap, pent);
	/* must tell server to bump refcnt */
	SendIncrementPixel(client, cmap, pent->pixel);

	reply.type = X_Reply;
	reply.length = 0;
	reply.sequenceNumber = LBXSequenceNumber(client);

	reply.screenRed = pent->rep_red;
	reply.screenGreen = pent->rep_green;
	reply.screenBlue = pent->rep_blue;
	reply.exactRed = rgbe->xred;
	reply.exactGreen = rgbe->xgreen;
	reply.exactBlue = rgbe->xblue;

	reply.pixel = pent->pixel;

	if (client->swapped) {
	    SwapAllocNamedColorReply(&reply);
	}
	if (LBXCacheSafe(client))
	    WriteToClient(client, sizeof(xAllocNamedColorReply), &reply);
	else			/* store for later */
	    SaveReplyData(client, (xReply *) & reply, 0, NULL);

#ifdef LBX_STATS
	anc_good++;
#endif

	return REQ_REPLACE;	/* packet sent anyways */
    } else {
	nr = NewReply(client);
	if (nr) {
	    nr->request_info.lbxallocnamedcolor.cmap = cmap;
	    strncpy(nr->request_info.lbxallocnamedcolor.name, (char *) &req[1],
		    nbytes);
	    nr->request_info.lbxallocnamedcolor.namelen = nbytes;

	    nr->sequenceNumber = LBXSequenceNumber(client);
	    nr->request = X_AllocNamedColor;
	}
    }

#ifdef LBX_STATS
    anc_miss++;
#endif

    return REQ_NOCHANGE;
}

static Bool
alloc_named_color_reply(client, data)
    ClientPtr   client;
    char       *data;
{
    xAllocNamedColorReply *reply;
    ReplyStuffPtr nr;
    CARD16      xred,
                xgreen,
                xblue,
                vred,
                vgreen,
                vblue;
    CARD32      pixel;
    char        n;

    reply = (xAllocNamedColorReply *) data;

    nr = GetMatchingReply(client, reply->sequenceNumber);
    assert(nr);

    xred = reply->exactRed;
    xgreen = reply->exactGreen;
    xblue = reply->exactBlue;
    vred = reply->screenRed;
    vgreen = reply->screenGreen;
    vblue = reply->screenBlue;
    pixel = reply->pixel;

    if (client->swapped) {
	swapl(&pixel, n);
	swaps(&xred, n);
	swaps(&xgreen, n);
	swaps(&xblue, n);
	swaps(&vred, n);
	swaps(&vgreen, n);
	swaps(&vblue, n);
    }
    StoreNamedPixel(client,
		    nr->request_info.lbxallocnamedcolor.cmap,
		    nr->request_info.lbxallocnamedcolor.name,
		    nr->request_info.lbxallocnamedcolor.namelen,
		    (int) xred, (int) xgreen, (int) xblue,
		    (int) vred, (int) vgreen, (int) vblue,
		    pixel);
    return TRUE;
}

static int
free_colormap_req(client, data)
    ClientPtr   client;
    char       *data;
{
    xResourceReq *req;
    Colormap    cmap;
    char        n;

    req = (xResourceReq *) data;
    cmap = req->id;
    if (client->swapped) {
	swapl(&cmap, n);
    }
    FreeColormap(client, cmap);

    return REQ_NOCHANGE;
}

static int
create_colormap_req(client, data)
    ClientPtr   client;
    char       *data;
{
    xCreateColormapReq *req;
    Window      win;
    Colormap    cmap;
    VisualID    vis;
    char        n;

    req = (xCreateColormapReq *) data;

    /* AllocAll are read/write, so ignore */
    if (req->alloc) {
	win = req->window;
	cmap = req->mid;
	vis = req->visual;
	if (client->swapped) {
	    swapl(&win, n);
	    swapl(&cmap, n);
	    swapl(&vis, n);
	}
	CreateColormap(client, cmap, win, vis);
    }
    return REQ_NOCHANGE;
}

static int
copy_colormap_and_free_req(client, data)
    ClientPtr   client;
    char       *data;
{
    xCopyColormapAndFreeReq *req;
    Colormap    cmap,
                srcmap;
    char        n;

    req = (xCopyColormapAndFreeReq *) data;

    cmap = req->mid;
    srcmap = req->srcCmap;
    if (client->swapped) {
	swapl(&cmap, n);
	swapl(&srcmap, n);
    }
    CopyAndFreeColormap(client, cmap, srcmap);

    return REQ_NOCHANGE;
}


static int
free_colors_req(client, data)
    ClientPtr   client;
    char       *data;
{
    xFreeColorsReq *req;
    int         num;
    Pixel      *pixels;
    CARD32      mask;
    Colormap    cmap;
    Bool        freepix = FALSE;
    char        n;

    req = (xFreeColorsReq *) data;
    mask = req->planeMask;
    cmap = req->cmap;

    if (client->swapped) {
	swapl(&cmap, n);
	swapl(&mask, n);
    }
    num = ((req->length << 2) - sizeof(xFreeColorsReq)) >> 2;

    if (client->swapped) {
	pixels = (Pixel *) ALLOCATE_LOCAL(num * sizeof(Pixel));
	if (pixels) {
	    bcopy((char *) &req[1], (char *) pixels, (num * sizeof(Pixel)));
	    SwapLongs((CARD32 *) pixels, num);
	}
	freepix = TRUE;
    } else
	pixels = (Pixel *) &req[1];

    /* XXX handle mask */
    FreePixels(client, cmap, num, pixels);

    if (freepix)
	DEALLOCATE_LOCAL(pixels);
    return REQ_NOCHANGE;
}

/* ARGSUSED */
static int
get_mod_map_req(client, data)
    ClientPtr   client;
    char       *data;
{
    ReplyStuffPtr nr;

    nr = NewReply(client);
    if (nr) {
	nr->sequenceNumber = LBXSequenceNumber(client);
	nr->request = X_LbxGetModifierMapping;
	nr->lbx_req = TRUE;
	nr->extension = client->server->lbxReq;

	/*
	 * this expects a reply.  since we write the data here, we have to be
	 * sure the seq number is in sync first
	 */
	ForceSequenceUpdate(client);
	SendGetModifierMapping(client);

	return REQ_REPLACE;
    } else
	return REQ_NOCHANGE;
}

void
FinishModmapReply(client, seqnum, kpm, data)
    ClientPtr   client;
    int         seqnum;
    int         kpm;
    pointer     data;
{
    xGetModifierMappingReply reply;
    int         len = kpm << 3;

    reply.type = X_Reply;
    reply.numKeyPerModifier = kpm;
    reply.sequenceNumber = seqnum;
    reply.length = len >> 2;
    if (client->swapped) {
	SwapModmapReply(&reply);
    }
    WriteToClient(client, sizeof(xGetModifierMappingReply), &reply);
    WriteToClientPad(client, len, data);
}

static Bool
get_mod_map_reply(client, data)
    ClientPtr   client;
    char       *data;
{
    xLbxGetModifierMappingReply *rep;
    int         len;
    pointer     tag_data;
    ReplyStuffPtr nr;
    QueryTagRec qt;
    CARD32      tag;
    char        n;

    rep = (xLbxGetModifierMappingReply *) data;

    nr = GetMatchingReply(client, rep->sequenceNumber);
    assert(nr);

    tag = rep->tag;
    if (client->swapped) {
	swapl(&tag, n);
    }
    len = rep->keyspermod << 3;
    if (rep->tag) {
	if (rep->length) {	/* first time, set tag */

#ifdef LBX_STATS
	    getmodmap_full++;
#endif

	    tag_data = (pointer) &rep[1];
	    if (!TagStoreData(global_cache, tag, len,
			      LbxTagTypeModmap, tag_data)) {
		/* tell server we lost it */
		SendInvalidateTag(client, tag);
	    }
	} else {
	    tag_data = TagGetData(global_cache, tag);
	    if (!tag_data) {
		qt.tag = tag;
		qt.tagtype = LbxTagTypeModmap;
		qt.typedata.modmap.keyspermod = rep->keyspermod;
		/* lost data -- ask again for tag value */
		QueryTag(client, &qt);

		/* XXX what is the right way to stack Queries? */
		return TRUE;
	    }

#ifdef LBX_STATS
	    getmodmap_tag++;
	    tag_bytes_unsent += (rep->keyspermod << 3);
#endif
	}
    } else {

#ifdef LBX_STATS
	getmodmap_full++;
#endif

	/* server didn't send us a tag for some reason -- just pass on data */
	tag_data = (pointer) &rep[1];
    }

    FinishModmapReply(client, rep->sequenceNumber, (int) rep->keyspermod,
		      tag_data);

    return TRUE;
}

static int
get_key_map_req(client, data)
    ClientPtr   client;
    char       *data;
{
    ReplyStuffPtr nr;
    xGetKeyboardMappingReq *req;

    req = (xGetKeyboardMappingReq *) data;
    nr = NewReply(client);
    if (nr) {
	nr->sequenceNumber = LBXSequenceNumber(client);
	nr->request = X_LbxGetKeyboardMapping;
	nr->lbx_req = TRUE;
	nr->extension = client->server->lbxReq;
	nr->request_info.lbxgetkeymap.count = req->count;
	nr->request_info.lbxgetkeymap.first = req->firstKeyCode;

	/*
	 * this expects a reply.  since we write the data here, we have to be
	 * sure the seq number is in sync first
	 */
	ForceSequenceUpdate(client);
	SendGetKeyboardMapping(client);

	return REQ_REPLACE;
    } else
	return REQ_NOCHANGE;
}

/*
 * always ask for the whole map from server, and send requested subset to
 * client
 */
void
FinishKeymapReply(client, seqnum, kpk, first, count, data)
    ClientPtr   client;
    int         seqnum;
    int         kpk;
    int         first;
    int         count;
    char       *data;
{
    xGetKeyboardMappingReply reply;
    int         len = (kpk * count) << 2;
    char       *sdata;
    Bool        freedata = FALSE;

    reply.type = X_Reply;
    reply.keySymsPerKeyCode = kpk;
    reply.sequenceNumber = seqnum;
    reply.length = len >> 2;

    data += kpk * (first - LBXMinKeyCode(client));
    if (client->swapped) {
	SwapKeymapReply(&reply);
	/* have to copy data because we could be handed the tag storage */
	sdata = (char *) ALLOCATE_LOCAL(len);
	if (sdata) {
	    bcopy(data, sdata, len);
	    SwapLongs((CARD32 *) sdata, len / 4);
	    freedata = TRUE;
	}
    } else {
	sdata = data;
    }
    WriteToClient(client, sizeof(xGetKeyboardMappingReply), &reply);
    WriteToClientPad(client, len, sdata);
    if (freedata)
	DEALLOCATE_LOCAL(sdata);
}


static Bool
get_key_map_reply(client, data)
    ClientPtr   client;
    char       *data;
{
    xLbxGetKeyboardMappingReply *rep;
    int         len;
    pointer     tag_data;
    ReplyStuffPtr nr;
    QueryTagRec qt;
    CARD32      tag;
    char        n;

    rep = (xLbxGetKeyboardMappingReply *) data;

    nr = GetMatchingReply(client, rep->sequenceNumber);
    assert(nr);
    tag = rep->tag;
    if (client->swapped) {
	swapl(&tag, n);
    }
    if (tag) {
	if (rep->length) {	/* first time, set tag */

#ifdef LBX_STATS
	    getkeymap_full++;
#endif

	    tag_data = (pointer) &rep[1];
	    len = rep->keysperkeycode *
		(LBXMaxKeyCode(client) - LBXMinKeyCode(client) + 1) * 4;
	    /* data always swapped, because reswapped when written */
	    if (client->swapped) {
		SwapLongs((CARD32 *) tag_data, len / 4);
	    }
	    if (!TagStoreData(global_cache, tag, len,
			      LbxTagTypeKeymap, tag_data)) {
		/* tell server we lost it */
		SendInvalidateTag(client, tag);
	    }
	} else {
	    tag_data = TagGetData(global_cache, tag);
	    if (!tag_data) {
		/* lost data -- ask again for tag value */

		qt.tag = tag;
		qt.tagtype = LbxTagTypeKeymap;
		qt.typedata.keymap.keyspercode = rep->keysperkeycode;
		qt.typedata.keymap.count = nr->request_info.lbxgetkeymap.count;
		qt.typedata.keymap.first = nr->request_info.lbxgetkeymap.first;
		QueryTag(client, &qt);

		/* XXX what is the right way to stack Queries? */
		return TRUE;
	    }

#ifdef LBX_STATS
	    getkeymap_tag++;
	    tag_bytes_unsent += (rep->keysperkeycode *
		    (LBXMaxKeyCode(client) - LBXMinKeyCode(client) + 1) * 4);
#endif
	}
    } else {

#ifdef LBX_STATS
	getkeymap_full++;
#endif

	/* server didn't send us a tag for some reason -- just pass on data */
	tag_data = (pointer) &rep[1];
	/* data always swapped, because reswapped when written */
	if (client->swapped) {
	    len = rep->keysperkeycode *
		(LBXMaxKeyCode(client) - LBXMinKeyCode(client) + 1) * 4;
	    SwapLongs((CARD32 *) tag_data, len / 4);
	}
    }

    FinishKeymapReply(client, rep->sequenceNumber, (int) rep->keysperkeycode,
		      nr->request_info.lbxgetkeymap.first,
		      nr->request_info.lbxgetkeymap.count,
		      tag_data);

    return TRUE;
}

static int
queryfont_req(client, data)
    ClientPtr   client;
    char       *data;
{
    ReplyStuffPtr nr;
    xResourceReq *req;
    Font        fid;
    char        n;

    req = (xResourceReq *) data;
    fid = req->id;
    if (client->swapped) {
	swapl(&fid, n);
    }
    nr = NewReply(client);
    if (nr) {
	nr->sequenceNumber = LBXSequenceNumber(client);
	nr->request = X_LbxQueryFont;
	nr->lbx_req = TRUE;
	nr->extension = client->server->lbxReq;

	/*
	 * this expects a reply.  since we write the data here, we have to be
	 * sure the seq number is in sync first
	 */
	ForceSequenceUpdate(client);
	SendQueryFont(client, fid);

	return REQ_REPLACE;
    } else
	return REQ_NOCHANGE;
}

void
FinishQueryFontReply(client, seqnum, length, data)
    ClientPtr   client;
    int         seqnum;
    int         length;
    pointer     data;
{
    xQueryFontReply *reply;

    reply = (xQueryFontReply *) ALLOCATE_LOCAL(length);
    if (reply) {
	bcopy((char *) data, (char *) reply, length);
	/* patch up certain fields */
	reply->type = X_Reply;
	reply->sequenceNumber = seqnum;
	reply->length = (length - sizeof(xGenericReply)) >> 2;

	length -= sizeof(xQueryFontReply);
	if (client->swapped) {
	    SwapFont(reply, TRUE);
	}
	WriteToClient(client, sizeof(xQueryFontReply), reply);
	WriteToClient(client, length, reply + 1);
	DEALLOCATE_LOCAL(reply);
    }
}


static Bool
get_queryfont_reply(client, data)
    ClientPtr   client;
    char       *data;
{
    xLbxQueryFontReply *rep;
    int         len;
    pointer     tag_data;
    TagData     td;
    ReplyStuffPtr nr;
    QueryTagRec qt;
    CARD32      tag;
    char        n;

    rep = (xLbxQueryFontReply *) data;

    nr = GetMatchingReply(client, rep->sequenceNumber);
    assert(nr);

    tag = rep->tag;
    if (client->swapped) {
	swapl(&tag, n);
    }
    if (tag) {
	if (rep->length) {	/* first time, set tag */

#ifdef LBX_STATS
	    queryfont_full++;
#endif

	    tag_data = (pointer) &rep[1];
	    len = rep->length << 2;
	    if (client->swapped) {
		SwapFont((xQueryFontReply *) tag_data, FALSE);
	    }
	    if (!TagStoreData(global_cache, tag, len,
			      LbxTagTypeFont, tag_data)) {
		/* tell server we lost it */
		SendInvalidateTag(client, tag);
	    }
	} else {
	    td = TagGetTag(global_cache, tag);
	    if (!td) {
		/* lost data -- ask again for tag value */

		qt.tag = tag;
		qt.tagtype = LbxTagTypeFont;
		QueryTag(client, &qt);

		/* XXX what is the right way to stack Queries? */
		return TRUE;
	    }
	    len = td->size;
	    tag_data = td->tdata;

#ifdef LBX_STATS
	    queryfont_tag++;
	    tag_bytes_unsent += len;
#endif
	}
    } else {

#ifdef LBX_STATS
	queryfont_full++;
#endif

	/* server didn't send us a tag for some reason -- just pass on data */
	tag_data = (pointer) &rep[1];
	if (client->swapped) {
	    SwapFont((xQueryFontReply *) tag_data, FALSE);
	}
	len = rep->length << 2;
    }

    FinishQueryFontReply(client, rep->sequenceNumber, len, tag_data);

    return TRUE;
}

int
ProcLBXInternAtom(client)
    ClientPtr   client;
{
    int         yank;

    yank = intern_atom_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

int
ProcLBXGetAtomName(client)
    ClientPtr   client;
{
    int         yank;

    yank = get_atom_name_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

int
ProcLBXCreateColormap(client)
    ClientPtr   client;
{
    int         yank;

    yank = create_colormap_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

int
ProcLBXAllocColor(client)
    ClientPtr   client;
{
    int         yank;

    yank = alloc_color_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

int
ProcLBXAllocNamedColor(client)
    ClientPtr   client;
{
    int         yank;

    yank = alloc_named_color_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

int
ProcLBXFreeColormap(client)
    ClientPtr   client;
{
    int         yank;

    yank = free_colormap_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

int
ProcLBXCopyColormapAndFree(client)
    ClientPtr   client;
{
    int         yank;

    yank = copy_colormap_and_free_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

int
ProcLBXFreeColors(client)
    ClientPtr   client;
{
    int         yank;

    yank = free_colors_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

int
ProcLBXLookupColor(client)
    ClientPtr   client;
{
    int         yank;

    yank = lookup_color_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

int
ProcLBXGetModifierMapping(client)
    ClientPtr   client;
{
    int         yank;

    yank = get_mod_map_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

int
ProcLBXGetKeyboardMapping(client)
    ClientPtr   client;
{
    int         yank;

    yank = get_key_map_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

int
ProcLBXQueryFont(client)
    ClientPtr   client;
{
    int         yank;

    yank = queryfont_req(client, client->requestBuffer);
    return FinishLBXRequest(client, yank);
}

int
MakeLBXReply(client)
    ClientPtr   client;
{
    ReplyStuffPtr nr;
    int	gen;

    REQUEST(xReq);

    /* create the reply struct for requests we don't do anything with */
    if (gen = GeneratesReplies(stuff)) {
	nr = NewReply(client);
	if (nr) {
	    nr->sequenceNumber = LBXSequenceNumber(client);
	    nr->request = stuff->reqType;
	    nr->extension = (stuff->reqType > X_NoOperation) ? stuff->data : 0;
	    if (gen == REQ_TYPE_MAYBE)
	    	nr->guessed = TRUE;
	}
    }
    return FinishLBXRequest(client, REQ_PASSTHROUGH);
}

int
FinishLBXRequest(client, yank)
    ClientPtr   client;
    int         yank;
{
    REQUEST(xReq);
    char        n;

#define	yankable(y)	(((y) == REQ_YANK) || ((y) == REQ_REPLACE))

#ifdef PROTOCOL_SLOW
    LBXCacheSafe(client) = FALSE;
#endif

#ifdef PROTOCOL_FULL
    if (!yankable(yank) && (GeneratesEvents(stuff) ||
			    GeneratesReplies(stuff)
			    || GeneratesErrors(stuff)))
	LBXCacheSafe(client) = FALSE;
    else
	LBXCacheSafe(client) = TRUE;
#endif

#ifdef PROTOCOL_MOST
    if (!yankable(yank)
	    (GeneratesEvents(stuff) || GeneratesReplies(stuff)))
	LBXCacheSafe(client) = FALSE;
    else
	LBXCacheSafe(client) = TRUE;
#endif

#ifdef PROTOCOL_POOR
    LBXCacheSafe(client) = TRUE;
#endif

    /* never safe to short-circuit if requests are outstanding */
    if (NumReplies(client) > 0)
	LBXCacheSafe(client) = FALSE;

/* XXX is this correct for anything but PROTOCOL_POOR ??? */
    /*
     * if we can't write it instantly, and there *may* be a reply pending,
     * (unknown, because its an unrecognized extension request) we can't even
     * delay a short-circuit, cause we'll never know when its safe to send the
     * delayed reply
     */
    if (!LBXCacheSafe(client))	{
    	if (NumReplies(client) && NumGuessedReplies(client) == 0)
	    LBXCanDelayReply(client) = TRUE;
	else
	    LBXCanDelayReply(client) = FALSE;
    } else {	/* always true if cacheable, so we get a chance to write */
	LBXCanDelayReply(client) = TRUE;
    }
    	

    if (yank == REQ_YANK) {
	LBXSequenceLost(client)++;
	LBXYanked(client)++;
	LBXLastReply(client) = LBXSequenceNumber(client);
	DBG(DBG_CLIENT, (stderr, "short-circuited client %d req %d\n",
			 client->index, stuff->reqType));
    }
    /* make sure server's sequence number is accurate */
    if ((!yankable(yank) && LBXSequenceLost(client)) ||
	    (LBXSequenceLost(client) >= MAX_SEQUENCE_LOST)) {
	BumpSequence(client);
    }
    /* put request length in proxy byte order */
    if (client->swapped) {
	swaps(&stuff->length, n);
    }
    if (yank == REQ_NOCHANGE) {
	WriteReqToServer(client, stuff->length << 2, (char *) stuff);
    }
    return Success;
}

/*
 * need to rewrite error codes for requests we've replaced.
 *
 * QueryFont regularly hits this in normal operation
 */
static int
patchup_error(client, err, nr)
    ClientPtr   client;
    xError     *err;
    ReplyStuffPtr nr;
{
    QueryTagPtr qtp;
    int         retval = 1;
    CARD16      minor_code;
    char        n;

    minor_code = err->minorCode;
    if (client->swapped) {
	swaps(&minor_code, n);
    }
    switch (minor_code) {
    case X_LbxGetModifierMapping:
	minor_code = X_GetModifierMapping;
	break;
    case X_LbxGetKeyboardMapping:
	minor_code = X_GetKeyboardMapping;
	break;
    case X_LbxGetProperty:
	minor_code = X_GetProperty;
	break;
    case X_LbxQueryFont:
	minor_code = X_QueryFont;
	break;
    case X_LbxQueryTag:
	qtp = &(nr->request_info.lbxquerytag.info);
	switch (qtp->tagtype) {
	case LbxTagTypeModmap:
	    minor_code = X_GetModifierMapping;
	    break;
	case LbxTagTypeKeymap:
	    minor_code = X_GetKeyboardMapping;
	    break;
	case LbxTagTypeProperty:
	    minor_code = X_GetProperty;
	    break;
	case LbxTagTypeFont:
	    minor_code = X_QueryFont;
	    break;
	case LbxTagTypeConnInfo:
	    /*
	     * What do we do now?!?!?  For now, return -1 so that caller can
	     * shut down the client.
	     */
	    retval = -1;
	default:
	    assert(0);
	    break;
	}
	break;
    default:
	retval = 0;		/* error caused by some LBX req that shouldn't
				 * have an error, so eat it */
	break;
    }
    if (client->swapped) {
	swaps(&minor_code, n);
    }
    err->minorCode = minor_code;
    return retval;
}

static Bool
error_matches(client, nr, err)
    ClientPtr   client;
    ReplyStuffPtr nr;
    xError     *err;
{
    CARD16      mc;
    char        n;

    mc = err->minorCode;
    if (client->swapped) {
	swaps(&mc, n);
    }
    if (nr && (err->sequenceNumber == (nr->sequenceNumber & 0xffff))) {
	if (err->majorCode == client->server->lbxReq &&
		(mc == nr->request)) {
	    return TRUE;
	} else if (err->majorCode == nr->request) {
	    return TRUE;
	}
    }
    return FALSE;
}

/*
 * returns TRUE if data (possibly modified) is to be sent on to client,
 * FALSE if data is replaced
 */
/* ARGSUSED */
Bool
DoLBXReply(client, data, len)
    ClientPtr   client;
    char       *data;
    int         len;
{
    xGenericReply *reply;
    xError     *err;
    ReplyStuffPtr nr;
    Bool        remove_it = TRUE;
    Bool        ret = TRUE;
    extern Bool lbxUseTags;
    char        n;

#ifdef PROTOCOL_RAW
    return;
#endif

    reply = (xGenericReply *) data;

    if (client->awaitingSetup) {
	xConnSetupPrefix *prefix = (xConnSetupPrefix *) reply;

	AttendClient(client);
	if (prefix->success) {
	    if (lbxUseTags) {
		get_tagged_setup_reply(client, (char *) reply);
		return FALSE;
	    } else {
		if (client->swapped) {
		    SwapConnectionInfo((xConnSetup *) & prefix[1]);
		}
		GetConnectionInfo(client, (xConnSetup *) & prefix[1], NULL);
	    }
	}
	return TRUE;
    }
    if (client->swapped) {	/* swap sequence number */
	swaps(&reply->sequenceNumber, n);
    }

#ifdef DEBUG
    if (reply->sequenceNumber < LBXLastReply(client))
	fprintf(stderr, "replies out of whack for client %d\n", client->index);
#endif

    if (reply->sequenceNumber > LBXSequenceNumber(client))
	fprintf(stderr, "BOGUS DATA\n");

    if (reply->type != X_Reply) {	/* event or error */

#ifdef TRACE
	fprintf(stderr, "got event/error %d (%d %d)\n",
		(reply->type & 0x7f), reply->sequenceNumber, client->XRemId);
#endif

	/* clear out pending replies that resulted in errors */
	if (reply->type == X_Error) {
	    err = (xError *) reply;
	    nr = GetMatchingReply(client, reply->sequenceNumber);
	    if (error_matches(client, nr, err)) {
		if (err->majorCode == client->server->lbxReq) {
		    int         eret;

		    if ((eret = patchup_error(client, err, nr)) < 0) {
			CloseDownClient(client);
			return FALSE;
		    } else if (eret == 0) {
			/* error for proxy -- eat it */
			ret = FALSE;
		    }
		}
		/* handle extension error */
		HandleExtensionError(client, err);
		RemoveReply(client, nr);
	    }
	}
	/* KeymapNotify has no sequence # */
	if ((reply->sequenceNumber < LBXLastReply(client)) &&
		reply->type != KeymapNotify) {
	    /*
	     * the server's sequence number is out-of-date. fix the
	     * event/error so its correct
	     */
	    err = (xError *) reply;

#ifdef DEBUG
	    fprintf(stderr, "rewriting error/event sequence number from %d to %d\n",
		    err->sequenceNumber, client->sequenceNumber);
#endif

	    err->sequenceNumber = LBXLastReply(client);

	}
	if (reply->type == MotionNotify) {
	    AllowMotion(client, 1);
	}
	HandleExtensionEvent(client, reply);
	if (client->swapped) {	/* put seq & length back */
	    swaps(&reply->sequenceNumber, n);
	    swapl(&reply->length, n);
	}
	return ret;
    }
    nr = GetMatchingReply(client, reply->sequenceNumber);

    if (!nr)			/* what is this???? */
	return TRUE;

    /*
     * all unknown (extension) requests get a reply struct.  since we don't
     * know if there's really a reply for them or not, we can't clean them out
     * till we do get a reply.  at this point, we clear them out till we reach
     * either a safe reply struct, or an extension struct that has a matching
     * sequence number.
     */

#ifdef old
    while (!nr->lbx_req && nr->extension && NumReplies(client) > 1) {
	if ((nr->sequenceNumber & 0xffff) == reply->sequenceNumber)
	    break;
	RemoveReply(client, nr);
	nr = GetReply(client);
	assert(nr);
    }
#endif


#ifdef TRACE
    fprintf(stderr, "got reply supposedly for %d (%d %d)\n",
	    nr->request, reply->sequenceNumber, client->index);
#endif

    /* sequence number coming back should be the same as what's coming out */
    if ((nr->sequenceNumber & 0xffff) != reply->sequenceNumber)
	fprintf(stderr, "HELP -- reply for %d out-of-sync %d %d\n",
		nr->request,
		reply->sequenceNumber, nr->sequenceNumber);

#ifndef PROTOCOL_SLOW
    LBXCacheSafe(client) = TRUE;/* found matching reply, go for it */
#endif

    if (!nr->extension) {
	switch (nr->request) {
	case X_InternAtom:
	    remove_it = intern_atom_reply(client, (char *) reply);
	    break;
	case X_GetAtomName:
	    remove_it = get_atom_name_reply(client, (char *) reply);
	    break;
	case X_LookupColor:
	    remove_it = lookup_color_reply(client, (char *) reply);
	    break;
	case X_AllocColor:
	    remove_it = alloc_color_reply(client, (char *) reply);
	    break;
	case X_AllocNamedColor:
	    remove_it = alloc_named_color_reply(client, (char *) reply);
	    break;
	default:
	    break;
	}
    } else if (nr->lbx_req) {
	/* handle various extensions we know about */
	switch (nr->request) {
	case X_LbxGetModifierMapping:
	    remove_it = get_mod_map_reply(client, (char *) reply);
	    ret = FALSE;
	    break;
	case X_LbxGetKeyboardMapping:
	    remove_it = get_key_map_reply(client, (char *) reply);
	    ret = FALSE;
	    break;
	case X_LbxGetProperty:
	    remove_it = GetLbxGetPropertyReply(client, (char *) reply);
	    ret = FALSE;
	    break;
	case X_LbxChangeProperty:
	    remove_it = GetLbxChangePropertyReply(client, (char *) reply);
	    ret = FALSE;
	    break;
	case X_LbxQueryFont:
	    remove_it = get_queryfont_reply(client, (char *) reply);
	    ret = FALSE;
	    break;
	case X_LbxQueryTag:
	    remove_it = GetQueryTagReply(client, (char *) reply);
	    ret = FALSE;
	    break;
	case X_LbxGetImage:
	    remove_it = GetLbxImageReply(client, (char *) reply);
	    ret = FALSE;
	    break;
	case X_LbxQueryExtension:
	    remove_it = HandleLbxQueryExtensionReply(client, (char *) reply);
	    ret = FALSE;
	    break;
	default:
	    break;
	}
    } else {
	/* XXX handle any other extensions we may know about */
	remove_it = HandleExtensionReply(client, reply, nr);
    }
    if (remove_it)
	RemoveReply(client, nr);
    if (client->swapped) {	/* put seq & length back */
	swaps(&reply->sequenceNumber, n);
	swapl(&reply->length, n);
    }
    return ret;
}
